#include "string.h"

#ifdef __cplusplus
extern "C" {
#endif

void reverseString(char* s) {
  char t;
  char *s0 = s;
  if (*s == '\0') { return; }
  while (*s != '\0') { ++s; }
  --s;
  while (*s == '0' && s > s0) { --s; };
  *(s + 1) = '\0';
  for (; s0 < s; --s, ++s0) {
    t = *s;
    *s = *s0;
    *s0 = t;
  }
}

#ifdef __cplusplus
}
#endif
