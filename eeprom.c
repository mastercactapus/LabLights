#include <avr/eeprom.h>
#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include "types.h"
#include "eeprom.h"

LIGHT_SETTINGS EEMEM EE_lightA = _DEFAULT_SETTINGS;
LIGHT_SETTINGS EEMEM EE_lightB = _DEFAULT_SETTINGS;

void read_settings(void) {
	cli();
	eeprom_busy_wait();

	lightA->settings = malloc(sizeof(LIGHT_SETTINGS));
	lightB->settings = malloc(sizeof(LIGHT_SETTINGS));

	lightA->status = OFF;
	lightB->status = OFF;

	lightA->minutes = 0;
	lightB->minutes = 0;

	eeprom_read_block(lightA->settings, &EE_lightA, sizeof(LIGHT_SETTINGS));
	eeprom_read_block(lightB->settings, &EE_lightB, sizeof(LIGHT_SETTINGS));

	if (!is_valid_settings(lightA->settings)) set_defaults(lightA->settings);
	if (!is_valid_settings(lightB->settings)) set_defaults(lightB->settings);
	sei();
}

void save_settings() {
	lightA->settings->checksum = calc_checksum(lightA->settings);
	lightB->settings->checksum = calc_checksum(lightB->settings);
	eeprom_busy_wait();

	eeprom_update_block(lightA->settings, &EE_lightA, sizeof(LIGHT_SETTINGS));
  eeprom_update_block(lightB->settings, &EE_lightB, sizeof(LIGHT_SETTINGS));
}

void set_defaults(LIGHT_SETTINGS *settings) {
	memset(settings,0,sizeof(LIGHT_SETTINGS));
	settings->checksum = calc_checksum(settings);
}

uint16_t calc_checksum(LIGHT_SETTINGS *settings) {
	uint16_t chk = 0;
	chk += settings->auto_off_delay;
	chk += settings->auto_dim_delay;
	chk += settings->power_level;
	chk += settings->dim_level;
	return ~chk;
}

uint8_t is_valid_settings(LIGHT_SETTINGS *settings) { //checksum?
	return (settings->checksum == calc_checksum(settings));
}
