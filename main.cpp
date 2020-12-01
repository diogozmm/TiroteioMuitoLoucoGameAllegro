/***************************************************************
*   TITULO: TIROTEIO MUITO LOUCO!                              *
*   ALUNOS: DIOGO ZIMMERMANN | EMANOEL FRANCISCO DOS PASSOS    *
*   VERSÃO 0.1                                                 *
*   TURMA: 722                                                 *
***************************************************************/


#include <stdio.h>
#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "Player.h"
#include "Outros.h"
#include "functions.h"



//GLOBALS==============================
const int WIDTH = 1024; //LARGURA DA TELA
const int HEIGHT = 768; //ALTURA DA TELA
const int NUM_PLAYER = 2; //NUMERO DE JOGADORES
const int NUM_TIROS =777 ; //NUMERO MÁXIMO DE TIROS NA TELA
const int NUM_OBS = 9; //NUMERO MÁXIMO DE PEDRAS ROLANDO NA TELA AO MESMO TEMPO
const int NUM_BARR = 10; //NUMERO MÁXIMO DE BARREIRAS
enum STATE{TITULO, JOGANDO, P1PERDEU, P2PERDEU}; //ESTADOS DO JOGO
enum KEYS {UP, DOWN, LEFT, RIGHT, W, A, S, D, SPACE, Z, ENTER}; //TECLAS DO JOGO
bool keys[11] = {false, false, false, false, false, false, false, false, false, false, false};


/*****************************
*  VARIÁVEIS DAS ESTRUTURAS  *
*****************************/

Jogador player [NUM_PLAYER];
AtaqueBase1 golpe1 [NUM_TIROS];
AtaqueBase2 golpe2 [NUM_TIROS];
Hole buraco [NUM_OBS];
Barreiras obstaculo [NUM_BARR];

/******************************************
*   INICIALIZAÇÃO DOS PONTEIROS DE SONS   *
******************************************/

ALLEGRO_SAMPLE *tiro = NULL;
ALLEGRO_SAMPLE *menu = NULL;
ALLEGRO_SAMPLE *luta = NULL;
ALLEGRO_SAMPLE *lost = NULL;
ALLEGRO_SAMPLE_INSTANCE *instanciamenu = NULL;
ALLEGRO_SAMPLE_INSTANCE *instancialuta = NULL;
ALLEGRO_SAMPLE_INSTANCE *instancialost = NULL;

