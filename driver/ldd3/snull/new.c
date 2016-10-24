#include <linux/config.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/sched.h>
#include <linux/kernel.h> /* printk() */
#include <linux/slab.h> /* kmalloc() */
#include <linux/errno.h>   /* error codes */
#include <linux/types.h>   /* size_t */
#include <linux/interrupt.h> /* mark_bh */
#include <linux/in.h>
#include <linux/netdevice.h> /* struct device, and other headers */
#include <linux/etherdevice.h> /* eth_type_trans */
#include <linux/ip.h>       /* struct iphdr */
#include <linux/tcp.h>       /* struct tcphdr */
#include <linux/skbuff.h>
#include "snull.h"
#include <linux/in6.h>
#include <asm/checksum.h>

MODULE_AUTHOR("Alessandro Rubini, Jonathan Corbet");
MODULE_LICENSE("Dual BSD/GPL");

/*
* Transmitter lockup simulation, normally disabled.
*/
static int lockup = 0;
module_param(lockup, int, 0);
static int timeout = SNULL_TIMEOUT;
module_param(timeout, int, 0);
/*
* Do we run in NAPI mode?
*/
static int use_napi = 0;
module_param(use_napi, int, 0);
/*
* A structure representing an in-flight packet.
*/

struct snull_packet {
	struct snull_packet *next;
	struct net_device *dev;
	int datalen;
	u8 data[ETH_DATA_LEN];
};

int pool_size = 8;
module_param(pool_size, int, 0);

/*
* This structure is private to each device. It is used to pass
* packets in and out, so there is place for a packet
*/
struct snull_priv {
	struct net_device_stats stats;
	int status;
	struct snull_packet *ppool;
	struct snull_packet *rx_queue;   /* List of incoming packets */
	int rx_int_enabled;
	int tx_packetlen;
	u8 *tx_packetdata;
	struct sk_buff *skb;
	spinlock_t lock;
};

static void snull_tx_timeout(struct net_device *dev);
static void (*snull_interrupt)(int, void *, struct pt_regs *);

/*
* 设置设备的包缓冲池.
* 当需要使用NAPI，而非中断处理的时候，设备需要能够保存多个数据包的能力，这个保存所需的缓存，
* 或者在板卡上，或者在内核的DMA环中。
* 作者这里的演示程序，根据pool_size的大小，分配pool_size个大小为struct snull_packet的缓冲区，
* 这个缓冲池用链表组织，“私有数据”结构的ppool成员指针指向链表首部。
*/
void snull_setup_pool(struct net_device *dev)
{
	struct snull_priv *priv = netdev_priv(dev);
	int i;
	struct snull_packet *pkt;
	priv->ppool = NULL;

	for (i = 0; i < pool_size; i++) {
		pkt = kmalloc (sizeof (struct snull_packet), GFP_KERNEL);
		if (pkt == NULL) {
			printk (KERN_NOTICE "Ran out of memory allocating packet pool\n");
			return;
		}

		pkt->dev = dev;	
		pkt->next = priv->ppool;
		priv->ppool = pkt;
	}
}

/*因为snull_setup_pool分配了pool_size个struct snull_packet，所以，驱动退出时，需要释放内存*/
void snull_teardown_pool(struct net_device *dev)
{
	struct snull_priv *priv = netdev_priv(dev);
	struct snull_packet *pkt;

	while ((pkt = priv->ppool)) {
		priv->ppool = pkt->next;
		kfree (pkt);
		/* FIXME - in-flight packets ? */
	}
}

/*
* 获取设备要传输的第一个包，传输队列首部相应的移动到下一个数据包.
*/
struct snull_packet *snull_get_tx_buffer(struct net_device *dev)
{
	struct snull_priv *priv = netdev_priv(dev);
	unsigned long flags;
	struct snull_packet *pkt;

