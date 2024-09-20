/*
Dimensões:
    tela 500x500
    grid 300x300 centralizado (margens de 100px)

    Letras tela final:
        Em caso de velha - largura: 72px 
                           altura: 100px
                           (margens de 50px para os lados e acima e espaçamento de 10px)
        Em caso de vitória - largura: 46px
                             altura: 64px
                             (margens de 50px para os lados e acima e espaçamento de 4px)

Coordenadas:
    Canto superior esquerdo: 100,100
    Canto superior direito: 400,100
    Canto inferior esquerdo: 100,400
    Canto inferior direito: 400,400

    Linha vertical 1 - superior: 200,100
    Linha vertical 1 - inferior: 200,400
    Linha vertical 2 - superior: 300,100
    Linha vertical 2 - inferior: 300,400

    Linha horizontal 1 - esquerda: 100,200
    Linha horizontal 1 - direita: 400,200
    Linha horizontal 2 - esquerda: 100,300
    Linha horizontal 2 - direita: 400,300

    Cruzamentos entre linhas:
        Cruzamento superior esquerdo: 200,200
        Cruzamento superior direito: 300,200
        Cruzamento inferior esquerdo: 200,300
        Cruzamento inferior direito: 300,300

    Posições do grid:
        Posição 0: 150,150
        Posição 1: 250,150
        Posição 2: 350,150
        Posição 3: 150,250
        Posição 4: 250,250
        Posição 5: 350,250
        Posição 6: 150,350
        Posição 7: 250,350
        Posição 8: 350,350
*/

#include <stdio.h>
#include <stdbool.h>
#include "telag.h"

//Definindo tipo de variável que representa estado do jogo
typedef struct
{
    char player;
    char winner;

    bool match_end;

    int position;
    int line;
    int column;

    int menu_index;
    char menu_selected;

    int number_of_plays;

} gamestate;

char lines(char matrix[3][3]);
char columns(char matrix[3][3]);
char diagonals(char matrix[3][3]);
char who_won(char matrix[3][3], gamestate* pointer);
void play(gamestate* pointer);
void draw_and_move_game(char grid[3][3], gamestate* pointer);
void final_screen(gamestate* pointer);
void draw_tie(void);
void draw_winner_screen(char winner);
void start_screen(gamestate* pointer);
void move_menu(gamestate* pointer);

int main(void)
{
	//Declaração de variáveis que representam o estado do jogo
    gamestate game;
    gamestate* pointer = &game;

	//Inicialização do programa
    tela_inicio(500, 500, "Benhur's Game 1");

	//Laço principal, onde a partida é jogada
    while (true)
    {
        //Inicializando o gamestate no começo do laço para garantir que o programa comece do zero todas as vezes
        game.player = ' ';
        game.winner = ' ';
        game.match_end = false;
        game.position = 0;
        game.line = 0;
        game.column = 0;
        game.menu_index = 0;
        game.menu_selected = ' ';
        game.number_of_plays = 0;

        start_screen(pointer);

        if (game.menu_selected == 'J')
        {
            play(pointer);
            final_screen(pointer);
        }
        else
        {
            break;
        }
    }
	
    //Finalização do programa
    tela_fim();
}

char lines(char matrix[3][3])
{
    for (int i = 0; i < 3; i++)
    {
        if (matrix[i][0] == matrix[i][1] && matrix[i][1] == matrix[i][2])
        {
            return matrix[i][0];
        }
    }

    return ' ';
}

char columns(char matrix[3][3])
{
    for (int i = 0; i < 3; i++)
    {
        if (matrix[0][i] == matrix[1][i] && matrix[1][i] == matrix[2][i])
        {
            return matrix[0][i];
        }
    }

    return ' ';
}

char diagonals(char matrix[3][3])
{
    if (matrix[0][0] == matrix[1][1] && matrix[1][1] == matrix[2][2])
    {
        return matrix[0][0];
    }
    else if (matrix[0][2] == matrix[1][1] && matrix[1][1] == matrix[2][0])
    {
        return matrix[0][2];
    }

    return ' ';
}

char who_won(char matrix[3][3], gamestate *pointer)
{
    char victorious[3] = {lines(matrix), columns(matrix), diagonals(matrix)};

    for (int i = 0; i < 3; i++)
    {
        if (victorious[i] != ' ')
        {
            pointer->winner = victorious[i];
            pointer->match_end = true;

            return victorious[i];
        }
    }

    pointer->winner = 'E';

    return 'E';
}

void play(gamestate *pointer)
{
    char grid[3][3] = { {' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '} };
    
    tela_atualiza();

    for(int i = 0; i < 10; i++)
    {
        if (i % 2 == 0)
        {
            pointer->player = 'X';
        }
        else
        {
            pointer->player = 'O';
        }

        draw_and_move_game(grid, pointer);

        pointer->winner = who_won(grid, pointer);

        if (pointer->match_end == true)
        {
            break;
        }
    }
}

