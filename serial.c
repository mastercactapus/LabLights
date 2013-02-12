#include "cpu.h"

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "serial.h"
#include "types.h"

#define TX_DELAY 94 //tuned for 9600bps
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
  s_buffer->flag = 0;
  s_buffer->byte = 0;

}

void send_byte(uint8_t b)
{
  b = ~b;
  TX_PORT &= ~(_BV(TX_PIN));     // start bit
  for(uint8_t i = 10; i; i--){   // 10 bits
    _delay_us( TX_DELAY );       // bit duration
    if(b & 1)
      TX_PORT &= ~(_BV(TX_PIN)); // data bit 0
    else
      TX_PORT |= _BV(TX_PIN);    // data bit 1 or stop bit
    b >>= 1;
  }
}

void send_bytes(uint8_t *buf, uint8_t len) {
  for (uint8_t i = 0;i<len;i++){
    send_byte(buf[i]);
  }
}

ISR (PCINT1_vect) {
  if (s_buffer->flag) return;
	_delay_us(RX_DELAY/2);
	uint8_t read = 0;
	for (uint8_t i=8; i; i--){
		_delay_us(RX_DELAY);
    read >>= 1;
		if (RX_PORT & _BV(RX_PIN))
      read |= _BV(7);
	}
	s_buffer->byte = read;
	s_buffer->flag = 1;
  _delay_us(RX_DELAY);
}

uint8_t read_byte(void) {
	while(!s_buffer->flag);
	s_buffer->flag=0;
	return s_buffer->byte;
}

