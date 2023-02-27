#include <stdio.h>

#include "config.h"
#include "debounce.h"
#include "globals.h"
#include "gpio_util.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "i2c_util.h"
#include "menu_handler.h"
#include "pico/binary_info.h"
#include "pico/multicore.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "util.h"

/**
@brief Callback function for GPIO interrupts on button presses
This function handles the GPIO interrupt for button presses and debounces the
input. It determines which button was pressed and performs the corresponding
action, such as scanning the I2C bus or showing device information. The function
keeps track of the button state and only triggers the
action when the button is stable. If the button is unstable or the input is
invalid, the function returns without taking any action.

@param gpio The GPIO pin number that triggered the interrupt
@param events The type of interrupt event (e.g., GPIO_IRQ_EDGE_RISE,
GPIO_IRQ_EDGE_FALL)
@return void
*/
void gpio_callback(uint gpio, uint32_t events) {

}
