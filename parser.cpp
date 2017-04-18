#include "parser.h"

token *tkn;

Node *parser() {
	tkn = driver();
	
	// Create root node
	Node *root = NULL;
	root = program();
	
	if(tkn->tknID == 1100) {
		root->tkn.push_back(tkn);
		
		return root;
	} else {
		printf("Error on line %i: Could not find EOF token\n", tkn->line);
		exit(EXIT_FAILURE);
	}
}

// <program> -> program <vars> <block>
Node *program() {
	//printf("Entered program\n");
	// Create new node
	Node *newNode = createNode("<program>");
	
	if(tkn->tknID == 1001 && tkn->tknWord == "program") {
		newNode->tkn.push_back(tkn);
		tkn = driver();
		
		newNode->child1 = vars();
		newNode->child2 = block();
		
		return newNode;
		
	} else {
		printf("Error on line %d: Expected \"program\"\n", tkn->line);
		exit(EXIT_FAILURE);
	}
}

// <block> -> start <vars> <stats> stop
Node *block() {
	//printf("Entered block\n");
	// Create new node
	Node *newNode = createNode("<block>");
	
	if(tkn->tknID == 1001 && tkn->tknWord == "start") {
		newNode->tkn.push_back(tkn);
		tkn = driver();
		
		newNode->child1 = vars();
		newNode->child2 = stats();
		
		if(tkn->tknID == 1001 && tkn->tknWord == "stop") {
			newNode->tkn.push_back(tkn);
			tkn = driver();
			
			return newNode;
			
		} else {
			printf("Error on line %d: Expected \"stop\"\n", tkn->line);
			exit(EXIT_FAILURE);
		}
		
	} else {
		printf("Error on line %d: Expected \"start\" tkn\n", tkn->line);
		exit(EXIT_FAILURE);
	}
}

// <vars> -> empty | int Identifier <mvars>
Node *vars() {
	//printf("Entered vars\n");
	// Create new node
	Node *newNode = createNode("<vars>");
	
	if(tkn->tknID == 1001 && tkn->tknWord == "int") {
		newNode->tkn.push_back(tkn);
		tkn = driver();
		
		if(tkn->tknID == 1001) {
			newNode->tkn.push_back(tkn);
			tkn = driver();
			newNode->child1 = mvars();
			
			return newNode;
			
		} else {
			printf("Error on line %d: Expected ID_TKN\n", tkn->line);
			exit(EXIT_FAILURE);
		}
	}
	
	return newNode;
}

// <mvars> -> empty | : Identifier <mvars>
Node *mvars() {
	//printf("Entered mvars\n");
	// Create new node
	Node *newNode = createNode("<mvars>");
	
	if(tkn->tknID == 1010) {
		newNode->tkn.push_back(tkn);
		tkn = driver();

		if(tkn->tknID == 1001 && !tkn->keyword) {
			newNode->tkn.push_back(tkn);
			tkn = driver();
			newNode->child1 = mvars();
			
			return newNode;
			
		} else {
			printf("Error on line %d: Expected ID_TKN\n", tkn->line);
			exit(EXIT_FAILURE);
		}
	}
	
	return newNode;
}

// <expr> -> <M> * <expr> | <M>
Node *expr() {
	//printf("Entered expr\n");
	// Create new node
	Node *newNode = createNode("<expr>");
	
	newNode->child1 = M();
	
	if(tkn->tknID == 1013) {
		newNode->tkn.push_back(tkn);
		tkn = driver();
		newNode->child2 = expr();
		
		return newNode;
	}
	
	return newNode;
}

// <M> -> <T> / <M> | <T>
Node *M() {
	//printf("Entered M\n");
	// Create new node
	Node *newNode = createNode("<M>");
	
	newNode->child1 = T();
	
	if(tkn->tknID == 1026) {
		newNode->tkn.push_back(tkn);
		tkn = driver();
		newNode->child2 = M();
		
		return newNode;
	}
	
	return newNode;
}

// <T> -> <F> + <T> | <F> - <T> | <F>
Node *T() {
	//printf("Entered T\n");
	// Create new node
	Node *newNode = createNode("<T>");
	
	newNode->child1 = F();
	
	if(tkn->tknID == 1011 || tkn->tknID == 1012) {
		newNode->tkn.push_back(tkn);
		tkn = driver();
		newNode->child2 = T();
		
		return newNode;
	}
	
	return newNode;
}

// <F> -> & <F> | <R>
Node *F() {
	//printf("Entered F\n");
	// Create new node
	Node *newNode = createNode("<F>");
	
	if(tkn->tknID == 1014) {	
		newNode->tkn.push_back(tkn);
		tkn = driver();
		newNode->child1 = F();
		
		return newNode;
	}
	
	newNode->child1 = R();
	
	return newNode;
}

