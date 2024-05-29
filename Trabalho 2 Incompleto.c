#include <stdio.h>
#include <stdbool.h>
#include "tela.h"
#include "tecla.h"
#include "jogodavelha.h"

int main()
{
    char jogo[3][3] = {{' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '}};
    char jogador, acao, vencedor;
    int linha = 0, coluna = 0, eixox = 56, eixoy = 12;
    
    
    tela_ini();
    tecla_ini();
    tela_cor_fundo(0,0,0);
    tela_limpa();
    
    tela_atualiza();

    while(true)
    {

        //Laço que executa a partida
        for(int i = 0; i < 10; i++)
        {
            //define o jogador da vez
            if(i % 2 == 0)
            {
                jogador = 'X';
            }
            else
            {
                jogador = 'O';
            }

            vencedor = ganhador(jogo);
            printf("%c", vencedor);

            desenha_jogo(jogo);
            tela_lincol(eixoy, eixox);
            tela_atualiza();

            while(true)
            {
                //Laço que recebe e responde a movimentação o jogador

                tela_lincol(eixox, eixoy);
                acao = tecla_le_char();
                

                
                if(acao == '\n')
                {
                    if(jogo[linha][coluna] == ' ')
                    {
                        jogo[linha][coluna] = jogador;
                        break;
                    }
                    else
                    {
                        tela_lincol(19, 47);
                        printf("Essa casa já está preenchida!\n");
                        tela_lincol(20, 55);
                        printf("Tente outra.");
                    }
                }
                else if((acao == 'w' || acao == 'W') && linha != 0)
                {
                    linha--;
                    eixoy -= 2;
                }
                else if((acao == 's' || acao == 'S') && linha != 2)
                {
                    linha++;
                    eixoy += 2;
                }
                else if((acao == 'a' || acao == 'A') && coluna != 0)
                {
                    coluna--;
                    eixox -= 4;
                }
                else if((acao == 'd' || acao == 'D') && coluna != 2)
                {
                    coluna++;
                    eixox += 4;
                }

                desenha_jogo(jogo);
                tela_lincol(eixoy, eixox);
                tela_atualiza();
            }

            tela_limpa();
        }
        break;
    }



    tecla_fim();
    tela_fim();
}
