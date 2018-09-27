#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
static char buf[65536];
/*static inline void gen_rand_expr() {
  buf[0] = '\0';
}*/
unsigned int choose(unsigned int n)
{
	return rand() % n;
}
void gen_num(unsigned loc)
{
	switch (choose(10))
	{
	case 0:buf[loc] = '0'; break;
	case 1:buf[loc] = '1'; break;
	case 2:buf[loc] = '2'; break;
	case 3:buf[loc] = '3'; break;
	case 4:buf[loc] = '4'; break;
	case 5:buf[loc] = '5'; break;
	case 6:buf[loc] = '6'; break;
	case 7:buf[loc] = '7'; break;
	case 8:buf[loc] = '8'; break;
	case 9:buf[loc] = '9'; break;
	default: break;
	}
}
void gen_rand_op(unsigned loc)
{
	switch (choose(4))
	{
	case 0:buf[loc] = '+'; break;
	case 1:buf[loc] = '-'; break;
	case 2:buf[loc] = '*'; break;
	case 3:buf[loc] = '/'; break;
	default: break;
	}
}
void gen_rand_expr(unsigned int head, unsigned int tail)
{
	if (tail - head >= 2)
	{
		switch (choose(10))
		{
		case 0:
		case 1:
		{
			if (tail - head >= 2)
				gen_rand_expr(head, tail);
			else
			{
				if ((head == 0 && head == tail) ||(head > 0 && buf[head - 1] > '0' && buf[head - 1] < '9'))
					for (unsigned int i = head; i <= tail; i++)
						gen_num(i);
				else
				{
					buf[head] = choose(9) + 1 + '0';
					for (unsigned int i = head + 1; i <= tail; i++)
						gen_num(i);
				} 
			}
		}
		break;
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		{ 
			unsigned int b = choose(tail - head) + head;
			if (b == head)
				b++;
			else if (b == tail)
				b--;
			gen_rand_op(b);
			gen_rand_expr(head, b - 1);
			gen_rand_expr(b + 1, tail);
		} 
		break;
		default:
			buf[head] = '('; gen_rand_expr(head + 1, tail - 1); buf[tail] = ')';
		break;
		}
	}
	else
	{ 
		if ((head == 0 && head == tail) ||( head>0 && buf[head - 1] > '0' && buf[head - 1] < '9'))
			for (unsigned int i = head; i <= tail; i++)
				gen_num(i);
		else
		{
			buf[head] = choose(9) + 1 + '0';
			for (unsigned int i = head + 1; i <= tail; i++)
				gen_num(i);
		}
	} 
}
static char code_buf[65536];
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
	uint32_t temp = choose(200);  
    temp++;
	buf[temp]='\0';
	gen_rand_expr(0,temp-1);

    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen(".code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc .code.c -o .expr");
    if (ret != 0) continue;

    fp = popen("./.expr", "r");
    assert(fp != NULL);

    int result;
    fscanf(fp, "%d", &result);
    pclose(fp);

    printf("%u %s\n", result, buf);
  }
  return 0;
}
