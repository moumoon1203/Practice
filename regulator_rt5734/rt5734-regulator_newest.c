#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/spi/spi.h>
#include <linux/platform_device.h>
#include <linux/regulator/driver.h>
#include <linux/regulator/of_regulator.h>

#define RT5734_NUM_DVS_MODE    3

struct rt5734_drv_data {
	struct spi_device *spi;
	struct device *dev;
	struct regulator_dev *rdev;
};

#if 0
static int rt5734_spi_block_read(struct spi_device *spi, uint8_t reg,
								 uint8_t *rx_buf, uint8_t size)
{
	uint8_t tx_buf[3] = {0};
	int ret = 0;

	if (!rx)
		return -EINVAL;
	tx_buf[0] = 0xc0;
	tx_buf[1] = reg;
	tx_buf[2] = size; /* packet length */
	ret = spi_write_then_read(spi, tx_buf, ARRAY_SIZE(tx_buf), rx_buf, size);
	if (ret < 0)
		return ret;
	return 0;
}

static int rt5734_spi_block_write(struct spi_device *spi, uint8_t reg,
								  uint8_t *tx, uint8_t size)
{
	uint8_t *tx_buf;
	int ret = 0;

	tx_buf = kzalloc(3 + size, GFP_KERNEL);
	if (!tx_buf)
		return -ENOMEM;
	tx_buf[0] = 0x40;
	tx_buf[1] = reg;
	tx_buf[2] = size;
	memcpy(tx_buf + 3, tx, size);
	ret = spi_write(spi, tx_buf, 3 + size);
	kfree(tx_buf);
	if (ret < 0)
		return ret;
	return 0;
}
#endif

static int rt5734_spi_read(struct spi_device *spi, uint8_t reg)
{
	uint8_t tx_buf[3] = {0};
	int ret = 0;

	tx_buf[0] = 0xc0;
	tx_buf[1] = reg;
	tx_buf[2] = 0x01; /* packet length */
	ret = spi_write_then_read(spi, tx_buf, ARRAY_SIZE(tx_buf), tx_buf, 1);
	if (ret < 0)
		return ret;
	return tx_buf[0];
}


static int rt5734_spi_write(struct spi_device *spi, uint8_t reg, uint8_t data)
{
	uint8_t tx_buf[4] = {0};

	tx_buf[0] = 0x40;
	tx_buf[1] = reg;
	tx_buf[2] = 0x01; /* packet length */
	tx_buf[3] = data;
	return spi_write(spi, tx_buf, ARRAY_SIZE(tx_buf));
}

static int rt5734_enable(struct regulator_dev *rdev)
{
	struct rt5734_drv_data *rdd = rdev_get_drvdata(rdev);
	int ret = 0;

	ret = rt5734_spi_write(rdd->spi, 0x49, 0x01);
	if (ret < 0)
		return ret;

	dev_info(rdd->dev, "%s\n", __func__);
	return ret;
}
static int rt5734_disable(struct regulator_dev *rdev)
{	
	struct rt5734_drv_data *rdd = rdev_get_drvdata(rdev);
	int ret = 0;

	ret = rt5734_spi_write(rdd->spi, 0x49, 0x00);
	if (ret < 0)
		return ret;

	dev_info(rdd->dev, "%s successfully\n", __func__);
	return ret;
}

static int rt5734_get_voltage_sel(struct regulator_dev *rdev)
{
	struct rt5734_drv_data *rdd = rdev_get_drvdata(rdev);
	int ret = 0;

	/* check DVS0 Enable */ 
	ret = rt5734_spi_write(rdd->spi, 0x49, 0x01);
	ret = rt5734_spi_read(rdd->spi, 0x49);
	if (ret < 0)
		return ret;
	if (ret == 0x1)
		dev_info(rdd->dev, "DVS0 Enable\n");
	
	ret = rt5734_spi_read(rdd->spi, 0x48);
	if (ret < 0)
		return ret;

	dev_info(rdd->dev, "%s successfully\n", __func__);
	return ret;
}

static int rt5734_set_voltage_sel(struct regulator_dev *rdev, unsigned selector)
{
	struct rt5734_drv_data *rdd = rdev_get_drvdata(rdev);
	int ret = 0;
	
	/* check DVS0 Enable */
	ret = rt5734_spi_read(rdd->spi, 0x49);
	if (ret < 0)
		return ret;
	if (ret == 0x1)
		dev_info(rdd->dev, "DVS0 Enable\n");
	
	/* get now rt5734 voltage */
	ret = rt5734_get_voltage_sel(rdev);
	if (ret < 0)
		return ret;
	
	ret &= 0x0;
	ret |= selector;		
	ret = rt5734_spi_write(rdd->spi, 0x48, ret);

	dev_info(rdd->dev, "%s\n", __func__);
	return 0;
}

