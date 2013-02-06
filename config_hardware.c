#include <stdlib.h>
#include <avr/io.h>
#include "config_hardware.h"

void setup_pwmAB(void) {
	TCCR0A |= _BV(WGM00) | _BV(WGM01) | _BV(COM0A1);
	TCCR0B |= _BV(COM0B1);
	DDRB |= _BV(PB2);
	DDRA |= _BV(PA7);
}

void start_pwmAB(void) {
	stop_pwmAB();
	TCCR0B |= _BV(CS00); //no prescaler - int clock
}

void stop_pwmAB(void) {
	TCCR0B &= ~(_BV(CS00) | _BV(CS01) | _BV(CS02));
}
