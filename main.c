#include "cpu.h"

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>

#include "types.h"
#include "eeprom.h"
#include "config_hardware.h"
#include "control.h"
#include "serial.h"
#include "main.h"
#include "strings.h"

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

  lightA->settings->power_level = 0xfe;
  lightA->settings->dim_level = 0x88;
  lightA->status = ON;

  //send_string("\r\n\nLighting System Online\r\n\n");
  sei();
  while(1){
    flush();
    if (peek_byte() & _BV(0))
      bin_cmd();
    else
      txt_cmd();
  };
  return 0;
}

void bin_cmd(void) {
  REQUEST *req = malloc(sizeof(REQUEST));
  read_bytes((uint8_t*)req, sizeof(REQUEST));
  uint8_t chk = 43;
  chk += *((uint8_t*)req) + *(((uint8_t*)req)+1);
  if (chk == req->chk) {
    uint8_t res = proc_req(req);
    if (req->read_write == READ)
      send_byte(res);
  }
}

void txt_cmd(void) {
  char *line, *word;
  send_string("LabLights: ");
  line = read_line(32);
  REQUEST *req = calloc(1,sizeof(REQUEST));

  req->read_write = WRITE;

  word = get_word(line);
  if(s_eq(word,"lightA")) req->light_AB = LIGHT_A;
  else if(s_eq(word,"lightB")) req->light_AB = LIGHT_B;
  else if (s_eq(word,"dump_config")) {dump_config(); return;}
  else {send_string("Unknown command\r\n\n");return;}

  word = get_word(line);
  if (s_eq(word,"dim_level")) req->command = DIM_LEVEL;
  else if (s_eq(word,"power_level")) req->command = POWER_LEVEL;
  else if (s_eq(word,"status")) req->command = STATUS;
  else if (s_eq(word,"dim_delay")) req->command = DIM_DELAY;
  else if (s_eq(word,"off_delay")) req->command = OFF_DELAY;
  else {send_string("Unknown parameter\r\n\n");return;}

  word = get_word(line);
  uint8_t val = 0;
  if (s_eq(word,"on")) val = (uint8_t)ON;
  else if (s_eq(word,"off")) val = (uint8_t)OFF;
  else if (s_eq(word,"dim")) val = (uint8_t)DIM;
  else val = hex_to_uint8(word);
  req->value = val;

  proc_req(req);
}

void dump_config(void) {
  dump_light_config(lightA,"lightA");
  dump_light_config(lightB,"lightB");
}
void dump_light_config(LIGHT *light, char *light_name) {
  char *status;
  if (lightB->status == ON) status = "on";
  else if (lightB->status == DIM) status = "dim";
  else if (lightB->status == OFF) status = "off";
  else status = "broken";

  send_string(light_name);
  send_string(" -> \r\n");
  send_string("   status:  ");
    send_string(status);
    send_string("\r\n");
  send_string("   minutes: ");
    send_string(uint8_to_hex(light->minutes));
    send_string("\r\n");
  send_string("   settings -> \r\n");
  send_string("     off_delay:   ");
    send_string(uint8_to_hex(light->settings->auto_off_delay));
    send_string("\r\n");
  send_string("     dim_delay:   ");
    send_string(uint8_to_hex(light->settings->auto_dim_delay));
    send_string("\r\n");
  send_string("     power_level: ");
    send_string(uint8_to_hex(light->settings->power_level));
    send_string("\r\n");
  uint8_to_hex(light->settings->dim_level);
  send_string("     dim_level:   ");
    send_string(uint8_to_hex(light->settings->dim_level));
    send_string("\r\n\n");
}


uint8_t proc_req(REQUEST *req) {
  LIGHT *work = (req->light_AB == LIGHT_A ? lightA : lightB);

  volatile uint8_t *val;
  switch(req->command) {
    case STATUS:
    val = &work->status;
    break;
    case DIM_LEVEL:
    val = &work->settings->dim_level;
    break;
    case POWER_LEVEL:
    val = &work->settings->power_level;
    break;
    case DIM_DELAY:
    val = &work->settings->auto_dim_delay;
    break;
    case OFF_DELAY:
    val = &work->settings->auto_off_delay;
    break;
    default:
    return 0;
  }

  if (req->read_write == READ)
    return *val;

  *val = req->value;
  return 1;
}