void draw_and_move_game(char grid[3][3], gamestate *pointer)
{
    //Inicializa uma string para cada posição do grid - da esquerda para a direita, de cima para baixo - para poder printar na tela
    char string_grid[9][4] =
    {
        {"   "},
        {"   "},
        {"   "},
        {"   "},
        {"   "},
        {"   "},
        {"   "},
        {"   "},
        {"   "},
    };
    //Variável que recebe a movimentação
    char move = ' ';
    
    int player_color;

    //Faz a atribuição dos elementos do grid para as strings
    string_grid[0][1] = grid[0][0];
    string_grid[1][1] = grid[0][1];
    string_grid[2][1] = grid[0][2];
    string_grid[3][1] = grid[1][0];
    string_grid[4][1] = grid[1][1];
    string_grid[5][1] = grid[1][2];
    string_grid[6][1] = grid[2][0];
    string_grid[7][1] = grid[2][1];
    string_grid[8][1] = grid[2][2];

    while (true)
    {
        //Pinta a posição em que o cursor está
        switch (pointer->position)
        {
        case 0:
            tela_retangulo(100, 100, 200, 200, 0, transparente, amarelo);
            break;

        case 1:
            tela_retangulo(200, 100, 300, 200, 0, transparente, amarelo);
            break;

        case 2:
            tela_retangulo(300, 100, 400, 200, 0, transparente, amarelo);
            break;

        case 3:
            tela_retangulo(100, 200, 200, 300, 0, transparente, amarelo);
            break;

        case 4:
            tela_retangulo(200, 200, 300, 300, 0, transparente, amarelo);
            break;

        case 5:
            tela_retangulo(300, 200, 400, 300, 0, transparente, amarelo);
            break;

        case 6:
            tela_retangulo(100, 300, 200, 400, 0, transparente, amarelo);
            break;

        case 7:
            tela_retangulo(200, 300, 300, 400, 0, transparente, amarelo);
            break;

        case 8:
            tela_retangulo(300, 300, 400, 400, 0, transparente, amarelo);
            break;
        }

        //Desenha o grid em si
        tela_retangulo(100, 100, 400, 400, 2, branco, transparente);
        tela_linha(200, 100, 200, 400, 2, branco);
        tela_linha(300, 100, 300, 400, 2, branco);
        tela_linha(100, 200, 400, 200, 2, branco);
        tela_linha(100, 300, 400, 300, 2, branco);

        //Preenche o grid com os elementos
        if (string_grid[0][1] == 'X')
        {
            player_color = vermelho;
        }
        else
        {
            player_color = azul;
        }
        tela_texto(150, 150, 50, player_color, string_grid[0]);

        if (string_grid[1][1] == 'X')
        {
            player_color = vermelho;
        }
        else
        {
            player_color = azul;
        }
        tela_texto(250, 150, 50, player_color, string_grid[1]);

        if (string_grid[2][1] == 'X')
        {
            player_color = vermelho;
        }
        else
        {
            player_color = azul;
        }
        tela_texto(350, 150, 50, player_color, string_grid[2]);

        if (string_grid[3][1] == 'X')
        {
            player_color = vermelho;
        }
        else
        {
            player_color = azul;
        }
        tela_texto(150, 250, 50, player_color, string_grid[3]);

        if (string_grid[4][1] == 'X')
        {
            player_color = vermelho;
        }
        else
        {
            player_color = azul;
        }
        tela_texto(250, 250, 50, player_color, string_grid[4]);

        if (string_grid[5][1] == 'X')
        {
            player_color = vermelho;
        }
        else
        {
            player_color = azul;
        }
        tela_texto(350, 250, 50, player_color, string_grid[5]);

        if (string_grid[6][1] == 'X')
        {
            player_color = vermelho;
        }
        else
        {
            player_color = azul;
        }
        tela_texto(150, 350, 50, player_color, string_grid[6]);

        if (string_grid[7][1] == 'X')
        {
            player_color = vermelho;
        }
        else
        {
            player_color = azul;
        }
        tela_texto(250, 350, 50, player_color, string_grid[7]);

        if (string_grid[8][1] == 'X')
        {
            player_color = vermelho;
        }
        else
        {
            player_color = azul;
        }
        tela_texto(350, 350, 50, player_color, string_grid[8]);
                                 
        //Atualiza a tela aqui para que o grid apareça antes de entrar no while da movimentação
        tela_atualiza();

        while (true)
        {
            move = tela_tecla();

            //Todas as atualizações de posição executam break para poder atualizar na tela a posição do cursor para o usuário
            if ((move == 'w' || move == 'W') && pointer->line != 0)
            {
                pointer->line--;
                pointer->position -= 3;
                break;
            }
            else if ((move == 's' || move == 'S') && pointer->line != 2)
            {
                pointer->line++;
                pointer->position += 3;
                break;
            }
            else if ((move == 'a' || move == 'A') && pointer->column != 0)
            {
                pointer->column--;
                pointer->position--;
                break;
            }
            else if ((move == 'd' || move == 'D') && pointer->column != 2)
            {
                pointer->column++;
                pointer->position++;
                break;
            }
            else if (move == '\n')
            {
                if (grid[pointer->line][pointer->column] == ' ')
                {
                    grid[pointer->line][pointer->column] = pointer->player;
                    pointer->number_of_plays++;
                    break;
                }
                else
                {
                    tela_texto(250, 450, 22, rosa, "Selecione outra casa!");
                    break;
                }
            }
            
            if (pointer->match_end == true || pointer->number_of_plays == 9)
            {
                pointer->match_end = true;
                break;
            }
        }

        //Condição para finalizar o while principal da função
        if (move == '\n' || pointer->match_end == true)
        {
            break;
        }
    }
}

