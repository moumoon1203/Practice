#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/spi.h>

#define N_RT5734_REGULATORS 3
#define N_RT5734_SELECTORS 255
#define RT5734_BUCK_NAME(id) ("Buck_DVS"#id)

struct rt5734_drv_data {
	struct spi_device *spi;
	struct device *dev;
};

struct rt5734_regulator_desc {
	struct regulator_desc regulator_desc;
	uint8_t vsel_addr;
	uint8_t en_addr;
	uint8_t en_mask;
};

static int rt5734_spi_read(struct spi_device *spi, uint8_t reg)
{
	struct rt5734_drv_data *rdd = spi_get_drvdata(spi);
	uint8_t tx_buf[3];

	tx_buf[0] = 0xc0;
	tx_buf[1] = reg;
	tx_buf[2] = 0x01;
	ret = spi_write_then_read(spi, tx_buf, ARRAY_SIZE(tx_buf), tx_buf, 1);
	if (ret < 0)
		return ret;
	return tx_buf[0];
}

static int rt5734_spi_write(struct spi_device *spi, uint8_t reg, uint8_t data)
{
	struct rt5734_drv_data *rdd = spi_get_drvdata(spi);
	uint8_t tx_buf[4];

	tx_buf[0] = 0x40;
	tx_buf[1] = reg;
	tx_buf[2] = 0x01; /* packet length */
	tx_buf[3] = data;
	return spi_write(spi, tx_buf, ARRAY_SIZE(tx_buf));
}

static int rt5734_spi_assign_bits(struct spi_device *spi, uint8_t reg,
								  uint8_t mask, uint8_t val)
{
	int ret = 0;
	
	ret = rt5734_spi_read(spi, reg);
	if (ret < 0)
		return ret;
	ret &= ~mask;
	ret |= (val & mask);
	return rt5734_spi_write(spi, reg, ret);
}

#define rt5734_spi_clr_bits(spi, reg, mask)\
	rt5734_spi_assign(spi, reg, mask, 0)
#define rt5734_spi_set_bits(spi, reg, mask)\
	rt5734_spi_assign(spi, reg, mask, mask)
static int rt5734_enable(struct regulator_dev *rdev)
{
	struct rt5734_drv_data *rdd = dev_get_drvdata(rdev->dev.parent);
	struct rt5734_regulator_desc *desc = rdev_get_drvdata(rdev);
	
	dev_info(&rdev->dev, "%s\n", __func__);
	return rt5734_spi_set_bits(rdd->spi, desc->en_addr, desc->en_mask);
}

static int rt5734_disable(struct regulator_dev *rdev)
{	
	struct rt5734_drv_data *rdd = dev_get_drvdata(rdev->dev.parent);
	struct rt5734_regulator_desc *desc = rdev_get_drvdata(rdev);
	
	dev_info(&rdev->dev, "%s\n", __func__);
	return rt5734_spi_clr_bits(rdd->spi, desc->en_addr, desc->en_mask);
}

static int rt5734_list_voltage(struct regulator_dev *rdev, uint8_t selector)
{
	int ret = 0;
	
	dev_info(&rdev->dev, "%s\n", __func__);
	if (selector > 0xc8)
		ret = 1300000 + 10000 * (selector - 200);
	else
		ret = 300000 + 5 * selector;
	return ret;
}

static int rt5734_get_voltage_sel(struct regulator_dev *rdev)
{
	struct rt5734_drv_data *rdd = dev_get_drvdata(rdev->dev.parent);
	struct regulator_desc *desc = rdev_get_drvdata(rdev);
	
	dev_info(&rdev->dev, "%s\n", __func__);
	return rt5734_spi_read(spi->dev, desc->vsel_addr);
}

static int rt5734_set_voltage_sel(struct regulator_dev *rdev, uint8_t selector)
{
	struct rt5734_drv_data *rdd = dev_get_drvdata(rdev->dev.parent);
	struct regulator_desc *desc = rdev_get_drvdata(rdev);
	
	dev_info(&rdev->dev, "%s\n", __func__);
	return rt5734_spi_write(spi->dev, desc->vsel_addr, selector);
}

static struct regulator_ops rt5734_ops = {
	.enable = rt5734_enable,
	.disable = rt5734_disable,
	.list_voltage = rt5734_list_voltage,
	.get_voltage_sel = rt5734_get_voltage_sel,
	.set_voltage_sel = rt5734_set_voltage_sel,
};

