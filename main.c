#include "lib.h"

bool keys[] = {false, false, false, false};
enum KEYS{UP, DOWN, LEFT, RIGHT};
enum tiles{ROCK, GRASS, FLOOR, LAVA, WATER, DOOR1F, DOOR1A, BOOK,  DOOR2F, DOOR2A};


int yE=1, xE=1;

float cameraPosicao[2] = {0,0} ;

bool ganhou = false;

ALLEGRO_TRANSFORM camera;

ALLEGRO_FONT *textFont 			= NULL; 
ALLEGRO_DISPLAY *janela 		= NULL;
ALLEGRO_TIMER *tempo 			= NULL;
ALLEGRO_EVENT_QUEUE *eventos 	= NULL;
ALLEGRO_EVENT_QUEUE *eventoRelogio 	= NULL;

ALLEGRO_SAMPLE *level1Sound 	= NULL;
ALLEGRO_SAMPLE *musica 			= NULL;
ALLEGRO_SAMPLE *devilLaugh 		= NULL;
ALLEGRO_SAMPLE *openDoor 		= NULL;
ALLEGRO_SAMPLE *introBadbaby	= NULL;
ALLEGRO_SAMPLE *grassWalk 		= NULL;
ALLEGRO_SAMPLE *floorWalk 		= NULL;
ALLEGRO_SAMPLE *waterWalk 		= NULL;
ALLEGRO_SAMPLE *bookSound 		= NULL;
ALLEGRO_SAMPLE *gameOverSound 	= NULL;
ALLEGRO_SAMPLE *creditSound 	= NULL;

ALLEGRO_BITMAP *backGround 		= NULL;
ALLEGRO_BITMAP *personagem 		= NULL;
ALLEGRO_BITMAP *enime	 		= NULL;
ALLEGRO_BITMAP *fundoCredits 	= NULL;
ALLEGRO_BITMAP *book 			= NULL;
ALLEGRO_BITMAP *grama 			= NULL;
ALLEGRO_BITMAP *rocha 			= NULL;
ALLEGRO_BITMAP *porta1f 		= NULL;
ALLEGRO_BITMAP *porta1a 		= NULL;

ALLEGRO_BITMAP *text 			= NULL;
ALLEGRO_BITMAP *fundoGameOver 	= NULL;
ALLEGRO_BITMAP *blocoEsq 		= NULL;
ALLEGRO_BITMAP *blocoDir 		= NULL;

ALLEGRO_BITMAP *logo 			= NULL;

ALLEGRO_BITMAP *sombra1 		= NULL;
ALLEGRO_BITMAP *sombra2 		= NULL;
ALLEGRO_BITMAP *sombra3 		= NULL;


ALLEGRO_BITMAP *fundo1 			= NULL;
ALLEGRO_BITMAP *fires1 			= NULL;
ALLEGRO_BITMAP *fires2 			= NULL;
ALLEGRO_BITMAP *btSair 			= NULL;
ALLEGRO_BITMAP *btJogar 		= NULL;
ALLEGRO_BITMAP *fogo 			= NULL;
ANIMATION_FIRE fogos[4];

ALLEGRO_BITMAP *ned1 = NULL;
ALLEGRO_BITMAP *ned2 = NULL;
ALLEGRO_BITMAP *luz = NULL;
ALLEGRO_BITMAP *sombra = NULL;
ALLEGRO_BITMAP *fundoMenu = NULL;


