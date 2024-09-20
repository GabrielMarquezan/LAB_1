
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>



//Declaração das funções

// tipos de dados
typedef struct {
	int largura;
	int altura;
} tamanho_t;

typedef struct {
	int x;
	int y;
} ponto_t;

typedef struct {
	ponto_t centro;
	int raio;
} circulo_t;

typedef struct {
	ponto_t inicio;
	tamanho_t tamanho;
} retangulo_t;

typedef struct {
	float vermelho;
	float verde;
	float azul;
	float opacidade;
} cor_t;

typedef struct {
	ponto_t posicao;  // onde está o mouse
	bool apertado[3]; // o estado de cada um dos 3 botões
	bool clicado[3];  // se cada botão foi solto desde a última leitura do estado
	int giro;         // quanto a rodela de rolagem girou desda a última leitura
} rato_t;

// inicialização da tela
// cria uma janela com o tamanho dado em pixels
// deve ser executada antes do uso de qualquer outra função da tela
void tela_inicio(tamanho_t tamanho, char nome[]);


// finalização da tela
// deve ser chamada no final da utilização da tela, nenhuma outra função da
// tela deve ser chamada depois desta.
void tela_fim(void);


// atualiza a tela
// faz com o que foi desenhado na tela desde a última atualização
// realmente apareça.
// caso necessário, espera até que a imagem anterior tenha ficado
// na tela pelo tempo adequado.
// antes da chamada a esta função a imagem anterior fica sendo exibida, 
// o conteúdo da nova imagem fica só na memória.
void tela_atualiza(void);

// frequencia de atualizacao da tela
#define QUADROS_POR_SEGUNDO 30.0
#define SEGUNDOS_POR_QUADRO (1/QUADROS_POR_SEGUNDO)

// DESENHO

// desenha um círculo
void tela_circulo(circulo_t circulo, float largura, cor_t cor_linha, cor_t cor_interna);

// desenha uma linha reta
void tela_linha(ponto_t inicio, ponto_t fim, float largura, cor_t cor_linha);

// desenha um retangulo
void tela_retangulo(retangulo_t retangulo, float largura,
	cor_t cor_linha, cor_t cor_interna);


// desenha texto
void tela_texto(ponto_t posicao, int tamanho, cor_t cor, char texto[]);

// retorna retângulo que contorna o texto
retangulo_t tela_texto_contorno(ponto_t posicao, int tamanho, char texto[]);


// ACESSO AO RATO

// retorna o estado do rato
rato_t tela_rato();


// ACESSO AO TECLADO

// retorna uma tecla digitada ou '\0' se nenhuma tecla for digitada
// se for digitado enter, retorna '\n'
// se for digitado backspace, retorna '\b'
char tela_tecla(void);

// retorna o estado das teclas shift e control (true se apertadas)
bool tela_shift();
bool tela_control();

// TEMPO

// retorna quantos segundos transcorreram desde algum momento no passado
double tela_relogio(void);


















//Criação da struct que representa o estado do jogo
typedef struct {
    int time;
    int initial_time;
    int total_match_time;
    int score;
    int menu_selected;
    
    //Referentes ao tabuleiro
    int boards_already_chosen[100];
    int number_of_boards_played;
    int chosen_board[9][9];
    int board_id;
    int difficulty_selected;
    int numbers_on_the_board[81];
    int player_numbers[81];
    ponto_t board_numbers_coordinates[81];
    ponto_t player_numbers_positions[81];
    FILE* boards_file;
    //Marcadores
    bool markers[81][9];
    ponto_t markers_matrix[27][27];
    cor_t markers_color;
    //Mensagens
    bool show_occupied_slot_message;
    bool show_number_already_on_minor_board_message;
    bool show_wrong_move_message;
    //Condições de parada
    bool match_end;
    bool exit;
    //Jogabilidade
    int number_selected;

    tamanho_t screen_size;

} gamestate;

//Criação da estrutura que registra os pontos
typedef struct {
    char player_name[11];
    int player_score;
    int board_played;
    int position;
} player_record;

//Declaração das funções
void choose_board(gamestate* pointer);
player_record* read_records_file(int* number_of_records_pointer);
void get_player_name(gamestate* pointer, player_record* new_player_pointer);
rato_t mouse_tracing(gamestate* pointer);
void main_menu(gamestate* pointer);
void configs_screen(gamestate* pointer);
void controls_screen(gamestate* pointer);
void difficulty_settings_screen(gamestate* pointer);
void draw_grid(void);
void calc_numbers_coordinates(gamestate* pointer);
void put_board_numbers(gamestate* pointer);
void draw_exit_button(void);
void activate_exit(gamestate* pointer, int mouse_x, int mouse_y, bool left_button_is_clicked);
void changing_numbers(gamestate* pointer, int mouse_spin);
void put_player_numbers(gamestate* pointer, bool mouse_left_button_is_clicked, int mouse_x, int mouse_y);
void calc_markers_coordinates(gamestate* pointer);
void put_markers(gamestate* pointer, bool mouse_right_button, int mouse_x, int mouse_y);
void put_time_passed(gamestate* pointer);
void draw_match(gamestate* pointer, bool mouse_right_button, bool mouse_left_button_is_clicked, int mouse_x, int mouse_y);
int play_match(gamestate* pointer);
void order_records_array(player_record* player_pointer, player_record* new_player_pointer, int* number_of_records_pointer);
void overwrite_record(int* number_of_records_pointer, player_record* player_pointer);
void final_screen(gamestate* pointer, player_record* player_pointer, player_record* new_player_pointer);

int main(void)
{
    //Declara estruturas
    gamestate game;

    //Inicializando estado do jogo
    game.time = 0;
    game.initial_time = 0;
    game.total_match_time = 0;
    game.score = 0;
    game.menu_selected = 1;

    //Inicializando informações referentes ao tabuleiro
    for (int i = 0; i < 100; i++)
    {
        game.boards_already_chosen[i] = 0;
    }
    //Número de tabuleiros jogados
    game.number_of_boards_played = 0;
    //Inicializa tabuleiro
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            game.chosen_board[i][j] = 0;
        }
    }
    //Inicializa informarções do tabuleiro
    game.board_id = 0;
    game.difficulty_selected = 0;
    for (int i = 0; i < 81; i++)
    {
        game.numbers_on_the_board[i] = 0;
    }
    //Abrindo tabuleiros.txt
    game.boards_file = fopen("tabuleiros.txt", "r");
    if (game.boards_file == NULL)
    {
        printf("Erro ao abrir o arquivo de tabuleiros\n");
        return -1;
    }

    //Inicializa os marcadores
    for (int i = 0; i < 81; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            game.markers[i][j] = false;
        }
    }
    //Inicializa informações dos marcadores
    game.markers_color.vermelho = 1;
    game.markers_color.verde = 1;
    game.markers_color.azul = 1;
    game.markers_color.opacidade = 1;
    //Inicializa atributos de jogabilidade
    game.number_selected = 9;
    //Inicializa mensagens
    game.show_occupied_slot_message = false;
    game.show_number_already_on_minor_board_message = false;
    game.show_wrong_move_message = false;
    //Inicializa informações de tela
    game.screen_size.altura = 500;
    game.screen_size.largura = 500;
    //Declara ponteiros
    gamestate* game_pointer = &game;

    tela_inicio(game.screen_size, "Benhur's Game 2");
    choose_board(game_pointer);
   
    //Atualiza o vetor de acordo com o tabuleiro escolhido
    for (int i = 0; i < 81; i++) game.player_numbers[i] = game.numbers_on_the_board[i];

    player_record new_player = { "          ", 0, game.board_id, 0 };
    int number_of_records = 0;

    get_player_name(game_pointer, &new_player);

    while (true)
    {
        tela_atualiza();

        //Inicializa condições de parada, permitindo jogar novamente
        game.exit = false;
        game.match_end = false;

        player_record * records_array = read_records_file(&number_of_records);
        main_menu(game_pointer);

        if (game.menu_selected == 0)
        {
            game.initial_time = tela_relogio();
            play_match(game_pointer);
            tela_atualiza();

            if (game.match_end)
            {
                order_records_array(records_array, &new_player, &number_of_records);
                overwrite_record(&number_of_records, records_array);
                final_screen(game_pointer, records_array, &new_player);
            }

            if (game.exit) continue;
        }
        else if (game.menu_selected == 1)
        {
            configs_screen(game_pointer);
            choose_board(game_pointer);

            //Atualiza o vetor de acordo com o tabuleiro escolhido
            for (int i = 0; i < 81; i++) game.player_numbers[i] = game.numbers_on_the_board[i];
        }
        else if (game.menu_selected == 2)
        {
            break;
        }
    }

    tela_fim();
    fclose(game.boards_file);
}

void choose_board(gamestate* pointer)
{
    int ultrapassou = 0;
    int reading = 0;
    int board_difficulty = -1;
    int counter = 0;
    bool banned = false;

    //Laço que repete a leitura enquanto não ultrapassar o arquivo
    while (ultrapassou != -1)
    {
        ultrapassou = fscanf(pointer->boards_file, "%d", &reading);

        //Todos os IDs são maiores que cem
        if (reading > 100)
        {
            //Garante que o tabuleiro não é repetido
            for (int i = 0; i < 100; i++)
            {
                if (reading == pointer->boards_already_chosen[i])
                {
                    banned = true;
                    break;
                }
            }

            if (banned)
            {
                banned = false;
                continue;
            }

            //Lê a dificuldade do tabuleiro
            ultrapassou = fscanf(pointer->boards_file, "%d", &board_difficulty);

            if (board_difficulty == pointer->difficulty_selected)
            {
                pointer->board_id = reading;

                //Adiciona o tabuleiro ao tabuleiros já jogados
                pointer->boards_already_chosen[pointer->number_of_boards_played];
                pointer->number_of_boards_played++;

                for (int i = 0; i < 81; i++)
                {
                    ultrapassou = fscanf(pointer->boards_file, "%d", &pointer->numbers_on_the_board[i]);
                }

                break;
            }
        }
    }

    //Transfere o tabuleiro para uma matriz 9x9
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            pointer->chosen_board[i][j] = pointer->numbers_on_the_board[counter];
            counter++;
        }

    }
}

