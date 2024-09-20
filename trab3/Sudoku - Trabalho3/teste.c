#include "telag2.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

// gcc -Wall -o sudoku sudoku.c telag2.c -lallegro_font -lallegro_color -lallegro_ttf -lallegro_primitives -lallegro -lm

//defines
#define TELA_X 1500
#define TELA_Y 800
#define TAMANHO_TEXTO_JOGAR 100

//structs
typedef struct {
    cor_t principal;
    cor_t fundo;
    cor_t secundaria;
    cor_t fundoinversa;
}PALETA; // peleta de cores com 4 cores diferentes

typedef struct {

    int bloco; // qual dos 3x3 esse quadradinho ta
    int estado; // 0 = vazio ou com marcações; 1 = preenchido pelo jogador; 2 = preenchido pelo jogo no começo.
    bool marca[9]; // 0 = número n ta marcado; 1 = número tá marcado.
    int num; // número que está marcado; 0 se nada

}CASINHA; // uma struct pra cada um dos quadradinhos do 9x9

typedef struct {

    int certo;
    int dificuldade;
    int id;
    CASINHA matriz[9][9];
    int num;

}TABULEIRO; // uma struct pro estado do tabuleiro

typedef struct {

    TABULEIRO tabuleiro;
    double data_inicio;
    double tempo_de_jogo;
    bool terminou;
    bool desistiu;
    rato_t mouse;
    int selecionado;
    bool erro;

}JOGO; // uma struct pra partida

typedef struct {

    int colocacao;
    int id;
    int pontos;
    char nome[50];

}RECORDE; // uma struct pros recordes

// variaveis globais

//pontos e retangulos e sla mais oq
ponto_t centro = { TELA_X / 2, TELA_Y / 2 };
ponto_t canto00 = { 0, 0 };
tamanho_t tamtela = { TELA_X, TELA_Y };
retangulo_t posicao_tab = { {((TELA_X - (TELA_Y / 4) * 3) / 2), ((TELA_Y / 4) / 2)}, {(TELA_Y / 4) * 3, (TELA_Y / 4) * 3} }; // retangulo de onde vai ficar o tabuleiro

//cores
cor_t fucsia = { 1, 0.2, 0.8, 1 };
cor_t cinza = { 0.6, 0.6, 0.6, 1 };
cor_t preto = { 0, 0, 0, 1 };
cor_t branco = { 1, 1, 1, 1 };
cor_t transparente = { 0, 0, 0, 0 };
cor_t vermelho = { 1, 0, 0, 1 };
cor_t amarelo = { 0.9, 0.6, 0, 1 };
cor_t azul = { 0, 0, 1, 1 };
cor_t verde = { 0, 1, 0, 1 };
cor_t roxo = { 1, 0, 1, 1 };
cor_t laranja = { 1, 0.4, 0, 1 };
cor_t ciano = { 0, 0.6, 0.5, 1 };


//funções

//funções aleatorias sla
int aleatorio_entre(int minimo, int maximo) { // função que tava no testa_telag2
    return minimo + rand() % (maximo - minimo);
}

retangulo_t retangulo_jogar(void) { // retorna o retangulo do botão de "jogar" da tela inicial.

    retangulo_t retbotjogar = tela_texto_contorno(canto00, TAMANHO_TEXTO_JOGAR, "JOGAR");
    //pro retangulo n ficar muito na borda das letras eu aumentei um pouco
    retbotjogar.tamanho.largura += 16;
    retbotjogar.tamanho.altura += 8;
    //ajeitando a posição do botão pra ficar no meio da tela
    retbotjogar.inicio.x = (TELA_X / 2) - (retbotjogar.tamanho.largura / 2);
    retbotjogar.inicio.y = (TELA_Y / 2) - (retbotjogar.tamanho.altura / 2);

    return retbotjogar;

}

retangulo_t retangulo_desistir(void) { // retorna o retangulo do botão de "desistir" da tela do jogo
    ponto_t ponto;
    retangulo_t ret;
    ret = tela_texto_contorno(canto00, 50, "Desistir");
    ponto.x = tamtela.largura - tamtela.largura / 32 - ret.tamanho.largura;
    ponto.y = tamtela.altura / 32 + ret.tamanho.altura;
    ret = tela_texto_contorno(ponto, 50, "Desistir");
    return ret;
}


