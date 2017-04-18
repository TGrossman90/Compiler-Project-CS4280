#ifndef SCANNER_H
#define SCANNER_H

#define FINAL 1000
#define ERROR -1

#include <iostream>
#include <unistd.h>
#include <fstream>
#include "token.h"

using namespace std;

extern char *buffer;
extern int index;
extern int lineNum;
extern int tokenCount;
extern string name;
extern token *tkn;

int checkKeyword(string);
int fileToArry(string, char *);
int getFileSize(string);
int getNext(char);

string getKeyword(int);

token *driver();
token *createToken(int, int, string);
token *createToken(int, int);

void getLine();
void printToken(token *);

#endif