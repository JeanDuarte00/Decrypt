#include "lib.h"
 
ALLEGRO_BITMAP *fundo = NULL;
ALLEGRO_DISPLAY *screem = NULL;
ALLEGRO_FONT *fonte = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;

 
char str[100];
 
void manipular_entrada(ALLEGRO_EVENT evento);
void exibir_texto_centralizado();
 
bool initAllegro(int xTam, int yTam);
bool carregar_arquivos();
void finalizar();
int telaPorta(int idPorta);

 
 
int telaPorta(int idPorta){
	char passe[50];
    bool sairPorta = false;
    bool concluidoPorta = false;
	char frase1[50], frase2[50];
 
    if (!initAllegro(480, 300))
    {
        return -1;
    }
 
    strcpy(str, "");
 
    if (!carregar_arquivos())
    {
        return -1;
    }
    
    switch(idPorta){
		case 2:
			strcpy(passe, "0159");
			strcpy(frase1, "Tendo 3!+4!+5!+7!, organize a" );
			strcpy(frase2, "soma de modo que vire o menor número");
			break;
		
		case 3:
			strcpy(passe, "70");
			strcpy(frase1, "Tomando o binário: 1110 0010" );
			strcpy(frase2, "quantas permutações podem ser feitas");
			break;
			
		case 4:
			strcpy(passe, "240");
			strcpy(frase1, "Tendo duas chaves circulares com botões" );
			strcpy(frase2, "qual o máximo de combinação, ambas tem 6 posições");
			break;
			
		case 5:
			strcpy(passe, "56");
			strcpy(frase1, "Quantos grupos podem ser formados" );
			strcpy(frase2, "com 3 smartphone de uma caixa de 8");
			break;
			
		case 6:
			strcpy(passe, "10");
			strcpy(frase1, "Tendo 10 números onde " );
			strcpy(frase2, "é tomado 2 em 2 ||  m = 4 e p = 2");
			break;	
			
		case 7:
			strcpy(passe, "cifra");
			strcpy(frase1, "Tendo a cifra 'djgsb', decifre!" );
			strcpy(frase2, "");
			break;	
			
		case 8:
			strcpy(passe, "decrypt");
			strcpy(frase1, "Tendo 'Nombizd' e sabendo que " );
			strcpy(frase2, "ela também é uma cifra, decifre!");
			break;	
		
	}
    
	printf("PASSE PORTA ID = %d",idPorta);//APAGAR
 
    while (!sairPorta)
    {
        while (!al_is_event_queue_empty(fila_eventos))
        {
            ALLEGRO_EVENT evento;
            al_wait_for_event(fila_eventos, &evento);
 
			
            if (!concluidoPorta)
            {
                manipular_entrada(evento);
                
				if(evento.type == ALLEGRO_EVENT_KEY_DOWN && evento.keyboard.keycode == ALLEGRO_KEY_SPACE){
					 finalizar();
					 return 0;			
				} 
 
 
                if (evento.type == ALLEGRO_EVENT_KEY_DOWN && evento.keyboard.keycode == ALLEGRO_KEY_ENTER)
                {

                    if(strcmp(str, passe) == 0){
						concluidoPorta = true;
						sairPorta = true;	
						setbuf(stdin, NULL);
						
					}					
                    
                }
            }
 
        }
 
        al_draw_bitmap(fundo, 0, 0, 0);
 
        if (!concluidoPorta)
        {
			al_draw_text(fonte, al_map_rgb(255, 255, 255), 220, 10, ALLEGRO_ALIGN_CENTRE, frase1);
            al_draw_text(fonte, al_map_rgb(255, 255, 255), 220, 50, ALLEGRO_ALIGN_CENTRE, frase2);
            al_draw_text(fonte, al_map_rgb(255, 255, 255), 220, 100, ALLEGRO_ALIGN_CENTRE, "DIGITE A SENHA:");
            
        }
 
        exibir_texto_centralizado();
		
        al_flip_display();
    }
 
    finalizar();
 
    if(concluidoPorta) return 1;
	return 0;
}
 
bool initAllegro(int xTam, int yTam){
    if (!al_init())
    {
        fprintf(stderr, "Falha ao inicializar a biblioteca Allegro.\n");
        return false;
    }
 
    if (!al_install_keyboard())
    {
        fprintf(stderr, "Falha ao inicializar teclado.\n");
        return false;
    }
 
    if (!al_init_image_addon())
    {
        fprintf(stderr, "Falha ao inicializar allegro_image.\n");
        return false;
    }
 
    al_init_font_addon();
 
    if (!al_init_ttf_addon())
    {
        fprintf(stderr, "Falha ao inicializar allegro_ttf.\n");
        return false;
    }
 
    screem = al_create_display(xTam, yTam);
    if (!screem)
    {
        fprintf(stderr, "Falha ao criar a screem.\n");
        return false;
    }
 
    fila_eventos = al_create_event_queue();
    if (!fila_eventos)
    {
        fprintf(stderr, "Falha ao criar fila de eventos.\n");
        return false;
    }
 
    al_set_window_title(screem, "Entrada de Texto");
 
    al_register_event_source(fila_eventos, al_get_display_event_source(screem));
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
 
    return true;
}
 
bool carregar_arquivos(){
	
    fundo = al_load_bitmap("imagens/fundo2.jpg");
    if (!fundo)
    {
        fprintf(stderr, "Falha ao carregar imagem de fundo tela de Senha.\n");
        return false;
    }
 
    fonte = al_load_font("fontes/cubic.ttf", 14, 0);
    if (!fonte)
    {
        fprintf(stderr, "Falha ao carregar fonte da tela de Senha.\n");
        return false;
    }
 
    return true;
}
 
void finalizar(){
	
    al_destroy_bitmap(fundo);
    al_destroy_font(fonte);
    al_destroy_event_queue(fila_eventos);
    al_destroy_display(screem);
}
 
void manipular_entrada(ALLEGRO_EVENT evento){
	
    if (evento.type == ALLEGRO_EVENT_KEY_CHAR){
        if (strlen(str) <= 16){
			
            char temp[] = {evento.keyboard.unichar, '\0'};
            
            if (evento.keyboard.unichar == ' ')            
                strcat(str, temp);
            
            else if (evento.keyboard.unichar >= '0' && evento.keyboard.unichar <= '9')            
                strcat(str, temp);
            
            else if (evento.keyboard.unichar >= 'A' && evento.keyboard.unichar <= 'Z')            
                strcat(str, temp);
            
            else if (evento.keyboard.unichar >= 'a' && evento.keyboard.unichar <= 'z')
				strcat(str, temp);
            
        }
 
        if (evento.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && strlen(str) != 0)        
            str[strlen(str) - 1] = '\0';
        
    }
}
 
void exibir_texto_centralizado(){
	
    if (strlen(str) > 0)
        al_draw_text(fonte, al_map_rgb(255, 255, 255), 220, 200, ALLEGRO_ALIGN_CENTRE, str);
    
}
