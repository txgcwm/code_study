/* 
 * name:scull.c
 * function:scull character device driver
 * time:2012-2-19
 *
 * author:txgcwm
 * mail:txgcwm@163.com
 * reference:Linux设备驱动程序(第三版)
 */

#ifndef __KERNEL__
#  define __KERNEL__
#endif
#ifndef MODULE
#  define MODULE
#endif

#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/kernel.h>   /* printk() */
#include <linux/fs.h>       /* everything... */
#include <linux/errno.h>    /* error codes */
#include <linux/types.h>    /* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h>    /* O_ACCMODE */
#include <linux/seq_file.h>
#include <linux/moduleparam.h>
#include <asm/system.h>     /* cli(), *_flags */
#include <asm/uaccess.h>


#include "scull.h"          /* local definitions */


int scull_major =   SCULL_MAJOR;
int scull_minor =   SCULL_MINOR;
int scull_nr_devs = SCULL_NR_DEVS;
int scull_quantum = SCULL_QUANTUM;
int scull_qset =    SCULL_QSET;

module_param(scull_major, int, S_IRUGO); 
module_param(scull_nr_devs, int, S_IRUGO);
module_param(scull_quantum, int, S_IRUGO);
module_param(scull_qset, int, S_IRUGO);

struct scull_dev *scull_devices;



int scull_trim(struct scull_dev *dev)
{
	struct scull_qset *next, *dptr;
	int qset = dev->qset; 
	int i;

	for (dptr = dev->data; dptr; dptr = next) 
	{
		if (dptr->data) 
		{
			for (i = 0; i < qset; i++)
				kfree(dptr->data[i]);
			kfree(dptr->data);
			dptr->data = NULL;
		}
		next = dptr->next;
		kfree(dptr);
	}
	dev->size = 0;
	dev->quantum = scull_quantum;
	dev->qset = scull_qset;
	dev->data = NULL;

	return 0;
}

#ifdef SCULL_DEBUG 
int scull_read_procmem(char *buf, char **start, off_t offset, int count, int *eof, void *data)
{
	int i, j, len = 0;
	int limit = count - 80;

	for (i = 0; i < scull_nr_devs && len <= limit; i++) 
	{
		struct scull_dev *d = &scull_devices[i];
		struct scull_qset *qs = d->data;

		if (down_interruptible(&d->sem))
			return -ERESTARTSYS;

		len += sprintf(buf+len,"\nDevice %i: qset %i, q %i, sz %li\n", i, d->qset, d->quantum, d->size);
		for (; qs && len <= limit; qs = qs->next) 
		{
			len += sprintf(buf + len, "  item at %p, qset at %p\n", qs, qs->data);
			if (qs->data && !qs->next)
				for (j = 0; j < d->qset; j++) 
				{
					if (qs->data[j])
						len += sprintf(buf + len, "    % 4i: %8p\n", j, qs->data[j]);
				}
		}
		up(&scull_devices[i].sem);
	}
	*eof = 1;

	return len;
}

static void *scull_seq_start(struct seq_file *s, loff_t *pos)
{
	if (*pos >= scull_nr_devs)
		return NULL; 

	return scull_devices + *pos;
}

static void *scull_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
	(*pos)++;

	if (*pos >= scull_nr_devs)
		return NULL;

	return scull_devices + *pos;
}

static void scull_seq_stop(struct seq_file *s, void *v)
{
	
}

static int scull_seq_show(struct seq_file *s, void *v)
{
	struct scull_dev *dev = (struct scull_dev *) v;
	struct scull_qset *d;
	int i;

	if (down_interruptible(&dev->sem))
		return -ERESTARTSYS;
	seq_printf(s, "\nDevice %i: qset %i, q %i, sz %li\n",(int) (dev - scull_devices), dev->qset, dev->quantum, dev->size);
	for (d = dev->data; d; d = d->next) 
	{
		seq_printf(s, "  item at %p, qset at %p\n", d, d->data);
		if (d->data && !d->next)
			for (i = 0; i < dev->qset; i++) 
			{
				if (d->data[i])
					seq_printf(s, "    % 4i: %8p\n", i, d->data[i]);
			}
	}

	up(&dev->sem);

	return 0;
}

static struct seq_operations scull_seq_ops = {
	.start = scull_seq_start,
	.next  = scull_seq_next,
	.stop  = scull_seq_stop,
	.show  = scull_seq_show
};

