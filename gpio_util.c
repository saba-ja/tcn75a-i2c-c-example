#include "gpio_util.h"

#include "hardware/gpio.h"

/**
 * Initializes and sets the direction of the GPIO pins based on the provided
 * GpioConfig array. Additionally, enables the internal pull-up resistors for
 * the pins that have `has_pullup` set to `true`.
 *
 * @param gpio Pointer to the GpioConfig array.
 * @param len Length of the GpioConfig array.
 */
void set_gpio(const GpioConfig *gpio, size_t len) {
  // Loop through the length of the GpioConfig array
    // Initialize the GPIO specified by the pin number in the current GpioConfig
    // structure
    // Set the direction of the GPIO specified by the pin number and direction
    // in the current GpioConfig structure

}

/**
 * Enables interrupts on the GPIO pins specified in the GpioConfig array that
 * have `has_irq` set to `true`. Also enables the GPIO IRQ bank.
 *
 * @param gpio Pointer to the GpioConfig array.
 * @param len Length of the GpioConfig array.
 */
void enable_irq(const GpioConfig *gpio, size_t len) {

}

/**
 * Disables interrupts on the GPIO pins specified in the GpioConfig array that
 * have `has_irq` set to `true`. Also disables the GPIO IRQ bank.
 *
 * @param gpio Pointer to the GpioConfig array.
 * @param len Length of the GpioConfig array.
 */
void disable_irq(const GpioConfig *gpio, size_t len) {

}