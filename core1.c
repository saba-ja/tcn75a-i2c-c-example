#include "core1.h"

#include <stdio.h>

#include "config.h"
#include "debounce.h"
#include "globals.h"
#include "gpio_util.h"
#include "menu_handler.h"
#include "pico/multicore.h"
#include "pico/stdlib.h"

/**
 * @brief Entry point for core1.
 *
 * This function is the entry point for the second core. It continuously
 * monitors an input GPIO pin for any alerts, and updates two output GPIO pins
 * to indicate the alert status. It also checks the multicore FIFO for any
 * pending requests, and calls the `handle_request()` function to handle them.
 *
 * @return void
 */
void core1_entry() {

}

/**
 * @brief Handle the specified request.
 *
 * This function takes a request code as a parameter and performs the
 * appropriate action based on the request. The supported requests and their
 * corresponding actions are as follows:
 * - SCAN_I2C_BUS: Shows the landing page and scans the I2C bus for devices.
 * - SHOW_CONFIG: Displays the configuration settings.
 * - SHOW_DEV_ID: Displays the device ID.
 * - SHOW_ALERT_MENU: Displays the alert menu.
 *
 * If the request is not one of the above, the function does nothing.
 *
 * @param request The request code to handle.
 * @return void
 */
void handle_request(uint32_t request) {

}

/**
 * @brief Displays the configuration menu and handles the user's configuration
 * choice.
 *
 * This function displays the configuration menu and waits for the user to make
 * a configuration choice. It then applies the corresponding configuration
 * change to the device, and displays the updated configuration status. The
 * supported configuration options and their corresponding bit masks are as
 * follows:
 * - Shutdown mode: SHUTDOWN_MODE_REQ_MASK
 * - Comparator/Interrupt mode: COMP_INT_MODE_REQ_MASK
 * - Alert polarity: ALERT_POLARITY_REQ_MASK
 * - Fault queue mode: FAULT_QUEUE_MODE_REQ_MASK
 * - ADC resolution: ADC_RESOLUTION_REQ_MASK
 * - One-shot mode: ONE_SHOT_MODE_REQ_MASK
 *
 * If the user does not make a valid configuration choice, or chooses to make no
 * change, the function does nothing. Additionally, the function disables
 * interrupts while the configuration menu is displayed and changes are applied,
 * to ensure data consistency.
 *
 * @return void
 */
void handle_show_config() {
}

/**
 * @brief Displays the device ID change menu and handles the user's device ID
 * choice.
 *
 * This function displays the device ID change menu and waits for the user to
 * enter a new device ID. It then checks if the device is accessible using the
 * new ID, and if successful, changes the device ID and displays a success
 * message. If the device is not accessible, the function displays a warning
 * message. Additionally, the function disables interrupts while the device ID
 * change menu is displayed, to ensure data consistency.
 *
 * @return void
 */
void handle_show_dev_id() {
}

/**
 * @brief Displays the alert menu and handles the user's alert choice.
 *
 * This function displays the alert menu and waits for the user to make an alert
 * choice. It then performs the corresponding action based on the choice made by
 * the user. The supported alert choices and their corresponding actions are as
 * follows:
 * - READ_TEMP_HYST_LIMIT: Reads the temperature hysteresis limit.
 * - READ_TEMP_SET_LIMIT: Reads the temperature set limit.
 * - WRITE_TEMP_HYST_LIMIT: Writes the temperature hysteresis limit using the
 * two-byte buffer passed by the user.
 * - WRITE_TEMP_SET_LIMIT: Writes the temperature set limit using the two-byte
 * buffer passed by the user.
 *
 * If the user does not make a valid alert choice, or chooses to perform no
 * action, the function does nothing. Additionally, the function disables
 * interrupts while the alert menu is displayed and actions are performed, to
 * ensure data consistency.
 *
 * @return void
 */
void handle_show_alert_menu() {

}
