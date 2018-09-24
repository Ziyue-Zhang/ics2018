#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
#include <stdlib.h>
#define set_tokens tokens[nr_token].type=rules[i].token_type;\
	    nr_token++;
uint32_t eval(int p, int q);
bool check_parentheses(int p, int q);
enum {
  TK_NOTYPE = 256, TK_EQ, HEX, NUM, TK_NOT, TK_AND, TK_OR, TK_NEQ, TK_LEQ, TK_GEQ, TK_LS, TK_GT, MINUS, POINTER, EAX = 300, ECX, EDX, EBX, ESP, EBP, ESI, EDI, EIP

  /* TODO: Add more token types */

};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
    */

  {"\\$eax",EAX},			// EAX
  {"\\$ecx",ECX},			// ECX
  {"\\$edx",EDX},			// EDX
  {"\\$ebx",EBX},			// EBX
  {"\\$esp",ESP},			// ESP
  {"\\$ebp",EBP},			// EBP
  {"\\$esi",ESI},			// ESI
  {"\\$edi",EDI},			// EDI
  {"\\$eip",EIP},			// EIP
  {"<=",TK_LEQ},
  {"!=",TK_NEQ},
  {">=",TK_GEQ},
  {"<",TK_LS},
  {">",TK_GT},
  {"!",TK_NOT},				//not
  {"&&",TK_AND},				//and
  {"\\|\\|",TK_OR},			//or
  {" +", TK_NOTYPE},		// spaces
  {"0x[A-Fa-f0-9]+",HEX},	// HEX
  {"[0-9]+",NUM},           // number
  {"\\(", '('},				// left bracket
  {"\\)", ')'},				// right bracket
  {"\\*", '*'},				// multiply
  {"/", '/'},				// divide
  {"\\+", '+'},				// plus
  {"\\-", '-'},				// minus
  {"==", TK_EQ}				// equal
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
  char str[33];
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
			 int add;
	 		 switch (rules[i].token_type) {
				 case EAX:
			     case ECX:
				 case EDX:
				 case EBX:
				 case ESP:
				 case EBP:
				 case ESI:
				 case EDI:
				 case EIP:
					add = rules[i].token_type - 300;
					uint32_t* address = (uint32_t*) &cpu;
					snprintf(tokens[nr_token].str,32, "%d", *(address + add));
					tokens[nr_token].type = NUM;
					nr_token++;
				    break;
				 case TK_OR:
					set_tokens;
					break;
				 case TK_AND:
					set_tokens;
					break;
				 case TK_EQ:
				 case TK_NEQ:
					set_tokens;
					break;
				 case TK_GEQ:
				 case TK_LEQ:
				 case TK_LS:
				 case TK_GT:
					set_tokens;
					break;
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
					if(substr_len > 32)
						assert(0);
					strncpy(tokens[nr_token].str,e+position-substr_len,substr_len);
					set_tokens;
					tokens[nr_token].str[32]='\0'; 
					break;
				 case HEX:
					if(substr_len > 8)
						assert(0);
					strncpy(tokens[nr_token].str,e+position-substr_len,substr_len); 
					tokens[nr_token].str[8]='\0'; 
					int temp;
					sscanf(tokens[nr_token].str,"0x%x",&temp);
					snprintf(tokens[nr_token].str,32,"%d",temp);	
					tokens[nr_token].type=NUM; nr_token++;
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
	/*for(int i = 0;i != nr_token; i++)
	{
		if (tokens[i].type == '*' && (i == 0 || tokens[i-1].type != NUM))
			tokens[i].type = POINTER;
		else if (tokens[i].type == '-' && ( i==0 || tokens[i-1].type != NUM))
			tokens[i].type = MINUS;
 	} */
	int bracket = 0;
	for(int i = 0; i != nr_token; i++)
	{ 
		if (bracket < 0)
		{
			*success = false;
			return 0;
		} 
		if (tokens[i].type == '(')
			bracket++;
		else if (tokens[i].type == ')')
			bracket--;
 	}
	if (bracket != 0)
 	{
		*success = false;
		return 0;
	} 
	/* TODO: Insert codes to evaluate the expression. */
  

	*success = true;
	return eval(0, nr_token - 1);
}

uint32_t eval(int p, int q)
{
	//printf("%d\t%d\n",p,q);
	if (p > q)
		assert(0);
	else if (p == q)
	{
		if(tokens[p].type == NUM)
			return atoi(tokens[p].str);
		else
			assert(0);
	} 
	else if (check_parentheses(p, q))
		return eval(p + 1, q - 1);
	else
	{ 
		int temp = p + 1;
		int op = 0;
		if (tokens[p].type == '(')
	 	{
			for(; tokens[temp].type != ')';  temp++);
			temp++;
			op = tokens[temp].type;
		//	printf("%d\n",op);
 		}
		else
	 	{
			bool flag = false;
			for (int i = p + 1; i < q && tokens[i].type != '('; i++)
				if (tokens[i].type == '/' || tokens[i].type == '*')
				{
					flag = true;
					temp = i;
 	 			}
			for (int i = p + 1; i < q && tokens[i].type != '('; i++)
				if (tokens[i].type == '-' || tokens[i].type == '+')
 				{
					flag = true;
					temp = i;
	 			}  
			if(flag)
				op = tokens[temp].type;
			else
				assert(0);
  		}
		uint32_t val1 = eval(p, temp-1);
	    uint32_t val2 = eval(temp + 1, q);
	    switch (op)
 		{ 
		    case '+': return val1 + val2;
			case '-': return val1 - val2;
		    case '*': return val1 * val2;
		    case '/': return val1 / val2;
		    default: assert(0);
	  	}
 	}
}

bool check_parentheses(int p, int q)
{ 
	int bracket = 0;
	for(int i = p; i <=  q; i++)
	{  
		if (bracket < 0)
			return false;
		if (tokens[i].type == '(')
			bracket++;
		else if (tokens[i].type==')')
			bracket--;
		if (bracket == 0 && i != q)
			return false;
  	}
	if (bracket != 0)
		return false;
	return true;
}