//funcoes pra arquivos de recordes e de tabuleiros já jogados
bool marca_como_jogado(int id) { // coloca um id novo no arquivo de tabuleiros ja jogados

    FILE arqr; // FILE pra ler
    FILE arqw; // FILE pra escrever (só p n confundir msm)
    int quant; // quantidade de ids que já tem no arquivo
    char lixo; // só pra ler os \n
    int* p; // pra salvar os ids (dessa vez realmente usei pq precisei, não tinha como fazer com vetores[até tem mas era mais trabalho e ia ficar feio])

    char nome[] = "re.txt";
    arqr = fopen(nome, "r");
    if (arqr == NULL) {
        quant = 0;
    }
    else {
        fscanf(arqr, "%d%c", &quant, &lixo); // ve a quantidade de tab que ja tem no arquivo
        p = malloc(quant * sizeof(int));
        if (p == NULL) {
            printf("deu merda pra alocar memoria na marca_como_jogada chefe");
            return false;
        }

        for (int i = 0; i < quant; i++) fscanf(arqr, "%d%c", &p[i], &lixo);

        fclose(arqr);
    }

    arqw = fopen(nome, "w");
    if (arqw == NULL) {
        printf("Não deu pra abrir/criar o arquivo de jogos ja jogados pra escrita no marca_como_jogado patrão");
        return false;
    }

    fprintf(arqw, "%d\n%d\n", quant + 1, id); // escreve o numero de tab jogados e o novo id
    for (int i = 0; i < quant; i++) fprintf(arqw, "%d\n", p[i]); // reescreve os outros tab

    fclose(arqw);
    if (arqr != NULL) free(p);
    return true;
}

bool ja_jogou(int id) { // verifica se um tabuleiro já foi jogado

    FILE* arq;
    char nome[] = "jogados.txt";
    int quant;
    char lixo;

    arq = fopen(nome, "r");
    if (arq == NULL) {
        return false;
    }

    fscanf(arq, "%d", &quant);
    fscanf(arq, "%c", &lixo);
    int ids[quant];
    for (int i = 0; i < quant; i++) {
        fscanf(arq, "%d", &ids[i]);
        fscanf(arq, "%c", &lixo);
        if (id == ids[i]) {
            fclose(arq);
            return true;
        }
    }
    fclose(arq);
    return false;

}

int le_recordes(RECORDE** p) { //le os recordes e retorna quantos tem

    FILE* arq;
    int quant; // quantidade de recordes que já tem no arquivo
    char lixo; // só pra ler os \n

    char nome[] = "recordes.txt";

    arq = fopen(nome, "r");
    if (arq == NULL) { // não existe arquivo de recordes, logo, temos 0 recordes, entao retorna 0
        *p = malloc(sizeof(RECORDE));
        return 0;
    }
    else { // le a quantidade de recordes feitos e aloca a memoria nescessaria(é assim que escreve?)
        fscanf(arq, "%d", &quant);
        fscanf(arq, "%c", &lixo);
        *p = malloc(quant * sizeof(RECORDE));
        if (*p == NULL) {
            printf("deu merda pra alocar memoria no le_recordes cupinxa\n");
            fclose(arq);
            return -1;
        }
        for (int i = 0; i < quant; i++) fscanf(arq, "%d %s %d %d%c", &(*p)[i].colocacao, (*p)[i].nome, &(*p)[i].id, &(*p)[i].pontos, &lixo);
        fclose(arq);
    }
    return quant;
}

bool reescrever_arq_recordes(RECORDE** p, int quant) {  // adivinha? retorna se deu certo
    FILE* arq;
    char nome[] = "recordes.txt";

    arq = fopen(nome, "w");
    if (arq == NULL) {
        printf("cara, assim, deu erro p abrir o recordes.txt pra escrever no atualiza_recordes");
        return false;
    }
    fprintf(arq, "%d\n", quant);
    for (int i = 0; i < quant; i++) {
        fprintf(arq, "%d %s %d %d\n", (*p)[i].colocacao, (*p)[i].nome, (*p)[i].id, (*p)[i].pontos);
    }
    fclose(arq);
    return true;
}

bool atualiza_recordes(RECORDE** p, RECORDE* novo, int quant) { // pega um recorde novo e bota ele no arquivo de recordes; retorna se deu bom ou não

    bool primeiro = true; // pra ver se o novo e o primeiro lugar
    RECORDE* temp = realloc(*p, (quant + 1) * sizeof(RECORDE));
    if (temp == NULL) {
        printf("Erro ao realocar memória em atualiza_recordes meu rei\n");
        return false;
    }
    *p = temp;

    for (int i = 0; i < quant; i++) { // verifica onde o novo recorde se encaixa
        if (novo->pontos > (*p)[quant - i - 1].pontos) {
            (*p)[quant - i] = (*p)[quant - i - 1];
            (*p)[quant - i].colocacao = quant - i + 1;
        }
        else {
            (*p)[quant - i] = *novo;
            (*p)[quant - i].colocacao = quant - i + 1;
            novo->colocacao = quant - i + 1;
            primeiro = false;
            break;
        }
    }
    if (primeiro) { // o novo recorde viu todos os outros e não achou nenhum maior, entao o novo e o novo primeiro lugar
        (*p)[0] = *novo;
        (*p)[0].colocacao = 1;
        novo->colocacao = 1;
    }

    if (reescrever_arq_recordes(&*p, quant + 1)) return true; // deu bom
    return false; //deu merda
}


