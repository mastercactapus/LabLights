#include <stdlib.h>
#include <avr/io.h>
#include "config_hardware.h"

//set up timer0 for fast pwm
void setup_pwmAB(void) {
	TCCR0A |= _BV(COM0A1) | _BV(COM0B1) | _BV(WGM01) | _BV(WGM00);
}

//set up timer1 for fade control and timekeeping
void setup_sliderAB(void) {
	TCCR1B |= _BV(WGM13) | _BV(WGM12);
	ICR1 = 250;
	OCR1A = 0;
	TIMSK1 |= _BV(OCF1A);
}

//stop then starts timer0
void start_pwmAB(void) {
	stop_pwmAB();
	OCR0A = 0;
	OCR0B = 0;
	TCCR0B |= _BV(CS00); //no prescaler - int clock
}

//stop then start timer1
void start_sliderAB(void) {
	stop_sliderAB();
	TCCR1B |= _BV(CS11); // /8 prescaler - int clock
	TCNT1 = 0;
}

//stop timer0
void stop_pwmAB(void) {
	TCCR0B &= ~(_BV(CS00) | _BV(CS01) | _BV(CS02));
}

//stop timer1
void stop_sliderAB(void) {
	TIMSK1 &= ~(_BV(TOIE1));
	TCCR1B &= ~(_BV(CS10) | _BV(CS11) | _BV(CS12));
}
