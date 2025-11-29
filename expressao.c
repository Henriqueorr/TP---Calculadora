#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "expressao.h"

#define MAX 512
#define PI 3.14159265358979323846

// Pilha para caracteres
typedef struct {
    char data[MAX];
    int top;
} CharStack;

// Pilha para floats
typedef struct {
    float data[MAX];
    int top;
} FloatStack;

// Pilha para strings
typedef struct {
    char *data[MAX];
    int top;
} StringStack;

// Funções auxiliares para CharStack
void initCharStack(CharStack *s) { s->top = -1; }
int isCharEmpty(CharStack *s) { return s->top == -1; }
int isCharFull(CharStack *s) { return s->top == MAX - 1; }
void pushChar(CharStack *s, char c) { if (!isCharFull(s)) s->data[++s->top] = c; }
char popChar(CharStack *s) { return isCharEmpty(s) ? '\0' : s->data[s->top--]; }
char peekChar(CharStack *s) { return isCharEmpty(s) ? '\0' : s->data[s->top]; }

// Funções auxiliares para FloatStack
void initFloatStack(FloatStack *s) { s->top = -1; }
int isFloatEmpty(FloatStack *s) { return s->top == -1; }
int isFloatFull(FloatStack *s) { return s->top == MAX - 1; }
void pushFloat(FloatStack *s, float f) { if (!isFloatFull(s)) s->data[++s->top] = f; }
float popFloat(FloatStack *s) { return isFloatEmpty(s) ? 0 : s->data[s->top--]; }

// Funções auxiliares para StringStack
void initStringStack(StringStack *s) { s->top = -1; }
int isStringEmpty(StringStack *s) { return s->top == -1; }
int isStringFull(StringStack *s) { return s->top == MAX - 1; }
void pushString(StringStack *s, char *str) { 
    if (!isStringFull(s)) s->data[++s->top] = strdup(str); 
}
char* popString(StringStack *s) { 
    return isStringEmpty(s) ? NULL : s->data[s->top--]; 
}

// Verifica se é operador
int isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^';
}

// Retorna precedência do operador
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/' || op == '%') return 2;
    if (op == '^') return 3;
    return 0;
}

// Verifica se é função matemática
int isFunction(char *token) {
    return strcmp(token, "sen") == 0 || strcmp(token, "cos") == 0 || 
           strcmp(token, "tg") == 0 || strcmp(token, "log") == 0 || 
           strcmp(token, "raiz") == 0;
}

// Aplica operação binária
float applyOp(float a, float b, char op) {
    switch(op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return b != 0 ? a / b : 0;
        case '%': return fmod(a, b);
        case '^': return pow(a, b);
    }
    return 0;
}

// Aplica função matemática
float applyFunction(char *func, float value) {
    if (strcmp(func, "sen") == 0) return sin(value * PI / 180.0);
    if (strcmp(func, "cos") == 0) return cos(value * PI / 180.0);
    if (strcmp(func, "tg") == 0) return tan(value * PI / 180.0);
    if (strcmp(func, "log") == 0) return log10(value);
    if (strcmp(func, "raiz") == 0) return sqrt(value);
    return 0;
}

// Calcula valor da expressão pós-fixa
float getValorPosFixa(char *StrPosFixa) {
    FloatStack stack;
    initFloatStack(&stack);
    
    char *str = strdup(StrPosFixa);
    char *token = strtok(str, " ");
    
    while (token != NULL) {
        if (isFunction(token)) {
            if (isFloatEmpty(&stack)) { free(str); return 0; }
            float val = popFloat(&stack);
            pushFloat(&stack, applyFunction(token, val));
        }
        else if (isOperator(token[0]) && strlen(token) == 1) {
            if (stack.top < 1) { free(str); return 0; }
            float b = popFloat(&stack);
            float a = popFloat(&stack);
            pushFloat(&stack, applyOp(a, b, token[0]));
        }
        else {
            pushFloat(&stack, atof(token));
        }
        token = strtok(NULL, " ");
    }
    
    float result = popFloat(&stack);
    free(str);
    return result;
}

// Converte pós-fixa para infixa
char* getFormaInFixa(char *Str) {
    StringStack stack;
    initStringStack(&stack);
    
    char *str = strdup(Str);
    char *token = strtok(str, " ");
    char buffer[MAX];
    
    while (token != NULL) {
        if (isFunction(token)) {
            if (isStringEmpty(&stack)) { 
                free(str); 
                return NULL; 
            }
            char *operand = popString(&stack);
            snprintf(buffer, MAX, "%s(%s)", token, operand);
            free(operand);
            pushString(&stack, buffer);
        }
        else if (isOperator(token[0]) && strlen(token) == 1) {
            if (stack.top < 1) { 
                free(str); 
                return NULL; 
            }
            char *right = popString(&stack);
            char *left = popString(&stack);
            snprintf(buffer, MAX, "(%s%c%s)", left, token[0], right);
            free(right);
            free(left);
            pushString(&stack, buffer);
        }
        else {
            pushString(&stack, token);
        }
        token = strtok(NULL, " ");
    }
    
    if (stack.top != 0) {
        free(str);
        return NULL;
    }
    
    char *result = strdup(stack.data[0]);
    free(stack.data[0]);
    free(str);
    return result;
}