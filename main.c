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


// #define NUMBER_OF_GPIOS 8
// #define NUMBER_OF_BTNS 8
// #define NUMBER_OF_I2C 1
// #define SHUTDOWN_MASK 0b00000001
// #define ALERT_MODE_MASK 0b00000010
// #define ALERT_POLARITY_MASK 0b00000100
// #define FAULT_QUEUE_MASK 0b00011000
// #define ADC_RESOLUTION_MASK 0b01100000
// #define ONE_SHOT_MASK 0b10000000

// Define the pin numbers for the GPIOs used in the project
// static const uint ONBOARD_LED = 25;
// static const uint LED0 = 6;
// static const uint LED1 = 7;
// static const uint BTN0 = 15;
// static const uint BTN1 = 14;
// static const uint BTN2 = 13;
// static const uint BTN3 = 12;
// static const uint BTN4 = 11;
// static const uint I2C_SDA_PIN = 16;
// static const uint I2C_SCL_PIN = 17;
// static const uint I2C_READ_TIMEOUT_MICRO_SEC = 100000;
// static const uint8_t TCN75A_DEFAULT_ADDR = 0x48;
// static const uint8_t AMBIENT_TEMP_REG = 0b00;
// static const uint8_t SENSOR_CONFIG_REG = 0b01;
// static const uint8_t TEMP_HYST_MIN_REG = 0b10;
// static const uint8_t TEMP_SET_MAX_REG = 0b11;
// static const uint TCN75A_BAUDRATE = 400 * 1000;
// static const uint32_t BLINK_LED_DELAY = 500;  

// Struct for button state
volatile BtnState btns[NUMBER_OF_BTNS] = {{.but_pin = BTN0,
                                           .prev_state = false,
                                           .curr_state = false,
                                           .prev_time = 0,
                                           .curr_time = 0},
                                          {.but_pin = BTN1,
                                           .prev_state = false,
                                           .curr_state = false,
                                           .prev_time = 0,
                                           .curr_time = 0},
                                          {.but_pin = BTN2,
                                           .prev_state = false,
                                           .curr_state = false,
                                           .prev_time = 0,
                                           .curr_time = 0},
                                          {.but_pin = BTN3,
                                           .prev_state = false,
                                           .curr_state = false,
                                           .prev_time = 0,
                                           .curr_time = 0},
                                          {.but_pin = BTN4,
                                           .prev_state = false,
                                           .curr_state = false,
                                           .prev_time = 0,
                                           .curr_time = 0}};

// const GpioConfig proj_gpio[NUMBER_OF_GPIOS] = {
//     {BTN0, GPIO_IN, true},   {BTN1, GPIO_IN, true},
//     {BTN2, GPIO_IN, true},   {BTN3, GPIO_IN, true},
//     {BTN4, GPIO_IN, true},   {LED0, GPIO_OUT, false},
//     {LED1, GPIO_OUT, false}, {ONBOARD_LED, GPIO_OUT, false},
// };

// const I2CConfig proj_i2c[NUMBER_OF_I2C] = {{.sda_pin_number = I2C_SDA_PIN,
//                                             .scl_pin_number = I2C_SCL_PIN,
//                                             .has_pullup = true}};

// static bool enable_read_temp = false;
// static i2c_inst_t *i2c = i2c0;
// static uint8_t dev_addr = TCN75A_DEFAULT_ADDR;

// void read_temp_reg(uint8_t dev_addr, uint8_t reg_addr, uint8_t *buf,
//                    uint8_t nbytes) {
//   uint8_t *buf_tmp = 0x0;
//   reg_write(i2c, dev_addr, reg_addr, buf_tmp, 1);
//   reg_read(i2c, dev_addr, reg_addr, buf, nbytes);
// }

// void write_temp_reg(uint8_t dev_addr, uint8_t reg_addr, uint8_t *buf,
//                     uint8_t nbytes) {
//   reg_write(i2c, dev_addr, reg_addr, buf, nbytes);
// }

// void print_ambient_temperature() {
//   uint8_t nbytes = 2;
//   uint8_t tmp[2] = {0, 0};
//   read_temp_reg(dev_addr, AMBIENT_TEMP_REG, tmp, nbytes);
//   clear_screen();
//   printf("Ambient Temparature\n");
//   print_temp_table(tmp[0], tmp[1]);
// }

// void read_temp_hyst_limit() {
//   uint8_t nbytes = 2;
//   uint8_t tmp[2] = {0, 0};
//   read_temp_reg(dev_addr, TEMP_HYST_MIN_REG, tmp, nbytes);
//   printf("Temperature Hyst Limit\n");
//   print_temp_table(tmp[0], tmp[1]);
// }

// void write_temp_hyst_limit(uint8_t integer_part, uint8_t decimal_part) {
//   uint8_t nbytes = 2;
//   uint8_t tmp[2] = {integer_part, decimal_part};
//   write_temp_reg(dev_addr, TEMP_HYST_MIN_REG, tmp, nbytes);
//   printf("Temperature Hyst Limit\n");
//   read_temp_hyst_limit();
// }

