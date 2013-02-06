#include <avr/eeprom.h>
#include <stdlib.h>
#include "types.h"
#include "eeprom.h"

SETTINGS* read_settings(uint8_t *light_index) {
	eeprom_busy_wait();
	uint16_t offset = *light_index * sizeof(SETTINGS);
	SETTINGS *out = malloc(sizeof(SETTINGS));
	eeprom_read_block(out, &offset, sizeof(SETTINGS));
	if (is_valid_settings(out)) {
		//set current power level to 0 if resume is disabled
		if (!out->resume_enable) {
			out->current_level = 0;
		}
		return out;
	} else {
		*out = (SETTINGS)_DEFAULT_SETTINGS;
	}
	return out;
}

void save_settings(uint8_t *light_index, SETTINGS *settings) {
	eeprom_busy_wait();
  uint16_t offset = *light_index * sizeof(SETTINGS);
  eeprom_update_block(settings, &offset, sizeof(SETTINGS));
}

uint8_t is_valid_settings(SETTINGS *settings) {
	return (settings->header == _HEADER) && (settings->revision == _REVISION);
}