int main(void)
{
    bool done = false;      // VARIÁVEL DE FIM DO JOGO
    bool redraw = true;
    const int FPS = 60;
	int state = -1;

	Background BG; //VARIÁVEL PARA A ESTRUTURA DE FUNDO


     /*************************
     *  VARIÁVEIS DO ALLEGRO  *
     *************************/

    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_FONT *font18 = NULL;
    ALLEGRO_BITMAP *jogadorimagem;
   	ALLEGRO_BITMAP *buracoImagem;
   	ALLEGRO_BITMAP *barreiraImagem;
   	ALLEGRO_BITMAP *titulo = NULL;
	ALLEGRO_BITMAP *p1perdeu = NULL;
	ALLEGRO_BITMAP *p2perdeu = NULL;
    ALLEGRO_BITMAP *bgImage = NULL;




    //FUNÇÕES DE INICIALIZAÇÃO

    if(!al_init())										//INICIALIZAÇÃO DO ALLEGRO
        return -1;

    display = al_create_display(WIDTH, HEIGHT);			//CRIA O DISPLAY

    if(!display)										//TESTA O DISPLAY
        return -1;

    /********************************
    *   INICIALIZAÇÃO DOS ADDONS    *
    ********************************/

    al_init_primitives_addon();
    al_install_keyboard();
    al_init_font_addon();
    al_init_ttf_addon();
	al_init_image_addon();
	al_install_audio();
	al_init_acodec_addon();

    event_queue = al_create_event_queue(); //CRIA A FILA DE EVENTOS
    timer = al_create_timer(1.0 / FPS); //CRIA O TIMER

    jogadorimagem = al_load_bitmap("imagens/Player.png"); // CRIA/CARREGA A SPRITE DOS JOGADORES
	al_convert_mask_to_alpha(jogadorimagem, al_map_rgb(77, 75, 118)); //"EXCLUI" O FUNDO DA IMAGEM

    buracoImagem = al_load_bitmap("imagens/rock.png"); //CRIA/CARREGA A SPRITE DAS PEDRAS QUE ROLAM
    al_convert_mask_to_alpha(buracoImagem, al_map_rgb(255, 255, 255));


    barreiraImagem = al_load_bitmap("imagens/Zen-Rock-Sprite.png"); //CRIA/CARREGA A SPRTE DAS BARREIRAS
   	al_convert_mask_to_alpha(barreiraImagem, al_map_rgb(255, 255, 255));


    titulo = al_load_bitmap("imagens/titulo.png"); //CRIA/CARREGA A IMAGEM DE TITULO
	p1perdeu = al_load_bitmap("imagens/p1lost.png"); //CRIA/CARREGA A IMAGEM DE DERROTA1
    p2perdeu = al_load_bitmap("imagens/p2lost.png"); //CRIA/CARREGA A IMAGEM DE DERROTA2

    bgImage = al_load_bitmap("imagens/grama.jpg"); //CRIA/CARREGA A IMAGEM DE FUNDO

    al_reserve_samples(10); // RESERVA UM ESPAÇO PARA OS SONS

    tiro = al_load_sample("sons/shot.wav"); //SOM DOS TIROS
    menu = al_load_sample("sons/MenuMusica.ogg"); //MUSICA DO MENU
    luta = al_load_sample("sons/musicabatalha.ogg"); // MUSICA DA LUTA
    lost = al_load_sample("sons/musicalost.ogg"); //MUSSICA DE GAMEOVER


    /************************************************************
    *   CRIA AS INSTANCIAS DE SONS QUE SERÃO TOCADAS EM LOOP    *
    ************************************************************/

    instanciamenu = al_create_sample_instance(menu);
	al_set_sample_instance_playmode(instanciamenu, ALLEGRO_PLAYMODE_LOOP);

	al_attach_sample_instance_to_mixer(instanciamenu, al_get_default_mixer());

	instancialuta = al_create_sample_instance(luta);
    al_set_sample_instance_playmode(instancialuta, ALLEGRO_PLAYMODE_LOOP);

    al_attach_sample_instance_to_mixer(instancialuta, al_get_default_mixer());

    instancialost = al_create_sample_instance(lost);
    al_set_sample_instance_playmode(instancialost, ALLEGRO_PLAYMODE_LOOP);

    al_attach_sample_instance_to_mixer(instancialost, al_get_default_mixer());



    srand(time(NULL));

    MudarEstado(state, TITULO); // MUDA O INICIO DO JOGO PARA O ESTADO TÍTULO

    InicJogador(player, jogadorimagem);            //Iniciando o jogador;
    InicAtaqueBaseJ1(golpe1, NUM_TIROS);           //INICIALIZA O ATAQUE1
    InicAtaqueBaseJ2(golpe2, NUM_TIROS);           //INICIALIZA O ATAQUE2
    InicBarreira(obstaculo, NUM_BARR, barreiraImagem); //INICIALIZA AS BARREIRAS
    ComecBuracos(buraco, NUM_OBS, buracoImagem); //INICIALIZA AS PEDRAS

    InicFundo(BG, 0, 0, 1, 0, 1024, 1024, -1, 1, bgImage); //INICIALIZA FUNDO



    font18 = al_load_font("veteran typewriter.ttf", 18, 0); //INICIALIZA A FONTE



    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_display_event_source(display));

    al_start_timer(timer);
    while(!done)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if(ev.type == ALLEGRO_EVENT_TIMER)
        {
            redraw = true;
            if(keys[UP])
                MoverJogador2Cima(player);
            else if(keys[DOWN])
                MoverJogador2Baixo(player);
            else
            if(keys[LEFT])
                MoverJogador2Esquerda(player);
            else if(keys[RIGHT])
                MoverJogador2Direita(player);
            else
                ResetAnimJogador(player, 2);        //RESETA A ANIMAÇÃO DO JOGADOR2 PARA A INICIAL
            if(keys[W])
                MoverJogador1Cima(player);
            else if(keys[S])
                MoverJogador1Baixo(player);
            else
            if(keys[A])
                MoverJogador1Esquerda(player);
            else if(keys[D])
                MoverJogador1Direita(player);
            else
                ResetAnimJogador(player, 1);        //RESETA A ANIMAÇÃO DO JOGADOR1 PARA A INICIAL


            if(state == TITULO)
			{
                al_play_sample_instance(instanciamenu); //TOCA A MUSICA DE MENU

			}

            else if(state == JOGANDO)
            {
                InicBuracos(buraco, NUM_OBS,  player);      //COMEÇA A APARECER AS PEDRAS
                UpdateAtaqueBaseJ1(player, golpe1, NUM_TIROS); //ATUALIZA O ESTADO DOS TIROS DO PLAYER1
                UpdateAtaqueBaseJ2(player, golpe2, NUM_TIROS); //ATUALIZA O ESTADO DOS TIROS DO PLAYER2
                UpdateBuracos(buraco, NUM_OBS); //ATUALIZA O ESTADO DAS PEDRAS ROLANTES

                /************************
                *   LÓGICA DE COLISÕES  *
                ************************/

                ColisaoBuracos(buraco, NUM_OBS, player);
                ColisaoJogadores(player);
                ColisaoAtaquebaseJ1(golpe1, NUM_TIROS, player);
                ColisaoAtaquebaseJ2(golpe2, NUM_TIROS, player);
                ColisaoBarreiraJogador(obstaculo, player, NUM_BARR);
                ColisaoBarreiraAtaqueBaseJ1(obstaculo, golpe1, NUM_BARR, NUM_TIROS);
                ColisaoBarreiraAtaqueBaseJ2(obstaculo, golpe2, NUM_BARR, NUM_TIROS);
                ColisaoBarreiraPedra(obstaculo, buraco, NUM_BARR, NUM_OBS);
                ColisaoAtaqueBaseJ1Pedra(golpe1, buraco, NUM_TIROS, NUM_OBS);
                ColisaoAtaqueBaseJ2Pedra(golpe2, buraco, NUM_TIROS, NUM_OBS);

                /***********************/

                /********************************
                *   CONDIÇÕES DE FIM DE JOGO    *
                ********************************/
                if (player[1].vidas <= 0)
                    MudarEstado(state, P2PERDEU);
                if (player[0].vidas <= 0)
                    MudarEstado(state, P1PERDEU);
            }

                else if(state == P1PERDEU)
                    {}
                else if(state == P2PERDEU)
                    {}

        }

        else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            done = true;
        }

        else if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            switch(ev.keyboard.keycode)
            {
            case ALLEGRO_KEY_ESCAPE:
                done = true;
                break;
                case ALLEGRO_KEY_UP:
                keys[UP] = true;
                break;
            case ALLEGRO_KEY_DOWN:
                keys[DOWN] = true;
                break;
            case ALLEGRO_KEY_LEFT:
                keys[LEFT] = true;
                break;
            case ALLEGRO_KEY_RIGHT:
                keys[RIGHT] = true;
                break;
            case ALLEGRO_KEY_SPACE:
                keys[SPACE] = true;
                if (state == JOGANDO)
                    AtaqueBaseJ2(player, golpe2, NUM_TIROS);
                break;
            case ALLEGRO_KEY_ENTER:
                keys[ENTER] = true;
                if (state == TITULO)
                    MudarEstado(state, JOGANDO);    //MUDA ESTDO PARA O JOGO
                else if (state == P1PERDEU)
                    MudarEstado(state, TITULO); //MUDA ESTADO PARA O MENU INICIAL
                else if (state == P2PERDEU)
                    MudarEstado(state, TITULO);
                    break;
            case ALLEGRO_KEY_W:
                keys[W] = true;
                break;
            case ALLEGRO_KEY_A:
                keys[A] = true;
                break;
            case ALLEGRO_KEY_S:
                keys[S] = true;
                break;
            case ALLEGRO_KEY_D:
                keys[D] = true;
                break;
            case ALLEGRO_KEY_Z:
                AtaqueBaseJ1(player, golpe1, NUM_TIROS);
                keys[Z] = true;
                break;

                }
        }
        else if(ev.type == ALLEGRO_EVENT_KEY_UP)
        {
            switch(ev.keyboard.keycode)
            {
            case ALLEGRO_KEY_ESCAPE:
                done = true;
                break;
                case ALLEGRO_KEY_UP:
                keys[UP] = false;
                break;
            case ALLEGRO_KEY_DOWN:
                keys[DOWN] = false;
                break;
            case ALLEGRO_KEY_LEFT:
                keys[LEFT] = false;
                break;
            case ALLEGRO_KEY_RIGHT:
                keys[RIGHT] = false;
                break;
            case ALLEGRO_KEY_SPACE:
                keys[SPACE] = false;
                break;
            case ALLEGRO_KEY_ENTER:
                keys[ENTER] = false;
                break;
             case ALLEGRO_KEY_W:
                keys[W] = false;
                break;
            case ALLEGRO_KEY_A:
                keys[A] = false;
                break;
            case ALLEGRO_KEY_S:
                keys[S] = false;
                break;
            case ALLEGRO_KEY_D:
                keys[D] = false;
                break;
            case ALLEGRO_KEY_Z:
                keys[Z] = false;
                break;

                }
        }

        if(redraw && al_is_event_queue_empty(event_queue))
        {
            redraw = false;

            if(state == TITULO)
                {
                    al_draw_bitmap(titulo, 0, 0, 0);    //DESENHA A IMAGEM DE TÍTULO
                }

            else if(state == JOGANDO)
            {
                /********************************
                *   DESENHA OS OBJETOS DO JOGO  *
                ********************************/

                DesenhaFundo(BG);
                DesenhaJogador(player);
                DesenhaBuracos(buraco, 9);
                DesenhaAtaqueBaseJ1(golpe1, NUM_TIROS);
                DesenhaAtaqueBaseJ2(golpe2, NUM_TIROS);
                DesenhaBarreira(obstaculo, NUM_BARR);

                al_draw_textf(font18, al_map_rgb(255,255,0), 5, 5, 0, "Player1 tem %i vida.",player[0].vidas);
                al_draw_textf(font18, al_map_rgb(255,255,0), (WIDTH - 200), 5, 0,"Player2 tem %i vidas.",player[1].vidas);
            }

            else if(state == P1PERDEU)
            {
                al_draw_bitmap(p1perdeu, 0, 0, 0); //DESENHA IMAGEM DE DERROTA DO JOGADOR1
            }

            else if(state == P2PERDEU)
            {
                al_draw_bitmap(p2perdeu, 0, 0, 0); //DESENHA IMAGEM DE DERROTA DO JOGADOR2
            }

            al_flip_display();
            al_clear_to_color(al_map_rgb(0,0,0));
        }
    }

    al_destroy_sample(tiro);
    al_destroy_sample(menu);
    al_destroy_sample(lost);
    al_destroy_bitmap(titulo);
    al_destroy_bitmap(p1perdeu);
    al_destroy_bitmap(p2perdeu);
   	al_destroy_bitmap(jogadorimagem);
   	al_destroy_bitmap(buracoImagem);
    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);
    al_destroy_font(font18);
    al_destroy_display(display);

    return 0;

}

