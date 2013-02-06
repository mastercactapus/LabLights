#define F_CPU 1000000UL
#define LIGHTS 2

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

#include "types.h"
#include "eeprom.h"
#include "config_hardware.h"


SETTINGS *lights[LIGHTS];

int main(void) {
  uint8_t i=0;

  lights[0] = read_settings((uint8_t *)0);
  lights[1] = read_settings((uint8_t *)1);
  
  
  set_pwm_a(&lights[0]->current_level);
  set_pwm_b(&lights[1]->current_level);
  
  setup_pwmAB();
  
  start_pwmAB();
  
  while(1) {
    PORTA ^= _BV(PA1);
    _delay_ms(1000);
  }

}