// void read_temp_set_limit() {
//   uint8_t nbytes = 2;
//   uint8_t tmp[2] = {0, 0};
//   read_temp_reg(dev_addr, TEMP_SET_MAX_REG, tmp, nbytes);
//   printf("Temperature set Limit\n");
//   print_temp_table(tmp[0], tmp[1]);
// }

// void write_temp_set_limit(uint8_t integer_part, uint8_t decimal_part) {
//   uint8_t nbytes = 2;
//   uint8_t tmp[2] = {integer_part, decimal_part};
//   write_temp_reg(dev_addr, TEMP_SET_MAX_REG, tmp, nbytes);
//   printf("Temperature set Limit\n");
//   read_temp_set_limit();
// }


static bool enable_read_temp = false;
static i2c_inst_t *i2c = i2c0;
static uint8_t dev_addr = TCN75A_DEFAULT_ADDR;

// void read_temp_reg(uint8_t reg_addr, uint8_t *buf, uint8_t nbytes) {
//   reg_read(i2c, dev_addr, reg_addr, buf, nbytes);
// }

// void write_temp_reg(uint8_t reg_addr, uint8_t *buf, uint8_t nbytes) {
//   reg_write(i2c, dev_addr, reg_addr, buf, nbytes);
// }

// void read_temperature_registers(uint8_t reg_addr, uint8_t *buf, uint8_t nbytes,
//                                  const char *message) {
//   read_temp_reg(reg_addr, buf, nbytes);
//   printf("%s\n", message);
//   print_temp_table(buf[0], buf[1]);
// }

// void print_ambient_temperature() {
//   uint8_t nbytes = 2;
//   uint8_t tmp[2] = {0, 0};
//   clear_screen();
//   read_temperature_registers(AMBIENT_TEMP_REG, tmp, nbytes, "Ambient Temperature");
// }

// void read_temp_hyst_limit() {
//   uint8_t nbytes = 2;
//   uint8_t tmp[2] = {0, 0};
//   read_temperature_registers(TEMP_HYST_MIN_REG, tmp, nbytes, "Temperature Hyst Limit");
// }

// void write_temp_hyst_limit(uint8_t integer_part, uint8_t decimal_part) {
//   uint8_t nbytes = 2;
//   uint8_t tmp[2] = {integer_part, decimal_part};
//   write_temp_reg(TEMP_HYST_MIN_REG, tmp, nbytes);
//   read_temp_hyst_limit();
// }

// void read_temp_set_limit() {
//   uint8_t nbytes = 2;
//   uint8_t tmp[2] = {0, 0};
//   read_temperature_registers(TEMP_SET_MAX_REG, tmp, nbytes, "Temperature set Limit");
// }

// void write_temp_set_limit(uint8_t integer_part, uint8_t decimal_part) {
//   uint8_t nbytes = 2;
//   uint8_t tmp[2] = {integer_part, decimal_part};
//   write_temp_reg(TEMP_SET_MAX_REG, tmp, nbytes);
//   read_temp_set_limit();
// }

// void read_config() {
//   uint8_t nbytes = 1;
//   uint8_t tmp[1] = {0};
//   read_temp_reg(SENSOR_CONFIG_REG, tmp, nbytes);
//   printf("Sensore Config Status\n");
//   parse_config(tmp[0]);
// }

// void parse_config(uint8_t conf) {
//   printf("+--------------------+--------------+\n");
//   printf("|       Setting      |     Value    |\n");
//   printf("+--------------------+--------------+\n");
//   printf("| %-18s | ", "Shutdown:");
//   if ((conf & SHUTDOWN_MASK) == 0) {
//     printf("%-12s |\n", "Disable");
//   } else if ((conf & SHUTDOWN_MASK) == SHUTDOWN_MASK) {
//     printf("%-12s |\n", "Enable");
//   } else {
//     printf("%-12s |\n", "UNKNOWN");
//   }

//   printf("| %-18s | ", "Alert Mode:");
//   if ((conf & ALERT_MODE_MASK) >> 1 == 0) {
//     printf("%-12s |\n", "Comp");
//   } else if ((conf & ALERT_MODE_MASK) >> 1 == 1) {
//     printf("%-12s |\n", "Intr");
//   } else {
//     printf("%-12s |\n", "UNKNOWN");
//   }

//   printf("| %-18s | ", "Alert Polarity:");
//   if ((conf & ALERT_POLARITY_MASK) >> 2 == 0) {
//     printf("%-12s |\n", "Low");
//   } else if ((conf & ALERT_POLARITY_MASK) >> 2 == 1) {
//     printf("%-12s |\n", "High");
//   } else {
//     printf("%-12s |\n", "UNKNOWN");
//   }

//   printf("| %-18s | ", "Fault Queue:");
//   if ((conf & FAULT_QUEUE_MASK) >> 3 == 0) {
//     printf("%-12d |\n", 1);
//   } else if ((conf & FAULT_QUEUE_MASK) >> 3 == 1) {
//     printf("%-12d |\n", 2);
//   } else if ((conf & FAULT_QUEUE_MASK) >> 3 == 2) {
//     printf("%-12d |\n", 4);
//   } else if ((conf & FAULT_QUEUE_MASK) >> 3 == 3) {
//     printf("%-12d |\n", 6);
//   } else {
//     printf("%-12s |\n", "UNKNOWN");
//   }

