#include <stdio.h>

void imprimir(int n, char c);
void t1(int n);
void t2(int n);
void t3(int n);
void t4(int n);
void t5(int n);
void t6(int n);
void t7(int n);

int main()
{
    t1(4);
    printf("\n");
    t2(4);
    printf("\n");
    t3(4);
    printf("\n");
    t4(4);
    printf("\n");
    t5(4);
    printf("\n");
    t6(4);
    printf("\n");
    t7(4);
}

void imprimir(int n, char c)
{
    for (int i = 0; i < n; i++)
    {
        printf("%c", c);
    }
}

void t1(int n)
{
    for (int i = 1; i <= n; i++)
    {
        imprimir(i, '*');
        printf("\n");
    }
}

void t2(int n)
{
    for(int i = n; i > 0; i--)
    {
        imprimir(i, '*');
        printf("\n");
    }
}

void t3(int n)
{
    int aux = n - 1;

    for(int i = 1; i <= n; i++)
    {
        imprimir(aux, ' ');
        imprimir(i, '*');
        printf("\n");

        aux--;
    }
}

void t4(int n)
{
    int aux = 0; 

    for(int i = n; i > 0; i--)
    {
        imprimir(aux, ' ');
        imprimir(i, '*');
        printf("\n");

        aux++;
    }
}

void t5(int n)
{
    int aux = n - 1; 
    int aux2 = n * 2 - 1;
    int aux3 = 1;

    for(int i = 1; i <= aux2; i += 2)
    {
        imprimir(aux, ' ');
        imprimir(i, '*');
        printf("\n");

        aux--;
    }  

    for(int i = aux2 - 2; i > 0; i -= 2)
    {
        imprimir(aux3 , ' ');
        imprimir(i, '*');
        printf("\n");

        aux3++;
    }
}

void t6(int n)
{   
    int aux = -1;
    int aux2 = n - 1;

    for(int i = n - 1; i >= 0; i--)
    {  
        imprimir(i, ' ');
        printf("*");
        imprimir(aux, ' ');
        
        if(aux > 0)
        {
            printf("*");
        }

        printf("\n");

        aux += 2;
    }

    for(int i = 1; i < n; i++)
    {
        imprimir(i, ' ');
        printf("*");
        imprimir(aux2, ' ');

        if(aux2 > 0)
        {
            printf("*");
        }

        printf("\n");

        aux2 -= 2;
    }
}

void t7(int n)
{
    int aux = n;
    int aux2 = n + 1;
    int aux3 = 2;

    for(int i = 0; i <= n * 2 + 1; i+= 2)
    {
        imprimir(aux2, '*');
        imprimir(i - 1, ' ');
        imprimir(aux, '*');
        printf("\n");

        aux2--;

        if(i > 1)
        {
            aux--;
        }
    }

    for(int i = n * 2 - 2; i >= 0; i -= 2)
    {
        imprimir(aux3, '*');
        imprimir(i - 1, ' ');

        if(aux3 != n + 1)
        {
            imprimir(aux3, '*');
        }
        else
        {
            imprimir(aux3 - 1, '*');
        }

        printf("\n");

        aux3++;
    }
}
