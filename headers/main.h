#ifndef BASICINTERPRETATOR_MAIN_H
#define BASICINTERPRETATOR_MAIN_H

void printError(char *error);
void printErrorLine(char *error, int line);
int loadProgram(char* fileName, char* program), countFileChars(char *fileName);

#endif
