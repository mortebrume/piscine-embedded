#include "../include/uart.h"
#include "avr/interrupt.h"
#include "avr/io.h"
#include "util/delay.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 32

typedef struct {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
} RGB;

volatile RGB colour;
char buffer[BUFFER_SIZE + 1];
volatile bool ready = false;

void init_rgb() {
  TCCR0A |= (1 << WGM00) | (1 << WGM01) | (1 << COM0A1) | (1 << COM0B1);
  TCCR0B |= (1 << CS00);
  TCCR2A |= (1 << COM2B1) | (1 << WGM20) | (1 << WGM21);
  TCCR2B |= (1 << CS20);

  OCR0A = 0;
  OCR0B = 0;
  OCR2B = 0;
}

void set_rgb() {
  OCR0B = colour.red;
  OCR0A = colour.green;
  OCR2B = colour.blue;
}

// Parse the buffer and fill the RGB struct if it's a valid RGB.
void create_rgb() {
  if (strlen(buffer) == 7 && buffer[0] == '#' &&
      strspn(&buffer[1], "0123456789ABCDEFabcdef") == 6) {
    colour.red = strtol(&buffer[1], NULL, 16) >> 16;
    colour.green = strtol(&buffer[3], NULL, 16) >> 8;
    colour.blue = strtol(&buffer[5], NULL, 16);
    ready = true;
  }
}

void buff_write(unsigned char c) {
  size_t len = strlen(buffer);
  if (len != BUFFER_SIZE) {
    buffer[len] = c;
    buffer[len + 1] = '\0';
    uart_tx(c);
  }
}

void handle_backspace() {
  size_t len = strlen(buffer);
  if (len) {
    buffer[len - 1] = '\0';
    uart_printstr("\b \b");
  }
}

int main() {
  DDRD |= (1 << PD3) | (1 << PD5) | (1 << PD6);

  uart_init();
  sei();
  init_rgb();

  for (;;) {
    if (ready) {
      set_rgb();
      ready = false;
    }
    _delay_ms(50);
  }

  return (0);
}

ISR(USART_RX_vect) {
  unsigned char c = UDR0;

  if (c == '\r') {
    create_rgb();
    // Clears the screen and reset the buffer.
    uart_printstr("\x1b[2J\x1b[H");
    memset(buffer, 0, sizeof buffer);
  } else if (c == 127) {
    handle_backspace();
  } else
    buff_write(c);
}