player_record* read_records_file(int* number_of_records_pointer)
{
    FILE* records_file = fopen("Sudoku Records.txt", "r");
    if (records_file == NULL)
    {
        printf("Erro ao abrir arquivo de records!");
        player_record error = { "Erro", -1, -1, -1 };
        return &error;
    }

    fscanf(records_file, "%d", number_of_records_pointer);

    player_record* player_pointer = malloc(sizeof(player_record) * (*number_of_records_pointer));

    if (player_pointer == NULL)
    {
        printf("Erro ao alocar memória\n");
        fclose(records_file);
        player_record error = { "Erro", -1, -1, -1 };
        return &error;
    }

    for (int i = 0; i < *number_of_records_pointer; i++)
    {
        fscanf(records_file, "%d %s %d %d", &player_pointer[i].position, &player_pointer[i].player_name, &player_pointer[i].board_played, &player_pointer[i].player_score);

    }

    fclose(records_file);
    return player_pointer;
}

void get_player_name(gamestate * pointer, player_record *new_player_pointer)
{
    char name_letter = ' ';
    char adapted_string_name[21] = "                    ";
    bool empty_name[10] = { false, false, false, false, false, false, false, false, false, false };
    bool put_john_doe = false;
    bool break_all = false;
    ponto_t digit_your_name = { 83, 100 };
    cor_t digit_your_name_color = { 1, 1, 1, 1 };
    ponto_t name_coordinates = { 50, 245 };
    cor_t name_color = { 1, 1, 0, 1 };
    ponto_t line_left = { 50, 250 };
    ponto_t line_right = { 450, 250 };
    ponto_t press_enter_coordinates = { 100, 450 };
    
    while (true)
    {
        name_letter = tela_tecla();

        for (int i = 0; i < 10; i++)
        {
            if ((name_letter != '\0' && name_letter != ' ') && new_player_pointer->player_name[i] == ' ')
            {
                new_player_pointer->player_name[i] = name_letter;
                break;
            }
            else if (name_letter == '\n')
            {
                if (new_player_pointer->player_name[i] == ' ') empty_name[i] = true;

                put_john_doe = put_john_doe && empty_name[i];

                if (put_john_doe && i == 9)
                {
                    new_player_pointer->player_name[0] = 'J';
                    new_player_pointer->player_name[1] = 'O';
                    new_player_pointer->player_name[2] = 'G';
                    new_player_pointer->player_name[3] = 'A';
                    new_player_pointer->player_name[4] = 'D';
                    new_player_pointer->player_name[5] = 'O';
                    new_player_pointer->player_name[6] = 'R';
                    new_player_pointer->player_name[7] = '4';
                    new_player_pointer->player_name[8] = '0';
                    new_player_pointer->player_name[9] = '4';

                    break;
                }        
                else
                {
                    break_all = true;
                    break;
                }
            }
            else if (name_letter == '\b')
            {
                for (int i = 0; i < 10; i++)
                {
                    if (new_player_pointer->player_name[i] == ' ')
                    {
                        new_player_pointer->player_name[i - 1] = ' ';
                        break;
                    }
                }

                break;
            }

            adapted_string_name[i * 2] = new_player_pointer->player_name[i];
        }

        if (break_all)
        {
            break;
        }

        tela_texto(digit_your_name, 40, digit_your_name_color, "Digite seu nome");
        tela_texto(name_coordinates, 30, name_color, adapted_string_name);
        tela_texto(press_enter_coordinates, 20, digit_your_name_color, "Pressione enter para continuar");

        tela_linha(line_left, line_right, 2, digit_your_name_color);

        mouse_tracing(pointer);

        tela_atualiza();
    }
}

rato_t mouse_tracing(gamestate* pointer)
{
    //Atualiza as informações do mouse
    rato_t mouse = tela_rato();

    circulo_t mouse_circle;

    mouse_circle.centro = mouse.posicao;
    mouse_circle.raio = 9;

    cor_t circle_color = { 1, 0, 0, 1 };
    cor_t transparent = { 0, 0, 0, 0 };
    ponto_t mouse_vertical_line_sup = { mouse.posicao.x, mouse.posicao.y - mouse_circle.raio };
    ponto_t mouse_vertical_line_inf = { mouse.posicao.x, mouse.posicao.y + mouse_circle.raio };
    ponto_t mouse_horizontal_line_left = { mouse.posicao.x - mouse_circle.raio, mouse.posicao.y };
    ponto_t mouse_horizontal_line_right = { mouse.posicao.x + mouse_circle.raio, mouse.posicao.y };

    //Desenha um círculo com uma cruz como cursror
    tela_circulo(mouse_circle, 2, circle_color, transparent);
    tela_linha(mouse_vertical_line_sup, mouse_vertical_line_inf, 2, circle_color);
    tela_linha(mouse_horizontal_line_left, mouse_horizontal_line_right, 2, circle_color);

    return mouse;
}

void main_menu(gamestate* pointer)
{
    ponto_t title_coordinate = { 20, 75 };
    cor_t title_color = { 1, 1, 1, 1 };
    char move_menu = ' ';

    while (true)
    {
        rato_t mouse = mouse_tracing(pointer);

        tela_texto(title_coordinate, 53, title_color, "Benhur's Game 2");

        if ((mouse.posicao.x > 215 && mouse.posicao.x < 300) && (mouse.posicao.y < 200 && mouse.posicao.y > 175))
        {
            ponto_t first_option_coordinate = { 200, 200 };
            ponto_t second_option_coordinate = { 200, 230 };
            ponto_t third_option_coordinate = { 227, 260 };

            tela_texto(first_option_coordinate, 30, title_color, "->Jogar");
            tela_texto(second_option_coordinate, 30, title_color, "Ajustes");
            tela_texto(third_option_coordinate, 30, title_color, "Sair");

            if (mouse.clicado[0])
            {
                pointer->menu_selected = 0;
                break;
            }
        }
        else if ((mouse.posicao.x > 195 && mouse.posicao.x < 315) && (mouse.posicao.y < 235 && mouse.posicao.y > 200))
        {
            ponto_t first_option_coordinate = { 219, 200 };
            ponto_t second_option_coordinate = { 181, 230 };
            ponto_t third_option_coordinate = { 227, 260 };

            tela_texto(first_option_coordinate, 30, title_color, "Jogar");
            tela_texto(second_option_coordinate, 30, title_color, "->Ajustes");
            tela_texto(third_option_coordinate, 30, title_color, "Sair");

            if (mouse.clicado[0])
            {
                pointer->menu_selected = 1;
                break;
            }
        }
        else if ((mouse.posicao.x > 225 && mouse.posicao.x < 290) && (mouse.posicao.y < 265 && mouse.posicao.y > 235))
        {
            ponto_t first_option_coordinate = { 219, 200 };
            ponto_t second_option_coordinate = { 200, 230 };
            ponto_t third_option_coordinate = { 207, 260 };

            tela_texto(first_option_coordinate, 30, title_color, "Jogar");
            tela_texto(second_option_coordinate, 30, title_color, "Ajustes");
            tela_texto(third_option_coordinate, 30, title_color, "->Sair");

            if (mouse.clicado[0])
            {
                pointer->menu_selected = 2;
                break;
            }
        }
        else
        {
            ponto_t first_option_coordinate = { 219, 200 };
            ponto_t second_option_coordinate = { 200, 230 };
            ponto_t third_option_coordinate = { 227, 260 };

            tela_texto(first_option_coordinate, 30, title_color, "Jogar");
            tela_texto(second_option_coordinate, 30, title_color, "Ajustes");
            tela_texto(third_option_coordinate, 30, title_color, "Sair");
        }

        mouse = mouse_tracing(pointer);

        tela_atualiza();
    }

}

void configs_screen(gamestate* pointer)
{
    ponto_t title_coordinate = { 160, 75 };
    cor_t title_color = { 1, 1, 1, 1 };

    int configs_selected = 0;

    while (true)
    {
        rato_t mouse = mouse_tracing(pointer);

        tela_texto(title_coordinate, 53, title_color, "Ajustes");

        if ((mouse.posicao.x > 180 && mouse.posicao.x < 330) && (mouse.posicao.y < 200 && mouse.posicao.y > 175))
        {
            ponto_t first_option_coordinate = { 165, 200 };
            ponto_t second_option_coordinate = { 170, 230 };
            ponto_t third_option_coordinate = { 225, 260 };

            tela_texto(first_option_coordinate, 30, title_color, "->Controles");
            tela_texto(second_option_coordinate, 30, title_color, "Dificuldade");
            tela_texto(third_option_coordinate, 30, title_color, "Sair");

            if (mouse.clicado[0])
            {
                configs_selected = 0;
                break;
            }
        }
        else if ((mouse.posicao.x > 170 && mouse.posicao.x < 345) && (mouse.posicao.y < 235 && mouse.posicao.y > 200))
        {
            ponto_t first_option_coordinate = { 185, 200 };
            ponto_t second_option_coordinate = { 150, 230 };
            ponto_t third_option_coordinate = { 225, 260 };

            tela_texto(first_option_coordinate, 30, title_color, "Controles");
            tela_texto(second_option_coordinate, 30, title_color, "->Dificuldade");
            tela_texto(third_option_coordinate, 30, title_color, "Sair");

            if (mouse.clicado[0])
            {
                configs_selected = 1;
                break;
            }
        }
        else if ((mouse.posicao.x > 225 && mouse.posicao.x < 290) && (mouse.posicao.y < 265 && mouse.posicao.y > 235))
        {
            ponto_t first_option_coordinate = { 185, 200 };
            ponto_t second_option_coordinate = { 170, 230 };
            ponto_t third_option_coordinate = { 200, 260 };

            tela_texto(first_option_coordinate, 30, title_color, "Controles");
            tela_texto(second_option_coordinate, 30, title_color, "Dificuldade");
            tela_texto(third_option_coordinate, 30, title_color, "->Sair");

            if (mouse.clicado[0])
            {
                configs_selected = 2;
                break;
            }
        }
        else
        {
            ponto_t first_option_coordinate = { 185, 200 };
            ponto_t second_option_coordinate = { 170, 230 };
            ponto_t third_option_coordinate = { 225, 260 };

            tela_texto(first_option_coordinate, 30, title_color, "Controles");
            tela_texto(second_option_coordinate, 30, title_color, "Dificuldade");
            tela_texto(third_option_coordinate, 30, title_color, "Sair");
        }

        mouse = mouse_tracing(pointer);

        tela_atualiza();
    }

    if (configs_selected == 0)
    {
        controls_screen(pointer);
    }
    else if (configs_selected == 1)
    {
        difficulty_settings_screen(pointer);
    }

}