bool playGame();
void gameOver();
void intro();
void credits();
void criarChama();
void animarChama(); 
bool inicializar();
bool menuPrincipal();
void desenharBackgroundRightLeft(BACKGROUND *fundo);
void desenharBackgroundDownUp(BACKGROUND *fundo);
void fadein(ALLEGRO_BITMAP *imagem, int velocidade);
void fadeinWindow(int velocidade, bool fadeIn);
void atualizarBackgroundRightLeft(BACKGROUND *fundo);
void atualizarBackgroundDownUp(BACKGROUND *fundo);
void inicializarBackground(BACKGROUND *fundo, float x, float y, float velX, float velY, int largura, int altura, int dirX, int dirY, ALLEGRO_BITMAP *imagem );
void abrirPorta(int mapa[DIM_COLUNA][DIM_LINHA], int *px, int *py, ALLEGRO_EVENT evento);
void copiarMapa(int copiado[][DIM_LINHA], int original[][DIM_LINHA]);
void initItem(ITEM *livro, int x, int y, char desc[]);
void getItem(int xOFF, int yOFF, ITEM *livro);
void printStage(int mapa[DIM_COLUNA][DIM_LINHA]);
void allegroInput(ALLEGRO_EVENT evento);
void exibir_texto_centralizado();
int telaPorta();
int relogio();
//void playEnime(int mapa[DIM_COLUNA][DIM_LINHA]);
/*
void playEnime(int mapa[DIM_COLUNA][DIM_LINHA]){
	
	bool lock = false;
	
	if(mapa[xE][yE+1] == 1){
		yE++;			
	
	}else if(mapa[xE][yE-1] == 1){
		yE--;		
		
	}
	
	if(mapa[xE+1][yE] == 1){
		xE++;
		
	}else if(mapa[xE-1][yE] == 1){
		xE--;
		
	}
	
	if(mapa[xE][yE]){
		
	}
	
	printf("POS ENIME: [%d][%d]\n", yE,xE);
	al_draw_bitmap(enime, yE*TILE_TAM, xE*TILE_TAM, 0);
}

void playEnime(int mapa[DIM_COLUNA][DIM_LINHA]){
	static int x=0,y=0;
	static int oldX=0, oldY=0;
	int posX;
	int posY;
	
	srand( (unsigned)time(NULL) );
	
	posX = rand() % 10;
	posY = rand() % 10;


	if(posX >= 6){
		
		if(x < 10)
			x = x + 1;			
	}else{
		
		if(x > 0)
			x = x - 1;
	}
	
	
	if(posY >= 6){
		
		if(y < 10)
			y = y + 1;
	}else{
		
		if(y > 0)
			y = y - 1;
	}

	printf("ENIME MAPA: [%d][%d]\n", y,x);	
	if(mapa[x][y] == GRASS){				
		oldX = x; oldY = y;
		al_draw_bitmap(enime, y*TILE_TAM, x*TILE_TAM, 0);	
		//al_rest(2);
		
	}else{
		printf("===========\nERRO POS\n===========\n");
		al_draw_bitmap(enime, oldY*TILE_TAM, oldX*TILE_TAM, 0);	
	}
	
	al_rest(0.001);
}
*/

void atualizarCamera(float cameraPosicao[], float x, float y, int largura, int altura){
	
	cameraPosicao[0] = -(LARGURA_TELA/2) + (x+largura/2);
	cameraPosicao[1] = -(LARGURA_TELA/2) + (y+altura/2);
	
	if(cameraPosicao[0]<0)
		cameraPosicao[0] = 0;
		
	if(cameraPosicao[1]<0)
		cameraPosicao[1] = 0;
		
}

void registrarInputs(){
	al_register_event_source(eventos, al_get_timer_event_source(tempo));
    al_register_event_source(eventos, al_get_display_event_source(janela));
   	al_register_event_source(eventos, al_get_mouse_event_source());
	al_register_event_source(eventos, al_get_keyboard_event_source());
}

void initBitmap(){
	
	sombra1				=		al_create_bitmap(50,50);
	al_set_target_bitmap(sombra1);
	al_clear_to_color( al_map_rgb(0, 0, 0) );
	
	sombra2			=		al_create_bitmap(50,50);
	al_set_target_bitmap(sombra2);
	al_clear_to_color( al_map_rgba_f(255, 255, 255, 1) );
	
	backGround		=		al_create_bitmap(5000,5000);
	al_set_target_bitmap(backGround);
	al_clear_to_color( al_map_rgb(255, 0, 0 ) );
	
		
	
	personagem 		= 		al_load_bitmap("imagens/ned/personagem.jpg");
	enime 			= 		al_create_bitmap(50,50);
	al_set_target_bitmap(enime);
	al_clear_to_color( al_map_rgb(0, 255, 0 ) );
	
	grama  			= 		al_load_bitmap("imagens/background/grass.png");
	rocha  			= 		al_load_bitmap("imagens/background/rock.png");
	porta1f			= 		al_load_bitmap("imagens/background/porta1f.png");
	porta1a 		= 		al_load_bitmap("imagens/background/porta1a.png");


	book			= 		al_load_bitmap("imagens/background/book.png");
	fundo1 			= 		al_load_bitmap("imagens/background/head2.png");	
	fires1 			= 		al_load_bitmap("imagens/background/fires.png");
	fires2 			= 		al_load_bitmap("imagens/background/fires2.png");


	fundoMenu		=		al_load_bitmap("imagens/fundo.png");		
	sombra  		=		al_load_bitmap("imagens/background/shadow.png");
	luz  			=		al_load_bitmap("imagens/background/light.png");		
	ned1   			= 		al_load_bitmap("imagens/ned/ned_down.png");
	ned2 			= 		al_load_bitmap("imagens/ned/ned_left.png");	


	text 			= 		al_load_bitmap("imagens/credits/text.png");
	fundoGameOver	= 		al_load_bitmap("imagens/fundo.png");	
	fundoCredits	= 		al_load_bitmap("imagens/fundoCredits.jpg");	
	blocoEsq		= 		al_load_bitmap("imagens/credits/left.png");
	blocoDir		= 		al_load_bitmap("imagens/credits/right.png");	

	logo 			= 		al_load_bitmap("imagens/badbaby.png");		
}

