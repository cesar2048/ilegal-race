#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/gl.h>

#include "CEscena.h"
#include "CObjeto.h"
#include "CCamara.h"
#include "glfont.h"

#ifndef __JUEGO_H
#define __JUEGO_H 1

#define ACEL   1.1
#define FRENO -1.2
#define RETRO -0.5
#define DACEL -0.3
#define BRAKE -0.56

#define MAX	   60
#define MIN    -20

typedef struct Carro {
	CObjeto *pObj;
	
	// estado del carro
	float zTop, zBottom, xLeft, xRight;
	float x,y,z;
	float giroH, giroV, velocidad;
	
	// modelo de mapa, sobre que segmento se halla el carro
	int index;
	
	// estado del carro en la carrera
	int lap, yaGano;
	
	// estados de entrada
	int acelera, brake;
	float gira;
} Carro;

#define TIPO_TIEMPO		0
#define TIPO_CIRCUITO	1
typedef struct Mapa {
	CObjeto *Pista, *Colision, *Lado;
	int nParts;
	char fileName[255];
	int tipoCarrera;
	Uint32 timeStart;
	Uint32 timeElapsed;
	int day;
	int dificultad;
	union {
		struct {
			int maxTime;// miliseconds
			int totalLaps;
		} timeData;
		
		struct {
			int iLeadCar;
			int totalLaps;
		} circuitData;
		
	} datos;
		char carro[255];
} Mapa;


void startMask();
void endMask();
void Controles(Carro car, Mapa m);
void guiInitialize(CEscena *pEscena, int alto, int ancho);
void drawText(char *txt, int x, int y);
void drawText(char *txt, int x, int y, int size);


#endif
