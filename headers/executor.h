#ifndef BASICINTERPRETATOR_EXECUTOR_H
#define BASICINTERPRETATOR_EXECUTOR_H

struct lexem {
    char *name;
    int type;
    int id;
    char *pointToProgram;
    int line;
};
struct label {
    char *name; //Имя метки
    char *location; //Указатель на место размещения в программе
};
struct infoLabels {
    int countLabels;
    struct label *labels;
};
struct variable {
    char *name;
    int value;
};
struct infoVariables {
    int count_vars;
    struct variable *vars;
};
struct Gosub {
    char *location;
};
struct infoGosub {
    struct Gosub *gStackStart; //Стек подпрограмм GOSUB
    struct Gosub *gStackEnd;
    int gIndex; //Индекс верхней части стека  gPush +1 . gPop -1
};

void getToken(struct lexem *token);

void start(char *program, struct infoLabels *infoLabels, struct infoVariables *infoVars, char *fileResult);

struct variable *findVariable(struct infoVariables *infoVars, char *name);

struct variable *addVariable(struct infoVariables *infoVars, char *name);

void comeBack(struct lexem *token);

char *mallocAndCopy(char *source, int steps);

int isWhite(char c), isDelim(char c), getIntCommand(char *command);


void assignment(struct lexem *token, struct infoVariables *infoVars);

void getExpression(int *result, struct lexem *token, struct infoVariables *infoVars);

void level2(int *result, struct lexem *token, struct infoVariables *infoVars);

void level3(int *result, struct lexem *token, struct infoVariables *infoVars);

void level4(int *result, struct lexem *token, struct infoVariables *infoVars);


void value(int *result, struct lexem *token, struct infoVariables *infoVars);

void unary(char o, int *r);

void arithmetic(char o, int *r, int *h);

void basicPrint(struct lexem *token, struct infoVariables *infoVars, char *fileResult);

void basicInput(struct lexem *token, struct infoVariables *infoVars);

void basicGoto(struct lexem *token, struct infoLabels *infoLabels);

void basicGosub(struct infoGosub *infoGosub, struct lexem *token, struct infoLabels *infoLabels);

void basicReturn(struct infoGosub *infoGosub, struct lexem *token);

void basicIf(struct lexem *token, struct infoLabels *infoLabels, struct infoVariables *infoVars, struct infoGosub *infoGosub, char *fileResult);

char *findLabel(struct infoLabels *infoLabels, char *label);

void gPush(struct infoGosub *infoGosub, char *pointer);

char *gPop(struct infoGosub *infoGosub);


#endif
