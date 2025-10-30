%{
#include <stdio.h>
#include <stdlib.h>

// Function declarations
int yylex(void);
int yyerror(const char *s);
%}

%token IDENTIFIER INVALID

%%
input:
    IDENTIFIER  { printf("Valid variable name\n"); exit(0); }
    | INVALID   { printf("Invalid variable name\n"); exit(1); }
    ;
%%

int yyerror(const char *s) {
    printf("Error: %s\n", s);
    return 1;
}

int main() {
    printf("Enter a variable name: ");
    yyparse();
    return 0;
}