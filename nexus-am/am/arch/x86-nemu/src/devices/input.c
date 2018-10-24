#include <am.h>
#include <x86.h>
#include <amdev.h>

size_t input_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_INPUT_KBD: {
      _KbdReg *kbd = (_KbdReg *)buf;
	  uint32_t k = inw(0x60);
      kbd->keydown = 0;
	  kbd->keycode = k;
      return sizeof(_KbdReg);
    }
  }
  return 0;
}

