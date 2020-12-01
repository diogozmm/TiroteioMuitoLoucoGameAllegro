#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

/******************************
*  FUN합ES JOGADORES - GERAL  *
******************************/

void InicJogador (Jogador player[], ALLEGRO_BITMAP *image = NULL);
void DesenhaJogador (Jogador player[]);
void ColisaoJogadores (Jogador player[]);


/**********************************
*  FUN합ES JOGADOR1 - MOVIMENTOS  *
**********************************/

void MoverJogador1Cima (Jogador player[]);
void MoverJogador1Baixo (Jogador player[]);
void MoverJogador1Esquerda (Jogador player[]);
void MoverJogador1Direita (Jogador player[]);
void ResetAnimJogador(Jogador player[], int posicao);


/*******************************
*  FUN합ES JOGADOR1 - ATAQUES  *
*******************************/

void InicAtaqueBaseJ1 (AtaqueBase1 golpe1[], int size);
void DesenhaAtaqueBaseJ1 (AtaqueBase1 golpe1[], int size);
void AtaqueBaseJ1 (Jogador player[], AtaqueBase1 golpe1[], int size);
void UpdateAtaqueBaseJ1 (Jogador player[], AtaqueBase1 golpe1[], int size);
void ColisaoAtaquebaseJ1(AtaqueBase1 golpe1[], int gsize, Jogador player[]);


/**********************************
*  FUN합ES JOGADOR2 - MOVIMENTOS  *
**********************************/

void MoverJogador2Cima (Jogador player[]);
void MoverJogador2Baixo (Jogador player[]);
void MoverJogador2Esquerda (Jogador player[]);
void MoverJogador2Direita (Jogador player[]);


/*******************************
*  FUN합ES JOGADOR2 - ATAQUES  *
*******************************/

void InicAtaqueBaseJ2 (AtaqueBase2 golpe2[], int size);
void DesenhaAtaqueBaseJ2 (AtaqueBase2 golpe2[], int size);
void AtaqueBaseJ2 (Jogador player[], AtaqueBase2 golpe2[], int size);
void UpdateAtaqueBaseJ2 (Jogador player[], AtaqueBase2 golpe2[], int size);
void ColisaoAtaquebaseJ2(AtaqueBase2 golpe2[], int gsize, Jogador player[]);


/*******************************
*  FUN합ES OBJETOS/OBST핧ULOS  *
*******************************/

void ComecBuracos(Hole buraco[], int size, ALLEGRO_BITMAP *image = NULL);
void InicBuracos (Hole buraco[], int size, Jogador player[]);
void DesenhaBuracos (Hole buraco[], int size);
void UpdateBuracos (Hole buraco[], int size);
void ColisaoBuracos (Hole buraco[], int size, Jogador player[]);


/**********************
*  FUN합ES BARREIRAS  *
**********************/

void InicBarreira(Barreiras obstaculo[], int size, ALLEGRO_BITMAP *image = NULL);
void DesenhaBarreira(Barreiras obstaculo[], int size);
void ColisaoBarreiraJogador (Barreiras obstaculo[], Jogador player[], int size);
void ColisaoBarreiraAtaqueBaseJ1 (Barreiras obstaculo[], AtaqueBase1 golpe1[], int osize, int gsize);
void ColisaoBarreiraAtaqueBaseJ2 (Barreiras obstaculo[], AtaqueBase2 golpe2[], int osize, int gsize);
void ColisaoBarreiraPedra (Barreiras obstaculo[], Hole buraco[], int osize, int bsize);
void ColisaoAtaqueBaseJ1Pedra (AtaqueBase1 golpe1[], Hole buraco[], int asize, int bsize);
void ColisaoAtaqueBaseJ2Pedra (AtaqueBase2 golpe2[], Hole buraco[], int asize, int bsize);

/*****************************
*  FUN플O DOS MENUS/ESTADOS  *
*****************************/

void MudarEstado(int &state, int novoState);

void InicFundo(Background &back, float x, float y, float velx, float vely, int width, int height, int dirX, int dirY, ALLEGRO_BITMAP *image);
void DesenhaFundo(Background &back);


#endif
