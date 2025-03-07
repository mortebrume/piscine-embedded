#include "avr/interrupt.h"
#include "avr/io.h"
#include "util/delay.h"
#include <stdbool.h>

volatile bool pressed = false;

int main() {
  DDRB |= (1 << PB0);

  // Enable the external interrupt INT0 linked to SW1 (p. 81);
  EIMSK |= (1 << INT0);
  // Activate the external interrupt INT0 when the button is released (p. 80).
  EICRA |= (1 << ISC00) | (1 << ISC01);

  sei();

  for (;;) {
    if (pressed) {
      PORTB ^= (1 << PB0);
      // The order is important here, for proper debouncing.
      _delay_ms(250);
      pressed = false;
    }
  }

  return (0);
}

ISR(INT0_vect) { pressed = true; }