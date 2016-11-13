#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/hrtimer.h>

struct my_drv_data {
	struct device *dev;
	struct hrtimer my_hrtimer;
};

static enum hrtimer_restart my_hrtimer_callback(struct hrtimer *hrt)
{
	struct my_drv_data *mdd = container_of(hrt, struct my_drv_data, my_hrtimer);

	dev_info(mdd->dev, "timeout, %s\n", __func__);
	hrtimer_forward_now(hrt, ktime_set(0, 200000000));
	return HRTIMER_RESTART;
}

static int driver_probe(struct platform_device *pdev){
	struct my_drv_data *mdd;
	ktime_t kt;

	mdd = devm_kzalloc(&pdev->dev, sizeof(struct my_drv_data), GFP_KERNEL);
	if (!mdd)
		return -ENOMEM;
	
	mdd->dev = &pdev->dev;
	platform_set_drvdata(pdev, mdd);

	kt = ktime_set(0, 200000000);
	hrtimer_init(&mdd->my_hrtimer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	mdd->my_hrtimer.function = my_hrtimer_callback;
	hrtimer_start(&mdd->my_hrtimer, kt, HRTIMER_MODE_REL);

	dev_info(&pdev->dev, "%s\n", __func__);
	return 0;
}

static int driver_remove(struct platform_device *pdev){
	struct my_drv_data *mdd = platform_get_drvdata(pdev);

	hrtimer_cancel(&mdd->my_hrtimer);
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
