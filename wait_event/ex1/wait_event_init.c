#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/wait.h>
#include <linux/slab.h>

#define N_KTHREADS 20

struct my_drv_data {
	struct device *dev;
	struct task_struct *tsk[N_KTHREADS];
	wait_queue_head_t wq_head;
	atomic_t v;
};

struct thread_data {
	struct my_drv_data *mdd;
	int thread_id;
};

static int kthread_function(void *data)
{
	struct thread_data *tdata = (struct thread_data *)data;
	struct my_drv_data *mdd = tdata->mdd;
	static int kthread_val = 0;
	int ret = 0;
	int i = 0;
	
	wait_event(mdd->wq_head, atomic_read(&mdd->v) == tdata->thread_id); 
	for (i = 0; i < 1000; i++)
		kthread_val++;

	dev_info(mdd->dev, "%d kthread stop, kthread_val = %d, %s\n", tdata->thread_id, kthread_val, __func__);
	atomic_inc(&mdd->v);
	wake_up(&mdd->wq_head);
	return ret;
}

static int driver_probe(struct platform_device *pdev){
	struct my_drv_data *mdd;
	struct thread_data *tdata[N_KTHREADS];
	int ret = 0;
	int i = 0;	
	
	mdd = devm_kzalloc(&pdev->dev, sizeof(struct my_drv_data), GFP_KERNEL);
	if (!mdd)
		return -ENOMEM;	
	mdd->dev = &pdev->dev;
	platform_set_drvdata(pdev, mdd);

	for (i = 0; i < N_KTHREADS; i++) {
		tdata[i] = kzalloc(sizeof(struct thread_data), GFP_KERNEL);
		if (!tdata[i]) {
			dev_err(&pdev->dev, "fail thread data allocate\n");
			ret = -ENOMEM;
			goto fail_thr_mem_alloc;
		}
	}

	init_waitqueue_head(&mdd->wq_head);	
	atomic_set(&mdd->v, 0);
	dev_info(&pdev->dev, "%s\n", __func__);

	for (i = 0; i < N_KTHREADS; i++) {
		tdata[i]->mdd = mdd;
		tdata[i]->thread_id = i;
		mdd->tsk[i] = kthread_run(kthread_function, (void *)tdata[i], "test_thread%d", i);
		if (IS_ERR(mdd->tsk[i])) {
			ret = PTR_ERR(mdd->tsk[i]);
			dev_err(&pdev->dev, "kthread_run failed : %d\n", i);
			goto fail_kthread_run;
		}
	}

	return 0;
fail_kthread_run:
	while(--i >= 0)
		kfree(tdata[i]);
fail_thr_mem_alloc:
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
