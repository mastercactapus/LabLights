#include <avr/eeprom.h>
#include "types.h"

SETTINGS* read_settings(uint8_t *light_index) {
	eeprom_busy_wait();
	uint16_t offset = *light_index * sizeof(SETTINGS);
	SETTINGS *out;
	eeprom_read_block(out, &offset, sizeof(SETTINGS));
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
