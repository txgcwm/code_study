#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>



/*
ssize_t vfs_read(struct file *file, char __user *buf, size_t count, loff_t *pos)
ssize_t vfs_write(struct file *file, const char __user *buf, size_t count, loff_t *pos)

在vfs_read和vfs_write函数中，其参数buf指向的用户空间的内存地址，如果我们直接使用内核空间的指针，
则会返回-EFALUT。这是因为使用的缓冲区超过了用户空间的地址范围。一般系统调用会要求你使用的缓冲区
不能在内核区，这个可以用set_fs()、get_fs()来解决。在include/asm/uaccess.h中，有如下定义：

#define MAKE_MM_SEG(s) ((mm_segment_t) { (s) })
#define KERNEL_DS MAKE_MM_SEG(0xFFFFFFFF)
#define USER_DS MAKE_MM_SEG(PAGE_OFFSET)
#define get_ds() (KERNEL_DS)
#define get_fs() (current->addr_limit)
#define set_fs(x) (current->addr_limit = (x))

使用示例如下：

mm_segment_t fs = get_fs();

set_fs(KERNEL_FS);
vfs_write();
vfs_read();
set_fs(fs);

详尽解释：系统调用本来是提供给用户空间的程序访问的，所以，对传递给它的参数（比如上面的buf），它默认
会认为来自用户空间，在read或write()函数中，为了保护内核空间，一般会用get_fs()得到的值来和USER_DS
进行比较，从而防止用户空间程序“蓄意”破坏内核空间；而现在要在内核空间使用系统调用，此时传递给read（）
或write（）的参数地址就是内核空间的地址了，在USER_DS之上(USER_DS ~ KERNEL_DS)，如果不做任何其它处理，
在write()函数中，会认为该地址超过了USER_DS范围，所以会认为是用户空间的“蓄意破坏”，从而不允许进一步的
执行；为了解决这个问题，set_fs(KERNEL_DS);将其能访问的空间限制扩大到KERNEL_DS,这样就可以在内核顺利
使用系统调用了。
*/

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