	spin_lock_irqsave(&priv->lock, flags);
	pkt = priv->ppool;
	priv->ppool = pkt->next;
	if (priv->ppool == NULL) {
		printk (KERN_INFO "Pool empty\n");
		netif_stop_queue(dev);
	}

	spin_unlock_irqrestore(&priv->lock, flags);

	return pkt;
}

/*将包缓存交还给缓存池*/
void snull_release_buffer(struct snull_packet *pkt)
{
        unsigned long flags;
        struct snull_priv *priv = netdev_priv(pkt->dev);     

        spin_lock_irqsave(&priv->lock, flags);
        pkt->next = priv->ppool;
        priv->ppool = pkt;
        spin_unlock_irqrestore(&priv->lock, flags);
        if (netif_queue_stopped(pkt->dev) && pkt->next == NULL)
                netif_wake_queue(pkt->dev);
}

/*将要传输的包加入到设备dev的传输队列首部，当然，这只是一个演示，这样一来，就变成先进先出了*/
void snull_enqueue_buf(struct net_device *dev, struct snull_packet *pkt)
{
        unsigned long flags;
        struct snull_priv *priv = netdev_priv(dev);

        spin_lock_irqsave(&priv->lock, flags);
        pkt->next = priv->rx_queue; /* FIXME - misorders packets */
        priv->rx_queue = pkt;
        spin_unlock_irqrestore(&priv->lock, flags);
}

/*取得传输队列中的第一个数据包*/
struct snull_packet *snull_dequeue_buf(struct net_device *dev)
{
        struct snull_priv *priv = netdev_priv(dev);
        struct snull_packet *pkt;
        unsigned long flags;

        spin_lock_irqsave(&priv->lock, flags);
        pkt = priv->rx_queue;
        if (pkt != NULL)
                priv->rx_queue = pkt->next;
        spin_unlock_irqrestore(&priv->lock, flags);

        return pkt;
}

/*
* 打开/关闭接收中断.
*/
static void snull_rx_ints(struct net_device *dev, int enable)
{
        struct snull_priv *priv = netdev_priv(dev);
        priv->rx_int_enabled = enable;
}

/*
* 设备打开函数，是驱动最重要的函数之一，它应该注册所有的系统资源（I/O端口，IRQ、DMA等等），
* 并对设备执行其他所需的设置。
* 因为这个例子中，并没有真正的物理设备，所以，它最重要的工作就是启动传输队列。
*/
int snull_open(struct net_device *dev)
{
        /* request_region(), request_irq(), .... (like fops->open) */
        /*
         * Assign the hardware address of the board: use "\0SNULx", where
         * x is 0 or 1. The first byte is '\0' to avoid being a multicast
         * address (the first byte of multicast addrs is odd).
         */
        memcpy(dev->dev_addr, "\0SNUL0", ETH_ALEN);
        if (dev == snull_devs[1])
                dev->dev_addr[ETH_ALEN-1]++; /* \0SNUL1 */
        netif_start_queue(dev);

        return 0;
}

/*设备停止函数，这里的工作就是停止传输队列*/
int snull_release(struct net_device *dev)
{
    /* release ports, irq and such -- like fops->close */
        netif_stop_queue(dev); /* can't transmit any more */

        return 0;
}

/*
* 当用户调用ioctl时类型为SIOCSIFMAP时，如使用ifconfig，系统会调用驱动程序的set_config 方法。
* 用户会传递一个ifmap结构包含需要设置的I/O地址、中断等参数。
*/
int snull_config(struct net_device *dev, struct ifmap *map)
{
        if (dev->flags & IFF_UP) /* 不能设置一个正在运行状态的设备 */
                return -EBUSY;

        /* 这个例子中，不允许改变 I/O 地址*/
        if (map->base_addr != dev->base_addr) {
                printk(KERN_WARNING "snull: Can't change I/O address\n");
                return -EOPNOTSUPP;
        }
        /* 允许改变 IRQ */
        if (map->irq != dev->irq) {
                dev->irq = map->irq;
                /* request_irq() is delayed to open-time */
        }
        /* ignore other fields */

        return 0;
}

