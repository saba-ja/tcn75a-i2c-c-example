// Include necessary header files.
#include <stdio.h>

#include "config.h"
#include "debounce.h"
#include "pico/multicore.h"


// Declare functions that will be used by the program.
// The extern keyword is used in C and C++ to declare a function or variable
// that is defined in another source file. The purpose of using extern in this
// way is to tell the compiler that these functions are defined elsewhere, and
// to prevent it from generating duplicate function definitions during the
// compilation process. This allows the program to call these functions without
// having to include their entire source code in the current file. Instead, the
// compiler will link the calls to these functions with their corresponding
// definitions during the linking stage of the compilation process.
extern void core1_entry();
extern void gpio_callback();

// Declare and initialize global variables used by the program.
bool enable_read_temp = false;
i2c_inst_t *i2c = i2c0;
uint8_t dev_addr = TCN75A_DEFAULT_ADDR;
// Define a struct to represent the state of each button.
volatile BtnState btns[NUMBER_OF_BTNS] = {{BTN0, false, false, 0, 0},
                                          {BTN1, false, false, 0, 0},
                                          {BTN2, false, false, 0, 0},
                                          {BTN3, false, false, 0, 0},
                                          {BTN4, false, false, 0, 0}};

// Define the main function.
int main() {
  // Initialize the standard input and output for the program.
  
  // Set up the GPIO pins used by the project.
  
  // Enable interrupts for the GPIO pins.
  
  // Set up a callback function to be called when a GPIO interrupt occurs.
  
  // Initialize the I2C communication protocol with a specific baud rate.
  
  // Set up the I2C device(s) used by the project.
  
  // Launch a second core to run a separate function.
  
  // Print the current temperature using the I2C communication protocol and
  // device address.
  
  // Loop indefinitely.
  while (1) {
    // If a request is received through the multicore FIFO, act on it.
  
      // Get the request value from the FIFO.
  
      // If the request is to disable interrupts, do so.
  
      // If the request is to enable interrupts, do so.
  
    // If temperature reading is enabled, print the current temperature.
  
    // Turn on the onboard LED.
    // Sleep for a set amount of time.
    // Turn off the onboard LED.
    // Sleep for a set amount of time.
  
  }
}
