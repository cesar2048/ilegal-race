#include "Menu.h"

// variables globales

int cont = 0;
int cont2 = 0;
int pos = 0;
int rad = 0;
int intro = 0;
Mix_Music *cancion;
Mix_Music *cancion_juego;
Mix_Music *est1, *est2, *est3, *est4, *est5, *est6, *est7, *est8, *est9, *efe1, *efe2;
Mix_Chunk *motor, *velbaja, *velalta;
SDL_Surface *pantalla1, *pantalla2, *pantalla3, *pantalla4, *pantalla5, *pantalla6;

int aux_n = 50;
SDL_Event event;
int volumen = 50;

static SDL_Surface *screen;

typedef struct Boton {
	SDL_Surface *img;
	char info[255];
	int w, h, x, y;
	int accion;
	bool seleccionado;
} Boton;

typedef struct Menu {
	SDL_Surface *background;
	Boton *botones;
	int nBotones;
}Menu;

Menu m;

void destruirMenu() {

    SDL_FreeSurface(pantalla1);
    SDL_FreeSurface(pantalla2);
    SDL_FreeSurface(pantalla3);
    SDL_FreeSurface(pantalla4);
    SDL_FreeSurface(pantalla5);
    SDL_FreeSurface(pantalla6);   
    
    Mix_FreeMusic(est1);
    Mix_FreeMusic(est2);
    Mix_FreeMusic(est3);
    Mix_FreeMusic(est4);
    Mix_FreeMusic(est5);
    Mix_FreeMusic(est6);
    Mix_FreeMusic(est7);
    Mix_FreeMusic(est8);
    Mix_FreeMusic(cancion);
    Mix_FreeMusic(cancion_juego);

    /*for(int i=0; i<m.nBotones; i++) {
		Boton *bt = &m.botones[i];
		SDL_FreeSurface( bt->img );
		free(bt);
	}*/
}

Menu menuInicializar() {
	FILE *f = fopen("menu/main.mnu", "r");
	char word[255];
	int nButtons, iButton=0;
	Menu m;
	
	// cargando el menu
    pantalla1 = IMG_Load("imagenes/intro/c.png");
    pantalla2 = IMG_Load("imagenes/intro/ginc.png");
    pantalla3 = IMG_Load("imagenes/intro/super.png");
    pantalla4 = IMG_Load("imagenes/intro/crea.bmp");
    pantalla5 = IMG_Load("imagenes/intro/intro.png");
    pantalla6 = IMG_Load("imagenes/intro/exit.png");
    
	while ( !feof(f)) {
		fscanf(f, "%s", word);
		if ( !strcmp(word, "BUTTONS_COUNT")) {
			fscanf(f, "%i %s", &nButtons, word);
			m.nBotones = nButtons;
			m.botones = (Boton*) malloc ( sizeof(Boton) * nButtons );
			m.background = IMG_Load(word);
			m.background = IMG_Load(word);
		    if (m.background == NULL) {
                printf("Error en IMG_Load= %s\n", SDL_GetError());
	        }
		} else if ( !strcmp(word, "BUTTON")) {
			int x, y, accion;
			char *info = m.botones[iButton].info;
			fscanf(f, "%i %i %i %s %s", &x, &y, &accion, info, word);
			m.botones[iButton].x = x;
			m.botones[iButton].y = y;
			m.botones[iButton].accion = accion;
			m.botones[iButton].seleccionado = false;
			m.botones[iButton].img = IMG_Load(word);
		    if (m.botones[iButton].img == NULL) {
                printf("Error en IMG_Load= %s\n", SDL_GetError());
	        }
	        iButton++;
		}
	}
	
	// cargando musica
    cancion = Mix_LoadMUS("sonidos/fondo.ogg");
    cancion_juego = Mix_LoadMUS("sonidos/carrera.ogg");

    motor = Mix_LoadWAV ( "sonidos/run.wav" );
	Mix_VolumeChunk(motor, 100);           

    velbaja = Mix_LoadWAV ( "sonidos/velocida_baja.wav" );
	Mix_VolumeChunk(velbaja, 10);           

    velalta = Mix_LoadWAV ( "sonidos/velocida_alta.wav" );
	Mix_VolumeChunk(velalta, 10);       
	cancion = Mix_LoadMUS("sonidos/carrera.ogg");
    
    est1 = Mix_LoadMUS("sonidos/radio4.ogg");
    est2 = Mix_LoadMUS("sonidos/radio3.ogg");        
    est3 = Mix_LoadMUS("sonidos/radio2.ogg");        
    est4 = Mix_LoadMUS("sonidos/radio1.ogg");        
    est5 = Mix_LoadMUS("sonidos/radio10.ogg");        
    est6 = Mix_LoadMUS("sonidos/radio9.ogg");       
    est7 = Mix_LoadMUS("sonidos/radio6.ogg");                
    est8 = Mix_LoadMUS("sonidos/radio5.ogg");
    
    
    if (!cancion)		   printf("Error en Mix_LoadMUS: %s\n", Mix_GetError());
    if (!cancion_juego)    printf("Error en Mix_LoadMUS: %s\n", Mix_GetError());
	
	if(f) fclose(f);
	return m;
}


