#include "avr/io.h"
#include "util/delay.h"

int main() {
  DDRB |= (1 << DDB0);

  while (1) {
    if (!(PIND & (1 << PD2))) {
      // Xor'ing the PB0 bit is equivalent to toggling it.
      PORTB ^= (1 << PB0);
      // Infinite loop until the button is released.
      while (!(PIND & (1 << PD2)))
        ;
      // Add a delay to avoid debounce.
      _delay_ms(100);
    }
  }

  return (0);
}