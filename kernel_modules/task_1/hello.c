#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE ("GPL");
MODULE_AUTHOR ("Druzhinin Maxim");
MODULE_VERSION ("1.0");

int init_module (void)		// загрузка модуля
{
	printk (KERN_INFO "Hello, world!\n");
	return 0;
}

void cleanup_module (void)		// выгрузка модуля
{
	printk (KERN_INFO "Bye!\n");
}
