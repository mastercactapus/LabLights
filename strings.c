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
  uint8_t out = 0;
  if (str_len(str) > 1) 
    return 16 * hex_value(str[0]) + hex_value(str[1]);
  return hex_value(str[0]);
}

void uint8_to_hex(char *buf, uint8_t num) {
  if (num < 16) {
    buf[0] = hex_char(num);
    buf[1] = 0;
  } else {
    buf[0] = hex_char(num / 16);
    buf[1] = hex_char(num % 16);
    buf[2] = 0;
  }
}

uint8_t scmp(char *str1, char *str2) {
  uint8_t i = 0;
  while (!((str1[i] == 0) && (str2[i] == 0))) {
    if (str1[i] != str2[i]) {
      return 0;
    } else if (i == 255) {
      break;
    } else {
      i++;
    }
  }
  return 1;
}
uint8_t str_len(char *str) {
  uint8_t i = 0;
  while (str[i] != 0) i++;
  return i;
}
void get_word(char *buf, char *str) {
  uint8_t i =0;
  while (str[i] != 32 && str[i] != 0) i++;
  if (str[i] == 0) {
    memcpy(buf,str,i);
    str[0] = 0;
    buf[i] = 0;
  } else {
    memcpy(buf,str,i);
    uint8_t l = i;
    while (str[l] != 0) l++;
    memcpy(str,str + i + 1, l-i);
  }
}
