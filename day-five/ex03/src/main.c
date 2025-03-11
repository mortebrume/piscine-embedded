#include "../include/uart.h"
#include "avr/interrupt.h"
#include "avr/io.h"
#include <stdbool.h>
#include <stdint.h>

volatile bool read = false;

void timer_setup() {
  TCCR1B |= (1 << WGM12) | (1 << CS11);
  OCR1A = 40000;
  TIMSK1 |= (1 << OCIE1A);
}

void adc_setup() {
  // Internal 1.1V voltage must be used for temp reading (p. 256).
  ADMUX |= (1 << REFS0) | (1 << REFS1);
  ADCSRA |= (1 << ADEN) | (1 << ADATE) | (1 << ADSC);
  ADCSRA |= (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);
  // Using ADC8 for temp sensor (p. 258).
  ADMUX |= (1 << MUX3);
}

int main() {
  uart_init();
  adc_setup();
  timer_setup();

  sei();

  for (;;) {
    if (read) {
      int32_t temp = (ADC - 364) / 1.25;
      uart_printnbr(temp);
      uart_printstr("\n\r");
      read = false;
    }
  }

  return (0);
}

ISR(TIMER1_COMPA_vect) { read = true; }