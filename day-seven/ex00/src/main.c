#include "../include/uart.h"
#include "avr/io.h"
#include <stdint.h>

void mem_wait() {
  while (EECR & (1 << EEPE))
    ;
}

uint8_t mem_read(uint16_t addr) {
  EEAR = addr;
  mem_wait();
  EECR |= (1 << EERE);
  return EEDR;
}

void print_addr(uint16_t addr) {
  uart_printstr("0000");
  uart_printhex((addr >> 8) & 0xFF);
  uart_printhex(addr & 0xFF);
  uart_tx(' ');
}

void mem_dump() {
  for (uint16_t addr = 0; addr <= E2END; addr += 16) {
    print_addr(addr);
    for (uint8_t i = 0; i < 16; i++) {
      if (addr + i > E2END)
        break;
      uart_printhex(mem_read(addr + i));
      if (i % 2)
        uart_tx(' ');
    }
    uart_printstr("\n\r");
  }
}

int main() {
  uart_init();
  mem_dump();

  return 0;
}