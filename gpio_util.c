#include "gpio_util.h"

void set_gpio(const GpioConfig *gpio, size_t len) {
  // Loop through the length of the GpioConfig array
  for (size_t i = 0; i < len; i++) {
    // Initialize the GPIO specified by the pin number in the current GpioConfig
    // structure
    gpio_init(gpio[i].pin_number);
    // Set the direction of the GPIO specified by the pin number and direction
    // in the current GpioConfig structure
    gpio_set_dir(gpio[i].pin_number, gpio[i].pin_dir);
  }
}

void enable_irq(const GpioConfig *gpio, size_t len) {
  for (size_t i = 0; i < len; i++) {
    if (gpio[i].has_irq) {
      gpio_set_irq_enabled(gpio[i].pin_number,
                           GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);
      irq_set_enabled(IO_IRQ_BANK0, true);
    }
  }
}

void disable_irq(const GpioConfig *gpio, size_t len) {
    for (size_t i = 0; i < len; i++) {
    if (gpio[i].has_irq) {
      gpio_set_irq_enabled(gpio[i].pin_number,
                           GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, false);
      irq_set_enabled(IO_IRQ_BANK0, false);
    }
  }
}