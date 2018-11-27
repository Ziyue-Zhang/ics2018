#include "proc.h"
#include <unistd.h>

#define DEFAULT_ENTRY 0x4000000
extern size_t ramdisk_read();
extern size_t get_ramdisk_size();
extern ssize_t fs_read();
extern int fs_open();
extern int fs_close();
size_t fs_filesz();

static uintptr_t loader(PCB *pcb, const char *filename) {
  //TODO();
  //ramdisk_read((void *)DEFAULT_ENTRY, 0, get_ramdisk_size());
  int fd = fs_open(filename, 0, 0);
  Log("filename:%s fd = %d", filename, fd);
  return DEFAULT_ENTRY;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  ((void(*)())entry) ();
}

void context_kload(PCB *pcb, void *entry) {
  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->tf = _kcontext(stack, entry, NULL);
}

void context_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);

  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->tf = _ucontext(&pcb->as, stack, stack, (void *)entry, NULL);
}
