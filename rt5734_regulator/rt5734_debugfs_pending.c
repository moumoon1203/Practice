#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/spi.h>

#include <linux/mt-plat/rt-regmap.h>

struct rt5734_drv_data {
	struct device *dev;
	struct spi_device *spi;
	struct rt_regmap_device *rd;
};

static unsigned char rt5734_reg_data[4][6] = {
	{0x48, 0x49, 0x7c, 0x7d, 0x62, 0x63},
	{0x48, 0x49, 0x7c, 0x7d, 0x62, 0x63},
	{0x48, 0x49, 0x7c, 0x7d, 0x62, 0x63},
	{0x48, 0x49, 0x7c, 0x7d, 0x62, 0x63},
};

staitc 

static struct rt_regmap_fops rt5734_rops = {
	.read_device = &rt5734_read_device,
	.write_device = &rt5734_write_device,
};

RT_REG_DECL(0x00, 24, RT_NORMAL_WR_ONCE, {});
RT_REG_DECL(0x01, 24, RT_NORMAL_WR_ONCE, {});
RT_REG_DECL(0x02, 24, RT_NORMAL_WR_ONCE, {});
RT_REG_DECL(0x03, 24, RT_NORMAL, {});

static struct rt_register_map_t rt5734_map[] = {
	RT_REG(0x00),
	RT_REG(0x01),
	RT_REG(0x02),
	RT_REG(0x03),
};

static struct rt_regmap_properties rt5734_rprop = {
	.register_num = REGISTER_NUM,
	.rm = rt5734_map,
	.rt_regmap_mode = RT_MULTI_BYTE | RT_CACHE_WR_THROUGH |
					  RT_IO_PASS_THROUGH | RT_DBG_SPECIAL,
	.name = "rt5734-regmap-properties",
	.aliases = "rt5734_rprop",
	.io_log_en = 1,
};

static int rt5734_debugfs_probe(struct spi_device *spi)
{
	struct	rt5734_drv_data *rdd;
	int ret = 0;

	rdd = devm_malloc(&spi->dev, sizeof(*rdd), GFP_KERNEL);
	if (!rdd)
		return -ENOMEM;
	rdd->dev = dev;
	rdd->spi = spi;
	rtc->data = (unsigned char **)rt5734_reg_data;
	spi_set_drvdata(spi, rdd);
	
	/* register rt_regmap_device */
	rdd->rd = rt_regmap_device_register(&rt5734_rprop, &rt5734_rops,
										&spi->dev, spi, rdd);
	if(!rdd->rd) {
		dev_err(&i2c->dev, "regmap device register fail\n");
		ret = -EINVAL;
		goto fail_register_regmap;
	};

	dev_info(&spi->dev, "%s successfully\n", __func__);	
	return 0;
fail_register_regmap:
	devm_kfree(&spi->dev);
	return ret;
}

static int rt5734_debugfs_remove(struct spi_device *spi)
{
	return 0;
}

struct const struct of_device_id rt5734_dt_match[] = {
	{ .compatible = "richtek,rt5734" },
	{ },
};

static const struct spi_dev_id = rt5734_dev_id[] = {
	{ "12d3000.spi", 0 },
	{ }
};

MODULE_DEVICE_TABLE(dt, rt5734_dt_match);

static struct spi_driver rt5734_debugfs_driver = {
	.driver = {
		.name = "rt5734_debugfs",
		.owner = THIS_MODULE,
		.of_match_table = rt5734_dt_match,
	},
	.probe = rt5734_debugfs_probe;
	.remove = rt5734_debugfs_remove;
};
module_spi_driver(rt5734_debugfs_driver);

MODULE_DESCRIPTION("rt5734 debugfs driver");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("gene_chen <gene_chen@richtek.com>");
MODULE_VERSION("1.0.0");