static struct rt5734_regulator_desc rt5734_desc[N_RT5734_REGULATORS] = {
		{
			.regulator_desc = {
				.name = RT5734_BUCK_NAME(0),
				.id = 0,
				.n_voltages =N_RT5734_SELECTORS,
				.type = REGULATOR_VOLTAGE,
				.owner = THIS_MODULE,
				.ops = &rt5734_ops,
			},
			.vsel_addr = 0x48,
			.en_addr = 0x49,
			.en_mask = 0x01,
		},
		{
			.regulator_desc = {
				.name = RT5734_BUCK_NAME(0),
				.id = 0,
				.n_voltages =N_RT5734_SELECTORS,
				.type = REGULATOR_VOLTAGE,
				.owner = THIS_MODULE,
				.ops = &rt5734_ops,
			},
			.vsel_addr = 0x7c,
			.en_addr = 0x7d,
			.en_mask = 0x01,
		},
		{
			.regulator_desc = {
				.name = RT5734_BUCK_NAME(2),
				.id = 0,
				.n_voltages =N_RT5734_SELECTORS,
				.type = REGULATOR_VOLTAGE,
				.owner = THIS_MODULE,
				.ops = &rt5734_ops,
			},
			.vsel_addr = 0x62,
			.en_addr = 0x63,
			.en_mask = 0x01,
		},

static int rt5734_spi_probe(struct spi_device *spi)
{
	struct rt5734_drv_data *rdd;
	struct regulator_init_data *rt_init_data;
	struct rt5734_regulator_desc *rt_desc;
	struct regulator_config config = { };
	struct device_node *np;
	int ret = 0;
	int i = 0;

	rdd = devm_kzalloc(&spi->dev, sizeof(*rdd), GFP_KERNEL);
	if (!rdd)
		return -ENOMEM;
	rdd->spi = spi;
	rdd->dev = &spi->dev;
	spi_set_drvdata(spi, rdd);

	spi->bits_per_words = 8;
	ret = spi_setup(spi);
	if(!ret) {
		dev_info(spi->dev, "spi_setup failed\n");
		goto out_chip_id;
	}

	/* test chip id */
	ret = rt5734_spi_read(spi, 0x01);
	if (ret < 0)
		goto out_chip_id;
	if (ret != 0x01) {
		dev_err(spi->dev, "invalid chip id = %d\n", ret);
		ret = -ENODEV;
		goto out_chip_id;
	}
	
	/* rt5734 regulator register */
	for (i = 0; i < N_RT5734_REGULATORS; i++) {
		rt_desc = rt5734_regulator_desc[i];	
		np = of_find_node_by_name(&spi->dev.of_node, RT5734_BUCK_NAME(i));
		if (!np)
			dev_err(spi->dev, "no %d node ???\n", i);
		rt_init_data = of_get_regulator_init_data(spi->dev, np);
		config.dev = &spi->dev;
		config.init_data = rt_init_data;
		config.driver_data = rt_desc;
		
		rdd->rdev[i] = regulator_register(&rt_desc->regulator_desc, &rt_config);
		if (IS_ERR(rdd->rdev[i])) {
			ret = PTR_ERR(rdd->rdev[i]);
			dev_err(spi->dev, "regulator register failed = %d\n", i);
			goto fail_regulator_register;
		}
	}

	return 0;
fail_regulator_register:
	while (--i < 0)
		regulator_unregister(&rdd->rdev[i]);
out_chip_id:
	devm_kfree(&spi->dev, rdd);
	return ret;
}

static int rt5734_spi_remove(struct spi_device *spi)
{

}

static const struct of_device_id rt5734_dt_match[] = {
	{ compatible = "richtek,rt5734" }.
	{},
};

static struct spi_driver rt5734_spi_driver = {
	.driver = {
		name = "rt5734",
		of_match_table = rt5734_dt_match,
	},
	.probe = rt5734_spi_probe,
	.remove = rt5734_spi_remove,
};
module_spi_driver(rt5734_spi_driver);

MODULE_DESCRIPTION("rt5734 regulator driver");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("gene_chen <gene_chen@richtek.com>");
MODULE_VERSION("1.0.0");
