#include <linux/module.h>
#include <linux/kernel.h>

MODULE_AUTHOR ("Druzhinin Maksim");
MODULE_VERSION ("1.0");
MODULE_LICENSE ("GPL");

int init_module (void) {
	printk (KERN_INFO "Hello, world!\n");
	return 0;
}

void cleanup_module (void) {
	printk (KERN_INFO "Bye!\n");
}
