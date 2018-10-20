#include "klib.h"
#include <stdarg.h>

//#ifndef __ISA_NATIVE__
#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  /*assert(fmt);
  va_list va;
  int n = 0;
  va_start(va, fmt);
  while(*fmt != '\0')
  {

  }*/
	return 0;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  return 0;
}

/*int sprintf(char *out, const char *fmt, ...) {
  assert(fmt);
  va_list va;
  int n = 0;
  va_start(va, fmt);
  while(*fmt != '\0')
  {
	if (*fmt == '%') {
		if (*(fmt + 1) == 's') {
			fmt++;
			char *s = va_arg(va,char*);
			while(*s != '\0')
			{
				*out = *s;
				out++;
				n++;
				s++;
			}
		}
		else if (*(fmt + 1) == 'd') {
			fmt++;
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
}*/

int sprintf(char *out, const char *fmt, ...) {
  assert(fmt);
  va_list va;
  int n = 0;
  va_start(va, fmt);
  while(*fmt != '\0')
  {
	if (*fmt == '%') {
		switch (*(fmt + 1)) {
		case 's':{
			fmt++;
			char *s = va_arg(va, char*);
			while (*s != '\0')
			{
				*out = *s;
				out++;
				n++;
				s++;
			}
		}
		case 'd':{
			fmt++;
			int result = va_arg(va, int);
			char d[32];
			int temp = result;
			int i;
			for (i = 0; temp != 0; i++)
			{
				d[i] = temp % 10 + '0';
				temp /= 10;
			}
			i--;
			while (i >= 0)
			{
				*out = d[i];
				i--;
				out++;
				n++;
			}
		}
		default:break;
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
