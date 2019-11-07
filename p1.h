#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#ifndef P1_H_
#define P1_H_

typedef struct Exp {
    char symbol;
    int num;
    struct Exp* previous;
    struct Exp* first;
    struct Exp* rest;
    bool empty_left;
} Exp;

Exp* curr;
Exp* root;

extern Exp* newExpOperation(char c, bool el);
extern Exp* newExpInt(char c[]);
extern bool isOperation(char c);
extern bool isOpenParen(char c);
extern bool isClosedParen(char c);
extern void addLevel(Exp* current, Exp* left);
extern void addLevelParen(Exp* current);
extern Exp* moveCurrEndParen(Exp* current);
extern void readInput();
extern void evaluateInput(char c[]);
extern void traversal(Exp* e);
extern Exp* readExpression();
extern void printSubset(char c[]);
extern void readSubset();
extern void printOutput(Exp* curr);
extern void readAppend();
extern void appendExp(Exp* add);
extern int solve(Exp* e);

#endif