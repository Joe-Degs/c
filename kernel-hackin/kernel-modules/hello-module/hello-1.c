#include <linux/module.h>
#include <linux/kernel.h>

/* the init function. on inserting the module, this function is executed to
 * set things up before kernel starts to execute the the module program
 */
int init_module(void)
{
	/* is essentially a logger, so you have to specify a loglevel which
	 * are constants in kernel.h. check em out
	*/
	printk(KERN_INFO "hello world 1\n");
	return 0;
}

/* cleanup function to undo all things that init function did
 */
void cleanup_module(void)
{
	printk(KERN_INFO "goodbye fuckers 1\n");
}
