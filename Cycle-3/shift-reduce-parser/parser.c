sr parser #include <stdio.h>
#include <string.h>

char stack[50];
int top=-1;

struct Rule
{
    char rhs[10];
    char lhs;
};

void push(char c)
{
    stack[++top]=c;
    stack[top+1]='\0';
}
void pop(int n)
{
    top-=n;
    if(top<0)
    top=-1;
    stack[top+1]='\0';
}
int reduce(struct Rule rules[],int count)
{
    for(int i=0;i<count;i++)
    {
        int len=strlen(rules[i].rhs);
        if(top+1>=len)
        {
            if(strncmp(&stack[top-len+1],rules[i].rhs,len)==0)
            {
                printf("REDUCE %s->%c\n",rules[i].rhs,rules[i].lhs);
                pop(len-1);
                stack[top]=rules[i].lhs;
                stack[top+1]='\0';
                return 1;
            }
        }
    }
    return 0;
}
int main()
{
    char input[50];
    int count;
    printf("enter the no of rules\n");
    scanf("%d",&count);
    struct Rule rules[20];
    for(int i=0;i<count;i++)
    {
        printf("enter the lhs \n");
        scanf(" %c",&rules[i].lhs);
        printf("enter the rhs \n");
        scanf("%s",rules[i].rhs);
    }
    printf("entered  grammar rules are \n");
    for(int i=0;i<count;i++)
    {
        printf("%c->%s\n",rules[i].lhs,rules[i].rhs);
    }
    printf("enter the input\n");
    scanf("%s",input);
    stack[0]='\0';
    int i=0;
    char a=input[i];
    printf("STACK\t INPUT\t ACTION\n");
    while(a!='\0')
    {
        printf("%s\t%s\tSHIFT %c\n",stack,input+i,a);
        push(a);
        a=input[++i];
        while(reduce(rules,count));
    }
    while(reduce(rules,count));
    if (top == 0 && ((stack[top] >= 'A' && stack[top] <= 'Z'))) {
        printf("%-10s\t%-10s\tACCEPTED\n", stack, input + i);
    } else {
        printf("Parsing Error!\n");
    }

    return 0;
}