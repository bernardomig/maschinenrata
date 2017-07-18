#include "hardware/uart.h"

#include <string.h>

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>

#include "config.h"

#define UART_DEMULT (F_CPU / 16 / UART_BAUDRATE - 1)

static uart_buffer uart_incoming, uart_outcoming;

void
uart_init()
{
  UBRR0H = (uint8_t)((UART_DEMULT) >> 8);
  UBRR0L = (uint8_t)(UART_DEMULT);

  UCSR0A = 0;
  UCSR0B = 0
           /* enable receiver and transmiter */
           | _BV(TXEN0) | _BV(RXEN0) | _BV(RXCIE0);
  UCSR0C = 0 | (3 << UCSZ00);
}

uint8_t
uart_write(uint8_t len, uint8_t* data)
{
  uint8_t i;
  for (i = 0; i < len; ++i) {
    uart_outcoming.buffer[uart_outcoming.write] = data[i];
    uart_outcoming.write = (uart_outcoming.write + 1) % UART_BUFFER_SIZE;
  }
  if (!(UCSR0C & (1 << UDRIE0))) {
    UCSR0B |= _BV(UDRIE0);
  }
  return len;
}

uint8_t
uart_read(uint8_t len, uint8_t* buffer)
{
  uint8_t read = 0;
  while (read < len && uart_available()) {
    buffer[read] = uart_incoming.buffer[uart_incoming.read];
    uart_incoming.read = (uart_incoming.read + 1) % UART_BUFFER_SIZE;
    ++read;
  }
  return read;
}

uint8_t
uart_read_until(uint8_t len, uint8_t* buffer, uint8_t delim)
{
  uint8_t read = 0;
  while (read < len && uart_available()) {
    uint8_t buf = uart_incoming.buffer[uart_incoming.read];
    buffer[read] = buf;
    uart_incoming.read = (uart_incoming.read + 1) % UART_BUFFER_SIZE;
    ++read;
    if (buf == delim)
      break;
  }
  return read;
}

void
uart_putc(char ch)
{
  uart_write(1, (uint8_t*)&ch);
}

uint8_t
uart_puts(char* str)
{
  return uart_write(strlen(str), (uint8_t*)str);
}

uint8_t
uart_available()
{
  if (uart_incoming.write >= uart_incoming.read) {
    return uart_incoming.write - uart_incoming.read;
  } else {
    return UART_BUFFER_SIZE - uart_incoming.read + uart_incoming.write;
  }
}

uint8_t
uart_find(char delimiter)
{
  uint8_t seek = 0;
  uint8_t ptr;
  while ((ptr = (uart_incoming.read + seek) % UART_BUFFER_SIZE) !=
         uart_incoming.write) {
    ++seek;
    if (uart_incoming.buffer[ptr] == delimiter)
      return seek;
  }
  return 0;
}

char
uart_getc()
{
  char ch = 0;
  uart_read(1, (uint8_t*)&ch);
  return ch;
}

uint8_t
uart_gets(uint8_t len, char* str)
{
  return uart_read(len, (uint8_t*)str);
}

uint8_t
uart_readline(uint8_t len, char* str)
{
  uint8_t read = uart_read_until(len, (uint8_t*)str, (uint8_t)'\n');
  str[read] = '\0';
  return read + 1;
}

ISR(USART_UDRE_vect)
{
  if (uart_outcoming.write != uart_outcoming.read) {
    UDR0 = uart_outcoming.buffer[uart_outcoming.read];
    uart_outcoming.read = (uart_outcoming.read + 1) % UART_BUFFER_SIZE;
  } else {
    UCSR0B &= ~_BV(UDRIE0);
  }
}

ISR(USART_RX_vect)
{
  uart_incoming.buffer[uart_incoming.write] = UDR0;
  uart_incoming.write = (uart_incoming.write + 1) % UART_BUFFER_SIZE;
}
