#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>

#include "printa.h"

struct my_drv_data {
	struct device *dev;
};

static int driver_probe(struct platform_device *pdev){
	struct my_drv_data *mdd;

	mdd = devm_kzalloc(&pdev->dev, sizeof(struct my_drv_data), GFP_KERNEL);
	if (!mdd)
		return -ENOMEM;
	
	mdd->dev = &pdev->dev;
	platform_set_drvdata(pdev, mdd);

	printa();	

	dev_info(&pdev->dev, "%s\n", __func__);
	return 0;
}

static int driver_remove(struct platform_device *pdev){
	struct my_drv_data *mdd = platform_get_drvdata(pdev);

	dev_info(mdd->dev, "%s\n", __func__);
	return 0;		
}

static struct platform_device_id pdev_id[] = {
	{"xxxx-test"}, 
	{ }
};
MODULE_DEVICE_TABLE(platform, pdev_id);

struct platform_device *pdev;
struct platform_driver pdrv = {
	.probe = driver_probe,
	.remove = driver_remove,
	.id_table = pdev_id,
	.driver = {
		.name = "yyyy-test",
	},
};

static int __init hello_init(void)
{
	int ret = 0;

	pdev = platform_device_register_simple("xxxx-test", -1, NULL, 0);
	if (!pdev)
		return -ENODEV;
		
	ret = platform_driver_register(&pdrv);
	if (ret < 0) {
		platform_device_unregister(pdev);
		pr_err("register platform_driver failed\n");
	}

	return 0;
}

static void __exit hello_exit(void)
{
	platform_driver_unregister(&pdrv);
	platform_device_unregister(pdev);
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_DESCRIPTION("platform_device and platform_driver");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("gene_chen <gene_chen@richtek.com>");
MODULE_VERSION("1.0.0");
