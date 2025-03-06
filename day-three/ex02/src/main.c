#include "avr/io.h"
#include "util/delay.h"
#include <stdint.h>

void set_rgb(uint8_t r, uint8_t g, uint8_t b) {
  OCR0B = r;
  OCR0A = g;
  OCR2B = b;
}

void init_rgb() {
  // Timer 0 for led R and G. Top is 255 by default, fast PWM mode.
  TCCR0A |= (1 << WGM00) | (1 << WGM01) | (1 << COM0A1) | (1 << COM0B1);
  // Pre-scale of 1.
  TCCR0B |= (1 << CS00);
  // Timer 2 for led B. Top is 255 by defauylt, fast PWM mode.
  TCCR2A |= (1 << COM2B1) | (1 << WGM20) | (1 << WGM21);
  // Pre-scale of 1.
  TCCR2B |= (1 << CS20);
  OCR0A = 0;
  OCR0B = 0;
  OCR2B = 0;
}

void wheel(uint8_t pos) {
  pos = 255 - pos;
  if (pos < 85) {
    set_rgb(255 - pos * 3, 0, pos * 3);
  } else if (pos < 170) {
    pos = pos - 85;
    set_rgb(0, pos * 3, 255 - pos * 3);
  } else {
    pos = pos - 170;
    set_rgb(pos * 3, 255 - pos * 3, 0);
  }
}

int main() {
  DDRD |= (1 << PD3) | (1 << PD5) | (1 << PD6);
  init_rgb();

  while (1) {
    for (uint8_t i = 0; i < 255; i++) {
      wheel(i);
      _delay_ms(20);
    }
  }

  return (0);
}