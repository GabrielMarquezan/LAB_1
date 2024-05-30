#ifndef JOGODAVELHA_H
#define JOGODAVELHA_H

#include <stdio.h>
#include <stdbool.h>


char ganhador(char matriz[3][3]);
char linhas(char matriz[3][3]);
char colunas(char matriz[3][3]);
char diagonais(char matriz[3][3]);
void desenha_jogo(char matriz[3][3]);
void resultado_jogo(char matriz[3][3]);

#endif
