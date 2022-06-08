#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

/* KERN_INFO is a macro defined in the Linux kernel headers that expands to a 
 * string literal when the preprocessor runs. When placing string literals 
 * adjacently in C code they are implicitly concatenated; when placing a 
 * variable between string literals, it's a syntax error.
 * If you preprocess your code to a file, you will observe this more easily
 */
static const char* hello_string __initdata = KERN_INFO "Hello World! %s\n";

static int __init hello_init(void)
{
	printk(hello_string, "From the string side");
	return 0;
}

static void __exit hello_exit(void)
{
	printk(KERN_INFO "GOODBYE!\n");
}

module_init(hello_init);
module_exit(hello_exit);
