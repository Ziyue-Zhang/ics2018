#include "cpu/exec.h"

make_EHelper(mov) {
  operand_write(id_dest, &id_src->val);
  print_asm_template2(mov);
  //printf("---%s---\n",decoding.assembly);
}

make_EHelper(push) {
//pr(&id_dest->val);
  rtl_push(&id_dest->val);
  rtlreg_t esp1 = cpu.esp;
     //printf("cpu.esp1 = %x\n",cpu.esp);
   if(id_dest->type != OP_TYPE_IMM)operand_write(id_dest, &id_dest->val);
   //rtlreg_t esp2 = cpu.esp;
   cpu.esp = esp1;
   //assert(esp1 == esp2);
   //printf("cpu.esp2 = %x\n",cpu.esp);
  print_asm_template1(push);
  //printf("---%s---\n",decoding.assembly);
}

make_EHelper(pop) {
	//pr(&id_dest->val);
  rtl_pop(&id_dest->val);
   if(id_dest->type != OP_TYPE_IMM)operand_write(id_dest, &id_dest->val);
  print_asm_template1(pop);
  //printf("---%s---\n",decoding.assembly);
}

make_EHelper(pusha) {
  if (decoding.is_operand_size_16) {
    rtl_li(&t0,reg_w(R_SP));
    rtl_li(&t1,reg_w(R_AX));
    rtl_push(&t1);
    rtl_li(&t1,reg_w(R_CX));
    rtl_push(&t1);
    rtl_li(&t1,reg_w(R_DX));
    rtl_push(&t1);
    rtl_li(&t1,reg_w(R_BX));
    rtl_push(&t1);
    rtl_push(&t0);
    rtl_li(&t1,reg_w(R_BP));
    rtl_push(&t1);
    rtl_li(&t1,reg_w(R_SI));
    rtl_push(&t1);
    rtl_li(&t1,reg_w(R_DI));
    rtl_push(&t1);
  }
  else{
    rtl_mv(&t0,&reg_l(R_ESP));
    rtl_push(&reg_l(R_EAX));
    rtl_push(&reg_l(R_ECX));
    rtl_push(&reg_l(R_EDX));
    rtl_push(&reg_l(R_EBX));
    rtl_push(&t0);
    rtl_push(&reg_l(R_EBP));
    rtl_push(&reg_l(R_ESI));
    rtl_push(&reg_l(R_EDI));
  }
  

  print_asm("pusha");
}

make_EHelper(popa) {
  rtl_pop(&t0);
  rtl_mv(&reg_l(R_EDI),&t0);
  rtl_pop(&t0);
  rtl_mv(&reg_l(R_ESI),&t0);
  rtl_pop(&t0);
  rtl_mv(&reg_l(R_EBP),&t0);
  rtl_pop(&t0);
  rtl_pop(&t0);
  rtl_mv(&reg_l(R_EBX),&t0);
  rtl_pop(&t0);
  rtl_mv(&reg_l(R_EDX),&t0);
  rtl_pop(&t0);
  rtl_mv(&reg_l(R_ECX),&t0);
  rtl_pop(&t0);
  rtl_mv(&reg_l(R_EAX),&t0);

  print_asm("popa");
}

make_EHelper(leave) {
  rtl_mv(&reg_l(R_ESP),&reg_l(R_EBP));
  rtl_pop(&reg_l(R_EBP));
  print_asm("leave");
  //printf("---%s---\n",decoding.assembly);
}

make_EHelper(cltd) {
  if (decoding.is_operand_size_16) {
    //pr(&reg_w(R_AX));
    bool msb = (reg_w(R_AX) >> 15) & 1;
    //pr(&msb);
    if(msb) rtl_ori(&reg_l(R_EDX),&reg_l(R_EDX),0xffff);
    else rtl_andi(&reg_l(R_EDX),&reg_l(R_EDX),0xffff0000);
    //pr(&reg_w(R_DX));
  }
  else {
  	//pr(&reg_l(R_EAX));
    bool msb = (reg_l(R_EAX) >> 31) & 1;
    //pr(&msb);
    if(msb) rtl_li(&reg_l(R_EDX),0xffffffff);
    else rtl_li(&reg_l(R_EDX),0);
    //pr(&reg_l(R_EDX));
      //printf("---%s---\n",decoding.assembly);
  }

  print_asm(decoding.is_operand_size_16 ? "cwtl" : "cltd");
}

make_EHelper(cwtl) {
  if (decoding.is_operand_size_16) {
    /*bool msb = (reg_l(R_EAX) >> 7) & 1;
    if(msb) rtl_ori(&reg_l(R_EAX),&reg_l(R_EAX),0xff00);
    else rtl_andi(&reg_l(R_EAX),&reg_l(R_EAX),0x00ff);*/
    rtl_lr(&t0,R_AL,1);
    t0 = (int16_t)(int8_t)t0;
    rtl_sr(R_AX,&t0,2);
  }
  else {
    /*bool msb = (reg_l(R_EAX) >> 15) & 1;
    if(msb) rtl_ori(&reg_l(R_EAX),&reg_l(R_EAX),0xffff0000);
    else rtl_andi(&reg_l(R_EAX),&reg_l(R_EAX),0x0000ffff);*/
    rtl_lr(&t0,R_AX,2);
    t0 = (int32_t)(int16_t)t0;
    rtl_sr(R_EAX,&t0,4);
  }

  print_asm(decoding.is_operand_size_16 ? "cbtw" : "cwtl");
}

make_EHelper(movsx) {
  id_dest->width = decoding.is_operand_size_16 ? 2 : 4;
  //pr(&id_src->width);
  if(id_src->width == 4) {
  	rtl_li(&t1,id_src->val & 0x0000ffff);
  	id_src->width = 2;
  }
  else{
  	rtl_li(&t1,id_src->val);
  }
  //pr(&t1);
  rtl_sext(&t0, &t1,id_src->width);
  //pr(&id_src->val);
  //pr(&t0);
  operand_write(id_dest, &t0);
  print_asm_template2(movsx);
  //printf("---%s---\n",decoding.assembly);
}

make_EHelper(movzx) {
  id_dest->width = decoding.is_operand_size_16 ? 2 : 4;
  if(id_dest->width == 4) {
  	if(id_src->width == 1) rtl_li(&id_dest->val,id_src->val & 0x000000ff);
  	else rtl_li(&id_dest->val,id_src->val & 0x0000ffff);
  }
  else{
	rtl_li(&id_dest->val,id_src->val & 0xffff00ff);
  }
  operand_write(id_dest, &id_dest->val);
  print_asm_template2(movzx);
      //printf("---%s---\n",decoding.assembly);
}

make_EHelper(lea) {
  operand_write(id_dest, &id_src->addr);
  print_asm_template2(lea);
    //printf("---%s---\n",decoding.assembly);
}
