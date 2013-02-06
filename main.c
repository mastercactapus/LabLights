#define LIGHTS 2
#include "cpu.h"

#include <avr/io.h>
#include <util/delay.h>

#include "types.h"
#include "eeprom.h"
#include "config_hardware.h"
#include "control.h"


SETTINGS *lights[LIGHTS];

int main(void) {
  uint8_t i=0;

  lights[0] = read_settings((uint8_t *)0);
  lights[1] = read_settings((uint8_t *)1);
  
  setup_pwmAB();

  set_pwm_a(&lights[0]->current_level);
  set_pwm_b(&lights[1]->current_level);

  start_pwmAB();
  
  i = 0;
  uint8_t dir=1;
  while(1) {
    if (i == 0xff) {
		dir=0;
	} else if (i == 0x01) {
		dir=1;
	}
	dir == 1 ? i++ : i--;
	set_pwm_a(&i);
	_delay_ms(2);
  }

}