void initSample(){

	openDoor 		= 		al_load_sample("som/openDoor.ogg");	
	devilLaugh 		=		al_load_sample("som/devilLaugh.ogg");
	grassWalk 		= 		al_load_sample("som/walkGrass.ogg");
	floorWalk 		= 		al_load_sample("som/walkGrass.ogg");
	bookSound	 	= 		al_load_sample("som/book.ogg");	
	musica 			= 		al_load_sample("som/fundo.ogg");
	gameOverSound 	= 		al_load_sample("som/gameOver.ogg");	
	introBadbaby 	= 		al_load_sample("som/laughBaby.ogg");	
	creditSound 	= 		al_load_sample("som/credits.ogg");
	level1Sound 	=		al_load_sample("som/level1.ogg");


}
 
void main(){
    if ( !inicializar() ){
        printf("impossivel iniciar allegro\n");
        return;
    }
    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    
	intro();    	
	while(true){
		if( menuPrincipal() ){
			printf("JOgar...\n");				
			
			if( playGame() ){							
				if(ganhou)				
					credits();
				
			}else{
				gameOver();
			}
			
		}else{			
			printf("Até a próxima\n");
			return;		
		}
	}
	al_destroy_display( janela );
} 


void copiarMapa(int copiado[][DIM_LINHA], int original[][DIM_LINHA]){
    int linha,
        coluna;
    
	for(coluna = 0 ; coluna < DIM_COLUNA ; coluna++){
		for(linha = 0 ; linha < DIM_LINHA ; linha++){
            copiado[linha][coluna] = original[linha][coluna];
		}
	}
}

void abrirPorta(int mapa[DIM_COLUNA][DIM_LINHA], int *px, int *py, ALLEGRO_EVENT evento){	
	
	int passe = 0, pos = 0;
	int Y = *py, X = *px;
	
				
	if( mapa[Y-1][X] == DOOR1F )//acima
		pos = -1;
	if( mapa[Y+1][X] == DOOR1F )//abaixo
		pos = 1;
	if( mapa[Y][X-1] == DOOR1F )//a esquerda
		pos = -2;
	if( mapa[Y][X+1] == DOOR1F )//a direita
		pos = 2;		
		
	if(pos != 0){
			
		//printf("Digite o passe: ");
		//scanf("%d", &passe);
		
		passe = telaPorta();
		
		if(passe == 1){
			printf("\n\nPASSE CORRETO\n\n");
			
			al_play_sample(openDoor, 1.0, 1.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);	
			al_rest(4);
			
			if(pos == 1)
				mapa[Y+1][X] = DOOR1A; 
			if(pos == -1)
				mapa[Y-1][X] = DOOR1A;
			if(pos == 2)
				mapa[Y][X+1] = DOOR1A;
			if(pos == -2)
				mapa[Y][X-1] = DOOR1A;				
				
		}
		else{
			printf("\n\nERRADO PORRA\n\n");
		}
		
		
		
	}        
}

