#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/pm_wakeup.h>

struct my_drv_data {
	struct device *dev;
	struct wakeup_source *ws;
};

static int driver_probe(struct platform_device *pdev)
{
	int ret = 0;
	struct my_drv_data *mdd;
	int cal = 0x22;
		pr_info("0x22*3+0x13 = %x\n", cal*3+0x13);
	
	mdd = devm_kzalloc(&pdev->dev, sizeof(struct my_drv_data), GFP_KERNEL);
	if (!mdd)
		return -ENOMEM;
	
	mdd->dev = &pdev->dev;
	platform_set_drvdata(pdev, mdd);

	device_set_wakeup_capable(&pdev->dev, 1);
	ret = device_wakeup_enable(&pdev->dev);
	if (ret < 0)
		dev_err(&pdev->dev, "device_wakeup_enable failed : %s\n", __func__);

	dev_info(&pdev->dev, "%s\n", __func__);
	return 0;
}

static int driver_remove(struct platform_device *pdev)
{
	struct my_drv_data *mdd = platform_get_drvdata(pdev);

	device_wakeup_disable(&pdev->dev);
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
		.name = "xxxx-test",
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
		pr_err("register pdrv failed\n");
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

MODULE_DESCRIPTION("wakelock init practice");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("gene_chen <gene_chen@richtek.com>");
MODULE_VERSION("1.0.0");
