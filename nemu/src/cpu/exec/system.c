#include "cpu/exec.h"

void difftest_skip_ref();
void difftest_skip_dut();
extern uint32_t pio_read_common(ioaddr_t addr, int len);
extern void pio_write_common(ioaddr_t addr, uint32_t data, int len); 
make_EHelper(lidt) {
  TODO();

  print_asm_template1(lidt);
}

make_EHelper(mov_r2cr) {
  TODO();

  print_asm("movl %%%s,%%cr%d", reg_name(id_src->reg, 4), id_dest->reg);
}

make_EHelper(mov_cr2r) {
  TODO();

  print_asm("movl %%cr%d,%%%s", id_src->reg, reg_name(id_dest->reg, 4));

#if defined(DIFF_TEST)
  difftest_skip_ref();
#endif
}

make_EHelper(int) {
  TODO();

  print_asm("int %s", id_dest->str);

#if defined(DIFF_TEST) && defined(DIFF_TEST_QEMU)
  difftest_skip_dut();
#endif
}

make_EHelper(iret) {
  TODO();

  print_asm("iret");
}

make_EHelper(in) {
  //TODO();
  t0 = pio_read_common(id_src->val, id_src->width);
  operand_write(id_dest, &t0);
  print_asm_template2(in);

#if defined(DIFF_TEST)
  difftest_skip_ref();
#endif
}

make_EHelper(out) {
  //TODO();
  pio_write_common(id_dest->val, id_src->val, id_src->width); 
  print_asm_template2(out);

#if defined(DIFF_TEST)
  difftest_skip_ref();
#endif
}
