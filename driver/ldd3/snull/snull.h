
/*
 * snull.h -- definitions for the network module
 *
 * 
 */

#ifndef __SNULL_H__
#define __SNULL_H__


#define SNULL_RX_INTR 0x0001
#define SNULL_TX_INTR 0x0002
#define SNULL_TIMEOUT 5  

struct snull_priv {
    struct net_device_stats stats;
    int status;
    int rx_packetlen;
    u8 *rx_packetdata;
    int tx_packetlen;
    u8 *tx_packetdata;
    struct sk_buff *skb;
    spinlock_t lock;
    struct net_device *dev;
};


#endif




