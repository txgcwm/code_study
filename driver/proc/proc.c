#include <linux/init.h>
#include <linux/module.h>
#include <linux/seq_file.h>
#include <linux/proc_fs.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <asm/uaccess.h>



#define MSC_PROC_NAME "testproc"


static struct proc_dir_entry *entry = NULL;





static int msc_proc_show(struct seq_file *seq, void *offset)
{
	printk("msc_proc_show\n");
		
	return 0;
}

static ssize_t proc_write(struct file *file, const char __user *buf,
								size_t count, loff_t *ppos)
{
	int err = 0;
	char data[128];

	if (count > sizeof(data))
		count = sizeof(data);

	if (copy_from_user(data, buf, count)) {
		err = -EFAULT;
		goto out;
	}
	data[count - 1] = 0;    /* Make string */

	err = count;

out:
	return err;
}

static int proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, msc_proc_show, NULL);
}

static int proc_release(struct inode *inode, struct file *file)
{
	return single_release(inode, file);
}

const struct file_operations proc_fops = {
	.open           = proc_open,
	.read           = seq_read,
	.write          = proc_write,
	.llseek         = seq_lseek,
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

MODULE_LICENSE("GPL");


