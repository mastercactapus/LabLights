#include "strings.h"

uint8_t hex_to_uint8(char *str) {
  uint8_t out = 0;
  if (str_len(str))
}

void uint8_to_hex(char *buf, uint8_t num) {

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
