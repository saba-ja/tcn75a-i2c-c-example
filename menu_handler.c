#include "menu_handler.h"

#include <stdio.h>

#include "hardware/address_mapped.h"
#include "i2c_util.h"
#include "pico/time.h"
#include "util.h"
#include "config.h"

void show_landing_page() {
  clear_screen();
  printf(" _____ ____ _   _ _____ ____    _    \n");
  printf("|_   _/ ___| \\ | |___  | ___|  / \\   \n");
  printf("  | || |   |  \\| |  / /|___ \\ / _ \\  \n");
  printf("  | || |___| |\\  | / /  ___) / ___ \\ \n");
  printf("  |_| \\____|_| \\_|/_/  |____/_/   \\_\\ \n");
  printf(" ------------------------------------------ \n");
  printf("| Press button 0 to scan addresses         | \n");
  printf("| Press button 1 to access config menu     | \n");
  printf("| Press button 2 to access device ID menu  | \n");
  printf("| Press button 3 to access alert  menu     | \n");
  printf("| Press button 4 to print temperature      | \n");
  printf(" ------------------------------------------ \n");
}

void show_config_menu() {
  char option;
  while (1) {
    clear_screen();
    printf("[0] SHUTDOWN Setting\n");
    printf("[1] COMP/INT Select\n");
    printf("[2] ALERT POLARITY\n");
    printf("[3] FAULT QUEUE\n");
    printf("[4] ADC RES\n");
    printf("[5] ONE-SHOT\n");
    printf("[x] QUIT\n");
    scanf(" %c", &option);

    if (option == '0') {
      while (1) {
        clear_screen();
        printf("SHUTDOWN Setting\n");
        printf("[0] Disable Shutdown\n");
        printf("[1] Enable Shutdown\n");
        printf("[x] Return to main\n");
        scanf(" %c", &option);

        if (option == '0') {
          printf("Disabling shutdown.\n");
        } else if (option == '1') {
          printf("Enabling shutdown.\n");
        } else if (option == 'x') {
          break;
        }
      }
    } else if (option == '1') {
      while (1) {
        clear_screen();
        printf("COMP/INT Select\n");
        printf("[0] Comparator mode\n");
        printf("[1] Interrupt mode\n");
        printf("[x] Return to main\n");
        scanf(" %c", &option);

        if (option == '0') {
          printf("Active Low selected.\n");
        } else if (option == '1') {
          printf("Active High selected.\n");
        } else if (option == 'x') {
          break;
        }
      }
    } else if (option == '2') {
      while (1) {
        clear_screen();
        printf("Alert Polarity\n");
        printf("[0] Active low\n");
        printf("[1] Active High\n");
        printf("[x] Return to main\n");
        scanf(" %c", &option);

        if (option == '0') {
          printf("Active Low selected.\n");
        } else if (option == '1') {
          printf("Active High selected.\n");
        } else if (option == 'x') {
          break;
        }
      }
    } else if (option == '3') {
      while (1) {
        clear_screen();
        printf("Fault Queue\n");
        printf("[0] 00\n");
        printf("[1] 01\n");
        printf("[2] 10\n");
        printf("[3] 11\n");
        printf("[x] Return to main\n");
        scanf(" %c", &option);

        if (option == '0') {
          printf("Active Low selected.\n");
        } else if (option == '1') {
          printf("Active High selected.\n");
        } else if (option == 'x') {
          break;
        }
      }
    } else if (option == '4') {
      while (1) {
        clear_screen();
        printf("ADC Resolution\n");
        printf("[0] 9 bit or 0.5C\n");
        printf("[1] 10 bit or 0.25C\n");
        printf("[2] 11 bit or 0.125C\n");
        printf("[3] 12 bit or 0.0625C\n");
        printf("[x] Return to main\n");
        scanf(" %c", &option);

        if (option == '0') {
          printf("Active Low selected.\n");
        } else if (option == '1') {
          printf("Active High selected.\n");
        } else if (option == 'x') {
          break;
        }
      }
    } else if (option == '5') {
      while (1) {
        clear_screen();
        printf("One SHOT setting\n");
        printf("[0] Disable\n");
        printf("[1] Enable\n");
        printf("[x] Return to main\n");
        scanf(" %c", &option);

        if (option == '0') {
          printf("Active Low selected.\n");
        } else if (option == '1') {
          printf("Active High selected.\n");
        } else if (option == 'x') {
          break;
        }
      }
    } else if (option == 'x') {
      break;
    }
  }
  clear_screen();
}

uint8_t show_dev_change_menu(uint8_t default_addr) {
  char option = '0';
  uint8_t addr = default_addr;
  clear_screen();
  while (1) {
    printf("Change Device ID\n");
    printf("[0] 0x48\n");
    printf("[1] 0x49\n");
    printf("[2] 0x4A\n");
    printf("[3] 0x4B\n");
    printf("[4] 0x4C\n");
    printf("[5] 0x4D\n");
    printf("[6] 0x4E\n");
    printf("[7] 0x4F\n");
    printf("[x] Return to main\n");
    scanf(" %c", &option);

    switch (option) {
      case 'x':
        addr = 0;
        break;
      case '0':
        addr = 0x48;
        break;
      case '1':
        addr = 0x49;
        break;
      case '2':
        addr = 0x4A;
        break;
      case '3':
        addr = 0x4B;
        break;
      case '4':
        addr = 0x4C;
        break;
      case '5':
        addr = 0x4D;
        break;
      case '6':
        addr = 0x4E;
        break;
      case '7':
        addr = 0x4F;
        break;
      default:
        clear_screen();
        continue;
    }

    clear_screen();
    return addr;
  }
}

