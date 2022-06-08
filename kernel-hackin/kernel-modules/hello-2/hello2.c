#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

static int __init hello_init(void)
{
	printk(KERN_INFO "hello number two\n");
	return 0;
}

static void __exit hello_exit(void)
{
	printk(KERN_INFO "exiting hello number two\n");
}

module_init(hello_init);
module_exit(hello_exit);
