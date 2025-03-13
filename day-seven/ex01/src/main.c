#include "../include/uart.h"
#include "avr/interrupt.h"
#include "avr/io.h"
#include "util/delay.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// clang-format off

uint16_t addr_i;
uint16_t val_i;
char buffer[BUFFER_SIZE + 1];
volatile bool ready = false;

void mem_wait() {
  while (EECR & (1 << EEPE));
}

uint8_t mem_read(uint16_t addr) {
  EEAR = addr;
  mem_wait();
  EECR |= (1 << EERE);
  return EEDR;
}

bool parse_param() {
  if (strlen(buffer) != strspn(buffer, " 0123456789ABCDEFabcdef"))
    return false;

  char *endptr;
  addr_i = (uint16_t)strtol(buffer, &endptr, 16);

  if (*endptr != ' ') return false;
  while (*endptr == ' ') endptr++;
  if (*endptr == '\0') return false;

  val_i = (uint16_t)strtol(endptr, NULL, 16);
  return true;
}

void print_addr(uint16_t addr) {
  uart_printstr("0000");
  uart_printhex((addr >> 8) & 0xFF);
  uart_printhex(addr & 0xFF);
  uart_tx(' ');
}

void mem_dump(uint16_t highlight) {
  for (uint16_t addr = 0; addr <= E2END; addr += 16) {
    print_addr(addr);
    for (uint8_t i = 0; i < 16; i++) {
      if (addr + i > E2END) break;

      if (addr + i == highlight) uart_printstr("\x1b[31m");
      uart_printhex(mem_read(addr + i));
      uart_printstr("\x1b[0m");

      if (i % 2) uart_tx(' ');
    }
    uart_printstr("\n\r");
  }
}

bool mem_write() {
  if (addr_i > E2END || mem_read(addr_i) == val_i) return false;

  mem_wait();
  EEAR = addr_i;
  EEDR = val_i;
  EECR |= (1 << EEMPE);
  EECR |= (1 << EEPE);
  return true;
}

int main() {
  uart_init();
  sei();

  for (;;) {
    if (!ready)
      continue;

    ready = false;
    if (!parse_param())
      uart_printstr("\x1b[2J\x1b[H");
    else {
      uint16_t highlight = mem_write() ? addr_i : UINT16_MAX;
      mem_dump(highlight);
    }
    memset(buffer, 0, sizeof buffer);
    _delay_ms(30);
  }

  return 0;
}

ISR(USART_RX_vect) {
  unsigned char c = UDR0;

  if (c == '\r') {
    uart_printstr("\n\r");
    ready = true;
  } else if (c == 127) {
    handle_backspace();
  } else
    buff_write(c);
}