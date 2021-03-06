#include "lib.h"

bool keys[] = {false, false, false, false};
enum KEYS{UP, DOWN, LEFT, RIGHT};
enum tiles{ROCK, GRASS, PORTA1, PORTA2, PORTA3, PORTA4, PORTA5, PORTA6, PORTA7, PORTA8, DOOROPEN};
enum quiz{PERMUT_SIMPLES};

int yE=1, xE=1;

float cameraPosicao[2] = {0,0} ;

bool ganhou = false;

ALLEGRO_TRANSFORM camera;

ALLEGRO_FONT *textFont 			= NULL; 
ALLEGRO_DISPLAY *window 		= NULL;
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

ALLEGRO_BITMAP *ghost	 		= NULL;


ALLEGRO_BITMAP *fundoCredits 	= NULL;
ALLEGRO_BITMAP *book 			= NULL;
ALLEGRO_BITMAP *grama 			= NULL;
ALLEGRO_BITMAP *rocha 			= NULL;
ALLEGRO_BITMAP *porta1f 		= NULL;
ALLEGRO_BITMAP *porta1a 		= NULL;
ALLEGRO_BITMAP *porta2f 		= NULL;
ALLEGRO_BITMAP *porta2a 		= NULL;

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
void initItem(ITEM *livro, ALLEGRO_BITMAP *explicacao,  int x, int y);
void getItem(int xOFF, int yOFF, ITEM *livro);
void printStage(int mapa[DIM_COLUNA][DIM_LINHA]);
void allegroInput(ALLEGRO_EVENT evento);
void exibir_texto_centralizado();
int telaPorta();
int relogio();
void playEnime(int mapa[DIM_COLUNA][DIM_LINHA], ENIME *vilao, bool pos, int personagemX, int personagemY);
void initEnime(ENIME *vilao, int x, int y, int vel);
void clearWindow();
bool enimeAtack(ENIME *vilao, int xOFF, int yOFF, int mapa[DIM_COLUNA][DIM_LINHA]);
void showInfo();
void waitFor();

void waitFor(){
	while (true){
		
		ALLEGRO_EVENT evento;        
		al_wait_for_event(eventos, &evento);
		
		
		if(evento.keyboard.keycode == ALLEGRO_KEY_ENTER){
			break;
		}
		
		
		
	}
}

void showInfo(){

	al_set_target_bitmap( al_get_backbuffer( janela ) ); 
		
	ALLEGRO_BITMAP *info = al_load_bitmap("imagens/info.png");
	al_draw_bitmap(info, 0, 0, 0);
	al_flip_display();
	
	waitFor();
	al_destroy_bitmap(info);
	
}

void clearWindow(){
	
	if(window != NULL){				
		al_set_target_bitmap( al_get_backbuffer( window ) );		
		al_clear_to_color(al_map_rgb(0,0,0));
		al_flip_display();
		al_set_target_bitmap( al_get_backbuffer( janela) );		
		
	}
	
}

bool enimeAtack(ENIME *vilao, int xOFF, int yOFF, int mapa[DIM_COLUNA][DIM_LINHA]){
	bool atack = false;
	
	if( (vilao->x == xOFF) && (vilao->y == yOFF) )	
		atack = true;		
	
	//atack a distancia - duas casas a frente	
	if( (vilao->x+TILE_TAM == xOFF) && (vilao->y == yOFF) )
		atack = true;
	
	if( (vilao->x-TILE_TAM == xOFF) && (vilao->y == yOFF) )
		atack = true;
		
	if( (vilao->x == xOFF) && (vilao->y+TILE_TAM == yOFF) )
		atack = true;
		
	if( (vilao->x == xOFF) && (vilao->y-TILE_TAM == yOFF) )
		atack = true;	
	
	
	if(atack)
		mapa[vilao->y/TILE_TAM][vilao->x/TILE_TAM] = 0;
		
	return atack;
}