//funções pra ler os tabuleiros
TABULEIRO ler_tabuleiro(FILE* arquivo) { //lê um tabuleiro

    TABULEIRO tab;
    char lixo;

    //ler o id e dificuldade
    fscanf(arquivo, "%d %d", &tab.id, &tab.dificuldade);
    //ler o \n no lixo só pra pular ele
    fscanf(arquivo, "%c", &lixo);
    //ler a matrizzzzz
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            fscanf(arquivo, "%d", &tab.matriz[i][j].num);
            fscanf(arquivo, "%c", &lixo);
        }
    }

    return tab;
}

int quantos_tab(FILE* arquivo) { // lê no arquivo quantos tabuleiros tem no arquivo

    int quant;
    char lixo;

    fscanf(arquivo, "%d", &quant);
    fscanf(arquivo, "%c", &lixo);

    return quant;
}

TABULEIRO configura_tabuleiro(TABULEIRO tab) { //inicializa e arruma tudo no tabuleiro pra poder começar o jogo

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            for (int k = 0; k < 9; k++) tab.matriz[i][j].marca[k] = false; //inicializa todos os marcadores como false
            tab.matriz[i][j].bloco = (j / 3) + ((i / 3) * 3) + 1; // define em qual bloco o quadrado tá, com a ordem sendo 1, 2 e 3 na linha superior e assim por diante
            if (tab.matriz[i][j].num != 0) { //se tem um número no quadrado, ele marca como 2 (preenchido pelo jogo), se não ele define como 0 (vazio)
                tab.matriz[i][j].estado = 2;
            }
            else tab.matriz[i][j].estado = 0;
        }
    }
    return tab;
}

TABULEIRO escolhe_jogo(void) { // lê o arquivo de tabuleiros e escolhe um que ainda não foi jogado(n é aleatorio)

    TABULEIRO tabmain;
    FILE* arquivo;
    char nome[] = "tabuleiros.txt";

    arquivo = fopen(nome, "r");
    if (arquivo == NULL) {
        printf("Não foi possível abrir o arquivo de tabuleiros para leitura\n");
        tabmain.certo = 0;
        return tabmain;
    }

    int quant = quantos_tab(arquivo);
    TABULEIRO* p = malloc(quant * sizeof(TABULEIRO));

    if (p == NULL) {
        printf("Não foi possível alocar memória meu camarada");
        tabmain.certo = 0;
        return tabmain;
    }

    for (int i = 0; i < quant;) { // le os arquivos, se ja foi jogado, ele ignora e pega outro no lugar
        p[i] = ler_tabuleiro(arquivo);
        p[i].num = i;
        if (ja_jogou(p[i].id)) {
            quant -= 1;
        }
        else i++;
    }

    fclose(arquivo);

    if (quant == 0) {
        printf("acabaram os tabuleiros não jogados, que nem o cara ali em baixo vai falar\n");
        tabmain.certo = 0;
        return tabmain;
    }

    tabmain = configura_tabuleiro(p[aleatorio_entre(0, quant)]);
    tabmain.certo = 1;
    marca_como_jogado(tabmain.id);
    free(p);
    return tabmain;

}


// funcões que desenham
void desenha_botão_desistir(PALETA cores) { //
    //desenha o botão de desistir(não consigo botar o // ali em cima n sei pq)
    ponto_t ponto;
    retangulo_t ret;
    ret = tela_texto_contorno(canto00, 50, "Desistir");

    ponto.x = tamtela.largura - tamtela.largura / 32 - ret.tamanho.largura;
    ponto.y = tamtela.altura / 32 + ret.tamanho.altura;
    tela_texto(ponto, 50, cores.principal, "Desistir");

}

void desenha_mouse(PALETA cores, rato_t mouse) { //desenha o mouse
    circulo_t circulo0 = { mouse.posicao, 5 };
    tela_circulo(circulo0, 3, cores.fundoinversa, cores.principal);
}

void desenha_fundo(PALETA cores) { // desenha o fundo da tela

    retangulo_t fundo = { canto00, tamtela };
    tela_retangulo(fundo, 16, cores.secundaria, cores.fundo);
    tela_retangulo(fundo, 8, cores.principal, transparente);

}

