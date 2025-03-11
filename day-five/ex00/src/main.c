#include "../include/uart.h"
#include "avr/interrupt.h"
#include "avr/io.h"
#include <stdint.h>

void timer_setup() {
  TCCR1B |= (1 << WGM12) | (1 << CS11);
  OCR1A = 40000;
  TIMSK1 |= (1 << OCIE1A);
}

void adc_setup() {
  ADMUX |= (1 << REFS0) | (1 << ADLAR);
  DIDR0 |= (1 << ADC0D);
  ADCSRA |= (1 << ADEN) | (1 << ADATE) | (1 << ADSC);
}

int main() {
  uart_init();
  adc_setup();
  timer_setup();

  sei();

  for (;;)
    ;

  return (0);
}

ISR(TIMER1_COMPA_vect) {
  uint8_t value = ADCH;
  uart_printhex(value);
  uart_printstr("\n\r");
}