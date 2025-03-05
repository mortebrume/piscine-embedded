#include "../include/uart.h"
#include "avr/io.h"

void uart_tx(unsigned char c) {
  while (!(UCSR0A & (1 << UDRE0)))
    ;
  UDR0 = c;
}

unsigned char uart_rx() {
  // Wait until there's data to be read.
  while (!(UCSR0A & (1 << RXC0)))
    ;
  return UDR0;
}

void uart_init() {
  UBRR0H = UBRRH_VALUE;
  UBRR0L = UBRRL_VALUE;

  UCSR0A |= (1 << U2X0);
  UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
  UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
}

int main() {
  uart_init();

  for (;;)
    uart_tx(uart_rx());
}