#include "p1.h"
#include <stdbool.h>
#include <string.h>

/*
 * creates a new operation expression node
 *
 * @param c - operation character
 * @param el - true if open paren, false if not
 */
Exp* newExpOperation(char c, bool el) { 
    Exp* e = malloc(5 * sizeof(struct Exp));
    e->first = NULL;
    e->rest = NULL;
    e->symbol = c;
    e->previous = NULL;
    e->num = -1;
    e->empty_left = el;
    return e;
}

/*
 * creates a new integer expression node
 *
 * @param c[] - string of the integer
 */
Exp* newExpInt(char c[]) {
    Exp* e = malloc(5 * sizeof(struct Exp));
    e->first = NULL;
    e->rest = NULL;
    e->symbol = '\0';
    e->num = atoi(c);
    e->previous = NULL;
    return e;
}

/*
 * checks to see if character is an operation
 *
 * @param c - character that is being checked
 */
bool isOperation(char c) {
    return (c == '+' || c == '-' || c == '*');
}

/*
 * checks to see if character is an open paren
 *
 * @param c - character that is being checked
 */
bool isOpenParen(char c) {
    return (c == '(');
}

/*
 * checks to see if character is an closed paren
 *
 * @param c - character that is being checked
 */
bool isClosedParen(char c) {
    return (c == ')');
}

/*
 * adds a new level to the data structure, one first node (left) and one rest node (right)
 *
 * @param current - current node on the data structure, location where new level should be added
 * @param left - node to be added on the left side
 */
void addLevel(Exp* current, Exp* left) {
    current->first = left;
    current->rest = newExpOperation('\0', false);
    current->previous = curr;
    curr = current->rest;
}

/*
 * adds a new level to the data structure, one first node (left) and one rest node (right), specifically for an open paren
 *
 * @param current - current node on the data structure, location where new level should be added
 */
void addLevelParen(Exp* current) {
    current->first = newExpOperation('\0', true);
    current->rest = newExpOperation('\0', false);
    curr = current->first;
}

/*
 * moves current node to the end of the paren
 *
 * @param current - current node on the data structure
 */
Exp* moveCurrEndParen(Exp* current) {
    while(current != current->previous->first) {
        current = current->previous;
    }
    return current->previous;
}

/*
 * reads input from the command line
 */
void readInput() {
    int n = 4;
    char command[n];
    printf("Enter a command \n");
    fgets(command, n, stdin);
    if(command[0] == 'c') {
        root = newExpOperation('\0', true); //makes a new root node to start the data structure
        curr = root;
        root = readExpression();
    }
    else if(command[0] == 'p') {
        printOutput(root);
    }
    else if(command[0] == 'e') {
        int ans = solve(root);
        printf("%d \n", ans);
        readInput();
    }
    else if(command[0] == 'a') {
        readAppend();
    }
    else if(command[0] == 's') {
        readSubset();
    }
    else {
        printf("ERROR \n");
    }
}

/*
 * evaluates input from command line
 *
 * @param c[] - string of inputted expression
 */
void evaluateInput(char c[]) {
    for(int i = 1; i < strlen(c); i++) {
        if(isOperation(c[i])) { //if operation
            Exp* temp = newExpOperation(c[i], false);
            addLevel(curr, temp);
        }
        else if(isOpenParen(c[i]) && i != 0) { //if open pare
            addLevelParen(curr);
        }
        else if(isClosedParen(c[i])) { //if close paren
            curr = moveCurrEndParen(curr);
        }
        else if(c[i] == ' ') { // if space
            continue;
        }
        else if(i == strlen(c) - 1) { // if out of bounds
            continue;
        }
        else { //integer
            char n[64];
            n[0] = c[i];
            i++;
            int count = 1;
            while(i < strlen(c) - 1 && c[i] != ' ') { //finds multi digit integers
                n[count] = c[i];
                count++;
                i++;
            }
            Exp* temp = newExpInt(n);
            addLevel(curr, temp);
        }
    }
    curr = root;
}

