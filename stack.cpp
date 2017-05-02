#include "stack.h"

stack theStack[100];
extern int loc;

int getScope() {
	return theStack[loc - 1].scope;
}

int searchStack(stack test) {
	int indx = loc;
	//printf("SCOPE: %i\n", test.scope);
	
	for(indx; indx >= 0; indx--) {
		if((theStack[indx].tknWord == test.tknWord) && (theStack[indx].scope == test.scope)) {
			return theStack[indx].lineNum;
		}
	}
	return 0;
}

int checkExists(stack test) {
	int indx = loc;
	for(indx; indx >= 0; indx--) {
		if((theStack[indx].tknWord == test.tknWord)) {
			return indx;
		}
	}
	return -1;
}

int checkScope(stack test) {
	int loc = checkExists(test);
	//printf("CHECK SCOPE: %i\n", test.scope);
	if(loc >= 0) {
		if(theStack[loc].scope > test.scope) {
			return 0;
		} else {
			return 1;
		}
	}
}

void push(stack test) {
	theStack[loc] = test;
	loc++;
}

void pop(int scope) {
	int indx = loc;
	
	stack temp;
	temp.tknWord = "";
	temp.lineNum = 0;
	temp.scope = -1;
	
	if(scope > 0) {
		for(indx; indx >= 0; indx--) {
			if(theStack[indx].scope == scope) {
				theStack[indx] = temp;
			}
		}
	}
}

int getPreviousDeclaration(stack test) {
	int indx = loc;
	
	for(indx; indx >= 0; indx--) {
		if(theStack[indx].tknWord == test.tknWord) {
			return theStack[indx].lineNum;
		}
	}
	
	return -1;
}