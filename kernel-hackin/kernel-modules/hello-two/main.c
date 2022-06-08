#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

/* this new method of writing modules does not restrict naming to "init_module"
 * and "cleanup_module". Using the init header, the name of init and cleanup
 * routines can be given any names of the coder's choice
 */
static int __init hello_init(void)
{
	printk(KERN_INFO "hello world number two people\n");
	return 0;
}

static void __exit hello_exit(void)
{
	printk(KERN_INFO "exiting hello number two people\n");
}

module_init(hello_init);
module_exit(hello_exit);
