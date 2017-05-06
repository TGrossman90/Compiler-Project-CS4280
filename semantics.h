#ifndef SEMANTICS_H
#define SEMANTICS_H

#include "node.h"
#include "token.h"
#include "stack.h"
#include "parser.h"

void semantics(Node*);
void check(token*);
int checkScope(stack);
int checkExists(stack);
void codeGen(Node*, char*);

#endif