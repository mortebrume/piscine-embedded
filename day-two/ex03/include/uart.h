#ifndef F_CPU
#define F_CPU 16000000
#endif

#ifndef UART_BAUDRATE
#define UART_BAUDRATE 115200
#endif

// Calculate the UBRR value for asynchronous double speed (p. 182)
// We are using double speed to be around 2% of error rate instead of 3%.
#define UBRR_VALUE ((F_CPU / (8UL * (UART_BAUDRATE))) - 1UL)
#define UBRRL_VALUE (UBRR_VALUE & 0xff)
#define UBRRH_VALUE (UBRR_VALUE >> 8)