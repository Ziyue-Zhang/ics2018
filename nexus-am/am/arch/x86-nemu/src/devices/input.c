#include <am.h>
#include <x86.h>
#include <amdev.h>
//#define KEYDOWN_MASK 0x8000

size_t input_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_INPUT_KBD: {
      _KbdReg *kbd = (_KbdReg *)buf;
	  int32_t k = inl(0x60);
	  //uint32_t s = inb(0x64);
      /*kbd->keydown = (k & KEYDOWN_MASK ? 1 : 0);
	  kbd->keycode = k & ~KEYDOWN_MASK;*/
	  //printf("%d\n",s);
	  kbd->keydown = ((k >> 15) & 1);
	  kbd->keydown = k & 0x7fff;
      //kbd->keydown = s;
	  //kbd->keycode = k;
	  //if(s!=0)
		//  kbd->keycode = _KEY_NONE;
      return sizeof(_KbdReg);
    }
  }
  return 0;
}

