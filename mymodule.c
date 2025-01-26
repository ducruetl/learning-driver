#include "linux/fs.h"
#include "linux/kern_levels.h"
#include "linux/printk.h"
#include "linux/stat.h"
#include "linux/types.h"
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>

dev_t device_number = 0;
int random_int = 0;

int notify_param(const char *val, const struct kernel_param *kp)
{
				int res = param_set_int(val, kp);
				if (res == 0) {
								printk(KERN_INFO "mymodule: Callback function called...\n");
								printk(KERN_INFO "mymodule: New value of random_int = %d\n", random_int);
								return 0;
				}
				return -1;
}

struct kernel_param_ops my_param_ops =
{
				.set = &notify_param,
				.get = &param_get_int,
};

module_param_cb(random_int, &my_param_ops, &random_int, S_IRUGO|S_IWUSR);

static int __init mod_init(void) 
{
				if(alloc_chrdev_region(&device_number, 0, 1, "mydriver") < 0)
				{
								printk(KERN_ERR "mymodule: Can't allocate major number for device 1\n");
								return -1;
				}
				printk(KERN_INFO "mymodule: Device major number : %d\n", MAJOR(device_number));
				printk(KERN_INFO "mymodule: Device minor number : %d\n", MINOR(device_number));
				printk(KERN_INFO "mymodule: random_int = %d\n", random_int);
				printk(KERN_INFO "mymodule: My kernel module has been inserted");
				return 0;	
}

static void __exit mod_exit(void) 
{
				unregister_chrdev_region(device_number, 1);
				printk(KERN_INFO "mymodule: My kernel module has been removed");
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Léo Ducruet");
MODULE_DESCRIPTION("A sample driver");
MODULE_VERSION("1.0");
