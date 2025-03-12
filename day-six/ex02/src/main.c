#include "../include/i2c.h"
#include "../include/uart.h"
#include "util/delay.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

double hum[3];
double temp[3];
uint8_t readings = 0;

bool calib_needed() {
  i2c_start();
  i2c_write(AHT20_W);
  i2c_write(AHT20_ST);
  i2c_start();
  i2c_write(AHT20_R);
  uint8_t status = i2c_read_nack();
  return !(status & (1 << 3));
}

void calib() {
  i2c_start();
  i2c_write(AHT20_W);
  i2c_write(AHT20_INIT);
  i2c_write(AHT20_IP1);
  i2c_write(EMPTY);
  _delay_ms(10);
}

void measure() {
  i2c_start();
  i2c_write(AHT20_W);
  i2c_write(AHT20_MSR);
  i2c_write(AHT20_MP1);
  i2c_write(EMPTY);
  _delay_ms(80);
}

void convert(uint8_t data[5]) {
  double hum_raw = ((int32_t)data[0] << 12) | ((int32_t)data[1] << 4) |
                   (((int32_t)data[2] >> 4) & 0x0F);
  double temp_raw = (((int32_t)data[2] & 0x0F) << 16) |
                    ((int32_t)data[3] << 8) | ((int32_t)data[4]);

  double hum_val = (hum_raw / 1048576.0) * 100.0;
  double temp_val = (temp_raw / 1048576.0) * 200.0 - 50.0;

  hum[readings - 1] = hum_val;
  temp[readings - 1] = temp_val;
}

void aht_read() {
  uint8_t data[5];

  i2c_start();
  i2c_write(AHT20_R);
  i2c_read_ack();
  for (uint8_t i = 0; i <= 4; i++) {
    data[i] = i2c_read_ack();
  }
  i2c_read_nack();
  i2c_stop();

  readings++;
  convert(data);
}

void print_results() {
  char hum_buf[16];
  char temp_buf[16];
  double hum_avg = (hum[0] + hum[1] + hum[2]) / 3.0;
  double temp_avg = (temp[0] + temp[1] + temp[2]) / 3.0;
  dtostrf(hum_avg, 0, 2, hum_buf);
  dtostrf(temp_avg, 0, 1, temp_buf);

  uart_printstr("Temperature: ");
  uart_printstr(temp_buf);
  uart_printstr(" C, Humidity: ");
  uart_printstr(hum_buf);
  uart_printstr("%\n\r");
  readings = 0;
}

int main() {
  uart_init();

  _delay_ms(40);
  i2c_init();

  if (calib_needed())
    calib();

  for (;;) {
    measure();
    aht_read();
    if (readings == 3)
      print_results();
    _delay_ms(2000);
  }

  return (0);
}