/*
* 接收数据包函数
* 它被“接收中断”调用，重组数据包，并调用函数netif_rx进一步处理。
* 我们从“硬件”中收到的包，是用struct snull_packet来描述的，但是内核中描述一个包，是使用
* struct sk_buff（简称skb），所以，这里要完成一个把硬件接收的包拷贝至内核缓存skb的一个
* 组包过程(PS：不知在接收之前直接分配一个skb，省去这一步，会如何提高性能，没有研究过，见笑了^o^)。
*/
void snull_rx(struct net_device *dev, struct snull_packet *pkt)
{
        struct sk_buff *skb;
        struct snull_priv *priv = netdev_priv(dev);

        /*
         * 分配skb缓存
         */
        skb = dev_alloc_skb(pkt->datalen + 2);
        if (!skb) {                        /*分配失败*/
                if (printk_ratelimit())
                        printk(KERN_NOTICE "snull rx: low on mem - packet dropped\n");
                priv->stats.rx_dropped++;
                goto out;
        }

        /*
         * skb_reserver用来增加skb的date和tail，因为以太网头部为14字节长，再补上两个字节就刚好16字节边界
         * 对齐，所以大多数以太网设备都会在数据包之前保留2个字节。
         */
        skb_reserve(skb, 2); /* align IP on 16B boundary */
        memcpy(skb_put(skb, pkt->datalen), pkt->data, pkt->datalen);
        skb->dev = dev;                        /*skb与接收设备就关联起来了，它在网络栈中会被广泛使用，没道理不知道数据是谁接收来的吧*/
        skb->protocol = eth_type_trans(skb, dev);        /*获取上层协议类型，这样，上层处理函数才知道如何进一步处理*/
        skb->ip_summed = CHECKSUM_UNNECESSARY;                 /* 设置较验标志：不进行任何校验，作者的驱动的收发都在内存中进行，是没有必要进行校验*/

        /*累加计数器*/
        priv->stats.rx_packets++;
        priv->stats.rx_bytes += pkt->datalen;

        /*
         * 把数据包交给上层。netif_rx会逐步调用netif_rx_schedule -->__netif_rx_schedule，
         * __netif_rx_schedule函数会调用__raise_softirq_irqoff(NET_RX_SOFTIRQ);触发网络接收数据包的软中断函数net_rx_action。
         * 软中断是Linux内核完成中断推后处理工作的一种机制，请参考《Linux内核设计与实现》第二版。
         * 唯一需要提及的是，这个软中断函数net_rx_action是在网络系统初始化的时候(linux/net/core/dev.c)：注册的
         * open_softirq(NET_RX_SOFTIRQ, net_rx_action, NULL);
         */

        netif_rx(skb);
out:
        return;
}

/*
* NAPI 的poll轮询函数.
*/
static int snull_poll(struct net_device *dev, int *budget)
{
        /*
         * dev->quota是当前CPU能够从所有接口中接收数据包的最大数目，budget是在
         * 初始化阶段分配给接口的weight值，轮询函数必须接受二者之间的最小值。表示
         * 轮询函数本次要处理的数据包个数。
         */

        int npackets = 0, quota = min(dev->quota, *budget);
        struct sk_buff *skb;
        struct snull_priv *priv = netdev_priv(dev);
        struct snull_packet *pkt;

            /*这个循环次数由要处理的数据包个数，并且，以处理完接收队列为上限*/
        while (npackets < quota && priv->rx_queue) {
                /*从队列中取出数据包*/
                pkt = snull_dequeue_buf(dev);

                /*接下来的处理，和传统中断事实上是一样的*/
                skb = dev_alloc_skb(pkt->datalen + 2);
                if (! skb) {
                        if (printk_ratelimit())
                                printk(KERN_NOTICE "snull: packet dropped\n");
                        priv->stats.rx_dropped++;
                        snull_release_buffer(pkt);
                        continue;
                }

                skb_reserve(skb, 2); /* align IP on 16B boundary */
                memcpy(skb_put(skb, pkt->datalen), pkt->data, pkt->datalen);
                skb->dev = dev;
                skb->protocol = eth_type_trans(skb, dev);
                skb->ip_summed = CHECKSUM_UNNECESSARY; /* don't check it */

                /*需要调用netif_receive_skb而不是net_rx将包交给上层协议栈*/
                netif_receive_skb(skb);

                /*累加计数器 */
                npackets++;
                priv->stats.rx_packets++;
                priv->stats.rx_bytes += pkt->datalen;
                snull_release_buffer(pkt);
        }

        /* If we processed all packets, we're done; tell the kernel and reenable ints */
        *budget -= npackets;
        dev->quota -= npackets;

        if (! priv->rx_queue) {
                netif_rx_complete(dev);
                snull_rx_ints(dev, 1);
                return 0;
        }

        /* We couldn't process everything. */
        return 1;
}

