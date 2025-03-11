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
  ADMUX |= (1 << REFS0) | (1 << ADLAR);
  // Disable input register for all required pins.
  DIDR0 |= (1 << ADC0D) | (1 << ADC1D) | (1 << ADC2D);
  ADCSRA |= (1 << ADEN) | (1 << ADPS1) | (1 << ADPS2);
}

uint8_t adc_read(int pin) {
  // The analog selection bit permits us to read from the required pin (p. 257).
  // We reset it at every read, not forgetting than 0000 is for ADC0.
  ADMUX &= ~((1 << MUX0) | (1 << MUX1) | (1 << MUX2) | (1 << MUX3));
  if (pin == ADC1D)
    ADMUX |= (1 << MUX0);
  if (pin == ADC2D)
    ADMUX |= (1 << MUX1);
  // Convert one time and wait until the conversion is done (p. 258).
  ADCSRA |= (1 << ADSC);
  while (ADCSRA & (1 << ADSC))
    ;
  return ADCH;
}

void print_adc(uint8_t rv1, uint8_t ldr, uint8_t ntc) {
  uart_printhex(rv1);
  uart_printstr(", ");
  uart_printhex(ldr);
  uart_printstr(", ");
  uart_printhex(ntc);
  uart_printstr("\n\r");
}

int main() {
  uart_init();
  adc_setup();
  timer_setup();

  sei();

  for (;;) {
    if (read) {
      uint8_t rv1 = adc_read(ADC0D);
      uint8_t ldr = adc_read(ADC1D);
      uint8_t ntc = adc_read(ADC2D);
      print_adc(rv1, ldr, ntc);
      read = false;
    }
  }

  return (0);
}

ISR(TIMER1_COMPA_vect) { read = true; }