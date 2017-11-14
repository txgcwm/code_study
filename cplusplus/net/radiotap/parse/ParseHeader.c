#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <endian.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

#include "radiotap_iter.h"


// 根据wireshark抓包抽取的radiotap头部数据
char radiotap_buf[][18] = {
                {0x00, 0x00, 0x12, 0x00, 0x2e, 0x48,
                 0x00, 0x00, 0x00, 0x02, 0x85, 0x09,
                 0xc0, 0x00, 0xc9, 0x00, 0x00, 0x00},
                {0x00, 0x00, 0x12, 0x00, 0x2e, 0x48,
                 0x00, 0x00, 0x00, 0x02, 0x85, 0x09,
                 0xa0, 0x00, 0xa8, 0x00, 0x00, 0x00}};

#define IEEE80211_CHAN_A (IEEE80211_CHAN_5GHZ | IEEE80211_CHAN_OFDM)
#define IEEE80211_CHAN_G (IEEE80211_CHAN_2GHZ | IEEE80211_CHAN_OFDM)

static void print_radiotap_namespace(struct ieee80211_radiotap_iterator *iter)
{
    char signal = 0;
    uint32_t phy_freq = 0;

	switch (iter->this_arg_index) {
	case IEEE80211_RADIOTAP_TSFT:
		printf("\tTSFT: %llu\n", le64toh(*(unsigned long long *)iter->this_arg));
		break;

	case IEEE80211_RADIOTAP_FLAGS:
		printf("\tflags: %02x\n", *iter->this_arg);
		break;

	case IEEE80211_RADIOTAP_RATE:
		printf("\trate: %.2f Mbit/s\n", (double)*iter->this_arg/2);
		break;

	case IEEE80211_RADIOTAP_CHANNEL:
        phy_freq = le16toh(*(uint16_t*)iter->this_arg); // 信道
        iter->this_arg = iter->this_arg + 2; // 通道信息如2G、5G，等
        int x = le16toh(*(uint16_t*)iter->this_arg);
        printf("\tfreq: %d type: ", phy_freq);

        if ((x & IEEE80211_CHAN_A) == IEEE80211_CHAN_A) {
            printf("A\n");
        } else if ((x & IEEE80211_CHAN_G) == IEEE80211_CHAN_G) {
            printf("G\n");
        } else if ((x & IEEE80211_CHAN_2GHZ) == IEEE80211_CHAN_2GHZ) {
            printf("B\n");
        }
        break;

	case IEEE80211_RADIOTAP_DBM_ANTSIGNAL:
        signal = *(signed char*)iter->this_arg;
        printf("\tsignal: %d dBm\n", signal);
        break;

	case IEEE80211_RADIOTAP_RX_FLAGS:
		printf("\tRX flags: %#.4x\n", le16toh(*(uint16_t *)iter->this_arg));
		break;

    case IEEE80211_RADIOTAP_ANTENNA:
        printf("\tantenna: %x\n", *iter->this_arg);
        break;

	case IEEE80211_RADIOTAP_RTS_RETRIES:
	case IEEE80211_RADIOTAP_DATA_RETRIES:
    case IEEE80211_RADIOTAP_FHSS:
	case IEEE80211_RADIOTAP_DBM_ANTNOISE:
	case IEEE80211_RADIOTAP_LOCK_QUALITY:
	case IEEE80211_RADIOTAP_TX_ATTENUATION:
	case IEEE80211_RADIOTAP_DB_TX_ATTENUATION:
	case IEEE80211_RADIOTAP_DBM_TX_POWER:
	case IEEE80211_RADIOTAP_DB_ANTSIGNAL:
	case IEEE80211_RADIOTAP_DB_ANTNOISE:
	case IEEE80211_RADIOTAP_TX_FLAGS:
		break;

	default:
		printf("\tBOGUS DATA\n");
		break;
	}
}

static int ParseBuffer()
{
    struct ieee80211_radiotap_iterator iter;
    int err;
    int i, j;

    for (i = 0; i < sizeof(radiotap_buf)/sizeof(radiotap_buf[0]); i++) {
        printf("parsing [%d]\n", i);

        err = ieee80211_radiotap_iterator_init(&iter, (struct ieee80211_radiotap_header *)radiotap_buf[i],
                                                sizeof(radiotap_buf[i]), NULL);
        if (err) {
            printf("not valid radiotap...\n");
            return -1;
        }

        j = 0;

        /**
         * 遍历时，this_arg_index表示当前索引(如IEEE80211_RADIOTAP_TSFT等)，
         * this_arg表示当前索引的值，this_arg_size表示值的大小。只有flag为true时才会进一步解析。
         */
        while (!(err = ieee80211_radiotap_iterator_next(&iter))) {
            printf("next[%d]: index: %d size: %d\n",
                    j, iter.this_arg_index, iter.this_arg_size);
            if (iter.is_radiotap_ns) { // 表示是radiotap的命名空间
                print_radiotap_namespace(&iter);
            }

            j++;
        }

        printf("==================================\n");
    }

    return 0;
}

static int ParseFile(const char* filename)
{
	int fd = -1;
	char buffer[1024] = {0};

	fd = open(filename, O_RDONLY);
	if(fd < 0) {
		printf("can not open file(%s)\n", filename);
		return -1;
	}

	while((read(fd, buffer, 1024)) > 0) {
		
	}
	
	close(fd);
	fd = -1;
	
    return 0;
}

int main(int argc, char** argv)
{
    int res = 0;
    int action = 0;
    char file[256] = {0};

    while((res = getopt(argc, argv, "?a:f:h")) != -1) {
        switch(res) {
        case 'a':
            action = atoi(optarg);
            break;

        case 'f':
            memcpy(file, optarg, strlen(optarg));
            break;

        case 'h':
        default:
            break;
        }
    }

    if(action == 0) {
        ParseBuffer();
    } else if (action == 1) {
        ParseFile(file);
    }

    return 0;
}


// https://github.com/radiotap/radiotap-library