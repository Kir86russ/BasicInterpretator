#include <mem.h>
#include <stdlib.h>

#include "headers/constants.h"
#include "headers/analyzer.h"
#include "headers/main.h"
#include "headers/executor.h"

struct resultByAnalyz analyzing(char *program) {
    struct resultByAnalyz *result = (struct resultByAnalyz *) malloc(sizeof(struct resultByAnalyz));
    if (result == NULL) printError("Error allocating memory");

    result->infoVars.count_vars = 0;
    result->infoVars.vars = NULL;
    result->infoLabels.countLabels = 0;
    result->infoLabels.labels = NULL;

    struct lexem *token = (struct lexem *) malloc(sizeof(struct lexem));
    if (token == NULL) printError("Error allocating memory");

    token->pointToProgram = program;
    token->line = 1;
    int readEnd = 0;
    int countIf = 0;
    int countFi = 0;
    do {
        getToken(token);

        if (token->type == NUMBER) {
            putLabel(&result->infoLabels, token);
        }

        if (token->type == VARIABLE) {
            analyzingAssignment(token, &result->infoVars);
        }

        if (token->type == COMMAND) {
            switch (token->id) {
                case INPUT:
                    analyzingInput(token, &result->infoVars);
                    break;
                case PRINT:
                    analyzingPrint(token, &result->infoVars);
                    break;
                case IF:
                    countIf++;
                    analyzingIf(token, &result->infoVars);
                    break;
                case GOTO:
                    analyzingGoto(token);
                    break;
                case GOSUB:
                    analyzingGosub(token);
                    break;
                case END:
                    readEnd = 1;
                    break;
                case ENDFI:
                    countFi++;
                    break;
                default:
                    break;
            }
        }

    } while (token->id != FINISHED);
    if (!readEnd)
        printError("Wait END");
    if (countFi != countIf)
        printError("Wait ENDFI");
    free(token);
    return *result;
}

void analyzingAssignment(struct lexem *token, struct infoVariables *info_vars) {

    int countChar = 0;
    char *name_var = token->name;

    while (*name_var != '\0'){
        name_var++;
        countChar++;
    }


    name_var = mallocAndCopy(token->name, countChar);
    getToken(token); // Считываем знак равно
    if (strcmp(token->name, "=")) {
        if (token->id == EOL)
            printErrorLine("Syntax error! Not find '=' after variable", token->line - 1);
        else printErrorLine("Syntax error! Not find '=' after variable", token->line);
    }
    getToken(token);
    if (token->id == EOL)
        printErrorLine("Syntax error! After '=' stay on it line", token->line - 1);
    else
        comeBack(token);
    int count = 0; // Для скобок
    do {
        getToken(token);
        if (token->type == VARIABLE) {
            struct variable *var = findVariable(info_vars, token->name);
            if (var == NULL)
                printErrorLine("Variable not init!", token->line);
        }
            // Проверка на присвоение выражения в скобках↓
        if (!strcmp(token->name, ")") && count == 0)
            printError("Syntax error! Fix bracket");
        if (!strcmp(token->name, "("))
            count++;
        if (!strcmp(token->name, ")"))
            count--;
    } while (token->id != EOL); // Смотрим чтобы скобки закрывались на одной строке
    if (count != 0)
        printError("Syntax error! Check using brackets");
    addVariable(info_vars, name_var);
}

void putLabel(struct infoLabels *infoLabels, struct lexem *token) {

    int countChar = 0;
    char *name_label = token->name;

    while (*name_label != '\0'){
        name_label++;
        countChar++;
    }

    name_label = mallocAndCopy(token->name, countChar);

    infoLabels->countLabels++;
    infoLabels->labels = (struct label *) realloc(infoLabels->labels, sizeof(struct label) * infoLabels->countLabels);
    struct label *temp = infoLabels->labels;

    int i = 1;
    while (i < infoLabels->countLabels) {
        if (!strcmp(temp->name, token->name)) {
            printError("Duplicate labels");
        }
        temp++;
        i++;
    }
    temp->name = name_label;
    temp->location = token->pointToProgram;
}

void analyzingInput(struct lexem *token, struct infoVariables *info_vars) {
    getToken(token);
    if (token->id == EOL)
        printErrorLine("Syntax error! You must set variable in current string!", token->line - 1);

    if (token->type == QUOTE) { // будет текст в кавычках, а затем ввод с клавиатуры
        getToken(token);
        if (strcmp(token->name, ",")) // если после кавычек не запятая
            printErrorLine("Syntax error! You must use comma and variable for INPUT", token->line - 1); // нет запятой
        if (!strcmp(token->name, ",")) {
            getToken(token);
            if (token->id == EOL) // после запятой EOL
                printErrorLine("Syntax error! You must set variable after comma", token->line - 1);// не указана переменная для ввода с клавиатуры
        }
    }
    if (token->type == VARIABLE) {
        int countChar = 0;
        char *name_var = token->name;

        while (*name_var != '\0'){
            name_var++;
            countChar++;
        }

        name_var = mallocAndCopy(token->name, countChar);
        addVariable(info_vars, name_var);
        getToken(token);
        if (token->id != EOL)
            printErrorLine("Syntax error! You must write with new string!", token->line);
    }
    if (token->type != QUOTE && token->type != VARIABLE && token->id != EOL)
        printErrorLine("Syntax error! Enter only variable or text with variable", token->line);
}