void desenha_telainicio(PALETA cores, rato_t mouse) { // desenha a tela de inicio

    //fundo
    desenha_fundo(cores);

    //titulo
    ponto_t titulopos = { 50, 120 };
    tela_texto(titulopos, 100, cores.principal, "SUDOKU");

    //botão de jogar
    retangulo_t retbotjogar = retangulo_jogar();
    //desenha o retangulo
    tela_retangulo(retbotjogar, 4, cores.secundaria, cores.principal);
    //ajeitada
    ponto_t coordtxt;
    coordtxt.x = retbotjogar.inicio.x + 16;
    coordtxt.y = retbotjogar.inicio.y - 8 + retbotjogar.tamanho.altura;
    //desenha o texto
    tela_texto(coordtxt, TAMANHO_TEXTO_JOGAR, cores.secundaria, "JOGAR");

    //mouse
    desenha_mouse(cores, mouse);

    tela_atualiza();

}

void desenha_colunas(PALETA* cores) { // desenha as colunas do tabuleiro
    //inicializa os pontos pra desenhar as linhas verticais
    ponto_t ponto_colunas;
    ponto_t ponto_cfim;
    ponto_colunas.y = posicao_tab.inicio.y;
    ponto_cfim.y = posicao_tab.inicio.y + posicao_tab.tamanho.altura;

    for (int i = 0; i < 8; i++) { // desenha as linhas verticais

        ponto_colunas.x = posicao_tab.inicio.x + ((i + 1) * (posicao_tab.tamanho.largura / 9));
        ponto_cfim.x = ponto_colunas.x;
        if ((i + 1) % 3 == 0) {
            tela_linha(ponto_colunas, ponto_cfim, 4, cores->fundoinversa);
        }
        else tela_linha(ponto_colunas, ponto_cfim, 2, cores->fundoinversa);

    }
}

void desenha_linhas(PALETA* cores) { // desenha as linhas do tabuleiro
    //inicializa os pontos pra desenhar as linhas horizontais    
    ponto_t ponto_linhas;
    ponto_t ponto_lfim;
    ponto_linhas.x = posicao_tab.inicio.x;
    ponto_lfim.x = posicao_tab.inicio.x + posicao_tab.tamanho.largura;

    for (int i = 0; i < 8; i++) { // desenha as linhas horizontais

        ponto_linhas.y = posicao_tab.inicio.y + ((i + 1) * (posicao_tab.tamanho.altura / 9));
        ponto_lfim.y = ponto_linhas.y;
        if ((i + 1) % 3 == 0) {
            tela_linha(ponto_linhas, ponto_lfim, 4, cores->fundoinversa);
        }
        else tela_linha(ponto_linhas, ponto_lfim, 2, cores->fundoinversa);

    }
}

void desenha_grade(PALETA cores) { // desenha o tabuleiro vazio

    tela_retangulo(posicao_tab, 4, cores.fundoinversa, transparente);

    desenha_linhas(&cores);

    desenha_colunas(&cores);

}

void desenha_marcacoes(PALETA cores, bool marcas[9], ponto_t inicio, double espacamento) { // desenha as marcacoes de uma determinada casa

    ponto_t ponto;
    char c[2] = "x";
    for (int i = 1; i < 10; i++) {
        if (marcas[i - 1]) {
            ponto.x = inicio.x + ((espacamento / 3) * ((i - 1) % 3));
            ponto.y = inicio.y + ((espacamento / 3) * (1 + (i - 1) / 3));
            sprintf(c, "%d", i);
            tela_texto(ponto, 15, cores.secundaria, c);
        }
    }
}

void desenha_numeros(PALETA cores, TABULEIRO tab, int sel) { // desenha todos os números marcados no tabuleiro

    double espacamento = posicao_tab.tamanho.largura / 9;
    ponto_t ponto;
    char c[2] = "x";

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {

            if (tab.matriz[i][j].estado == 2) {
                ponto.x = posicao_tab.inicio.x + (espacamento * j) + (espacamento / 4);
                ponto.y = posicao_tab.inicio.y + (espacamento * (1 + i)) - (espacamento / 4);
                sprintf(c, "%d", tab.matriz[i][j].num);
                tela_texto(ponto, 40, cores.fundoinversa, c);
            }
            else if (tab.matriz[i][j].estado == 1) {
                ponto.x = posicao_tab.inicio.x + (espacamento * j) + (espacamento / 4);
                ponto.y = posicao_tab.inicio.y + (espacamento * (1 + i)) - (espacamento / 4);
                sprintf(c, "%d", tab.matriz[i][j].num);
                tela_texto(ponto, 40, cores.principal, c);
            }
            else if (tab.matriz[i][j].estado == 0) {
                ponto.x = posicao_tab.inicio.x + (espacamento * j) + (espacamento / 18);
                ponto.y = posicao_tab.inicio.y + (espacamento * i) - (espacamento / 18);
                desenha_marcacoes(cores, tab.matriz[i][j].marca, ponto, espacamento);
            }
        }
    }

}

void desenha_tabuleiro(PALETA cores, JOGO* j) { // desenha o tabuleiro preenchido

    desenha_grade(cores);
    desenha_numeros(cores, j->tabuleiro, j->selecionado);

}

