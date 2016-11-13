#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>

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

	dev_info(&pdev->dev, "%s\n", __func__);
	return 0;
}

static int driver_remove(struct platform_device *pdev){
	struct my_drv_data *mdd = platform_get_drvdata(pdev);

	dev_info(mdd->dev, "%s\n", __func__);
	return 0;		
}

struct platform_driver pdrv = {
	.probe = driver_probe,
	.remove = driver_remove,
	.driver = {
		.name = "xxxx-test",
	},
};

static int __init hello_init(void)
{
	int ret = 0;

	ret = platform_driver_register(&pdrv);
	if (ret < 0) {
		pr_err("register platform_driver failed\n");
		return ret;
	}

	return 0;
}

static void __exit hello_exit(void)
{
	platform_driver_unregister(&pdrv);
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_DESCRIPTION("platform_driver");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("gene_chen <gene_chen@richtek.com>");
MODULE_VERSION("1.0.0");
