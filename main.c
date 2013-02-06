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

  lights[0] = read_settings((uint8_t *)0);
  lights[1] = read_settings((uint8_t *)1);
  
  setup_pwmAB();

  set_pwm_a(&lights[0]->current_level);
  set_pwm_b(&lights[1]->current_level);

  start_pwmAB();
  
  fade_pwm_a((uint8_t *) 1000,(uint8_t *)  0xff);
  fade_pwm_a((uint8_t *) 500,(uint8_t *)  0x4f);
  fade_pwm_a((uint8_t *) 500,(uint8_t *)  0xcf);
  fade_pwm_a((uint8_t *) 1000,(uint8_t *)  0x00);
  fade_pwm_a((uint8_t *) 8000,(uint8_t *)  0xff);

}

