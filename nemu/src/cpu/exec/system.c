#include "cpu/exec.h"
#include "device/port-io.h"
void difftest_skip_ref();
void difftest_skip_dut();

make_EHelper(lidt) {
	cpu.idtr.base = vaddr_read(id_dest->addr + 2,4);
	cpu.idtr.limit = vaddr_read(id_dest->addr,2);
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
  //pr(&id_dest->val);
  raise_intr(id_dest->val,cpu.eip+2);

  print_asm("int %s", id_dest->str);

#if defined(DIFF_TEST) && defined(DIFF_TEST_QEMU)
  difftest_skip_dut();
#endif
}

make_EHelper(iret) {
  rtl_pop(&t0);
  rtl_pop(&cpu.CS);
  rtl_pop(&cpu.eflags.val);
  rtl_j(t0);

  print_asm("iret");
}

make_EHelper(in) {
//pr(&id_src->val);
//pr(&id_src->addr);
  switch(id_src->width){
		case 4:
			rtl_li(&id_dest->val,pio_read_l((ioaddr_t)id_src->val));
			break;
		case 2:
			rtl_li(&id_dest->val,pio_read_w((ioaddr_t)id_src->val));
			break;
		case 1:
		//printf("ret = %d\n",pio_read_b((ioaddr_t)id_src->val));
			rtl_li(&id_dest->val,pio_read_b((ioaddr_t)id_src->val));
			break;
	}
	//pr(&id_dest->val);
  operand_write(id_dest,&id_dest->val);

  print_asm_template2(in);
//printf("---%s---\n",decoding.assembly);
#if defined(DIFF_TEST)
  difftest_skip_ref();
#endif
}

make_EHelper(out) {
  //printf("data = %x\n",id_src->val);
  pio_write_b((ioaddr_t)id_dest->val,id_src->val);
	
  print_asm_template2(out);
  //printf("---%s---\n",decoding.assembly);

#if defined(DIFF_TEST)
  difftest_skip_ref();
#endif
}