void difficulty_settings_screen(gamestate* pointer)
{

    ponto_t diff_title = { 100, 65 };
    cor_t title_color = { 1, 1, 1, 1 };
    cor_t transparent = { 0, 0, 0, 0 };

    ponto_t easy_text = { 250, 180 };
    cor_t easy_color = { 0, 1, 0, 1 };
    retangulo_t easy_rect = { {200, 152} , {30, 30} };

    ponto_t medium_text = { 250, 230 };
    cor_t medium_color = { 1, 1, 1, 1 };
    retangulo_t medium_rect = { {200, 202} , {30, 30} };

    ponto_t hard_text = { 250, 280 };
    cor_t hard_color = { 1, 1, 0, 1 };
    retangulo_t hard_rect = { {200, 252} , {30, 30} };

    ponto_t extreme_text = { 250, 330 };
    cor_t extreme_color = { 1, 0, 0, 1 };
    retangulo_t extreme_rect = { {200, 302} , {30, 30} };

    ponto_t exit_diff_setting = { 200, 460 };
    retangulo_t exit_diff_rect = { {199, 435}, {74, 30} };

    while (true)
    {

        rato_t mouse = mouse_tracing(pointer);

        tela_texto(diff_title, 55, title_color, "Dificuldade");

        if (pointer->difficulty_selected == 0)
        {
            ponto_t marked = { 192, 180 };

            tela_texto(easy_text, 35, easy_color, "Facil");
            tela_retangulo(easy_rect, 4, easy_color, transparent);
            tela_texto(marked, 35, easy_color, " X ");

            tela_texto(medium_text, 35, medium_color, "Medio");
            tela_retangulo(medium_rect, 4, medium_color, transparent);

            tela_texto(hard_text, 35, hard_color, "Dificil");
            tela_retangulo(hard_rect, 4, hard_color, transparent);

            tela_texto(extreme_text, 35, extreme_color, "Extremo");
            tela_retangulo(extreme_rect, 4, extreme_color, transparent);
        }
        else if (pointer->difficulty_selected == 1)
        {
            ponto_t marked = { 192, 230 };

            tela_texto(easy_text, 35, easy_color, "Facil");
            tela_retangulo(easy_rect, 4, easy_color, transparent);

            tela_texto(medium_text, 35, medium_color, "Medio");
            tela_retangulo(medium_rect, 4, medium_color, transparent);
            tela_texto(marked, 35, medium_color, " X ");

            tela_texto(hard_text, 35, hard_color, "Dificil");
            tela_retangulo(hard_rect, 4, hard_color, transparent);

            tela_texto(extreme_text, 35, extreme_color, "Extremo");
            tela_retangulo(extreme_rect, 4, extreme_color, transparent);
        }
        else if (pointer->difficulty_selected == 2)
        {
            ponto_t marked = { 192, 280 };

            tela_texto(easy_text, 35, easy_color, "Facil");
            tela_retangulo(easy_rect, 4, easy_color, transparent);

            tela_texto(medium_text, 35, medium_color, "Medio");
            tela_retangulo(medium_rect, 4, medium_color, transparent);

            tela_texto(hard_text, 35, hard_color, "Dificil");
            tela_retangulo(hard_rect, 4, hard_color, transparent);
            tela_texto(marked, 35, hard_color, " X ");

            tela_texto(extreme_text, 35, extreme_color, "Extremo");
            tela_retangulo(extreme_rect, 4, extreme_color, transparent);
        }
        else
        {
            ponto_t marked = { 192, 330 };

            tela_texto(easy_text, 35, easy_color, "Facil");
            tela_retangulo(easy_rect, 4, easy_color, transparent);

            tela_texto(medium_text, 35, medium_color, "Medio");
            tela_retangulo(medium_rect, 4, medium_color, transparent);

            tela_texto(hard_text, 35, hard_color, "Dificil");
            tela_retangulo(hard_rect, 4, hard_color, transparent);

            tela_texto(extreme_text, 35, extreme_color, "Extremo");
            tela_retangulo(extreme_rect, 4, extreme_color, transparent);
            tela_texto(marked, 35, extreme_color, " X ");
        }

        tela_texto(exit_diff_setting, 25, extreme_color, "Voltar");
        tela_retangulo(exit_diff_rect, 2, extreme_color, transparent);

        if (mouse.apertado[0])
        {
            if ((mouse.posicao.x > 200 && mouse.posicao.x < 230) && (mouse.posicao.y > 152 && mouse.posicao.y < 182)) pointer->difficulty_selected = 0;
            if ((mouse.posicao.x > 200 && mouse.posicao.x < 230) && (mouse.posicao.y > 202 && mouse.posicao.y < 232)) pointer->difficulty_selected = 1;
            if ((mouse.posicao.x > 200 && mouse.posicao.x < 230) && (mouse.posicao.y > 252 && mouse.posicao.y < 282)) pointer->difficulty_selected = 2;
            if ((mouse.posicao.x > 200 && mouse.posicao.x < 230) && (mouse.posicao.y > 302 && mouse.posicao.y < 332)) pointer->difficulty_selected = 3;

            if ((mouse.posicao.x > 199 && mouse.posicao.x < 273) && (mouse.posicao.y > 435 && mouse.posicao.y < 465)) break;
        }

        mouse = mouse_tracing(pointer);

        tela_atualiza();
    }
}

void controls_screen(gamestate* pointer)
{
    ponto_t mouse_title = { 175, 100 };
    ponto_t keyboard_title = { 345, 100 };
    cor_t title_color = { 1, 1, 1, 1 };

    ponto_t vertical_line_sup = { 310, 50 };
    ponto_t vertical_line_inf = { 310, 450 };
    ponto_t horizontal_line_1_left = { 150, 110 };
    ponto_t horizontal_line_1_right = { 480, 110 };
    ponto_t horizontal_line_2_left = { 150, 140 };
    ponto_t horizontal_line_2_right = { 480, 140 };
    ponto_t horizontal_line_3_left = { 150, 170 };
    ponto_t horizontal_line_3_right = { 480, 170 };
    ponto_t horizontal_line_4_left = { 150, 200 };
    ponto_t horizontal_line_4_right = { 480, 200 };
    ponto_t horizontal_line_5_left = { 150, 230 };
    ponto_t horizontal_line_5_right = { 480, 230 };
    ponto_t horizontal_line_6_left = { 150, 260 };
    ponto_t horizontal_line_6_right = { 480, 260 };

    ponto_t change_numbers_title = { 20, 130 };
    ponto_t change_numbers_mouse = { 205, 130 };
    ponto_t change_numbers_keyboard = { 335, 130 };

    ponto_t put_numbers_title = { 20, 160 };
    ponto_t put_numbers_mouse = { 165, 160 };
    ponto_t put_numbers_keyboard = { 380, 160 };

    ponto_t put_markers_title = { 10, 190 };
    ponto_t put_markers_mouse = { 178, 190 };
    ponto_t put_markers_keyboard = { 355, 190 };

    ponto_t erase_title = { 50, 220 };
    ponto_t erase_mouse = { 178, 220 };
    ponto_t erase_keyboard = { 355, 220 };

    ponto_t exit_title = { 60, 250 };
    ponto_t exit_mouse = { 211, 250 };
    ponto_t exit_keyboard = { 385, 250 };

    ponto_t exit_controls_button_text = { 40, 450 };
    retangulo_t exit_controls_button_rect = { {39, 430}, {68, 22} };
    cor_t exit_controls_color = { 1, 0, 0, 1 };
    cor_t transparent = { 0, 0, 0, 0 };

    while (true)
    {
        rato_t mouse = mouse_tracing(pointer);
        tela_texto(mouse_title, 30, title_color, "Mouse");
        tela_texto(keyboard_title, 30, title_color, "Teclado");

        tela_linha(vertical_line_sup, vertical_line_inf, 2, title_color);
        tela_linha(horizontal_line_1_left, horizontal_line_1_right, 2, title_color);
        tela_linha(horizontal_line_2_left, horizontal_line_2_right, 2, title_color);
        tela_linha(horizontal_line_3_left, horizontal_line_3_right, 2, title_color);
        tela_linha(horizontal_line_4_left, horizontal_line_4_right, 2, title_color);
        tela_linha(horizontal_line_5_left, horizontal_line_5_right, 2, title_color);
        tela_linha(horizontal_line_6_left, horizontal_line_6_right, 2, title_color);


        tela_texto(change_numbers_title, 15, title_color, "Mudar numero");
        tela_texto(change_numbers_mouse, 15, title_color, "Scroll");
        tela_texto(change_numbers_keyboard, 15, title_color, "Teclas numericas");

        tela_texto(put_numbers_title, 15, title_color, "Inserir numeros");
        tela_texto(put_numbers_mouse, 15, title_color, "Botao esquerdo");
        tela_texto(put_numbers_keyboard, 15, title_color, "Enter");

        tela_texto(put_markers_title, 15, title_color, "Inserir marcadores");
        tela_texto(put_markers_mouse, 15, title_color, "Botao direito");
        tela_texto(put_markers_keyboard, 15, title_color, "Ctrl+Enter");

        tela_texto(erase_title, 15, title_color, "Apagar");
        tela_texto(erase_mouse, 15, title_color, "Botao direito");
        tela_texto(erase_keyboard, 15, title_color, "Backspace");

        tela_texto(exit_title, 15, title_color, "Sair");
        tela_texto(exit_mouse, 15, title_color, "Sair");
        tela_texto(exit_keyboard, 15, title_color, "ESC");

        tela_texto(exit_controls_button_text, 22, exit_controls_color, "Voltar");
        tela_retangulo(exit_controls_button_rect, 2, exit_controls_color, transparent);

        tela_atualiza();

        if (mouse.apertado[0])
        {
            if ((mouse.posicao.x > 39 && mouse.posicao.x < 109) && (mouse.posicao.y > 429 && mouse.posicao.y < 452))
            {
                break;
            }
        }
    }
}

