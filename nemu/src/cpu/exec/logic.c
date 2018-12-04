#include "cpu/exec.h"
#include "cpu/cc.h"

make_EHelper(test) {
    rtl_and(&id_dest->val,&id_dest->val,&id_src->val);
    rtl_update_ZFSF(&id_dest->val,id_dest->width);
	rtl_li(&t0,0);
	rtl_set_CF(&t0);
	rtl_set_OF(&t0);
  print_asm_template2(test);
     //printf("---%s---\n",decoding.assembly);
}

make_EHelper(and) {
  //TODO();
  //pr(&id_dest->val);pr(&id_src->val);pr(&id_src2->val);
  
  rtl_and(&id_dest->val,&id_dest->val,&id_src->val);
  
  //pr(&id_dest->val);
    operand_write(id_dest, &id_dest->val);
    rtl_li(&t0,0);
    rtl_set_CF(&t0);
    rtl_set_OF(&t0);
    rtl_update_ZFSF(&id_dest->val,id_dest->width);
    //pr(&id_dest);
  print_asm_template2(and);
   // printf("---%s---\n",decoding.assembly);
}

make_EHelper(xor) {
  //printf("eax1 = %x\n",cpu.eax);
  rtl_xor(&id_dest->val,&id_dest->val,&id_src->val);
  operand_write(id_dest, &id_dest->val);
  rtl_li(&t0,0);
    rtl_set_CF(&t0);
    rtl_set_OF(&t0);
    rtl_update_ZFSF(&id_dest->val,id_dest->width);
  //printf("eax2 = %x\n",cpu.eax);
  print_asm_template2(xor);
  //printf("---%s---\n",decoding.assembly);
}

make_EHelper(or) {
  rtl_or(&id_dest->val,&id_dest->val,&id_src->val);
  operand_write(id_dest, &id_dest->val);
  rtl_li(&t0,0);
    rtl_set_CF(&t0);
    rtl_set_OF(&t0);
  rtl_update_ZFSF(&id_dest->val,id_dest->width);

  print_asm_template2(or);
  //printf("---%s---\n",decoding.assembly);
}

make_EHelper(sar) {
	//pr(&id_dest->val);
	//pr(&id_src->val);
	//pr(&id_dest->width);
	//rtl_sext(&id_dest->val,&id_dest->val,id_dest->width);
	//pr(&id_dest->val);
  if(id_dest->width == 1){
  	id_dest->val = (int8_t)id_dest->val;
  }
  if(id_dest->width == 2){
  	id_dest->val = (int16_t)id_dest->val;
  }
  rtl_sar(&id_dest->val,&id_dest->val,&id_src->val);
  //pr(&id_dest->val);
  rtl_update_ZFSF(&id_dest->val,id_dest->width);
  operand_write(id_dest, &id_dest->val);
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(sar);
  //printf("---%s---\n",decoding.assembly);
}

make_EHelper(shl) {
  rtl_shl(&id_dest->val,&id_dest->val,&id_src->val);
  rtl_update_ZFSF(&id_dest->val,id_dest->width);
  operand_write(id_dest, &id_dest->val);
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(shl);
}

make_EHelper(shr) {
  rtl_shr(&id_dest->val,&id_dest->val,&id_src->val);
  rtl_update_ZFSF(&id_dest->val,id_dest->width);
  operand_write(id_dest, &id_dest->val);
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(shr);
}

make_EHelper(setcc) {
  uint32_t cc = decoding.opcode & 0xf;

  rtl_setcc(&t2, cc);
  operand_write(id_dest, &t2);

  print_asm("set%s %s", get_cc_name(cc), id_dest->str);
    //printf("---%s---\n",decoding.assembly);
}

make_EHelper(not) {
  rtl_not(&id_dest->val,&id_dest->val);
  operand_write(id_dest,&id_dest->val);

  print_asm_template1(not);
     // printf("---%s---\n",decoding.assembly);
}

make_EHelper(rol){
  rtl_shl(&t0,&id_dest->val,&id_src->val);
  rtl_shri(&t1,&id_dest->val,id_dest->width*8 - id_src->val);
  rtl_or(&t0,&t0,&t1);
  operand_write(id_dest,&t0);
    print_asm_template2(tol);
	/*int count = id_src->val;
	int tempcf = 0;
	while(count){
		count--;
		if (decoding.is_operand_size_16) tempcf = (id_dest->val >> 15) & 1;
		else  tempcf = (id_dest->val >> 31) & 1;
		rtl_li(&id_dest->val,id_dest->val*2+tempcf);
	}
	if(id_src->val == 1){
		if (decoding.is_operand_size_16) {
			if(cpu.eflags.CF != ((id_dest->val >> 15) & 1)) cpu.eflags.OF = 1;
			else cpu.eflags.OF = 0;
		}
		else{
			if(cpu.eflags.CF != ((id_dest->val >> 31) & 1)) cpu.eflags.OF = 1;
			else cpu.eflags.OF = 0;
		}
	}*/
}