/***********************************************************
*  MUDANÇA DE ESTADOS                                      *
*  Esta função altera o estado em que o jogo se encontra.  *
***********************************************************/

void MudarEstado(int &state, int novoState)
{

	if(state == TITULO)
	{
        al_play_sample_instance(instanciamenu);

	}
	else if(state == JOGANDO)
	{
        al_play_sample_instance(instancialuta);
        al_stop_sample_instance(instanciamenu);

	}
	else if(state == P1PERDEU)
	{
        al_stop_sample_instance(instanciamenu);
        al_stop_sample_instance(instancialuta);
        al_play_sample_instance(instancialost);


	}
	else if(state == P2PERDEU)
    {
        al_stop_sample_instance(instanciamenu);
        al_stop_sample_instance(instancialuta);
        al_play_sample_instance(instancialost);
    }

	state = novoState;

	if(state == TITULO)
	{
        al_stop_sample_instance(instancialuta);
        al_stop_sample_instance(instancialost);
        al_play_sample_instance(instanciamenu);


	}
	else if(state == JOGANDO)
	{
        InicJogador(player);
        InicAtaqueBaseJ1(golpe1, NUM_TIROS);
        InicAtaqueBaseJ2(golpe2, NUM_TIROS);
        InicBarreira(obstaculo, NUM_BARR);
        ComecBuracos(buraco, NUM_OBS);

        al_play_sample_instance(instancialuta);
        al_stop_sample_instance(instanciamenu);
        al_stop_sample_instance(instancialost);


	}
	else if(state == P1PERDEU)
	{
        al_stop_sample_instance(instanciamenu);
        al_stop_sample_instance(instancialuta);
        al_play_sample_instance(instancialost);
	}
	else if(state == P2PERDEU)
    {
        al_stop_sample_instance(instanciamenu);
        al_stop_sample_instance(instancialuta);
        al_play_sample_instance(instancialost);
    }
}

