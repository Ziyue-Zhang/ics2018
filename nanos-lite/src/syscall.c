#include "common.h"
#include "syscall.h"

extern void _putc(char ch);

uint32_t sys_write(int fd, const void *buf, size_t count)
{
	uint32_t len = 0;
	const char *temp = buf;
	if(fd == 1 || fd == 2)
	{
		for (; len < count; ++len) 
			_putc(temp[len]);
	}
	else
	{
		Log("nmsl");
		count=-1;
	}
	return count;
}

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;
  switch (a[0]) {
	case SYS_yield: _yield(), c->GPRx = 0; break;
	case SYS_exit: _halt(a[0]); break;
	case SYS_write: a[0] = sys_write(a[1], (void *)a[2], a[3]); break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
