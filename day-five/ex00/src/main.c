#include "../include/uart.h"
#include "avr/interrupt.h"
#include "avr/io.h"
#include <stdint.h>

// Trigger an interrupt every 2ms.
void timer_setup() {
  TCCR1B |= (1 << WGM12) | (1 << CS11);
  OCR1A = 40000;
  TIMSK1 |= (1 << OCIE1A);
}

void adc_setup() {
  // We use AVCC as reference voltage (p. 257).
  // Using left adjust result to only read 8 bits.
  ADMUX |= (1 << REFS0) | (1 << ADLAR);
  // Disable input for the pin associated to ADC0 (p. 261).
  DIDR0 |= (1 << ADC0D);
  // Enable ADC in free running mode (p. 258).
  ADCSRA |= (1 << ADEN) | (1 << ADATE) | (1 << ADSC);
  // We use a prescaler of 64 to be around the 200kHz required range (p. 259).
  ADCSRA |= (1 << ADPS1) | (1 << ADPS2);
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

// Read the convert value from ADC and print it via UART.
ISR(TIMER1_COMPA_vect) {
  uint8_t value = ADCH;
  uart_printhex(value);
  uart_printstr("\n\r");
}