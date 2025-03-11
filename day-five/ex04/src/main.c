#include "avr/interrupt.h"
#include "avr/io.h"
#include <stdbool.h>
#include <stdint.h>

volatile bool read = false;

void set_rgb(uint8_t r, uint8_t g, uint8_t b) {
  OCR0B = r;
  OCR0A = g;
  OCR2B = b;
}

void timer_setup() {
  TCCR0A |= (1 << WGM00) | (1 << WGM01) | (1 << COM0A1) | (1 << COM0B1);
  TCCR0B |= (1 << CS00);
  TCCR2A |= (1 << COM2B1) | (1 << WGM20) | (1 << WGM21);
  TCCR2B |= (1 << CS20);
  TCCR1B |= (1 << WGM12) | (1 << CS11);
  OCR1A = 40000;
  TIMSK1 |= (1 << OCIE1A);
}

void adc_setup() {
  ADMUX |= (1 << REFS0) | (1 << ADLAR);
  DIDR0 |= (1 << ADC0D);
  ADCSRA |= (1 << ADEN) | (1 << ADATE) | (1 << ADSC);
  ADCSRA |= (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);
}

void wheel(uint8_t pos) {
  pos = 255 - pos;
  if (pos < 85) {
    set_rgb(255 - pos * 3, 0, pos * 3);
  } else if (pos < 170) {
    pos = pos - 85;
    set_rgb(0, pos * 3, 255 - pos * 3);
  } else {
    pos = pos - 170;
    set_rgb(pos * 3, 255 - pos * 3, 0);
  }
}

void toggle_leds(uint8_t val) {
  uint8_t tolerance = UINT8_MAX / 20;
  uint8_t p25 = UINT8_MAX / 4;
  uint8_t p50 = UINT8_MAX / 2;
  uint8_t p75 = (UINT8_MAX / 4) * 3;
  uint8_t p100 = UINT8_MAX;

  if (val > p25 + tolerance)
    PORTB |= (1 << PB0);
  if (val < p25 - tolerance)
    PORTB &= ~(1 << PB0);

  if (val > p50 + tolerance)
    PORTB |= (1 << PB1);
  if (val < p50 - tolerance)
    PORTB &= ~(1 << PB1);

  if (val > p75 + tolerance)
    PORTB |= (1 << PB2);
  if (val < p75 - tolerance)
    PORTB &= ~(1 << PB2);

  if (val > p100 - tolerance)
    PORTB |= (1 << PB4);
  if (val < p100 - tolerance)
    PORTB &= ~(1 << PB4);
}

int main() {
  DDRD |= (1 << PD3) | (1 << PD5) | (1 << PD6);
  DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4);
  adc_setup();
  timer_setup();

  sei();

  for (;;) {
    if (read) {
      uint8_t val = ADCH;
      toggle_leds(val);
      wheel(val);
      read = false;
    }
  }

  return (0);
}

ISR(TIMER1_COMPA_vect) { read = true; }