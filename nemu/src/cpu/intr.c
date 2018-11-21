#include "cpu/exec.h"
#include "memory/mmu.h"

void raise_intr(uint8_t NO, vaddr_t ret_addr) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */

  //TODO();
  rtl_push((rtlreg_t*)&cpu.eflags);
  rtl_push((rtlreg_t*)&cpu.cs);
  rtl_push((rtlreg_t*)&cpu.eip);
}

void dev_raise_intr() {
}
