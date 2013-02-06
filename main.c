#define F_CPU 1000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "types.h"
#include "eeprom.h"

SETTINGS *lightA;
SETTINGS *lightB;

int main() {
  
  DDRA = _BV(PA1);

  while(1) {
    PORTA ^= _BV(PA1);
    _delay_ms(1000);
  }

}

