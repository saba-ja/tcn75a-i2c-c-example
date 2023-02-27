#include "core1.h"

#include <stdio.h>

#include "config.h"
#include "debounce.h"
#include "globals.h"
#include "gpio_util.h"
#include "menu_handler.h"
#include "pico/multicore.h"
#include "pico/stdlib.h"


void core1_entry() {
  uint32_t request = 0xFFFFFFFF;

  while (1) {
    bool has_alert_N = !gpio_get(ALERT_GP);  // Assuming active low
    gpio_put(LED1, has_alert_N);             // red
    gpio_put(LED0, !has_alert_N);            // green

    if (multicore_fifo_rvalid()) {
      uint32_t request = multicore_fifo_pop_blocking();
      handle_request(request);
    }
  }
}

void handle_request(uint32_t request) {
  switch (request) {
    case SCAN_I2C_BUS:
      show_landing_page();
      scan_i2c_bus(i2c, I2C_READ_TIMEOUT_MICRO_SEC);
      multicore_fifo_drain();
      break;
    case SHOW_CONFIG:
      handle_show_config();
      break;
    case SHOW_DEV_ID:
      handle_show_dev_id();
      break;
    case SHOW_ALERT_MENU:
      handle_show_alert_menu();
      break;
    default:
      // Handle unknown request
      break;
  }
}

void handle_show_config() {
  multicore_fifo_push_blocking(DISABLE_IRQ);
  uint32_t user_config_result = show_config_menu();
  uint8_t curr_config_result = read_config(i2c, dev_addr);
  uint8_t new_config_result = 0x0;
  bool has_new_change = true;
  if (user_config_result & (1 << 16)) {
    // No change - do nothing
    has_new_change = false;
  } else if (user_config_result & (1 << 31)) {
    // setting shutdown mode
    new_config_result = (curr_config_result & 0b11111110) | ((user_config_result & 0b00000001));
  } else if (user_config_result & (1 << 30)) {
    // setting comp/int mode
    new_config_result = (curr_config_result & 0b11111101) | ((user_config_result & 0b00000010));
  } else if (user_config_result & (1 << 29)) {
    // setting alert polarity
    new_config_result = (curr_config_result & 0b11111011) | ((user_config_result & 0b00000100));
  } else if (user_config_result & (1 << 28)) {
    // setting fault queue mode
    new_config_result = (curr_config_result & 0b11100111) | ((user_config_result & 0b00011000));
  } else if (user_config_result & (1 << 27)) {
    // setting ADC resolution
    new_config_result = (curr_config_result & 0b10011111) | ((user_config_result & 0b01100000));
  } else if (user_config_result & (1 << 26)) {
    // setting one-shot mode
    new_config_result = (curr_config_result & 0b01111111) | ((user_config_result & 0b10000000));
  } else {
    // unknown
    has_new_change = false;
  }

  if (has_new_change) {
    write_config(i2c, dev_addr, new_config_result);
  }

  show_landing_page();
  uint8_t config_result = read_config(i2c, dev_addr);
  printf("Sensor Config Status\n");
  printf("curr 0x%x\n", curr_config_result);
  printf("new 0x%x\n", new_config_result);
  printf("user 0x%x\n", user_config_result);
  parse_config(config_result);
  multicore_fifo_push_blocking(ENABLE_IRQ);
}

void handle_show_dev_id() {
  multicore_fifo_push_blocking(DISABLE_IRQ);
  uint8_t addr = show_dev_change_menu(TCN75A_DEFAULT_ADDR);
  show_landing_page();
  multicore_fifo_push_blocking(ENABLE_IRQ);

  if (addr != 0) {
    uint8_t rxdata;
    int ret = check_addr(i2c, addr, &rxdata, I2C_READ_TIMEOUT_MICRO_SEC);

    if (ret > 0) {
      dev_addr = addr;
      printf("[SUCCESS] Dev ID changed to 0x%x\n", addr);
    } else {
      printf("[WARNING] Could not communicate with Dev ID 0x%x\n", addr);
    }
  }

  multicore_fifo_drain();
}

void handle_show_alert_menu() {
  multicore_fifo_push_blocking(DISABLE_IRQ);
  uint8_t buf[2];
  uint32_t result = show_alert_menu(buf);
  show_landing_page();

  if ((result & READ_TEMP_HYST_LIMIT) == READ_TEMP_HYST_LIMIT) {
    read_temp_hyst_limit(i2c, dev_addr);
  } else if ((result & READ_TEMP_SET_LIMIT) == READ_TEMP_SET_LIMIT) {
    read_temp_set_limit(i2c, dev_addr);
  } else if ((result & WRITE_TEMP_HYST_LIMIT) == WRITE_TEMP_HYST_LIMIT) {
    write_temp_hyst_limit(i2c, dev_addr, buf[0], buf[1]);
  } else if ((result & WRITE_TEMP_SET_LIMIT) == WRITE_TEMP_SET_LIMIT) {
    write_temp_set_limit(i2c, dev_addr, buf[0], buf[1]);
  }

  multicore_fifo_push_blocking(ENABLE_IRQ);
}
