#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int n, a, f;

struct node {
    int state;
    struct node *next;
};

void reset(int n, int a, struct node* transition[n][a]) {
    for(int i=0; i<n; i++) {
        for(int j=0; j<a; j++) {
            transition[i][j] = NULL;
        }
    }
}

int findalpha(char inp, int a, char alphabet[a]) {
    for(int i=0; i<a; i++) {
        if(alphabet[i] == inp) {
            return i;
        }
    }
    return -1;
}

void findClosure(int start, int state, int set[n], int n, int a, struct node *transition[n][a], int e_closure[n][n]) {
    if(set[state] == 1)
        return;
    set[state] = 1;
    e_closure[start][state] = 1;
    struct node *temp = transition[state][a-1]; // epsilon is last symbol
    while(temp != NULL) {
        findClosure(start, temp->state, set, n, a, transition, e_closure);
        temp = temp->next;
    }
}

void printTransition(int n, int a, struct node* transition[n][a], char alphabet[a]) {
    printf("\nTransition table:\n");
    printf("State\t");
    for(int j=0; j<a; j++) {
        printf("%c\t", alphabet[j]);
    }
    printf("\n");
    
    for(int i=0; i<n; i++) {
        printf("q%d\t", i);
        for(int j=0; j<a; j++) {
            struct node *head = transition[i][j];
            printf("{");
            while(head != NULL) {
                printf("q%d", head->state);
                head = head->next;
                if(head != NULL) printf(", ");
            }
            printf("}\t");
        }
        printf("\n");
    }
}

// ---------- Renaming State Sets ----------
int state_sets[100][100]; // [state_set_index][n]
int state_set_count = 0;

int get_state_set_index(int set[], int n) {
    for(int i=0; i<state_set_count; i++) {
        int match = 1;
        for(int j=0; j<n; j++) {
            if(state_sets[i][j] != set[j]) {
                match = 0;
                break;
            }
        }
        if(match) return i;
    }

    // Not found: add as new set
    for(int j=0; j<n; j++) {
        state_sets[state_set_count][j] = set[j];
    }
    return state_set_count++;
}

void print_named_state(int set[], int n) {
    int index = get_state_set_index(set, n);
    printf("q%d", index);
}

int is_final_state(int state_set[n], int final_states[], int f, int e_closure[n][n]) {
    for(int i=0; i<f; i++) {
        for(int j=0; j<n; j++) {
            if(state_set[j] && e_closure[j][final_states[i]]) {
                return 1;
            }
        }
    }
    return 0;
}

int main() {
    printf("Enter no of states: ");
    scanf("%d", &n);
    printf("Enter no of input symbols: ");
    scanf("%d", &a);

    struct node *transition[n][a];
    char alphabet[a];
    int e_closure[n][n], set[n];

    reset(n, a, transition);

    printf("Enter %d input symbols separated by space (if epsilon is present, it should be the last):\n", a);
    for(int i = 0; i < a; i++) {
        scanf(" %c", &alphabet[i]);
    }

    printf("Enter total number of final states: ");
    scanf("%d", &f);
    int final[f];

    printf("Enter %d final state(s) separated by space:\n", f);
    for(int i = 0; i < f; i++) {
        scanf("%d", &final[i]);
    }

    printf("Enter transitions in format <from_state> <input_symbol> <to_state> (Enter -1 to stop):\n");
    while (1) {
        int from, to;
        char symbol;
        scanf("%d", &from);
        if (from == -1) break;
        scanf(" %c %d", &symbol, &to);

        if(from >= n || to >= n) {
            printf("Invalid state number\n");
            continue;
        }

        int index = findalpha(symbol, a, alphabet);
        if(index == -1) {
            printf("Invalid symbol: %c\n", symbol);
            continue;
        }

        struct node *temp = (struct node *)malloc(sizeof(struct node));
        temp->state = to;
        temp->next = transition[from][index];
        transition[from][index] = temp;
    }

    printTransition(n, a, transition, alphabet);

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            set[j] = 0;
            e_closure[i][j] = 0;
        }
        findClosure(i, i, set, n, a, transition, e_closure);
    }

    printf("\nEpsilon closures:\n");
    for(int i = 0; i < n; i++) {
        printf("e-closure(q%d) = {", i);
        int first = 1;
        for(int j = 0; j < n; j++) {
            if(e_closure[i][j]) {
                if(!first) printf(", ");
                printf("q%d", j);
                first = 0;
            }
        }
        printf("}\n");
    }

    printf("\nNFA without epsilon transitions:\n");
    printf("State\tSymbol\tResult\n");
    for(int i=0; i<n; i++) {
        for(int j=0; j<a-1; j++) { // skip epsilon
            for(int x=0; x<n; x++) set[x] = 0;

            for(int k=0; k<n; k++) {
                if(e_closure[i][k]) {
                    struct node *temp = transition[k][j];
                    while(temp != NULL) {
                        for(int m=0; m<n; m++) {
                            if(e_closure[temp->state][m]) {
                                set[m] = 1;
                            }
                        }
                        temp = temp->next;
                    }
                }
            }

            print_named_state(e_closure[i], n);
            printf("\t%c\t", alphabet[j]);
            print_named_state(set, n);
            printf("\n");
        }
    }

    printf("\nFinal states:\n");
    for(int i=0; i<state_set_count; i++) {
        if(is_final_state(state_sets[i], final, f, e_closure)) {
            print_named_state(state_sets[i], n);
            printf("\n");
        }
    }

    for(int i=0; i<n; i++) {
        for(int j=0; j<a; j++) {
            struct node *current = transition[i][j];
            while(current != NULL) {
                struct node *temp = current;
                current = current->next;
                free(temp);
            }
        }
    }

    return 0;
}
