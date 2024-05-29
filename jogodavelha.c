#include "jogodavelha.h"
#include "tela.h"

void resultado_jogo(char matriz[3][3])
{
    char vencedor = ganhador(matriz);
    desenha_jogo(matriz);
    
    if(vencedor != ' ' && vencedor != 'E')
    {
        printf("Vencedor: %c\n", ganhador(matriz));
    }
    else if(vencedor == ' ')
    {
        printf("Jogo incompleto\n");
    }
    else
    {
        printf("Houve empate\n");
    }
}

char ganhador(char matriz[3][3])
{
    char vitorioso[3] = {linhas(matriz), colunas(matriz), diagonais(matriz)};

    for(int i = 0; i < 3; i++)
    {
        if(vitorioso[i] != ' ')
        {
            return vitorioso[i];
        }
    }

    return 'E';
}

bool verifica(char vetor[3])
{
    if(vetor[0] == vetor[1] && vetor[0] == vetor[2])
    {
        return true;
    }
    
    return false;
}

char linhas(char matriz[3][3])
{
    char aux[3];

    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            aux[j] = matriz[i][j];
        }

        if(verifica(aux))
        {
            return aux[0];
        }
    }
    return ' ';
}

char colunas(char matriz[3][3])
{
    char aux[3];

    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            aux[j] = matriz[j][i];
        }

        if(verifica(aux))
        {
            return aux[0];
        }
    }
    return ' ';
}

char diagonais(char matriz[3][3])
{
    char aux[3];
    int j = 0;

    for(int i = 0; i < 3; i++)
    {
        aux[i] = matriz[i][i];
        
        if(verifica(aux))
        {
            return aux[0];
        }
    }

    for(int i = 2; i >= 0; i--)
    {
        aux[i] = matriz[j][i];

        if(verifica(aux))
        {
            return aux[0];
        }

        j++;
    }

    return ' ';
}

void desenha_jogo(char matriz[3][3])
{
    int fix = 12;

    tela_lincol(fix - 1, 54);
    printf("┏━━━┳━━━┳━━━┓\n");

    for(int i = 0; i < 3; i++)
    {
        tela_lincol(fix, 54);
        printf("┃");
        for(int j = 0; j < 3; j++)
        {
            printf(" %c ┃", matriz[i][j]);
        }
        printf("\n");
        
        
        if(i < 2)
        {
            tela_lincol(fix + 1, 54);
            printf("┣━━━╋━━━╋━━━┫\n");
            fix += 2;
        }
    }
    
    tela_lincol(fix + 1, 54);
    printf("┗━━━┻━━━┻━━━┛\n");
    printf("\n");
}
