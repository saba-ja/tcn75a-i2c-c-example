#include "config.h"

const GpioConfig proj_gpio[NUMBER_OF_GPIOS] = {
    {BTN0, GPIO_IN, true},   {BTN1, GPIO_IN, true},
    {BTN2, GPIO_IN, true},   {BTN3, GPIO_IN, true},
    {BTN4, GPIO_IN, true},   {LED0, GPIO_OUT, false},
    {LED1, GPIO_OUT, false}, {ONBOARD_LED, GPIO_OUT, false},
};

const I2CConfig proj_i2c[NUMBER_OF_I2C] = {{.sda_pin_number = I2C_SDA_PIN,
                                            .scl_pin_number = I2C_SCL_PIN,
                                            .has_pullup = true}};