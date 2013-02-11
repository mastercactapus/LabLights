#include <avr/eeprom.h>
#include <stdlib.h>
#include "types.h"
#include "eeprom.h"

LIGHT_SETTINGS EEMEM EE_lightA = _DEFAULT_SETTINGS;
LIGHT_SETTINGS EEMEM EE_lightB = _DEFAULT_SETTINGS;

void read_settings(void) {
	eeprom_busy_wait();


	lightA->settings = malloc(sizeof(LIGHT_SETTINGS));
	lightB->settings = malloc(sizeof(LIGHT_SETTINGS));

	eeprom_read_block(lightA->settings, &EE_lightA, sizeof(LIGHT_SETTINGS));
	eeprom_read_block(lightB->settings, &EE_lightB, sizeof(LIGHT_SETTINGS));

	if (!is_valid_settings(lightA->settings)) set_defaults(lightA->settings);
	if (!is_valid_settings(lightB->settings)) set_defaults(lightB->settings);
}

void save_settings() {
	eeprom_busy_wait();

	eeprom_update_block(lightA->settings, &EE_lightA, sizeof(LIGHT_SETTINGS));
  eeprom_update_block(lightB->settings, &EE_lightB, sizeof(LIGHT_SETTINGS));
}

void set_defaults(LIGHT_SETTINGS *settings) {
	LIGHT_SETTINGS out = _DEFAULT_SETTINGS;
	free(settings);
	settings = &out;
}

uint8_t is_valid_settings(LIGHT_SETTINGS *settings) { //checksum?
	uint16_t chk = 0;
	chk += settings->auto_off_delay;
	chk += settings->auto_dim_delay;
	chk += settings->power_level;
	chk += settings->dim_level;

	return (settings->checksum == ~chk);
}
