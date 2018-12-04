#include "cpu/exec.h"
#include "cpu/cc.h"

make_EHelper(jmp) {
  // the target address is calculated at the decode stage
  rtl_j(decoding.jmp_eip);

  print_asm("jmp %x", decoding.jmp_eip);
}

make_EHelper(jcc) {
  // the target address is calculated at the decode stage
  uint32_t cc = decoding.opcode & 0xf;
  rtl_setcc(&t0, cc);
  rtl_li(&t1, 0);
  rtl_jrelop(RELOP_NE, &t0, &t1, decoding.jmp_eip);
  //pr(&decoding.jmp_eip);

  print_asm("j%s %x", get_cc_name(cc), decoding.jmp_eip);
   // printf("---%s---\n",decoding.assembly);
}

make_EHelper(jmp_rm) {
  rtl_jr(&id_dest->val);

  print_asm("jmp *%s", id_dest->str);
}

make_EHelper(call) {
  // the target address is calculated at the decode stage
  //printf("esp = %x\n",cpu.esp);
 // printf("rtl_push %d\n",*(&id_dest->addr));
  //printf("decoding.seq_eip = %x\n",decoding.seq_eip);
  
  
  rtl_push(&decoding.seq_eip);

 
 
 // printf("&id_dest->addr = %x\n",id_dest->addr);
  //printf("before call, eip = %x\n", decoding.seq_eip);
  
  
  decoding.jmp_eip = decoding.seq_eip + id_dest->imm;
  
  rtl_j(decoding.jmp_eip);
  //decoding.seq_eip = decoding.seq_eip + id_dest->imm;
  //printf("after call, eip = %x\n", decoding.seq_eip);
  print_asm("call %x", decoding.jmp_eip);
}

make_EHelper(ret) {
  rtl_pop(&id_dest->addr);
  //printf("id_dest->addr = %x\n",id_dest->addr);
  decoding.seq_eip = id_dest->addr;

  print_asm("ret");
}

make_EHelper(call_rm) {

	//pr(&cpu.eip);
  rtl_push(&decoding.seq_eip);
  /*rtl_mv(&cpu.eip,&id_dest->val);
  operand_write(&cpu.eip,&id_dest->val);*/
  decoding.jmp_eip = id_dest->val;
  
  rtl_j(decoding.jmp_eip);

  

  print_asm("call *%s", id_dest->str);
}
