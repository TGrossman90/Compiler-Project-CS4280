#include "scanner.h"
#include "node.h"
#include "parser.h"
#include "semantics.h"
#include "treePrint.h"

char *buffer = new char[1000000];
int index = 0;
int lineNum = 1;
int tokenCount = 0;

int main(int argc, char* argv[]) {

	// Checks if a file is being redirected or arguments being passed
	if (isatty(STDIN_FILENO)) {
		
		// If no arguments, return error
		if(argc == 1) {
			printf("Scanner Error: You must pass/redirect a file to the scanner!\n");
			exit(EXIT_FAILURE);

		// If there is one argument passed to the project...			
		} else if(argc == 2) {
			
			// To deal with implicit file names, I've implemented a way to open a file without an extension
			// and without limiting extensions.
			// To start, create a command line argument in the form of
			// ls argv[1]*
			string command;
			command = "ls ";
			command.append(argv[1]);
			command.append("*");
			
			// Run the command with the popen() function which returns the outstream of the system command
			int BUFFSIZ = 150;
			char buff[BUFSIZ];
			FILE *fp = popen(command.c_str(), "r");
			while (fgets( buff, BUFSIZ, fp) != NULL );
			pclose(fp);
			
			// Figure out how many characters are in the filename + extension
			int len = 0;
			for(int c = 0; c < BUFFSIZ; c++) {
				if((isalnum(buff[c])))
					len++;
			}
			len++;
			
			// Change the filename from a char array into a string 
			string name;
			for(int c = 0; c < len; c++)
				name += buff[c];
			
			// Take input file and store it into a character array
			int size = fileToArry(name, buffer);
			
			// This is the for-loop that repeatedly calls the 
			// scanner until no tokens are left to find
			
			//token *tkns[1000000];
			//int c = 0;
			//for(index; index < size; index++) {
			//	token *tkn = driver();
			//	printToken(tkn);
			//	free(tkn);
			//}
			Node *root;
			root = parser();
			
			printTree(root, 0); 
			
			semantics(root);
			
			printf("Finished\n");
			
		// If too many arguments were passed to the program
		} else if(argc > 2) {
			printf("Scanner Error: You've entered too many arguments. Please try again.\n");
		}
		
	// If the program was run with a file being redirected to it
	} else {
		
		// Capture redirection
		string userInput;
		ofstream tempFile;
		string name = "temp.txt";
		remove("temp.txt");
		
		while(getline(cin, userInput)) {
			
			// Put redirected file into a temp file
			tempFile.open(name.c_str(), ios::app);
			tempFile << userInput << "\n";
			tempFile.close();
		}
		
		// Take input file and store it into a character array
		int size = fileToArry(name, buffer);
		
		// This is the for-loop that repeatedly calls the 
		// scanner until no tokens are left to find
		//token *tkns[1000000];
		//	int c = 0;
		//	for(index; index < size; index++) {
		//		token *tkn = driver();
		//		printToken(tkn);
		//		free(tkn);
		//	}
		
		Node *root;
		root = parser();
			
		printTree(root, 0);
		semantics(root);
			printf("Finished\n");

		
		// Remove the temporary file that was used to store redirected file
		remove("temp.txt");
	}
	
	return 0;
}
