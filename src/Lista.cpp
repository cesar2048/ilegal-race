#include "Lista.h"
#include "CMaterial.h"

/**
 * Crea una lista nueva
 */
lista_t *listaNueva(int elementSize) {
	lista_t *lst = (lista_t*) malloc( sizeof(lista_t));
	lst->elemSize=elementSize;
	lst->size = 0;
	lst->head = NULL;
	lst->tail = NULL;
	return lst;
}

/**
 * agrega un elemento al principio de la lista
 * devuleve el nuevo comienzo de la lista
 */
void listaAnteponerElemento(lista_t *lst, void *dato) {
	if (!lst) {
		fprintf(stderr, "[ERROR], se recibio una lista nula en %s:%i -- %s()\n", __FILE__, __LINE__, __FUNCTION__);
		return;
	}
	nodo* nuevoNodo = (nodo*) malloc ( sizeof(nodo));
	if ( lst->size ) {
		nuevoNodo->siguiente = lst->head;
		nuevoNodo->siguiente->anterior = nuevoNodo;
		nuevoNodo->dato = dato;
		lst->head = nuevoNodo;
	}else{
		nuevoNodo->siguiente = NULL;
		lst->head = lst->tail = nuevoNodo;
	}
	nuevoNodo->anterior = NULL;
	lst->size++;
}

/**
 * agrega un elemento al final de la lista
 * devuelve la misma lista
 */
void listaAgregarElemento(lista_t *lst, void* dato){
	if (!lst) {
		fprintf(stderr, "[ERROR], se recibio una lista nula en %s:%i -- %s()\n", __FILE__, __LINE__, __FUNCTION__);
		return;
	}
	nodo *aux = lst->head, *nuevoNodo = (nodo*) malloc ( sizeof(nodo));
	//memset(nodo, 0, sizeof(nodo));
	nuevoNodo->siguiente = NULL;
	nuevoNodo->dato = dato;
	lst->size++;
	
	if ( aux == NULL ) {
		lst->head = nuevoNodo;
		return;
	}
	if ( aux->siguiente == NULL ){
		aux->siguiente = nuevoNodo;
		return;
	}
	while ( aux->siguiente->siguiente != NULL) {
		aux = aux->siguiente;
	}
	aux->siguiente->siguiente = nuevoNodo;
}

/**
 * devuelve el dato de la posicion i
 * o NULL si no existe
 */
void *listaGet(lista_t *l, int i) {
	if (!l) {
		return NULL;
	}
	if( l->head == NULL ){
		return NULL;
	}
	nodo *aux = l->head;
	while(i > 0) {
		if ( !aux->siguiente ) return NULL;
		aux = aux->siguiente;
		i--;
	}
	return aux->dato;
}

/**
 * libera todos los nodos a partir del nodo dado
 */
void liberaNodos(nodo *aux) {
	if ( !aux ) return;
	if ( aux->siguiente ) {
		liberaNodos( aux->siguiente);
	}
	if (aux->dato) delete (CMaterial*) aux->dato;//free(aux->dato);
	free(aux);
}

/**
 * Libera la memoria de la lista
 */
void listaLiberar(lista_t *lst) {
	if(!lst) return;
	liberaNodos(lst->head);
	free(lst);
}

/**
 * devuelve el numero de elementos en la lista
 */
int listaLargo(lista_t *l){
	if(!l || !l->head) return 0;
	nodo *aux = l->head;
	
	int i;
	for(i=1; aux->siguiente != NULL; i++, aux = aux->siguiente );
	return i;
}


