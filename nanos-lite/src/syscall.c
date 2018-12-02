#include "common.h"
#include "fs.h"
#include "syscall.h"

extern void _putc(char ch);

/*uint32_t sys_write(int fd, const void *buf, size_t count)
{
	uint32_t len = 0;
	const char *temp = buf;
	if(fd == 1 || fd == 2)
	{
		for (; len < count; ++len) 
			_putc(temp[len]);
	}
	return count;
}*/

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;
  switch (a[0]) {
	case SYS_exit: Log("%s", a[0]), _halt(a[0]); break;
	case SYS_yield: _yield(), c->GPRx = 0; break;
    case SYS_open: c->GPRx = fs_open((char *)a[1], a[2], a[3]); break;
	case SYS_read: c->GPRx = fs_read(a[1], (void *)a[2], a[3]); break;
	case SYS_write: c->GPRx = fs_write(a[1], (void *)a[2], a[3]); break;
	case SYS_close: c->GPRx = fs_close(a[1]); break;
	case SYS_lseek: c->GPRx = fs_lseek(a[1], a[2], a[3]); break;
	case SYS_brk: c->GPRx = 0;	break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
