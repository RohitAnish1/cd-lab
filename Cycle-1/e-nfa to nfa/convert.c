#include <stdio.h>
#include <string.h>

#define MAX 10

int n, symbols;
char inputs[MAX];
int trans[MAX][MAX][MAX];  // trans[from][symbol_index][to]
int eclose[MAX][MAX];      // epsilon closure
int visited[MAX];

int getSymbolIndex(char c) {
    if (c == 'e') return symbols; // last index for epsilon
    for (int i = 0; i < symbols; i++)
        if (inputs[i] == c) return i;
    return -1;
}

void epsilonClosure(int state) {
    if (visited[state]) return;
    visited[state] = 1;
    for (int i = 0; i < n; i++) {
        if (trans[state][symbols][i])  // last index = epsilon
            epsilonClosure(i);
    }
}

int main() {
    int i, j, k, m;
    char symbol;
    printf("Enter number of states: ");
    scanf("%d", &n);

    printf("Enter number of input symbols (excluding e): ");
    scanf("%d", &symbols);

    printf("Enter the symbols: ");
    for (i = 0; i < symbols; i++) scanf(" %c", &inputs[i]);

    printf("Enter number of transitions: ");
    scanf("%d", &m);

    memset(trans, 0, sizeof(trans));

    printf("Enter transitions (from to symbol): (use 'e' for epsilon)\n");
    for (i = 0; i < m; i++) {
        int from, to;
        scanf("%d %d %c", &from, &to, &symbol);
        int idx = getSymbolIndex(symbol);
        if (idx == -1) idx = symbols; // treat 'e' as last index
        trans[from][idx][to] = 1;
    }

    // Find epsilon closure
    for (i = 0; i < n; i++) {
        memset(visited, 0, sizeof(visited));
        epsilonClosure(i);
        for (j = 0; j < n; j++)
            if (visited[j]) eclose[i][j] = 1;
    }

    printf("\nEpsilon closures:\n");
    for (i = 0; i < n; i++) {
        printf("q%d: ", i);
        for (j = 0; j < n; j++)
            if (eclose[i][j]) printf("q%d ", j);
        printf("\n");
    }

    printf("\nNFA without epsilon transitions:\n");
    for (i = 0; i < n; i++) {
        for (k = 0; k < symbols; k++) {
            printf("δ(q%d, %c) = { ", i, inputs[k]);
            int printed = 0;
            for (j = 0; j < n; j++) {
                if (eclose[i][j]) {
                    for (int p = 0; p < n; p++) {
                        if (trans[j][k][p]) {
                            for (int q = 0; q < n; q++) {
                                if (eclose[p][q]) {
                                    printf("q%d ", q);
                                    printed = 1;
                                }
                            }
                        }
                    }
                }
            }
            if (!printed) printf("∅");
            printf("}\n");
        }
    }
    return 0;
}
