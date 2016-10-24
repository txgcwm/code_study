#include <linux/init.h>
#include <linux/module.h>
#include <linux/seq_file.h>
#include <linux/proc_fs.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <asm/uaccess.h>



#define MSC_PROC_NAME "testproc"


static struct proc_dir_entry *entry = NULL;


/*
 * 调用程序对read的返回值解释如下：
 * 1、如果返回值等于传递给read系统调用的count参数，则说明所请求的字节数传输成功完成了。
 * 2、如果返回值是正的，但是比count小，则说明只有部分数据成功传送。这种情况因为设备的不同可能有许多原因。
 *    大部分情况下，程序会重新读数据。例如，如果用fread函数读数据，这个库函数就会不断调用系统调用，直至
 *    所请求的数据传输完毕为止。
 * 3、如果返回值为0，则表示已经到达了文件尾。
 * 4、负值意味着发生了错误，该值指明了发生什么错误，错误码在<linux/errno.h>中定义
*/
ssize_t proc_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
	return 0;
}

static int proc_open(struct inode *inode, struct file *file)
{
	return 0;
}

static int proc_release(struct inode *inode, struct file *file)
{
	return 0;
}

const struct file_operations proc_fops = {
	.owner 			= THIS_MODULE,
	.open           = proc_open,
	.read           = proc_read,
	// .write          = proc_write,
	.release        = proc_release,
};

static int proc_init(void)
{
	printk(KERN_INFO "proc init\n");

	if(entry == NULL) {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,36)     
		entry = proc_create_data(MSC_PROC_NAME, S_IRUSR|S_IWUSR, NULL, &proc_fops, NULL);
#else  
		entry = create_proc_entry(MSC_PROC_NAME, S_IRUGO, NULL);  
		if (!entry) {
			return -ENOMEM;
		}

		entry->proc_fops = &proc_fops;
#endif   /* LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,36) */
	}
	
	return 0;
}

static void proc_exit(void)
{
	printk(KERN_INFO "proc exit\n");

	if(entry != NULL) {
		remove_proc_entry(MSC_PROC_NAME, NULL);
		entry = NULL;
	}

	return;
}



module_init(proc_init);
module_exit(proc_exit);

MODULE_AUTHOR("txgcwm");
MODULE_VERSION("proctest_v1.0");
MODULE_LICENSE("GPL");


