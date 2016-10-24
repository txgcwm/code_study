#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kfifo.h>



void prt_kfifo_size(struct kfifo *fifo)
{
    printk(KERN_INFO "kfifo size : %u bytes\n", kfifo_size(fifo));
    printk(KERN_INFO "kfifo len : %u bytes\n", kfifo_len(fifo));
    printk(KERN_INFO "kfifo avail : %u bytes\n", kfifo_avail(fifo));
}

static int test_init(void)
{
    struct kfifo fifo;
    int ret;
    int i;
    int buf[4096];
    int nbytes;

    ret = kfifo_alloc(&fifo, 4096, GFP_KERNEL);
    if (ret) {
        return ret;
    }

    for (i = 0; i < 32; i++) {
        kfifo_in(&fifo, &i, sizeof(i));
    }

    prt_kfifo_size(&fifo);
    
    nbytes = kfifo_out_peek(&fifo, buf, 4096);
    
    printk(KERN_INFO "peek : %d bytes\n", nbytes);

    for (i = 0; i < nbytes/sizeof(int); i++) {
        printk(KERN_INFO "%d ", buf[i]);
    }

    printk(KERN_INFO "\n======= after peek =======\n");

    prt_kfifo_size(&fifo);

    nbytes = kfifo_out(&fifo, buf, sizeof(int));
    printk(KERN_INFO "out : %d bytes\n", nbytes);
    
    printk(KERN_INFO "\n======= after out =======\n");
    prt_kfifo_size(&fifo);
    
    kfifo_free(&fifo);
    
    return 0;
}

static void test_exit(void)
{
	printk(KERN_INFO "\n======= exit =======\n");
}




module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("txgcwm");



