#include <unistd.h>
#include <sys/stat.h>
#include <setjmp.h>
#include <sys/time.h>
#include <assert.h>
#include <time.h>
#include "syscall.h"

#if defined(__ISA_X86__)
intptr_t _syscall_(int type, intptr_t a0, intptr_t a1, intptr_t a2){
  int ret = -1;
  asm volatile("int $0x80": "=a"(ret): "a"(type), "b"(a0), "c"(a1), "d"(a2));
  return ret;
}
#elif defined(__ISA_AM_NATIVE__)
intptr_t _syscall_(int type, intptr_t a0, intptr_t a1, intptr_t a2){
  intptr_t ret = 0;
  asm volatile("call *0x100000": "=a"(ret): "a"(type), "S"(a0), "d"(a1), "c"(a2));
  return ret;
}
#else
#error _syscall_ is not implemented
#endif

void _exit(int status) {
if(status==3)
	while(1);
  _syscall_(SYS_exit, status, 0, 0);
  while (1);
}

int _open(const char *path, int flags, mode_t mode) {
  //_exit(SYS_open);
  //return 0;
   return _syscall_(SYS_open, (uintptr_t)path, flags, mode);
}

int _write(int fd, void *buf, size_t count){
  //_exit(SYS_write);
   return _syscall_(SYS_write, fd, (intptr_t)buf, count);
}

extern end;

void *_sbrk(intptr_t increment){
    static char *_end = &end;
	char *pro_brk = _end + increment;
	if(_syscall_(SYS_brk, (uintptr_t)pro_brk, increment, 0) != 0)
		return (void *)-1;
	else
	{
		void *old_pro_brk = _end;
		_end = pro_brk;
		return old_pro_brk;
	}
}

int _read(int fd, void *buf, size_t count) {
  //_exit(SYS_read);
  return _syscall_(SYS_read, fd, (intptr_t)buf, count);
  //return 0;
}

int _close(int fd) {
  //_exit(SYS_close);
  //return 0;
  return _syscall_(SYS_close, fd, 0, 0);
}

off_t _lseek(int fd, off_t offset, int whence) {
  //_exit(SYS_lseek);
	//assert(0);
  return _syscall_(SYS_lseek, fd, offset, whence);
  //return 0;
}

int _execve(const char *fname, char * const argv[], char *const envp[]) {
  //_exit(SYS_execve);
  //return 0;
  //assert(0);
  return _syscall_(SYS_execve, (intptr_t)fname, 0, 0);
}

// The code below is not used by Nanos-lite.
// But to pass linking, they are defined as dummy functions

int _fstat(int fd, struct stat *buf) {
  return 0;
}

int _kill(int pid, int sig) {
  _exit(-SYS_kill);
  return -1;
}
void _fork() {
}
void _wait() {
}
pid_t _getpid() {
  _exit(-SYS_getpid);
  return 1;
}
