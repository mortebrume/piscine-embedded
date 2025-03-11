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
  ADMUX |= (1 << REFS0);
  DIDR0 |= (1 << ADC0D) | (1 << ADC1D) | (1 << ADC2D);
  ADCSRA |= (1 << ADEN) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);
}

uint16_t adc_read(int pin) {
  ADMUX &= ~((1 << MUX0) | (1 << MUX1) | (1 << MUX2) | (1 << MUX3));
  if (pin == ADC1D)
    ADMUX |= (1 << MUX0);
  if (pin == ADC2D)
    ADMUX |= (1 << MUX1);
  ADCSRA |= (1 << ADSC);
  while (ADCSRA & (1 << ADSC))
    ;
  return ADC;
}

void print_adc(uint16_t rv1, uint16_t ldr, uint16_t ntc) {
  uart_printnbr(rv1);
  uart_printstr(", ");
  uart_printnbr(ldr);
  uart_printstr(", ");
  uart_printnbr(ntc);
  uart_printstr("\n\r");
}

int main() {
  uart_init();
  adc_setup();
  timer_setup();

  sei();

  for (;;) {
    if (read) {
      uint16_t rv1 = adc_read(ADC0D);
      uint16_t ldr = adc_read(ADC1D);
      uint16_t ntc = adc_read(ADC2D);
      print_adc(rv1, ldr, ntc);
      read = false;
    }
  }

  return (0);
}

ISR(TIMER1_COMPA_vect) { read = true; }