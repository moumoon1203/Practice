#include <linux/init.h>
#include <linux/module.h>

static int __init hello_init(void)
{
	pr_info("Module init\n");
	return 0;
}

static void __exit hello_exit(void)
{
	pr_info("Module exit\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_DESCRIPTION("module create practice");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("gene_chen <gene_chen@richtek.com>");
MODULE_VERSION("1.0.0");
