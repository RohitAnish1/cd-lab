#include <stdio.h>
#include <string.h>
#include <ctype.h>

int isKeyword(char *word) {
    char keywords[][10] = {"int", "float", "if", "else", "while", "for", "return"};
    for (int i = 0; i < 7; i++) {
        if (strcmp(word, keywords[i]) == 0)
            return 1;
    }
    return 0;
}

int main() {
    FILE *fp;
    char ch, word[100];
    int i;

    fp = fopen("input.c", "r"); // open input file
    if (!fp) {
        printf("Cannot open file.\n");
        return 0;
    }

    while ((ch = fgetc(fp)) != EOF) {

        // skip spaces, tabs, newlines
        if (ch == ' ' || ch == '\t' || ch == '\n')
            continue;

        // if letter → identifier or keyword
        if (isalpha(ch)) {
            i = 0;
            while (isalnum(ch)) {
                word[i++] = ch;
                ch = fgetc(fp);
            }
            word[i] = '\0';
            fseek(fp, -1, SEEK_CUR); // go one step back
            if (isKeyword(word))
                printf("KEYWORD: %s\n", word);
            else
                printf("IDENTIFIER: %s\n", word);
        }

        // if digit → number
        else if (isdigit(ch)) {
            i = 0;
            while (isdigit(ch)) {
                word[i++] = ch;
                ch = fgetc(fp);
            }
            word[i] = '\0';
            fseek(fp, -1, SEEK_CUR);
            printf("NUMBER: %s\n", word);
        }

        // if operator
        else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '=')
            printf("OPERATOR: %c\n", ch);

        // if delimiter
        else if (ch == '(' || ch == ')' || ch == '{' || ch == '}' || ch == ';' || ch == ',')
            printf("DELIMITER: %c\n", ch);

        else
            printf("UNKNOWN: %c\n", ch);
    }

    fclose(fp);
    return 0;
}