void playSomPasso(int mapa[DIM_COLUNA][DIM_LINHA], int px, int py){	
	float volume = 0.7;
	if(mapa[py][px] == GRASS)
		al_play_sample(grassWalk, volume, 1.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
	if(mapa[py][px] == FLOOR)
		al_play_sample(floorWalk, volume, 1.0, 0.5, ALLEGRO_PLAYMODE_ONCE, NULL);
}

void getItem(int xOFF, int yOFF, ITEM *livro){
	
	if(xOFF == livro->x && yOFF == livro->y){
		printf("GOT BOOK: %s\n\n", livro->descricao);
		al_play_sample(bookSound, 1.0, 1.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);		
		livro->pego = true;	
	}
	
}

void initItem(ITEM *livro, int x, int y, char desc[]){
	livro->item = book;//imagem livro
	livro->id = 1;
	livro->x = x;
	livro->y = y;
	livro->lido = false;
	livro->pego = false;
	strcpy(livro->descricao, desc);
	
}


void printStage(int mapa[DIM_COLUNA][DIM_LINHA]){
	
	int x,y;
	for(y=0;y<DIM_COLUNA;y++){
		
		for(x=0; x<DIM_LINHA; x++){
			
			if(mapa[y][x] == GRASS)
				al_draw_bitmap(grama, x*TILE_TAM, y*TILE_TAM, 0);				
			else if(mapa[y][x] == ROCK)	
				al_draw_bitmap(rocha, x*TILE_TAM, y*TILE_TAM, 0);		
			else if(mapa[y][x] == DOOR1F)
				al_draw_bitmap(porta1f, x*TILE_TAM, y*TILE_TAM, 0);		
			else if(mapa[y][x] == DOOR1A)
				al_draw_bitmap(porta1a, x*TILE_TAM, y*TILE_TAM, 0);		
		
			
		}
		
	}
	
}

void printfShadow(){
	int x,y;
	for(y=0; y<DIM_COLUNA; y++){
		for(x=0; x<DIM_LINHA; x++){

			al_draw_tinted_bitmap(sombra1, al_map_rgba_f(1, 1, 1, 0.8), x*TILE_TAM, y*TILE_TAM, 0);			
			
		}
	}
		
}

bool playGame(){
	
	
	al_start_timer(tempo);
	al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_ARROW);
	al_set_window_title(janela, "DECRYPT - THE GAME - Playing...");
	
	extern int mapa1[DIM_COLUNA][DIM_LINHA];
	extern int mapa2[DIM_COLUNA][DIM_LINHA];
	extern int mapa3[DIM_COLUNA][DIM_LINHA];		
	extern int mapa4[DIM_COLUNA][DIM_LINHA];
		
	int mapa[DIM_COLUNA][DIM_LINHA];
	int estagios = 1;
		
	BACKGROUND BG;
	
	ITEM livro;
	
	
	bool render		= false;
	bool sair 		= false;
	bool gotBook 	= false;
	bool flagPos	= true;
	
	int min = 0;
	int seg = 0;
	int x,y;
	int px			= 0;
	int py			= 0;		
	int move 		= 0;
	int xOFF 		= 0;
	int yOFF 		= 0;			

	extern char descricoes[4][3000];	

	eventos			= al_create_event_queue();
	eventoRelogio	= al_create_event_queue();
	tempo 			= al_create_timer(0.1);
	
	ALLEGRO_BITMAP *jogador;
	
	registrarInputs();
			
	al_play_sample(level1Sound, 0.5, 1.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
		
			
	inicializarBackground(&BG, 0, 0, 1, 0, LARGURA_TELA, ALTURA_TELA, -1, 1, fundoMenu);

	jogador = ned1;
	
	while( !ganhou ){
		
		if(estagios == 1){						
			copiarMapa(mapa, mapa1);
			initItem(&livro, 29*TILE_TAM, 28*TILE_TAM, descricoes[estagios-1]);			
			sair = false;
			
		}else if(estagios == 2){
			copiarMapa(mapa, mapa2);
			initItem(&livro, 27*TILE_TAM, 5*TILE_TAM, descricoes[estagios-1]);
			sair = false;
			
		}else if(estagios == 3){						
			copiarMapa(mapa, mapa3);
			initItem(&livro, 37*TILE_TAM, 3*TILE_TAM, descricoes[estagios-1]);
			sair = false;
			
		}else if(estagios == 4){						
			copiarMapa(mapa, mapa4);
			initItem(&livro, 15*TILE_TAM, 24*TILE_TAM, descricoes[estagios-1]);
			sair = false;
			
		}else{			
			xOFF = 0; yOFF = 0;
			atualizarCamera(cameraPosicao, xOFF, yOFF, 0, 0);
			
			ganhou = true;
			sair = true;				
		}			
		
		
		while( !sair ){		
			
			al_set_target_bitmap( al_get_backbuffer( janela ) ); 
			
			al_draw_bitmap(backGround, 0, 0, 0);
			
			printStage(mapa);
			
			if(!livro.pego){
				al_draw_bitmap(livro.item, livro.x, livro.y, 0);				
				getItem(xOFF, yOFF, &livro);			
			}
			
			al_draw_bitmap(jogador, xOFF, yOFF, move);		
			

			al_draw_bitmap(blocoEsq, cameraPosicao[0]-100, cameraPosicao[1], 0);
			al_draw_bitmap(blocoDir, cameraPosicao[0]+335, cameraPosicao[1], 0);
			

			while ( !al_is_event_queue_empty(eventos) ){
				ALLEGRO_EVENT evento;        
				al_wait_for_event(eventos, &evento);
				
				atualizarCamera(cameraPosicao, xOFF, yOFF, 0, 0);
				
				al_identity_transform(&camera);
				al_translate_transform(&camera, -cameraPosicao[0], -cameraPosicao[1]);
				al_use_transform(&camera);

				if(evento.type == ALLEGRO_EVENT_TIMER){
					
						seg++;
					if (seg == 60)
					{
						min++;
						seg = 0;
					}
						
				}


				if(evento.keyboard.keycode == ALLEGRO_KEY_ENTER){
					abrirPorta(mapa, &px, &py, evento);			
				}
				
				if(mapa[py][px] == DOOR1A){
					sair = true;
					//ganhou = true;
					estagios += 1;					
					printf("PRÓXIMO NÍVEL\n\n");
				}


				
				if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
					
					sair = true;
					ganhou = true;				
						
				}else if(evento.type == ALLEGRO_EVENT_KEY_DOWN){
					
					if(evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
						sair = true;
						ganhou = true;
					}
					
					if(evento.keyboard.keycode == ALLEGRO_KEY_LEFT){
						
						if(mapa[py][px-1] != ROCK && mapa[py][px-1] != DOOR1F){
								xOFF -= TILE_TAM;
								px -= 1;
								jogador = ned2;
								move = 2;			
								playSomPasso(mapa, px, py);							
						}				
					}

					if(evento.keyboard.keycode == ALLEGRO_KEY_RIGHT){					
						if(mapa[py][px+1] != ROCK && mapa[py][px+1] != DOOR1F){					
							xOFF += TILE_TAM;
							px += 1;
							jogador = ned2;
							move = 1;						
							playSomPasso(mapa, px, py);							
						}				
					}
					
					if(evento.keyboard.keycode == ALLEGRO_KEY_DOWN){					
						if(mapa[py+1][px] != ROCK && mapa[py+1][px] != DOOR1F){					
							yOFF += TILE_TAM;
							py += 1;
							jogador = ned1;
							move = 1;			
							playSomPasso(mapa, px, py);							
						}				
					}
					
					if(evento.keyboard.keycode == ALLEGRO_KEY_UP){					
						if(mapa[py-1][px] != ROCK && mapa[py-1][px] != DOOR1F){					
							yOFF -= TILE_TAM;
							py -= 1;
							jogador = ned1;
							move = 2;
							playSomPasso(mapa, px, py);					
						}				
					}

					
				}
			
				
				if(render && al_is_event_queue_empty(eventos)){
					render = false;

					al_flip_display();
					al_clear_to_color(al_map_rgb(0,0,0));
				}
			}
					
			al_flip_display();
		}
	}
	al_destroy_sample(level1Sound);
	al_destroy_bitmap(fundoMenu);
	al_destroy_event_queue(eventos);	
	
	
	return true;
}


