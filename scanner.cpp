#include "scanner.h"

// Returns size of input file
int getFileSize(string fileName) {
	ifstream file(fileName.c_str(), ifstream::in | ifstream::binary);
	
	file.seekg(0, ios::end);
	int size = file.tellg();
	file.close();
	
	return size;
} 

// Returns the ASCII value of the next character in the array
int getNext(char ch) {
	if(isalpha(ch)) {
		return 1;
	} else if(isdigit(ch)) {
		return 2;
	} else if(ch == '\n') {
		return 24;
	} else if(ch == '\0') {
		return 25;
	} else {
		
		int num = ch;
		switch(num) {
			case 13:
				return 0;
			// WS
			case 32:
				return 24;
			// !
			case 33:
				return 6;
			// #
			case 35:
				return 23;
			// %
			case 37:
				return 13;
			// &
			case 38:
				return 12;
			// (
			case 40:
				return 15;
			// )
			case 41:
				return 16;
			// *
			case 42:
				return 10;
			// +
			case 43:
				return 8;
			// ,
			case 44:
				return 17;
			// -
			case 45:
				return 9;
			// .
			case 46:
				return 14;
			// /	
			case 47:
				return 11;
			// :
			case 58:
				return 7;
			// ;
			case 59:
				return 20;
			// <
			case 60:
				return 4;
			// =
			case 61:
				return 3;
			// >
			case 62:
				return 5;
			// [
			case 91:
				return 21;
			// ]
			case 93:
				return 22;
			// {
			case 123:
				return 18;
			// }
			case 125:
				return 19;
			// If the character was not defined
			default:
				printf("Scanner Error: Character %c does not exist in this language!\n", (char)num);
				exit(EXIT_FAILURE);
		}
	}
}

// Takes input file and converts it into a character array
// and returns the size
int fileToArry(string fileName, char *buff) {
	int size = getFileSize(fileName);
	int indx = 0;
	
	FILE *file = fopen(fileName.c_str(), "r");
	if(file) {
		
		char ch;
		while(true) {

			ch = fgetc(file);
			if(ch == '\n') {
				buff[indx] = ch;
				indx++;
				continue;
				
			} else if(ch == EOF) {
				buff[indx] = '\0';
				break;
				
			} else if(ch == '\t') { 
				size--;
				continue;
				
			} else if(ch == '#') {
				buff[indx] = ch;
				indx++;
				
				do {
					ch = fgetc(file);
					size--;
				} while(ch != ' ');
				
				continue;
				
			} else {	
				buff[indx] = ch;
				indx++;
			}
		}
		
	} else {
		printf("Scanner Error: File Not Opened\n");
		exit(EXIT_FAILURE);
	}
	
	fclose(file);
	buff = (char*)realloc(buff, size);
	
	return size;
}

// The token scanner/driver of the entire program
token *driver() {
	int state = DFA[0][0];
	int nextState;
	int nextChar = getNext(buffer[index]);
	
	string word;
	token *tkn;
	
	while (state <= FINAL) { 
		nextChar = getNext(buffer[index]);
		nextState = DFA[state][nextChar];
		
		if (nextState == ERROR) {
			printf("Scanner Error: Driver returned an error!\n");
			exit(EXIT_FAILURE);
		}
		
		// If driver returns a final state
		if (nextState >= FINAL) {
			if (nextState == 1001) {
				tkn = createToken(nextState, lineNum, word);
					
				tokenCount++;
				return tkn;
				
			} else if (nextState == 1002) {
				tkn = createToken(nextState, lineNum, word);
				
				tokenCount++;
				return tkn;
				
			} else {
				if(buffer[index] != ' ' && buffer[index] != '\n' && buffer[index] != '\0') {
						word.push_back(buffer[index]);
				}
				
				if(nextState == 1004 || nextState == 1006 || nextState == 1008 || nextState == 1009 || nextState >= 1010) { 
					index++;
				}
				
				tkn = createToken(nextState, lineNum, word);
				tokenCount++;
				return tkn;
			}
		
		// If the driver returns a non-final state
		} else {
			state = nextState;
			
			if(buffer[index] != '\n' && buffer[index] != ' ') {
				word.push_back(buffer[index]);
			}		
		} 
		
		if(buffer[index] == '\n') {
			lineNum++;
		}
		
		index++;
	}
}

// Check if the passed string is a keyword defined by the language
int checkKeyword(string word) {
	for(int c = 0; c < 10; c++) {
		if(!(word.compare(keywords[c]))) {
			return 1;
		}
	}
	return 0;
}

