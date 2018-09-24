#ifndef BASICINTERPRETATOR_ANALYZER_H
#define BASICINTERPRETATOR_ANALYZER_H

#include "executor.h"

struct resultByAnalyz {
    struct infoLabels infoLabels;
    struct infoVariables infoVars;
};

struct resultByAnalyz analyzing(char *point_to_program);

void putLabel(struct infoLabels *infoLabels, struct lexem *token);

void analyzingAssignment(struct lexem *token, struct infoVariables *info_vars);

void analyzingInput(struct lexem *token, struct infoVariables *info_vars);

void analyzingPrint(struct lexem *token, struct infoVariables *info_vars);

void analyzingIf(struct lexem *token, struct infoVariables *info_vars);

void analyzingCondition(struct lexem *token, struct infoVariables *info_vars);

void analyzingGoto(struct lexem *token);

void analyzingGosub(struct lexem *token);

#endif
