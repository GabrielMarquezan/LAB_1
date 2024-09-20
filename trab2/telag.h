#ifndef _telag_h_  // para evitar problemas com inclus�o m�ltipla deste arquivo
#define _telag_h_

//
// telag.h
// ------
//
// fun��es de acesso a tela, mouse, rel�gio
//

#include <stdbool.h>

// inicializa��o da tela
// cria uma janela com o tamanho dado em pixels
// deve ser executada antes do uso de qualquer outra fun��o da tela
void tela_inicio(int largura, int altura, char nome[]);


// finaliza��o da tela
// deve ser chamada no final da utiliza��o da tela, nenhuma outra fun��o da
// tela deve ser chamada depois desta.
void tela_fim(void);


// atualiza a tela
// faz com o que foi desenhado na tela desde a �ltima atualiza��o
// realmente apare�a.
// caso necess�rio, espera at� que a imagem anterior tenha ficado
// na tela pelo tempo adequado.
// antes da chamada a esta fun��o a imagem anterior fica sendo exibida, 
// o conte�do da nova imagem fica s� na mem�ria.
void tela_atualiza(void);

// frequencia de atualizacao da tela
#define QUADROS_POR_SEGUNDO 30.0
#define SEGUNDOS_POR_QUADRO (1/QUADROS_POR_SEGUNDO)

// DESENHO

// desenha um c�rculo
// x e y s�o as coordenadas do centro do c�rculo, r � o raio
// l � a largura da linha, corl a cor da linha e corint � a cor
// do interior do c�rculo
void tela_circulo(float x, float y, float r, float l, int corl, int corint);

// desenha uma linha reta
// x1, y1 e x2, y2 s�o as coordenadas das pontas, l a largura e corl a cor
void tela_linha(float x1, float y1, float x2, float y2, float l, int corl);

// desenha um retangulo
// x1, y1 s�o as coordenadas do canto superior esquerdo, 
// x2, y2 as do inferior direito
// l � a largura da linha, corl a cor da linha e corint � a cor
// do interior do retangulo
void tela_retangulo(float x1, float y1, float x2, float y2, float l,
    int corl, int corint);


// desenha texto centrado
// x,y coordenadas do meio do texto, tam tamanho das letras, c cor, t texto
void tela_texto(float x, float y, int tam, int c, char t[]);

// desenha texto � esquerda
// x,y coordenadas do fim do texto, tam tamanho das letras, c cor, t texto
void tela_texto_esq(float x, float y, int tam, int c, char t[]);

// desenha texto � direita
// x, y coordenadas do inicio do texto, tam tamanho das letras, c cor, t texto
void tela_texto_dir(float x, float y, int tam, int c, char t[]);



// CORES

// valores para representar cores pr�-definidas
#define transparente 0
#define azul 1
#define vermelho 2
#define verde 3
#define amarelo 4
#define preto 5
#define laranja 6
#define rosa 7
#define branco 8
#define marrom 9

// altera uma cor (cor entre 0 e 99, componentes entre 0 e 1)
void tela_altera_cor(int cor,
    float vm, float az, float vd, float opacidade);



// ACESSO AO RATO

// coloca nos inteiros apontador por px e py a posi��o x, y do mouse
void tela_rato_pos(int* px, int* py);

// retorna se o bot�o do mouse est� apertado
bool tela_rato_apertado(void);

// retorna se o bot�o do mouse foi clicado (apertado e solto) desde a �ltima
// chamada a esta fun��o
bool tela_rato_clicado(void);

// retorna a posi��o do mouse na �ltima vez que ele foi clicado
void tela_rato_pos_clique(int* px, int* py);

// retorna uma tecla digitada ou '\0' se nenhuma tecla for digitada
// se for digitado enter, retorna '\n'
// se for digitado backspace, retorna '\b'
char tela_tecla(void);


// TEMPO

// retorna quantos segundos transcorreram desde algum momento no passado
double tela_relogio(void);

#endif // _telag_h_#pragma once
