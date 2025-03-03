#include "avr/io.h"
#include "util/delay.h"

int main() {
  // DDRB to output mode as usual.
  // DDRD is by default at input mode.
  // We now the button SW1 is linked to PD2 (schema).
  DDRB |= (1 << DDB0);

  while (1) {
    // SW1 is a pull-up resistor (schema), meaning the voltage is high if not
    // pressed. We read the state of PD2, which is 0 if the button is pressed.
    if (!(PIND & (1 << PD2))) {
      PORTB |= (1 << PORTB0);
      // Add a delay to avoid debounce.
      _delay_ms(100);
    } else {
      // Unset the B0 bit of PORTB.
      PORTB &= ~(1 << PORTB0);
    }
  }

  return (0);
}