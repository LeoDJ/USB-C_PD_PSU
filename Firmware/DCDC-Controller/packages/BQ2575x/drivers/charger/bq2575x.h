#pragma once

#include <zephyr/drivers/i2c.h>
#include "../../src/BQ2575x.h"

struct bq2575x_config {
    struct i2c_dt_spec i2c;
    BQ2575x bq2575x;
};

#define CHARGER_PROP_BQ_INST (CHARGER_PROP_CUSTOM_BEGIN+0)