/*
* 设备的中断函数，当需要发/收数据，出现错误，连接状态变化等，它会被触发
* 对于典型的网络设备，一般会在open函数中注册中断函数，这样，当网络设备产生中断时，如接收到数据包时，
* 中断函数将会被调用。不过在这个例子中，因为没有真正的物理设备，所以，不存在注册中断，也就不存在触
* 发，对于接收和发送，它都是在自己设计的函数的特定位置被调用。
* 这个中断函数设计得很简单，就是取得设备的状态，判断是“接收”还是“发送”的中断，以调用相应的处理函数。
* 而对于，“是哪个设备产生的中断”这个问题，则由调用它的函数通过第二个参数的赋值来决定。
*/
static void snull_regular_interrupt(int irq, void *dev_id, struct pt_regs *regs)
{
        int statusword;
        struct snull_priv *priv;
        struct snull_packet *pkt = NULL;

        /*
         * 通常，需要检查 "device" 指针以确保这个中断是发送给自己的。
         * 然后为 "struct device *dev" 赋
         */
        struct net_device *dev = (struct net_device *)dev_id;
        /* paranoid */
        if (!dev)
                return;

        /* 锁住设备 */
        priv = netdev_priv(dev);
        spin_lock(&priv->lock);
        /* 取得设备状态指字，对于真实设备，使用I/O指令，比如：int txsr = inb(TX_STATUS); */
        statusword = priv->status;
        priv->status = 0;
        if (statusword & SNULL_RX_INTR) {                /*如果是接收数据包的中断*/
                /* send it to snull_rx for handling */
                pkt = priv->rx_queue;
                if (pkt) {
                        priv->rx_queue = pkt->next;
                        snull_rx(dev, pkt);
                }
        }

        if (statusword & SNULL_TX_INTR) {                /*如果是发送数据包的中断*/
                /* a transmission is over: free the skb */
                priv->stats.tx_packets++;
                priv->stats.tx_bytes += priv->tx_packetlen;
                dev_kfree_skb(priv->skb);
        }

        /* 释放锁 */
        spin_unlock(&priv->lock);

        /*释放缓冲区*/
        if (pkt) 
			snull_release_buffer(pkt); /* Do this outside the lock! */

        return;
}

