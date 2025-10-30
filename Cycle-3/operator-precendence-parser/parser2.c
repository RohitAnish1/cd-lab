#include <stdio.h>
#include <string.h>


char stack[50];
int top=-1;
char terminals[50];
char precedence[10][10];

void push(char c)
{
    stack[++top]=c;
    stack[top + 1] = '\0';
}
void pop()
{
    if(top>=0)
    top--;
    stack[top + 1] = '\0';
}
char peek()
{
    if(top>=0)
    return stack[top];
    return '\0';
}

int getindex(char c ,char terminals[],int n)
{
    for(int i=0;i<n;i++)
    {
        if(terminals[i]==c)
        return i;
    }
    return -1;
}

int main()
{
    int n;
    printf("enter the no of terminals\n");
    scanf("%d",&n);
    printf("enter the terminals");
    scanf("%s",terminals);
    printf("enter the table values\n");
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            printf("enter the precedence for %c and %c\n",terminals[i],terminals[j]);
            scanf(" %c",&precedence[i][j]);
        }
    }
    char input[50];
    printf("enter the input \n");
    scanf("%s",input);
    strcat(input,"$");
    int i=0;
    char a=input[i];
    push('$');
    printf("STACK\tINPUT\tACTION\n");
    while(1)
    {
        printf("%s\t%s\t",stack,input+i);
        char topc=peek();
        if(topc=='$'&&a=='$')
        {
            printf("accepted \n");
            break;
        }
        int row=getindex(topc,terminals,n);
        int col=getindex(a,terminals,n);
        char rel=precedence[row][col];
        
        if(rel=='<'||rel=='=')
        {
            printf("shift %c\n",a);
            push(a);
            a=input[++i];
        }
        else if(rel=='>')
        {
            printf("reduce \n");
            pop();
        }
        else
        {
            printf("invalid relation\n");
            break;
        }
    }
    return 0;
}