%{
    #include<stdio.h>
    #include<string.h>
    char str[100];
    int valid=0;
%}
%%
[a-zA-Z]+ {if(strstr(yytext,str)==NULL)
            {
                printf("%s\n",yytext);
            }
            else{
                valid=1;
            }
          }
\n {return 0;}
%%
int main()
{
    printf("Enter the four letters:");
    scanf("%[^\n]",str);
    getchar();
    printf("Enter the string:");
    yylex();
    if(valid==0)
    {
        printf("the given string didnt have %s\n",str);
    }
    else{
        printf("The given string had %s\n",str);
    }
    return 0;
}
int yywrap()
{
    return 1;
}