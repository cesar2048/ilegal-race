#include "Vector.h"

Vertice nuevoVertice(float x, float y, float z){
	Vertice vert;
	vert.coords[0] = x;
	vert.coords[1] = y;
	vert.coords[2] = z;
	return vert;
}

Cara nuevaCara(int a, int b, int c){
	Cara cara;
	cara.Vertices[0] = a;
	cara.Vertices[1] = b;
	cara.Vertices[2] = c;
	return cara;
}

void ponerVerticesCara(Cara *cara, int a, int b, int c){
	cara->Vertices[0] = a;
	cara->Vertices[1] = b;
	cara->Vertices[2] = c;
}

void ponerTVerticesCara(Cara *cara, int a, int b, int c){
	cara->TVertices[0] = a;
	cara->TVertices[1] = b;
	cara->TVertices[2] = c;
}