void desenha_relogio(PALETA cores, int tempo) { // desenha a quanto tempo tu ta jogando no canto superior eesquerdo

    ponto_t ponto;
    char relogio[6];

    relogio[0] = ((tempo / 60) / 10) + 48;
    relogio[1] = ((tempo / 60) % 10) + 48;
    relogio[2] = 58;
    relogio[3] = ((tempo % 60) / 10) + 48;
    relogio[4] = ((tempo % 60) % 10) + 48;
    relogio[5] = '\0';

    ponto.x = tamtela.largura / 32;
    ponto.y = tamtela.altura / 32 + tela_texto_contorno(canto00, 50, "00:00").tamanho.altura;
    tela_texto(ponto, 50, cores.principal, relogio);

}

void desenha_selecionado(PALETA cores, int n) { // desenha qual número está selecionado pra ser jogado

    char c[2] = "X";
    sprintf(c, "%d", n);
    ponto_t ponto;
    retangulo_t ret = tela_texto_contorno(canto00, 100, "0");
    ponto.y = (tamtela.altura / 2) - (ret.tamanho.altura / 2);
    ponto.x = (tamtela.largura / 16) - (ret.tamanho.largura / 2);

    tela_texto(ponto, 100, cores.principal, c);

}

void desenha_erro(PALETA cores, bool erro) { // desenha uma mensagem de erro caso o jogador tente fazer uma jogada que não pode

    char c[] = "ERRO: voce nao pode jogar ai";
    retangulo_t ret = tela_texto_contorno(canto00, 40, c);
    ponto_t pos = { (TELA_X / 2) - (ret.tamanho.largura / 2), ((TELA_Y - posicao_tab.tamanho.altura) / 2) - (ret.tamanho.altura / 2) };
    tela_texto(pos, 40, cores.secundaria, c);

}

void desenha_jogo(PALETA cores, JOGO j) { //desenha o jogo

    desenha_fundo(cores);
    desenha_tabuleiro(cores, &j);
    desenha_relogio(cores, j.tempo_de_jogo);
    desenha_selecionado(cores, j.selecionado);
    desenha_botão_desistir(cores);
    desenha_mouse(cores, j.mouse);
    if (j.erro) desenha_erro(cores, j.erro);
    tela_atualiza();

}

void desenha_pontos(PALETA* cores, char nome[4], int pontos) { // desenha a tela de pontos

    int espacamentoy = tamtela.altura / 20;
    char msg[100];
    sprintf(msg, "Parabens! Voce fez %d pontos!", pontos);

    retangulo_t retmsg1 = tela_texto_contorno(centro, 60, msg);
    retangulo_t retmsg2 = tela_texto_contorno(centro, 50, "Digite seu nome:");
    retangulo_t retnome = tela_texto_contorno(centro, 50, "AAA");

    retmsg1.inicio.x = retmsg1.inicio.x - (retmsg1.tamanho.largura / 2);
    retmsg1.inicio.y = centro.y - retmsg2.tamanho.altura - espacamentoy;
    retmsg2.inicio.x = retmsg2.inicio.x - (retmsg2.tamanho.largura / 2);
    retnome.inicio.x = retnome.inicio.x - (retnome.tamanho.largura / 2);
    retnome.inicio.y = centro.y + retnome.tamanho.altura + espacamentoy;

    desenha_fundo(*cores);
    tela_texto(retmsg1.inicio, 60, cores->fundoinversa, msg);
    tela_texto(retmsg2.inicio, 50, cores->fundoinversa, "Digite seu nome:");
    tela_texto(retnome.inicio, 50, cores->principal, nome);

}

void desenha_nome(cor_t cor, int pos, int n, RECORDE p) { //desenha um recorde em uma posicao selecionada
    int espacamento = tamtela.altura / 32;
    char msg[100];
    sprintf(msg, "%do lugar: %s com %d pontos", n, p.nome, p.pontos);
    retangulo_t retrank = tela_texto_contorno(centro, 40, msg);
    retrank.inicio.x = centro.x - (retrank.tamanho.largura / 2);

    switch (pos) {
    case 1:
        retrank.inicio.y = centro.y - retrank.tamanho.altura - espacamento;
        sprintf(msg, "%do lugar: %s com %d pontos", n, p.nome, p.pontos);
        tela_texto(retrank.inicio, 40, cor, msg);
        break;
    case 2:
        retrank.inicio.y = centro.y;
        sprintf(msg, "%do lugar: %s com %d pontos", n, p.nome, p.pontos);
        tela_texto(retrank.inicio, 40, cor, msg);
        break;
    case 3:
        retrank.inicio.y = centro.y + retrank.tamanho.altura + espacamento;
        sprintf(msg, "%do lugar: %s com %d pontos", n, p.nome, p.pontos);
        tela_texto(retrank.inicio, 40, cor, msg);
        break;
    }

}