void final_screen(gamestate* pointer)
{
    while (true)
    {
        //Decide qual resultado será exibido
        if (pointer->winner != 'E')
        {
            draw_winner_screen(pointer->winner);
        }
        else
        {
            draw_tie();
        }

        tela_atualiza();

        //Condição de finalização da tela final
        if (tela_tecla() != '\0')
        {
            break;
        }
    }
}

void draw_tie(void)
{
    //Desenha o V
    tela_linha(50, 50, 86, 151, 6, branco); //Linha esquerda
    tela_linha(122, 50, 86, 151, 6, branco); //Linha direita
    tela_retangulo(86, 151, 86, 151, 4, branco, transparente); //Junção das linhas
    tela_retangulo(49, 50, 50, 50, 4, branco, transparente); //Topo da linha esquerda
    tela_retangulo(122, 50, 123, 50, 4, branco, transparente); //Topo da linha direita

    //Desenha o E
    tela_linha(130, 51, 200, 51, 6, branco); //Linha de cima
    tela_linha(130, 150, 200, 150, 6, branco); //Linha de Baixo
    tela_linha(133, 50, 133, 150, 6, branco); //Linha vertical
    tela_linha(133, 100, 168, 100, 6, branco); //Linha do meio

    //Desenha o L
    tela_linha(211, 50, 211, 150, 6, branco); //Linha vertical
    tela_linha(208, 150, 282, 150, 6, branco); //Linha horizontal

    //Desenha o H
    tela_linha(292, 50, 292, 153, 6, branco); //Linha vertical esquerda
    tela_linha(364, 50, 364, 153, 6, branco); //Linha vertical direita
    tela_linha(292, 100, 364, 100, 6, branco); //Linha horizontal

    //Desenha o A
    tela_linha(411, 50, 375, 151, 6, branco); //Linha esquerda
    tela_linha(411, 50, 447, 151, 6, branco); //Linha direita
    tela_linha(393, 100, 429, 100, 6, branco); //Linha do meio
    tela_retangulo(374, 151, 375, 151, 4, branco, transparente); //"Pé" esquerdo
    tela_retangulo(447, 151, 448, 151, 4, branco, transparente); //"Pé" direito
    tela_retangulo(411, 50, 411, 50, 4, branco, transparente); //Topo

    tela_texto(250, 250, 20, branco, "Pressione qualquer tecla para continuar");
}