/****************************************************
*   INICIALIZA OS JOGADORES                         *
*   Dá valores as variáveis da estrutura JOGADOR    *
****************************************************/
void InicJogador(Jogador player[], ALLEGRO_BITMAP *image) {

//JOGADOR 1:
    player[0].x = 50;
    player[0].y = HEIGHT/2;
    player[0].vidas = 300;
    player[0].mov = 10;
    player[0].boundx = 15;
    player[0].boundy = 30;

    player[0].maxFrame = 8;
	player[0].curFrame = 0;
	player[0].frameCount = 0;
	player[0].frameDelay = 6;
	player[0].frameWidth = 50;
	player[0].frameHeight = 50;
	player[0].animationColumns = 8;
	player[0].animationDirection = 1;

	player[0].animationRow = 0;

	if (image != NULL)
       	player[0].image = image;


//JOGADOR 2:
    player[1].x = 970;
    player[1].y = HEIGHT/2;
    player[1].vidas = 300;
    player[1].mov = 10;
    player[1].boundx = 15;
    player[1].boundy = 30;

    player[1].maxFrame = 8;
	player[1].curFrame = 0;
	player[1].frameCount = 0;
	player[1].frameDelay = 50;
	player[1].frameWidth = 50;
	player[1].frameHeight = 50;
	player[1].animationColumns = 8;
	player[1].animationDirection = 1;

	player[1].animationRow = 3;

    if (image != NULL)
	player[1].image = image;
}

/************************************
*  RESET ANIMAÇÕES JOGADORES 1 E 2  *
************************************/

void ResetAnimJogador(Jogador player[], int posicao)
{
	if(posicao == 1)
		player[0].animationRow = 0;

    if(posicao == 2)
        player[1].animationRow = 3;

}

/********************************
*  DESENHAR OS JOGADORES 1 E 2  *
********************************/

void DesenhaJogador(Jogador player[]) {

    //=========== JOGADOR 1 =============

    int fx1 = (player[0].curFrame % player[0].animationColumns) * player[0].frameWidth;
    int fy1 = player[0].animationRow * player[0].frameHeight;

	al_draw_bitmap_region(player[0].image, fx1, fy1, player[0].frameWidth,
		player[0].frameHeight, player[0].x - player[0].frameWidth / 2, player[0].y - player[0].frameHeight / 2, 0);

    player[0].frameCount++;



    //=========== JOGADOR 2 =============

    int fx2 =(player[1].curFrame % player[1].animationColumns) * player[1].frameWidth;
    int fy2 = player[1].animationRow * player[1].frameHeight;

	al_draw_bitmap_region(player[1].image, fx2, fy2, player[1].frameWidth,
		player[1].frameHeight, player[1].x - player[1].frameWidth / 2, player[1].y - player[1].frameHeight / 2, 0);

    player[1].frameCount++;


}

//================ JOGADOR 2 =================

/***************************
*  JOGADOR 1 - MOVIMENTOS  *
***************************/

void MoverJogador1Cima (Jogador player[]) {

    player[0].animationRow = 1; //LINHA DA SPRITE EM QUE SE REFERE

    if (player[0].frameCount >= player[0].frameDelay)
    {
        if (player[0].curFrame++ >= player[0].maxFrame)
        {
            player[0].curFrame = 0;
        }
    }

    player[0].y -= player[0].mov;


    if (player[0].y < 0) {
        player[0].y = 0;
    }

}

void MoverJogador1Baixo (Jogador player[]) {

    player[0].animationRow = 1;

    if (player[0].frameCount >= player[0].frameDelay)
    {
        if (++player[0].curFrame >= player[0].maxFrame)
        {
            player[0].curFrame = 0;
        }
    }

    player[0].y += player[0].mov;

    if (player[0].y > HEIGHT) {
        player[0].y = HEIGHT;

    }

}

void MoverJogador1Esquerda (Jogador player[]) {

    player[0].animationRow = 1;

    if (player[0].frameCount >= player[0].frameDelay)
    {
        if (++player[0].curFrame >= player[0].maxFrame)
        {
            player[0].curFrame = 0;
        }
    }
    player[0].x -= player[0].mov;

    if (player[0].x < 0) {
        player[0].x = 0;
    }


}

void MoverJogador1Direita (Jogador player[]) {

    player[0].animationRow = 1;

    if (player[0].frameCount >= player[0].frameDelay)
    {
        if (++player[0].curFrame >= player[0].maxFrame)
        {
            player[0].curFrame = 0;
        }
    }


    player[0].x += player[0].mov;

    if (player[0].x > (WIDTH/2 - 70)) {
        player[0].x = (WIDTH/2 - 70);
    }

}

//================ JOGADOR 2 =================

/***************************
*  JOGADOR 2 - MOVIMENTOS  *
***************************/

