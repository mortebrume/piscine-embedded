#include "../include/uart.h"
#include "avr/interrupt.h"
#include "avr/io.h"

void uart_tx(unsigned char c) {
  while (!(UCSR0A & (1 << UDRE0)))
    ;
  UDR0 = c;
}

static void uart_printstr(char *s) {
  // Transmit character until NULL is reached.
  while (*s > 0)
    uart_tx(*s++);
  // LF & CR.
  uart_tx('\n');
  uart_tx('\r');
}

void uart_init() {
  UBRR0H = UBRRH_VALUE;
  UBRR0L = UBRRL_VALUE;

  UCSR0A |= (1 << U2X0);
  UCSR0B |= (1 << TXEN0);
  UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
}

int main() {
  uart_init();

  TCCR1B |= (1 << WGM12);
  TIMSK1 |= (1 << OCIE1A);

  sei();

  // 0.5Mhz.
  OCR1A = 31249;
  // 1024 prescaler.
  TCCR1B |= (1 << CS10) | (1 << CS12);

  for (;;)
    ;
}

ISR(TIMER1_COMPA_vect) { uart_printstr("Hello World!"); }