int menu(Mapa *mapa, int nAncho, int nAlto, bool fullscreen) {
        SDL_Event event;
        SDL_Surface *selected;
        int selectedOne = 0;
        int selectedOne2 = 0;        

        int done = 0, returnValue = SALIR;
        m = menuInicializar();
	
		// datos estaticos
		strcpy( mapa->fileName, "carrerav2.ase");
		mapa->tipoCarrera = TIPO_TIEMPO;
		
		int flags = SDL_HWSURFACE;
		if(fullscreen)
			flags |= SDL_FULLSCREEN;
		screen = SDL_SetVideoMode(nAncho, nAlto, 16, flags);
	    if (screen == NULL) {
	            printf("Error: %s\n", SDL_GetError());
	            return 1;
	    }
	
        if (cancion) {
            if (Mix_PlayMusic(cancion, -1) == -1)   printf("Error en Mix_PlayMusic: %s\n", Mix_GetError());
        }
        /*yo*/
        if (intro == 0){
				imprimir( screen,pantalla1, 0, 0);
				 SDL_Flip(screen);
                SDL_Delay(1000); 
                intro = 1;
                }
             if (intro == 1){
                                       SDL_Delay(1000); 
				imprimir( screen,pantalla2, 0, 0);
				 SDL_Flip(screen);
                SDL_Delay(1000); 
                intro = 2;
     }
              if (intro == 2){
                                       SDL_Delay(1000); 
				imprimir( screen,pantalla3, 0, 0);
				 SDL_Flip(screen);
                SDL_Delay(1000); 
                intro = 3;
     }
              if (intro == 3){
                                       SDL_Delay(1000); 
				imprimir( screen,pantalla4, 0, 0);
				 SDL_Flip(screen);
                SDL_Delay(1000); 
                intro = 4;
     }
              if (intro == 4){
                                       SDL_Delay(1000); 
				imprimir( screen,pantalla5, 0, 0);
				 SDL_Flip(screen);
                SDL_Delay(1000); 
                intro = 5;
     }
	/*end_yo*/
		/** CICLO PRINCIPAL **/
		while (!done)  {
                SDL_PollEvent(&event);

                if (event.type == SDL_QUIT) {    
                        done = 1;
				}
				// averiguando si pulsó algun boton                        
                if (event.type == SDL_MOUSEBUTTONDOWN) {
					if (event.button.button == SDL_BUTTON_LEFT) {
						for(int i=0; i<m.nBotones; i++) {
							Boton *bt = &m.botones[i];
							if ( event.button.x >= bt->x && 
								 event.button.x <= bt->x + bt->img->w &&
								 event.button.y >= bt->y && 
								 event.button.y <= bt->y + bt->img->h
							) {
								//printf("Botton de accion: %i pulsado\n", bt->accion );
								switch ( bt->accion ){
									case SALIR:
                                             imprimir( screen,pantalla6, 0, 0);
                                             SDL_Flip(screen);
                                             SDL_Delay(7000);     
										returnValue = SALIR;
										done++;
										break;
									case INICIA_CARRERA:
										returnValue = INICIA_CARRERA;
										if(selectedOne) {
											done++;
										}	
										break;
									case SELECCIONAR_CARRO:
										selectedOne2++;
										strcpy( mapa->carro, bt->info);
										for(int i=0; i<m.nBotones; i++) {
											if ( m.botones[i].accion == SELECCIONAR_CARRO) {
												m.botones[i].seleccionado = false;
											}
										}
										bt->seleccionado = true;
										break;																				
									case SELECCIONAR_MAPA:
										selectedOne++;
										strcpy( mapa->fileName, bt->info);
										for(int i=0; i<m.nBotones; i++) {
											if ( m.botones[i].accion == SELECCIONAR_MAPA) {
												m.botones[i].seleccionado = false;
											}
										}
										bt->seleccionado = true;
										break;
									case SELECCIONAR_DIFIC:
										sscanf( m.botones[i].info, "%i", &mapa->dificultad);
										for(int i=0; i<m.nBotones; i++) {
											if ( m.botones[i].accion == SELECCIONAR_DIFIC) {
												m.botones[i].seleccionado = false;
											}
										}
										bt->seleccionado = true;
										break;
								}
							}
						}
					}
				}

				// redibujando
				limpiar(screen);
				
				imprimir( screen, m.background, 0, 0); 
				
                for(int i=0; i<m.nBotones; i++) {
					Boton *bt = &m.botones[i];
					if ( bt->seleccionado ) {
				        selected = SDL_CreateRGBSurface(SDL_HWSURFACE, bt->img->w + 10, bt->img->h + 10, 32, 0,0,0,0);
				        SDL_FillRect( selected, NULL, 0xff00ff00 );
						imprimir(screen, selected, bt->x - 5, bt->y - 5);
					}
					imprimir(screen, bt->img, bt->x, bt->y);
				}
				
				SDL_Flip(screen);
		}

        if (cancion) {
                Mix_HaltMusic();
                Mix_FreeMusic(cancion);
        }
        
        return returnValue;
}


