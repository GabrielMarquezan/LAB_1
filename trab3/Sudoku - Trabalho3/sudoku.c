#include <stdio.h>
#include <stdbool.h>
#include "telag2.h"

//Cria��o da struct que representa o estado do jogo
typedef struct {
	int time;
	int score;
	int difficulty_selected;
	int number_selected;
	int menu_selected;
	int board_id;

	char player_name[20];
} gamestate;

//Declara��o das fun��es


int main()
{
	gamestate game = { 0, 0, 0, 0, 0, 0, "" };

	//Declara��o do tabuleiro
	int board[81][81] = { 0 };
}