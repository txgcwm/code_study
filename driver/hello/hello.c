#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>



/*
ssize_t vfs_read(struct file *file, char __user *buf, size_t count, loff_t *pos)
ssize_t vfs_write(struct file *file, const char __user *buf, size_t count, loff_t *pos)

��vfs_read��vfs_write�����У������bufָ����û��ռ���ڴ��ַ���������ֱ��ʹ���ں˿ռ��ָ�룬
��᷵��-EFALUT��������Ϊʹ�õĻ������������û��ռ�ĵ�ַ��Χ��һ��ϵͳ���û�Ҫ����ʹ�õĻ�����
�������ں��������������set_fs()��get_fs()���������include/asm/uaccess.h�У������¶��壺

#define MAKE_MM_SEG(s) ((mm_segment_t) { (s) })
#define KERNEL_DS MAKE_MM_SEG(0xFFFFFFFF)
#define USER_DS MAKE_MM_SEG(PAGE_OFFSET)
#define get_ds() (KERNEL_DS)
#define get_fs() (current->addr_limit)
#define set_fs(x) (current->addr_limit = (x))

ʹ��ʾ�����£�

mm_segment_t fs = get_fs();

set_fs(KERNEL_FS);
vfs_write();
vfs_read();
set_fs(fs);

�꾡���ͣ�ϵͳ���ñ������ṩ���û��ռ�ĳ�����ʵģ����ԣ��Դ��ݸ����Ĳ��������������buf������Ĭ��
����Ϊ�����û��ռ䣬��read��write()�����У�Ϊ�˱����ں˿ռ䣬һ�����get_fs()�õ���ֵ����USER_DS
���бȽϣ��Ӷ���ֹ�û��ռ�������⡱�ƻ��ں˿ռ䣻������Ҫ���ں˿ռ�ʹ��ϵͳ���ã���ʱ���ݸ�read����
��write�����Ĳ�����ַ�����ں˿ռ�ĵ�ַ�ˣ���USER_DS֮��(USER_DS ~ KERNEL_DS)����������κ���������
��write()�����У�����Ϊ�õ�ַ������USER_DS��Χ�����Ի���Ϊ���û��ռ�ġ������ƻ������Ӷ��������һ����
ִ�У�Ϊ�˽��������⣬set_fs(KERNEL_DS);�����ܷ��ʵĿռ���������KERNEL_DS,�����Ϳ������ں�˳��
ʹ��ϵͳ�����ˡ�
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
