#include <stdio.h>
#include <string.h>
#include <ctype.h>

char reg = 'X';

void generate_code(char *expr, char op) {
    char temp[100];
    int i = 0, j = 0;
    
    while(expr[i] != '\0') {
        if(expr[i] == op) {
            // Extract operands
            char op1 = expr[i-1];
            char op2 = expr[i+1];
            
            // Generate three-address code
            printf("%c\t%c\t%c\t%c\n", op, reg, op1, op2);
            
            // Replace the operation with result
            temp[j-1] = reg;  // Overwrite op1 with result
            reg++;            // Next temporary register
            i += 2;           // Skip operator and op2
        } else {
            temp[j++] = expr[i++];
        }
    }
    temp[j] = '\0';
    strcpy(expr, temp);
}

int main() {
    char expr[100];
    
    printf("Enter expression (e.g., a=b+c*d): ");
    scanf("%s", expr);
    
    printf("\nOperator\tDest\tOp1\tOp2\n");
    printf("--------------------------------\n");
    
    // Generate code in precedence order
    generate_code(expr, '/');
    generate_code(expr, '*');
    generate_code(expr, '+');
    generate_code(expr, '-');
    generate_code(expr, '=');
    
    return 0;
}