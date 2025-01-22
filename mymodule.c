#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>

static int __init mod_init(void) 
{
	printk(KERN_INFO "My kernel module has been inserted");
	return 0;	
}

static void __exit mod_exit(void) 
{
	printk(KERN_INFO "My kernel module has been removed");
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Léo Ducruet");
MODULE_DESCRIPTION("A sample driver");
MODULE_VERSION("1.0");
