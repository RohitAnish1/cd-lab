%{
#include <stdio.h>
#include <string.h>

char forbidden_substring[5]; // To store the first 4 characters of name + null terminator
%}

%%
.*        {
             if (strstr(yytext, forbidden_substring)) {
                 printf("Invalid Variable: '%s' contains forbidden substring '%s'\n", yytext, forbidden_substring);
             } else {
                 printf("Accepted: %s\n", yytext);
             }
         }
\n        ; // Ignore newlines
%%

int main() {
    char name[100];

    // Step 2: Prompt user
    printf("Enter your name: ");

    // Step 3: Read name
    fgets(name, sizeof(name), stdin);

    // Remove newline if present
    name[strcspn(name, "\n")] = '\0';

    // Step 4: Extract first 4 characters
    strncpy(forbidden_substring, name, 4);
    forbidden_substring[4] = '\0'; // Null-terminate manually

    printf("Now enter strings (Ctrl+D to stop):\n");

    // Step 6: Start scanning inputs
    yylex();
    return 0;
}

int yywrap() {
    return 1;
}
