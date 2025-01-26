#include "linux/device/class.h"
#include "linux/err.h"
#include "linux/fs.h"
#include "linux/kern_levels.h"
#include "linux/printk.h"
#include "linux/stat.h"
#include "linux/types.h"
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/device.h>
#include <linux/kdev_t.h>

dev_t dev = 0;
static struct class *dev_class;

int random_int = 0;

/* Callback function for random_int*/
int notify_param(const char *val, const struct kernel_param *kp)
{
				int res = param_set_int(val, kp);
				if (res == 0) {
								pr_info("mymodule: Callback function called...\n");
								pr_info("mymodule: New value of random_int = %d\n", random_int);
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
				/* Allocating major number */
				if(alloc_chrdev_region(&dev, 0, 1, "mydriver") < 0)
				{
								pr_err("mymodule: Can't allocate major number for device 1\n");
								return -1;
				}
				pr_info("mymodule: Device major number : %d\n", MAJOR(dev));
				pr_info("mymodule: Device minor number : %d\n", MINOR(dev));

				/* Creating struct class */
				dev_class = class_create("mymodule");
				if (IS_ERR(dev_class)) {
								pr_err("mymodule: Can't create struct class\n");
								goto err_class;
				}

				/* Creating device */
				if (IS_ERR(device_create(dev_class, NULL, dev, NULL, "mydriver_device"))) {
								pr_err("mymodule: Can't create device\n");
								goto err_device;
				}

				pr_info("mymodule: random_int = %d\n", random_int);
				pr_info("mymodule: My kernel module has been inserted\n");
				return 0;

err_device:
				class_destroy(dev_class);
err_class:
				unregister_chrdev_region(dev, 1);
				return -1;

}

static void __exit mod_exit(void) 
{
				device_destroy(dev_class, dev);
				class_destroy(dev_class);
				unregister_chrdev_region(dev, 1);
				pr_info("mymodule: My kernel module has been removed\n");
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Léo Ducruet");
MODULE_DESCRIPTION("A sample driver");
MODULE_VERSION("1.0");
