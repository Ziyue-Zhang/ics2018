#include "klib.h"
#include <stdarg.h>

//#ifndef __ISA_NATIVE__
#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  assert(fmt);
  va_list va;
  int n = 0;
  va_start(va, fmt);
  while(*fmt != '\0')
  { 
	if (*fmt == '%') {
		fmt++;
		if (*fmt == 's') {
			fmt++;
			char *s = va_arg(va,char*);
			while(*s != '\0')
			{
				_putc(*s);
				n++;
				s++;
			}
		}
		else if (*fmt == 'd') {
			int result = va_arg(va, int);
			char d[32];
			int temp = result;
			int i;
			for(i = 0; temp != 0; i++)
			{
				d[i] = temp % 10 + '0';
				temp /= 10;
			}
			i--;
			while(i >= 0)
			{
				_putc(d[i]);
				i--;
				n++;
			}
		}
		else if(*(fmt + 1) == '0') {
			fmt++;
			fmt++;
		}			
	}
	else {
		_putc(*fmt);
		n++;
	}
	fmt++;
  }
  va_end(va);
  return n;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  return 0;
}

int sprintf(char *out, const char *fmt, ...) {
  assert(fmt);
  va_list va;
  int n = 0;
  va_start(va, fmt);
  while(*fmt != '\0')
  { 
	if (*fmt == '%') {
		fmt++;
		if (*fmt == 's') {
			char *s = va_arg(va,char*);
			while(*s != '\0')
			{
				*out = *s;
				out++;
				n++;
				s++;
			}
		}
		else if (*fmt == 'd') {
			int result = va_arg(va, int);
			char d[32];
			int temp = result;
			int i;
			for(i = 0; temp != 0; i++)
			{
				d[i] = temp % 10 + '0';
				temp /= 10;
			}
			i--;
			while(i >= 0)
			{
				*out = d[i];
				i--;
				out++;
				n++;
			}
		}
	}
	else {
		*out = *fmt;
		out++;
		n++;
	}
	fmt++;
  }
  *out = '\0';
  va_end(va);
  return n;
}


int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}

#endif
