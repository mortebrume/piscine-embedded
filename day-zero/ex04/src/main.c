#include "avr/io.h"
#include "util/delay.h"
#include <stdint.h>

void update_leds(uint8_t count) {
  // Only unset the bits that we are using.
  PORTB &= ~((1 << PORTB0) | (1 << PORTB1) | (1 << PORTB2) | (1 << PORTB4));
  // We are manually setting the 4th bit (if its set) to PORTB4.
  if (count & 0b1000) {
    PORTB |= (1 << PORTB4);
    // Removing the 4th bit to avoid setting PORTB3.
    count ^= 0b1000;
  }
  // The rest will be assigned from PORTB0 to PORTB2.
  PORTB |= count;
}

void toggle_count(volatile uint8_t *count, int pin) {
  if (!(PIND & (1 << pin))) {
    // We are using modulo there to wrap around 16.
    if (pin == PD2)
      *count = (*count + 1) % 16;
    if (pin == PD4)
      *count = (*count - 1 + 16) % 16;
    update_leds(*count);
    while (!(PIND & (1 << pin)))
      ;
    _delay_ms(500);
  }
}

int main() {
  // The 4th LED is at the PIN4 and not the PIN3 (schema).
  DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2) | (1 << DDB4);
  // Using volatile to avoid eager cpu optimization since we are using delay.
  volatile uint8_t count = 0;

  while (1) {
    toggle_count(&count, PD2);
    toggle_count(&count, PD4);
  }

  return (0);
}