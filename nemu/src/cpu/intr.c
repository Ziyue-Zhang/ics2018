#include "cpu/exec.h"
#include "memory/mmu.h"
#define NR_IRQ    256
extern GateDesc idt[NR_IRQ];

void raise_intr(uint8_t NO, vaddr_t ret_addr) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */

  //TODO();
  rtl_push((rtlreg_t*)&cpu.eflags);
  rtl_push((rtlreg_t*)&cpu.cs);
  rtl_push((rtlreg_t*)&ret_addr); //this could be ret_addr
  uint32_t base = cpu.idtr.base;
  uint32_t offset;
  offset = (vaddr_read(base + NO * 8, 4) & 0x0000ffff) | (vaddr_read(base + NO * 8 + 4, 4) & 0xffff0000);
  rtl_j(offset);
}

void dev_raise_intr() {
}