void desenha_rank(PALETA* cores, int n, int quant, RECORDE* p) { // desenha a tela de ranking

    char msg[100];
    sprintf(msg, "Voce ficou na %da posicao do ranking!", n);
    retangulo_t retcolocacao = tela_texto_contorno(canto00, 50, msg);

    retcolocacao.inicio.x = centro.x - (retcolocacao.tamanho.largura / 2);
    retcolocacao.inicio.y = tamtela.altura / 8;

    tela_texto(retcolocacao.inicio, 50, cores->fundoinversa, msg);

    if (n == quant && quant > 1) { // ficou em ultimo
        desenha_nome(cores->secundaria, 1, n + 1, p[n - 2]);
        desenha_nome(cores->principal, 2, n, p[n - 1]);
    }
    else if (n == 1 && quant > 1) { // ficou em primeiro
        desenha_nome(cores->principal, 1, n, p[n - 1]);
        desenha_nome(cores->secundaria, 2, n + 1, p[n]);
    }
    else if (n == 1 && quant == 1) { // foi o primeiro recorde registrado
        desenha_nome(cores->principal, 1, n, p[n - 1]);
    }
    else {// qualquer outro caso
        desenha_nome(cores->secundaria, 1, n - 1, p[n - 2]);
        desenha_nome(cores->principal, 2, n, p[n - 1]);
        desenha_nome(cores->secundaria, 3, n + 1, p[n]);
    }
}

void desenha_de_novo(PALETA* cores, retangulo_t sim, retangulo_t nao, retangulo_t msg) { // desenha a tela de "quer jogar de novo?"

    tela_texto(msg.inicio, 40, cores->fundoinversa, "Quer jogar de novo?");
    tela_texto(sim.inicio, 70, cores->principal, "SIM");
    tela_texto(nao.inicio, 70, cores->principal, "NAO");

}


//funções pras jogadas e pro jogo funcionarr
bool tem_igual(TABULEIRO tab, int i, int j, int n, int bloco) { // vê se tem algum número igual na linha, coluna ou bloco 3x3

    //verifica o bloco 3x3
    int x0 = (bloco % 3) * 3;
    int y0 = (bloco / 3) * 3;
    for (int x = x0; x < x0 + 3; x++) {
        for (int y = y0; y < y0 + 3; y++) {
            if (tab.matriz[y][x].num == n) return true;
        }
    }

    // verifica linhas e colunas
    for (int k = 0; k < 9; k++) {
        if (n == tab.matriz[k][j].num) return true;
        if (n == tab.matriz[i][k].num) return true;
    }

    return false;
}

bool pode_jogar(JOGO* jogo, int i, int j) { //vê se a jogada que o jogador quer fazer é possivel
    if (jogo->selecionado == 0) {
        if (jogo->tabuleiro.matriz[i][j].estado == 2) {
            jogo->erro = true;
            return false;
        }
    }
    if (jogo->selecionado != 0) {
        if (jogo->tabuleiro.matriz[i][j].estado == 2) {
            jogo->erro = true;
            return false;
        }
        if (tem_igual(jogo->tabuleiro, i, j, jogo->selecionado, jogo->tabuleiro.matriz[i][j].bloco - 1)) {
            jogo->erro = true;
            return false;
        }
    }
    jogo->erro = false;
    return true;
}

void faz_jogada(JOGO* jogo, int i, int j) { // proscessa a jogada

    //ve qual botão foi apertado e o que tem q fazer
    if (jogo->mouse.clicado[0]) {
        if (jogo->selecionado == 0 && pode_jogar(jogo, i, j)) {
            jogo->tabuleiro.matriz[i][j].estado = 0;
            jogo->tabuleiro.matriz[i][j].num = 0;
        }
        else if (jogo->selecionado != 0 && pode_jogar(jogo, i, j)) {
            jogo->tabuleiro.matriz[i][j].estado = 1;
            jogo->tabuleiro.matriz[i][j].num = jogo->selecionado;
        }
    }
    else if (jogo->mouse.clicado[1]) {
        if (jogo->selecionado == 0) {
            for (int k = 0; k < 9; k++) {
                jogo->tabuleiro.matriz[i][j].marca[k] = false;
            }
        }
        else {
            jogo->tabuleiro.matriz[i][j].marca[jogo->selecionado - 1] = !jogo->tabuleiro.matriz[i][j].marca[jogo->selecionado - 1];
        }
    }

}

