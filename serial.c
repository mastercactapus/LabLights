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


void setup_serial(void) {
  TX_PORT |= _BV(TX_PIN);
  DDRB |= _BV(TX_PIN);
}

void send_byte(uint8_t b)
{
  cli();
  b = ~b;
  TX_PORT &= ~(_BV(TX_PIN));            // start bit
  for(uint8_t i = 10; i; i--){        // 10 bits
    _delay_us( TX_DELAY );            // bit duration
    if(b & 1)
      TX_PORT &= ~(_BV(TX_PIN));        // data bit 0
    else
      TX_PORT |= _BV(TX_PIN);           // data bit 1 or stop bit
    b >>= 1;
  }
  sei();
}

void send_bytes(uint8_t *buf, uint8_t len) {
  for (uint8_t i = 0;i<len;i++){
    send_byte(buf[i]);
  }
}

uint8_t read_byte(void)
{
	uint8_t out=0;
	while (!(RX_PORT & _BV(RX_PIN)));
	cli();
	for (uint8_t i=10; i; i--){
		_delay_us(RX_DELAY);
		if (RX_PORT & _BV(i)) out |= (1 << i);
	}
	out = ~out;
	sei();
	return out;
}