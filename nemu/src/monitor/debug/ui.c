#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint64_t);
long change(int c)
{
  if (c >= 'A' && c <= 'Z')
    return c + 'a' - 'A';
  else
    return c;
}
long htoi(char s[])
{
  int i;
  long n = 0;
  if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X'))
    {
    i = 2;
    }
  else
   i = 0;
  for (; (s[i] >= '0' && s[i] <= '9') || (s[i] >= 'a' && s[i] <= 'f') || (s[i] >= 'A' && s[i] <= 'F'); ++i)
    {
    if (change(s[i]) > '9')
      {
      n = 16 * n + (10 + change(s[i]) - 'a');
      }
    else
      {
      n = 16 * n + (change(s[i]) - '0');
      }
    }
  return n;
}
/* We use the `readline' library to provide more flexibility to read from stdin. */
char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}

static int cmd_q(char *args) {
  return -1;
}

static int cmd_help(char *args);

static int cmd_si(char *args);

static int cmd_info(char *args);

static int cmd_x(char *args);

static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  { "si", "Step instructions exactly", cmd_si},
  { "info", "Generic command for showing things about the program being debugged", cmd_info}, 
  { "x", "Scan memory", cmd_x},
  /* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

static int cmd_si(char *args) {
  char *arg = strtok(NULL, " ");
  
  if (arg == NULL) {
    cpu_exec(1);
  }
  else {
   int num = atoi(arg);
   cpu_exec(num);
   return 0;
  }
  return 0;
}

static int cmd_info(char *args) {
  char *arg = strtok(NULL, " ");

  if (arg[0] == 'r') {
     printf("$eax = 0x%x\t\t$edx = 0x%x\n", cpu.eax,cpu.edx);
     printf("$ecx = 0x%x\t\t$ebx = 0x%x\n", cpu.ecx,cpu.ebx);
     printf("$ebp = 0x%x\t\t$esi = 0x%x\n", cpu.ebp,cpu.esi);
     printf("$edi = 0x%x\t\t$esp = 0x%x\n", cpu.edi,cpu.esp);
    }
  else if (arg[0] == 'w') {

    }
  else
    printf("Unknown command '%s'\n", arg);
  return 0;
}

static int cmd_x(char *args) {
  char *num = strtok(NULL, " ");
		
  if (num == NULL)
    printf("Unknown command \n");
  else {
    char *arg = strtok(NULL, " ");
    if (arg == NULL)
      printf("Unknown command '%s'\n", arg);
    else
      {
      int n = atoi(num);
      if (arg[0] == '0' && arg[1] == 'x')
	{
	int *address = (int *) htoi(arg);
	for (int i = 0; i < n; i++, address++)
          printf("0x%p: %x\n", address, *address);
	}
      return 0;
      }
	}
  return 0;
}

void ui_mainloop(int is_batch_mode) {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  while (1) {
    char *str = rl_gets();
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef HAS_IOE
    extern void sdl_clear_event_queue(void);
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}
