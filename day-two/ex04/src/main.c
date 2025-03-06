#include "../include/uart.h"
#include "avr/interrupt.h"
#include "avr/io.h"
#include "util/delay.h"
#include <stdbool.h>
#include <string.h>

#define BUFFER_SIZE 32

const char *username = "saske";
const char *password = "openplz";

char useri[BUFFER_SIZE + 1];
char passi[BUFFER_SIZE + 1];

bool READING = false;

enum state { USER, PASS };
enum state s = USER;

void buff_write(unsigned char c) {
  char *buf = s == USER ? useri : passi;
  unsigned char co = s == USER ? c : '*';
  size_t len = strlen(buf);
  if (len != BUFFER_SIZE) {
    buf[len] = c;
    buf[len + 1] = '\0';
    uart_tx(co);
  }
}

void handle_backspace() {
  char *buf = s == USER ? useri : passi;
  size_t len = strlen(buf);
  if (len) {
    buf[len - 1] = '\0';
    uart_printstr("\b \b");
  }
}

int main() {
  DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2) | (1 << DDB4);

  uart_init();
  sei();

  for (;;) {
    memset(useri, 0, sizeof useri);
    memset(passi, 0, sizeof passi);
    s = USER;
    uart_printstr("Enter your login:\n\r");
    uart_printstr("\tusername: ");
    READING = true;

    while (READING)
      _delay_ms(20);

    s = PASS;
    uart_printstr("\n\r\tpassword: ");
    READING = true;

    while (READING)
      _delay_ms(20);

    if (!strncmp(username, useri, BUFFER_SIZE) &&
        !strncmp(password, passi, BUFFER_SIZE)) {
      uart_printstr("\n\r\n\rHello saske !\n\r");
      uart_printstr("Welcome to a truly beautiful terminal.\n\r");
      break;
    } else {
      uart_printstr("\n\rBad combination username/password\n\r\n\r");
      continue;
    }
  }

  for (;;) {
    PORTB ^= (1 << PORTB0) | (1 << PORTB1) | (1 << PORTB2) | (1 << PORTB4);
    _delay_ms(200);
  }
}

ISR(USART_RX_vect) {
  if (!READING)
    return;

  unsigned char c = UDR0;
  if (c == 127)
    handle_backspace();
  else if (c == '\r')
    READING = false;
  else
    buff_write(c);
}