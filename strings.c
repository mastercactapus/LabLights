#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "strings.h"

uint8_t hex_value(char chr) { //invalid values will return 0
  if (chr>102) return 0; //lower case letters a-f
  if (chr>=97) return chr - 87;

  if (chr>71) return 0; //upper case letters A-F
  if (chr>=65) return chr - 55;

  if (chr>57) return 0; //numbers 0-9
  if (chr>=48) return chr-48;

  return 0; //not a-fA-F0-9 return 0
}

char hex_char(uint8_t val) {
  if (val < 10) return (char)(val + 48);
  if (val < 16) return (char)(val + 87);
  return 0; //invalid return 0
}

uint8_t hex_to_uint8(char *str) {
  if (strlen(str) > 1) 
    return 16 * hex_value(str[0]) + hex_value(str[1]);
  return hex_value(str[0]);
}

char* uint8_to_hex(uint8_t num) {
  char *buf = calloc(4,1);
  if (num < 16) {
    buf[0] = hex_char(num);
    buf[1] = 0;
  } else {
    buf[0] = hex_char(num / 16);
    buf[1] = hex_char(num % 16);
    buf[2] = 0;
  }
  return buf;
}


char* get_word(char *str) {
  char *out = calloc(16,1);
  char *end = strchrnul(str,32);
  size_t len = end - str;
  memcpy(out, str, len);
  size_t size = strlen(str);
  memmove(str, end + 1, size - len);
  return out;
}