void draw_grid(void)
{
    retangulo_t extern_part_of_grid = { {100,100}, {300,300} }; //Quadrado do grid

    //Linhas verticais maiores do grid externo
    ponto_t vertical_line1_1 = { 200,100 };
    ponto_t vertical_line1_2 = { 200,400 };
    ponto_t vertical_line2_1 = { 300,100 };
    ponto_t vertical_line2_2 = { 300,400 };

    //Linhas horizontais do grid externo
    ponto_t horizontal_line1_1 = { 100,200 };
    ponto_t horizontal_line1_2 = { 400,200 };
    ponto_t horizontal_line2_1 = { 100,300 };
    ponto_t horizontal_line2_2 = { 400,300 };

    //Linhas verticais dos grids internos
    ponto_t vertical_intern_line1_1 = { 133,100 };
    ponto_t vertical_intern_line1_2 = { 133,400 };
    ponto_t vertical_intern_line2_1 = { 167,100 };
    ponto_t vertical_intern_line2_2 = { 167,400 };
    ponto_t vertical_intern_line3_1 = { 233,100 };
    ponto_t vertical_intern_line3_2 = { 233,400 };
    ponto_t vertical_intern_line4_1 = { 267,100 };
    ponto_t vertical_intern_line4_2 = { 267,400 };
    ponto_t vertical_intern_line5_1 = { 333,100 };
    ponto_t vertical_intern_line5_2 = { 333,400 };
    ponto_t vertical_intern_line6_1 = { 367,100 };
    ponto_t vertical_intern_line6_2 = { 367,400 };

    //Linhas horizontais dos grids internos
    ponto_t horizontal_intern_line1_1 = { 100, 133 };
    ponto_t horizontal_intern_line1_2 = { 400, 133 };
    ponto_t horizontal_intern_line2_1 = { 100, 167 };
    ponto_t horizontal_intern_line2_2 = { 400, 167 };
    ponto_t horizontal_intern_line3_1 = { 100, 233 };
    ponto_t horizontal_intern_line3_2 = { 400, 233 };
    ponto_t horizontal_intern_line4_1 = { 100, 267 };
    ponto_t horizontal_intern_line4_2 = { 400, 267 };
    ponto_t horizontal_intern_line5_1 = { 100, 333 };
    ponto_t horizontal_intern_line5_2 = { 400, 333 };
    ponto_t horizontal_intern_line6_1 = { 100, 367 };
    ponto_t horizontal_intern_line6_2 = { 400, 367 };


    cor_t grid_color = { 1, 1, 1, 1 };
    cor_t grid_inside_color = { 1, 1, 1, 0 };
    cor_t transparent = { 0, 0, 0, 0 };

    //Desenha linhas verticais internas
    tela_linha(vertical_intern_line1_1, vertical_intern_line1_2, 1, grid_inside_color);
    tela_linha(vertical_intern_line2_1, vertical_intern_line2_2, 1, grid_inside_color);
    tela_linha(vertical_intern_line3_1, vertical_intern_line3_2, 1, grid_inside_color);
    tela_linha(vertical_intern_line4_1, vertical_intern_line4_2, 1, grid_inside_color);
    tela_linha(vertical_intern_line5_1, vertical_intern_line5_2, 1, grid_inside_color);
    tela_linha(vertical_intern_line6_1, vertical_intern_line6_2, 1, grid_inside_color);
    //Desenha linhas horizontais internas
    tela_linha(horizontal_intern_line1_1, horizontal_intern_line1_2, 1, grid_inside_color);
    tela_linha(horizontal_intern_line2_1, horizontal_intern_line2_2, 1, grid_inside_color);
    tela_linha(horizontal_intern_line3_1, horizontal_intern_line3_2, 1, grid_inside_color);
    tela_linha(horizontal_intern_line4_1, horizontal_intern_line4_2, 1, grid_inside_color);
    tela_linha(horizontal_intern_line5_1, horizontal_intern_line5_2, 1, grid_inside_color);
    tela_linha(horizontal_intern_line6_1, horizontal_intern_line6_2, 1, grid_inside_color);
    //Desenha grid maior
    tela_retangulo(extern_part_of_grid, 5, grid_color, transparent);
    tela_linha(vertical_line1_1, vertical_line1_2, 5, grid_color);
    tela_linha(vertical_line2_1, vertical_line2_2, 5, grid_color);
    tela_linha(horizontal_line1_1, horizontal_line1_2, 5, grid_color);
    tela_linha(horizontal_line2_1, horizontal_line2_2, 5, grid_color);
}

void calc_numbers_coordinates(gamestate* pointer)
{
    int small_counter_x = 0;
    int big_counter_x = 0;
    int small_counter_y = 0;
    int big_counter_y = 0;

    for (int i = 0; i < 81; i++)
    {
        pointer->board_numbers_coordinates[i].x = 105 + 33 * small_counter_x + 100 * big_counter_x;
        pointer->board_numbers_coordinates[i].y = 124 + 33 * small_counter_y + 100 * big_counter_y;

        small_counter_x++;

        if (small_counter_x == 3)
        {
            big_counter_x++;
            small_counter_x = 0;
        }

        if (big_counter_x == 3)
        {
            small_counter_y++;
            big_counter_x = 0;
        }

        if (small_counter_y == 3)
        {
            big_counter_y++;
            small_counter_y = 0;
        }
    }
}

void put_board_numbers(gamestate* pointer)
{
    //Inicializa a matriz que conterá o tabuleiro em forma de string
    char string_board[81][4];

    for (int i = 0; i < 81; i++)
    {
        string_board[i][0] = ' ';
        string_board[i][1] = ' ';
        string_board[i][2] = ' ';
    }

    for (int i = 0; i < 81; i++)
    {
        string_board[i][1] = pointer->numbers_on_the_board[i] + 48;
    }

    //Cores dos números
    cor_t numbers_color = { 0, 1, 1, 1 };

    for (int i = 0; i < 61; i += 3)
    {
        if (string_board[i][1] != 48) tela_texto(pointer->board_numbers_coordinates[i], 20, numbers_color, string_board[i]);
        if (string_board[i + 1][1] != 48) tela_texto(pointer->board_numbers_coordinates[i + 1], 20, numbers_color, string_board[i + 1]);
        if (string_board[i + 2][1] != 48) tela_texto(pointer->board_numbers_coordinates[i + 2], 20, numbers_color, string_board[i + 2]);
        if (string_board[i + 9][1] != 48) tela_texto(pointer->board_numbers_coordinates[i + 9], 20, numbers_color, string_board[i + 9]);
        if (string_board[i + 10][1] != 48) tela_texto(pointer->board_numbers_coordinates[i + 10], 20, numbers_color, string_board[i + 10]);
        if (string_board[i + 11][1] != 48) tela_texto(pointer->board_numbers_coordinates[i + 11], 20, numbers_color, string_board[i + 11]);
        if (string_board[i + 18][1] != 48) tela_texto(pointer->board_numbers_coordinates[i + 18], 20, numbers_color, string_board[i + 18]);
        if (string_board[i + 19][1] != 48) tela_texto(pointer->board_numbers_coordinates[i + 19], 20, numbers_color, string_board[i + 19]);
        if (string_board[i + 20][1] != 48) tela_texto(pointer->board_numbers_coordinates[i + 20], 20, numbers_color, string_board[i + 20]);
    }
}

void draw_exit_button(void)
{
    ponto_t exit_txt = { 420,50 };
    retangulo_t outline = { {420,33}, {41,20} };
    cor_t exit_color = { 1, 0, 0, 1 };
    cor_t outline_color = { 1, 0, 0, 1 };
    cor_t transparent = { 0, 0, 0, 0, };

    tela_retangulo(outline, 1, outline_color, transparent);
    tela_texto(exit_txt, 20, exit_color, "Sair");
}

void activate_exit(gamestate* pointer, int mouse_x, int mouse_y, bool left_button_is_clicked)
{
    if (left_button_is_clicked)
    {
        if ((mouse_x > 419 && mouse_x < 461) && (mouse_y > 32 && mouse_y < 54))
        {
            pointer->exit = true;
        }
    }
}

void changing_numbers(gamestate* pointer, int mouse_spin)
{
    char string_number[4] = "   ";
    ponto_t show_number = { 435, 250 };
    ponto_t show_erase = { 415, 250 };
    cor_t number_color = { 1, 1, 1, 1 };

    pointer->number_selected += mouse_spin;

    //Garante que os números nunca serão menores que 0 e maiores que 9
    if (pointer->number_selected < 0)
    {
        pointer->number_selected = 0;
    }
    else if (pointer->number_selected > 9)
    {
        pointer->number_selected = 9;
    }

    string_number[2] = pointer->number_selected + 48;

    if (pointer->number_selected != 0)
    {
        tela_texto(show_number, 20, number_color, string_number);
    }
    else
    {
        //Ao invés de printar 0, printa "Apagar"
        tela_texto(show_erase, 20, number_color, "Apagar");
    }
}

