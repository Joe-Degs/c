#include <linux/module.h>
#include <linux/kernel.h>

int init_module(void)
{
	printk(KERN_INFO "hELLO BITCHES - FROM ANOTHER FILE\n");
	return 0;
}
