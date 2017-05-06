#include "stack.h"

stack theStack[100];
deque<stack> theRealStack;
extern int loc;

int searchRealStack(stack test) {
	int size = theRealStack.size();
	
	if(!theRealStack.size()) {
		return -1;
	}
	
	for(int c = 0; c < theRealStack.size(); c++) {
		if((theRealStack[c].tknWord == test.tknWord && theRealStack[c].scope == test.scope)) {
			return theRealStack.at(c).lineNum;
		}
	}
	return -1;
}

int find(stack test) {
	int size = theRealStack.size();
	//size--;
	stack temp;
	
	for(int c = 0; c < size; c++) {
		if((theRealStack[c].tknWord == test.tknWord) && (theRealStack[c].scope <= test.scope)) {
			return c;
		}
	}
	
	printf("Var %s has not been declared in this scope\n", test.tknWord.c_str());
	exit(EXIT_FAILURE);
}

void pushToRealStack(stack test, FILE *fp) {
	int result = searchRealStack(test);

	if(result == -1) {
		theRealStack.push_front(test);
		fprintf(fp, "PUSH\n");
	} else {
		printf("Var %s was previously declared on line #%i\n", test.tknWord.c_str(), result);
		exit(EXIT_FAILURE);
	}
}

void popFromRealStack(int scope, FILE *fp, int &vars) {
	int size = theRealStack.size();
	size--;
	
	for(size; size >= 0; size--) {
		if(theRealStack[size].scope == scope) {
			fprintf(fp, "POP\n");
			theRealStack.erase((theRealStack.begin() + size));
			vars--;
		}
	}
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
		if((theStack[indx].tknWord == test.tknWord) && (theStack[indx].scope <= test.scope)) {
			return indx;
		}
	}
	return -1;
}

int checkScope(stack test) {
	int lo = checkExists(test);
	//printf("CHECK SCOPE: %i\n", test.scope);
	if(lo >= 0) {
		if(theStack[lo].scope > test.scope) {
			return 0;
		} else {
			return 1;
		}
	}
}

int checkExistsScope(stack test) {
	int indx = loc;
	for(int c = 0; c < indx; c++) {
		if(theStack[c].tknWord == test.tknWord) {
			if(theStack[c].scope <= test.scope) {
				return c;
			}
		}
	}	
}

void push(stack test) {
	theStack[loc] = test;
	loc++;
}

void pop(int scope, FILE *fp, int &vars) {
	int indx = loc;
	
	stack temp;
	temp.tknWord = "";
	temp.lineNum = -1;
	temp.scope = -1;
	
	if(scope > 0) {
		for(indx; indx >= 0; indx--) {
			if(theStack[indx].scope == scope) {
				theStack[indx] = temp;
				fprintf(fp, "POP\n\n");
				loc--;
				vars--;
			}
		}
		printf("\n");
	}
}

void pop(FILE *fp, int &vars) {
	stack temp;
	temp.tknWord = "";
	temp.lineNum = -1;
	temp.scope = -1;
	
	theStack[loc] = temp;
	fprintf(fp, "POP\n\n");
	loc--;
	vars--;
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

void printStack() {
	int indx = loc;
	int c = 0;
	for(indx; indx >= 0; indx--) {
		printf("%i : %s : %i\n", c, theStack[indx].tknWord.c_str(), indx);
		c++;
	}
}

void reorganizeStack() {
	int indx = loc;
	
	stack temp;
	temp.tknWord = "";
	temp.lineNum = -1;
	temp.scope = -1;
	for(indx; indx >= 0; indx--) {
		theStack[indx + 1] = theStack[indx];
		theStack[indx] = temp;
	}
}
		