void MoverJogador2Cima (Jogador player[]) {

    player[1].animationRow = 5;

    if (player[1].frameCount >= player[1].frameDelay)
    {
        if (++player[1].curFrame >= player[1].maxFrame)
        {
            player[1].curFrame = 0;
        }
    }

    player[1].y -= player[1].mov;

    if (player[1].y < 0) {
        player[1].y = 0;
    }

}

void MoverJogador2Baixo (Jogador player[]) {

    player[1].animationRow = 5;

    if (player[1].frameCount >= player[1].frameDelay)
    {
        if (++player[1].curFrame >= player[1].maxFrame)
        {
            player[1].curFrame = 0;
        }
    }

    player[1].y += player[1].mov;

    if (player[1].y > HEIGHT) {
        player[1].y = HEIGHT;

    }

}

void MoverJogador2Esquerda (Jogador player[]) {

    player[1].animationRow = 5;

    if (player[1].frameCount >= player[1].frameDelay)
    {
        if (++player[1].curFrame >= player[1].maxFrame)
        {
            player[1].curFrame = 0;
        }
    }

    player[1].x -= player[1].mov;

    if (player[1].x < (WIDTH/2 + 55)) {
        player[1].x = (WIDTH/2 + 55);
    }

}

void MoverJogador2Direita (Jogador player[]) {

    player[1].animationRow = 5;

    if (player[1].frameCount >= player[1].frameDelay)
    {
        if (++player[1].curFrame >= player[1].maxFrame)
        {
            player[1].curFrame = 0;
        }
    }

    player[1].x += player[1].mov;

    if (player[1].x > WIDTH) {
        player[1].x = WIDTH;
    }

}

/**********************
*  COLISÃO JOGADORES  *
**********************/

void ColisaoJogadores (Jogador player[]) {


        //BOUNDING BOX DOS JOGADORES:

    if (player[0].x - player[0].boundx < player[1].x + player[1].boundx &&
            player[0].x + player[0].boundx > player[1].x - player[1].boundx &&
            player[0].y - player[0].boundy < player[1].y + player[1].boundy &&
            player[0].y + player[0].boundy > player[1].y - player[1].boundy ||
            player[1].x - player[1].boundx < player[0].x + player[0].boundx &&
            player[1].x + player[1].boundx > player[0].x - player[0].boundx &&
            player[1].y - player[1].boundy < player[0].y + player[0].boundy &&
            player[1].y + player[1].boundy > player[0].y - player[0].boundy)
    {

        if (player[1].x < player[0].x) //COLISÃO DA ESQUERDA
        {
            player[0].x = player[0].x +15;
            player[1].x = player[1].x -15;
        }
        if (player[1].x > player[0].x ) //COLISÃO DIREITA
        {
            player[0].x = player[0].x -15;
            player[1].x = player[1].x +15;
        }

        if (player[1].y < player[0].y) //COLISÃO BAIXO
        {
            player[0].y = player[0].y + 15;
            player[1].y = player[1].y - 15;
        }

        if (player[1].y > player[0].y) //COLISÃO CIMA
        {
            player[0].y = player[0].y - 15;
            player[1].y = player[1].y + 15;
        }
    }

 }

//=============== ATAQUES ====================

//=============== JOGADOR1 ===================

/******************************
*  ATAQUE J1 - INICIALIZAÇÃO  *
*******************************/

void InicAtaqueBaseJ1 (AtaqueBase1 golpe1[], int size) {

    for(int i = 0; i < size; i++)
    {
        golpe1[i].mov = 10;
        golpe1[i].estado = false;
    }

}

/************************
*  ATAQUE J1 - DESENHO  *
*************************/

void DesenhaAtaqueBaseJ1 (AtaqueBase1 golpe1[], int size) {

    for(int i = 0; i < size; i++)
    {
        if(golpe1[i].estado)
        {
            al_draw_filled_circle(golpe1[i].x, golpe1[i].y, 5, al_map_rgb(255, 255, 255));
        }
    }
}

/*************************
*  ATAQUE J1 - ATIVAÇÃO  *
**************************/

