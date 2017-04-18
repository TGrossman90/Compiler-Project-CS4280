#ifndef NODE_H
#define NODE_H

#include <vector>
#include <string>
#include "token.h"

using namespace std;

struct Node {
	string name;
	vector<token*> tkn;
	struct Node *child1; 
	struct Node *child2;
	struct Node *child3;
	struct Node *child4; 
	int level;
};

#endif