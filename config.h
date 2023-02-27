#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "gpio_util.h"
#include "i2c_util.h"

#define ONBOARD_LED 25
#define LED0 6
#define LED1 7
#define BTN0 15
#define BTN1 14
#define BTN2 13
#define BTN3 12
#define BTN4 11
#define I2C_SDA_PIN 16
#define I2C_SCL_PIN 17
#define I2C_READ_TIMEOUT_MICRO_SEC 100000
#define TCN75A_DEFAULT_ADDR 0x48
#define AMBIENT_TEMP_REG 0b00
#define SENSOR_CONFIG_REG 0b01
#define TEMP_HYST_MIN_REG 0b10
#define TEMP_SET_MAX_REG 0b11
#define TCN75A_BAUDRATE (400 * 1000)
#define BLINK_LED_DELAY 500

#define NUMBER_OF_GPIOS 8
#define NUMBER_OF_BTNS 8
#define NUMBER_OF_I2C 1
#define SHUTDOWN_MASK 0b00000001
#define ALERT_MODE_MASK 0b00000010
#define ALERT_POLARITY_MASK 0b00000100
#define FAULT_QUEUE_MASK 0b00011000
#define ADC_RESOLUTION_MASK 0b01100000
#define ONE_SHOT_MASK 0b10000000


extern const GpioConfig proj_gpio[NUMBER_OF_GPIOS];
extern const I2CConfig proj_i2c[NUMBER_OF_I2C];
#endif