void analyzingPrint(struct lexem *token, struct infoVariables *info_vars) {
    getToken(token);
    if (token->id == EOL)
        printErrorLine("Syntax error! You must write in current string!", token->line - 1);
    else
        comeBack(token);
    do {
        getToken(token);
        if (token->type == QUOTE) {
            getToken(token);
            if (!strcmp(token->name, ",")) { // если после текста в кавычках стоит запятая для переменной , но нет переменной
                getToken(token);
                if (token->id == EOL)
                    printErrorLine("Syntax error! Enter data after comma", token->line - 1);
            } else {
                if (token->id != EOL)  //если нет запятой , и нет EOL
                printErrorLine("Syntax error! You must make end of line after your text", token->line);
            }
        }
        if (token->type == VARIABLE) {
            struct variable *var = findVariable(info_vars, token->name);
            if (var == NULL)
                printErrorLine("Variable not init", token->line);
            getToken(token);
            if (!strcmp(token->name, ",")) {
                getToken(token);
                if (token->id == EOL)
                    printErrorLine("Syntax error! Enter data after comma", token->line - 1); // если после запятой EOL
            } else {
                if (token->id != EOL)
                    printErrorLine("Syntax error! You must make end of line after your text", token->line);
            }
        }
        if (token->type == NUMBER || *token->name == '(') {
            int answer = 0;
            getExpression(&answer, token, info_vars);
            getToken(token);
            if (!strcmp(token->name, ",")) {
                getToken(token);
                if (token->id == EOL)
                    printErrorLine("Syntax error! Enter data after comma", token->line - 1);
            } else {
                if (token->id != EOL)
                    printErrorLine("Syntax error! You must make end of line after your number", token->line);
            }
        }
        comeBack(token);
        if (token->type != NUMBER && token->type != VARIABLE && token->type != QUOTE && token->id != EOL && *token->name != '(')
            printErrorLine("Syntax error! Enter only variable or text with variable", token->line);
    } while (token->id != EOL);
}

void analyzingIf(struct lexem *token, struct infoVariables *info_vars) {
    analyzingCondition(token, info_vars);
}

void analyzingCondition(struct lexem *token, struct infoVariables *info_vars) {
    int count = 0;
    do {
        getToken(token);
        if (token->type == VARIABLE) {
            struct variable *var = findVariable(info_vars, token->name);
            if (var == NULL)
                printErrorLine("Variable not init!", token->line);
        }
        if ((!strcmp(token->name, ">") || !strcmp(token->name, "<") || !strcmp(token->name, "=")) && count != 0)  // ((() >   ()
            printErrorLine("Syntax error! Fix brackets", token->line);
        if (!strcmp(token->name, ")") && count == 0) // Четное кол-во скобок должно быть
            printErrorLine("Syntax error! Correcting brackets", token->line);
        if (!strcmp(token->name, "("))
            count++;
        if (!strcmp(token->name, ")"))
            count--;
        if (token->id == EOL)
            printErrorLine("Syntax error! No way end of line, while not THEN", token->line - 1);
    } while (token->id != THEN);
    if (count != 0) printErrorLine("Syntax error! Fix brackets", token->line);
    getToken(token);
    if (token->id != EOL)
        printErrorLine("Syntax error! You must write with next string, after THEN", token->line);
}

void analyzingGoto(struct lexem *token) {
    getToken(token);
    if (token->id == EOL)
        printErrorLine("Syntax error! GOTO -> (number)", token->line - 1);
    if (token->type != NUMBER)
        printErrorLine("Syntax error! GOTO -> (number)", token->line);
    getToken(token);
    if (token->id != EOL)
        printErrorLine("Syntax error! Need space, after GOTO -> (number)", token->line);
}

void analyzingGosub(struct lexem *token) {
    getToken(token);
    if (token->id == EOL)
        printErrorLine("Syntax error! GOSUB -> (number)", token->line - 1);
    if (token->type != NUMBER)
        printErrorLine("Syntax error! GOSUB -> (number)!", token->line);
    getToken(token);
    if (token->id != EOL)
        printErrorLine("Syntax error! Need space, after GOSUB -> (number)", token->line);
}



