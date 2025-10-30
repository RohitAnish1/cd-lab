#include <stdio.h>

int n;                       // number of states
int eps[20][20];             // epsilon transitions matrix
int visited[20];             // visited array

void epsilonClosure(int state) {
    visited[state] = 1;
    for (int i = 0; i < n; i++) {
        if (eps[state][i] && !visited[i]) {
            epsilonClosure(i);
        }
    }
}

int main() {
    int i, j, t, from, to;
    char symbol;

    printf("Enter number of states: ");
    scanf("%d", &n);

    // Initialize epsilon matrix
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            eps[i][j] = 0;

    printf("Enter number of transitions: ");
    scanf("%d", &t);

    printf("Enter transitions (from to symbol):\n");
    printf("(Use 'e' for epsilon)\n");

    for (i = 0; i < t; i++) {
        scanf("%d %d %c", &from, &to, &symbol);
        if (symbol == 'e')
            eps[from - 1][to - 1] = 1; // use 0-based indexing
    }

    printf("\nEpsilon Closures:\n");
    for (i = 0; i < n; i++) {
        // reset visited array
        for (j = 0; j < n; j++)
            visited[j] = 0;

        epsilonClosure(i);

        printf("q%d: ", i + 1);
        for (j = 0; j < n; j++) {
            if (visited[j])
                printf("q%d,", j + 1);
        }
        printf("\n");
    }

    return 0;
}
