#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char inp[100];

int curr = 0;

int E();
int E_();
int T();
int T_();
int F();

int main()
{
    printf("enter input : \n");
    scanf("%s", inp);

    int res = E();

    if (res == 1 &&curr == strlen(inp))
    {
        printf("Input has been accepted \n");
    }
    else
    {
        printf("Input has been rejected \n");
    }

    return 0;
}

// E->TE'

int E()
{
    if (T())
    {
        if (E_())
        {
            return 1;
        }
    }
    return 0;
}

// E' -> +TE' | -TE' | e

int E_()
{
    if (inp[curr] == '+' || inp[curr] == '-')
    {
        curr++;
        if (T())
        {
            if (E_())
                return 1;
            else
                return 0;
        }
        else
        {
            return 0;
        }
    }
    return 1;
}

// T-> FT'

int T()
{
    if (F())
    {
        if (T_())
            return 1;
    }
    return 0;
}

// T' -> *FT' | /FT' | e

int T_()
{
    if (inp[curr] == '*' || inp[curr] == '/')
    {
        curr++;
        if (F())
        {
            if (T_())
                return 1;
            else
                return 0;
        }
        else
        {
            return 0;
        }
    }
    return 1;
}

// F-> (E) / i

int F()
{
    if (inp[curr] == '(')
    {
        curr++;
        if (E())
        {
            if (inp[curr] == ')')
            {
                curr++;
                return 1;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
    else if (inp[curr] == 'i')
    {
        curr++;
        return 1;
    }
    return 0;
}