void put_player_numbers(gamestate* pointer, bool mouse_left_button_is_clicked, int mouse_x, int mouse_y)
{
    //Inicializa a matriz que conterá o tabuleiro em forma de string

    char string_board[81][4];

    for (int i = 0; i < 81; i++)
    {
        string_board[i][0] = ' ';
        string_board[i][1] = ' ';
        string_board[i][2] = ' ';
    }

    int positions[9][9];
    int counter = 0;

    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            positions[i][j] = counter;
            counter++;
        }
    }

    int column = 0;
    int numbers_size = 20;
    int position_x_in_board_numbers = 0;
    cor_t numbers_color = { 1, 1, 0, 1 };
    bool can_print_number = true;
    bool wrong_move = false;

    //Identifica dentro de qual quadradinho foi clicado
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            position_x_in_board_numbers = i + 9 * j;

            if (mouse_left_button_is_clicked && ((mouse_x > 100 && mouse_x < 400) && (mouse_y > 100 && mouse_y < 400)))
            {
                if (mouse_x < 133 + 33 * i)
                {
                    if (mouse_y < 133 + 33 * j)
                    {
                        if (pointer->numbers_on_the_board[position_x_in_board_numbers] == 0)
                        {
                            if (i < 3)
                            {
                                if (j < 3)
                                {
                                    int index[9] = { 0, 1, 2, 9, 10, 11, 18, 19, 20 };
                                    for (int ii = 0; ii < 9; ii++)
                                    {
                                        if (pointer->number_selected != 0)
                                        {
                                            if (pointer->player_numbers[index[ii]] == pointer->number_selected && position_x_in_board_numbers != index[ii]) can_print_number = false;
                                        }
                                    }
                                }
                                else if (j < 6)
                                {
                                    int index[9] = { 27, 28, 29, 36, 37, 38, 45, 46, 47 };
                                    for (int ii = 0; ii < 9; ii++)
                                    {
                                        if (pointer->number_selected != 0)
                                        {
                                            if (pointer->player_numbers[index[ii]] == pointer->number_selected && position_x_in_board_numbers != index[ii]) can_print_number = false;
                                        }
                                    }
                                }
                                else
                                {
                                    int index[9] = { 54, 55, 56, 63, 64, 65, 72, 73, 74 };
                                    for (int ii = 0; ii < 9; ii++)
                                    {
                                        if (pointer->number_selected != 0)
                                        {
                                            if (pointer->player_numbers[index[ii]] == pointer->number_selected && position_x_in_board_numbers != index[ii]) can_print_number = false;
                                        }
                                    }
                                }
                            }
                            else if (i < 6)
                            {
                                if (j < 3)
                                {
                                    int index[9] = { 3, 4, 5, 12, 13, 14, 21, 22, 23 };
                                    for (int ii = 0; ii < 9; ii++)
                                    {
                                        if (pointer->number_selected != 0)
                                        {
                                            if (pointer->player_numbers[index[ii]] == pointer->number_selected && position_x_in_board_numbers != index[ii]) can_print_number = false;
                                        }
                                    }
                                }
                                else if (j < 6)
                                {
                                    int index[9] = { 30, 31, 32, 39, 40, 41, 48, 49, 50 };
                                    for (int ii = 0; ii < 9; ii++)
                                    {
                                        if (pointer->number_selected != 0)
                                        {
                                            if (pointer->player_numbers[index[ii]] == pointer->number_selected && position_x_in_board_numbers != index[ii]) can_print_number = false;
                                        }
                                    }
                                }
                                else
                                {
                                    int index[9] = { 57, 58, 59, 66, 67, 68, 75, 76, 77 };
                                    for (int ii = 0; ii < 9; ii++)
                                    {
                                        if (pointer->number_selected != 0)
                                        {
                                            if (pointer->player_numbers[index[ii]] == pointer->number_selected && position_x_in_board_numbers != index[ii]) can_print_number = false;
                                        }
                                    }
                                }
                            }
                            else
                            {
                                if (j < 3)
                                {
                                    int index[9] = { 6, 7, 8, 15, 16, 17, 24, 25, 26 };
                                    for (int ii = 0; ii < 9; ii++)
                                    {
                                        if (pointer->number_selected != 0)
                                        {
                                            if (pointer->player_numbers[index[ii]] == pointer->number_selected && position_x_in_board_numbers != index[ii]) can_print_number = false;
                                        }
                                    }
                                }
                                else if (j < 6)
                                {
                                    int index[9] = { 33, 34, 35, 42, 43, 44, 51, 52, 53 };
                                    for (int ii = 0; ii < 9; ii++)
                                    {
                                        if (pointer->number_selected != 0)
                                        {
                                            if (pointer->player_numbers[index[ii]] == pointer->number_selected && position_x_in_board_numbers != index[ii]) can_print_number = false;
                                        }
                                    }
                                }
                                else
                                {
                                    int index[9] = { 60, 61, 62, 69, 70, 71, 78, 79, 80 };
                                    for (int ii = 0; ii < 9; ii++)
                                    {
                                        if (pointer->number_selected != 0)
                                        {
                                            if (pointer->player_numbers[index[ii]] == pointer->number_selected && position_x_in_board_numbers != index[ii]) can_print_number = false;
                                        }
                                    }
                                }
                            }

                            //Garante a regra do número repetido
                            for (int ii = 0; ii < 9; ii++)
                            {
                                for (int jj = 0; jj < 9; jj++)
                                {
                                    if (positions[ii][jj] == position_x_in_board_numbers && pointer->number_selected != 0)
                                    {
                                        for (int k = 0; k < 9; k++)
                                        {
                                            if (ii != k)
                                            {
                                                if (pointer->player_numbers[positions[ii][k]] == pointer->number_selected) wrong_move = true;
                                            }

                                            if (jj != k)
                                            {
                                                if (pointer->player_numbers[positions[k][jj]] == pointer->number_selected) wrong_move = true;
                                            }
                                        }
                                    }
                                }
                            }

                            if (can_print_number && wrong_move == false)
                            {
                                pointer->player_numbers[position_x_in_board_numbers] = pointer->number_selected;
                            }
                            else if (wrong_move == false)
                            {
                                pointer->show_number_already_on_minor_board_message = true;
                            }
                            else if (wrong_move)
                            {
                                pointer->show_wrong_move_message = true;
                            }

                            mouse_left_button_is_clicked = false;
                        }
                        else
                        {
                            mouse_left_button_is_clicked = false;
                            pointer->show_occupied_slot_message = true;
                        }
                    }
                }
            }
        }
    }






    for (int i = 0; i < 81; i++)
    {
        string_board[i][1] = pointer->player_numbers[i] + 48;
    }

    //Printa os números modificados pelo jogador
    for (int i = 0; i < 61; i += 3)
    {
        if (string_board[i][1] != 48 && pointer->numbers_on_the_board[i] == 0) tela_texto(pointer->board_numbers_coordinates[i], 20, numbers_color, string_board[i]);
        if (string_board[i + 1][1] != 48 && pointer->numbers_on_the_board[i + 1] == 0) tela_texto(pointer->board_numbers_coordinates[i + 1], 20, numbers_color, string_board[i + 1]);
        if (string_board[i + 2][1] != 48 && pointer->numbers_on_the_board[i + 2] == 0) tela_texto(pointer->board_numbers_coordinates[i + 2], 20, numbers_color, string_board[i + 2]);
        if (string_board[i + 9][1] != 48 && pointer->numbers_on_the_board[i + 9] == 0) tela_texto(pointer->board_numbers_coordinates[i + 9], 20, numbers_color, string_board[i + 9]);
        if (string_board[i + 10][1] != 48 && pointer->numbers_on_the_board[i + 10] == 0) tela_texto(pointer->board_numbers_coordinates[i + 10], 20, numbers_color, string_board[i + 10]);
        if (string_board[i + 11][1] != 48 && pointer->numbers_on_the_board[i + 11] == 0) tela_texto(pointer->board_numbers_coordinates[i + 11], 20, numbers_color, string_board[i + 11]);
        if (string_board[i + 18][1] != 48 && pointer->numbers_on_the_board[i + 18] == 0) tela_texto(pointer->board_numbers_coordinates[i + 18], 20, numbers_color, string_board[i + 18]);
        if (string_board[i + 19][1] != 48 && pointer->numbers_on_the_board[i + 19] == 0) tela_texto(pointer->board_numbers_coordinates[i + 19], 20, numbers_color, string_board[i + 19]);
        if (string_board[i + 20][1] != 48 && pointer->numbers_on_the_board[i + 20] == 0) tela_texto(pointer->board_numbers_coordinates[i + 20], 20, numbers_color, string_board[i + 20]);

        if (pointer->show_occupied_slot_message)
        {
            ponto_t message = { 103,430 };
            cor_t message_color = { 1, 1, 0, 1 };
            tela_texto(message, 20, message_color, "Essa casa ja esta preenchida!");
        }

        if (pointer->show_number_already_on_minor_board_message)
        {
            ponto_t message = { 50,430 };
            cor_t message_color = { 1, 1, 0, 1 };
            tela_texto(message, 20, message_color, "Esse numero ja esta nesse tabuleiro menor");
        }

        if (pointer->show_wrong_move_message)
        {
            ponto_t message = { 10,430 };
            cor_t message_color = { 1, 1, 0, 1 };
            tela_texto(message, 20, message_color, "Jogada errada!");
        }
    }
}

void calc_markers_coordinates(gamestate* pointer)
{
    //Inicializa variáveis matemáticas referentes à coordenada X
    int small_counter_x = 0;
    int medium_counter_x = 0;
    int big_counter_x = 0;
    //Inicializa variáveis matemáticas referentes à coordenada Y
    int small_counter_y = 0;
    int medium_counter_y = 0;
    int big_counter_y = 0;

    for (int i = 0; i < 27; i++)
    {
        for (int j = 0; j < 27; j++)
        {

            pointer->markers_matrix[i][j].x = 102 + small_counter_x * 10 + medium_counter_x * 2 + big_counter_x * 100;
            pointer->markers_matrix[i][j].y = 110 + small_counter_y * 10 + medium_counter_y * 2 + big_counter_y * 100;

            small_counter_x++;

            if (small_counter_x == 3 || small_counter_x == 6)
            {
                medium_counter_x++;
            }
            else if (small_counter_x == 9)
            {
                small_counter_x = 0;
                medium_counter_x = 0;
                big_counter_x++;
            }

            if (big_counter_x == 3)
            {
                big_counter_x = 0;
                small_counter_y++;
            }

            if ((small_counter_y == 3 && medium_counter_y == 0) || (small_counter_y == 6 && medium_counter_y == 1))
            {
                medium_counter_y++;
            }
            else if (small_counter_y == 9)
            {
                small_counter_y = 0;
                medium_counter_y = 0;
                big_counter_y++;
            }
        }
    }
}

