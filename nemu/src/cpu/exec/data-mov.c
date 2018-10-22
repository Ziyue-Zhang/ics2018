#include "cpu/exec.h"

make_EHelper(mov) {
  operand_write(id_dest, &id_src->val);
  print_asm_template2(mov);
}

make_EHelper(push) {
 // TODO();
  rtl_push(&id_dest->val);
  print_asm_template1(push);
}

make_EHelper(pop) {
  //TODO();
  rtl_pop(&t0);
  operand_write(id_dest, &t0);
  print_asm_template1(pop);
}

make_EHelper(pusha) {
  TODO();

  print_asm("pusha");
}

make_EHelper(popa) {
  TODO();

  print_asm("popa");
}

make_EHelper(leave) {
  //TODO();
  rtl_mv(&cpu.esp, &cpu.ebp);
  rtl_pop(&cpu.ebp);
  print_asm("leave");
}

make_EHelper(cltd) {
  if (decoding.is_operand_size_16) {
    //TODO();
	if((int16_t) cpu.eax < 0) {
		t0 = 0xffff;
		rtl_sr(R_DX, &t0, 2);
	}
	else {
		t0 = 0;
		rtl_sr(R_DX, &t0, 2);
	}
  } 
  else { 
    //TODO();
	if(cpu.eax < 0)
		interpret_rtl_li(&cpu.edx, 0xFFFFFFFF);
	else
		interpret_rtl_li(&cpu.edx, 0x0);
  }

  print_asm(decoding.is_operand_size_16 ? "cwtl" : "cltd");
}

make_EHelper(cwtl) {
  if (decoding.is_operand_size_16) {
    //TODO();
	rtl_lr(&t0, R_AL, 1);
	t0 = (int16_t)(int8_t)(uint8_t)t0;
	//t0=0;
	rtl_sr(R_AX, &t0, 2);
  } 
  else {
    //TODO();
	rtl_lr(&t0, R_AX, 2);
	t0 = (int32_t)(int16_t)(uint16_t)t0;
	//t0=0;
	rtl_sr(R_EAX, &t0, 4);
  }
  print_asm(decoding.is_operand_size_16 ? "cbtw" : "cwtl");
}

make_EHelper(movsx) {
  id_dest->width = decoding.is_operand_size_16 ? 2 : 4;
  rtl_sext(&t0, &id_src->val, id_src->width);
  operand_write(id_dest, &t0);
  print_asm_template2(movsx);
}

make_EHelper(movzx) {
  id_dest->width = decoding.is_operand_size_16 ? 2 : 4;
  operand_write(id_dest, &id_src->val);
  print_asm_template2(movzx);
}

make_EHelper(lea) {
	//printf("fuck1");
  operand_write(id_dest, &id_src->addr);
	//printf("fuck2");
  print_asm_template2(lea);
}
