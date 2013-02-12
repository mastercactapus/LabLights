#include "cpu.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "control.h"
#include "types.h"


//prescale by 8
//set count to 125, interrupt will fire once per ms
#define _PRESCALE 8
#define _TICKS_PER_SECOND 125000 //F_CPU/_PRESCALE
#define _FADE_STEPS 256

#define _STEPS_PER_SECOND 1000
#define _SECONDS_PER_MIN 60
#define _COUNT (F_CPU/64)/5


#define lightB_on DDRA |= _BV(PA7)
#define lightA_on DDRB |= _BV(PB2)
#define lightB_off DDRA &= ~(_BV(PA7))
#define lightA_off DDRB &= ~(_BV(PB2))




//called when timer1 is up
ISR (TIM1_COMPA_vect) {
	// static uint16_t steps = 0;
	// static uint8_t seconds = 0;
	// static uint8_t minutes = 0;

	// if (steps >= _STEPS_PER_SECOND) {
	// 	steps=0;
	// 	seconds++;
	// } else {
	// 	steps++;
	// }

	// if (seconds >= _SECONDS_PER_MIN) {
	// 	seconds=0;
	// 	minutes++;
	// } else {
	// 	seconds++;
	// }

	
	if (OCR0A == 1) lightA_on;
	switch(lightA->status) {
		case ON:
			if (OCR0A > lightA->settings->power_level) OCR0A--;
			else if (OCR0A < lightA->settings->power_level) OCR0A++;
			break;
		case DIM:
			if (OCR0A > lightA->settings->dim_level) OCR0A--;
			else if (OCR0A < lightA->settings->dim_level) OCR0A++;
			break;
		case OFF:
			if (OCR0A > 0) OCR0A--;
			else lightA_off;
			break;
	}

	if (OCR0B == 1) lightB_on;
	switch(lightB->status) {
		case ON:
			if (OCR0B > lightB->settings->power_level) OCR0B--;
			else if (OCR0B < lightB->settings->power_level) OCR0B++;
			break;
		case DIM:
			if (OCR0B > lightB->settings->dim_level) OCR0B--;
			else if (OCR0B < lightB->settings->dim_level) OCR0B++;
			break;
		case OFF:
			if (OCR0B > 0) OCR0B--;
			else lightB_off;
			break;
	}

}