// Returns keywords that were defined by the language
string getKeyword(int val) {
	char *keyword;
	string word;
	
	switch(val) {
		case 0:
			keyword = (char*)keywords[val];
			break;
		case 1:
			keyword = (char*)keywords[val];
			break;
		case 2:
			keyword = (char*)keywords[val];
			break;
		case 3:
			keyword = (char*)keywords[val];
			break;
		case 4:
			keyword = (char*)keywords[val];
			break;
		case 5:
			keyword = (char*)keywords[val];
			break;
		case 6:
			keyword = (char*)keywords[val];
			break;
		case 7:
			keyword = (char*)keywords[val];
			break;
		case 8:
			keyword = (char*)keywords[val];
			break;
		case 9:
			keyword = (char*)keywords[val];
			break;
	}
	
	word = string(keyword);
	return word;
}

// Creates tokens when the driver returns a final state
token *createToken(int ID, int line, string tknWord) {
	token *toReturn = new token;
	
	toReturn->tknID = ID;
	toReturn->line = line;
		
	if(ID == 1100) {
		toReturn->tknName = "EOF_TKN";
		toReturn->tknWord = "EOF";
		return toReturn;
	} else if(ID == 1001) {
		toReturn->tknName = "ID_TKN";
	} else if(ID == 1002) {
		toReturn->tknName = "NUM_TKN";
	} else if(ID == 1003) {
		toReturn->tknName = "=_TKN";
	} else if(ID == 1004) {
		toReturn->tknName = "==_TKN";
	} else if(ID == 1005) {
		toReturn->tknName = "<_TKN";
	} else if(ID == 1006) {
		toReturn->tknName = "<<_TKN";
	} else if(ID == 1007) {
		toReturn->tknName = ">_TKN";
	} else if(ID == 1008) {
		toReturn->tknName = ">>_TKN";
	} else if(ID == 1009) {
		toReturn->tknName = "=!_TKN";
	} else if(ID == 1010) {
		toReturn->tknName = ":_TKN";
	} else if(ID == 1011) {
		toReturn->tknName = "+_TKN";
	} else if(ID == 1012) {
		toReturn->tknName = "-_TKN";
	} else if(ID == 1013) {
		toReturn->tknName = "*_TKN";
	} else if(ID == 1014) {
		toReturn->tknName = "&_TKN";
	} else if(ID == 1015) {
		toReturn->tknName = "%_TKN";
	} else if(ID == 1016) {
		toReturn->tknName = "._TKN";
	} else if(ID == 1017) {
		toReturn->tknName = "(_TKN";
	} else if(ID == 1018) {
		toReturn->tknName = ")_TKN";
	} else if(ID == 1019) {
		toReturn->tknName = ",_TKN";
	} else if(ID == 1020) {
		toReturn->tknName = "{_TKN";
	} else if(ID == 1021) {
		toReturn->tknName = "}_TKN";
	} else if(ID == 1022) {
		toReturn->tknName = ";_TKN";
	} else if(ID == 1023) {
		toReturn->tknName = "[_TKN";
	} else if(ID == 1024) {
		toReturn->tknName = "]_TKN";
	} else if(ID == 1025) {
		toReturn->tknName = "[_TKN";
	} else if(ID == 1026) {
		toReturn->tknName = "/_TKN";
	} else {
		printf("Error: Found token that does not exist with ID: %i\n", ID);
		exit(EXIT_FAILURE);
	}
		
	if(checkKeyword(tknWord)) {
		toReturn->tknWord = tknWord;
		toReturn->keyword = 1;
	} else {
		toReturn->tknWord = tknWord;
		toReturn->keyword = 0;
	}
	
	//printToken(toReturn);
	return toReturn;
}

// Creates tokens when the driver returns a final state
token *createToken(int ID, int line, string tknName, string tknWord) {
	token *toReturn = new token;
	
	toReturn->tknID = ID;
	toReturn->line = line;
	toReturn->tknName = tknName;
	toReturn->tknWord = tknWord;
	
	//printToken(toReturn);
	return toReturn;
}

// Prints a token
void printToken(token *tkn) {
	if(tkn->tknWord == "NULL") {
		printf("Type: %s - ID: %d - Line %d\n", tkn->tknName.c_str(), tkn->tknID, tkn->line);
	} else {
	printf("Type: %s - ID: %d - Line %d - Word: %s\n", tkn->tknName.c_str(), tkn->tknID, tkn->line, tkn->tknWord.c_str());
	}
}
		
