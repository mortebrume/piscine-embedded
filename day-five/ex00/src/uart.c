#include "../include/uart.h"
#include "avr/io.h"
#include <stdint.h>

void uart_tx(unsigned char c) {
  while (!(UCSR0A & (1 << UDRE0)))
    ;
  UDR0 = c;
}

void uart_printhex(uint8_t n) {
  const char hex_chars[] = "0123456789abcdef";
  uart_tx(hex_chars[(n >> 4) & 0x0F]);
  uart_tx(hex_chars[n & 0x0F]);
}

void uart_printstr(char *s) {
  while (*s > 0)
    uart_tx(*s++);
}

void uart_init() {
  UBRR0H = UBRRH_VALUE;
  UBRR0L = UBRRL_VALUE;

  UCSR0A |= (1 << U2X0);
  UCSR0B |= (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
  UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
}