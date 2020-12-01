/*************************************
* ARQUIVO COM AS ESTRUTURAS DO JOGO  *
*************************************/


struct Jogador
{
    int x; //POSI플O X
    int y; //POSI플O Y
    int vidas; //VIDA DO JOGADOR
    int mov; //MOVIMENTOS
    int boundx; //POSI플O X DO BOUNDING BOX
    int boundy; // POSI플O Y DO BOUNDING BOX


    int maxFrame; //NUMERO M햆IMO DE FRAMES
	int curFrame; //FRAME ATUAL
	int frameCount; //CONTAGEM DE FRAME
	int frameDelay; //ATRASO DE FRAME
	int frameWidth; //LARGURA DO FRAME
	int frameHeight; //ALTURA DO FRAME
	int animationColumns; //COLUNAS
	int animationDirection; // DIRE플O

	int animationRow; // FILEIRA

	ALLEGRO_BITMAP *image; //PONTEIRO DE IMAGEM
};

struct AtaqueBase1
{
    int x;
    int y;
    int estado;
    int mov;
    int boundx;
    int boundy;
};

struct AtaqueBase2
{
    int x;
    int y;
    int estado;
    int mov;
    int boundx;
    int boundy;
};

