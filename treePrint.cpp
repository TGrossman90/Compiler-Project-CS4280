#include <string>
#include <fstream>
#include <vector>
#include "treePrint.h"

using namespace std;
	
void printTree(Node *nodePtr, int level) 
{
	if(nodePtr == NULL) {
		return;
	}
	
	string line;
	token *tkn;
	for(int c = 0; c < level; c++) {
		line.append("  ");
	}
	
	line.append(nodePtr->name.c_str());
	line.append(" ");
	for(int i = 0; i < nodePtr->tkn.size(); i++) {
		tkn = nodePtr->tkn[i];
		line.append(tkn->tknWord);
		line.append(" ");
	}
	
// If uncommented, skips empty tokens	
//	if(nodePtr->tkn.size() > 0) {
		printf("%s\n", line.c_str());
//	}
	
	printTree(nodePtr->child1, level + 1);
	printTree(nodePtr->child2, level + 1);
	printTree(nodePtr->child3, level + 1);
	printTree(nodePtr->child4, level + 1);
}