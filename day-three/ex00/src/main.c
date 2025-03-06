#include "avr/io.h"
#include "util/delay.h"
#include <stdint.h>

// Enum to hold the colours to easily loop in the required order.
typedef enum { RED, GREEN, BLUE, COUNT } Colour;

// Map of colours with every revelant bitvalue.
const uint8_t color_map[] = {(1 << PD5), (1 << PD6), (1 << PD3)};

void display_color(Colour colour) {
  // Reset the colours.
  PORTD &= ~((1 << PD3) | (1 << PD5) | (1 << PD6));
  // Set the colour using the map.
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