#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;

  /* TODO: Add more members if necessary */
  char exp[100];
  int value;

} WP;

void new_wp(char* args);
void free_wp(WP *wp);
void del(int num);
void wp_info();
bool check();

#endif
