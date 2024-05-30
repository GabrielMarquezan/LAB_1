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

    printf("%c\n", vitorioso[0]);
    printf("%c\n", vitorioso[1]);
    printf("%c\n", vitorioso[2]);
    for(int i = 0; i < 3; i++)
    {
        if(vitorioso[i] != ' ')
        {
            return vitorioso[i];
        }
    }

    return 'E';
}

char linhas(char matriz[3][3])
{
    for(int i = 0; i < 3; i++)
    {
        if(matriz[i][0] == matriz[i][1] && matriz[i][1] == matriz[i][2])
        {
            return matriz[i][0];
        }
    }

    return ' ';
}

char colunas(char matriz[3][3])
{
    for(int i = 0; i < 3; i++)
    {
        if(matriz[0][i] == matriz[1][i] && matriz[1][i] == matriz[2][i])
        {
            return matriz[0][i];
        }
    }
    
    return ' ';
}

char diagonais(char matriz[3][3])
{
    if(matriz[0][0] == matriz[1][1] && matriz[1][1] == matriz[2][2])
    {
        return matriz[0][0];
    }
    else if(matriz[0][2] == matriz[1][1] && matriz[1][1] == matriz[2][0])
    {
        return matriz[0][2];
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
