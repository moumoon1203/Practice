#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/kthread.h>
#include <linux/delay.h>

#define N_KTHREADS 20

struct my_drv_data {
	struct device *dev;
	struct task_struct *tsk[N_KTHREADS];
};

static int kthread_function(void *data)
{
	struct my_drv_data *mdd = (struct my_drv_data *)data;	
	static int kthread_val = 0;
	int ret = 0;
	int i = 0;
	
	for (i = 0; i < 1000; i++) { 
		kthread_val++;
		mdelay(1);
	};
	
	dev_info(mdd->dev, "kthread stop, kthread_val = %d, %s\n", kthread_val, __func__);
	return ret;
}

static int driver_probe(struct platform_device *pdev){
	struct my_drv_data *mdd;
	int ret = 0;
	int i = 0;

	mdd = devm_kzalloc(&pdev->dev, sizeof(struct my_drv_data), GFP_KERNEL);
	if (!mdd)
		return -ENOMEM;
	
	mdd->dev = &pdev->dev;
	platform_set_drvdata(pdev, mdd);
	
	for (i = 0; i < N_KTHREADS; i++) {
		mdd->tsk[i] = kthread_run(kthread_function, (void *)mdd, "test_thread%d", i);
		if (IS_ERR(mdd->tsk[i])) {
			ret = PTR_ERR(mdd->tsk[i]);
			dev_err(&pdev->dev, "kthread_run failed : %d\n", i);
			goto fail_kthread_run;
		}
	}

	dev_info(&pdev->dev, "%s\n", __func__);
	return 0;
fail_kthread_run:
	devm_kfree(&pdev->dev, mdd);
	return ret;
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