void put_markers(gamestate* pointer, bool mouse_right_button_is_clicked, int mouse_x, int mouse_y)
{
    char string_numbers[9][4] = {
        {" 1 "},
        {" 2 "},
        {" 3 "},
        {" 4 "},
        {" 5 "},
        {" 6 "},
        {" 7 "},
        {" 8 "},
        {" 9 "}
    };

    int markers_size = 10;
    int position_x_in_markers = 0;
    int counter_markers = 0;

    //Identifica dentro de qual quadradinho foi clicado
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            position_x_in_markers = i + 9 * j;

            if (mouse_right_button_is_clicked && ((mouse_x > 100 && mouse_x < 400) && (mouse_y > 100 && mouse_y < 400)))
            {
                if (mouse_x < 133 + 33 * i)
                {
                    if (mouse_y < 133 + 33 * j)
                    {
                        if (pointer->numbers_on_the_board[position_x_in_markers] == 0 && pointer->player_numbers[position_x_in_markers] == 0)
                        {
                            switch (pointer->number_selected)

                            {
                            case 0:
                                pointer->markers[position_x_in_markers][0] = false;
                                pointer->markers[position_x_in_markers][1] = false;
                                pointer->markers[position_x_in_markers][2] = false;
                                pointer->markers[position_x_in_markers][3] = false;
                                pointer->markers[position_x_in_markers][4] = false;
                                pointer->markers[position_x_in_markers][5] = false;
                                pointer->markers[position_x_in_markers][6] = false;
                                pointer->markers[position_x_in_markers][7] = false;
                                pointer->markers[position_x_in_markers][8] = false;
                                break;
                            case 1:
                                pointer->markers[position_x_in_markers][0] = !pointer->markers[position_x_in_markers][0];
                                break;
                            case 2:
                                pointer->markers[position_x_in_markers][1] = !pointer->markers[position_x_in_markers][1];
                                break;
                            case 3:
                                pointer->markers[position_x_in_markers][2] = !pointer->markers[position_x_in_markers][2];
                                break;
                            case 4:
                                pointer->markers[position_x_in_markers][3] = !pointer->markers[position_x_in_markers][3];
                                break;
                            case 5:
                                pointer->markers[position_x_in_markers][4] = !pointer->markers[position_x_in_markers][4];
                                break;
                            case 6:
                                pointer->markers[position_x_in_markers][5] = !pointer->markers[position_x_in_markers][5];
                                break;
                            case 7:
                                pointer->markers[position_x_in_markers][6] = !pointer->markers[position_x_in_markers][6];
                                break;
                            case 8:
                                pointer->markers[position_x_in_markers][7] = !pointer->markers[position_x_in_markers][7];
                                break;
                            case 9:
                                pointer->markers[position_x_in_markers][8] = !pointer->markers[position_x_in_markers][8];
                                break;
                            }

                            //Garante que o marcador não aparecerá em outras posições
                            mouse_right_button_is_clicked = false;
                        }
                        else
                        {
                            pointer->show_occupied_slot_message = true;
                            mouse_right_button_is_clicked = false;
                        }
                    }
                }
            }

            if (pointer->player_numbers[position_x_in_markers] != 0)
            {
                pointer->markers[position_x_in_markers][0] = false;
                pointer->markers[position_x_in_markers][1] = false;
                pointer->markers[position_x_in_markers][2] = false;
                pointer->markers[position_x_in_markers][3] = false;
                pointer->markers[position_x_in_markers][4] = false;
                pointer->markers[position_x_in_markers][5] = false;
                pointer->markers[position_x_in_markers][6] = false;
                pointer->markers[position_x_in_markers][7] = false;
                pointer->markers[position_x_in_markers][8] = false;
            }
        }
    }

    //Printa os marcadores
    for (int i = 0; i < 25; i += 3)
    {
        for (int j = 0; j < 25; j += 3)
        {
            if (pointer->markers[counter_markers][0]) tela_texto(pointer->markers_matrix[i][j], markers_size, pointer->markers_color, string_numbers[0]);
            if (pointer->markers[counter_markers][1]) tela_texto(pointer->markers_matrix[i][j + 1], markers_size, pointer->markers_color, string_numbers[1]);
            if (pointer->markers[counter_markers][2]) tela_texto(pointer->markers_matrix[i][j + 2], markers_size, pointer->markers_color, string_numbers[2]);
            if (pointer->markers[counter_markers][3]) tela_texto(pointer->markers_matrix[i + 1][j], markers_size, pointer->markers_color, string_numbers[3]);
            if (pointer->markers[counter_markers][4]) tela_texto(pointer->markers_matrix[i + 1][j + 1], markers_size, pointer->markers_color, string_numbers[4]);
            if (pointer->markers[counter_markers][5]) tela_texto(pointer->markers_matrix[i + 1][j + 2], markers_size, pointer->markers_color, string_numbers[5]);
            if (pointer->markers[counter_markers][6]) tela_texto(pointer->markers_matrix[i + 2][j], markers_size, pointer->markers_color, string_numbers[6]);
            if (pointer->markers[counter_markers][7]) tela_texto(pointer->markers_matrix[i + 2][j + 1], markers_size, pointer->markers_color, string_numbers[7]);
            if (pointer->markers[counter_markers][8]) tela_texto(pointer->markers_matrix[i + 2][j + 2], markers_size, pointer->markers_color, string_numbers[8]);

            if (pointer->show_occupied_slot_message)
            {
                ponto_t message = { 103,430 };
                cor_t message_color = { 1, 1, 0, 1 };
                tela_texto(message, 20, message_color, "Essa casa ja esta preenchida!");
            }

            counter_markers++;
        }
    }
}

void put_time_passed(gamestate* pointer)
{
    pointer->time = tela_relogio();
    ponto_t time_position = { 35,60 };
    cor_t time_color = { 1, 1, 1, 1 };

    char string_time[6] = "  :  ";

    int seconds_passed = 0;
    int minutes_passed = 0;

    seconds_passed = pointer->time - pointer->initial_time;
    pointer->total_match_time = pointer->time - pointer->initial_time;

    for (int i = 1; i < 10; i++)
    {
        if (pointer->time > 60 * i)
        {
            seconds_passed = pointer->time - (60 * i);
            minutes_passed = i;
        }
    }

    string_time[0] = minutes_passed / 10 + 48;
    string_time[1] = minutes_passed % 10 + 48;
    string_time[3] = seconds_passed / 10 + 48;
    string_time[4] = seconds_passed % 10 + 48;

    tela_texto(time_position, 30, time_color, string_time);
}

void draw_match(gamestate* pointer, bool mouse_right_button_is_clicked, bool mouse_left_button_is_clicked, int mouse_x, int mouse_y)
{
    pointer->show_occupied_slot_message = false;
    pointer->show_number_already_on_minor_board_message = false;
    pointer->show_wrong_move_message = false;

    draw_grid();
    calc_numbers_coordinates(pointer);
    put_board_numbers(pointer);
    put_player_numbers(pointer, mouse_left_button_is_clicked, mouse_x, mouse_y);
    draw_exit_button();
    calc_markers_coordinates(pointer);
    put_markers(pointer, mouse_right_button_is_clicked, mouse_x, mouse_y);
}

int play_match(gamestate* pointer)
{
    while (true)
    {
        pointer->time = tela_relogio();
        if (pointer->total_match_time > 0) pointer->score = 1285 + 97418 * (pointer->difficulty_selected + 1) / pointer->total_match_time;
        if (pointer->match_end) return 0;
        if (pointer->exit) return -1;

        rato_t mouse_parameters = mouse_tracing(pointer);
        draw_match(pointer, mouse_parameters.clicado[1], mouse_parameters.clicado[0], mouse_parameters.posicao.x, mouse_parameters.posicao.y);
        mouse_tracing(pointer);
        changing_numbers(pointer, mouse_parameters.giro);
        activate_exit(pointer, mouse_parameters.posicao.x, mouse_parameters.posicao.y, mouse_parameters.clicado[0]);
        put_time_passed(pointer);
        tela_atualiza();

        if ((pointer->show_occupied_slot_message || pointer->show_number_already_on_minor_board_message) || pointer->show_wrong_move_message)
        {
            for (int i = 0; i < 400000000; i++)
            {
                //Pra dar tempo de ler a mensagem
            }
        }

        pointer->match_end = true;

        for (int i = 0; i < 81; i++)
        {
            if (pointer->player_numbers[i] == 0) pointer->match_end = false;
        }
    }
}

void order_records_array(player_record* player_pointer, player_record* new_player_pointer, int* number_of_records_pointer)
{
    bool first_place = true;

    player_pointer = realloc(player_pointer, (*number_of_records_pointer + 1) * sizeof(player_record));
    if (player_pointer == NULL) {
        printf("Erro ao realocar memória!");
        return false;
    }

    for (int i = 0; i < *number_of_records_pointer; i++)
    {
        if (new_player_pointer->player_score > player_pointer[*number_of_records_pointer - i - 1].player_score)
        {
            player_pointer[*number_of_records_pointer - i] = player_pointer[*number_of_records_pointer - i - 1];
            player_pointer[*number_of_records_pointer - i].position = number_of_records_pointer - i + 1;
            first_place = true;
        }
        else
        {
            player_pointer[*number_of_records_pointer - i] = *new_player_pointer;
            player_pointer[*number_of_records_pointer - i].position = *number_of_records_pointer - i + 1;
            new_player_pointer->position = *number_of_records_pointer - i + 1;
            first_place = false;
            break;
        }
    }

    if (first_place)
    {
        player_pointer[0] = *new_player_pointer;
        player_pointer[0].position = 1;
        new_player_pointer->position = 1;
    }
}

void overwrite_record(int* number_of_records_pointer, player_record* player_pointer)
{
    FILE* records_file = fopen("Sudoku Records.txt", "w");
    if (records_file == NULL) 
    {
        printf("Erro ao abrir arquivo de recordes!");
        return false;
    }

    fprintf(records_file, "%d\n", *number_of_records_pointer);

    for (int i = 0; i < *number_of_records_pointer; i++)
    {
        fprintf(records_file, "%d %s %d %d\n", player_pointer[i].position, player_pointer[i].player_name, player_pointer[i].board_played, player_pointer[i].player_score);
    }

    fclose(records_file);
}

