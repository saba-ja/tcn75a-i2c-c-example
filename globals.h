#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include "pico/stdlib.h"
#include "i2c_util.h"
#include "debounce.h"
#include "config.h"

extern bool enable_read_temp;
extern i2c_inst_t *i2c;
extern uint8_t dev_addr;
extern volatile BtnState btns[NUMBER_OF_BTNS];
#endif