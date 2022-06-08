/*
 * this module demonstrates how to document module
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#define DRIVER_AUTHOR "Joe Degs <joe@debianpc.org>";
#define DRIVER_DESC "A hello world kernel module";

static int __init hello_init(void)
{
	printk(KERN_INFO "Hello World with module descriptions\n");
	return 0;
}

static void __exit hello_exit(void)
{
	printk(KERN_INFO "Goodbye!\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("Dual MIT/GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_SUPPORTED_DEVICE("testdevice");