/*
* A NAPI interrupt handler.
* 在设备初始化的时候，poll指向指向了snull_poll函数，所以，NAPI中断处理函数很简单，
* 当“接收中断”到达的时候，它就屏蔽此中断，然后netif_rx_schedule函数接收，接收函数
* 会在未来某一时刻调用注册的snull_poll函数实现轮询，当然，对于“传输中断”，处理方法
* 同传统中断处理并无二致。
*/
static void snull_napi_interrupt(int irq, void *dev_id, struct pt_regs *regs)
{
        int statusword;
        struct snull_priv *priv;

        /*
         * As usual, check the "device" pointer for shared handlers.
         * Then assign "struct device *dev"
         */
        struct net_device *dev = (struct net_device *)dev_id;
        /* ... and check with hw if it's really ours */
        /* paranoid */
        if (!dev)
                return;

        /* Lock the device */
        priv = netdev_priv(dev);
        spin_lock(&priv->lock);
        /* retrieve statusword: real netdevices use I/O instructions */
        statusword = priv->status;
        priv->status = 0;

        /*
         * 唯一的区别就在这里，它先屏蔽掉接收中断，然后调用netif_rx_schedule，而不是netif_rx
         * 重点还是在于poll函数的设计。
         */
        if (statusword & SNULL_RX_INTR) {
                snull_rx_ints(dev, 0); /* Disable further interrupts */
                netif_rx_schedule(dev);
        }

        if (statusword & SNULL_TX_INTR) {
                /* a transmission is over: free the skb */
                priv->stats.tx_packets++;
                priv->stats.tx_bytes += priv->tx_packetlen;
                dev_kfree_skb(priv->skb);
        }

        /* Unlock the device and we are done */
        spin_unlock(&priv->lock);

        return;
}

/*
* Transmit a packet (low level interface)
*/
static void snull_hw_tx(char *buf, int len, struct net_device *dev)
{
        /*
         * This function deals with hw details. This interface loops
         * back the packet to the other snull interface (if any).
         * In other words, this function implements the snull behaviour,
         * while all other procedures are rather device-independent
         */

        struct iphdr *ih;
        struct net_device *dest;
        struct snull_priv *priv;
        u32 *saddr, *daddr;
        struct snull_packet *tx_buffer;

        /* I am paranoid. Ain't I? */
        if (len < sizeof(struct ethhdr) + sizeof(struct iphdr)) {
                printk("snull: Hmm... packet too short (%i octets)\n",len);
                return;
        }

        if (0) { /* enable this conditional to look at the data */
                int i;
                PDEBUG("len is %i\n" KERN_DEBUG "data:",len);
                for (i=14 ; i<len; i++)
                        printk(" %02x",buf[i]&0xff);
                printk("\n");
        }

        /*
         * 取得来源IP和目的IP地址
         */
        ih = (struct iphdr *)(buf+sizeof(struct ethhdr));
        saddr = &ih->saddr;
        daddr = &ih->daddr;

        /*
         * 这里做了三个调换，以实现欺骗：来源地址第三octet 0<->1，目的地址第三octet 0<->1，设备snX编辑0<->1，这样做的理由是：
         * sn0(发):192.168.0.88 --> 192.168.0.99        做了调换后，就变成：
         * sn1(收):192.168.1.88 --> 192.168.1.99        因为sn1的地址就是192.168.1.99，所以，它收到这个包后，会回应：
         * sn1(发):192.168.1.99 --> 192.168.1.88        ，同样地，做了这样的调换后，就变成：
         * sn0(收):192.168.0.99 --> 192.168.0.88        这样，sn0就会收到这个包，实现了ping的请求与应答，^o^
         */

        ((u8 *)saddr)[2] ^= 1; /* change the third octet (class C) */
        ((u8 *)daddr)[2] ^= 1;

        /*重新计算较验和*/
        ih->check = 0;         /* and rebuild the checksum (ip needs it) */
        ih->check = ip_fast_csum((unsigned char *)ih,ih->ihl);

        /*输出调试信息*/
        if (dev == snull_devs[0])
                PDEBUGG("%08x:%05i --> %08x:%05i\n",
                                ntohl(ih->saddr),ntohs(((struct tcphdr *)(ih+1))->source),
                                ntohl(ih->daddr),ntohs(((struct tcphdr *)(ih+1))->dest));
        else
                PDEBUGG("%08x:%05i <-- %08x:%05i\n",
                                ntohl(ih->daddr),ntohs(((struct tcphdr *)(ih+1))->dest),
                                ntohl(ih->saddr),ntohs(((struct tcphdr *)(ih+1))->source));

        /*调换设备编号，即dest指向接收设备，原因如前所述*/
        dest = snull_devs[dev == snull_devs[0] ? 1 : 0];
      
        /*将发送的数据添加到接收设备的接收队列中*/
        priv = netdev_priv(dest);
        tx_buffer = snull_get_tx_buffer(dev);
        tx_buffer->datalen = len;
        memcpy(tx_buffer->data, buf, len);
        snull_enqueue_buf(dest, tx_buffer);

        /*
         * 如果设备接收标志打开，就调用中断函数把数据包发送给目标设备——即触发目的设备的接收中断，这样
         * 中断程序就会自接收设备的接收队列中接收数据包，并交给上层网络栈处理
        */
        if (priv->rx_int_enabled) {
                priv->status |= SNULL_RX_INTR;
                snull_interrupt(0, dest, NULL);
        }

        /*发送完成后，触发“发送完成”中断*/
        priv = netdev_priv(dev);
        priv->tx_packetlen = len;
        priv->tx_packetdata = buf;
        priv->status |= SNULL_TX_INTR;

        /*
         * 如果insmod驱动的时候，指定了模拟硬件锁的lockup=n，则在会传输n个数据包后，模拟一次硬件锁住的情况，
         * 这是通过调用netif_stop_queue函数来停止传输队列，标记“设备不能再传输数据包”实现的，它将在传输的超
         * 时函数中，调用netif_wake_queue函数来重新启动传输队例，同时超时函数中会再次调用“接收中断”，这样
         * stats.tx_packets累加，又可以重新传输新的数据包了(参接收中断和超时处理函数的实现)。
         */

        if (lockup && ((priv->stats.tx_packets + 1) % lockup) == 0) {
                /* Simulate a dropped transmit interrupt */
                netif_stop_queue(dev);                        /*停止数据包的传输*/
                PDEBUG("Simulate lockup at %ld, txp %ld\n", jiffies,(unsigned long) priv->stats.tx_packets);
        }
        else
        /*发送完成后，触发中断，中断函数发现发送完成，就累加计数器，释放skb缓存*/
        snull_interrupt(0, dev, NULL);      

        /*
         * 看到这里，我们可以看到，这个发送函数其实并没有把数据包通过I/O指令发送给硬件，而仅仅是做了一个地址/设备的调换，
         * 并把数据包加入到接收设备的队例当中。
         */              
}