void credits(){
	al_set_target_bitmap( al_get_backbuffer( janela ) ); 
	BACKGROUND BG, BG2;
	textFont 	=	al_load_font("fontes/orderElfic.ttf", 50, 0);
	
	inicializarBackground(&BG, 0, 0, 1, 0, LARGURA_TELA, ALTURA_TELA, -1, 1, fundoCredits);
	
	al_play_sample(creditSound, 2.0, 1.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
	al_set_window_title(janela, "Credits");

	int c=500;
	while(c > -7000){				
		
		desenharBackgroundRightLeft(&BG);
		al_draw_text(textFont, al_map_rgb(255,255,255), c, LARGURA_TELA/3, ALTURA_TELA/8, 
		"BadBaby GAMES              Decrypt - The Game             Aluno            =           Desenvolvedor             =             Designer              =                 Jean Duarte                         Obrigado ao Google e a todos que me ajudaram!                             0______0                            Obrigado por jogar!");
		
		al_draw_bitmap(blocoEsq, -100, 0, 0);
		al_draw_bitmap(blocoDir, 300, 0, 0);
		
			
		al_flip_display();
		c--;
	}
	
	fadeinWindow(1, false);
	
	al_destroy_sample(creditSound);	
	al_destroy_bitmap(fundoCredits);
}

void intro(){	
		
	al_set_target_bitmap( al_get_backbuffer( janela ) ); 

	al_set_window_title(janela, "BadBaby GAMES");

	al_draw_bitmap(logo, 110, 50, 0);
	
	al_play_sample(introBadbaby, 1.0, 1.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
	
	fadeinWindow(0.5, true);
	fadeinWindow(0.5, false);

	al_destroy_sample(introBadbaby);
	al_destroy_bitmap(logo);
}

void gameOver(){	
	textFont 	=	al_load_font("fontes/orderElfic.ttf", 100, 0);
	
	al_set_target_bitmap( al_get_backbuffer( janela ) ); 

	al_set_window_title(janela, "GAME OVER - YOU LOSE - YOU DIED - GANHOU UM OVO");

	al_draw_bitmap(fundoGameOver, 0, 0, 0);
	
	al_play_sample(gameOverSound, 1.0, 1.0, 0.9, ALLEGRO_PLAYMODE_ONCE, NULL);
	al_rest(1);	
		
	al_draw_text(textFont, al_map_rgb(255,0,0), (LARGURA_TELA/8)-30,(ALTURA_TELA/3)+30, 0, "GAME OVER");	
	al_draw_bitmap(blocoEsq, 0, 0, 0);
	al_draw_bitmap(blocoDir, 300, 0, 0);	
	
	al_play_sample(devilLaugh, 2.0, 1.0, 0.8, ALLEGRO_PLAYMODE_ONCE, NULL);
	
	fadeinWindow(0.5, true);
	
	al_rest(2);	
	
	al_destroy_sample(gameOverSound);
	al_destroy_bitmap(fundoGameOver);
	al_destroy_bitmap(blocoDir);
	al_destroy_bitmap(blocoEsq);
}

void fadeinWindow(int velocidade, bool fadeIn){
    ALLEGRO_BITMAP *buffer = NULL;
    buffer = al_create_bitmap(LARGURA_TELA, ALTURA_TELA);
    al_set_target_bitmap(buffer);
    al_draw_bitmap(al_get_backbuffer(janela), 0, 0, 0);
    al_set_target_bitmap(al_get_backbuffer(janela));
 
    if (velocidade <= 0)
    {
        velocidade = 1;
    }
    else if (velocidade > 15)
    {
        velocidade = 15;
    }
 
    int alfa;
    for (alfa = 0; alfa <= 255; alfa += velocidade)
    {
        al_clear_to_color(al_map_rgba(0, 0, 0, 0));
        if(fadeIn)
	        al_draw_tinted_bitmap(buffer, al_map_rgba(alfa, alfa, alfa, alfa), 0, 0, 0);
    	else
    		al_draw_tinted_bitmap(buffer, al_map_rgba(255-alfa, 255-alfa, 255-alfa, 255-alfa), 0, 0, 0);
        al_flip_display();
        al_rest(0.005); 
    }
 
    al_destroy_bitmap(buffer);
}

void fadein(ALLEGRO_BITMAP *imagem, int velocidade){
    if (velocidade < 0)
    {
        velocidade = 1;
    }
    else if (velocidade > 15)
    {
        velocidade = 15;
    }
 
    int alfa;
    for (alfa = 0; alfa <= 255; alfa += velocidade)
    {
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_tinted_bitmap(imagem, al_map_rgba(alfa, alfa, alfa, alfa), -30, -100, 0);
        al_flip_display();
        al_rest(0.005); // Não é necessário caso haja controle de FPS
                
    }
}

void criarChama(){

	fogos[0].imagem = al_load_bitmap("imagens/tocha/fogo1.png");	
	fogos[1].imagem = al_load_bitmap("imagens/tocha/fogo2.png");	
	fogos[2].imagem = al_load_bitmap("imagens/tocha/fogo3.png");	
	fogos[3].imagem = al_load_bitmap("imagens/tocha/fogo4.png");	

}

void animarChama(int *c){

	if(*c >= 4) *c = 0;
	fogo = fogos[*c].imagem;			
	al_flip_display();
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_draw_bitmap(fogo, 0, 0, 0);		
	al_rest(0.5);


}

void atualizarBackgroundRightLeft(BACKGROUND *fundo){
	fundo->x += fundo->velX * fundo->dirX;
	if(fundo->x + fundo->largura <= 0)
		fundo->x = 0;
}

void atualizarBackgrouddndDownUp(BACKGROUND *fundo){
	fundo->y += fundo->velY * fundo->dirX;
	if(fundo->y + fundo->altura >= 0)
		fundo->y = 0;
}

void inicializarBackground(BACKGROUND *fundo, float x, float y, float velX, float velY, int largura, int altura, int dirX, int dirY, ALLEGRO_BITMAP *imagem ){
	
	fundo->x = x;
	fundo->y = y;
	fundo->velX = velX;
	fundo->velY = velY;
	fundo->dirX = dirX;
	fundo->dirY = dirY;
	fundo->altura = altura;
	fundo->largura = largura;
	fundo->imagem = imagem;	
	
}

void desenharBackgroundDownUp(BACKGROUND *fundo){
	al_draw_bitmap(fundo->imagem, fundo->x, fundo->y, 0);
	
	if(fundo->y + fundo->altura > TELA_ALTURA)
		al_draw_bitmap(fundo->imagem, fundo->x, -fundo->y+fundo->altura, 0);
}

void desenharBackgroundRightLeft(BACKGROUND *fundo){
	al_draw_bitmap(fundo->imagem, fundo->x, fundo->y, 0);
	
	if(fundo->x + fundo->largura < TELA_LARGURA)
		al_draw_bitmap(fundo->imagem, fundo->x+fundo->largura, fundo->y, 0);
}


void playReadyGo(){
	al_play_sample(devilLaugh, 3.0, 1.0, 1.5, ALLEGRO_PLAYMODE_ONCE, NULL);
	al_rest(1);
	
}

bool menuPrincipal(void){
	
	BACKGROUND BG;
	BACKGROUND BG2;
	BACKGROUND BG3;
	
	int pos = 0;
	
	bool start = false;
	
	bool jogar = false;
    bool sair = false;
    bool render = false;
 	
	textFont 	=	al_load_font("fontes/orderElfic.ttf", 28, 0);
    eventos 	= 	al_create_event_queue();
	
	btJogar 	= 	al_create_bitmap(200, 50);
	btSair 		= 	al_create_bitmap(200, 50);	
	tempo 		= 	al_create_timer(1.0/60);
	
	registrarInputs();
	
	al_set_window_title(janela, "DECRYPT - THE GAME");
	al_play_sample(musica, 1.0, 1.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
 	
	al_set_target_bitmap(btJogar);
	al_clear_to_color( al_map_rgb(77, 77, 77) );
	al_set_target_bitmap(btSair);
	al_clear_to_color( al_map_rgb(77, 77, 77) );

	inicializarBackground(&BG, -30, -100, 1, 0, LARGURA_TELA, ALTURA_TELA, -1, 1, fundo1);

	inicializarBackground(&BG2, 800, -100, 1, 0, LARGURA_TELA, ALTURA_TELA, -1, 1, fires1);
	
	inicializarBackground(&BG3, 800, -100, 0.5, 0, LARGURA_TELA, ALTURA_TELA, -1, 1, fires2);
	
	//LOOP DO MENU
 	while( !sair ){ 	 		
		al_set_target_bitmap( al_get_backbuffer( janela ) ); 		 		 		

		desenharBackgroundRightLeft(&BG);//tanto faz o tipo de desenhar		

		desenharBackgroundRightLeft(&BG3);
		atualizarBackgroundRightLeft(&BG3);				

		al_draw_bitmap(btJogar, 200, 250, 0);		
		al_draw_bitmap(btSair, 200, 350, 0);	

		al_draw_text(textFont, al_map_rgb(255,255,255), 260, 260, 0, "JOGAR");//botão para jogar com cor BASE
		al_draw_text(textFont, al_map_rgb(255,255,255), 270, 360, 0, "SAIR");//botão para sair com cor BASE

		desenharBackgroundRightLeft(&BG2);
		atualizarBackgroundRightLeft(&BG2);

		if(start == false){
			 
			 fadeinWindow(1, true);
			 start = true;
		}
			
 		while ( !al_is_event_queue_empty(eventos) ){
	        
			ALLEGRO_EVENT evento;        
		    al_wait_for_event(eventos, &evento);
	 		
	 		if( evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE ){
	 			sair = true;
	 		
	 		}else if( evento.type == ALLEGRO_EVENT_MOUSE_AXES){
		
				if(evento.mouse.x >= 200 && evento.mouse.y >= 250 && evento.mouse.x <= LARGURA_TELA - 250 && evento.mouse.y <= ALTURA_TELA - 200){					
					al_set_target_bitmap(btJogar);
					al_clear_to_color( al_map_rgb(165, 42, 42) );				
					al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK); //botão para jogar
				
				}else if(evento.mouse.x >= 200 && evento.mouse.y >= 350 && evento.mouse.x <= LARGURA_TELA - 250 && evento.mouse.y <= ALTURA_TELA - 100){
					al_set_target_bitmap(btSair);
					al_clear_to_color( al_map_rgb(255, 0, 0) );
					al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);//botão para sair
				
				}else{
					al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_ARROW);
					al_set_target_bitmap(btJogar);
					al_clear_to_color( al_map_rgb(77, 77, 77) );
					al_set_target_bitmap(btSair);
					al_clear_to_color( al_map_rgb(77, 77, 77) );

				}
		
			}else if ( evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN ){
			   
				if (evento.mouse.x >= 200 && evento.mouse.y >= 350 && evento.mouse.x <= LARGURA_TELA - 250 && evento.mouse.y <= ALTURA_TELA - 100){				
				   	//botão para sair
				   	sair = true;
					jogar = false;		
		
				}else if(evento.mouse.x >= 200 && evento.mouse.y >= 250 && evento.mouse.x <= LARGURA_TELA - 250 && evento.mouse.y <= ALTURA_TELA - 200){
				   	//botão para jogar				   	
					sair = true;
					jogar = true;						
					playReadyGo();	
					fadeinWindow(2.5,false);			
				}
				
			}else if(evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){
				
					if(evento.mouse.button & 2)
						printf("bt dir\n");
					else if(evento.mouse.button & 1)
						printf("bt esq\n");				
				
			}
        }

 		al_flip_display();

 	}
 	
	al_destroy_bitmap( fundo1 );	
	al_destroy_sample( musica );	
	al_destroy_event_queue( eventos ); 

 	return jogar;
}
 
