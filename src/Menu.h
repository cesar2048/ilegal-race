
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "Juego.h"


void destruirMenu();
void limpiar (SDL_Surface *screen);
void imprimir(SDL_Surface *screen, SDL_Surface *ima, int x, int y);
void mover_objeto(int *x, int *y, int ancho, int alto);
void stopMusic();
void startGameMusic();
void startMenuMusic();
void startCarMusic();
void setRPM(int n);
int menu(Mapa *mapa, int nAncho, int nAlto, bool fullscreen);

#define SALIR 				0
#define SELECCIONAR_MAPA	1
#define SELECCIONAR_CARRO	2
#define INICIA_CARRERA		3
#define SELECCIONAR_DIFIC	4