void AtaqueBaseJ1 (Jogador player[], AtaqueBase1 golpe1[], int size) {



        for(int i = 0; i < size; i++)
        {
            if(!golpe1[i].estado)
            {
                golpe1[i].x = player[0].x + 50;
                golpe1[i].y = player[0].y;
                golpe1[i].estado = true;
                al_play_sample(tiro, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
                break;
            }
        }
    }

/****************************
*  ATAQUE J1 - ATUALIZAÇÃO  *
*****************************/

void UpdateAtaqueBaseJ1 (Jogador player[], AtaqueBase1 golpe1[], int size) {

        for(int i = 0; i < size; i++)
        {
           if(golpe1[i].estado)
            {
                golpe1[i].x += golpe1[i].mov;
                if(golpe1[i].x > WIDTH)
                    golpe1[i].estado = false;
            }
        }
    }

/************************
*  ATAQUE J1 - COLISÃO  *
*************************/

void ColisaoAtaquebaseJ1(AtaqueBase1 golpe1[], int gsize, Jogador player[])
{
    for(int i = 0; i < gsize; i++)
    {
        if(golpe1[i].estado)
        {

                if(golpe1[i].x > (player[1].x - player[1].boundx) &&
                        golpe1[i].x < (player[1].x + player[1].boundx) &&
                        golpe1[i].y > (player[1].y - player[1].boundy) &&
                        golpe1[i].y < (player[1].y + player[1].boundy))
                {
                    golpe1[i].estado = false;
                    player[1].vidas -= 20;





            }
        }
    }
}

//=============== JOGADOR2 ===================

/******************************
*  ATAQUE J2 - INICIALIZAÇÃO  *
*******************************/

void InicAtaqueBaseJ2 (AtaqueBase2 golpe2[], int size) {

    for(int i = 0; i < size; i++)
    {
        golpe2[i].mov = 10;
        golpe2[i].estado = false;
    }
}

/************************
*  ATAQUE J2 - DESENHO  *
*************************/

void DesenhaAtaqueBaseJ2 (AtaqueBase2 golpe2[], int size) {

    for(int i = 0; i < size; i++)
    {
        if(golpe2[i].estado)
        {
            al_draw_filled_circle(golpe2[i].x, golpe2[i].y, 5, al_map_rgb(255, 255, 255));
        }
    }
}

/*************************
*  ATAQUE J2 - ATIVAÇÃO  *
**************************/

void AtaqueBaseJ2 (Jogador player[], AtaqueBase2 golpe2[], int size) {


        for(int i = 0; i < size; i++)
        {
            if(!golpe2[i].estado)
            {
                golpe2[i].x = player[1].x - 50;
                golpe2[i].y = player[1].y;
                golpe2[i].estado = true;
                al_play_sample(tiro, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
                break;
            }
        }
    }

/****************************
*  ATAQUE J2 - ATUALIZAÇÃO  *
*****************************/
void UpdateAtaqueBaseJ2 (Jogador player[], AtaqueBase2 golpe2[], int size) {


        for(int i = 0; i < size; i++)
        {
            if(golpe2[i].estado)
            {
                golpe2[i].x -= golpe2[i].mov;
                if(golpe2[i].x < 0)
                    golpe2[i].estado = false;

            }
        }
    }

/************************
*  ATAQUE J2 - COLISÃO  *
*************************/
void ColisaoAtaquebaseJ2(AtaqueBase2 golpe2[], int gsize, Jogador player[])
{
    for(int i = 0; i < gsize; i++)
    {
        if(golpe2[i].estado)
        {

                if(golpe2[i].x > (player[0].x - player[0].boundx) &&
                        golpe2[i].x < (player[0].x + player[0].boundx) &&
                        golpe2[i].y > (player[0].y - player[0].boundy) &&
                        golpe2[i].y < (player[0].y + player[0].boundy))
                {
                    golpe2[i].estado = false;
                    player[0].vidas -= 20;





            }
        }
    }
}

/************************************
*  PEDRAS ROLANTES - INICIALIZAÇÃO  *
************************************/

void ComecBuracos(Hole buraco[], int size, ALLEGRO_BITMAP *image)
{
	for(int i = 0; i < size; i++)
	{
		buraco[i].estado = false;
		buraco[i].mov = 5;
		buraco[i].boundx = 105;
		buraco[i].boundy = 65;

		buraco[i].maxFrame = 64;
		buraco[i].curFrame = 0;
		buraco[i].frameCount = 0;
		buraco[i].frameDelay = 2;
		buraco[i].frameWidth = 96;
		buraco[i].frameHeight = 96;
		buraco[i].animationColumns = 8;

		if(rand() % 2)
			buraco[i].animationDirection = 1;
		else
			buraco[i].animationDirection = -1;

        if (image != NULL)
            buraco[i].image = image;
	}
}

/******************************
*  PEDRAS ROLANTES - DESENHO  *
******************************/

void DesenhaBuracos (Hole buraco[], int size) {

    for (int i = 0; i < size; i++)
    {
        if (buraco[i].estado)
        {
            int fx = (buraco[i].curFrame % buraco[i].animationColumns) * buraco[i].frameWidth;
			int fy = (buraco[i].curFrame / buraco[i].animationColumns) * buraco[i].frameHeight;

			al_draw_bitmap_region(buraco[i].image, fx, fy, buraco[i].frameWidth,
				buraco[i].frameHeight, buraco[i].x - buraco[i].frameWidth / 2, buraco[i].y - buraco[i].frameHeight / 2, 0);        }
    }

}

/*******************************
*  PEDRAS ROLANTES - ATIVAÇÃO  *
*******************************/

void InicBuracos ( Hole buraco[], int size, Jogador player[]) {

     for(int i = 0; i < size; i++)
    {
        if(!buraco[i].estado)
        {
            if(rand() % 300 == 0)
            {

                buraco[i].estado = true;
                buraco[i].x = 30 + rand() % (WIDTH - 60);
                buraco[i].y = 0;
                break;
            }
        }
    }

}

/**********************************
*  PEDRAS ROLANTES - ATUALIZAÇÃO  *
**********************************/

void UpdateBuracos (Hole buraco[], int size) {

    for(int i = 0; i < size; i++)
    {
        if (buraco[i].estado)
        {
            if(++buraco[i].frameCount >= buraco[i].frameDelay)
			{
				buraco[i].curFrame += buraco[i].animationDirection;
				if(buraco[i].curFrame >= buraco[i].maxFrame)
					buraco[i].curFrame = 0;
				else if( buraco[i].curFrame <= 0)
					buraco[i].curFrame = buraco[i].maxFrame - 1;

				buraco[i].frameCount = 0;
			}
        }

        buraco[i].mov = 5;
        buraco[i].y += buraco[i].mov;
    }

}

/******************************
*  OBJETOS/COMETAS - COLISÃO  *
*******************************/

void ColisaoBuracos (Hole buraco[], int size, Jogador player[])
{

    for(int i = 0; i < size; i++)
    {
        buraco[i].boundx = 30;
        buraco[i].boundy = 30;

        if(buraco[i].estado)
        {
            if(buraco[i].x - buraco[i].boundx < player[0].x + player[0].boundx &&
                    buraco[i].x + buraco[i].boundx > player[0].x - player[0].boundx &&
                    buraco[i].y - buraco[i].boundy < player[0].y + player[0].boundy &&
                    buraco[i].y + buraco[i].boundy > player[0].y - player[0].boundy)
            {
                player[0].vidas -= 40; //RETIRA 40 DE VIDA DO JOGADOR 1
                buraco[i].estado = false;
            }
            if(buraco[i].y > HEIGHT || buraco[i].x > WIDTH)
            {
                buraco[i].estado = false;
            }
            if(buraco[i].x - buraco[i].boundx < player[1].x + player[1].boundx &&
                    buraco[i].x + buraco[i].boundx > player[1].x - player[1].boundx &&
                    buraco[i].y - buraco[i].boundy < player[1].y + player[1].boundy &&
                    buraco[i].y + buraco[i].boundy > player[1].y - player[1].boundy)
            {
                player[1].vidas -= 40; //RETIRA 40 DE VIDA DO JOGADOR 2
                buraco[i].estado = false;
            }

        }
    }
}

/******************************
*  BARREIRAS - INICIALIZAÇÃO  *
******************************/

void InicBarreira(Barreiras obstaculo[], int size, ALLEGRO_BITMAP *image) {

    obstaculo[0].x = 120;
    obstaculo[1].x = 150;
    obstaculo[2].x = WIDTH/2 - 115;
    obstaculo[3].x = WIDTH/2 - 220;
    obstaculo[4].x = WIDTH/2 - 100;
    obstaculo[5].x = WIDTH - 150;
    obstaculo[6].x = WIDTH - 150;
    obstaculo[7].x = WIDTH/2 + 100;
    obstaculo[8].x = WIDTH/2 + 200;
    obstaculo[9].x = WIDTH/2 + 100;
    obstaculo[0].y = HEIGHT/2 - 100;
    obstaculo[1].y = HEIGHT/2 + 100;
    obstaculo[2].y = 200;
    obstaculo[3].y = HEIGHT/2;
    obstaculo[4].y = HEIGHT -100;
    obstaculo[5].y = HEIGHT/2 - 100;
    obstaculo[6].y = HEIGHT/2 + 100;
    obstaculo[7].y = 100;
    obstaculo[8].y = HEIGHT/2;
    obstaculo[9].y = HEIGHT -100;

    for (int i = 0; i < size; i++)
    {
        obstaculo[i].boundx = 130;
        obstaculo[i].boundy = 150;
        obstaculo[i].maxFrame = 1;
		obstaculo[i].curFrame = 0;
		obstaculo[i].frameCount = 0;
		obstaculo[i].frameDelay = 2;
		obstaculo[i].frameWidth = 70;
		obstaculo[i].frameHeight = 89;
		obstaculo[i].animationRows = 0;
		obstaculo[i].animationColumns = 1;

		if (image != NULL)
            obstaculo[i].image = image;

    }
}

/************************
*  BARREIRAS - DESENHO  *
************************/

void DesenhaBarreira(Barreiras obstaculo[], int size) {

    for (int i = 0; i < size; i++)
    {
        int fx1 = (obstaculo[i].curFrame % obstaculo[i].animationColumns) * obstaculo[i].frameWidth;
        int fy1 = obstaculo[i].animationRows * obstaculo[i].frameHeight;

        al_draw_bitmap_region(obstaculo[i].image, fx1, fy1, obstaculo[i].frameWidth,
            obstaculo[i].frameHeight, obstaculo[i].x - obstaculo[i].frameWidth / 2, obstaculo[i].y - obstaculo[i].frameHeight / 2, 0);
    }
}

/************************
*  BARREIRAS - COLISÃO  *
************************/

void ColisaoBarreiraJogador (Barreiras obstaculo[], Jogador player[], int size) {

     for(int i = 0; i < size; i++)
    {
        obstaculo[i].boundx = 40;
        obstaculo[i].boundy = 40;
            if(obstaculo[i].x - obstaculo[i].boundx < player[0].x + player[0].boundx &&
                    obstaculo[i].x + obstaculo[i].boundx > player[0].x - player[0].boundx &&
                    obstaculo[i].y - obstaculo[i].boundy < player[0].y + player[0].boundy &&
                    obstaculo[i].y + obstaculo[i].boundy > player[0].y - player[0].boundy)
            {

                if (obstaculo[i].x < player[0].x) //COLISÃO DA ESQUERDA
                    {
                        player[0].x = player[0].x +10;

                    }
                if (obstaculo[i].x > player[0].x ) //COLISÃO DIREITA
                    {
                    player[0].x = player[0].x -10;

                    }

                if (obstaculo[i].y < player[0].y) //COLISÃO BAIXO
                    {
                    player[0].y = player[0].y + 10;

                    }

                if (obstaculo[i].y > player[0].y) //COLISÃO CIMA
                    {
                    player[0].y = player[0].y - 10;

                    }
            }

             if(obstaculo[i].x - obstaculo[i].boundx < player[1].x + player[1].boundx &&
                    obstaculo[i].x + obstaculo[i].boundx > player[1].x - player[1].boundx &&
                    obstaculo[i].y - obstaculo[i].boundy < player[1].y + player[1].boundy &&
                    obstaculo[i].y + obstaculo[i].boundy > player[1].y - player[1].boundy)
            {

                if (obstaculo[i].x < player[1].x) //COLISÃO DA ESQUERDA
                    {
                        player[1].x = player[1].x +10;

                    }
                if (obstaculo[i].x > player[1].x ) //COLISÃO DIREITA
                    {
                    player[1].x = player[1].x -10;

                    }

                if (obstaculo[i].y < player[1].y) //COLISÃO BAIXO
                    {
                    player[1].y = player[1].y + 10;

                    }

                if (obstaculo[i].y > player[0].y) //COLISÃO CIMA
                    {
                    player[1].y = player[1].y - 10;

                    }
            }
    }
}

void ColisaoBarreiraAtaqueBaseJ1 (Barreiras obstaculo[], AtaqueBase1 golpe1[], int osize, int gsize) {

    for (int i = 0; i < osize; i++)
    {
        obstaculo[i].boundy = 40;
        for (int j = 0; j < gsize; j++)
        {
            golpe1[j].boundx = 15;
            golpe1[j].boundy = 15;
            if (golpe1[j].estado)
        {
            if(obstaculo[i].x > (golpe1[j].x - golpe1[j].boundx) &&
                            obstaculo[i].x < (golpe1[j].x + golpe1[j].boundx) &&
                            obstaculo[i].y + obstaculo[i].boundy > (golpe1[j].y - golpe1[j].boundy) &&
                            obstaculo[i].y - obstaculo[i].boundy  < (golpe1[j].y + golpe1[j].boundy))
                            {
                                golpe1[j].estado = false;
                }
        }
    }

}
}

void ColisaoBarreiraAtaqueBaseJ2 (Barreiras obstaculo[], AtaqueBase2 golpe2[], int osize, int gsize) {

    for (int i = 0; i < osize; i++)
    {
        obstaculo[i].boundy = 40;

        for (int j = 0; j < gsize; j++)
        {
            golpe2[j].boundx = 15;
            golpe2[j].boundy = 15;
            if (golpe2[j].estado)
                {
                    if(obstaculo[i].x > (golpe2[j].x - golpe2[j].boundx) &&
                       obstaculo[i].x < (golpe2[j].x + golpe2[j].boundx) &&
                       obstaculo[i].y + obstaculo[i].boundy > (golpe2[j].y - golpe2[j].boundy) &&
                       obstaculo[i].y - obstaculo[i].boundy < (golpe2[j].y + golpe2[j].boundy))
                            {
                                golpe2[j].estado = false;
                            }
                }
        }
    }
}

void ColisaoBarreiraPedra (Barreiras obstaculo[], Hole buraco[], int osize, int bsize) {

    for (int i = 0; i < osize; i++)
    {
        obstaculo[i].boundx = 40;
        obstaculo[i].boundy = 40;
        for (int j = 0; j < bsize; j++)
        {
            if (buraco[j].estado)
            {
                if(obstaculo[i].x + obstaculo[i].boundx  > (buraco[j].x - buraco[j].boundx) &&
                    obstaculo[i].x - obstaculo[i].boundx < (buraco[j].x + buraco[j].boundx) &&
                    obstaculo[i].y + obstaculo[i].boundy > (buraco[j].y - buraco[j].boundy) &&
                    obstaculo[i].y - obstaculo[i].boundx < (buraco[j].y + buraco[j].boundy))
                {
                    buraco[j].estado = false;
                }
            }
        }
    }
}

void ColisaoAtaqueBaseJ1Pedra (AtaqueBase1 golpe1[], Hole buraco[], int asize, int bsize) {

    for(int i = 0; i < asize; i++)
    {
        if(golpe1[i].estado)
        {
            for(int j =0; j < bsize; j++)
            {
                if(buraco[j].estado)
                {
                    if(golpe1[i].x > (buraco[j].x - buraco[j].boundx) &&
                            golpe1[i].x < (buraco[j].x + buraco[j].boundx) &&
                            golpe1[i].y > (buraco[j].y - buraco[j].boundy) &&
                            golpe1[i].y < (buraco[j].y + buraco[j].boundy))
                    {
                        golpe1[i].estado = false;
                    }
                }
            }
        }
    }

}

void ColisaoAtaqueBaseJ2Pedra (AtaqueBase2 golpe2[], Hole buraco[], int asize, int bsize) {

    for(int i = 0; i < asize; i++)
    {
        if(golpe2[i].estado)
        {
            for(int j =0; j < bsize; j++)
            {
                if(buraco[j].estado)
                {
                    if(golpe2[i].x > (buraco[j].x - buraco[j].boundx) &&
                            golpe2[i].x < (buraco[j].x + buraco[j].boundx) &&
                            golpe2[i].y > (buraco[j].y - buraco[j].boundy) &&
                            golpe2[i].y < (buraco[j].y + buraco[j].boundy))
                    {
                        golpe2[i].estado = false;
                    }
                }
            }
        }
    }
}


/****************************
*   FUNDO - INICIALIZAÇÃO   *
****************************/

void InicFundo(Background &back, float x, float y, float velx, float vely, int width, int height, int dirX, int dirY, ALLEGRO_BITMAP *image)
{
	back.x = x;
	back.y = y;
	back.velX = velx;
	back.velY = vely;
	back.width = width;
	back.height = height;
	back.dirX = dirX;
	back.dirY = dirY;
	back.image = image;
}


/**********************
*   FUNDO - DESENHO   *
**********************/

void DesenhaFundo(Background &back)
{
	al_draw_bitmap(back.image, back.x, back.y, 0);

	if(back.x + back.width < WIDTH)
		al_draw_bitmap(back.image, back.x + back.width, back.y, 0);
}
