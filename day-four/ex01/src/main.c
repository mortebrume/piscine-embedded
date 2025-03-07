#include "avr/interrupt.h"
#include "avr/io.h"
#include <stdint.h>

// 140hz at 8 prescaler.
#define TOP 14285

// We are going to use 256 steps for 0-100 in duty cycle.
// Since we are going from 0 to 100 and then back to 0,
// it means 512 steps per phase of 1s.
// We need around 2ms per step, which is equivalent of
// 2 overflows of timer0 at 64 prescaler.
volatile uint8_t duty_cycle = 0;
volatile int8_t dir = 1;
volatile uint16_t ovf = 0;

void timer_setup() {
  // Normal overflow mode at 255 (p. 115).
  TCCR0A = 0;
  TCCR0B |= (1 << CS00) | (1 << CS01);
  // Interrupts when overflow (p. 118).
  TIMSK0 |= (1 << TOIE0);

  TCCR1A |= (1 << COM1A1) | (1 << WGM11);
  TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS11);
  ICR1 = TOP;
}

int main() {
  DDRB |= (1 << PB1);
  timer_setup();

  sei();

  for (;;)
    ;

  return (0);
}

ISR(TIMER0_OVF_vect) {
  ovf++;

  // Every 2 overflows.
  if (ovf >= 2) {
    ovf = 0;
    duty_cycle += dir;
    if (duty_cycle == 0 || duty_cycle == 255)
      dir = -dir;

    // Convert the 8bit duty cycle to the range of TOP.
    OCR1A = ((uint32_t)duty_cycle * TOP) / 255;
  }
}