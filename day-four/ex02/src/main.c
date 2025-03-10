#include "avr/interrupt.h"
#include "avr/io.h"
#include "util/delay.h"
#include <stdbool.h>
#include <stdint.h>

volatile bool sw1 = false;
volatile bool sw2 = false;

void update_leds(uint8_t count) {
  PORTB &= ~((1 << PORTB0) | (1 << PORTB1) | (1 << PORTB2) | (1 << PORTB4));
  if (count & 0b1000) {
    PORTB |= (1 << PORTB4);
    count ^= 0b1000;
  }
  PORTB |= count;
}

void toggle_count(volatile uint8_t *count, int pin) {
  if (pin == PD2)
    *count = (*count + 1) % 16;
  if (pin == PD4)
    *count = (*count - 1 + 16) % 16;
  update_leds(*count);
}

int main() {
  DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2) | (1 << DDB4);
  EIMSK |= (1 << INT0);
  EICRA |= (1 << ISC00) | (1 << ISC01);
  // SW2 is PCINT20 (p. 12).
  PCICR |= (1 << PCIE2);
  PCMSK2 |= (1 << PCINT20);

  sei();

  volatile uint8_t count = 0;

  for (;;) {
    if (sw1) {
      toggle_count(&count, PD2);
      _delay_ms(250);
      sw1 = false;
    } else if (sw2) {
      toggle_count(&count, PD4);
      _delay_ms(250);
      sw2 = false;
    }
  }

  return (0);
}

ISR(INT0_vect) { sw1 = true; }

ISR(PCINT2_vect) { sw2 = true; }