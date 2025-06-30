#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/charger.h>

#include "util.h"

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   1000

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

static const struct gpio_dt_spec dcdc_en_ = GPIO_DT_SPEC_GET(DT_NODELABEL(dcdc_en), gpios);

const struct device *dcdc = DEVICE_DT_GET(DT_NODELABEL(bq2575x));

void i2c_scan(void);
void i2c_dump(uint16_t addr, uint8_t regStart, uint8_t regStop);

int main(void)
{
	int ret;
	bool led_state = true;

	gpio_pin_configure_dt(&dcdc_en_, GPIO_OUTPUT_INACTIVE);

	if (device_is_ready(dcdc)) {
		charger_propval val;
		charger_get_prop(dcdc, CHARGER_PROP_CONSTANT_CHARGE_VOLTAGE_UV, &val);
		printf("CC mV: %d\n", val.const_charge_voltage_uv);
	}

	if (!gpio_is_ready_dt(&led)) {
		return 0;
	}

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 0;
	}

	while (1) {
		ret = gpio_pin_toggle_dt(&led);
		if (ret < 0) {
			return 0;
		}

		led_state = !led_state;
		printf("LED state: %s\n", led_state ? "ON" : "OFF");
		k_msleep(SLEEP_TIME_MS);

		// i2c_scan();
		i2c_dump(0x6B, 0, 0xFF);

	}
	return 0;
}

// I2C scan code from: https://github.com/iFransL/i2c-scanner
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/i2c.h>

#define CONFIG_I2C_SCAN_ADDR_START 8
#define CONFIG_I2C_SCAN_ADDR_STOP  127

/* Check if devicetree node identifier with the alias "my-i2c" is defined						*/
/* If not defined use the default defined node for the arduino_i2c pins on the development kit 	*/
#if DT_NODE_HAS_STATUS(DT_ALIAS(my_i2c), okay)
	#define I2C_NODE DT_ALIAS(my_i2c)
	#define I2C_DEV	DT_PROP(I2C_NODE, label)
#else
	#define I2C_NODE DT_NODELABEL(arduino_i2c)
	#if DT_NODE_HAS_STATUS(I2C_NODE, okay)
		#define I2C_DEV	DT_PROP(I2C_NODE, label)
	#else
		#error "Arduino_i2c not defined, specify the I2C bus to use with the alias "my-i2c""
		#define I2C_DEV ""
	#endif
#endif

void i2c_scan(void)
{
	const struct device *i2c_dev;

	k_sleep(K_SECONDS(1));
	printf("*** I2C scanner started.                    ***\nBoard name      : %s\n", CONFIG_BOARD);

	// i2c_dev = device_get_binding(I2C_DEV);
	// 	if (!i2c_dev) {
	// 	printf("I2C: Device driver not found.\n");
	// 	return;
	// }

	i2c_dev = DEVICE_DT_GET(DT_NODELABEL(gpio_i2c0));

	printf("I2C Port        : %s \n",I2C_DEV);
	printf("Clock Frequency : %d \n",DT_PROP(I2C_NODE, clock_frequency));

#ifndef CONFIG_PINCTRL_NRF
	// From NCS2.0.0 pin-control is used, not possible to read out the used pins from I2C_NODE 
	// printf("SDA-PIN         : %d \n",DT_PROP(I2C_NODE, sda_pin));
	// printf("SCL-PIN         : %d \n",DT_PROP(I2C_NODE, scl_pin));
#endif

	printf("\n    | 0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0a 0x0b 0x0c 0x0d 0x0e 0x0f |\n");
	printf(  "----|---------------------------------------------------------------------------------");
	
	uint8_t error = 0u;
	uint8_t dst;
	uint8_t i2c_dev_cnt = 0;
	struct i2c_msg msgs[1];
	msgs[0].buf = &dst;
	msgs[0].len = 1U;
	msgs[0].flags = I2C_MSG_WRITE | I2C_MSG_STOP;

	/* Use the full range of I2C address for display purpose */	
	for (uint16_t x = 0; x <= 0x7f; x++) {
		/* New line every 0x10 address */
		if (x % 0x10 == 0) {
			printf("|\n0x%02x| ",x);	
		}
		/* Range the test with the start and stop value configured in the kconfig */
		if (x >= CONFIG_I2C_SCAN_ADDR_START && x <= CONFIG_I2C_SCAN_ADDR_STOP)	{	
			/* Send the address to read from */
			error = i2c_transfer(i2c_dev, &msgs[0], 1, x);
				/* I2C device found on current address */
				if (error == 0) {
					printf("0x%02x ",x);
					i2c_dev_cnt++;
				}
				else {
					printf(" --  ");
				}
		} else {
			/* Scan value out of range, not scanned */
			printf("     ");
		}
	}
	printf("|\n");
	printf("\nI2C device(s) found on the bus: %d\nScanning done.\n\n", i2c_dev_cnt);
	printf("Find the registered I2C address on: https://i2cdevices.org/addresses\n\n");
}


void i2c_dump(uint16_t addr, uint8_t regStart, uint8_t regStop) {
	uint8_t buf[regStop - regStart + 1] = {0};
	const struct device *i2c_dev;
	i2c_dev = DEVICE_DT_GET(DT_NODELABEL(gpio_i2c0));

	uint8_t dst;
	struct i2c_msg msg = {
		.buf = &dst,
		.len = 1,
		.flags = I2C_MSG_READ | I2C_MSG_STOP,
	};

	i2c_write_read(i2c_dev, addr, &regStart, 1, buf, sizeof(buf));

	printHex(buf, sizeof(buf));
}