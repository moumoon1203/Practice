#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>


struct platform_device *pdev,*pdev2;

static int __init hello_init(void)
{
	pdev = platform_device_register_simple("xxxx-test", -1, NULL, 0);
	if (!pdev)
		return -ENODEV;
	pdev2 = platform_device_register_simple("xxxx-test", 223, NULL, 0);
	if (!pdev2) {
		platform_device_unregister(pdev);
		return -ENODEV;
	}

	return 0;
}

static void __exit hello_exit(void)
{
	platform_device_unregister(pdev2);
	platform_device_unregister(pdev);
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_DESCRIPTION("platform device");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("gene_chen <gene_chen@richtek.com>");
MODULE_VERSION("1.0.0");
