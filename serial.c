#include "cpu.h"

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "serial.h"

#define TX_DELAY 94 //tuned for 9600bps
#define TX_PORT PORTB
#define TX_PIN  PB0

#define RX_DELAY 94 //tuned for 9600bps
#define RX_PORT PORTB
#define RX_PIN  PB1

uint8_t data_flag = 0;
uint8_t data_byte = 0;

void setup_serial(void) {
  TX_PORT |= _BV(TX_PIN);
  DDRB |= _BV(TX_PIN);
  PCMSK1 |= _BV(PCINT9);
  GIMSK |= _BV(PCIE1);
}

void send_byte(uint8_t b)
{
  cli();
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
  sei();
}

void send_bytes(uint8_t *buf, uint8_t len) {
  for (uint8_t i = 0;i<len;i++){
    send_byte(buf[i]);
  }
}

ISR (PCINT0_vect) {
	cli();

	_read_byte();

	sei();
}

uint8_t read_byte(void) {
	while(!data_flag);
	data_flag=0;
	return data_byte;
}

void _read_byte(void)
{
	data_byte=0;
	for (uint8_t i=10; i; i--){
		_delay_us(RX_DELAY);
		if (RX_PORT & _BV(i)) data_byte |= (1 << i);
	}
	data_byte = ~data_byte;
	data_flag = 1;
}