//   printf("| %-18s | ", "ADC Resolution:");
//   if ((conf & ADC_RESOLUTION_MASK) >> 5 == 0) {
//     printf("%-12s |\n", "0.5C");
//   } else if ((conf & ADC_RESOLUTION_MASK) >> 5 == 1) {
//     printf("%-12s |\n", "0.25C");
//   } else if ((conf & ADC_RESOLUTION_MASK) >> 5 == 2) {
//     printf("%-12s |\n", "0.125C");
//   } else if ((conf & ADC_RESOLUTION_MASK) >> 5 == 3) {
//     printf("%-12s |\n", "0.0625C");
//   } else {
//     printf("%-12s |\n", "UNKNOWN");
//   }

//   printf("| %-18s | ", "One-Shot:");
//   if ((conf & ONE_SHOT_MASK) >> 7 == 0) {
//     printf("%-12s |\n", "Enable");
//   } else if ((conf & ONE_SHOT_MASK) >> 7 == 1) {
//     printf("%-12s |\n", "Disable");
//   } else {
//     printf("%-12s |\n", "UNKNOWN");
//   }

//   printf("+--------------------+--------------+\n");
// }


void gpio_callback(uint gpio, uint32_t events) {
  volatile BtnState *target_btn;
  uint32_t btn_action;
  if (gpio == BTN0) {
    target_btn = &btns[0];
    btn_action = SCAN_I2C_BUS;
  } else if (gpio == BTN1) {
    target_btn = &btns[1];
    btn_action = SHOW_CONFIG;
  } else if (gpio == BTN2) {
    target_btn = &btns[2];
    btn_action = SHOW_DEV_ID;
  } else if (gpio == BTN3) {
    target_btn = &btns[3];
    btn_action = SHOW_ALERT_MENU;
  } else if (gpio == BTN4) {
    target_btn = &btns[4];
    btn_action = SHOW_TEMP;
  } else {
    return;
  }

  if ((events & GPIO_IRQ_EDGE_RISE) && (events & GPIO_IRQ_EDGE_FALL)) {
    reset_btn_state(target_btn);
    return;
  } else if (events & GPIO_IRQ_EDGE_RISE) {
    set_rising_edge_state(target_btn);
    return;
  } else if (events & GPIO_IRQ_EDGE_FALL) {
    set_falling_edge_state(target_btn);
  }

  bool is_btn_stable = debounce(*target_btn);

  if (is_btn_stable) {
    if (btn_action == SHOW_TEMP) {
      enable_read_temp = true;
      return;
    } else {
      enable_read_temp = false;
      multicore_fifo_drain();
      multicore_fifo_push_timeout_us(btn_action, 1000);
    }
  }
}

void core1_entry() {
  uint32_t request = 0xFFFFFFFF;
  while (1) {
    if (multicore_fifo_rvalid()) {
      uint32_t request = multicore_fifo_pop_blocking();
      if (request == SCAN_I2C_BUS) {
        show_landing_page();
        scan_i2c_bus(i2c, I2C_READ_TIMEOUT_MICRO_SEC);
        multicore_fifo_drain();
      } else if (request == SHOW_CONFIG) {
        multicore_fifo_push_blocking(DISABLE_IRQ);
        show_config_menu();
        show_landing_page();
        read_config(i2c, dev_addr);
        multicore_fifo_push_blocking(ENABLE_IRQ);
      } else if (request == SHOW_DEV_ID) {
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
      } else if (request == SHOW_ALERT_MENU) {
        multicore_fifo_push_blocking(DISABLE_IRQ);
        uint8_t buf[2];
        uint32_t result = show_alert_menu(buf);
        show_landing_page();
        if ((result & READ_TEMP_HYST_LIMIT) == READ_TEMP_HYST_LIMIT) {
          read_temp_hyst_limit(i2c, dev_addr );
        } else if ((result & READ_TEMP_SET_LIMIT) == READ_TEMP_SET_LIMIT) {
          read_temp_set_limit(i2c, dev_addr);
        } else if ((result & WRITE_TEMP_HYST_LIMIT) == WRITE_TEMP_HYST_LIMIT) {
          write_temp_hyst_limit(i2c, dev_addr, buf[0], buf[1]);
        } else if ((result & WRITE_TEMP_SET_LIMIT) == WRITE_TEMP_SET_LIMIT) {
          write_temp_set_limit(i2c, dev_addr, buf[0], buf[1]);
        }
        multicore_fifo_push_blocking(ENABLE_IRQ);
      }
    }
  }
}

int main() {
  stdio_init_all();
  set_gpio(proj_gpio, NUMBER_OF_GPIOS);
  enable_irq(proj_gpio, NUMBER_OF_GPIOS);
  gpio_set_irq_callback(&gpio_callback);
  i2c_init(i2c, TCN75A_BAUDRATE);
  set_i2c(proj_i2c, NUMBER_OF_I2C);

  multicore_launch_core1(core1_entry);

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