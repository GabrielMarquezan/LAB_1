#include <stdio.h>
#include <stdbool.h>
#include "tela.h"
#include "tecla.h"

char ganhador(char matriz[3][3]);
bool verifica(char vetor[3]);
char linhas(char matriz[3][3]);
char colunas(char matriz[3][3]);
char diagonais(char matriz[3][3]);
void desenha_jogo(char matriz[3][3]);
void resultado_jogo(char matriz[3][3]);

int main()
{
    char jogo[3][3] = {{' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '}};
    char jogador, acao;
    int linha = 0, coluna = 0, posicaoy = 0, posicaox = 0;

    tela_ini();
    tela_mostra_cursor(true);
    tecla_ini();

    for(int i = 0; i < 10; i++)
    {
        tela_limpa();
        tela_atualiza();
        desenha_jogo(jogo);
        tela_atualiza();

        if(i % 2 == 0)
        {
            jogador = 'O';
        }
        else
        {
            jogador = 'X';
        }
        
        while(true)
        {
            acao = tecla_le_char();
        
            switch(acao)
            {
                case '\n':
                if(jogo[linha][coluna] == ' ')
                {
                    jogo[linha][coluna] = jogador;
                }
                break;

                case 'w':
                if(linha != 0)
                {
                    linha--;
                    tela_lincol(linha, coluna);
                }
                break;

                case 'W':
                if(linha != 0)
                {
                    linha--;
                    tela_lincol(linha, coluna);
                }
                break;

                case 's':
                if(linha != 2)
                {
                    linha++;
                    tela_lincol(linha, coluna);
                }
                break;

                case 'S':
                if(linha !=2)
                {
                    linha++;
                    tela_lincol(linha, coluna);
                }
                break;

                case 'a':
                if(coluna != 0)
                {
                    coluna--;
                    tela_lincol(linha, coluna);
                }
                break;

                case 'A':
                if(coluna != 0)
                {
                    coluna--;
                    tela_lincol(linha, coluna);
                }
                break;

                case 'd':
                if(coluna != 2)
                {
                    coluna++;
                    tela_lincol(linha, coluna);
                }
                break;

                case 'D':
                if(coluna != 2)
                {
                    coluna++;
                    tela_lincol(linha, coluna);
                }
                break;
            }


            if(acao == '\n')
            {
                break;
            }
        }
    }

    printf("Vencedor; %c", ganhador(jogo));

    tecla_fim();
    tela_fim();
}


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
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            if(matriz[i][j] == ' ')
            {
                return ' ';
            }
        }
    }
    
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
    printf("—————————————\n");

    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            printf("| %c ", matriz[i][j]);
        }

        printf("|\n");
        printf("—————————————\n");
    }
    printf("\n");
}
