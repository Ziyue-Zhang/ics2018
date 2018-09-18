#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
#define set_tokens tokens[nr_token].type=rules[i].token_type;\
	    nr_token++;

enum {
  TK_NOTYPE = 256, TK_EQ, NUM, MINUS, POINTER

  /* TODO: Add more token types */

};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces
  {"[0-9]+",NUM},		// number
  {"\\(", '('},			// left bracket
  {"\\)", ')'},			// right bracket
  {"\\*", '*'},			// multiply
  {"/", '/'},			// divide
  {"\\+", '+'},			// plus
  {"-", '-'},			// minus
  {"==", TK_EQ}			// equal
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
	 /* Try all rules one by one. */
	 for (i = 0; i < NR_REGEX; i ++) {
		 if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
			 char *substr_start = e + position;
			 int substr_len = pmatch.rm_eo;
		     
			 Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
				 i, rules[i].regex, position, substr_len, substr_len, substr_start);
			 position += substr_len;

			 /* TODO: Now a new token is recognized with rules[i]. Add codes
			  * to record the token in the array `tokens'. For certain types
			  * of tokens, some extra actions should be performed.
			  */

			 switch (rules[i].token_type) {
			     case'+':
			     case'-':
					set_tokens;
					break;
				 case'*':
				 case'/':
					set_tokens;
					break;
				 case'(':
				 case')':
					set_tokens;
					break;
				 case NUM:
					if(substr_len > 31)
						assert(0);
					strncpy(tokens[nr_token].str,e+position-substr_len,substr_len);
					set_tokens;
					break;
				 default: panic("wrong");
			 }

			break;
		 }
	 }		
	 if (i == NR_REGEX) {
		 printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
		 return false; 	 
	}
}

  return true;
}

uint32_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
  TODO();

  return 0;
}



bool check_parentheses(char *e, int p, int q)
{
	int bracket = 0;
	for(int i = p; i < q; i++)
	{
		if(bracket < 0)
			return false;
		if(tokens[i].type == '(')
			bracket++;
		else if(tokens[i].type==')')
			bracket--;
		if(bracket == 0 && i != q)
			return false;
	}
	if(bracket != 0)
		return false;
	return true;
}