static int scull_proc_open(struct inode *inode, struct file *file)
{
	return seq_open(file, &scull_seq_ops);
}

static struct file_operations scull_proc_ops = {
	.owner   = THIS_MODULE,
	.open    = scull_proc_open,
	.read    = seq_read,
	.llseek  = seq_lseek,
	.release = seq_release
};
	
static void scull_create_proc(void)
{
	struct proc_dir_entry *entry;
	
	create_proc_read_entry("scullmem", 0, NULL, scull_read_procmem, NULL);
	entry = create_proc_entry("scullseq", 0, NULL);

	if (entry)
		entry->proc_fops = &scull_proc_ops;

	return;
}

static void scull_remove_proc(void)
{
	remove_proc_entry("scullmem", NULL);
	remove_proc_entry("scullseq", NULL);
}
#endif /* SCULL_DEBUG */

/*
open方法提供给驱动程序以初始化的能力。它应完成如下工作：
	1、检查设备特定的错误；
	2、如果设备是首次打开，则对其进行初始化；
	3、如有必要，更新f_op指针；
	4、分配并填写置于filp->private_data里的数据结构。
*/
int scull_open(struct inode *inode, struct file *filp)
{
	struct scull_dev *dev; 

	dev = container_of(inode->i_cdev, struct scull_dev, cdev);
	filp->private_data = dev; 
	
	if ( (filp->f_flags & O_ACCMODE) == O_WRONLY) 
	{
		if (down_interruptible(&dev->sem))
			return -ERESTARTSYS;
		scull_trim(dev); 
		up(&dev->sem);
	}

	return 0;       
}

/*
release主要的工作是：
	1、释放由open分配的、保存在filp->private_data中的所有内容；
	2、在最后一次关闭操作时关闭设备。
*/
int scull_release(struct inode *inode, struct file *filp)
{ 
    return 0;
}

struct scull_qset *scull_follow(struct scull_dev *dev, int n)
{
	struct scull_qset *qs = dev->data;

	if (!qs) 
	{
		qs = dev->data = kmalloc(sizeof(struct scull_qset), GFP_KERNEL);
		if (qs == NULL)
			return NULL; 
		memset(qs, 0, sizeof(struct scull_qset));
	}

	while (n--) 
	{
		if (!qs->next) 
		{
			qs->next = kmalloc(sizeof(struct scull_qset), GFP_KERNEL);
			if (qs->next == NULL)
				return NULL; 
			memset(qs->next, 0, sizeof(struct scull_qset));
		}
		qs = qs->next;
		continue;
	}

	return qs;
}

/*
 * 调用程序对read的返回值解释如下：
 * 1、如果返回值等于传递给read系统调用的count参数，则说明所请求的字节数传输成功完成了。
 * 2、如果返回值是正的，但是比count小，则说明只有部分数据成功传送。这种情况因为设备的不同可能有许多原因。
 *    大部分情况下，程序会重新读数据。例如，如果用fread函数读数据，这个库函数就会不断调用系统调用，直至
 *    所请求的数据传输完毕为止。
 * 3、如果返回值为0，则表示已经到达了文件尾。
 * 4、负值意味着发生了错误，该值指明了发生什么错误，错误码在<linux/errno.h>中定义
*/
ssize_t scull_read(struct file *filp, char *buf, size_t count,loff_t *f_pos)
{
	struct scull_dev *dev = filp->private_data; 
	struct scull_qset *dptr;	
	int quantum = dev->quantum, qset = dev->qset;
	int itemsize = quantum * qset; 
	int item, s_pos, q_pos, rest;
	ssize_t retval = 0;

	if (down_interruptible(&dev->sem))
		return -ERESTARTSYS;
	if (*f_pos >= dev->size) 
		goto out;
	if (*f_pos + count > dev->size) 
		count = dev->size - *f_pos;	

	item = (long)*f_pos / itemsize; 
	rest = (long)*f_pos % itemsize; 
	s_pos = rest / quantum;	
	q_pos = rest % quantum; 

	dptr = scull_follow(dev, item); 

	if (dptr == NULL || !dptr->data || ! dptr->data[s_pos])
		goto out; 

	if (count > quantum - q_pos)
		count = quantum - q_pos;

	if (copy_to_user(buf, dptr->data[s_pos] + q_pos, count)) 
	{
		retval = -EFAULT;
		goto out;
	}
	*f_pos += count; 
	retval = count;

  out:
	up(&dev->sem);
	return retval;
}

