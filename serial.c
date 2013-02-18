#include "cpu.h"

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include "serial.h"
#include "types.h"

#define TX_DELAY 92 //tuned for 9600bps
#define TX_PORT PORTB
#define TX_PIN  PB0

#define RX_DELAY 94 //tuned for 9600bps
#define RX_PORT PINB
#define RX_PIN  PB1

SBUF *s_buffer;

void setup_serial(void) {
  TX_PORT |= _BV(TX_PIN);
  DDRB |= _BV(TX_PIN);
  PCMSK1 |= _BV(PCINT9);
  GIMSK |= _BV(PCIE1);
  s_buffer = malloc(sizeof(SBUF));
  s_buffer->count = 0;
}

void send_byte(uint8_t b)
{
  cli();
  b = ~b;
  TX_PORT &= ~(_BV(TX_PIN));
  _delay_us( TX_DELAY );
  for(uint8_t i = 8; i; i--){
    if(b & 1)
      TX_PORT &= ~(_BV(TX_PIN));
    else
      TX_PORT |= _BV(TX_PIN);
    b >>= 1;
    _delay_us(TX_DELAY);
  }
  TX_PORT |= _BV(TX_PIN);
  _delay_us(TX_DELAY);
  sei();
}

void send_bytes(uint8_t *buf, uint8_t len) {
  for (uint8_t i = 0;i<len;i++){
    send_byte(buf[i]);
  }
}

void send_string(char *str) {
  uint8_t i=0;
  while (str[i] != 0) {
    send_byte((uint8_t)str[i]);
    i++;
  }
}

ISR (PCINT1_vect) {
  if (RX_PORT & _BV(RX_PIN)) return;
  if (s_buffer->count >= BUFFER) {
    _delay_us(RX_DELAY * 9.5);
  } else {
    _delay_us(RX_DELAY/2);
    uint8_t read = 0;
    for (uint8_t i=8; i; i--){
      _delay_us(RX_DELAY);
      read >>= 1;
      if (RX_PORT & _BV(RX_PIN))
        read |= _BV(7);
    }
    s_buffer->byte[s_buffer->count++] = read;
    _delay_us(RX_DELAY);
    PORTA ^= _BV(PA1);
  }
}

void read_line(char *buf, uint8_t max_len) {
  uint8_t i=0;
  uint8_t c;
  while ((peek_byte() == 13) || (peek_byte() == 10)) read_byte();
  c = read_byte();
  send_byte(c);
  while ((c != 13) && (c != 10)) {
    buf[i] = (char)c;
    c=read_byte();
    if (i<max_len) {
      i++;
      send_byte(c);
    }
  }
  send_byte(10);
  send_byte(13);
}
void flush(void) {
  memset(&s_buffer->byte,0,BUFFER);
  s_buffer->count = 0;
}
void read_bytes(uint8_t *buf, uint8_t len) {
  for (uint8_t i=0;i<len;i++){
    buf[i] = read_byte();
  }
}
uint8_t peek_byte(void) {
  while(s_buffer->count == 0);
  return s_buffer->byte[0];
}
uint8_t read_byte(void) {
	while(s_buffer->count == 0);
	uint8_t out =s_buffer->byte[0];
  memcpy(&s_buffer->byte, &s_buffer->byte + 1, --s_buffer->count);
	return out;
}
