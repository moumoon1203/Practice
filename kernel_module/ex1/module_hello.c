#include <linux/init.h>
#include <linux/module.h>

static int __init module_hello_init(void)
{
	pr_info("module_hello_init\n");
	return 0;
}

static void __exit module_hello_exit(void)
{
	printk(KERN_INFO "module_exit\n");
}

module_init(module_hello_init);
module_exit(module_hello_exit);

MODULE_DESCRIPTION("kernel module");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("gene_chen <gene_chen@richtek.com>");
MODULE_VERSION("1.0.0");
