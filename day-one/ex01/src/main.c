#include "avr/io.h"

int main() {
  // The DDR needs to be set so the OC1A can override PB1 (p. 140 and schema)
  DDRB |= (1 << PB1);

  // We are using the timer 1, which uses 16 bits (p. 120).
  // To configure it, we need to use 2 registers, TCCR1A and TCCR1B.
  // To enable CTC with OCR1A, we need to enable the bit WGM12 (p. 141)
  TCCR1B |= (1 << WGM12);
  // This enables toggling OC1A on compare match (p. 140)
  TCCR1A |= (1 << COM1A0);

  // Using the equation : (CPU_Clock / (Prescaler * Desired_Frequency)) - 1
  // We are using the smallest prescaler value that still fits in 2^16 bits.
  // Our timer count for 2Hz, using a 256 prescale.
  OCR1A = 31249;
  // Selects the clock source, we use the 256 pre-scaler (p. 143)
  TCCR1B |= (1 << CS12);

  for (;;)
    ;

  return (0);
}