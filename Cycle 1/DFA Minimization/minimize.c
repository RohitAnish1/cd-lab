#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int findalpha(char inp, int a, char alphabet[a]) {
    for(int i = 0; i < a; i++) {
        if(alphabet[i] == inp) {
            return i;
        }
    }
    return -1;
}

void reset(int n, int table[n][n]) {
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            table[i][j] = 0;
        }
    }
}

int isFinal(int i, int f, int final[f]) {
    for(int j = 0; j < f; j++) {
        if(final[j] == i) {
            return 1;
        }
    }
    return 0;
}

void printTable(int n, int table[n][n]) {
    printf("\n");
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            printf("%d ", table[i][j]);
        }
        printf("\n");
    }
}

void merge(int n, int empty[n][n], int i, int j) {
    int flag = 0;
    for(int k = 0; k < n; k++) {
        if(empty[i][k] == 1 && empty[j][k] == 1) {
            flag = 1;
            break;
        }
    }

    if(flag) {
        for(int k = 0; k < n; k++) {
            if(empty[j][k] == 1) {
                empty[i][k] = 1;
                empty[j][k] = 0;
            }
        }
    }
}

int main() {
    int n, a, from, to, end, index;
    char state1[3], state2[3], inp[2];

    printf("Enter total no of states: ");
    scanf("%d", &n);

    printf("Enter total size of alphabet: ");
    scanf("%d", &a);
    
    char alphabet[a+1]; // +1 for null terminator
    int transition[n][a];
    int table[n][n];

    reset(n, table);

    printf("Enter alphabet characters (e.g., for '0 1' enter: 01): ");
    scanf("%s", alphabet);

    int f;
    printf("Enter no of final states: ");
    scanf("%d", &f);
    int final[f];

    printf("Enter final states: ");
    for(int i = 0; i < f; i++) {
        scanf("%d", &final[i]);
    }

    // Initialize transition table
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < a; j++) {
            transition[i][j] = -1; // Initialize to invalid state
        }
    }

    // Build Transition Matrix
    FILE *INPUT = fopen("DFA.txt", "r");
    if(INPUT == NULL) {
        printf("Error opening file DFA.txt\n");
        return 1;
    }

    while((end = fscanf(INPUT, "%s %s %s", state1, inp, state2)) != EOF) {
        if(end != 3) {
            printf("Error reading transition\n");
            continue;
        }
        from = state1[1] - '0';
        to = state2[1] - '0';
        index = findalpha(inp[0], a, alphabet);
        if(index == -1) {
            printf("Invalid symbol: %c\n", inp[0]);
            continue;
        }
        transition[from][index] = to;
    }
    fclose(INPUT);

    // Mark initial pairs
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < i; j++) {
            if((isFinal(i, f, final) && !isFinal(j, f, final))) {
                table[i][j] = 1;
            } else if((isFinal(j, f, final) && !isFinal(i, f, final))) {
                table[i][j] = 1;
            }
        }
    }

    printf("\nAfter Marking initial pairs:\n");
    printTable(n, table);

    int out1, out2, flag = 1;

    while(flag) {
        flag = 0;

        for(int i = 0; i < n; i++) {
            for(int j = 0; j < i; j++) {
                if(table[i][j] == 1) {
                    continue;
                }

                for(int k = 0; k < a; k++) {
                    out1 = transition[i][k];
                    out2 = transition[j][k];
                    
                    if(out1 == -1 || out2 == -1) {
                        // Handle undefined transitions
                        if(out1 != out2) {
                            table[i][j] = 1;
                            flag = 1;
                        }
                        continue;
                    }

                    if((out1 > out2 && table[out1][out2] == 1) || 
                       (out2 > out1 && table[out2][out1] == 1)) {
                        table[i][j] = 1;
                        flag = 1;
                        break;
                    }
                }
            }
        }
    }

    printf("\nFinal Myhill-Nerode table:\n");
    printTable(n, table);

    int empty[n][n];
    reset(n, empty);
    int flagarr[n];
    for(int i = 0; i < n; i++) {
        flagarr[i] = 0;
    }

    int k = 0;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < i; j++) {
            if(table[i][j] == 0) {
                empty[k][i] = 1;
                empty[k][j] = 1;
                flagarr[i] = 1;
                flagarr[j] = 1;
                k++;
            }
        }
    }

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if(i != j) {
                merge(n, empty, i, j);
            }
        }
    }

    for(int i = 0; i < n; i++) {
        if(flagarr[i] == 0) {
            empty[k][i] = 1;
            k++;
        }
    }

    printf("\nEquivalence classes:\n");
    printTable(n, empty);

    printf("\nMinimized DFA transitions:\n");
    for(int i = 0; i < n; i++) {
        // Find first state in this equivalence class
        int first_state = -1;
        for(int j = 0; j < n; j++) {
            if(empty[i][j] == 1) {
                first_state = j;
                break;
            }
        }
        
        if(first_state == -1) continue; // Skip empty classes

        for(int b = 0; b < a; b++) {
            // Print current state (equivalence class)
            printf("{");
            int first = 1;
            for(int k = 0; k < n; k++) {
                if(empty[i][k] == 1) {
                    if(!first) printf(", ");
                    printf("q%d", k);
                    first = 0;
                }
            }
            printf("}");

            // Print input symbol
            printf("\t%c\t", alphabet[b]);

            // Find transition for first state in class
            if(transition[first_state][b] == -1) {
                printf("UNDEFINED\n");
                continue;
            }

            int to_state = transition[first_state][b];

            // Find which equivalence class the to_state belongs to
            int found = 0;
            for(int y = 0; y < n; y++) {
                if(empty[y][to_state] == 1) {
                    printf("{");
                    first = 1;
                    for(int k = 0; k < n; k++) {
                        if(empty[y][k] == 1) {
                            if(!first) printf(", ");
                            printf("q%d", k);
                            first = 0;
                        }
                    }
                    printf("}\n");
                    found = 1;
                    break;
                }
            }
            
            if(!found) {
                printf("UNDEFINED\n");
            }
        }
    }

    return 0;
}