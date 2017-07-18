#ifndef _UART_H_
#define _UART_H_

#include <avr/io.h>

typedef struct _uart_buffer_
{
  uint8_t buffer[UART_BUFFER_SIZE];
  uint8_t write, read;
} uart_buffer;

void
uart_init();

uint8_t
uart_write(uint8_t, uint8_t*);
uint8_t
uart_read(uint8_t, uint8_t*);
uint8_t
uart_read_until(uint8_t, uint8_t*, uint8_t);

void
uart_putc(char);
uint8_t
uart_puts(char*);

uint8_t
uart_available();
uint8_t
uart_find(char);
char
uart_getc();
uint8_t
uart_gets(uint8_t, char*);
uint8_t
uart_readline(uint8_t, char*);

#endif
