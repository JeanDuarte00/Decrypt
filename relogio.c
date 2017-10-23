#include "lib.h"


 

ALLEGRO_DISPLAY *janelaRelo = NULL;
ALLEGRO_EVENT_QUEUE *fila_evento = NULL, *fila_contador = NULL;
ALLEGRO_TIMER *timer = NULL, *contador = 0;
extern ALLEGRO_FONT *fonte;
 
int relogio(){
	
	janelaRelo = al_create_display(LARGURA_TELA, ALTURA_TELA);
	fila_evento = al_create_event_queue();
	timer = al_create_timer(0.1);
	fonte = al_load_font("fontes/cubic.ttf", 40, 0);
    
 
	contador = al_create_timer(1.0);
    fila_contador = al_create_event_queue();
    al_register_event_source(fila_evento, al_get_display_event_source(janelaRelo));
    al_register_event_source(fila_evento, al_get_timer_event_source(timer));
    al_register_event_source(fila_contador, al_get_timer_event_source(contador));
 
    
    
    bool sair = false;
    int r = 0, g = 0, b = 0;
    int min = 0, seg = 0;
 
 
    al_start_timer(timer);
    al_start_timer(contador);
 
    while (!sair)
    {
        if (!al_is_event_queue_empty(fila_evento))
        {
            ALLEGRO_EVENT evento;
            al_wait_for_event(fila_evento, &evento);
 
            if (evento.type == ALLEGRO_EVENT_TIMER)
            {
                if (++r >= 255) r = 0;
                if (++g >= 255) g = 0;
                if (++b >= 255) b = 0;
            }
            else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                sair = true;
            }
        }
 
        if (!al_is_event_queue_empty(fila_contador))
        {
            ALLEGRO_EVENT evento;
            al_wait_for_event(fila_contador, &evento);
 
            if (evento.type == ALLEGRO_EVENT_TIMER)
            {
                seg++;
                if (seg == 60)
                {
                    min++;
                    seg = 0;
                }
            }
        }
 
        al_clear_to_color(al_map_rgb(r, g, b));
        al_draw_textf(fonte, al_map_rgb(255, 0, 0), LARGURA_TELA / 2, 200, ALLEGRO_ALIGN_CENTRE, "%d:%d", min, seg);
        al_flip_display();
    }
 
    al_destroy_timer(timer);
    al_destroy_event_queue(fila_evento);
    al_destroy_display(janelaRelo);
 
    return 0;
}
 
