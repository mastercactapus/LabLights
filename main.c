#include "cpu.h"

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/interrupt.h>

#include "types.h"
#include "eeprom.h"
#include "config_hardware.h"
#include "control.h"
#include "serial.h"

LIGHT *lightA;
LIGHT *lightB;

int main(void) {


  lightA = malloc(sizeof(LIGHT));
  lightB = malloc(sizeof(LIGHT));
  DDRA |= _BV(PA1);
  setup_serial();
  read_settings();

  setup_pwmAB();
  setup_sliderAB();

  start_pwmAB();
  start_sliderAB();

  lightA->settings->power_level = 0xff;
  lightA->settings->dim_level = 0x10;
  lightA->status = ON;

  sei();
  

  _delay_ms(1000);
  lightA->status = DIM;


  _delay_ms(1000);
  lightA->status = OFF;


  while (1) {
    uint8_t t = read_byte();
    send_byte(t);
  };
  return 0;
}

