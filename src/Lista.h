#ifndef _LISTA_ENLAZADA_
#define _LISTA_ENLAZADA_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct nodo {
	void*			dato;
	struct nodo *	siguiente;
	struct nodo *	anterior;
} nodo;

typedef struct lista {
	int elemSize, size;
	nodo *head;
	nodo *tail;
	
} lista_t;

//lista_t listaNueva(int elementSize);
lista_t* listaNueva(int elementSize);
void listaAnteponerElemento(lista_t *lst, void *dato);
void listaAgregarElemento(lista_t *lst, void* dato);
void *listaGet(lista_t *l, int i);
void listaLiberar(lista_t *lst);
int listaLargo(lista_t *l);

#endif