static int rt5734_spi_config(struct regulator_dev *rdev)
{
	struct rt5734_drv_data *rdd = rdev_get_drvdata(rdev);
	int ret = 0;

	ret = rt5734_spi_read(rdd->spi, 0x23);
	if (ret < 0) {
		dev_err(rdd->dev, "read IO_SPICFG failed\n");
		return -ENODEV;
	}

	if (ret & (0x1 << 0))
		dev_info(rdd->dev, "IO_I2C_SPEED : High speed mode\n");
	else	
		dev_info(rdd->dev, "IO_I2C_SPEED : Fast mode\n");
		
	if (ret & (0x1 << 1))
		dev_info(rdd->dev, "IO_SDA_SLEWFLTR : Enable slew filtering\n");
	else	
		dev_info(rdd->dev, "IO_SDA_SLEWFLTR : Disable slew filtering\n");
	
	dev_info(rdd->dev, "%s\n", __func__);
	return 0;
}

static struct regulator_ops rt5734_ops = {
	.enable				= rt5734_enable,
	.disable			= rt5734_disable,
	.get_voltage_sel	= rt5734_get_voltage_sel,
	.set_voltage_sel	= rt5734_set_voltage_sel,
};

static const struct regulator_desc regulator = {
	.name = "RT5734_VOUT",
	.id = 0,
	.n_voltages = RT5734_NUM_DVS_MODE,
	.ops = &rt5734_ops,
	.type = REGULATOR_VOLTAGE,
	.owner = THIS_MODULE,
 };

static int rt5734_spi_probe(struct spi_device *spi)
{
	struct rt5734_drv_data *rdd;
	struct regulator_init_data *reg_data;
	struct regulator_config config = { };
	int ret = 0;

	rdd = devm_kzalloc(&spi->dev, sizeof(*rdd), GFP_KERNEL);
	if (!rdd)
		return -ENOMEM;
	rdd->spi = spi;
	rdd->dev = &spi->dev;
	
	reg_data = of_get_regulator_init_data(&spi->dev, spi->dev.of_node);

	config.dev = &spi->dev;	
	config.init_data = reg_data;
	config.driver_data = rdd;
	
	rdd->rdev = regulator_register(&regulator, &config);
	if (IS_ERR(rdd->rdev)) {
		ret = PTR_ERR(rdd->rdev);
		dev_err(rdd->dev, "regulator init failed (%d)\n", ret);
		return ret;
	}

	spi_set_drvdata(spi, rdd);

	spi->bits_per_word = 8;
	ret = spi_setup(spi);
	if (ret < 0) {
		dev_err(&spi->dev, "setup spi fail\n");
		goto out_chip_id;
	}

	/* check chip id */
	ret = rt5734_spi_read(spi, 0x01);
	if (ret < 0)
		goto out_chip_id;
	if (ret != 0x01) {
		dev_err(&spi->dev, "chip id = %d\n", ret);
		ret = -ENODEV;
		goto out_chip_id;
	}

	/* show spi info */
	rt5734_spi_config(rdd->rdev);

	/* test rt5734_get_voltage_sel */
	ret = rt5734_get_voltage_sel(rdd->rdev);
	if (ret < 0)

	dev_info(&spi->dev, "%s successfully\n", __func__);
	return 0;
out_chip_id:
	devm_kfree(&spi->dev, rdd);
	return ret;
}

static int rt5734_spi_remove(struct spi_device *spi)
{
	struct rt5734_drv_data *rdd = spi_get_drvdata(spi);

	dev_info(rdd->dev, "%s successfully\n", __func__);
	return 0;
}

static const struct of_device_id rt5734_dt_match[] = {
	{ .compatible = "richtek,rt5734" },
	{},
};
MODULE_DEVICE_TABLE(dt, rt5734_dt_match);

static struct spi_driver rt5734_spi_driver = {
	.driver = {
		.name = "rt5734",
		.of_match_table = rt5734_dt_match,
	},
	.probe = rt5734_spi_probe,
	.remove = rt5734_spi_remove,
};
module_spi_driver(rt5734_spi_driver);

MODULE_DESCRIPTION("rt5734 regulator driver");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("gene_chen <gene_chen@richtek.com>");
MODULE_VERSION("1.0.0");