void imprimir(SDL_Surface *screen, SDL_Surface *ima, int x, int y) {
        SDL_Rect rect = {x, y, 0, 0};
        SDL_BlitSurface(ima, NULL, screen, &rect);
        
}


void limpiar (SDL_Surface *screen)
{
        Uint32 a = SDL_MapRGB(screen->format, 200, 200, 200);
        SDL_FillRect(screen, NULL, a);
}
void startMenuMusic()
{
             Mix_PlayMusic(cancion, -1);
}
void startGameMusic() {
	       
	switch(cont){
           case 0:
             Mix_PlayMusic(est1, -1);         
             break;
           case 1:     
             break;                    
           case 2:             
             Mix_PlayMusic(est3, -1);
             break;             
           case 3:             
             Mix_PlayMusic(est4, -1);
             break;             
           case 4:             
             Mix_PlayMusic(est5, -1);
             break;             
           case 5:                                     
             Mix_PlayMusic(efe2, -1);                                  
             break;             
           case 6:             
             Mix_PlayMusic(est6, -1);
             break;             
           case 7:             
             Mix_PlayMusic(est7, -1);                                                                                
             break;             
           case 8:             
             Mix_PlayMusic(est9, -1);
             break;
		}
   
		cont = cont + 1;
		if (cont == 9)cont = 0;
}

void stopMusic() {
                 Mix_HaltMusic();
}
void startCarMusic() {
                Mix_PlayChannel ( 1, motor, 1);
}

void setRPM(int n) {
                Mix_VolumeChunk(motor, 0);
     if((n>=100)and(n<200))
           {
            volumen = volumen + 5;             
           Mix_VolumeChunk(velbaja, volumen);
           Mix_PlayChannel ( 1,velbaja, 1);
           }
     if(n>=200)
           {
            volumen = volumen + 5;             
           Mix_VolumeChunk(velalta, volumen);
                      Mix_PlayChannel ( 1,velalta, 1);
           }
}
