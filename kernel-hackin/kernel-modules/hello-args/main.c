#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/stat.h>
#include <linux/moduleparam.h>

// read user, write user, read group
#define RUWURG (S_IRUSR|S_IWUSR|S_IRGRP)

MODULE_LICENSE("Dual MIT/GPL");
MODULE_AUTHOR("Joseph Norkplim Attah <jpedegs8990@gmail.com>");

static short int myshort = 1;
static int myint         = 420;
static long int mylong   = 9999;
static char* mystring    = "blah";
static int myIntArray[2] = {-1, -1};
static int argc     = 0;

/*
 * module_param(<variable>, <type>, <permission_bits>)
 * but why do we need permissions for variables??
 * - cos modules are represented in sysfs and parameters also get
 *   their representation in sysfs. Setting permissions will enable
 *   sysfs to set the right file permission when representing the
 *   variable on the filesystem
 * - putting a zero as the permission bit means the param does not
 *   get sysfs entry at all. Also perm = 0 means you can set the the
 *   value at module load time.
 */

module_param(myshort, short, RUWURG | S_IWGRP);
MODULE_PARM_DESC(myshort, "A short integer");
module_param(myint, int, RUWURG | S_IROTH);
MODULE_PARM_DESC(myint, "An integer");
module_param(mylong, long, S_IRUSR);
module_param(mystring, charp, 0000);
MODULE_PARM_DESC(mystring, "A character string");

/*
 * module_param_array(<array_var>, <arr_element_type>, <int_pointer>, <perms>)
 * the <int_pointer> type represents the variable which will store the number of
 * elemenents of the array is initialized by the user at module loading time
 * its just like an "argc" but for kernel modules
 */
module_param_array(myIntArray, int, &argc, 0000);

static int __init args_init(void)
{
	int i;
	printk(KERN_INFO "Hello, World! with arguments ====================\n");
	printk(KERN_INFO "myshort is a short integer: %hd\n", myshort);
	printk(KERN_INFO "myint is an integer: %d\n", myint);
	printk(KERN_INFO "mylong is a long integer: %ld\n", mylong);
	printk(KERN_INFO "mystring is a null terminated string: %s\n", mystring);

	for (i=0; i<sizeof(myIntArray)/sizeof(int); i++) {
		printk(KERN_INFO "myIntArray[%d] = %d\n", i, myIntArray[i]);
	}
	return 0;
}

static void __exit args_exit(void)
{
	printk(KERN_INFO "Exiting... kernel module example with args\n");
}

module_init(args_init);
module_exit(args_exit);
