#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/types.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Natesh");
MODULE_DESCRIPTION("A chardev example");


static int chardev_init(void);
static void chardev_exit(void);
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);
module_init(chardev_init);
module_exit(chardev_exit);

#define SUCCESS 0
#define DEVICE_NAME "chardev"
#define BUF_LEN 80

static int Major;
static int Device_Open = 0;

static char msg[BUF_LEN];
static char *msg_Ptr;

static struct file_operations fops = {
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release
};

static int chardev_init(void)
{
	Major = register_chrdev(0,DEVICE_NAME,&fops);

	if(Major<0)
	{
		printk(KERN_ALERT "Registering char device failed with %d\n", Major);
		return Major;
	}

	printk(KERN_INFO "I was assigned Major number %d. to talk to\n", Major);
	printk(KERN_INFO "the driver create a dev file with\n");
	printk(KERN_INFO "mknod /dev/%s c %d 0.\n", DEVICE_NAME, Major);
	printk(KERN_INFO "the device file.\n");
	printk(KERN_INFO "Remove the device file and module when done.\n");

	return SUCCESS;
}

static void chardev_exit(void)
{
	unregister_chrdev(Major, DEVICE_NAME);
	/*if(ret<0)
	{
		printk(KERN_ALERT "Error in unregister_chrdev: %d.\n", ret);
	}*/
}

static int device_open(struct inode *inode, struct file *file)
{
	static int counter = 0;

	if(Device_Open)
	{
		return -EBUSY;
	}

	Device_Open++;
	sprintf(msg,"I alread told you %d times Hello World!\n", counter++);
	msg_Ptr=msg;
	try_module_get(THIS_MODULE);

	return SUCCESS;
}

static int device_release(struct inode *inode, struct file *file)
{
	Device_Open--;
	module_put(THIS_MODULE);

	return 0;
}

static ssize_t device_read(struct file *filp, char *buffer, size_t length, loff_t *offset)
{
	int bytes_read = 0;

	if(*msg_Ptr == 0)
	{
		return 0;
	}
	while(length && *msg_Ptr)
	{
		put_user(*(msg_Ptr++), buffer++);
		length--;
		bytes_read++;
	}

	return bytes_read;
}

static ssize_t device_write(struct file *filp, const char *buffer, size_t length, loff_t *offset)
{
	printk(KERN_ALERT "Sorry, this operation isn't supported.\n");
	return -EINVAL;
}





