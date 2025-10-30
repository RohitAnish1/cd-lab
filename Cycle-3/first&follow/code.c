#include <stdio.h>
#include <string.h>
#include <ctype.h>

int n;
char p[10][10];
char first[10][10], follow[10][10];
int computed[10] = {0};

void add(char *set, char c) {
    if(!strchr(set, c)) {
        int len = strlen(set);
        set[len] = c;
        set[len+1] = '\0';
    }
}

void findFirst(char c) {
    int idx = c - 'A';
    if(computed[idx]) return;
    computed[idx] = 1;
    
    for(int i=0; i<n; i++) {
        if(p[i][0] == c) {
            char *rhs = p[i] + 2;
            if(*rhs == 'e') { // epsilon
                add(first[idx], '#');
            }
            else if(!isupper(*rhs)) { // terminal
                add(first[idx], *rhs);
            }
            else { // non-terminal
                for(char *s = rhs; *s; s++) {
                    if(isupper(*s)) {
                        findFirst(*s);
                        for(char *f = first[*s-'A']; *f; f++) {
                            if(*f != '#') add(first[idx], *f);
                        }
                        if(!strchr(first[*s-'A'], '#')) break;
                    } else {
                        add(first[idx], *s);
                        break;
                    }
                }
            }
        }
    }
}

void findFollow(char c) {
    int idx = c - 'A';
    
    // Start symbol gets $
    if(p[0][0] == c) 
        add(follow[idx], '$');
    
    for(int i=0; i<n; i++) {
        char *rhs = p[i] + 2;
        for(int j=0; rhs[j]; j++) {
            if(rhs[j] == c) {
                if(rhs[j+1]) { // Something follows
                    if(!isupper(rhs[j+1])) { // Terminal
                        add(follow[idx], rhs[j+1]);
                    } else { // Non-terminal
                        for(char *f = first[rhs[j+1]-'A']; *f; f++) {
                            if(*f != '#') add(follow[idx], *f);
                        }
                        if(strchr(first[rhs[j+1]-'A'], '#')) {
                            findFollow(p[i][0]);
                            for(char *fl = follow[p[i][0]-'A']; *fl; fl++)
                                add(follow[idx], *fl);
                        }
                    }
                } else { // End of production
                    if(p[i][0] != c) {
                        findFollow(p[i][0]);
                        for(char *fl = follow[p[i][0]-'A']; *fl; fl++)
                            add(follow[idx], *fl);
                    }
                }
            }
        }
    }
}

int main() {
    printf("Number of productions: ");
    scanf("%d", &n);
    printf("Enter productions (use 'e' for epsilon):\n");
    for(int i=0; i<n; i++) scanf("%s", p[i]);
    
    // Compute FIRST sets
    for(int i=0; i<n; i++) 
        findFirst(p[i][0]);
    
    // Reset for FOLLOW computation
    memset(computed, 0, sizeof(computed));
    
    // Compute FOLLOW sets  
    for(int i=0; i<n; i++)
        findFollow(p[i][0]);
    
    printf("\nFIRST Sets:\n");
    for(int i=0; i<n; i++) {
        printf("FIRST(%c) = { ", p[i][0]);
        for(int j=0; first[p[i][0]-'A'][j]; j++)
            printf("%c ", first[p[i][0]-'A'][j]);
        printf("}\n");
    }
    
    printf("\nFOLLOW Sets:\n");
    for(int i=0; i<n; i++) {
        printf("FOLLOW(%c) = { ", p[i][0]);
        for(int j=0; follow[p[i][0]-'A'][j]; j++)
            printf("%c ", follow[p[i][0]-'A'][j]);
        printf("}\n");
    }
    
    return 0;
}