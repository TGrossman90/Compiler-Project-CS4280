#ifndef STACK_H
#define STACK_H

#include "scanner.h"
#include "node.h"

typedef struct Stack {
	string tknWord;
	int lineNum;
	int scope;
}stack;

extern stack theStack[];

int getScope();
int getPreviousDeclaration(stack);
int searchStack(stack);
void push(stack);
void pop(int);


#endif 