// <R> -> ( <expr> ) | Identifier | Number 
Node *R() {
	//printf("Entered R\n");
	// Create new node
	Node *newNode = createNode("<R>");
	
	if(tkn->tknID == 1017) {
		newNode->tkn.push_back(tkn);
		tkn = driver();
		newNode->child1 = expr();
		
		if(tkn->tknID == 1018) {
			newNode->tkn.push_back(tkn);
			tkn = driver();
			
			return newNode;
			
		} else {
			printf("Error on line %i: Expected )\n", tkn->line);
			exit(EXIT_FAILURE);
		}
		
	} else if(tkn->tknID == 1001 && !tkn->keyword) {
		newNode->tkn.push_back(tkn);
		tkn = driver();
		
		return newNode;
		
	} else if(tkn->tknID == 1002) {
		newNode->tkn.push_back(tkn);
		tkn = driver();
		
		return newNode;
		
	} else {
		printf("Error on line %i: Expected (, ID_TKN, NUM_TKN\n", tkn->line);
		exit(EXIT_FAILURE);
	}
}

// <stats> -> <stat> <mStat>
Node *stats() {
	//printf("Entered stats\n");
	// Create new node
	Node *newNode = createNode("<stats>");
	
	newNode->child1 = stat();
	newNode->child2 = mStat();
	
	return newNode;
}

// <mStat> -> empty | <stat> <mStat>
Node *mStat() {
	//printf("Entered mStat\n");
	// Create new node
	Node *newNode = createNode("<mStat>");
	
	if((tkn->tknID == 1001 && tkn->tknWord == "read") || (tkn->tknID == 1001 && tkn->tknWord == "print") || (tkn->tknID == 1001 && tkn->tknWord == "start") || (tkn->tknID == 1001 && tkn->tknWord == "decision") || (tkn->tknID == 1001 && tkn->tknWord == "while") || (tkn->tknID == 1001 && !tkn->keyword)) {
		newNode->child1 = stat();
		newNode->child2 = mStat();
		
		return newNode;
	}
	
	return newNode;
}

//<stat> -> <in> | <out> | <block> | <if> | <loop> | <assign>
Node *stat() {
	//printf("Entered stat\n");
	// Create new node
	Node *newNode = createNode("<stat>");
	
	if(tkn->tknID == 1001 && tkn->tknWord == "read") {
		newNode->child1 = in();
		
		return newNode;
		
	} else if(tkn->tknID == 1001 && tkn->tknWord == "print") {
		newNode->child1 = out();
		
		return newNode;
		
	} else if(tkn->tknID == 1001 && tkn->tknWord == "start") {
		newNode->child1 = block();
		
		return newNode;
		
	} else if(tkn->tknID == 1001 && tkn->tknWord == "decision") {
		newNode->child1 = _if();
		
		return newNode;
		
	} else if(tkn->tknID == 1001 && tkn->tknWord == "while") {
		newNode->child1 = loop();
		
		return newNode;
		
	} else if(tkn->tknID == 1001 && !tkn->keyword) {
		newNode->child1 = assign();
		
		return newNode;
		
	} else {
		printf("Error on line %d: missing expected read, print, start, decision, or while token\n", tkn->line);
		exit(EXIT_FAILURE);
	}
}
	
// <in> -> read >> Identifier ;
Node *in() {
	//printf("Entered in\n");
	// Create new node
	Node *newNode = createNode("<in>");
	
	if(tkn->tknID == 1001 && tkn->tknWord == "read") {
		newNode->tkn.push_back(tkn);
		tkn = driver();
		
		if(tkn->tknID == 1008) {
			newNode->tkn.push_back(tkn);
			tkn = driver();
			
			if(tkn->tknID == 1001 && !tkn->keyword) {
				newNode->tkn.push_back(tkn);
				tkn = driver();
				
				if(tkn->tknID == 1022) {
					newNode->tkn.push_back(tkn);
					tkn = driver();
					
					return newNode;
					
				} else {
					printf("Error on line %d: Expected ;_TKN\n", tkn->line);
					exit(EXIT_FAILURE);
				}
				
			} else {
				printf("Error on line %d: Expected ID_TKN\n", tkn->line);
				exit(EXIT_FAILURE);
			}
			
		} else {
			printf("Error on line %d: Expected >>_TKN\n", tkn->line);
			exit(EXIT_FAILURE);
		}
		
	} else {
		printf("Error on line %d: Expected read_TKN\n", tkn->line);
		exit(EXIT_FAILURE);
	}
}
	
// <out> -> print << <expr>
Node *out() {
	//printf("Entered out\n");
	// Create new node
	Node *newNode = createNode("<out>");
	
	if(tkn->tknID == 1001 && tkn->tknWord == "print") {
		newNode->tkn.push_back(tkn);
		tkn = driver();
		
		if(tkn->tknID == 1006) {
			newNode->tkn.push_back(tkn);
			tkn = driver();
			
			newNode->child1 = expr();
			
			return newNode;
		} else {
			printf("Error on line %d: Expected <<_TKN\n", tkn->line);
			exit(EXIT_FAILURE);
		}
		
	} else {
		printf("Error on line %d: Expectedf print_TKN\n", tkn->line);
		exit(EXIT_FAILURE);
	}
}

