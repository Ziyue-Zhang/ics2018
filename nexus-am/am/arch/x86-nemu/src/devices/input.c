#include <am.h>
#include <x86.h>
#include <amdev.h>
int readkey();

size_t input_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_INPUT_KBD: {
      _KbdReg *kbd = (_KbdReg *)buf;
      kbd->keydown = 0;
      kbd->keycode = readkey();
	  //kbd->keycode = _KEY_NONE;
      return sizeof(_KbdReg);
    }
  }
  return 0;
}

int readkey() {
  if (inb(0x64))
	return inl(0x60);
  return _KEY_NONE;
}
