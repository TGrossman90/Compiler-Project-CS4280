#include "semantics.h"
#include "stack.h"

static int scope = 0;
int loc = 0;
int inLoop = 0;

token *temp;

void semantics(Node *root) {
	if(root == NULL) {
		return;
	}
	
	if(root->name == "<program>") {
		printf("PROGRAM\n");
		
		semantics(root->child1);
		semantics(root->child2);
	}
	
	if(root->name == "<vars>") {
		printf("VARS\n");	
		if(!root->tkn.empty()) {
			temp = root->tkn.front();
			root->tkn.erase(root->tkn.begin());
			if(temp->tknID == 1001) {
				//cout << "Var " << temp->tknWord << "\n";
				check(temp);
				semantics(root->child1);
			}
		} else {
			return;
		}
	}
	
	if(root->name == "<block>") {
		printf("BLOCK\n");
		scope++;
		printf("SCOPE: %i\n", scope);
		semantics(root->child1);
		semantics(root->child2);
		pop(scope);
		scope--;
		printf("SCOPE: %i\n", scope);
		return;
	}
	
	if(root->name == "<mvars>") {
		printf("MVARS\n");
		if(!root->tkn.empty()) {
			temp = root->tkn.front();
			root->tkn.erase(root->tkn.begin());

			if(temp->tknID == 1001) {
				check(temp);
				semantics(root->child1);
			}
		} else {
			return;
		}
	}
	
	if(root->name == "<expr>") {
		printf("EXPR\n");
		if(!root->tkn.empty()) {
			semantics(root->child1);
			semantics(root->child2);
			return;
		} else {
			semantics(root->child1);
			return;
		}
	}

	if(root->name == "<M>") {
		printf("M\n");
		if(!root->tkn.empty()) {
			semantics(root->child1);
			semantics(root->child2);
			return;
		} else {
			semantics(root->child1);
			return;
		}
	}
	
	if(root->name == "<T>") {
		printf("T\n");
		if(!root->tkn.empty()) {
			semantics(root->child1);
			semantics(root->child2);
			return;
		} else {
			semantics(root->child1);
			return;
		}
		
	}
	
	if(root->name == "<F>") {
		printf("F\n");
		if(!root->tkn.empty()) {
			semantics(root->child1);
			return;
		} else {
			printf("R\n");
			semantics(root->child2);
			return;
		}
	}
	
	if(root->name == "<R>") {
		printf("R\n");
		if(!root->tkn.empty()) {
			temp = root->tkn.front();
			root->tkn.erase(root->tkn.begin());
			stack tmp;
			
			tmp.tknWord = temp->tknWord;
			tmp.lineNum = temp->line;
			tmp.scope = scope;
			
			if(temp->tknID == 1001) {
				if(checkScope(tmp)) {
					cout << "Var: " << tmp.tknWord << "\n";
					return;
				} else {
					cout << "Var: " << tmp.tknWord << " has not been declared in this scope\n";
					exit(EXIT_SUCCESS);
				}
			} else if(temp->tknID == 1002) {
				cout << "Num: " << tmp.tknWord << "\n";
				return;
			}
		} else {
			semantics(root->child1);		
		}
	}


	if(root->name == "<mStat>") {
		printf("MSTATS\n");
		
	}	
	
	if(root->name == "<in>") {
		printf("IN\n");
		temp = root->tkn.front();
		root->tkn.erase(root->tkn.begin());

		if(temp->tknID == 1001) {
			check(temp);
			return;
		}
	}
	
	if(root->name == "<out>") {
		printf("OUT\n");

		semantics(root->child1);
		return;
	}
	
	if(root->name == "<if>") {
		printf("IF\n");

		semantics(root->child1);
		semantics(root->child2);
		semantics(root->child3);
		semantics(root->child4);
		
		return;
	}
	
	if(root->name == "<loop>") {
		printf("LOOP\n");
		
		semantics(root->child1);
		semantics(root->child2);
		semantics(root->child3);
		semantics(root->child4);
		
		return;	
	}
		
	if(root->name == "<assign>") {
		
		
	}	
	
	if(root->name == "<RO>") {
		
		
	}
	
	semantics(root->child1);
	semantics(root->child2);
	semantics(root->child3);
	semantics(root->child4);
}

void check(token *tkn) {
	stack tmp;
	tmp.tknWord = tkn->tknWord;
	tmp.lineNum = tkn->line;
	tmp.scope = scope;
	
	int result = searchStack(tmp);
	if(result > 0) {
		cout << "Variable " << tmp.tknWord << " has been previously declared on line " << result << "\n";	
		exit(EXIT_SUCCESS);
	} else {
		cout << "Variable " << tmp.tknWord << " has been stored\n";
		push(tmp);
	}
}