bool inicializar(){
	
		
    if ( !al_init() ){
        fprintf(stderr, "Falha ao inicializar ALLEGRO.\n");
        return false;
    }
 
	if( !al_init_image_addon() ){
		fprintf(stderr, "Falha ao inicializar IMAGE ADDON.\n");
        return false;
	}
	
 
    if ( !al_install_audio() ){
        fprintf(stderr, "Falha ao inicializar ÁUDIO.\n");
        return false;
    }
 
    if ( !al_init_acodec_addon() ){
        fprintf(stderr, "Falha ao inicializar CODECS DE ÁUDIO.\n");
        return false;
    }
 
    if ( !al_reserve_samples(6) ){
        fprintf(stderr, "Falha ao alocar CANAIS DE ÁUDIO.\n");
        return false;
    }
 
    if ( !al_install_keyboard() ){
        fprintf(stderr, "Falha ao inicializar TECLADO.\n");
        return false;
    }
    
    if ( !al_install_mouse() ){
        fprintf(stderr, "Falha ao inicializar MOUSE.\n");
        return false;
    }
 
    if ( !al_init_font_addon() ){
        fprintf(stderr, "Falha ao inicializar FONT ADDON.\n");
        return false;
    }
    
    if ( !al_init_ttf_addon() ){
        fprintf(stderr, "Falha ao inicializar TTF ADDON.\n");
        return false;
    }

    if ( !al_init_primitives_addon() ){
        fprintf(stderr, "Falha ao inicializar PRIMITIVES ADDON.\n");
        return false;
    }   
 
	initBitmap();
	initSample();
 
    return true;
}



//END
