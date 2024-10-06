#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/init.h>
#include <linux/hwmon.h>
#include <linux/hwmon-sysfs.h>
#include <linux/device.h>


struct emul_data {
	struct platform_device *pdev;
	int temperature;
	int fan_speed;
};

static ssize_t temp_label_show(struct device *dev,
				      struct device_attribute *attr,
				      char *buf)
{
	return sprintf(buf, "CPU Package\n");
}


static ssize_t fan_label_show(struct device *dev,
				      struct device_attribute *attr,
				      char *buf)
{
	return sprintf(buf, "CPU Fan\n");
}

static ssize_t temperature_show(struct device *dev,
				      struct device_attribute *attr,
				      char *buf)
{
	struct emul_data *emul_data = dev_get_drvdata(dev);

	return sprintf(buf, "%d\n", emul_data->temperature);
}

static ssize_t fan_speed_show(struct device *dev,
				   struct device_attribute *attr, char *buf)
{
	struct emul_data *emul_data = dev_get_drvdata(dev);

	return sprintf(buf, "%d\n", emul_data->fan_speed);
}



static SENSOR_DEVICE_ATTR_RO(temp1_input, temperature, 0);
static SENSOR_DEVICE_ATTR_RO(temp1_label, temp_label, 0);
static SENSOR_DEVICE_ATTR_RO(fan1_input, fan_speed, 0);
static SENSOR_DEVICE_ATTR_RO(fan1_label, fan_label, 0);

static struct attribute *emulmon_attrs[] = {
	&sensor_dev_attr_temp1_input.dev_attr.attr,
	&sensor_dev_attr_temp1_label.dev_attr.attr,
	&sensor_dev_attr_fan1_input.dev_attr.attr,
	&sensor_dev_attr_fan1_label.dev_attr.attr,
	NULL
};

ATTRIBUTE_GROUPS(emulmon);

static int emulmon_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct device *hwmon_dev;
	struct emul_data *emul_data;

	emul_data = devm_kzalloc(dev, sizeof(*emul_data), GFP_KERNEL);
	if (!emul_data)
		return -ENOMEM;

	emul_data->pdev = pdev;
	emul_data->temperature = 34500;
	emul_data->fan_speed = 1650;

	hwmon_dev = devm_hwmon_device_register_with_groups(dev, pdev->name,
							   emul_data, emulmon_groups);
	return PTR_ERR_OR_ZERO(hwmon_dev);
}

static struct platform_driver emulmon_driver = {
    .driver = {
        .name = "emulmon",
    },
    .probe = emulmon_probe,
};


static struct platform_device *emulmon_device;

static int __init emulmon_init(void)
{
    int ret;

    ret = platform_driver_register(&emulmon_driver);
	if (ret < 0) return ret;

	emulmon_device = platform_device_register_simple("emulmon", -1, NULL, 0);
	if (IS_ERR(emulmon_device)) {
		ret = PTR_ERR(emulmon_device);
		platform_driver_unregister(&emulmon_driver);
	}

	return ret;
}

static void __exit emulmon_exit(void)
{
    platform_driver_unregister(&emulmon_driver);
    platform_device_unregister(emulmon_device);
}

module_init(emulmon_init);
module_exit(emulmon_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("MrMati");
MODULE_DESCRIPTION("configurable hwmon device emulation");