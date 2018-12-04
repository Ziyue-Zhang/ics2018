#include "cpu/exec.h"
#include "memory/mmu.h"

void raise_intr(uint8_t NO, vaddr_t ret_addr) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */
  uint32_t addr = cpu.idtr.base + NO*8;
  uint32_t low = vaddr_read(addr,2);
  uint32_t high = vaddr_read(addr+6,2);
  uint32_t jp = (high << 16) | low;
  cpu.eflags.val = 6;//(cpu.eflags.CF + 2 + (cpu.eflags.ZF << 6) + (cpu.eflags.SF << 7) + (cpu.eflags.IF << 9) + (cpu.eflags.OF << 11));
  rtl_push(&cpu.eflags.val);
  rtl_push(&cpu.CS);
  rtl_push(&ret_addr);
  rtl_j(jp);
 // cpu.eip = ret_addr;
}

void dev_raise_intr() {
}
