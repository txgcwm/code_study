#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>



static int hello_init(void)
{
	struct file *fp = NULL;
    mm_segment_t fs;
	loff_t pos;

    fp = filp_open("/tmp/test.txt", O_RDWR | O_CREAT, 0644);
    if (IS_ERR(fp)) {
		printk("create file error\n");
		return -1;
	}

	fs = get_fs();
    set_fs(KERNEL_DS);
	pos = 0;
	vfs_write(fp, "hello world", strlen("hello world"), &pos);

	filp_close(fp, NULL);
	set_fs(fs);

	printk(KERN_ALERT "hello world!\n");

    return 0;
}

static void hello_exit(void)
{
    printk(KERN_ALERT "goodbye!\n");

	return;
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
