#include "cpu.h"

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <string.h>

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
  lightA->settings->dim_level = 0x88;
  lightA->status = ON;

  sei();

  send_string("\r\n\nLighting System Online\r\n\n");


  while(1){
    char *line;
    send_string("LabLights: ");
    line = read_line();
    if (strcmp(line, "light on") == 0) {lightA->status = ON;}
    else if (strcmp(line, "light dim") == 0) {lightA->status = DIM;}
    else if (strcmp(line, "light off") == 0) {lightA->status = OFF;}
    else {send_string("Unknown Command!\r\n\n");}
  };
  return 0;
}

