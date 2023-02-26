#include "i2c_util.h"

#include "util.h"

void set_i2c(const I2CConfig *i2c, size_t len) {
  for (size_t i; i < len; i++) {
    gpio_set_function(i2c->sda_pin_number, GPIO_FUNC_I2C);
    gpio_set_function(i2c->scl_pin_number, GPIO_FUNC_I2C);
    if (i2c->has_pullup) {
      gpio_pull_up(i2c->sda_pin_number);
      gpio_pull_up(i2c->scl_pin_number);
    }
  }
}

// Write 1 byte to the specified register
int reg_write(i2c_inst_t *i2c, const uint8_t addr, const uint8_t reg,
              uint8_t *buf, const uint8_t nbytes) {
  int num_bytes_read = 0;
  uint8_t msg[nbytes + 1];

  // Check to make sure caller is sending 1 or more bytes
  if (nbytes < 1) {
    return 0;
  }

  // Append register address to front of data packet
  msg[0] = reg;
  for (int i = 0; i < nbytes; i++) {
    msg[i + 1] = buf[i];
  }

  // Write data to register(s) over I2C
  num_bytes_read = i2c_write_blocking(i2c, addr, msg, (nbytes + 1), false);

  return num_bytes_read;
}

// Read byte(s) from specified register. If nbytes > 1, read from consecutive
// registers.
int reg_read(i2c_inst_t *i2c, const uint8_t addr, const uint8_t reg,
             uint8_t *buf, const uint8_t nbytes) {
  int num_bytes_read = 0;

  // Check to make sure caller is asking for 1 or more bytes
  if (nbytes < 1) {
    return 0;
  }

  // Read data from register(s) over I2C
  i2c_write_blocking(i2c, addr, &reg, 1, true);
  num_bytes_read = i2c_read_blocking(i2c, addr, buf, nbytes, false);

  return num_bytes_read;
}

int check_addr(i2c_inst_t *i2c, uint8_t addr, uint8_t *rxdata, uint timeout) {
  int ret = i2c_read_timeout_us(i2c, addr, rxdata, 1, false, timeout);
  return ret;
}

void scan_i2c_bus(i2c_inst_t *i2c, uint timeout) {
  printf("\nI2C Bus Scan\n");
  printf("   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");

  for (uint8_t addr = 0; addr < (1 << 7); ++addr) {
    if (addr % 16 == 0) {
      printf("%02x ", addr);
    }

    // Perform a 1-byte dummy read from the probe address. If a slave
    // acknowledges this address, the function returns the number of bytes
    // transferred. If the address byte is ignored, the function returns
    // -1.

    // Skip over any reserved addresses.
    int ret;
    uint8_t rxdata;
    if (reserved_addr(addr))
      printf("X");
    else {
      ret = check_addr(i2c, addr, &rxdata, timeout);
      //   ret = i2c_read_blocking(i2c, addr, &rxdata, 1, false);
      printf(ret <= 0 ? "." : "@");
    }

    printf(addr % 16 == 15 ? "\n" : "  ");
  }
  printf("Done.\n");
}

// I2C reserves some addresses for special purposes. We exclude these from the
// scan. These are any addresses of the form 000 0xxx or 111 1xxx
bool reserved_addr(uint8_t addr) {
  // Check if value is of the form 0000xxx or 1111xxx
  if ((addr & 0x78) == 0 || (addr & 0x78) == 0x78) {
    return 1;
  } else {
    return 0;
  }
}