/*
 * 调用程序对write的返回值解释如下：
 * 1、如果返回值等于count，则完成了所请求数目的字节传送。
 * 2、如果返回值是正的，但小于count，则只传送了部分数据。程序很可能再次试图写入余下的数据。
 * 3、如果值为0，意味着什么也没有写入。这个结果不是错误，而且也没有理由返回一个错误码。
 * 4、负值意味着发生了错误。
*/
ssize_t scull_write(struct file *filp, const char *buf, size_t count,loff_t *f_pos)
{
	struct scull_dev *dev = filp->private_data;
	struct scull_qset *dptr;
	int quantum = dev->quantum, qset = dev->qset;
	int itemsize = quantum * qset;
	int item, s_pos, q_pos, rest;
	ssize_t retval = -ENOMEM; 

	if (down_interruptible(&dev->sem))
		return -ERESTARTSYS;

	item = (long)*f_pos / itemsize;
	rest = (long)*f_pos % itemsize;
	s_pos = rest / quantum; 
	q_pos = rest % quantum;

	dptr = scull_follow(dev, item);
	if (dptr == NULL)
		goto out;
	if (!dptr->data) 
	{
		dptr->data = kmalloc(qset * sizeof(char *), GFP_KERNEL);
		if (!dptr->data)
			goto out;
		memset(dptr->data, 0, qset * sizeof(char *));
	}
	if (!dptr->data[s_pos]) 
	{
		dptr->data[s_pos] = kmalloc(quantum, GFP_KERNEL);
		if (!dptr->data[s_pos])
			goto out;
	}

	if (count > quantum - q_pos)
		count = quantum - q_pos;

	if (copy_from_user(dptr->data[s_pos]+q_pos, buf, count)) 
	{
		retval = -EFAULT;
		goto out;
	}
	*f_pos += count;
	retval = count;

	if (dev->size < *f_pos)
		dev->size = *f_pos;

  out:
	up(&dev->sem);
	return retval;
}

/*
 * 除了读取和写入设备之外，大部分驱动程序还需要另外一种能力，即通过设备驱动程序执行各种类型的硬件控制。
 */
int scull_ioctl(struct inode *inode, struct file *filp,unsigned int cmd, unsigned long arg)
{
	int err = 0, tmp;
	int retval = 0;

	if (_IOC_TYPE(cmd) != SCULL_IOC_MAGIC) 
		return -ENOTTY;
	if (_IOC_NR(cmd) > SCULL_IOC_MAXNR) 
		return -ENOTTY;

	if (_IOC_DIR(cmd) & _IOC_READ)
		err = !access_ok(VERIFY_WRITE, (void __user *)arg, _IOC_SIZE(cmd));
	else if (_IOC_DIR(cmd) & _IOC_WRITE)
		err =  !access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd));
	if (err) 
		return -EFAULT;

	switch(cmd) 
	{
	  case SCULL_IOCRESET:
		scull_quantum = SCULL_QUANTUM;
		scull_qset = SCULL_QSET;
		break;      
	  case SCULL_IOCSQUANTUM: 
		if (! capable (CAP_SYS_ADMIN))
			return -EPERM;
		retval = __get_user(scull_quantum, (int __user *)arg);
		break;
	  case SCULL_IOCTQUANTUM: 
		if (! capable (CAP_SYS_ADMIN))
			return -EPERM;
		scull_quantum = arg;
		break;
	  case SCULL_IOCGQUANTUM: 
		retval = __put_user(scull_quantum, (int __user *)arg);
		break;
	  case SCULL_IOCQQUANTUM: 
		return scull_quantum;
	  case SCULL_IOCXQUANTUM: 
		if (! capable (CAP_SYS_ADMIN))
			return -EPERM;
		tmp = scull_quantum;
		retval = __get_user(scull_quantum, (int __user *)arg);
		if (retval == 0)
			retval = __put_user(tmp, (int __user *)arg);
		break;
	  case SCULL_IOCHQUANTUM:
		if (! capable (CAP_SYS_ADMIN))
			return -EPERM;
		tmp = scull_quantum;
		scull_quantum = arg;
		return tmp;        
	  case SCULL_IOCSQSET:
		if (! capable (CAP_SYS_ADMIN))
			return -EPERM;
		retval = __get_user(scull_qset, (int __user *)arg);
		break;
	  case SCULL_IOCTQSET:
		if (! capable (CAP_SYS_ADMIN))
			return -EPERM;
		scull_qset = arg;
		break;
	  case SCULL_IOCGQSET:
		retval = __put_user(scull_qset, (int __user *)arg);
		break;
	  case SCULL_IOCQQSET:
		return scull_qset;
	  case SCULL_IOCXQSET:
		if (! capable (CAP_SYS_ADMIN))
			return -EPERM;
		tmp = scull_qset;
		retval = __get_user(scull_qset, (int __user *)arg);
		if (retval == 0)
			retval = put_user(tmp, (int __user *)arg);
		break;
	  case SCULL_IOCHQSET:
		if (! capable (CAP_SYS_ADMIN))
			return -EPERM;
		tmp = scull_qset;
		scull_qset = arg;
		return tmp;
	  case SCULL_P_IOCTSIZE:
		scull_p_buffer = arg;
		break;
	  case SCULL_P_IOCQSIZE:
		return scull_p_buffer;
	  default:
		return -ENOTTY;
	}

	return retval;
}

