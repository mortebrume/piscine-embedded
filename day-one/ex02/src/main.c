#include "avr/io.h"

int main() {
  DDRB |= (1 << PB1);

  // We set up the timer in fast PWM mode (p.132 & p.142).
  // ICR1 will hold the timer count, and OCR1A will be the bottom.
  // Manipulating OCR1A is equivalent to manipulating the duty cycle.
  TCCR1B |= (1 << WGM13) | (1 << WGM12);
  TCCR1A |= (1 << WGM11);

  TCCR1A |= (1 << COM1A1);

  // 1Hz.
  ICR1 = 62500;
  // 10% of ICR1.
  OCR1A = 6250;

  TCCR1B |= (1 << CS12);

  for (;;)
    ;

  return (0);
}