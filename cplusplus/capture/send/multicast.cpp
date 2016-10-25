#include <sys/time.h>

extern "C"
{
#include "aes.h"
}
#include "libnet.h"


#define libnet_timersub(tvp, uvp, vvp)                                  \
        do {                                                            \
                (vvp)->tv_sec = (tvp)->tv_sec - (uvp)->tv_sec;          \
                (vvp)->tv_usec = (tvp)->tv_usec - (uvp)->tv_usec;       \
                if ((vvp)->tv_usec < 0) {                               \
                        (vvp)->tv_sec--;                                \
                        (vvp)->tv_usec += 1000000;                      \
                }                                                       \
        } while (0)


typedef struct {
    u_char 	emac[6];
    u_int32_t 	ip;
    u_int16_t 	port;
} ip_info_t;

int send_multicast_data(libnet_t *l, ip_info_t sour, ip_info_t dest, u_int8_t *payload, u_int16_t payload_s)
{
	int c = 0;
	int ret = -1;
	libnet_ptag_t t = 0;

	do {
		t = libnet_build_udp(sour.port, dest.port, LIBNET_UDP_H + payload_s, 0,
					payload, payload_s, l, t);         
		if (t == -1) {
		    fprintf(stderr, "Can't build UDP header: %s\n", libnet_geterror(l));
			break;
		}

		t = libnet_build_ipv4(LIBNET_IPV4_H + LIBNET_UDP_H + payload_s, 0, 242, 0,
				      64, IPPROTO_UDP, 0, sour.ip, dest.ip, NULL, 0, l, 0);
		if (t == -1) {
			fprintf(stderr, "Can't build IP header: %s\n", libnet_geterror(l));
			break;
		}

		t = libnet_build_ethernet(dest.emac, sour.emac, ETHERTYPE_IP,
					    NULL, 0, l, 0);
		if (t == -1) {
			fprintf(stderr, "Can't build ethernet header: %s\n", libnet_geterror(l));
			break;
		}

		c = libnet_write(l); 
		if (c == -1) {
		    fprintf(stderr, "write error: %s\n", libnet_geterror(l));
			break;
		} else {
		    //fprintf(stderr, "wrote %d byte UDP packet to port\n", c);
			ret = 0;
		}
	} while(0);

	libnet_clear_packet(l);

	return ret;
}

void init_ip_info(ip_info_t &sour, ip_info_t &dest, libnet_t *l, char *destip, char *sourip)
{
    u_char enet_dst[6] = {0x00, 0x10, 0x67, 0x00, 0xb1, 0x86};

    memcpy(dest.emac, enet_dst, 6);
    
    sour.port = 5555;
    dest.port = 7777;
    
    if ((dest.ip = libnet_name2addr4(l, destip, LIBNET_RESOLVE)) == -1) {
		fprintf(stderr, "Bad destination IP address:\n");                    
		return;
    }

    if ((sour.ip = libnet_name2addr4(l, sourip, LIBNET_RESOLVE)) == -1) {
		fprintf(stderr, "Bad source IP address:\n");
        return;
    }
    
    return;
}

void encrypt_transmit_data(const unsigned char *text, unsigned char *encrypted)
{
	AES_KEY key;
	
	const unsigned char master_key[16] = {
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
		0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
	};

	AES_set_encrypt_key(master_key, 128, &key); // 128 for 128-bit version AES
	AES_encrypt(text, encrypted, &key);

	return;
}

int main(int argc, char **argv)
{
    struct timeval r;
    struct timeval s;
    struct timeval e;
    libnet_t *l;
    struct libnet_stats ls;
    ip_info_t sour, dest;
    char errbuf[LIBNET_ERRBUF_SIZE];

    printf("libnet 1.1.2 packet shaping: UDP2[link]\n");

    l = libnet_init(LIBNET_LINK, NULL, errbuf);
    if (l == NULL) {
        fprintf(stderr, "libnet_init() failed: %s", errbuf);
        exit(EXIT_FAILURE);
    }
    
    init_ip_info(sour, dest, l, (char *)"239.0.0.1", (char *)"127.0.0.1");
    
    gettimeofday(&s, NULL);

	int len = 0, size = 0;
	char *text = "afafafafi5866afaf756af!@=1234567";
                 
	len = strlen(text);
	size = (len/16 + 1) * 16;

	unsigned char *encrypted = (unsigned char *)malloc(size);
	u_char *raw = (u_char *)malloc(size);

	memset(encrypted, 0x0, size);
	memset(raw, 0x30, size);

	for(int i=0; i<len; i++)
		raw[i] = text[i];

	for(int i=0; i<size; i++) {
		printf("%02x ", raw[i]);
		if( (i + 1) % 16 == 0 )   
           printf("\n"); 
	}

	printf("len: %d, raw: %d\n", len, size);

	encrypt_transmit_data((const unsigned char *)raw, encrypted);

	free(raw);

	for(int i=0; i<size; i++) {
		printf("%02x ", encrypted[i]);
		if( (i + 1) % 16 == 0 )   
           printf("\n"); 
	}

    while(1) {
		memset(sour.emac, 0xff, 6);
		sour.emac[0] &= ((size/5 + 1) | (1<<7));

		if(send_multicast_data(l, sour, dest, NULL, 0) < 0) {
			continue;
		}
	
		for(int i = 0; i < size; i += 5) {
			memset(sour.emac, 0x00, 6);
			sour.emac[0] |= i/5 + 1;
			memcpy(sour.emac + 1, encrypted + i, 5);

			if(send_multicast_data(l, sour, dest, NULL, 0) < 0) {
				continue;
			}
		}
    }

    free(encrypted);
	encrypted = NULL;

    gettimeofday(&e, NULL);

    libnet_timersub(&e, &s, &r);
    fprintf(stderr, "Total time spent in loop: %ld.%ld\n", r.tv_sec, r.tv_usec);

    libnet_stats(l, &ls);
    fprintf(stderr, "Packets sent:  %lld\n"
                    "Packet errors: %lld\n"
                    "Bytes written: %lld\n",
                    ls.packets_sent, ls.packet_errors, ls.bytes_written);

    libnet_destroy(l);

    return (EXIT_SUCCESS);
}