/*
* 数据包传输函数，Linux网络堆栈，在发送数据包时，会调用驱动程序的hard_start_transmit函数，
* 在设备初始化的时候，这个函数指针指向了snull_tx。
*/
int snull_tx(struct sk_buff *skb, struct net_device *dev)
{
        int len;
        char *data, shortpkt[ETH_ZLEN];
        struct snull_priv *priv = netdev_priv(dev);
    
        data = skb->data;
        len = skb->len;

        if (len < ETH_ZLEN) {                        /*处理短帧的情况，如果小于以太帧最小长度，不足位全部补0*/
                memset(shortpkt, 0, ETH_ZLEN);
                memcpy(shortpkt, skb->data, skb->len);
                len = ETH_ZLEN;
                data = shortpkt;
        }

        dev->trans_start = jiffies; /* 保存时间戳 */

        /*
         * 因为“发送”完成后，需要释放skb，所以，先要保存它 ,释放都是在网卡发送完成，产生中断，而中断函数收
         * 到网卡的发送完成的中断信号后释放
         */
		/*
         * 让硬件把数据包发送出去，对于物理设备，就是一个读网卡寄存器的过程，不过，这里，只是一些
         * 为了实现演示功能的虚假的欺骗函数，比如操作源/目的IP，然后调用接收函数（所以，接收时不用调用中断）
         */

        snull_hw_tx(data, len, dev);

        return 0; /* Our simple device can not fail */
}

