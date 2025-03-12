#include "../include/i2c.h"
#include "../include/uart.h"

int main() {
  uart_init();

  i2c_init();
  i2c_start();
  i2c_stop();

  return (0);
}