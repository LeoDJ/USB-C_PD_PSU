#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/charger.h>

#include "bq2575x.h"

#define DT_DRV_COMPAT ti_bq2575x

LOG_MODULE_REGISTER(bq2575x, CONFIG_CHARGER_LOG_LEVEL);

static int bq2575x_get_prop(const struct device* dev, charger_prop_t prop, union charger_propval* val) {
    BQ2575x *bq = &((bq2575x_config*)dev->config)->bq2575x;

    switch(prop) {
        case CHARGER_PROP_INPUT_REGULATION_VOLTAGE_UV:  val->input_voltage_regulation_voltage_uv =  bq->getValue<BQ2575x::VAC_DPM>();    break;
        case CHARGER_PROP_INPUT_REGULATION_CURRENT_UA:  val->input_current_regulation_current_ua =  bq->getValue<BQ2575x::IAC_DPM>();    break;
        case CHARGER_PROP_CONSTANT_CHARGE_VOLTAGE_UV:   val->const_charge_voltage_uv =              bq->getValue<BQ2575x::VOUT_REG>();   break;
        case CHARGER_PROP_CONSTANT_CHARGE_CURRENT_UA:   val->const_charge_current_ua =              bq->getValue<BQ2575x::IOUT_REG>();   break;
        case CHARGER_PROP_BQ_INST:                      val->system_voltage_notification = (uint32_t)bq; /* hacky test stuff */                      break;
        default: return -ENOTSUP;
    }

    return 0;
}

static int bq2575x_set_prop(const struct device* dev, charger_prop_t prop, const union charger_propval* val) {

    BQ2575x *bq = &((bq2575x_config*)dev->config)->bq2575x;

    switch(prop) {
        case CHARGER_PROP_INPUT_REGULATION_VOLTAGE_UV:  bq->setValue<BQ2575x::VAC_DPM>(val->input_voltage_regulation_voltage_uv);   break;
        case CHARGER_PROP_INPUT_REGULATION_CURRENT_UA:  bq->setValue<BQ2575x::IAC_DPM>(val->input_current_regulation_current_ua);   break;
        case CHARGER_PROP_CONSTANT_CHARGE_VOLTAGE_UV:   bq->setValue<BQ2575x::VOUT_REG>(val->const_charge_voltage_uv);              break;
        case CHARGER_PROP_CONSTANT_CHARGE_CURRENT_UA:   bq->setValue<BQ2575x::IOUT_REG>(val->const_charge_current_ua);              break;
        default: return -ENOTSUP;
    }

    return 0;
}

static int bq2575x_charge_enable(const struct device *dev, const bool enable) {
    BQ2575x *bq = &((bq2575x_config*)dev->config)->bq2575x;

    bq->setValue<BQ2575x::EN_CHG>(enable);

    return 0;
}

static const struct charger_driver_api bq2575x_api = {
	.get_property = bq2575x_get_prop,    
	.set_property = bq2575x_set_prop,
	.charge_enable = bq2575x_charge_enable,
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
    static const struct bq2575x_config bq2575x_config_##inst = {                                              \
        .i2c = bq_i2c_##inst,                                                                                 \
        .bq2575x = BQ2575x(bq_i2c_read_##inst, bq_i2c_write_##inst),                                          \
    };                                                                                                        \
                                                                                                              \
    DEVICE_DT_INST_DEFINE(inst, bq2515x_init, NULL, NULL, &bq2575x_config_##inst, POST_KERNEL,                \
                          CONFIG_CHARGER_INIT_PRIORITY, &bq2575x_api);

DT_INST_FOREACH_STATUS_OKAY(CHARGER_BQ2515x_INIT)