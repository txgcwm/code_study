#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/dcache.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/string.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/kernel.h>  
#include <linux/list.h>  
#include <linux/cpumask.h> 

#include <asm/fcntl.h>
#include <asm/processor.h>
#include <asm/uaccess.h>



#define MY_FILE  "/tmp/log_file.log"

static int __init file_init(void)
{
	int result = 0;
	char *read_buf = NULL;
	struct file *filp = NULL;
	mm_segment_t old_fs;

	read_buf = kmalloc(PAGE_SIZE, GFP_KERNEL);
	if (!read_buf) {
		printk("Read Buf Alloc Error!\r\n");
		return 0;
	}
	
	memcpy(read_buf, "hello", 5);

	filp = filp_open(MY_FILE, O_CREAT | O_RDWR, 0600);	//创建文件
	if (filp) {
		old_fs = get_fs();
		set_fs(get_ds());

		result = filp->f_op->write(filp, read_buf, PAGE_SIZE, &filp->f_pos);	//写文件
		if (result) {
			printk("New Log Write OK Length: %d\n", result);
		} else {
			printk("Write Log File Error\n");
		}

		set_fs(old_fs);

		filp_close(filp, NULL);
	} else {
		printk("Create New Log file failtrue!\n");
	}

	kfree(read_buf);
	read_buf = NULL;

#if 0
	struct module *mod,*relate;  
    int cpu;  
  
    // 打印本模块的模块名和模块状态  
	printk(KERN_ALERT"[insmod mymod] name:%s state:%d\n", THIS_MODULE->name, THIS_MODULE->state);  
  
	// 遍历模块列表，查找target模块  
	list_for_each_entry(mod,THIS_MODULE->list.prev,list) {  
		if(strcmp(mod->name, "target")==0) {   
  
                        // 打印target的模块名、模块状态、引用计数  
                        printk(KERN_ALERT"name:%s state:%d refcnt:%u ",mod->name,mod->state,module_refcount(mod));  
  
                        // 打印出所有依赖target的模块名  
                        if(!list_empty(&mod->modules_which_use_me)) {   
                                list_for_each_entry(relate,&mod->modules_which_use_me,modules_which_use_me)  
                                        printk(KERN_ALERT"%s ",relate->name);  
                        } else  
                                printk(KERN_ALERT"used by NULL\n");  
  
                        // 把target的引用计数置为0  
                        for_each_possible_cpu(cpu)  
                                local_set(__module_ref_addr(mod,cpu),0);  
  
                        // 再看看target的名称、状态、引用计数  
			printk(KERN_ALERT"name:%s state:%d refcnt:%u\n",mod->name,mod->state,module_refcount(mod));  
		}  
	}  
#endif

	printk(KERN_INFO "Loading the module ...\n");

	return 0;
}

static void __exit file_exit(void)
{
	printk(KERN_INFO "Unloading the module...\n");

	return;
}


module_init(file_init);
module_exit(file_exit);

MODULE_LICENSE("Dual BSD/GPL");

