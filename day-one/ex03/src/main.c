#include "avr/io.h"
#include "util/delay.h"
#include <stdint.h>

// Using the usual timer count as global.
// Steps are made of 10% increments.
const uint16_t TC = 31249;
const uint16_t STEP = TC / 10;

// We check for which button was pressed to adjust OCR1A.
void toggle_duty(int pin) {
  if (!(PIND & (1 << pin))) {
    if (pin == PD2 && OCR1A < ICR1)
      OCR1A += STEP;
    if (pin == PD4 && OCR1A > STEP)
      OCR1A -= STEP;
    while (!(PIND & (1 << pin)))
      ;
    _delay_ms(500);
  }
}

int main() {
  DDRB |= (1 << PB1);

  TCCR1B |= (1 << WGM13) | (1 << WGM12);
  TCCR1A |= (1 << WGM11);

  TCCR1A |= (1 << COM1A1);

  // Using the globals defined at the top.
  ICR1 = TC;
  OCR1A = STEP;

  TCCR1B |= (1 << CS12);

  for (;;) {
    toggle_duty(PD2);
    toggle_duty(PD4);
  }

  return (0);
}