void playEnime(int mapa[DIM_COLUNA][DIM_LINHA], ENIME *vilao, bool pos, int personagemX, int personagemY){

	
	vilao->check++;
	//se verdade || true - mover na horizontal
	if(vilao->check > vilao->lazy){
		
						
		if(vilao->qtd < 10 && vilao->go){
			vilao->mode = 0;				
			vilao->qtd++;
			if(pos)
				vilao->x = vilao->x+TILE_TAM;
			else
				vilao->y = vilao->y+TILE_TAM;
			vilao->go = true;
			
		}
		else{
			vilao->mode = 1;
			vilao->qtd--;	
			if(pos)
				vilao->x = vilao->x-TILE_TAM;
			else
				vilao->y = vilao->y-TILE_TAM;
			vilao->go = false;
			
			if(vilao->qtd == 0)
				vilao->go = true;
				
			
		}
	
		vilao->check = 0;
	}
	
	/*printf("POS ENIME: y[%d]x[%d]qtd[%d]check[%d]", vilao->y,vilao->x,vilao->qtd,vilao->check);
	if(vilao->go)
		puts("true");
	else
		puts("false");
	*/
}

void initEnime(ENIME *vilao,int x, int y, int vel){
	
	vilao->imagem = ghost;//ghost é global
	vilao->vivo = true;
	vilao->x = x;
	vilao->y = y;
	vilao->lazy = vel;
	vilao->qtd = 0;
	vilao->go = true;	
	vilao->check = 0;
	
}

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
	al_clear_to_color( al_map_rgb(0, 200, 0 ) );
	
		
	
	personagem 		= 		al_load_bitmap("imagens/ned/personagem.jpg");
	enime 			= 		al_create_bitmap(50,50);
	al_set_target_bitmap(enime);
	al_clear_to_color( al_map_rgb(0, 0, 255 ) );
	
	grama  			= 		al_load_bitmap("imagens/background/grass.png");
	rocha  			= 		al_load_bitmap("imagens/background/rock1.png");
	porta1f			= 		al_load_bitmap("imagens/background/porta1f.png");
	porta1a 		= 		al_load_bitmap("imagens/background/porta1a.png");
	porta2f			= 		al_load_bitmap("imagens/background/porta2f.png");
	porta2a 		= 		al_load_bitmap("imagens/background/porta2a.png");



	book			= 		al_load_bitmap("imagens/background/book.png");
	fundo1 			= 		al_load_bitmap("imagens/background/head2.png");	
	fires1 			= 		al_load_bitmap("imagens/background/fires.png");
	fires2 			= 		al_load_bitmap("imagens/background/fires2.png");


	fundoMenu		=		al_load_bitmap("imagens/fundo.png");		
	sombra  		=		al_load_bitmap("imagens/background/shadow.png");
	luz  			=		al_load_bitmap("imagens/background/light.png");		
	
	ned1   			= 		al_load_bitmap("imagens/ned/ned_down.png");
	ned2 			= 		al_load_bitmap("imagens/ned/ned_left.png");	

	ghost			= 		al_load_bitmap("imagens/background/enime.png");
	


	text 			= 		al_load_bitmap("imagens/credits/text.png");
	fundoGameOver	= 		al_load_bitmap("imagens/fundo.png");	
	fundoCredits	= 		al_load_bitmap("imagens/fundoCredits.jpg");	
	blocoEsq		= 		al_load_bitmap("imagens/credits/left.png");
	blocoDir		= 		al_load_bitmap("imagens/credits/right.png");	

	logo 			= 		al_load_bitmap("imagens/badbaby.png");	
	fogo 			= 		al_load_bitmap("imagens/background/lava.png");		
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
    al_set_window_position(janela, 0,0);
    
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
	
	int passe = 0, pos = 0, tipo = 0;
	int Y = *py, X = *px;
	
				
	if( mapa[Y-1][X] == 2 || mapa[Y-1][X] == 3 || mapa[Y-1][X] == 4 || mapa[Y-1][X] == 5 || mapa[Y-1][X] == 6 || mapa[Y-1][X] == 7 || mapa[Y-1][X] == 8)//acima
		pos = -1;
	if( mapa[Y+1][X] == 2 || mapa[Y+1][X] == 3 || mapa[Y+1][X] == 4 || mapa[Y+1][X] == 5 || mapa[Y+1][X] == 6 || mapa[Y+1][X] == 7 || mapa[Y+1][X] == 8)// abaixo
		pos = 1;
	if( mapa[Y][X-1] == 2 || mapa[Y][X-1] == 3 || mapa[Y][X-1] == 4 || mapa[Y][X-1] == 5 || mapa[Y][X-1] == 6 || mapa[Y][X-1] == 7 || mapa[Y][X-1] == 8)//a esquerda
		pos = -2;
	if( mapa[Y][X+1] == 2 || mapa[Y][X+1] == 3 || mapa[Y][X+1] == 4 || mapa[Y][X+1] == 5 || mapa[Y][X+1] == 6 || mapa[Y][X+1] == 7 || mapa[Y][X+1] == 8)//a direita
		pos = 2;		
		
	if(pos != 0){
		
		if(pos == -1) tipo = mapa[Y-1][X];
		if(pos == 1) tipo = mapa[Y+1][X];
		if(pos == -2) tipo = mapa[Y][X-1];
		if(pos == 2) tipo = mapa[Y][X+1];
		
		passe = telaPorta(tipo);
		
		if(passe == 1){
			printf("\n\nPASSE CORRETO\n\n");

			al_play_sample(openDoor, 1.0, 1.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);	
			al_rest(4);
			
			if(pos == 1)
				mapa[Y+1][X] = DOOROPEN; 
			if(pos == -1)
				mapa[Y-1][X] = DOOROPEN;
			if(pos == 2)
				mapa[Y][X+1] = DOOROPEN;
			if(pos == -2)
				mapa[Y][X-1] = DOOROPEN;				
				
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
	
}

void getItem(int xOFF, int yOFF, ITEM *livro){
	
	if(xOFF == livro->x && yOFF == livro->y){
		
		al_play_sample(bookSound, 1.0, 1.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);		
		livro->pego = true;	
		
		if(window == NULL)
			window = al_create_display(400, 300);		
		
		al_set_target_bitmap( al_get_backbuffer( window ) );		
		al_draw_bitmap(livro->explicacao, 0, 0, 0);

		al_flip_display();				
		
		al_set_target_bitmap( al_get_backbuffer( janela ) );		

	}
	
}

void initItem(ITEM *livro, ALLEGRO_BITMAP *explicacao,  int x, int y){
	livro->item = book;//imagem livro
	livro->explicacao = explicacao;
	livro->id = 1;
	livro->x = x;
	livro->y = y;
	livro->lido = false;
	livro->pego = false;
	
	
}

void printStage(int mapa[DIM_COLUNA][DIM_LINHA]){
	
	int x,y;
	for(y=0;y<DIM_COLUNA;y++){
		
		for(x=0; x<DIM_LINHA; x++){
			
			if(mapa[y][x] != ROCK)//se diferente de parede desenha grama, assim terá grama onde ficam as portas
				al_draw_bitmap(grama, x*TILE_TAM, y*TILE_TAM, 0);	
							
			if(mapa[y][x] == ROCK)				
				al_draw_bitmap(rocha, x*TILE_TAM, y*TILE_TAM, 0);		
			
			if(mapa[y][x] == 2 || mapa[y][x] == 3 || mapa[y][x] == 4 || mapa[y][x] == 5 || mapa[y][x] == 6 || mapa[y][x] == 7 || mapa[y][x] == 8)
				al_draw_bitmap(porta1f, x*TILE_TAM, y*TILE_TAM, 0);		
			
			if(mapa[y][x] == DOOROPEN)
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
	
	ganhou = false;
	level1Sound 	=		al_load_sample("som/level1.ogg");
	fundoMenu		=		al_load_bitmap("imagens/fundo.png");	
	
	al_start_timer(tempo);
	al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_ARROW);
	al_set_window_title(janela, "DECRYPT - THE GAME - Playing...");
		
	ALLEGRO_BITMAP *explicacao1 = al_load_bitmap("imagens/toLearn/permutacaoSimples.png");
	ALLEGRO_BITMAP *explicacao2 = al_load_bitmap("imagens/toLearn/permutacaoRepeticao.png");
	ALLEGRO_BITMAP *explicacao3 = al_load_bitmap("imagens/toLearn/permutacaoCircular.png");
	ALLEGRO_BITMAP *explicacao4 = al_load_bitmap("imagens/toLearn/combinacaoSimples.png");
	ALLEGRO_BITMAP *explicacao5 = al_load_bitmap("imagens/toLearn/combinacaoRepeticao.png");
	ALLEGRO_BITMAP *explicacao6 = al_load_bitmap("imagens/toLearn/cifra1.png");
	ALLEGRO_BITMAP *explicacao7 = al_load_bitmap("imagens/toLearn/cifra2.png");
	ALLEGRO_BITMAP *explicacao8 = al_load_bitmap("imagens/toLearn/paraMais.png");
	
	extern int mapa1[DIM_COLUNA][DIM_LINHA];
	extern int mapa2[DIM_COLUNA][DIM_LINHA];
	extern int mapa3[DIM_COLUNA][DIM_LINHA];		
	extern int mapa4[DIM_COLUNA][DIM_LINHA];
	extern int mapa5[DIM_COLUNA][DIM_LINHA];
	extern int mapa6[DIM_COLUNA][DIM_LINHA];
	extern int mapa7[DIM_COLUNA][DIM_LINHA];
	extern int mapa8[DIM_COLUNA][DIM_LINHA];
		
	int mapa[DIM_COLUNA][DIM_LINHA];
	int estagios = 1;
		
	BACKGROUND BG;
	
	ITEM livro1;

	ENIME vilao1;
	ENIME vilao2;	
	ENIME vilao3;
	ENIME vilao4;
	
	
	bool render		= false;
	bool sair 		= false;
	bool aborta		= false;
	bool gotBook 	= false;
	bool flagPos	= true;
	
	
	int contResp = 0;
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
		
	jogador = ned1;
		
	showInfo();	
	
	al_play_sample(level1Sound, 0.5, 1.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
			
	inicializarBackground(&BG, 0, 0, 1, 0, LARGURA_TELA, ALTURA_TELA, -1, 1, fundoMenu);

	
	while( !ganhou && !aborta){
		
		
		if(estagios == 1){						
			copiarMapa(mapa, mapa1);			
			initItem(&livro1, explicacao1, 29*TILE_TAM, 28*TILE_TAM);				
			initEnime(&vilao1, 18*TILE_TAM, 1*TILE_TAM, 10);			
			initEnime(&vilao2, 14*TILE_TAM, 34*TILE_TAM, 6);
			initEnime(&vilao3, 2*TILE_TAM, 1*TILE_TAM, 10);	
			initEnime(&vilao4, 25*TILE_TAM, 20*TILE_TAM, 10);								
			sair = false;
			
		}else if(estagios == 2){
			copiarMapa(mapa, mapa2);
			initItem(&livro1, explicacao2, 27*TILE_TAM, 5*TILE_TAM);
			initEnime(&vilao1,27*TILE_TAM, 5*TILE_TAM, 10);
			initEnime(&vilao2,2*TILE_TAM, 1*TILE_TAM, 10);	
			initEnime(&vilao3,40*TILE_TAM, 30*TILE_TAM, 10);	
			initEnime(&vilao4,4*TILE_TAM, 3*TILE_TAM, 10);	
			sair = false;
			
		}else if(estagios == 3){						
			copiarMapa(mapa, mapa3);
			initItem(&livro1, explicacao3, 37*TILE_TAM, 3*TILE_TAM);
			initEnime(&vilao1, 2*TILE_TAM, 20*TILE_TAM, 5);	
			initEnime(&vilao2, 2*TILE_TAM, 2*TILE_TAM, 15);	
			initEnime(&vilao3, 14*TILE_TAM, 34*TILE_TAM, 6);	
			initEnime(&vilao4, 25*TILE_TAM, 20*TILE_TAM, 10);	
			sair = false;
			
		}else if(estagios == 4){						
			copiarMapa(mapa, mapa4);
			initItem(&livro1, explicacao4, 15*TILE_TAM, 24*TILE_TAM);
			initEnime(&vilao1, 2*TILE_TAM, 20*TILE_TAM, 5);	
			initEnime(&vilao2, 14*TILE_TAM, 34*TILE_TAM, 3);	
			initEnime(&vilao3, 25*TILE_TAM, 20*TILE_TAM, 6);	
			initEnime(&vilao4, 5*TILE_TAM, 2*TILE_TAM, 4);	
			sair = false;
			
		}else if(estagios == 5){						
			copiarMapa(mapa, mapa5);
			initItem(&livro1, explicacao5, 15*TILE_TAM, 24*TILE_TAM);
			initEnime(&vilao1,  2*TILE_TAM, 20*TILE_TAM, 5);	
			initEnime(&vilao2, 14*TILE_TAM, 34*TILE_TAM, 3);	
			initEnime(&vilao3, 25*TILE_TAM, 20*TILE_TAM, 6);	
			initEnime(&vilao4, 5*TILE_TAM, 2*TILE_TAM, 4);	
			sair = false;
			
		}else if(estagios == 6){						
			copiarMapa(mapa, mapa6);
			initItem(&livro1, explicacao6, 15*TILE_TAM, 24*TILE_TAM);
			initEnime(&vilao1, 2*TILE_TAM, 20*TILE_TAM, 6);	
			initEnime(&vilao2, 14*TILE_TAM, 34*TILE_TAM, 3);	
			initEnime(&vilao3, 25*TILE_TAM, 20*TILE_TAM, 4);	
			initEnime(&vilao4, 5*TILE_TAM, 2*TILE_TAM, 2);	
			sair = false;
			
		}else if(estagios == 7){						
			copiarMapa(mapa, mapa7);
			initItem(&livro1, explicacao7, 15*TILE_TAM, 24*TILE_TAM);
			initEnime(&vilao1, 2*TILE_TAM, 20*TILE_TAM, 2);	
			initEnime(&vilao2, 14*TILE_TAM, 34*TILE_TAM, 2);	
			initEnime(&vilao3, 25*TILE_TAM, 20*TILE_TAM, 2);	
			initEnime(&vilao4, 5*TILE_TAM, 2*TILE_TAM, 2);	
			sair = false;
			
		}else if(estagios == 8){						
			copiarMapa(mapa, mapa8);
			initItem(&livro1, explicacao8, 15*TILE_TAM, 23*TILE_TAM);
			initEnime(&vilao1, 2*TILE_TAM, 20*TILE_TAM, 4);	
			initEnime(&vilao2, 14*TILE_TAM, 34*TILE_TAM, 4);	
			initEnime(&vilao3, 25*TILE_TAM, 20*TILE_TAM, 4);	
			initEnime(&vilao4, 5*TILE_TAM, 2*TILE_TAM, 4);	
			sair = false;
			
		}else{					
						
			xE = xOFF; yE = yOFF;	
			ganhou = true;
			sair = true;				
		}			
		
		
		//aqui é onde ficar toda execução real do jogo
		while( !sair ){		
			
			al_set_target_bitmap( al_get_backbuffer( janela ) ); 
			
			al_draw_bitmap(backGround, 0, 0, 0);
			
			printStage(mapa);
						
			//-------------LIVROS---------------------
			if(!livro1.pego){
				al_draw_bitmap(livro1.item, livro1.x, livro1.y, 0);				
				getItem(xOFF, yOFF, &livro1);			
			}

			al_draw_bitmap(jogador, xOFF, yOFF, move);	
			//----------------------------------------
			
			
			//---------------------ENIME-----------			
			al_draw_bitmap(vilao1.imagem, vilao1.x, vilao1.y, 2);					
			playEnime(mapa, &vilao1, false, xOFF*TILE_TAM, yOFF*TILE_TAM);
			
			al_draw_bitmap(vilao2.imagem, vilao2.x, vilao2.y, 2);		
			playEnime(mapa, &vilao2, true, xOFF*TILE_TAM, yOFF*TILE_TAM);		
			
			al_draw_bitmap(vilao3.imagem, vilao3.x, vilao3.y, 3);					
			playEnime(mapa, &vilao3, false, xOFF*TILE_TAM, yOFF*TILE_TAM);
						
			al_draw_bitmap(vilao4.imagem, vilao4.x, vilao4.y, 3);					
			playEnime(mapa, &vilao4, false, xOFF*TILE_TAM, yOFF*TILE_TAM);			
						 
			if( enimeAtack(&vilao1, xOFF, yOFF, mapa) == true || enimeAtack(&vilao2, xOFF, yOFF, mapa) == true || enimeAtack(&vilao3, xOFF, yOFF, mapa) == true || enimeAtack(&vilao4, xOFF, yOFF, mapa) == true){
				clearWindow();
				al_destroy_sample(level1Sound);
				return false;
			}	
			//----------------------------------------

			//------ATUALIZA POSIÇÃO CÂMERA-----------
			al_draw_bitmap(blocoEsq, cameraPosicao[0]-100, cameraPosicao[1], 0);
			al_draw_bitmap(blocoDir, cameraPosicao[0]+335, cameraPosicao[1], 0);
			//----------------------------------------

			while ( !al_is_event_queue_empty(eventos) ){
				ALLEGRO_EVENT evento;        
				al_wait_for_event(eventos, &evento);
				
				atualizarCamera(cameraPosicao, xOFF, yOFF, 0, 0);
				
				al_identity_transform(&camera);
				al_translate_transform(&camera, -cameraPosicao[0], -cameraPosicao[1]);
				al_use_transform(&camera);

				
			
				if(evento.type == ALLEGRO_EVENT_KEY_DOWN && evento.keyboard.keycode == ALLEGRO_KEY_ENTER){
					abrirPorta(mapa, &px, &py, evento);			
				}		
		
				
				
				if(mapa[py][px] == DOOROPEN){
					sair = true;					
					estagios += 1;					
					printf("PRÓXIMO NÍVEL\n\n");
				}

				if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
									
					sair = true;
					aborta = true;
					
						
				}else if(evento.type == ALLEGRO_EVENT_KEY_DOWN){
					
					
					if(evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
						
						sair = true;
						aborta = true;
					
					}
					
					if(evento.keyboard.keycode == ALLEGRO_KEY_LEFT){
					
						if(mapa[py][px-1] != ROCK && !(mapa[py][px-1] >= 2 && mapa[py][px-1] <= 8) ){
								xOFF -= TILE_TAM;
								px -= 1;
								jogador = ned2;
								move = 2;			
								playSomPasso(mapa, px, py);							
						}				
					}

					if(evento.keyboard.keycode == ALLEGRO_KEY_RIGHT){					
						if(mapa[py][px+1] != ROCK && !(mapa[py][px+1] >= 2 && mapa[py][px+1] <= 8)){					
							xOFF += TILE_TAM;
							px += 1;
							jogador = ned2;
							move = 1;						
							playSomPasso(mapa, px, py);							
						}				
					}
					
					if(evento.keyboard.keycode == ALLEGRO_KEY_DOWN){					
						if(mapa[py+1][px] != ROCK && !(mapa[py+1][px] >= 2 && mapa[py+1][px] <= 8)){					
							yOFF += TILE_TAM;
							py += 1;
							jogador = ned1;
							move = 1;			
							playSomPasso(mapa, px, py);							
						}				
					}
					
					if(evento.keyboard.keycode == ALLEGRO_KEY_UP){					
						if(mapa[py-1][px] != ROCK && !(mapa[py-1][px] >= 2 && mapa[py-1][px] <= 8)){					
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
	
	clearWindow();
	return true;
}


void credits(){	
	cameraPosicao[0] = 0;
	cameraPosicao[1] = 0;

	al_identity_transform(&camera);
	al_translate_transform(&camera, cameraPosicao[0], cameraPosicao[1]);
	al_use_transform(&camera);

	al_set_target_bitmap( al_get_backbuffer( janela ) ); 
	BACKGROUND BG;
	textFont 	=	al_load_font("fontes/orderElfic.ttf", 50, 0);
	
	inicializarBackground(&BG, cameraPosicao[0], cameraPosicao[1], 1, 0, LARGURA_TELA, ALTURA_TELA, -1, 1, fundoCredits);
	
	al_play_sample(creditSound, 2.0, 1.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
	al_set_window_title(janela, "Credits");

	int c=500;
	while(c > -7500){				

		desenharBackgroundRightLeft(&BG);
		al_draw_text(textFont, al_map_rgb(255,255,255), c, cameraPosicao[1]+200, ALTURA_TELA/8, 
		"BadBaby GAMES          Decrypt - The Game             Aluno            =           Desenvolvedor             =             Designer              =                 Jean Duarte                         Obrigado ao Google e a todos que me ajudaram!                             0______0                            Obrigado por jogar!");
		
		al_draw_bitmap(blocoEsq, cameraPosicao[0], cameraPosicao[1], 0);
		al_draw_bitmap(blocoDir, cameraPosicao[0]+250, cameraPosicao[1], 0);
		
			
		al_flip_display();
		c--;
	}
	
	fadeinWindow(1, false);
	al_destroy_sample(creditSound);

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
	cameraPosicao[0] = 0;
	cameraPosicao[1] = 0;

	al_identity_transform(&camera);
	al_translate_transform(&camera, cameraPosicao[0], cameraPosicao[1]);
	al_use_transform(&camera);

	al_set_target_bitmap( al_get_backbuffer( janela ) ); 
	
	textFont 	=	al_load_font("fontes/orderElfic.ttf", 100, 0);
	
	al_set_window_title(janela, "GAME OVER - YOU LOSE - YOU DIED - GANHOU UM OVO");
	
	al_draw_bitmap(fundoMenu, cameraPosicao[0], cameraPosicao[1], 0);
	
	al_play_sample(gameOverSound, 1.0, 1.0, 0.9, ALLEGRO_PLAYMODE_ONCE, NULL);
	al_rest(1);	
		
	al_draw_text(textFont, al_map_rgb(255,0,0), (LARGURA_TELA/8)-30,(ALTURA_TELA/3)+30, 0, "GAME OVER");	
	al_draw_bitmap(blocoEsq, 0, 0, 0);
	al_draw_bitmap(blocoDir, 300, 0, 0);	
	
	al_play_sample(devilLaugh, 2.0, 1.0, 0.8, ALLEGRO_PLAYMODE_ONCE, NULL);
	
	fadeinWindow(0.5, true);
	
	al_rest(3);	
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
	cameraPosicao[0] = 0;
	cameraPosicao[1] = 0;

	al_identity_transform(&camera);
	al_translate_transform(&camera, cameraPosicao[0], cameraPosicao[1]);
	al_use_transform(&camera);


	BACKGROUND BG;
	BACKGROUND BG2;
	BACKGROUND BG3;
	
	int pos = 0;
	
	bool start = false;
	
	bool jogar = false;
    bool sair = false;
    bool render = false;
 	
 	musica 		= 	al_load_sample("som/fundo.ogg");
	textFont 	=	al_load_font("fontes/orderElfic.ttf", 28, 0);
	fundo1 		=	al_load_bitmap("imagens/background/head2.png");	
	fires1 		=	al_load_bitmap("imagens/background/fires.png");
	fires2 		=	al_load_bitmap("imagens/background/fires2.png");

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
 	
	
	al_destroy_sample( musica );	
	al_destroy_bitmap(fundo1);
	al_destroy_bitmap(fires1);
	al_destroy_bitmap(fires2);
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