void draw_winner_screen(char winner)
{
    int winner_color = branco;

    //Desenha o V
    tela_linha(50, 50, 73, 114, 4, winner_color); //Linha esquerda
    tela_linha(96, 50, 73, 114, 4, winner_color); //Linha direita
    tela_retangulo(73, 114, 73, 114, 4, winner_color, transparente); //Junção das linhas
    tela_retangulo(50, 50, 50, 50, 4, winner_color, transparente); //Topo da linha esquerda
    tela_retangulo(96, 50, 96, 50, 4, winner_color, transparente); //Topo da linha direita

    //Desenha o E
    tela_linha(100, 50, 146, 50, 4, winner_color); //Linha de cima
    tela_linha(100, 114, 146, 114, 4, winner_color); //Linha de baixo
    tela_linha(102, 50, 102, 114, 4, winner_color); //Linha vertical
    tela_linha(100, 82, 134, 82, 4, winner_color); //Linha do meio

    //Desenha o N
    tela_linha(150, 48, 150, 116, 4, winner_color); //Linha esquerda
    tela_linha(196, 48, 196, 116, 4, winner_color); //Linha direita
    tela_linha(150, 49, 196, 115, 4, winner_color); //Linha diagonal

    //Desenha o C
    tela_linha(200, 50, 246, 50, 4, winner_color); //Linha de cima
    tela_linha(200, 114, 246, 114, 4, winner_color); //Linha de baixo
    tela_linha(202, 50, 202, 114, 4, winner_color); //Linha vertical

    //Desenha o E
    tela_linha(250, 50, 296, 50, 4, winner_color); //Linha de cima
    tela_linha(250, 114, 296, 114, 4, winner_color); //Linha de baixo
    tela_linha(252, 50, 252, 114, 4, winner_color); //Linha vertical
    tela_linha(250, 82, 284, 82, 4, winner_color); //Linha do meio

    //Desenha o D
    tela_linha(302, 50, 302, 114, 4, winner_color); //Linha vertical esquerda
    tela_linha(300, 114, 323, 114, 4, winner_color); //Linha horizontal de baixo
    tela_linha(300, 50, 323, 50, 4, winner_color); //Linha horizontal de cima
    tela_linha(323, 114, 346, 97, 4, winner_color); //Linha diagonal de baixo
    tela_linha(323, 50, 346, 67, 4, winner_color); //Linha diagonal de cima
    tela_linha(346, 65, 346, 99, 4, winner_color); //Linha vertical direita

    //Desenha o O
    tela_linha(350, 50, 396, 50, 4, winner_color); //Linha de cima
    tela_linha(350, 114, 396, 114, 4, winner_color); //Linha de baixo
    tela_linha(352, 50, 352, 114, 4, winner_color); //Linha da esquerda
    tela_linha(394, 50, 394, 114, 4, winner_color); //Linha da direita

    //Desenha o R
    tela_linha(400, 48, 400, 116, 4, winner_color); //Linha vertical esquerda
    tela_linha(400, 50, 435, 50, 4, winner_color); //Linha horizontal superior
    tela_linha(400, 82, 435, 82, 4, winner_color); //Linha horizontal inferior
    tela_linha(445, 59, 445, 73, 5, winner_color); //Linha vertical direita
    tela_linha(435, 50, 446, 60, 4, winner_color); //Linha diagonal menor superior
    tela_linha(435, 82, 446, 72, 4, winner_color); //Linha diagonal menor inferior
    tela_linha(400, 82, 446, 114, 4, winner_color); //Linha diagonal (perna)
    tela_retangulo(446, 114, 446, 114, 4, winner_color, transparente); //"Pé"

    if (winner == 'X')
    {
        winner_color = vermelho;

        //Desenha o X
        tela_linha(145, 150, 345, 400, 4, winner_color);
        tela_linha(155, 150, 355, 400, 4, winner_color);
        tela_linha(144, 150, 156, 150, 4, winner_color);
        tela_linha(344, 400, 356, 400, 4, winner_color);
        tela_linha(345, 150, 145, 400, 4, winner_color); 
        tela_linha(355, 150, 155, 400, 4, winner_color); 
        tela_linha(344, 150, 356, 150, 4, winner_color);
        tela_linha(144, 400, 156, 400, 4, winner_color);
    }
    else
    {
        winner_color = azul;

        //Desenha o O
        tela_circulo(250, 275, 125, 4, winner_color, transparente);
    }

    tela_texto(250, 450, 20, branco, "Pressione qualquer tecla para continuar");
}

void start_screen(gamestate* pointer)
{
    while(true)
    {
        tela_texto(250, 100, 50, branco, "BENHUR'S GAME 1");

        if (pointer->menu_index == 0)
        {
            tela_texto(250, 250, 25, branco, ">Jogar");
            tela_texto(250, 275, 25, branco, "Sair");
        }
        else if (pointer->menu_index == 1)
        {
            tela_texto(250, 250, 25, branco, "Jogar");
            tela_texto(250, 275, 25, branco,">Sair");
        }
        else
        {
            break;
        }

        tela_atualiza();

        move_menu(pointer);
    }
}

void move_menu(gamestate* pointer)
{
    char move = ' ';

    while (true)
    {
        move = tela_tecla();

        //Todas as atualizações de posição executam break para poder atualizar na tela a posição do cursor para o usuário
        if ((move == 'w' || move == 'W') && pointer->menu_index != 0)
        {
            pointer->menu_index--;
            break;
        }
        else if ((move == 's' || move == 'S') && pointer->menu_index != 1)
        {
            pointer->menu_index++;
            break;
        }
        else if (move == '\n')
        {
            if (pointer->menu_index == 0)
            {
                pointer->menu_selected = 'J';
            }
            else
            {
                pointer->menu_selected = 'S';
            }

            pointer->menu_index = -1;

            break;
        }
    }
}
