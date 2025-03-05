#include "../include/uart.h"
#include "avr/interrupt.h"
#include "avr/io.h"

void uart_tx(unsigned char c) {
  // Only send data if it's ready to be sent.
  if (UCSR0A & (1 << UDRE0))
    UDR0 = c; // Register to send data (p. 200)
}

void uart_init() {
  // Set the BAUD rate for UART (p. 204)
  UBRR0H = UBRRH_VALUE;
  UBRR0L = UBRRL_VALUE;

  // Enable double speed mode (p. 200).
  UCSR0A |= (1 << U2X0);
  // Enable transmission (p. 201).
  UCSR0B |= (1 << TXEN0);
  // Use 8 bits character frame (p. 202).
  UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
}

int main() {
  uart_init();

  TCCR1B |= (1 << WGM12);
  // Enable CTC interrupt (p. 145).
  TIMSK1 |= (1 << OCIE1A);

  // Enable global interrupts.
  sei();

  // 1Mhz.
  OCR1A = 62499;
  TCCR1B |= (1 << CS12);

  for (;;)
    ;
}

// Interrupt Service Routine for the TIMER1 Interrupt.
ISR(TIMER1_COMPA_vect) { uart_tx('Z'); }