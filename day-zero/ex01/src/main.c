#include "avr/io.h"

int main() {
  // The LED D1 is linked to the pin B0 (schema).
  // Firstly, we need to setup the pin (p. 85) to ouput mode, using the DDR.
  // As seen in the datasheet (p.100), the DDR for B0 is called DDB01.
  DDRB |= (1 << DDB0);

  // Since we want to write to output, we toggle the B0 bit of PORTB.
  PORTB |= (1 << PORTB0);

  return (0);
}