void final_screen(gamestate* pointer, player_record* player_pointer, player_record* new_player_pointer)
{
    ponto_t result_coordinates = { 105, 60 };
    ponto_t score_coordinates = { 255, 60 };
    ponto_t press_any_key_coordinates = { 50, 450 };
    ponto_t general_position_coordinates = { 130, 150 };
    ponto_t podium_coordinates = { 230, 195 };
    ponto_t top_one_coordinates = { 100, 230 };
    ponto_t top_two_coordinates = { 100, 260 };
    ponto_t top_three_coordinates = { 100, 290 };
    cor_t result_color = { 1, 1, 1, 1 };

    char string_score[7] = "      ";

    int first_digit = 0;
    int second_digit = 0;
    int third_digit = 0;
    int fourth_digit = 0;
    int fifth_digit = 0;
    int sixth_digit = 0;

    int counter = 0;

    if (pointer->score > 100000)
    {
        first_digit = pointer->score / 100000;
        second_digit = pointer->score / 10000 % 10;
        third_digit = pointer->score / 1000 % 10;
        fourth_digit = pointer->score / 100 % 10;
        fifth_digit = pointer->score / 10 % 10;
        sixth_digit = pointer->score % 10;

        string_score[0] = first_digit + 48;
        string_score[1] = second_digit + 48;
        string_score[2] = third_digit + 48;
        string_score[3] = fourth_digit + 48;
        string_score[4] = fifth_digit + 48;
        string_score[5] = sixth_digit + 48;
    }
    else if (pointer->score > 10000)
    {
        first_digit = pointer->score / 10000;
        second_digit = pointer->score / 1000 % 10;
        third_digit = pointer->score / 100 % 10;
        fourth_digit = pointer->score / 10 % 10;
        fifth_digit = pointer->score % 10;

        string_score[0] = first_digit + 48;
        string_score[1] = second_digit + 48;
        string_score[2] = third_digit + 48;
        string_score[3] = fourth_digit + 48;
        string_score[4] = fifth_digit + 48;
    }
    else if (pointer->score > 1000)
    {
        first_digit = pointer->score / 1000;
        second_digit = pointer->score / 100 % 10;
        third_digit = pointer->score / 10 % 10;
        fourth_digit = pointer->score % 10;

        string_score[0] = first_digit + 48;
        string_score[1] = second_digit + 48;
        string_score[2] = third_digit + 48;
        string_score[3] = fourth_digit + 48;
    }
    else if (pointer->score > 100)
    {
        first_digit = pointer->score / 100;
        second_digit = pointer->score / 10 % 10;
        third_digit = pointer->score % 10;

        string_score[0] = first_digit + 48;
        string_score[1] = second_digit + 48;
        string_score[2] = third_digit + 48;
    }
    else if (pointer->score > 10)
    {
        first_digit = pointer->score / 10;
        second_digit = pointer->score % 10;

        string_score[0] = first_digit + 48;
        string_score[1] = second_digit + 48;
    }
    else
    {
        string_score[0] = pointer->score + 48;
    }


    char string_player_position[21] = "Posicao geral:       ";



    if (new_player_pointer->position > 100000)
    {
        first_digit = new_player_pointer->position / 100000;
        second_digit = new_player_pointer->position / 10000 % 10;
        third_digit = new_player_pointer->position / 1000 % 10;
        fourth_digit = new_player_pointer->position / 100 % 10;
        fifth_digit = new_player_pointer->position / 10 % 10;
        sixth_digit = new_player_pointer->position % 10;

        string_player_position[15] = first_digit + 48;
        string_player_position[16] = second_digit + 48;
        string_player_position[17] = third_digit + 48;
        string_player_position[18] = fourth_digit + 48;
        string_player_position[19] = fifth_digit + 48;
        string_player_position[20] = sixth_digit + 48;
    }
    else if (new_player_pointer->position > 10000)
    {
        first_digit = new_player_pointer->position / 10000;
        second_digit = new_player_pointer->position / 1000 % 10;
        third_digit = new_player_pointer->position / 100 % 10;
        fourth_digit = new_player_pointer->position / 10 % 10;
        fifth_digit = new_player_pointer->position % 10;

        string_player_position[15] = first_digit + 48;
        string_player_position[16] = second_digit + 48;
        string_player_position[17] = third_digit + 48;
        string_player_position[18] = fourth_digit + 48;
        string_player_position[19] = fifth_digit + 48;
    }
    else if (pointer->score > 1000)
    {
        first_digit = new_player_pointer->position / 1000;
        second_digit = new_player_pointer->position / 100 % 10;
        third_digit = new_player_pointer->position / 10 % 10;
        fourth_digit = new_player_pointer->position % 10;

        string_player_position[15] = first_digit + 48;
        string_player_position[16] = second_digit + 48;
        string_player_position[17] = third_digit + 48;
        string_player_position[18] = fourth_digit + 48;
    }
    else if (pointer->score > 100)
    {
        first_digit = new_player_pointer->position / 100;
        second_digit = new_player_pointer->position / 10 % 10;
        third_digit = new_player_pointer->position % 10;

        string_player_position[15] = first_digit + 48;
        string_player_position[16] = second_digit + 48;
        string_player_position[17] = third_digit + 48;
    }
    else if (pointer->score > 10)
    {
        first_digit = new_player_pointer->position / 10;
        second_digit = new_player_pointer->position % 10;

        string_player_position[15] = first_digit + 48;
        string_player_position[16] = second_digit + 48;
    }
    else
    {
        string_player_position[15] = new_player_pointer->position + 48;
    }



    char first_place[37] = "Primeiro Lugar:                    ";
    int index = 0;

    while (player_pointer[index].player_name[counter] != '\0')
    {
        first_place[16 + counter] = player_pointer[index].player_name[counter];
        counter++;
    }

    first_place[16 + counter] = ' ';
    first_place[16 + counter + 1] = '-';

    if (player_pointer[index].player_score > 100000)
    {
        first_digit = player_pointer[index].player_score / 100000;
        second_digit = player_pointer[index].player_score / 10000 % 10;
        third_digit = player_pointer[index].player_score / 1000 % 10;
        fourth_digit = player_pointer[index].player_score / 100 % 10;
        fifth_digit = player_pointer[index].player_score / 10 % 10;
        sixth_digit = player_pointer[index].player_score % 10;

        first_place[16 + counter + 3] = first_digit + 48;
        first_place[16 + counter + 4] = second_digit + 48;
        first_place[16 + counter + 5] = third_digit + 48;
        first_place[16 + counter + 6] = fourth_digit + 48;
        first_place[16 + counter + 7] = fifth_digit + 48;
        first_place[16 + counter + 8] = sixth_digit + 48;
    }
    else if (player_pointer[index].player_score > 10000)
    {
        first_digit = player_pointer[index].player_score / 10000;
        second_digit = player_pointer[index].player_score / 1000 % 10;
        third_digit = player_pointer[index].player_score / 100 % 10;
        fourth_digit = player_pointer[index].player_score / 10 % 10;
        fifth_digit = player_pointer[index].player_score % 10;

        first_place[16 + counter + 3] = first_digit + 48;
        first_place[16 + counter + 4] = second_digit + 48;
        first_place[16 + counter + 5] = third_digit + 48;
        first_place[16 + counter + 6] = fourth_digit + 48;
        first_place[16 + counter + 7] = fifth_digit + 48;
    }
    else if (player_pointer[index].player_score > 1000)
    {
        first_digit = player_pointer[index].player_score / 1000;
        second_digit = player_pointer[index].player_score / 100 % 10;
        third_digit = player_pointer[index].player_score / 10 % 10;
        fourth_digit = player_pointer[index].player_score % 10;

        first_place[16 + counter + 3] = first_digit + 48;
        first_place[16 + counter + 4] = second_digit + 48;
        first_place[16 + counter + 5] = third_digit + 48;
        first_place[16 + counter + 6] = fourth_digit + 48;
    }
    else if (player_pointer[index].player_score > 100)
    {
        first_digit = player_pointer[index].player_score / 100;
        second_digit = player_pointer[index].player_score / 10 % 10;
        third_digit = player_pointer[index].player_score % 10;

        first_place[16 + counter + 3] = first_digit + 48;
        first_place[16 + counter + 4] = second_digit + 48;
        first_place[16 + counter + 5] = third_digit + 48;
    }
    else if (player_pointer[index].player_score > 10)
    {
        first_digit = player_pointer[index].player_score / 10;
        second_digit = player_pointer[index].player_score % 10;

        first_place[16 + counter + 3] = first_digit + 48;
        first_place[16 + counter + 4] = second_digit + 48;
    }
    else
    {
        first_place[16 + counter + 3] = first_digit + 48;
    }

    counter = 0;

    char second_place[36] = "Segundo Lugar:                     ";
    index = 1;

    while (player_pointer[index].player_name[counter] != '\0')
    {
        second_place[15 + counter] = player_pointer[index].player_name[counter];
        counter++;
    }

    second_place[15 + counter + 1] = '-';

    if (player_pointer[index].player_score > 100000)
    {
        first_digit = player_pointer[index].player_score / 100000;
        second_digit = player_pointer[index].player_score / 10000 % 10;
        third_digit = player_pointer[index].player_score / 1000 % 10;
        fourth_digit = player_pointer[index].player_score / 100 % 10;
        fifth_digit = player_pointer[index].player_score / 10 % 10;
        sixth_digit = player_pointer[index].player_score % 10;

        second_place[15 + counter + 3] = first_digit + 48;
        second_place[15 + counter + 4] = second_digit + 48;
        second_place[15 + counter + 5] = third_digit + 48;
        second_place[15 + counter + 6] = fourth_digit + 48;
        second_place[15 + counter + 7] = fifth_digit + 48;
        second_place[15 + counter + 8] = sixth_digit + 48;
    }
    else if (player_pointer[index].player_score > 10000)
    {
        first_digit = player_pointer[index].player_score / 10000;
        second_digit = player_pointer[index].player_score / 1000 % 10;
        third_digit = player_pointer[index].player_score / 100 % 10;
        fourth_digit = player_pointer[index].player_score / 10 % 10;
        fifth_digit = player_pointer[index].player_score % 10;

        second_place[15 + counter + 3] = first_digit + 48;
        second_place[15 + counter + 4] = second_digit + 48;
        second_place[15 + counter + 5] = third_digit + 48;
        second_place[15 + counter + 6] = fourth_digit + 48;
        second_place[15 + counter + 7] = fifth_digit + 48;
    }
    else if (player_pointer[index].player_score > 1000)
    {
        first_digit = player_pointer[index].player_score / 1000;
        second_digit = player_pointer[index].player_score / 100 % 10;
        third_digit = player_pointer[index].player_score / 10 % 10;
        fourth_digit = player_pointer[index].player_score % 10;

        second_place[15 + counter + 3] = first_digit + 48;
        second_place[15 + counter + 4] = second_digit + 48;
        second_place[15 + counter + 5] = third_digit + 48;
        second_place[15 + counter + 6] = fourth_digit + 48;
    }
    else if (player_pointer[index].player_score > 100)
    {
        first_digit = player_pointer[index].player_score / 100;
        second_digit = player_pointer[index].player_score / 10 % 10;
        third_digit = player_pointer[index].player_score % 10;

        second_place[15 + counter + 3] = first_digit + 48;
        second_place[15 + counter + 4] = second_digit + 48;
        second_place[15 + counter + 5] = third_digit + 48;
    }
    else if (player_pointer[index].player_score > 10)
    {
        first_digit = player_pointer[index].player_score / 10;
        second_digit = player_pointer[index].player_score % 10;

        second_place[15 + counter + 3] = first_digit + 48;
        second_place[15 + counter + 4] = second_digit + 48;
    }
    else
    {
        second_place[15 + counter + 3] = first_digit + 48;
    }

    counter = 0;

    char third_place[37] = "Terceiro Lugar:                    ";
    index = 2;

    while (player_pointer[index].player_name[counter] != '\0')
    {
        third_place[16 + counter] = player_pointer[index].player_name[counter];
        counter++;
    }

    third_place[16 + counter + 1] = '-';

    if (player_pointer[index].player_score > 100000)
    {
        first_digit = player_pointer[index].player_score / 100000;
        second_digit = player_pointer[index].player_score / 10000 % 10;
        third_digit = player_pointer[index].player_score / 1000 % 10;
        fourth_digit = player_pointer[index].player_score / 100 % 10;
        fifth_digit = player_pointer[index].player_score / 10 % 10;
        sixth_digit = player_pointer[index].player_score % 10;

        third_place[16 + counter + 3] = first_digit + 48;
        third_place[16 + counter + 4] = second_digit + 48;
        third_place[16 + counter + 5] = third_digit + 48;
        third_place[16 + counter + 6] = fourth_digit + 48;
        third_place[16 + counter + 7] = fifth_digit + 48;
        third_place[16 + counter + 8] = sixth_digit + 48;
    }
    else if (player_pointer[index].player_score > 10000)
    {
        first_digit = player_pointer[index].player_score / 10000;
        second_digit = player_pointer[index].player_score / 1000 % 10;
        third_digit = player_pointer[index].player_score / 100 % 10;
        fourth_digit = player_pointer[index].player_score / 10 % 10;
        fifth_digit = player_pointer[index].player_score % 10;

        third_place[16 + counter + 3] = first_digit + 48;
        third_place[16 + counter + 4] = second_digit + 48;
        third_place[16 + counter + 5] = third_digit + 48;
        third_place[16 + counter + 6] = fourth_digit + 48;
        third_place[16 + counter + 7] = fifth_digit + 48;
    }
    else if (player_pointer[index].player_score > 1000)
    {
        first_digit = player_pointer[index].player_score / 1000;
        second_digit = player_pointer[index].player_score / 100 % 10;
        third_digit = player_pointer[index].player_score / 10 % 10;
        fourth_digit = player_pointer[index].player_score % 10;

        third_place[16 + counter + 3] = first_digit + 48;
        third_place[16 + counter + 4] = second_digit + 48;
        third_place[16 + counter + 5] = third_digit + 48;
        third_place[16 + counter + 6] = fourth_digit + 48;
    }
    else if (player_pointer[index].player_score > 100)
    {
        first_digit = player_pointer[index].player_score / 100;
        second_digit = player_pointer[index].player_score / 10 % 10;
        third_digit = player_pointer[index].player_score % 10;

        third_place[16 + counter + 3] = first_digit + 48;
        third_place[16 + counter + 4] = second_digit + 48;
        third_place[16 + counter + 5] = third_digit + 48;
    }
    else if (player_pointer[index].player_score > 10)
    {
        first_digit = player_pointer[index].player_score / 10;
        second_digit = player_pointer[index].player_score % 10;

        third_place[16 + counter + 3] = first_digit + 48;
        third_place[16 + counter + 4] = second_digit + 48;
    }
    else
    {
        third_place[16 + counter + 3] = first_digit + 48;
    }

    counter = 0;

    while (true)
    {
        tela_texto(result_coordinates, 40, result_color, "Pontos: ");
        tela_texto(score_coordinates, 40, result_color, string_score);
        tela_texto(general_position_coordinates, 25, result_color, string_player_position);
        tela_texto(podium_coordinates, 22, result_color, "Top 3");
        tela_texto(top_one_coordinates, 20, result_color, first_place);
        tela_texto(top_two_coordinates, 20, result_color, second_place);
        tela_texto(top_three_coordinates, 20, result_color, third_place);
        tela_texto(press_any_key_coordinates, 20, result_color, "Pressione qualquer tecla para continuar");

        char acao = tela_tecla();
        if (acao != '\0') break;

        tela_atualiza();
    }
}


