/*
 * llseek方法实现了lseek和llseek系统调用。如果设备操作未定义llseek方法，内核默认通过修改filp->f_pos而执行定位，
 * filp->f_pos是文件的当前读取/写入位置。为了使lseek系统调用能正确工作，read和write方法必须通过更新它们收到的偏
 * 移量参数来配合。如果能够明确处理一个数据区，那么对llseek的实现是有意义的。然而大多数设备只提供了数据流（就像串口
 * 和键盘），而不是数据区，定位这些设备是没有意义的。在这种情况下，不能简单地不声明llseek操作，因为默认方法是允许定
 * 位的。相反，我们应该在我们的open方法中调用nonseekable_open,以便通知内核设备不支持llseek。通过nonseekable_open的调用，
 * 会把给定的filp标记为不可定位；这样，内核就不会让这种文件上的lseek调用成功。通过这种方式标记文件，还可以确保通过pread
 * 和pwrite系统调用也不能定位文件。
 * 为了完整起见，还应该将file_operation结构中的llseek方法设置为特殊的辅助函数no_llseek，该函数定义在<linux/fs.h>中。
 */
loff_t scull_llseek(struct file *filp, loff_t off, int whence)
{
	struct scull_dev *dev = filp->private_data;
	loff_t newpos;

	switch(whence) 
	{
		case 0: /* SEEK_SET */
			newpos = off;
			break;
		case 1: /* SEEK_CUR */
			newpos = filp->f_pos + off;
			break;
		case 2: /* SEEK_END */
			newpos = dev->size + off;
			break;
	  	default: /* can't happen */
			return -EINVAL;
	}

	if (newpos < 0) 
		return -EINVAL;

	filp->f_pos = newpos;

	return newpos;
}

struct file_operations scull_fops = {
	.owner 	=	THIS_MODULE,
	.llseek	=	scull_llseek,
	.read	=	scull_read,
	.write	=	scull_write,
	.ioctl	=	scull_ioctl,
	.open	=	scull_open,
	.release =	scull_release,
};

static void scull_setup_cdev(struct scull_dev *dev, int index)
{
	int err, devno = MKDEV(scull_major, scull_minor + index);
    
	/*在内核内部，使用struct cdev结构来表示字符设备，以下函数可以初始化已分配到的cdev结构。因为本例中我们将cdev结构
	  嵌入到自己的特定设备结构中，所以采用下面的函数。也可以使用cdev_alloc()来分配一个cdev字符设备结构*/
	cdev_init(&dev->cdev, &scull_fops);	
	/*初始化cdev结构的所有者字段*/
	dev->cdev.owner = THIS_MODULE;	
	/*初始化设备的文件操作接口结构的指针*/	
	dev->cdev.ops = &scull_fops;		
	/*将cdev结构告诉内核，第一个参数是struct cdev机构，第二个是该设备对应的第一个设备编号，第三个参数是和该设备关联的
	  设备编号的数量。在使用cdev_add时，需要注意：首先，这个调用可能会失败。如果它返回一个负的错误码，则设备不会被添加到
	  系统中。但这个调用总是会成功返回。只要cdev_add成功返回了，设备就启动了，它的操作就会被内核调用。因此，在驱动程序还
	  没有完全准备好处理设备上的操作时，就不能调用cdev_add。*/
	err = cdev_add (&dev->cdev, devno, 1);
	
	if (err)
		printk(KERN_NOTICE "Error %d adding scull%d", err, index);

	return;
}

