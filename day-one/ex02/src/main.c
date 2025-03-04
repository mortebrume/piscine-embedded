#include "avr/io.h"

int main() {
  DDRB |= (1 << PB1);

  // We set up the timer in fast PWM mode (p.132 & p.142).
  // ICR1 will hold the timer count, and OCR1A will be the bottom.
  // Manipulating OCR1A is equivalent to manipulating the duty cycle.
  TCCR1B |= (1 << WGM13) | (1 << WGM12);
  TCCR1A |= (1 << WGM11);

  TCCR1A |= (1 << COM1A1);

  // Still at 2Hz.
  ICR1 = 31249;
  // 10% of ICR1.
  OCR1A = 3124;

  TCCR1B |= (1 << CS12);

  for (;;)
    ;

  return (0);
}