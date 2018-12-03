#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP];
static WP *head, *free_;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */
void new_wp(char* args){
	//printf("args = %s\n",args);
	WP* x;
	if(free_ == NULL){
		panic("wp_pool is out of space.");
	}
	else{
		x = free_;
		free_ = x->next;
		x->next = NULL;
		//printf("x: NO=%d\n",x->NO);
		if(head == NULL){
			head = x;
			//printf("Head\n");
		}
		else{
			WP* cur = head;
			while(cur->next) cur = cur->next;
			cur->next = x;
		}
	}
	strcpy(x->exp,args);
	//printf("arg = %s,exp = %s\n",args,x->exp);
	bool success = true;
	x->value = expr(args,&success);
	//printf("wp%d is built. exp = %s, value = %u\n",x->NO,x->exp,x->value);
}

void free_wp(WP *wp){
	//printf("free wp%d\n",wp->NO);
	wp->next = NULL;
	WP* cur = free_;
	if(cur == NULL) free_ = wp;
	else{
		while(cur->next) cur = cur->next;
		cur->next = wp; 
	}
	//wp->next = free_;
	//free_ = wp;
}

void del(int num){
	WP* cur = head;
	if(cur->NO == num){
		//printf("head->NO = num\n");
		head = head->next;
		free_wp(cur);
	}
	else{
		bool found = false;
		while(cur->next){
			if(cur->next->NO == num){
				//printf("wp%d->next->NO == num\n",cur->NO);
				found = true; 
				WP* temp = cur->next;
				cur->next = temp->next;
				//printf("wp%d is free now. Previous: wp%d\n",temp->NO,cur->NO);
				free_wp(temp);
				break;
			}
			cur = cur->next;
		}
		if(!found) Log("NO watchpoint %d",num);
	}
}

void wp_info(){
	printf("NO\texpression\tcurrent value\n");
	WP* cur = head;
	while(cur){
		printf("%d\t%s\t\t%d\n",cur->NO,cur->exp,cur->value);
		cur = cur->next;
	}
}

bool check(){
	WP* cur = head;
	bool success = true;
	bool flag = false;
	while(cur){
		uint32_t ans = expr(cur->exp,&success); 
		if(ans != cur->value){
			printf("Watchpoint %d: %s\nOld value:%u\nNew value:%u\n",cur->NO,cur->exp,cur->value,ans);
			cur->value = ans;
			flag = true;
		}
		cur = cur->next;
	}
	if(flag)return true;
	return false;
}

