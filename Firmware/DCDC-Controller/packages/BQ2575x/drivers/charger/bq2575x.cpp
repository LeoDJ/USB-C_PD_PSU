#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/charger.h>

#include "../../src/BQ2575x.h"

#define DT_DRV_COMPAT ti_bq2575x

LOG_MODULE_REGISTER(bq2575x, CONFIG_CHARGER_LOG_LEVEL);

struct bq2575x_config {
    struct i2c_dt_spec i2c;
    BQ2575x bq2575x;
};

static int bq25180_get_prop(const struct device* dev, charger_prop_t prop, union charger_propval* val) {

    return -ENOTSUP;
}

static int bq25180_set_prop(const struct device* dev, charger_prop_t prop, const union charger_propval* val) {

    return -ENOTSUP;
}

static const struct charger_driver_api bq2515x_api = {
	.get_property = bq25180_get_prop,    
	.set_property = bq25180_set_prop,
	.charge_enable = nullptr,           // TODO
};

static int bq2515x_init(const struct device *dev) {
    
    return 0;
}

#define CHARGER_BQ2515x_INIT(inst)                                                                            \
    static const struct i2c_dt_spec bq_i2c_##inst = I2C_DT_SPEC_INST_GET(inst);                               \
                                                                                                              \
    static int bq_i2c_read_##inst(uint16_t devAddr, uint8_t regStart, uint8_t* buf, size_t numBytes) {        \
        /* TODO: assert devAddr == spec->addr? */                                                             \
        return i2c_write_read_dt(&bq_i2c_##inst, &regStart, 1, buf, numBytes);                                \
    }                                                                                                         \
                                                                                                              \
    static int bq_i2c_write_##inst(uint16_t devAddr, uint8_t regStart, const uint8_t* buf, size_t numBytes) { \
        /* TODO: assert devAddr == spec->addr? */                                                             \
        uint8_t tx_buf[numBytes + 1] = {0};                                                                   \
        tx_buf[0] = regStart;                                                                                 \
        memcpy(tx_buf + 1, buf, numBytes);                                                                    \
        return i2c_write_dt(&bq_i2c_##inst, tx_buf, numBytes + 1);                                            \
    }                                                                                                         \
                                                                                                              \
    static const struct bq2575x_config bq25180_config_##inst = {                                              \
        .i2c = bq_i2c_##inst,                                                                                 \
        .bq2575x = BQ2575x(bq_i2c_read_##inst, bq_i2c_write_##inst),                                          \
    };                                                                                                        \
                                                                                                              \
    DEVICE_DT_INST_DEFINE(inst, bq2515x_init, NULL, NULL, &bq25180_config_##inst, POST_KERNEL,                \
                          CONFIG_CHARGER_INIT_PRIORITY, &bq2515x_api);

DT_INST_FOREACH_STATUS_OKAY(CHARGER_BQ2515x_INIT)