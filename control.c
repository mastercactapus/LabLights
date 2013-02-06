#include "cpu.h"
#include <avr/io.h>
#include <util/delay.h>
#include "control.h"

void fade_pwm_a(uint8_t *time, uint8_t *pwm_level) {
	uint8_t up = 1;
	if (*pwm_level < OCR0A) up = 0;

	uint16_t delay = up ? (*time * 100 / (*pwm_level - OCR0A)) : (*time * 100 / (OCR0A - *pwm_level));

	if (OCR0A == *pwm_level) {
		custom_delay(*time * 100);
	} else if (up) {
			while (OCR0A < *pwm_level) {
				OCR0A++;
				custom_delay(delay);
			}
	} else {
			while (OCR0A > *pwm_level) {
				OCR0A--;
				custom_delay(delay);
			}
	}
}
void fade_pwm_b(uint8_t *time, uint8_t *pwm_level) {
	uint8_t up = 1;
	if (*pwm_level < OCR0B) up = 0;

	uint16_t delay = up ? ((*pwm_level - OCR0B) * 100 / *time) : ((OCR0B - *pwm_level) * 100 / *time);

	if (OCR0B == *pwm_level) {
		custom_delay(*time * 100);
	} else if (up) {
			while (OCR0B < *pwm_level) {
				OCR0B++;
				custom_delay(delay);
			}
	} else {
			while (OCR0B > *pwm_level) {
				OCR0B--;
				custom_delay(delay);
			}
	}
}

void set_pwm_a(uint8_t *pwm_level) {
	OCR0A = *pwm_level;
}
void set_pwm_b(uint8_t *pwm_level) {
	OCR0B = *pwm_level;
}

void custom_delay(uint16_t itr) {
	while (itr) {
		itr--;
		_delay_us(10);
	}
}