// <if> -> decision ( <expr> <RO> <expr> ) <block>
Node *_if() {
	//printf("Entered _if\n");
	// Create new node
	Node *newNode = createNode("<if>");
	
	if(tkn->tknID == 1001 && tkn->tknWord == "decision") {
		newNode->tkn.push_back(tkn);
		tkn = driver();
		
		if(tkn->tknID == 1017) {
			newNode->tkn.push_back(tkn);
			tkn = driver();

			newNode->child1 = expr();
			newNode->child2 = RO();
			newNode->child3 = expr();
			
			if(tkn->tknID == 1018) {
				newNode->tkn.push_back(tkn);
				tkn = driver();
				
				newNode->child4 = block();
				
				return newNode;
				
			} else {
				printf("Error on line %d: Expected )_TKN\n", tkn->line);
				exit(EXIT_FAILURE);
			} 
			
		} else {
			printf("Error on line %d: Expected (_TKN\n", tkn->line);
			exit(EXIT_FAILURE);
		}
		
	} else {
		printf("Error on line %d: Expected decision_TKN\n", tkn->line);
		exit(EXIT_FAILURE);
	}
}

// <loop> -> while ( <expr> <RO> <expr> ) <block>
Node *loop() {
	//printf("Entered loop\n");
	// Create new node
	Node *newNode = createNode("<loop>");
	
	if(tkn->tknID == 1001 & tkn->tknWord == "while") {
		newNode->tkn.push_back(tkn);
		tkn = driver();
		
		if(tkn->tknID == 1017) {
			newNode->tkn.push_back(tkn);
			tkn = driver();

			newNode->child1 = expr();
			newNode->child2 = RO();
			newNode->child3 = expr();
			
			if(tkn->tknID == 1018) {
				newNode->tkn.push_back(tkn);
				tkn = driver();
				
				newNode->child4 = block();
				
				return newNode;
				
			} else {
				printf("Error on line %d: Expected )_TKN\n", tkn->line);
				exit(EXIT_FAILURE);
			} 
			
		} else {
			printf("Error on line %d: Expected (_TKN\n", tkn->line);
			exit(EXIT_FAILURE);
		}
		
	} else {
		printf("Error on line %d: Expected while_TKN\n", tkn->line);
		exit(EXIT_FAILURE);
	}
}

// <assign> -> Identifier = <expr> ;
Node *assign() {
	//printf("Entered assign\n");
	// Create new node
	Node *newNode = createNode("<assign>");
	
	if(tkn->tknID == 1001 && !tkn->keyword) {
		newNode->tkn.push_back(tkn);
		tkn = driver();
		
		if(tkn->tknID == 1003) {
			newNode->tkn.push_back(tkn);
			tkn = driver();
		} else {
			printf("Error on line %d: Expected =_TKN\n", tkn->line);
			exit(EXIT_FAILURE);
		}
		
		newNode->child1 = expr();
		
		if(tkn->tknID == 1022) {
			newNode->tkn.push_back(tkn);
			tkn = driver();
			
			return newNode;
		} else {
			printf("Error on line %d: Expected ;_TKN\n", tkn->line);
			exit(EXIT_FAILURE);
		}
		
	} else {
		printf("Error on line %d: Expected ID_TKN\n", tkn->line);
		exit(EXIT_FAILURE);
	}
}

// <RO> -> << | << = | >> | >> = | == | =!
Node *RO() {		
	//printf("Entered RO\n");
	// Create new node
	Node *newNode = createNode("<RO>");
	
	if(tkn->tknID == 1006) {
		newNode->tkn.push_back(tkn);
		tkn = driver();
		
		if(tkn->tknID == 1003) { 
			newNode->tkn.push_back(tkn);
			tkn = driver();
			
			return newNode;
		}
		
		return newNode;
		
	} else if(tkn->tknID == 1008) {
		newNode->tkn.push_back(tkn);
		tkn = driver();
		
		if(tkn->tknID == 1003) {
			newNode->tkn.push_back(tkn);
			tkn = driver();
			
			return newNode;
		} 
		
		return newNode;
		
	} else if(tkn->tknID == 1004) {
		newNode->tkn.push_back(tkn);
		tkn = driver();
		
		return newNode;

	} else if(tkn->tknID == 1009) {
		newNode->tkn.push_back(tkn);
		tkn = driver();
		
		return newNode;

	} else {
		printf("Error on line %d: Expected << | << = | >> | >> = | == | =! TKN\n", tkn->line);
		exit(EXIT_FAILURE);
	}
}

Node *createNode(string name) {
	Node* newNode = new Node();
	newNode->child1 = NULL;
	newNode->child2 = NULL;
	newNode->child3 = NULL;
	newNode->child4 = NULL;
	newNode->name = name;
	
	return newNode;
}
	
		

	
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		