/*模块的清除函数*/
static void scull_cleanup_module(void)
{
	int i;
	dev_t devno;

	/*获取起始设备号*/
	devno = MKDEV(scull_major, scull_minor);

	if (scull_devices) 
	{
		for (i = 0; i < scull_nr_devs; i++) 
		{
			scull_trim(scull_devices + i);
			/*从系统中移除一个字符设备*/
			cdev_del(&scull_devices[i].cdev);
		}
		kfree(scull_devices);
	}

#ifdef SCULL_DEBUG
	scull_remove_proc();
#endif

	/*释放不再使用的设备编号，第一个参数是起始设备编号，第二个参数是需要释放的连续编号数*/
	unregister_chrdev_region(devno, scull_nr_devs);

	scull_p_cleanup();
	scull_access_cleanup();

	return; 
}

/*模块的初始化函数*/
static int scull_init_module(void)
{
	/*在内核的2.6.0版本中，dev_t是一个32位的数，其中12位用来表示主设备号，而其余20位用来表示次设备号*/
	dev_t dev;
    int result = -1,i = 0;

	/*用来获取主设备号，可由程序指定（或在insmod的时候指定相应值）或采用内核动态分配的方式*/
    if(scull_major)
	{
		/*将主设备号和次设备号转换成dev_t*/
		dev = MKDEV(scull_major,scull_minor);	
		/*获得设备的一个或多个编号，该种方式是由程序指定设备号，函数的第一个参数是设备号的起始值（通常次设备号都被设置成0），
		第二个参数表示所请求的连续设备编号的个数，第三个参数是和该编号范围关联的设备名称，它将出现在/proc/devices和sysfs中*/
		result = register_chrdev_region(dev,scull_nr_devs,"scull");	
	}
	else
	{
		/*通常情况下，我们都不知道使用哪个设备号。使用以下的函数，内核将恰当的为我们分配所需的主设备号，第一个参数仅用于输出的参数，
		在成功完成调用后将保存已分配范围的第一个编号，第二个参数是要使用的被请求的第一个次设备号，第三个参数表示所请求的连续设备编号
		的个数，第四个参数是和该编号范围关联的设备名称，它将出现在/proc/devices和sysfs中*/
		result = alloc_chrdev_region(&dev,scull_minor,scull_nr_devs,"scull");
		/*获取主设备号，采用MINOR（dev）可以获取到次设备号*/
		scull_major = MAJOR(dev);	
	}

	if(result < 0)
	{
		printk(KERN_WARNING"scull: can't get major %d\n",scull_major);
		return result;
	}

	scull_devices = kmalloc(scull_nr_devs * sizeof(struct scull_dev),GFP_KERNEL);
	if(scull_devices == NULL)
	{
		printk(KERN_WARNING"scull: can't kmalloc memory\n");
		result = -ENOMEM;
		goto fail;
	}
	memset(scull_devices,0,scull_nr_devs * sizeof(struct scull_dev));
	
	for (i = 0; i < scull_nr_devs; i++) 
	{
		scull_devices[i].quantum = scull_quantum;														
		scull_devices[i].qset = scull_qset;
		/*在运行时初始化互斥体*/
		init_MUTEX(&scull_devices[i].sem);
		scull_setup_cdev(&scull_devices[i], i);
	}
	dev = MKDEV(scull_major, scull_minor + scull_nr_devs);
	dev += scull_p_init(dev);
	dev += scull_access_init(dev);	

#ifdef SCULL_DEBUG
	scull_create_proc();
#endif

	return 0;

fail:
	scull_cleanup_module();

    return result;
}



/*用于指定模块的初始化和清除函数的宏*/
module_init(scull_init_module);
module_exit(scull_cleanup_module);

/*在目标文件中添加关于模块的文档信息*/
MODULE_AUTHOR("txgcwm");
MODULE_VERSION("scull_v1.0");
MODULE_LICENSE("GPL");