/*
* 传输超时处理函数
* 比如在传输数据时，由于缓冲已满，需要关闭传输队列，但是驱动程序是不能丢弃数据包，它将在“超时”的时候触发
* 超时处理函数，这个函数将发送一个“传输中断”，以填补丢失的中断，并重新启动传输队例子
*/
void snull_tx_timeout (struct net_device *dev)
{
        struct snull_priv *priv = netdev_priv(dev);
        PDEBUG("Transmit timeout at %ld, latency %ld\n", jiffies,jiffies - dev->trans_start);

        /* Simulate a transmission interrupt to get things moving */
        priv->status = SNULL_TX_INTR;
        snull_interrupt(0, dev, NULL);
        priv->stats.tx_errors++;
        netif_wake_queue(dev);
        return;
}

/*
* Ioctl 命令
*/
int snull_ioctl(struct net_device *dev, struct ifreq *rq, int cmd)
{
        PDEBUG("ioctl\n");
        return 0;
}

/*
* 获取设备的状态
*/
struct net_device_stats *snull_stats(struct net_device *dev)
{
        struct snull_priv *priv = netdev_priv(dev);
        return &priv->stats;
}

/*
* 有些网络有硬件地址(比如Ethernet)，并且在发送硬件帧时需要知道目的硬件 地址会进行ARP请求/应答，以完成MAC地址解析，
* 需要做arp请求的设备在发送之前会调用驱动程序的rebuild_header函数。需要做arp的的设备在发送之前会调用驱动程序的
* rebuild_header方 法。调用的主要参数包括指向硬件帧头的指针，协议层地址。如果驱动程序能够解 析硬件地址，就返回1，
* 如果不能，返回0。
* 当然，作者实现的演示设备中，不支持这个过程。
*/

int snull_rebuild_header(struct sk_buff *skb)
{
        struct ethhdr *eth = (struct ethhdr *) skb->data;
        struct net_device *dev = skb->dev;

        memcpy(eth->h_source, dev->dev_addr, dev->addr_len);
        memcpy(eth->h_dest, dev->dev_addr, dev->addr_len);
        eth->h_dest[ETH_ALEN-1]   ^= 0x01;   /* dest is us xor 1 */
        return 0;
}

/*
* 为上层协议创建一个二层的以太网首部。
* 事实上，如果一开始调用alloc_etherdev分配以太设备，它会调用ether_setup进行初始化，初始化函数会设置：
*        dev->hard_header        = eth_header;
*        dev->rebuild_header         = eth_rebuild_header;
* 驱动开发人员并不需要自己来实现这个函数，作者这样做，只是为了展示细节。
*/

int snull_header(struct sk_buff *skb, struct net_device *dev,
                unsigned short type, void *daddr, void *saddr,
                unsigned int len)
{

        /*获取以太头指针*/
        struct ethhdr *eth = (struct ethhdr *)skb_push(skb,ETH_HLEN);

        eth->h_proto = htons(type);                /*填写协议*/
     
        /*填写来源/目的MAC地址，如果地址为空，则用设备自己的地址代替之*/
        memcpy(eth->h_source, saddr ? saddr : dev->dev_addr, dev->addr_len);
        memcpy(eth->h_dest,   daddr ? daddr : dev->dev_addr, dev->addr_len);     

        /*
         * 将第一个octet设为0，主要是为了可以在不支持组播链路，如ppp链路上运行
         * PS：作者这样做，仅仅是演示在PC机上的实现，事实上，直接使用ETH_ALEN-1是
         * 不适合“大头”机器的。
         */
        eth->h_dest[ETH_ALEN-1]   ^= 0x01;   /* dest is us xor 1 */
        return (dev->hard_header_len);
}

/*
* 改变设备MTU值.
*/

int snull_change_mtu(struct net_device *dev, int new_mtu)
{
        unsigned long flags;
        struct snull_priv *priv = netdev_priv(dev);
        spinlock_t *lock = &priv->lock;

        /* check ranges */
        if ((new_mtu < 68) || (new_mtu > 1500))

                return -EINVAL;

        /*
         * Do anything you need, and the accept the value
         */
        spin_lock_irqsave(lock, flags);
        dev->mtu = new_mtu;
        spin_unlock_irqrestore(lock, flags);

        return 0; /* success */
}

