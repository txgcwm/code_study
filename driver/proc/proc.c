#include <linux/init.h>
#include <linux/module.h>
#include <linux/seq_file.h>
#include <linux/proc_fs.h>
#include <linux/kfifo.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <asm/uaccess.h>



#define PROC_NAME 		"testproc"
#define FIFO_BUFFER_SIZE	4096

struct proc_info {
	struct semaphore sem;     /* mutual exclusion semaphore */
	struct kfifo fifo;
	struct proc_dir_entry *pentry;
	struct task_struct *thread;
};

static struct proc_info g_pinfo;


/***************************************************************************************
 *
 * 调用程序对read的返回值解释如下：
 *
 * 1、如果返回值等于传递给read系统调用的count参数，则说明所请求的字节数传输成功完成了。
 * 2、如果返回值是正的，但是比count小，则说明只有部分数据成功传送。这种情况因为设备的不同可能有许多原因。
 *    大部分情况下，程序会重新读数据。例如，如果用fread函数读数据，这个库函数就会不断调用系统调用，直至
 *    所请求的数据传输完毕为止。
 * 3、如果返回值为0，则表示已经到达了文件尾。
 * 4、负值意味着发生了错误，该值指明了发生什么错误，错误码在<linux/errno.h>中定义
 *
 ***************************************************************************************/
ssize_t proc_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
	ssize_t retval = -1;
	char text[1200] = {0};
	struct proc_info *pinfo = filp->private_data;

	if (down_interruptible(&pinfo->sem)) {
		return -ERESTARTSYS;
	}

	count = kfifo_out(&pinfo->fifo, text, sizeof(text));
	if(count <= 0) {
		goto out;
	}

	if(copy_to_user(buf, text, count)) {
		retval = -EFAULT;
		goto out;
	}

	retval = count;

out:
	up(&pinfo->sem);

	return retval;
}

static int testkthread(void* argv)  
{
	struct proc_info *pinfo = (struct proc_info *)argv;
	char *msg[] = {"1: afafaa", "2: 697977", "3: []vafa", "4: afafiurl", "5: wo,cxzfg"};
	int i = 0, loop = 5;

    while(!kthread_should_stop()) {
    	for(i = 0; i < loop; i++) {
    		kfifo_in(&pinfo->fifo, msg[i], strlen(msg[i]));

    		if(i == (loop -1)) {
    			i = 0;
    		}

    		__set_current_state(TASK_INTERRUPTIBLE);
			schedule_timeout(10); 
    	}    
    }

    return 0;  
}  

static int proc_open(struct inode *inode, struct file *filp)
{
	int ret = 0;

	printk(KERN_INFO "proc open\n");

	ret = kfifo_alloc(&g_pinfo.fifo, FIFO_BUFFER_SIZE, GFP_KERNEL);
    if(ret) {
    	printk(KERN_ERR "kfifo alloc error!\n");
        return -1;
    }

	filp->private_data = &g_pinfo;

	g_pinfo.thread = kthread_run(testkthread, &g_pinfo, "testthread");

	return 0;
}

static int proc_release(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "proc release\n");

	if(g_pinfo.thread != NULL) {
		kthread_stop(g_pinfo.thread);
	}

	kfifo_free(&g_pinfo.fifo);

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

	if(g_pinfo.pentry == NULL) {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,36)     
		g_pinfo.pentry = proc_create_data(PROC_NAME, S_IRUSR|S_IWUSR, NULL, &proc_fops, NULL);
#else  
		g_pinfo.pentry = create_proc_entry(PROC_NAME, S_IRUGO, NULL);  
		if (!g_pinfo.pentry) {
			return -ENOMEM;
		}

		g_pinfo.pentry->proc_fops = &proc_fops;
#endif   /* LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,36) */

		sema_init(&g_pinfo.sem, 1);
	}
	
	return 0;
}

static void proc_exit(void)
{
	printk(KERN_INFO "proc exit\n");

	if(g_pinfo.pentry != NULL) {
		remove_proc_entry(PROC_NAME, NULL);
		g_pinfo.pentry = NULL;
	}

	return;
}



module_init(proc_init);
module_exit(proc_exit);

MODULE_AUTHOR("txgcwm");
MODULE_VERSION("proctest_v1.0");
MODULE_LICENSE("GPL");


