#include "../include/uart.h"
#include "avr/interrupt.h"
#include "avr/io.h"
#include <ctype.h>

void uart_tx(unsigned char c) {
  while (!(UCSR0A & (1 << UDRE0)))
    ;
  UDR0 = c;
}

void uart_init() {
  UBRR0H = UBRRH_VALUE;
  UBRR0L = UBRRL_VALUE;

  UCSR0A |= (1 << U2X0);
  // Enable interrupt on the RXC0 flag (p. 201).
  UCSR0B |= (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
  UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
}

int main() {
  uart_init();

  sei();

  for (;;)
    ;
}

// Interrupt Service Routine for the RX Interrupt.
ISR(USART_RX_vect) {
  unsigned char c = UDR0;

  if (islower(c))
    c = toupper(c);
  else if (isupper(c))
    c = tolower(c);

  uart_tx(c);
}