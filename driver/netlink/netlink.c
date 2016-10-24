#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <net/sock.h>
#include <linux/netlink.h>

#define NETLINK_TEST 	17
#define MAX_PAYLOAD 	1024


static struct sock *g_generic_nlsk = NULL;

int netlink_send_msg(int pid)
{
    struct sk_buff *skb = NULL;
    struct nlmsghdr *nlh = NULL;

    skb = nlmsg_new(MAX_PAYLOAD, GFP_ATOMIC);
    if(skb == NULL) {
        printk(KERN_ERR"Failed to alloc skb\n");
        return 0;
    }

    printk(KERN_INFO"Kernel sending hello to client %d | portid %d\n", pid, 10);

    // put into skb
    nlh = nlmsg_put(skb, 0, 0, 0, MAX_PAYLOAD, 0);
    printk(KERN_INFO"In Sent Msg type|len|flags|pid|seq %d|%d|%d|%d|%d\n",
           nlh->nlmsg_type,
           nlh->nlmsg_len,
           nlh->nlmsg_flags,
           nlh->nlmsg_pid,
           nlh->nlmsg_seq);
 
    // below line is meaningless
    memcpy(NLMSG_DATA(nlh), "Hello Client", sizeof("Hello Client"));
    printk(KERN_INFO"sk is kernel %s\n", ((int *)(g_generic_nlsk+1))[3] & 0x1 ? "TRUE" : "FALSE");
    if(netlink_unicast(g_generic_nlsk, skb, pid, 0) < 0) {
        printk(KERN_ERR"Failed to unicast skb\n");
        return 0;
    }
 
    // free message
    // nlmsg_free(skb);

    return 1;
}

static void nl_data_ready(struct sk_buff *skb)
{  
	int pid;
    struct nlmsghdr *nlh = NULL;

    if(skb == NULL) {
        printk(KERN_INFO"skb is NULL\n");
        return;
    }

    nlh = (struct nlmsghdr *)skb->data;
    printk(KERN_INFO"%s:received message from %d|%d|%d|%d: %s\n", __FUNCTION__, nlh->nlmsg_pid, NETLINK_CB(skb).portid, nlmsg_total_size(0), skb->len, (char *)NLMSG_DATA(nlh));
     
    // print info of nlh
    printk(KERN_INFO"In Recved Msg type|len|flags|pid|seq %d|%d|%d|%d|%d\n",
           nlh->nlmsg_type,
           nlh->nlmsg_len,
           nlh->nlmsg_flags,
           nlh->nlmsg_pid,
           nlh->nlmsg_seq);
    pid = nlh->nlmsg_pid;
    
    netlink_send_msg(nlh->nlmsg_pid);

	return;
}

static int netlink_k_init(void)
{
	struct netlink_kernel_cfg cfg = {
		.input = nl_data_ready,
	};

	g_generic_nlsk = netlink_kernel_create(&init_net, NETLINK_TEST, &cfg);
	if (g_generic_nlsk == NULL) {
		printk("create NETLINK_GENERIC fail!\n");
		return -1;
	}

	printk("create NETLINK_GENERIC done\n");

	return 0;
}

static int __init netlink_init(void)
{
	printk("netlink_k init\n");
	netlink_k_init();

	return 0;
}

static void __exit netlink_exit(void)
{
	if (g_generic_nlsk) {
		netlink_kernel_release(g_generic_nlsk);
	}

	printk("netlink exit\n");

	return;
}



module_init(netlink_init);
module_exit(netlink_exit);

MODULE_LICENSE("GPL");



