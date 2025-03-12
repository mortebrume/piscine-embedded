#pragma once

#ifndef F_CPU
#define F_CPU 16000000
#endif

#ifndef UART_BAUDRATE
#define UART_BAUDRATE 115200
#endif

#define UBRR_VALUE ((F_CPU / (8UL * (UART_BAUDRATE))) - 1UL)
#define UBRRL_VALUE (UBRR_VALUE & 0xff)
#define UBRRH_VALUE (UBRR_VALUE >> 8)

#include <stdint.h>

void uart_tx(unsigned char c);
void uart_printstr(char *s);
void uart_printhex(uint8_t n);
void uart_printnbr(int32_t n);
void uart_init();