void proscessa_botoes(JOGO* jogo) { // vê se o botão de desistir foi clicado ou se o jogador fez uma jogada

    int i;
    int j;

    // vê se o mouse ta dentro do tabuleiro
    if (jogo->mouse.posicao.x > posicao_tab.inicio.x && jogo->mouse.posicao.x < posicao_tab.inicio.x + posicao_tab.tamanho.largura) {
        if (jogo->mouse.posicao.y > posicao_tab.inicio.y && jogo->mouse.posicao.y < posicao_tab.inicio.y + posicao_tab.tamanho.altura) {

            //descobre em que casa o mouse ta
            j = (jogo->mouse.posicao.x - ((TELA_X - posicao_tab.tamanho.largura) / 2)) / (posicao_tab.tamanho.largura / 9);
            i = (jogo->mouse.posicao.y - ((TELA_Y - posicao_tab.tamanho.altura) / 2)) / (posicao_tab.tamanho.altura / 9);

            faz_jogada(jogo, i, j);
        }
    }
    else {
        if (jogo->mouse.clicado[0] || jogo->mouse.apertado[0]) { // vê se o botão esquerdo do mouse foi clicado
            retangulo_t ret = retangulo_desistir();
            //vê se o mouse clicado ta em cima do botão de desistir
            if (jogo->mouse.posicao.x > ret.inicio.x && jogo->mouse.posicao.x < ret.inicio.x + ret.tamanho.largura) {
                if (jogo->mouse.posicao.y > ret.inicio.y && jogo->mouse.posicao.y < ret.inicio.y + ret.tamanho.altura) {
                    jogo->desistiu = true;
                }
            }
        }
    }
}

void proscessa_rodinha(JOGO* j) { // vê a rodinha do mouse e muda o estado do jogo

    int n = j->mouse.giro;

    if (j->selecionado - n < 0) {
        j->selecionado = 0;
    }
    else if (j->selecionado - n > 9) {
        j->selecionado = 9;
    }
    else {
        j->selecionado -= n;
    }

}

bool ganhou(JOGO jogo) { // vê se o jogador ganhou

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (jogo.tabuleiro.matriz[i][j].estado == 0) return false;
        }
    }
    return true;
}

void proscessa_tempo(JOGO* j) { // ve o tempo
    j->tempo_de_jogo = tela_relogio() - j->data_inicio;
}


// funções principais
void iniciar(PALETA* pmain) { // tela inical com "jogar", "placar", etc

    PALETA paleta1 = { ciano, branco, cinza, preto };
    PALETA paleta2 = { laranja, branco, ciano, preto };
    PALETA paleta3 = { vermelho, branco, azul, preto };
    PALETA paleta4 = { fucsia, preto, amarelo, branco };

    *pmain = paleta1;

    tela_inicio(tamtela, "SUDOKU");

    //obter o retangulo de jogar pra saber onde o mouse pode clicar
    retangulo_t botjogar;
    botjogar = retangulo_jogar();
    rato_t mouse;
    int c;

    while (true) {

        //seletor de paleta pro jogo
        mouse = tela_rato();
        c = tela_tecla();
        switch (c) {
        case '1':
            *pmain = paleta1;
            break;

        case '2':
            *pmain = paleta2;
            break;

        case '3':
            *pmain = paleta3;
            break;

        case '4':
            *pmain = paleta4;

        default:
            break;
        }

        desenha_telainicio(*pmain, mouse);

        //ver se o botão de jogar foi clicado
        if (mouse.posicao.x > botjogar.inicio.x && mouse.posicao.x < botjogar.inicio.x + botjogar.tamanho.largura) {
            if (mouse.posicao.y > botjogar.inicio.y && mouse.posicao.y < botjogar.inicio.y + botjogar.tamanho.altura) {
                if (mouse.clicado[0] || mouse.apertado[0]) {
                    break;
                }
            }
        }
    }
}

JOGO jogar(PALETA cores) { //pra começar o jogo e jogarrr

    JOGO j;

    j.tabuleiro = escolhe_jogo();
    if (j.tabuleiro.certo == 0) {
        tela_fim();
        printf("rolou algum erro no escolhe_jogo, provavelmente não tem mais tabuleiros não jogados que nem o cara ali em cima talvez tenha dito\n");
        j.terminou = false;
        return j;
    }

    //inicializa umas variaveis
    j.terminou = false;
    j.data_inicio = tela_relogio();
    j.tempo_de_jogo = 0;
    j.mouse = tela_rato();
    j.selecionado = 0;
    j.desistiu = false;
    j.erro = false;

    while (!j.terminou) {
        j.mouse = tela_rato();
        proscessa_rodinha(&j);
        proscessa_botoes(&j);
        proscessa_tempo(&j);
        desenha_jogo(cores, j);

        if (ganhou(j)) j.terminou = true;

        if (j.desistiu) break;
    }

    return j;

}

