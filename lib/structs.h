#include "../lib.h"



typedef struct image{

	float x;
	float y;

	float velX;
	float velY;

	int dirX;
	int dirY;

	int largura;
	int altura;

	ALLEGRO_BITMAP *imagem;	

}BACKGROUND;


typedef struct fire{

	ALLEGRO_BITMAP *imagem;	
	
}ANIMATION_FIRE;


typedef struct item{

	ALLEGRO_BITMAP *item;	
	int id;
	int x;
	int y;
	bool lido;
	bool pego;
	char descricao[3000];
	
}ITEM;
