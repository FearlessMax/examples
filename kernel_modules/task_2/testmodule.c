#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Druzhinin Maxim");
MODULE_VERSION ("1.0");
MODULE_SUPPORTED_DEVICE("test"); /* /dev/test */

#define BUF_LEN 80
#define DEVICE_NAME "test"

/*--- Функции ---*/
static int device_open (struct inode *, struct file *);
static int device_release (struct inode *, struct file *);
static ssize_t device_read (struct file *, char *buffer, size_t, loff_t *);
static ssize_t device_write (struct file *, const char *buffer, size_t, loff_t *);

/*--- Переменные ---*/
static int major;
static int is_device_open = 0;
static char text[BUF_LEN];
static char *text_ptr = text;

/*--- FOPS ---*/
struct file_operations fops =
{
	.open = device_open,
	.release = device_release,
	.read = device_read,
	.write = device_write
};

/*--- Инициализация ---*/
int init_module (void)
{
	major = register_chrdev(0, DEVICE_NAME, &fops); //Регистрация

	if (major < 0)
	{
		printk(KERN_ALERT "Registering the character device failed with %d\n", major);
		return major;
	}

	printk(KERN_INFO "Test module is loaded!\n" );
	printk(KERN_INFO "Please, create a dev file with 'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, major);

	return 0;
}

/*--- Выгрузка ---*/
void cleanup_module (void)
{
	unregister_chrdev(major, DEVICE_NAME); //Удаление драйвера
	printk(KERN_ALERT "Test module is unloaded!\n");
}

/*--- Open ---*/
static int device_open(struct inode *inode, struct file *file)
{
	static int counter = 0;	

	if (is_device_open)
		return -EBUSY;

	is_device_open++;

	sprintf(text, "I already told you %d times Hello World!\n", counter++);

	text_ptr = text;

	try_module_get (THIS_MODULE);

	return 0;
}

/*--- Release ---*/
static int device_release (struct inode *inode, struct file *file)
{
	is_device_open--;

	module_put (THIS_MODULE);

	return 0;
}

/*--- Write ---*/
static ssize_t device_write (struct file *filp,const char *buffer, size_t lenght, loff_t * offset)
{
	printk( "???\n" );
	return -EINVAL;
}

/*--- Read ---*/
static ssize_t device_read (struct file *filp, char *buffer, size_t length, loff_t * offset)
{
	static int bytes_read = 0;

	if (*text_ptr == 0)
		return 0;

	while (length && *text_ptr)
	{
		put_user(*( text_ptr++ ), buffer++);
		length--;
		bytes_read++;
	}	

	return bytes_read;
}