void fechar(PALETA* cores) { // fecha o jogo com a telinha de 'até mais'

    char c;

    desenha_fundo(*cores);
    retangulo_t ret0 = tela_texto_contorno(canto00, 100, "Ate mais!");
    retangulo_t ret1 = tela_texto_contorno(canto00, 50, "[pressione ENTER para fechar]");
    ret0.inicio.x = centro.x - (ret0.tamanho.largura / 2);
    ret0.inicio.y = centro.y;
    ret1.inicio.x = centro.x - (ret1.tamanho.largura / 2);
    ret1.inicio.y = centro.y + ret1.tamanho.altura;
    tela_texto(ret0.inicio, 100, cores->principal, "Ate mais!");
    tela_texto(ret1.inicio, 50, cores->secundaria, "[pressione ENTER para fechar]");
    tela_atualiza();

    while (true) {
        c = tela_tecla();
        if (c == '\n') break;
    }

    tela_fim();

}

RECORDE apresentar_pontos(PALETA* cores, JOGO* j) {

    RECORDE novo;
    int pontos = 1285 + 97418 * (j->tabuleiro.dificuldade + 1) / j->tempo_de_jogo;
    int i = 0;
    char c;
    char nome[4] = "_";
    nome[3] = '\0';
    novo.nome[3] = '\0';

    while (true) {
        c = tela_tecla();
        if (c != '\n' && c != '\0') { // ler o nome do individuo
            if (c == '\b') {
                if (i > 0 && nome[2] == '_') i--;
                nome[i] = '_';
            }
            else {
                nome[i] = c;
                if (i < 2) i++;
            }
        }
        else if (c == '\n' && nome[2] != '_') break; // deu enter e o nome foi preenchido

        desenha_pontos(cores, nome, pontos);
        tela_atualiza();
    }
    novo.id = j->tabuleiro.id;
    for (int i = 0; i < 3; i++) {
        novo.nome[i] = nome[i];
    }
    //strcpy(nome, novo.nome);
    novo.pontos = pontos;
    return novo;
}

void mostra_rank(PALETA* cores, int n, int quant, RECORDE* p) { // tela que mostra o rank do jogador

    char c;

    while (true) {
        c = tela_tecla();
        if (c == '\n') {
            break;
        }
        desenha_fundo(*cores);
        desenha_rank(cores, n, quant, p);
        tela_atualiza();
    }

}

bool jogar_de_novo(PALETA* cores) { // tela que pergunta se o jogador quer jogar de novo

    rato_t mouse;
    int espacamentox = tamtela.largura / 32;
    retangulo_t sim = tela_texto_contorno(centro, 70, "SIM");
    retangulo_t nao = tela_texto_contorno(centro, 70, "NAO");
    retangulo_t msg = tela_texto_contorno(centro, 40, "Quer jogar de novo?");

    sim.inicio.x = centro.x - sim.tamanho.largura - espacamentox;
    nao.inicio.x = centro.x + espacamentox;
    sim.inicio.y = centro.y + (sim.tamanho.altura / 2);
    nao.inicio.y = centro.y + (nao.tamanho.altura / 2);
    msg.inicio.x = centro.x - (msg.tamanho.largura / 2);
    msg.inicio.y = centro.y - sim.tamanho.altura;

    while (true) {
        mouse = tela_rato();
        desenha_fundo(*cores);
        desenha_de_novo(cores, sim, nao, msg);
        desenha_mouse(*cores, mouse);
        if (mouse.clicado[0]) {
            if (mouse.posicao.y < sim.inicio.y && mouse.posicao.y > sim.inicio.y - sim.tamanho.altura) {
                if (mouse.posicao.x > sim.inicio.x && mouse.posicao.x < sim.inicio.x + sim.tamanho.largura) return true;
                if (mouse.posicao.x > nao.inicio.x && mouse.posicao.x < nao.inicio.x + nao.tamanho.largura) return false;
            }
        }
        tela_atualiza();
    }
    return false;
}

int main() {

    PALETA cores;

    iniciar(&cores);
    JOGO j;
    RECORDE* p;
    RECORDE novos;
    int nrec = le_recordes(&p);

    if (nrec == -1) { // verifica se deu certo o le_recordes
        tela_fim();
        free(p);
        return 0;
    }

    while (true) {
        j = jogar(cores);
        if (!j.terminou) { //if pra se o jogador n ganhou. Isso acontece se o jogador desistir ou não tiverem mais tabuleiros disponiveis (ou algum erro aleatorio tbm);
            free(p);
            fechar(&cores);
            return 0;
        }

        novos = apresentar_pontos(&cores, &j);
        atualiza_recordes(&p, &novos, nrec);
        nrec++;
        mostra_rank(&cores, novos.colocacao, nrec, &*p);
        if (!jogar_de_novo(&cores)) break;
    }

    fechar(&cores);

    tela_fim();

}