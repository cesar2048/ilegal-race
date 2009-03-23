// INCLUDES //////////////////////////////////////
#ifndef _CVECTOR_H_
#define _CVECTOR_H_

//#include "CObjeto.h"

typedef struct Vertice {
	float coords[3];
} Vertice;

typedef struct Cara {
	int Vertices[3];
	int TVertices[3];
} Cara;


Vertice nuevoVertice(float x, float y, float z);
Cara nuevaCara(int a, int b, int c);
void ponerVerticesCara(Cara *cara, int a, int b, int c);
void ponerTVerticesCara(Cara *cara, int a, int b, int c);

#endif