/*
 * traverses through the data structure and prints it out
 *
 * @param e - starting node
 */
void traversal(Exp* e) { //helps with printout and solving tree
    if(e == NULL) {
        return;
    }
    if(e->first && e->first->empty_left) { // finds paren 
        printf("(");
        traversal(e->first);
        printf(")");
        
    }
    if(e->first && e->first->num != -1) { // tests to see if its a number
        printf("%d ", e->first->num);
        traversal(e->rest);
    }
    if(e->first && e->first->symbol != '\0') { //tests to see if its an operation
        printf("%c ", e->first->symbol);
        traversal(e->rest);
    }
}

/*
 * Reads in expression from command line
 */
Exp* readExpression() {
    int n = 64;
    char command[n];
    printf("Enter an expression \n");
    fgets(command, n, stdin);
    evaluateInput(command);
    readInput();
    return curr;
}

/*
 * prints out a subset of the expression
 *
 * @param c[] - subset inputted from command line
 */
void printSubset(char c[]) {
    Exp* current = root;
    for(int i = 1; i < strlen(c); i++) {
        if(c[i] == 'f') {
            current = curr->first;
        }
        else if(c[i] == 'r') {
            current = curr->rest;
        }
    }
    printOutput(current);
    free(current);
}

/*
 * reads in subset from command line
 */
void readSubset() {
    int n = 64;
    char command[n];
    printf("Enter a subset \n");
    fgets(command, n, stdin);
    printSubset(command);
}

/*
 * prints out expression
 *
 * @param curr - starting node
 */
void printOutput(Exp* curr) {
    printf("Expression: ( ");    
    traversal(curr);
    printf(") \n");
    readInput();
}

/*
 * reads the append command from command line
 */
void readAppend() {
    int n = 64;
    char command[n];
    printf("Enter expression to append: \n");
    fgets(command, n, stdin);
    Exp* e = newExpInt(command);
    appendExp(e);
    readInput();
}

/*
 * adds an expression to current expression
 *
 * @param add - node to be added
 */
void appendExp(Exp* add) {
    Exp* curr = root;
    while(curr->rest) { //moves curr all the way to the right
        curr = curr->rest;
    }
    addLevel(curr, add);
}

/*
 * solves the expression
 *
 * @param e - starting node
 */
int solve(Exp* e) {
	int ans = 0;
    int count = 0; //used for -, * operations
    
    if(e->first && e->first->symbol != '\0') { //checks char
        char op = e->first->symbol; //current operation
        Exp* currentNode = e;
        while(currentNode->rest) {
            currentNode = currentNode->rest;
            if(e->first && e->first->empty_left) { //checks if open paren
                int curr_ans = solve(e->first);
                if(op == '+') {
                    printf("PAREN +");
                    ans += curr_ans;
                }
                else if(op == '-') {
                    printf("PAREN -");
                    if(ans == 0 && count == 0) {
                        ans += curr_ans;
                    }
                    else {
                        ans -= curr_ans;
                    }
                }
                else if(op == '*') {
                    printf("PAREN *");
                    if(ans == 0 && count == 0) {
                        ans += curr_ans;
                    }
                    else {
                        ans *= curr_ans;
                    }
                }
                count++;
            }
            else {
                if(currentNode->first) { //checs if there is a first node
                    if(op == '+') {
                        ans += currentNode->first->num;
                    }
                    else if(op == '-') {
                        if(ans == 0 && count == 0) {
                            ans += currentNode->first->num;
                        }
                        else {
                            ans -= currentNode->first->num;
                        }
                    }
                    else if(op == '*') {
                        if(ans == 0 && count == 0) {
                            ans += currentNode->first->num;
                        }
                        else {
                            ans *= currentNode->first->num;
                        }
                    }
                    count++;
                }
            }
        }
        free(e);
        return ans;
    }
}