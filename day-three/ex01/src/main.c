#include "avr/io.h"
#include "util/delay.h"
#include <stdint.h>

typedef enum { RED, GREEN, BLUE, YELLOW, CYAN, MAGENTA, WHITE, COUNT } Colour;

const uint8_t color_map[] = {
    (1 << PD5),
    (1 << PD6),
    (1 << PD3),
    (1 << PD5) | (1 << PD6),
    (1 << PD6) | (1 << PD3),
    (1 << PD5) | (1 << PD3),
    (1 << PD5) | (1 << PD6) | (1 << PD3),
};

void display_color(Colour colour) {
  PORTD &= ~((1 << PD3) | (1 << PD5) | (1 << PD6));
  PORTD |= color_map[colour];
}

int main() {
  DDRD |= (1 << PD3) | (1 << PD5) | (1 << PD6);

  while (1) {
    for (Colour c = RED; c < COUNT; c++) {
      display_color(c);
      _delay_ms(1000);
    }
  }

  return (0);
}