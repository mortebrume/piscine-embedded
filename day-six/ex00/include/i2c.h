#pragma once

#include "util/twi.h"

// Addresses and parameters needed for AHT20 (p. 8).
#define AHT20_R ((0x38 << 1) | 1)
#define AHT20_W ((0x38 << 1) | 0)
#define AHT20_INIT 0xbe
#define AHT20_IP1 0x08
#define AHT20_MSR 0xAC
#define AHT20_MP1 0x33
#define AHT20_ST 0x71
#define EMPTY 0x00

#define TM ((1 << TWS7) | (1 << TWS6) | (1 << TWS5) | (1 << TWS4) | (1 << TWS3))
#define TWS (TWSR & TM)

void i2c_init();
void i2c_start();
void i2c_stop();
void i2c_wait();
void print_status(uint8_t status_code);