uint32_t show_alert_menu(uint8_t *buf) {
  char option;
  while (1) {
    clear_screen();
    printf("ALERT Config\n");
    printf("[0] Alert Enable/Disable\n");
    printf("[1] Write Temp Hyst Limit\n");
    printf("[2] Write Temp Set Limit\n");
    printf("[3] Show Temp Hyst Limit\n");
    printf("[4] Show Temp Set Limit\n");
    printf("[x] Return to main\n");
    scanf(" %c", &option);

    if (option == '0') {
      while (1) {
        clear_screen();
        printf("ALERT Setting\n");
        printf("[0] Disable ALERT\n");
        printf("[1] Enable ALERT\n");
        printf("[x] Return to main\n");
        scanf(" %c", &option);

        if (option == '0') {
          printf("Disabling ALERT.\n");
        } else if (option == '1') {
          printf("Enabling ALERT.\n");
        } else if (option == 'x') {
          break;
        }
      }
    } else if (option == '1') {
      char input[8];
      int32_t output[2];
      clear_screen();
      printf("Enter Temp Hyst Limit: ");
      get_input(input, 8);
      bool is_success = str_to_fixed_point(input, output);
      if (is_success) {
          buf[0] = output[0];
          buf[1] = output[1];
          return WRITE_TEMP_HYST_LIMIT;
      }  else {
        printf("Invalid input! Returning to the previous menu\n");
        sleep_ms(2000);
      }
    } else if (option == '2') {
      char input[8];
      int32_t output[2];
      clear_screen();
      printf("Enter Temp Set Limit: ");
      get_input(input, 8);
      bool is_success = str_to_fixed_point(input, output);
      if (is_success) {
          buf[0] = output[0];
          buf[1] = output[1];
          return WRITE_TEMP_SET_LIMIT;
      }  else {
        printf("Invalid input! Returning to the previous menu\n");
        sleep_ms(2000);
      }
    } else if (option == '3') {
      return READ_TEMP_HYST_LIMIT;
    } else if (option == '4') {
      return READ_TEMP_SET_LIMIT;
    } else if (option == 'x') {
      clear_screen();
      break;
    } else {
    }
  }
}

void print_temp_table(uint8_t integer_part, uint8_t decimal_part) {
  printf("%-8s--%-8s\n", "-------", "-------");
  printf("%-8s| %-8s\n", "Temp C", "Temp F");
  printf("%-8s+ %-8s\n", "-------", "-------");
  float celsius = fixedToFloat(integer_part, decimal_part);
  printf("%-8.4f| %-8.4f\n", celsius, c2f(celsius));
}

void parse_config(uint8_t conf) {
  printf("+--------------------+--------------+\n");
  printf("|       Setting      |     Value    |\n");
  printf("+--------------------+--------------+\n");
  printf("| %-18s | ", "Shutdown:");
  if ((conf & SHUTDOWN_MASK) == 0) {
    printf("%-12s |\n", "Disable");
  } else if ((conf & SHUTDOWN_MASK) == SHUTDOWN_MASK) {
    printf("%-12s |\n", "Enable");
  } else {
    printf("%-12s |\n", "UNKNOWN");
  }

  printf("| %-18s | ", "Alert Mode:");
  if ((conf & ALERT_MODE_MASK) >> 1 == 0) {
    printf("%-12s |\n", "Comp");
  } else if ((conf & ALERT_MODE_MASK) >> 1 == 1) {
    printf("%-12s |\n", "Intr");
  } else {
    printf("%-12s |\n", "UNKNOWN");
  }

  printf("| %-18s | ", "Alert Polarity:");
  if ((conf & ALERT_POLARITY_MASK) >> 2 == 0) {
    printf("%-12s |\n", "Low");
  } else if ((conf & ALERT_POLARITY_MASK) >> 2 == 1) {
    printf("%-12s |\n", "High");
  } else {
    printf("%-12s |\n", "UNKNOWN");
  }

  printf("| %-18s | ", "Fault Queue:");
  if ((conf & FAULT_QUEUE_MASK) >> 3 == 0) {
    printf("%-12d |\n", 1);
  } else if ((conf & FAULT_QUEUE_MASK) >> 3 == 1) {
    printf("%-12d |\n", 2);
  } else if ((conf & FAULT_QUEUE_MASK) >> 3 == 2) {
    printf("%-12d |\n", 4);
  } else if ((conf & FAULT_QUEUE_MASK) >> 3 == 3) {
    printf("%-12d |\n", 6);
  } else {
    printf("%-12s |\n", "UNKNOWN");
  }

  printf("| %-18s | ", "ADC Resolution:");
  if ((conf & ADC_RESOLUTION_MASK) >> 5 == 0) {
    printf("%-12s |\n", "0.5C");
  } else if ((conf & ADC_RESOLUTION_MASK) >> 5 == 1) {
    printf("%-12s |\n", "0.25C");
  } else if ((conf & ADC_RESOLUTION_MASK) >> 5 == 2) {
    printf("%-12s |\n", "0.125C");
  } else if ((conf & ADC_RESOLUTION_MASK) >> 5 == 3) {
    printf("%-12s |\n", "0.0625C");
  } else {
    printf("%-12s |\n", "UNKNOWN");
  }

  printf("| %-18s | ", "One-Shot:");
  if ((conf & ONE_SHOT_MASK) >> 7 == 0) {
    printf("%-12s |\n", "Enable");
  } else if ((conf & ONE_SHOT_MASK) >> 7 == 1) {
    printf("%-12s |\n", "Disable");
  } else {
    printf("%-12s |\n", "UNKNOWN");
  }

  printf("+--------------------+--------------+\n");
}
