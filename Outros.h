struct Hole
{
    int x;
    int y;
    int estado;
    int boundx;
    int boundy;
    int mov;

    int maxFrame;
	int curFrame;
	int frameCount;
	int frameDelay;
	int frameWidth;
	int frameHeight;
	int animationColumns;
	int animationDirection;

	ALLEGRO_BITMAP *image;

};


struct Barreiras
{
    int x;
    int y;
    int estado;
    int boundx;
    int boundy;

    int maxFrame;
	int curFrame;
	int frameCount;
	int frameDelay;
	int frameWidth;
	int frameHeight;
	int animationRows;
	int animationColumns;
	int animationDirection;

	ALLEGRO_BITMAP *image;

};

struct Background
{
	float x;
	float y;
	float velX;
	float velY;
	int dirX;
	int dirY;

	int width;
	int height;

	ALLEGRO_BITMAP *image;
};

