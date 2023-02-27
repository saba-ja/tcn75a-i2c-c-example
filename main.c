#include <stdio.h>

#include "config.h"
#include "debounce.h"
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

extern void core1_entry();
extern void gpio_callback();

bool enable_read_temp = false;
i2c_inst_t *i2c = i2c0;
uint8_t dev_addr = TCN75A_DEFAULT_ADDR;

// Struct for button state
volatile BtnState btns[NUMBER_OF_BTNS] = {{BTN0, false, false, 0, 0},
                                          {BTN1, false, false, 0, 0},
                                          {BTN2, false, false, 0, 0},
                                          {BTN3, false, false, 0, 0},
                                          {BTN4, false, false, 0, 0}};

int main() {
  stdio_init_all();
  set_gpio(proj_gpio, NUMBER_OF_GPIOS);
  enable_irq(proj_gpio, NUMBER_OF_GPIOS);
  gpio_set_irq_callback(&gpio_callback);
  i2c_init(i2c, TCN75A_BAUDRATE);
  set_i2c(proj_i2c, NUMBER_OF_I2C);

  multicore_launch_core1(core1_entry);
  print_ambient_temperature(i2c, dev_addr);
  while (1) {
    if (multicore_fifo_rvalid()) {
      uint32_t request = multicore_fifo_pop_blocking();
      if (request == DISABLE_IRQ) {
        disable_irq(proj_gpio, NUMBER_OF_GPIOS);
      } else if (request == ENABLE_IRQ) {
        enable_irq(proj_gpio, NUMBER_OF_GPIOS);
      }
    }
    if (enable_read_temp) {
      print_ambient_temperature(i2c, dev_addr);
    }
    gpio_put(ONBOARD_LED, 1);
    sleep_ms(BLINK_LED_DELAY);
    gpio_put(ONBOARD_LED, 0);
    sleep_ms(BLINK_LED_DELAY);
  }
}