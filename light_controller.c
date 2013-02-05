#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 1000000UL
#include <util/delay.h>

#include "types.h"
int main() {
  
  DDRA = _BV(PA1);

  while(1) {
    PORTA ^= _BV(PA1);
    _delay_ms(5);
  }

}

