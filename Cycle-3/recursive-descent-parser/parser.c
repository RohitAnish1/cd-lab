#include <stdio.h>
#include <ctype.h>
#include <string.h>

char input[100];
int i = 0; // pointer to current character

// Function declarations
void E();
void Eprime();
void T();
void Tprime();
void F();
void match(char expected);
void error();

//------------------ MATCH FUNCTION ------------------
void match(char expected) {
    if (input[i] == expected) {
        
        i++; // consume the character
    } else {
        error();
    }
}

//------------------ ERROR FUNCTION ------------------
void error() {
    printf("❌ Syntax Error at position %d near '%c'\n", i, input[i]);
}

//------------------ GRAMMAR FUNCTIONS ------------------
void E() {
    T();
    Eprime();
}

void Eprime() {
    if (input[i] == '+') {
        match('+');
        T();
        Eprime();
    }
    // else epsilon — do nothing
}

void T() {
    F();
    Tprime();
}

void Tprime() {
    if (input[i] == '*') {
        match('*');
        F();
        Tprime();
    }
    // else epsilon — do nothing
}

void F() {
    if (input[i] == '(') {
        match('(');
        E();
        match(')');
    } else if (isalpha(input[i])) {
        match(input[i]); // match an identifier (a, b, c, ...)
    } else {
        error();
    }
}

//------------------ MAIN FUNCTION ------------------
int main() {
    printf("Enter an expression: ");
    scanf("%s", input);

    E(); // Start parsing from E

    // If entire input is parsed successfully
    if (input[i] == '\0')
        printf("✅ Parsing successful! The expression is valid.\n");
    else
        printf("❌ Parsing failed! Extra characters after parsing.\n");

    return 0;
}