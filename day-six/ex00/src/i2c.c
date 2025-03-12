#include "../include/i2c.h"
#include "../include/uart.h"

void print_status(uint8_t status_code) {
  switch (status_code) {
  case TW_START:
    uart_printstr("START acknowledge.\r\n");
    break;
  case TW_REP_START:
    uart_printstr("REPEATED START acknowledge.\r\n");
    break;
  case TW_MT_SLA_ACK:
    uart_printstr("Master Transmitter: Slave ACK\r\n");
    break;
  case TW_MT_SLA_NACK:
    uart_printstr("Master Transmitter : Slave NACK\r\n");
    break;
  case TW_MT_DATA_ACK:
    uart_printstr("Master Transmitter : Data ACK\r\n");
    break;
  case TW_MT_DATA_NACK:
    uart_printstr("Master Transmitter: Data NACK\r\n");
    break;
  case TW_MR_SLA_ACK:
    uart_printstr("Master Receiver : Slave ACK\r\n");
    break;
  case TW_MR_SLA_NACK:
    uart_printstr("Master Receiver : Slave NACK\r\n");
    break;
  case TW_MR_DATA_ACK:
    uart_printstr("Master Receiver : Data ACK\r\n");
    break;
  case TW_MR_DATA_NACK:
    uart_printstr("Master Receiver : Data NACK\r\n");
    break;
  case TW_MT_ARB_LOST:
    uart_printstr("Arbitration Lost\r\n");
    break;
  case TW_ST_SLA_ACK:
    uart_printstr("Slave Transmitter : Slave ACK\r\n");
    break;
  case TW_ST_ARB_LOST_SLA_ACK:
    uart_printstr("Arbitration Lost in SLA+R/W, Slave ACK\r\n");
    break;
  case TW_ST_DATA_ACK:
    uart_printstr("Slave Transmitter : Data ACK\r\n");
    break;
  case TW_ST_DATA_NACK:
    uart_printstr("Slave Transmitter : Data NACK\r\n");
    break;
  case TW_ST_LAST_DATA:
    uart_printstr("Slave Transmitter : Last Data\r\n");
    break;
  case TW_SR_SLA_ACK:
    uart_printstr("Slave Receiver : Slave ACK\r\n");
    break;
  case TW_SR_ARB_LOST_SLA_ACK:
    uart_printstr("Arbitration Lost in SLA+R/W, Slave ACK\r\n");
    break;
  case TW_SR_GCALL_ACK:
    uart_printstr("General Call : Slave ACK\r\n");
    break;
  case TW_SR_ARB_LOST_GCALL_ACK:
    uart_printstr("Arbitration Lost in General Call, Slave ACK\r\n");
    break;
  case TW_SR_DATA_ACK:
    uart_printstr("Slave Receiver : Data ACK\r\n");
    break;
  case TW_SR_DATA_NACK:
    uart_printstr("Slave Receiver : Data NACK\r\n");
    break;
  case TW_SR_GCALL_DATA_ACK:
    uart_printstr("General Call : Data ACK\r\n");
    break;
  case TW_SR_GCALL_DATA_NACK:
    uart_printstr("General Call : Data NACK\r\n");
    break;
  case TW_SR_STOP:
    uart_printstr("Slave Receiver : STOP received\r\n");
    break;
  case TW_NO_INFO:
    uart_printstr("No state information available\r\n");
    break;
  case TW_BUS_ERROR:
    uart_printstr("Bus Error\r\n");
    break;
  default:
    uart_printstr("Unknown Status Code\r\n");
  }
}

// Wait until the I2C has finished his job, when TWINT = 0 (p. 239).
void i2c_wait() {
  while (!(TWCR & (1 << TWINT)))
    ;
}

void i2c_init() {
  // SDA and SCL input, required by I2C.
  DDRC &= ~(1 << DDC5) & ~(1 << DDC4);
  // PULLUP, required by I2C.
  PORTC |= (1 << PORTC5) | (1 << PORTC4);
  // Set the bitrate to 100kHz (p. 238).
  TWBR = ((F_CPU / 100000) - 16) / 12;
  // Enable I2C (p. 240).
  TWCR |= (1 << TWEN);
}

void i2c_start() {
  // Send the start signal to the I2C (p. 240).
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTA);
  i2c_wait();
  print_status(TWS);
}

void i2c_stop() {
  // Send the stop signal to the I2C.
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}