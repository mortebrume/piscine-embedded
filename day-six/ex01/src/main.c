#include "../include/i2c.h"
#include "../include/uart.h"
#include "util/delay.h"
#include <stdbool.h>
#include <stdint.h>

// Check if calidration is needed by getting the status.
// We need to verify if the 3rd bit of the status is not set.
bool calib_needed() {
  i2c_start();
  i2c_write(AHT20_W);
  i2c_write(AHT20_ST);
  i2c_start();
  i2c_write(AHT20_R);
  uint8_t status = i2c_read_nack();
  return !(status & (1 << 3));
}

// Start the calibration process.
void calib() {
  i2c_start();
  i2c_write(AHT20_W);
  i2c_write(AHT20_INIT);
  i2c_write(AHT20_IP1);
  i2c_write(EMPTY);
  _delay_ms(10);
}

// Start the measurement.
void measure() {
  i2c_start();
  i2c_write(AHT20_W);
  i2c_write(AHT20_MSR);
  i2c_write(AHT20_MP1);
  i2c_write(EMPTY);
  _delay_ms(80);
}

void print_hex_value(char c) { uart_printhex(c); }

// Reads the 7 data bytes (status + measurement + crc) and print it via UART.
void i2c_read() {
  i2c_start();
  i2c_write(AHT20_R);
  uint8_t data;
  for (uint8_t i = 0; i < 6; i++) {
    data = i2c_read_ack();
    print_hex_value(data);
    uart_tx(' ');
  }
  data = i2c_read_nack();
  print_hex_value(data);
  uart_printstr("\n\r");
}

int main() {
  uart_init();

  _delay_ms(40);
  i2c_init();

  if (calib_needed())
    calib();

  for (;;) {
    measure();
    i2c_read();
    i2c_stop();

    // Wait every 2s between each read (p.5).
    _delay_ms(2000);
  }

  return (0);
}