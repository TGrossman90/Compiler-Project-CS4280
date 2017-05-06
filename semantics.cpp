#include "semantics.h"
#include "stack.h"

static int scope = 0;
int loc = 0;
int loopCount = 0;
int variablesOnStack = 0;
int tempVariables = 0;

FILE *fp;

token *temp;

void codeGen(Node *root, char *file) {
	string tempFile(file);
	string fileName = tempFile.substr(0, tempFile.find("."));
	fileName.append(".asm");
	
	fp = fopen(fileName.c_str(), "w");
	if(fp == NULL) {
		printf("Couldn't open file\n");
		exit(EXIT_FAILURE);
	}
	
	semantics(root);
	
	fprintf(fp, "STOP\n\n");
	popFromRealStack(scope, fp, variablesOnStack);

	
	while(tempVariables > 0) {
		fprintf(fp, "T%i 0\n", tempVariables - 1);
		tempVariables--;
	}
}

void semantics(Node *root) {
	if(root == NULL) {
		return;
	}
	
	// <program> -> <vars> <mvars>
	if(root->name == "<program>") {
		semantics(root->child1);
		semantics(root->child2);
	}
	
	// <block> -> start <vars> <stats> stop
	if(root->name == "<block>") {
		scope++;
//		for(int c = 0; c < scope; c++)
//			printf("  ");
//		printf("BLOCK\n");
		//printf("SCOPE: %i\n", scope);
		semantics(root->child1);
		semantics(root->child2);
		popFromRealStack(scope, fp, variablesOnStack);
		scope--;
		return;
	}
	
	// <vars> -> empty | int Identifier <mvars>	
	if(root->name == "<vars>") {
//		for(int c = 0; c < scope; c++)
//			printf("  ");
//		printf("VARS\n");	
		if(!root->tkn.empty()) {
			temp = root->tkn.front();
			root->tkn.erase(root->tkn.begin());
			
			stack test;
			test.tknWord = temp->tknWord;
			test.scope = scope;
			test.lineNum = temp->line;
			//printf("TKN: %s\tLINE: %i\tSCOPE: %i\n", test.tknWord.c_str(), test.lineNum, test.scope);
			
			if(temp->tknID == 1001) {
				pushToRealStack(test, fp);
				semantics(root->child1);
				variablesOnStack++;
			}
		} else {
			return;
		}
	}
	
	// <mvars> -> empty | : Identifier <mvars>
	if(root->name == "<mvars>") {
//		for(int c = 0; c < scope; c++)
//			printf("  ");
//		printf("MVARS\n");
		if(!root->tkn.empty()) {
			temp = root->tkn.front();
			root->tkn.erase(root->tkn.begin());
			
			stack test;
			test.tknWord = temp->tknWord;
			test.scope = scope;
			test.lineNum = temp->line;
			//printf("TKN: %s\tLINE: %i\tSCOPE: %i\n", test.tknWord.c_str(), test.lineNum, test.scope);
			
			if(temp->tknID == 1001) {
				pushToRealStack(test, fp);
				semantics(root->child1);
				variablesOnStack++;
				return;
			}
		} else {
			return;
		}
	}
	
	// <expr> -> <M> * <expr> | <M>
	if(root->name == "<expr>") {
//		for(int c = 0; c < scope; c++)
//			printf("  ");
//		printf("EXPR\n");
		if(root->child2 != NULL) {
			int local = tempVariables;
			tempVariables++;
				
			semantics(root->child1);
			fprintf(fp, "STORE T%i\n", local);
			semantics(root->child2);
			fprintf(fp, "MULT T%i\n", local);
			return;
		} else {
			semantics(root->child1);
			return;
		}
	}

	// <M> -> <T> / <M> | <T>
	if(root->name == "<M>") {
//		for(int c = 0; c < scope; c++)
//			printf("  ");
//		printf("M\n");
		if(root->child2 != NULL) {
			int local = tempVariables;
			tempVariables++;
			
			semantics(root->child2);
			fprintf(fp, "STORE T%i\n", local);
			semantics(root->child1);
			fprintf(fp, "DIV T%i\n", local);
			return;
			
		} else {
			semantics(root->child1);
			return;
		}
	}
	
	// <T> -> <F> + <T> | <F> - <T> | <F>
	if(root->name == "<T>") {
//		for(int c = 0; c < scope; c++)
//			printf("  ");
//		printf("T\n");
		if(!root->tkn.empty()) {
			int local = tempVariables;
			tempVariables++;
			
			temp = root->tkn.front();
			root->tkn.erase(root->tkn.begin());
			
			if(temp->tknID == 1011) {
				semantics(root->child1);
				fprintf(fp, "STORE T%i\n", local);
				semantics(root->child2);
				fprintf(fp, "ADD T%i\n", local);
				return;
				
			} else if(temp->tknID == 1012) {
				semantics(root->child2);
				fprintf(fp, "STORE T%i\n", local);
				semantics(root->child1);
				fprintf(fp, "SUB T%i\n", local);
				return;
			}
		} else {
			semantics(root->child1);
			return;
		}
	}
	
	// <F> -> & <F> | <R>
	if(root->name == "<F>") {
//		for(int c = 0; c < scope; c++)
//			printf("  ");
//		printf("F\n");
		
		if(root->child1 == NULL) {
			semantics(root->child2);
			return;
		} else {
			semantics(root->child1);
			fprintf(fp, "MULT -1\n");
			return;
		}
	}
	
	// <R> -> ( <expr> ) | Identifier | Number 
	if(root->name == "<R>") {
//		for(int c = 0; c < scope; c++)
//			printf("  ");
//		printf("R\n");
		if(!root->tkn.empty()) {
			temp = root->tkn.front();
			root->tkn.erase(root->tkn.begin());
			stack tmp;
			
			tmp.tknWord = temp->tknWord;
			tmp.lineNum = temp->line;
			tmp.scope = scope;
			
			if(temp->tknID == 1001) {
				fprintf(fp, "STACKR %i\n", find(tmp));
				return;
			} else if(temp->tknID == 1002) {
				fprintf(fp, "LOAD %s\n", tmp.tknWord.c_str());
				return;
			}
		} else {
			semantics(root->child1);	
			return;
		}
	}
	
	// <stats> -> <stat> <mStat>
	if(root->name == "<stats>") {
//		for(int c = 0; c < scope; c++)
//			printf("  ");
//		printf("STATS\n");
		
		semantics(root->child1);
		semantics(root->child2);
		return;
	}
	
	// <mStat> -> empty | <stat> <mStat>
	if(root->name == "<mStat>") {
//		for(int c = 0; c < scope; c++)
//			printf("  ");
//		printf("MSTAT\n");
		
		if(root->child1 != NULL) {
			semantics(root->child1);
			semantics(root->child2);
			return;
		}
	}	
	
	//<stat> -> <in> | <out> | <block> | <if> | <loop> | <assign>
	if(root->name == "<stat>") {
//		for(int c = 0; c < scope; c++)
//			printf("  ");
//		printf("STAT\n");
		
		semantics(root->child1);
		return;
	}
	
	// <in> -> read >> Identifier ;
	if(root->name == "<in>") {
//		for(int c = 0; c < scope; c++)
//			printf("  ");
//		printf("IN\n");
		if(!root->tkn.empty()) {
			temp = root->tkn.front();
			root->tkn.erase(root->tkn.begin());
			stack tmp;
			
			tmp.tknWord = temp->tknWord;
			tmp.lineNum = temp->line;
			tmp.scope = scope;
			
			int local = tempVariables;
			tempVariables++;
			
			fprintf(fp, "READ T%i\n", local);
			fprintf(fp, "LOAD T%i\n", local);
			fprintf(fp, "STACKW %i\n", find(tmp));
			return;
		}
		
		return;
	}
	
	// <out> -> print << <expr>
	if(root->name == "<out>") {
//		for(int c = 0; c < scope; c++)
//			printf("  ");
//		printf("OUT\n");
		int local = tempVariables;
		tempVariables++;
		
		semantics(root->child1);
		fprintf(fp, "STORE T%i\n", local);
		fprintf(fp, "WRITE T%i\n", local);	
		return;
	}
	
	// <if> -> decision ( <expr> <RO> <expr> ) <block>
	if(root->name == "<if>") {
//		for(int c = 0; c < scope; c++)
//			printf("  ");
//		printf("IF\n");
		
		int thisLoop = ++loopCount;
		int local = tempVariables;
		tempVariables++;
		
		// Child 3 <RO> Child 2
		
		fprintf(fp, "\nloop%i: ", loopCount);
		// Child 1 == <expr>
		semantics(root->child1);
		fprintf(fp, "STORE T%d\n", local);
		// Child 3 == <expr>
		semantics(root->child3);
		fprintf(fp, "SUB T%d\n", local);
		// Child 2 == <RO>
		semantics(root->child2);
		// Child 4 == <block>
		semantics(root->child4);
		fprintf(fp, "end%d: NOOP\n\n", thisLoop);
		
		return;
	}
	
	// <loop> -> while ( <expr> <RO> <expr> ) <block>
	if(root->name == "<loop>") {
//		for(int c = 0; c < scope; c++)
//			printf("  ");
//		printf("LOOP\n");
		
		int thisLoop = ++loopCount;
		int local = tempVariables;
		tempVariables++;
		
		fprintf(fp, "\nloop%i: ", loopCount);
		// Child 1 == <expr>
		semantics(root->child1);
		fprintf(fp, "STORE T%d\n", local);
		// Child 3 == <expr>
		semantics(root->child3);
		fprintf(fp, "SUB T%d\n", local);
		// Child 2 == <RO>
		semantics(root->child2);
		// Child 4 == <block>
		semantics(root->child4);
		fprintf(fp, "BR loop%d\n", thisLoop);
		
		fprintf(fp, "end%d: NOOP\n\n", thisLoop);
		
		return;	
	}
		
	// <assign> -> Identifier = <expr> ;	
	if(root->name == "<assign>") {
//		for(int c = 0; c < scope; c++)
//			printf("  ");

		semantics(root->child1);
		
		if(!root->tkn.empty()) {
			temp = root->tkn.front();
			root->tkn.erase(root->tkn.begin());
			
			stack tmp;
			tmp.tknWord = temp->tknWord;
			tmp.lineNum = temp->line;
			tmp.scope = scope;
			
			fprintf(fp, "STACKW %i\n\n", find(tmp));
			return;

		}
	}			
	
	// <RO> -> << | << = | >> | >> = | == | =!
	if(root->name == "<RO>") {
//		for(int c = 0; c < scope; c++)
//			printf("  ");
		if(!root->tkn.empty()) {
			temp = root->tkn.front();
			root->tkn.erase(root->tkn.begin());
			
			if(temp->tknWord == "<<") {
				if(!root->tkn.empty()) {
					token *temp2 = root->tkn.front();
					root->tkn.erase(root->tkn.begin());
					
					if(temp2->tknWord == "=") {
						fprintf(fp, "BRNEG end%d\n", loopCount);
						return;
					}
				}
				
				fprintf(fp, "BRZNEG end%d\n", loopCount);
				return;
			}
			
			if(temp->tknWord == ">>") {
				if(!root->tkn.empty()) {
					token *temp2 = root->tkn.front();
					root->tkn.erase(root->tkn.begin());
					
					if(temp2->tknWord == "=") {
						fprintf(fp, "BRPOS end%d\n", loopCount);
						return;
					}
				}
				
				fprintf(fp, "BRZPOS end%d\n", loopCount);
				return;
			}
			
			if(temp->tknWord == "==") {
				fprintf(fp, "BRPOS end%d\n", loopCount);
				fprintf(fp, "BRNEG end%d\n", loopCount);
				return;
			}
			
			if(temp->tknWord == "=!") {
				fprintf(fp, "BRZERO end%d\n", loopCount);
				return;
			}
		}
	}
}

void check(token *tkn) {
	stack tmp;
	tmp.tknWord = tkn->tknWord;
	tmp.lineNum = tkn->line;
	tmp.scope = scope;
	
	int result = searchStack(tmp);
	if(result > 0) {
		cout << "Variable " << tmp.tknWord << " has been previously declared on line " << result << "\n";	
		exit(EXIT_FAILURE);
	} else {
		//cout << "Variable " << tmp.tknWord << " has been stored\n";
		push(tmp);
	}
}
