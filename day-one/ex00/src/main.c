#include "avr/io.h"
#include <stdint.h>

// Using volatile to avoid eager optimization of cpu.
void delay(volatile uint16_t n) {
  for (volatile uint16_t i = 0; i < n; i++)
    // For each milisecond, we need to loop 1000 times.
    for (volatile uint16_t j = 0; j < 1000; j++)
      ;
}

int main() {
  DDRB |= (1 << DDB1);

  while (1) {
    // Toggling the led status with the xor.
    PORTB ^= (1 << PORTB1);
    // 500ms of a delay is 0.5s.
    delay(500);
  }

  return (0);
}