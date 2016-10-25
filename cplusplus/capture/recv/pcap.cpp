#include <netinet/if_ether.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <time.h>
#include <errno.h>
#include <string.h>

extern "C"
{
#include "aes.h"
}

//链路层数据包格式
typedef struct {
    u_char DestMac[6];
    u_char SrcMac[6];
    u_char Etype[2];
} ETHHEADER;

typedef struct {
	bool flag;
	int blk_num;
	int cur_num;
	u_char *data;
	int len;
} MultiData;

MultiData mult;

void decrypt_transmit_data(const u_char *encrypted, u_char *decrypted)
{
	AES_KEY key;

	const unsigned char master_key[16] = {
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
		0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
	};
	
	AES_set_decrypt_key(master_key, 128, &key);
	AES_decrypt(encrypted, decrypted, &key);

	return;
}

void parse_multicast_data(const u_char *srcmac)
{
	if(!mult.flag) {
		if((srcmac[0] & 0xf0) && srcmac[1] == 0xff && srcmac[2] == 0xff 
			&& srcmac[3] == 0xff && srcmac[4] == 0xff && srcmac[5] == 0xff) {
			mult.blk_num = srcmac[0] & (~0xf0);
			mult.len = ((mult.blk_num * 5) / 16) * 16;
			mult.data = (u_char *)malloc(mult.len);
			mult.flag = true;
		}

		return;
	}

	if(srcmac[0] & 0xf0)
		return;

	int cur = srcmac[0];
	
	if(cur == mult.cur_num + 1) {
		if(mult.cur_num < mult.blk_num - 1) {
			memcpy(mult.data + mult.cur_num * 5, srcmac + 1, 5);
		} else {
			int le = 5 - (mult.blk_num * 5 - mult.len);
			for(int i = 0; i < le; i++) {
				mult.data[mult.cur_num * 5 + i] = srcmac[1 + i];
			}
		}

		mult.cur_num = cur;

		if(mult.cur_num == mult.blk_num) {
			for(int i=0; i<mult.len; i++) {
				printf("%02x ", mult.data[i]);
				if( (i + 1) % 16 == 0 ) {
					printf("\n"); 
				}           
			}

			u_char *decrypted = (u_char *)malloc(((mult.blk_num * 5) / 16) * 16);
			decrypt_transmit_data(mult.data, decrypted);
			printf("decrypt: %s\n", decrypted);
			free(decrypted);
			free(mult.data);
			exit(0);
		}
	}

	return;
}

void pcap_handle(u_char* user,const struct pcap_pkthdr* header,const u_char* pkt_data)
{
    ETHHEADER *eth_header = (ETHHEADER*)pkt_data;
    printf("---------------Begin Analysis-----------------\n");
    printf("----------------------------------------------\n");
    printf("Packet length: %d\n", header->len);

    for(int i = 0; i < (int)header->len; ++i) {  
        printf(" %02x", pkt_data[i]);  
        if( (i + 1) % 16 == 0 )   
            printf("\n");  
    }  
    printf("\n\n");

    if(header->len >= 14 && pkt_data[0] == 0x00 && pkt_data[1] == 0x10 && pkt_data[2] == 0x67 
		&& pkt_data[3] == 0x00 && pkt_data[4] == 0xb1 && pkt_data[5] == 0x86) {      
        printf("Source MAC : %02X-%02X-%02X-%02X-%02X-%02X==>",eth_header->SrcMac[0],eth_header->SrcMac[1],eth_header->SrcMac[2],eth_header->SrcMac[3],eth_header->SrcMac[4],eth_header->SrcMac[5]);
        printf("Dest   MAC : %02X-%02X-%02X-%02X-%02X-%02X\n",eth_header->DestMac[0],eth_header->DestMac[1],eth_header->DestMac[2],eth_header->DestMac[3],eth_header->DestMac[4],eth_header->DestMac[5]);

		parse_multicast_data(pkt_data + 6);
    }

	return;
}

int main(int argc, char **argv)
{
    char *device = "eth0";
    char errbuf[1024];
    pcap_t *phandle;
    bpf_u_int32 ipaddress,ipmask;
    struct bpf_program fcode;
    int datalink;
    
    if((device = pcap_lookupdev(errbuf)) == NULL){
        perror(errbuf);
        return 1;
    }

    printf("device: %s\n", device);
    
    phandle = pcap_open_live(device, 200, 0, 500, errbuf);
    if(phandle == NULL){
        perror(errbuf);
        return 1;
    }
    
    if(pcap_lookupnet(device, &ipaddress, &ipmask, errbuf) == -1){
        perror(errbuf);
        return 1;
    }
  
    if((datalink = pcap_datalink(phandle)) == -1){
        fprintf(stderr,"pcap_datalink: %s\n", pcap_geterr(phandle));
        return 1;
    }
    
    printf("datalink= %d\n", datalink);

    pcap_loop(phandle, -1, pcap_handle, NULL);
    
    return 0;
}



