#include "klib.h"
#include <stdarg.h>

#ifndef __ISA_NATIVE__
//#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  return 0;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  return 0;
}

int sprintf(char *out, const char *fmt, ...) {
  int n;
  va_list va;
  va_start(va, fmt);
  n = vsprintf(out, fmt, va);
  va_end(va);
  return n;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}

#endif
