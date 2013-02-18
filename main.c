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

  sei();
  send_string("\r\n\nLighting System Online\r\n\n");
  while(1){
    if (peek_byte() & _BV(0))
      bin_cmd();
    else
      txt_cmd();
  };
  return 0;
}


void bin_cmd(void) {
  REQUEST *req = malloc(sizeof(REQUEST));
  read_bytes(req, sizeof(REQUEST));

}

void txt_cmd(void) {
  char line[48];
  send_string("LabLights: ");
  read_line(line, 48);
  REQUEST *req = malloc(sizeof(REQUEST));

  req->read_write = WRITE;

  char word[32];
  get_word(word,line);
  if(scmp(word,"lightA")) req->light_AB = LIGHT_A;
  else if(scmp(word,"lightB")) req->light_AB = LIGHT_B;
  else if (scmp(word,"dump_config")) {dump_config(); return;}
  else {send_string("Unknown command\r\n\n");return;}

  get_word(word,line);
  if (scmp(word,"dim_level")) req->command = DIM_LEVEL;
  else if (scmp(word,"power_level")) req->command = POWER_LEVEL;
  else if (scmp(word,"status")) req->command = STATUS;
  else if (scmp(word,"dim_delay")) req->command = DIM_DELAY;
  else if (scmp(word,"off_delay")) req->command = OFF_DELAY;
  else {send_string("Unknown command\r\n\n");return;}

  get_word(word,line);
  uint8_t val = 0;
  if (strcmp(word,"on")) val = ON;
  else if (strcmp(word,"off")) val = OFF;
  else if (strcmp(word,"dim")) val = DIM;
  else val = hex_to_uint8(word);
  req->value = val;


}

void dump_config(void) {
  char buf[8];
  if (lightA->status == ON) memcpy(buf, "on", 3);
  else if (lightA->status == DIM) memcpy(buf, "dim", 4);
  else if (lightA->status == OFF) memcpy(buf, "off", 4);
  else memcpy(buf, "broken", 7);

  send_string(" lightA -> \r\n");
  send_string("   status:  ");send_string(buf);send_string("\r\n");
  uint8_to_hex(buf,lightA->minutes);
  send_string("   minutes: ");send_string(buf);send_string("\r\n");
  send_string("   settings -> \r\n");
  uint8_to_hex(buf,lightA->settings->auto_off_delay);
  send_string("     off_delay:   ");send_string(buf);send_string("\r\n");
  uint8_to_hex(buf,lightA->settings->auto_dim_delay);
  send_string("     dim_delay:   ");send_string(buf);send_string("\r\n");
  uint8_to_hex(buf,lightA->settings->power_level);
  send_string("     power_level: ");send_string(buf);send_string("\r\n");
  uint8_to_hex(buf,lightA->settings->dim_level);
  send_string("     dim_level:   ");send_string(buf);send_string("\r\n");
  send_string("\r\n");


  if (lightB->status == ON) memcpy(buf, "on", 3);
  else if (lightB->status == DIM) memcpy(buf, "dim", 4);
  else if (lightB->status == OFF) memcpy(buf, "off", 4);
  else memcpy(buf, "broken", 7);

  send_string(" lightB -> \r\n");
  send_string("   status:  ");send_string(buf);send_string("\r\n");
  uint8_to_hex(buf,lightB->minutes);
  send_string("   minutes: ");send_string(buf);send_string("\r\n");
  send_string("   settings -> \r\n");
  uint8_to_hex(buf,lightB->settings->auto_off_delay);
  send_string("     off_delay:   ");send_string(buf);send_string("\r\n");
  uint8_to_hex(buf,lightB->settings->auto_dim_delay);
  send_string("     dim_delay:   ");send_string(buf);send_string("\r\n");
  uint8_to_hex(buf,lightB->settings->power_level);
  send_string("     power_level: ");send_string(buf);send_string("\r\n");
  uint8_to_hex(buf,lightB->settings->dim_level);
  send_string("     dim_level:   ");send_string(buf);send_string("\r\n");
  send_string("\r\n");
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