/*
* 设备初始化函数，它必须在 register_netdev 函数被调用之前调用
*/
void snull_init(struct net_device *dev)
{
        /*设备的“私有”结构，保存一些设备一些“私有数据”*/
        struct snull_priv *priv;

            /*
         * 初始化以太网设备的一些共用的成员
         */
        ether_setup(dev); /* assign some of the fields */

        /*设置设备的许多成员函数指针*/
        dev->open            = snull_open;
        dev->stop            = snull_release;
        dev->set_config      = snull_config;
        dev->hard_start_xmit = snull_tx;
        dev->do_ioctl        = snull_ioctl;
        dev->get_stats       = snull_stats;
        dev->change_mtu      = snull_change_mtu;
        dev->rebuild_header = snull_rebuild_header;
        dev->hard_header     = snull_header;
        dev->tx_timeout      = snull_tx_timeout;
        dev->watchdog_timeo = timeout;    

        /*如果使用NAPI，设置pool函数*/

        if (use_napi) {
                dev->poll        = snull_poll;
                dev->weight      = 2;                /*weight是接口在资源紧张时，在接口上能承受多大流量的权重*/
        }

        /* keep the default flags, just add NOARP */
        dev->flags           |= IFF_NOARP;
        dev->features        |= NETIF_F_NO_CSUM;
        dev->hard_header_cache = NULL;      /* Disable caching */
        /*
         * 取得私有数据区，并初始化它.
         */

        priv = netdev_priv(dev);
        memset(priv, 0, sizeof(struct snull_priv));
        spin_lock_init(&priv->lock);
        snull_rx_ints(dev, 1);                /* 打开接收中断标志 */
        snull_setup_pool(dev);                /*设置使用NAPI时的接收缓冲池*/
}

/*
* The devices
*/
struct net_device *snull_devs[2];
/*
* Finally, the module stuff
*/
void snull_cleanup(void)
{
        int i;

    for (i = 0; i < 2; i++) 
	{
                if (snull_devs[i]) {
                        unregister_netdev(snull_devs[i]);
                        snull_teardown_pool(snull_devs[i]);
                        free_netdev(snull_devs[i]);
                }
        }

        return;
}

/*模块初始化，初始化的只有一个工作：分配一个设备结构并注册它*/
int snull_init_module(void)
{
        int result, i, ret = -ENOMEM;

        /*中断函数指针，因是否使用NAPI而指向不同的中断函数*/
        snull_interrupt = use_napi ? snull_napi_interrupt : snull_regular_interrupt;

        /*
         * 分配两个设备，网络设备都是用struct net_device来描述，alloc_netdev分配设备，第三个参数是
         * 对struct net_device结构成员进行初始化的函数，对于以太网来说，可以把alloc_netdev/snull_init
         * 两个函数变为一个，alloc_etherdev，它会自动调用以太网的初始化函数ether_setup，因为以太网的初
         * 始化函数工作都是近乎一样的 */

        snull_devs[0] = alloc_netdev(sizeof(struct snull_priv), "sn%d",snull_init);
        snull_devs[1] = alloc_netdev(sizeof(struct snull_priv), "sn%d",snull_init);

        /*分配失败*/
        if (snull_devs[0] == NULL || snull_devs[1] == NULL)
                goto out;

        ret = -ENODEV;

        /*向内核注册网络设备，这样，设备就可以被使用了*/
        for (i = 0; i < 2; i++)
                if ((result = register_netdev(snull_devs[i])))
                	printk("snull: error %i registering device \"%s\"\n",result, snull_devs[i]->name);
                else
                	ret = 0;

   out:

        if (ret)
        	snull_cleanup();
        return ret;
}

module_init(snull_init_module);
module_exit(snull_cleanup);
