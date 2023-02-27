#include "config.h"

const GpioConfig proj_gpio[NUMBER_OF_GPIOS] = {
    {BTN0, GPIO_IN, true, false},    {BTN1, GPIO_IN, true, false},
    {BTN2, GPIO_IN, true, false},    {BTN3, GPIO_IN, true, false},
    {BTN4, GPIO_IN, true, false},    {LED0, GPIO_OUT, false, false},
    {LED1, GPIO_OUT, false, false},  {ONBOARD_LED, GPIO_OUT, false, false},
    {ALERT_GP, GPIO_IN, false, true}};

const I2CConfig proj_i2c[NUMBER_OF_I2C] = {{.sda_pin_number = I2C_SDA_PIN,
                                            .scl_pin_number = I2C_SCL_PIN,
                                            .has_pullup = true}};