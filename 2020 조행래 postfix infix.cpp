#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STACK_SIZE 100
#define MAX_EXPR_SIZE 100
typedef enum { lparen, rparen, plus, minus, times, divide, mod, square, eos, operand }precedence;
int isp[] = { 0,19,12,12,13,13,13,14,0 };
int icp[] = { 20,19,12,12,13,13,13,14,0 };

precedence stack[MAX_STACK_SIZE];
int stack2[MAX_STACK_SIZE];
char expr[MAX_EXPR_SIZE];

struct Example {
	precedence v1;
	int v2;
};

struct Example p[MAX_EXPR_SIZE];
struct Example p2[MAX_EXPR_SIZE];

int count = 0;
int count2 = 0;
int top;

void push(precedence num);
void push_int(int num);
int pop_int();
precedence pop();
void put_data(char* string);
void postfix();
int eval();

int main() {
	FILE* fp;
	char fname[20];

		printf("파일 이름? ");
		gets_s(fname);

		fopen_s(&fp, fname, "r");

		if (*fname == NULL)
			printf("프로그램 종료\n");
		if (fp == NULL)
			puts("파일오픈 실패\n");

		fgets(expr, MAX_EXPR_SIZE, fp);
		fclose(fp);

		printf("수식: %s \n", expr);


		char* string;
		char* context;

		string = strtok_s(expr, " ", &context);
		while (string != NULL) {
			put_data(string);
			string = strtok_s(NULL, " ", &context);

		}

		postfix();

		printf("후위수식 : ");
		for (int i = 0; i < count2; i++) {
			switch (p2[i].v1) {
			case 2:printf("+ "); break;
			case 3:printf("- "); break;
			case 4:printf("* "); break;
			case 5:printf("/ "); break;
			case 6:printf("%c ", '%'); break;
			case 7:printf("^ "); break;
			case 9:printf("%d ", p2[i].v2); break;
			}
		}

		printf("\n");

		printf("계산결과: %d\n", eval());
	
}

void push(precedence num) {
	if (top >= MAX_STACK_SIZE - 1) {
		printf("스택이 가득 찼습니다\n");
	}
	else
		stack[++top] = num;
}

void push_int(int num) {
	if (top >= MAX_STACK_SIZE - 1) {
		printf("스택이 가득 찼습니다\n");
	}
	else
		stack2[++top] = num;
}


precedence pop() {
	if (top == -1)
		printf("스택이 텅 비었습니다\n");
	else
		return stack[top--];
}

int pop_int() {
	if (top == -1)
		printf("스택이 텅 비었습니다\n");
	else
		return stack2[top--];
}

void put_data(char* string) {
	if (strcmp("(", string) == 0) {
		p[count].v1 = lparen;
		p[count].v2 = 0;
		count++;
	}
	else if (strcmp(")", string) == 0) {
		p[count].v1 = rparen;
		p[count].v2 = 0;
		count++;
	}
	else if (strcmp("+", string) == 0) {
		p[count].v1 = plus;
		p[count].v2 = 0;
		count++;
	}
	else if (strcmp("-", string) == 0) {
		p[count].v1 = minus;
		p[count].v2 = 0;
		count++;
	}
	else if (strcmp("/", string) == 0) {
		p[count].v1 = divide;
		p[count].v2 = 0;
		count++;
	}
	else if (strcmp("*", string) == 0) {
		p[count].v1 = times;
		p[count].v2 = 0;
		count++;
	}
	else if (strcmp("%", string) == 0) {
		p[count].v1 = mod;
		p[count].v2 = 0;
		count++;
	}
	else if (strcmp("^", string) == 0) {
		p[count].v1 = square;
		p[count].v2 = 0;
		count++;
	}
	else {
		p[count].v1 = operand;
		p[count].v2 = atoi(string);
		count++;
	}

}

void postfix() {
	top = -1;
	push(eos);
	precedence token;
	for (int i = 0; i < count; i++) {
		if (p[i].v1 == operand) {
			p2[count2].v1 = operand;
			p2[count2].v2 = p[i].v2;
			count2++;
		}
		else if (p[i].v1 == rparen) {
			while (stack[top] != lparen) {
				p2[count2].v1 = pop();
				p2[count2].v2 = 0;
				count2++;
			}
			pop();
		}
		else {
			while (isp[stack[top]] > icp[p[i].v1]) {
				p2[count2].v1 = pop();
				p2[count2].v2 = 0;
				count2++;
			}
			push(p[i].v1);
		}
		
	}
	while ((token = pop()) != eos) {
		p2[count2].v1 = token;
		p2[count2].v2 = 0;
		count2++;
	}
	
}

int eval() {
	int op1, op2;
	top = -1;
	int sq=1;
	for (int i = 0; i < count2; i++) {
		if (p2[i].v1 == operand)
			push_int(p2[i].v2);
		else {
			op2 = pop_int();
			op1 = pop_int();
			switch (p2[i].v1) {
			case plus: push_int(op1 + op2); break;
			case minus: push_int(op1 - op2); break;
			case times: push_int(op1 * op2); break;
			case divide: push_int(op1 / op2); break;
			case mod: push_int(op1 % op2); break;
			case square: for (int j = 0; j < op2; j++) sq = sq*op1;
				push_int(sq); sq = 1; break;
			}
		}
	}
	return pop_int();
}