static void cai_fora(char* msg)
{
    int cai = 42;
    int fora = 42;
    fprintf(stderr, "\n\nERRO\n%s\n\n", msg);
    assert(cai - fora);
}

static void tela_inicializa_janela(tamanho_t tamanho, char nome[])
{
    // pede para tentar linhas mais suaves (multisampling)
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    // cria uma janela
    ALLEGRO_DISPLAY* janela = al_create_display(tamanho.largura, tamanho.altura);
    if (janela == NULL) {
        cai_fora("problema na criação de janela do allegro");
    }
    // esconde o cursor do mouse
    al_hide_mouse_cursor(janela);
    al_set_window_title(janela, nome);
}

static ALLEGRO_COLOR conv_cor(cor_t cor)
{
    return (ALLEGRO_COLOR) { cor.vermelho, cor.verde, cor.azul, cor.opacidade };
}

// fila para receber os eventos do teclado
static ALLEGRO_EVENT_QUEUE* tela_eventos_teclado;
static void tela_inicializa_teclado(void)
{
    if (!al_install_keyboard()) {
        cai_fora("problema na inicialização do teclado do allegro");
    }
    // cria e inicializa a fila de eventos do teclado
    tela_eventos_teclado = al_create_event_queue();
    if (tela_eventos_teclado == NULL) {
        cai_fora("problema na criação da fila de eventos do teclado do allegro");
    }
    al_register_event_source(tela_eventos_teclado,
        al_get_keyboard_event_source());
}

void tela_inicio(tamanho_t tamanho, char nome[])
{
    // inicializa os subsistemas do allegro
    if (!al_init()) cai_fora("problema na inicialização do allegro");
    if (!al_install_mouse()) cai_fora("problema na inicialização do mouse do allegro");
    if (!al_init_primitives_addon()) cai_fora("problema na inicialização de addons do allegro");
    al_init_font_addon();
    if (!al_init_ttf_addon()) cai_fora("problema na inicialização do addon de fontes ttf");

    // inicializa a tela
    tela_inicializa_janela(tamanho, nome);
    tela_inicializa_teclado();
}


void tela_fim(void)
{
    // badabum!
    al_uninstall_system();
}

void tela_atualiza(void)
{
    static double tempo_ultima_tela = 0;
    double agora = tela_relogio();
    double quando_mostrar = tempo_ultima_tela + SEGUNDOS_POR_QUADRO;
    double tempo_ate_mostrar = quando_mostrar - agora;
    if (tempo_ate_mostrar > 0) {
        // é muito cedo, dá uma cochilada
        al_rest(tempo_ate_mostrar);
    }
    // troca a tela mostrada pela que foi desenhada em memória
    al_flip_display();
    tempo_ultima_tela = tela_relogio();

    // limpa todo o canvas em memória, para desenhar a próxima tela
    cor_t preto = { 0, 0, 0, 1 };
    al_clear_to_color(conv_cor(preto));
}


void tela_circulo(circulo_t circulo, float largura, cor_t cor_linha, cor_t cor_interna)
{
    // preenche
    al_draw_filled_circle(circulo.centro.x, circulo.centro.y, circulo.raio,
        conv_cor(cor_interna));
    // faz o contorno
    al_draw_circle(circulo.centro.x, circulo.centro.y, circulo.raio,
        conv_cor(cor_linha), largura);
}

void tela_linha(ponto_t inicio, ponto_t fim, float largura, cor_t cor_linha)
{
    al_draw_line(inicio.x, inicio.y, fim.x, fim.y, conv_cor(cor_linha), largura);
}

void tela_retangulo(retangulo_t retangulo, float largura,
    cor_t cor_linha, cor_t cor_interna)
{
    int x1 = retangulo.inicio.x;
    int y1 = retangulo.inicio.y;
    int x2 = retangulo.inicio.x + retangulo.tamanho.largura;
    int y2 = retangulo.inicio.y + retangulo.tamanho.altura;
    al_draw_filled_rectangle(x1, y1, x2, y2, conv_cor(cor_interna));
    al_draw_rectangle(x1, y1, x2, y2, conv_cor(cor_linha), largura);
}

// retorna uma fonte com o tamanho pedido
#define N_FONTES 5  // número de fontes a guardar
static ALLEGRO_FONT* tela_fonte(int tam)
{
    static struct {
        int tamanho;
        ALLEGRO_FONT* fonte;
    } fontes[N_FONTES] = { { 0, NULL } };
    static int prox_indice = 0;

    for (int i = 0; i < N_FONTES; i++) {
        if (fontes[i].tamanho == tam) return fontes[i].fonte;
    }

    // carrega uma fonte, para poder escrever na tela
    ALLEGRO_FONT* fonte = al_load_font("DejaVuSans.ttf", tam, 0);
    if (fonte == NULL) {
        al_uninstall_system();
        fprintf(stderr,
            "\n\nERRO FATAL\n"
            "O ARQUIVO COM O DESENHO DAS LETRAS (DejaVuSans.ttf) NAO FOI ENCONTRADO.\n"
            "COPIE ESSE ARQUIVO, OU MUDE telag2.c PARA USAR UM QUE EXISTA.\n"
            " (pode ser tambem que tela_inicio() nao tenha sido chamada)\n");
        exit(1);
    }

    // se ja tinha uma fonte carregada, livra-se dela antes de carregar outra
    if (fontes[prox_indice].fonte != NULL) {
        al_destroy_font(fontes[prox_indice].fonte);
    }
    fontes[prox_indice].fonte = fonte;
    fontes[prox_indice].tamanho = tam;
    prox_indice++;
    if (prox_indice >= N_FONTES) {
        prox_indice = 0;
    }

    return fonte;
}

void tela_texto(ponto_t posicao, int tamanho, cor_t cor, char texto[])
{
    ALLEGRO_FONT* fonte = tela_fonte(tamanho);
    posicao.y -= al_get_font_ascent(fonte);
    al_draw_text(fonte, conv_cor(cor), posicao.x, posicao.y, ALLEGRO_ALIGN_LEFT, texto);
}

retangulo_t tela_texto_contorno(ponto_t posicao, int tamanho, char texto[])
{
    ALLEGRO_FONT* fonte = tela_fonte(tamanho);
    retangulo_t ret;
    int rx, ry, rw, rh;
    al_get_text_dimensions(fonte, texto, &rx, &ry, &rw, &rh);
    ret.inicio.x = posicao.x + rx;
    ret.inicio.y = posicao.y + ry - al_get_font_ascent(fonte);
    ret.tamanho.largura = rw;
    ret.tamanho.altura = rh;
    return ret;
}

rato_t tela_rato()
{
    ALLEGRO_MOUSE_STATE mouse;
    rato_t rato;
    al_get_mouse_state(&mouse);
    rato.posicao.x = al_get_mouse_state_axis(&mouse, 0);
    rato.posicao.y = al_get_mouse_state_axis(&mouse, 1);

    static int ultimo_giro = INT_MAX;
    int giro = al_get_mouse_state_axis(&mouse, 2);
    if (ultimo_giro == INT_MAX) ultimo_giro = giro;
    rato.giro = ultimo_giro - giro;
    ultimo_giro = giro;

    static bool ultimo_apertado[3] = { false, false, false };
    for (int b = 0; b < 3; b++) {
        rato.apertado[b] = al_mouse_button_down(&mouse, b + 1);
        rato.clicado[b] = ultimo_apertado[b] && !rato.apertado[b];
        ultimo_apertado[b] = rato.apertado[b];
    }

    return rato;
}

static bool t_shift, t_control;

bool tela_shift() { return t_shift; }
bool tela_control() { return t_control; }

char tela_tecla(void)
{
    ALLEGRO_EVENT ev;

    while (al_get_next_event(tela_eventos_teclado, &ev)) {
        t_shift = ev.keyboard.modifiers & ALLEGRO_KEYMOD_SHIFT;
        t_control = ev.keyboard.modifiers & ALLEGRO_KEYMOD_CTRL;
        if (ev.type == ALLEGRO_EVENT_KEY_CHAR) {
            int k = ev.keyboard.keycode;
            switch (k) {
            case ALLEGRO_KEY_ENTER:     return '\n';
            case ALLEGRO_KEY_BACKSPACE: return '\b';
            }
            int c = ev.keyboard.unichar;
            return c;
        }
    }
    // nada foi pressionado (ou foi pressionado algo não imprimível)
    return '\0';
}


double tela_relogio(void)
{
    return al_get_time();
}
