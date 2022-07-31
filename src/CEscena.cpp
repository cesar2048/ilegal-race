#include "CEscena.h"

CEscena::CEscena() {
	listaMateriales = listaNueva( sizeof(CMaterial*));
	listaObjetos = NULL;
	nMateriales = nObjetos = 0;
}

CEscena::~CEscena() {
	for( int i=0; i < nObjetos; i++) {
		delete listaObjetos[i];
	}
	free(listaObjetos);
	listaLiberar(listaMateriales);
	listaMateriales = NULL;
}

CObjeto *CEscena::NuevoObjeto(const char *nombre) {
	CObjeto *pNuevoObjeto = new CObjeto();
			pNuevoObjeto->pEscena = this;
			pNuevoObjeto->cambiarNombre(nombre);
	
	if ( listaObjetos == NULL ){
		listaObjetos = ( CObjeto**) malloc ( sizeof(CObjeto*) );
	}else{
		listaObjetos = (CObjeto**) realloc ( listaObjetos, sizeof(CObjeto*) * (nObjetos+1) );
	}
	listaObjetos[nObjetos] = pNuevoObjeto;
	return listaObjetos[nObjetos++];
}

CObjeto *CEscena::getObjectByName(const char *nombre) {
	int i=0;
	for(; i < nObjetos; i++ ) {
		if ( !strcmp(nombre, listaObjetos[i]->nombre ) ) {
			return listaObjetos[i];
		}
	}
	return NULL;
}
