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

typedef struct tVilao{

	ALLEGRO_BITMAP *imagem;	
	bool vivo;	
	int x;
	int y;
	int lazy;
	int qtd;
	int mode;
	bool go;	
	int check;
	
	
}ENIME;

typedef struct itens{

	ALLEGRO_BITMAP *item;	
	ALLEGRO_BITMAP *explicacao;
	int id;
	int x;
	int y;
	bool lido;
	bool pego;
	char resposta[3000];
	
}ITEM;
