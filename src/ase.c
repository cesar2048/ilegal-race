#include <stdio.h>
#include <string.h>
//#include <utility>
//#include <exception>
#include "ase.h"
#include "Lista.h"
 
//#ifdef DEBUG
//	#define __debugear  fprintf(stdout, "--- call to %s();\n", __FUNCTION__);
//#else
	#define __debugear ;
//#endif

// prototipos
bool		medirProfundidad		(char*, int*);
CObjeto*	CargarObjeto			(FILE*, CEscena*);
CMaterial*	CargarMaterial			(FILE*  );
bool		CargarMalla				(FILE*, CObjeto*);
bool		CargarListaVertices		(FILE*, CObjeto*);
bool		CargarListaCaras		(FILE*, CObjeto*);
bool		CargarListaTVertices	(FILE*, CObjeto*);
bool		CargarListaTCaras		(FILE*, CObjeto*);
bool		CargarListaMateriales	(FILE*, CEscena*);

// globales
int nMaterialIni = 0;


bool medirProfundidad	(char *data, int *num){
	if( !strcmp(data, "{") ) {	(*num)++;	return true;}
	if( !strcmp(data, "}") ) {	(*num)--;	return true;}
	return false;
}

void leerEntreComillas(FILE *hFichero, char *destino){
	int caracter=0, pos=0;
	while( caracter != '\"' ){
		caracter = fgetc(hFichero);
	}
	do{
		caracter = fgetc(hFichero);
		destino[pos] = caracter;
		pos++;
	}while( caracter != '\"' );
	destino[pos-1] = 0;
}

/**
 * Cargar
 * Carga una escena desde un archivo tipo ASE
 *
 * return bool -> true si la carga fue exitosa
 */
bool CargarModeloASE( char *nombreArchivo, CEscena *pEscena) {
	__debugear
	nMaterialIni = listaLargo(pEscena->listaMateriales);
	FILE *f = fopen ( nombreArchivo, "r" );
	char palabra[255];
	
	if( !f ){
		fprintf(stderr, "Archivo no existe");
		return false;
	}
	
	fscanf( f, "%s", palabra);
	if ( strcmp(palabra, "*3DSMAX_ASCIIEXPORT" ) ){
		fprintf(stderr, "Archivo ASE incorrecto\n");
		return false;
	}
	
	while( !feof(f) ){
		fscanf(f, "%s", palabra);

		if ( !strcmp(palabra, "*MATERIAL_LIST" )) 
		{
			pEscena->nMateriales++;
			CargarListaMateriales(f, pEscena);
		}
		else if ( !strcmp(palabra, "*GEOMOBJECT" )) 
		{
			CargarObjeto( f, pEscena);
		}
		memset(palabra, 0, 255);
	}
	
	// creando punteros de referencia en los objetos, a sus materiales
	CObjeto *pObj;
	for(int i=0; i < pEscena->nObjetos; i++) {
		//pObj = &pEscena->listaObjetos->at(i);
		pObj = pEscena->listaObjetos[i];
		pObj->pMaterial = (CMaterial*) listaGet(pEscena->listaMateriales, pObj->materialID);
	}
	
	fclose(f);
	return true;
}

/**
 * CargarObjeto()
 * Carga las definiciones de un GEOMOBJECT
 *
 * return puntero a objeto si la carga fue exitosa, NULL en otro caso
 */
CObjeto *CargarObjeto(FILE *f, CEscena *pEscena){
	__debugear
	
	char palabra[255];
	CObjeto *pObj = pEscena->NuevoObjeto("");
	int profundidad = 0;
	
	fscanf( f, "%s", palabra);
	medirProfundidad(palabra, &profundidad);
	
	while ( !feof(f) && profundidad > 0){
		fscanf( f, "%s", palabra);

		if( medirProfundidad(palabra, &profundidad)) {
			// simbolo capturado
		} 
		// nombre del objeto
		else if( !strcmp(palabra,  "*NODE_NAME") )
		{
			leerEntreComillas(f, palabra);
			pObj->cambiarNombre(palabra);
		}
		// el material asignado al objeto
		else if( !strcmp(palabra, "*MATERIAL_REF") )
		{
			int ID;
			fscanf( f, "%d", &ID);
			pObj->materialID = ID + nMaterialIni;
		}
		// el color del objeto sin textura
		else if( !strcmp(palabra, "*WIREFRAME_COLOR") )
		{
			float *pColor = pObj->fColor;
			fscanf( f, "%f %f %f", pColor, pColor+1, pColor+2);
		}
		// la definicion de la malla
		else if( !strcmp(palabra, "*MESH") )
		{
			CargarMalla(f, pObj);
		}
		else if ( !strcmp(palabra, "*TM_POS") )
		{
			float *coord = pObj->centro.coords;
			fscanf( f, "%f %f %f", coord, coord+2, coord+1);
		}
	}
	return pObj;
}


bool CargarMalla(FILE *f, CObjeto *pObj) {
	__debugear
	
	char palabra[255];
	unsigned int nVertices, nCaras, nTVertices, nTCaras;
	int profundidad=0;
	bool bResult = true;
	
	fscanf(f, "%s", palabra);
	medirProfundidad(palabra, &profundidad);
	
	while ( !feof(f) && profundidad > 0){
		fscanf(f, "%s", palabra);
		
		if( medirProfundidad(palabra, &profundidad) ){
			// simbolo capturado
		}else if ( !strcmp(palabra, "*MESH_NUMVERTEX"))
		{	// numero de VERTICES
			fscanf(f, "%u", &nVertices);
			pObj->nVertices = nVertices;
			pObj->listaVertices = (Vertice*) malloc ( sizeof(Vertice) * nVertices);
			//pObj->nVertices = nVertices;
			//pObj->listaVertices = new std::vector<Vertice>( nVertices );
		}
		else if ( !strcmp(palabra, "*MESH_NUMFACES"))
		{	// numero de CARAS
			fscanf(f, "%u", &nCaras);
			pObj->nCaras = nCaras;
			pObj->listaCaras = (Cara*) malloc ( sizeof(Cara) * nCaras );
			//pObj->listaCaras = new std::vector<Cara> ( nCaras );
		}
		else if ( !strcmp(palabra, "*MESH_NUMTVERTEX"))
		{	// numero de VERTICES de TEXTURA
			fscanf(f, "%u", &nTVertices);
			pObj->nTVertices = nTVertices;
			pObj->listaTVertices = (Vertice*) malloc ( sizeof(Vertice) * nTVertices );
			//pObj->listaTVertices = new std::vector<Vertice> ( nTVertices );
		}
		else if ( !strcmp(palabra, "*MESH_NUMTVFACES"))
		{	// numero de CARAS de TEXTURA
			fscanf(f, "%u", &nTCaras);
		}
		else if ( !strcmp(palabra, "*MESH_VERTEX_LIST"))
		{	// empieza la lista de Vertices
			if( !CargarListaVertices(f, pObj) ){
				bResult = false;
				break;
			}
		}
		else if ( !strcmp(palabra, "*MESH_FACE_LIST"))
		{	// empieza la lista de Caras
			if( !CargarListaCaras(f, pObj)) {
				bResult = false;	
				break;
			}
		}
		else if ( !strcmp(palabra, "*MESH_TVERTLIST"))
		{	// empieza la lista de Vertices de TExtura
			CargarListaTVertices(f, pObj);
		}
		else if ( !strcmp(palabra, "*MESH_TFACELIST"))
		{	// empieza La lista de Caras de textura
			CargarListaTCaras(f, pObj);
		}
	}

	return bResult;
}

/**
 * Carga la lista de vertices de un objeto
 */
bool CargarListaVertices (FILE *f, CObjeto *pObj) {
	__debugear
	
	char palabra[255];
	int profundidad=0, i, total=0;
	float x,y,z;
	
	fscanf(f, "%s", palabra);
	medirProfundidad(palabra, &profundidad);
	
	while( !feof(f) && profundidad > 0) {
		fscanf(f, "%s", palabra);

		if ( medirProfundidad(palabra, &profundidad)){
			// simbolo capturado
		} else if( !strcmp(palabra, "*MESH_VERTEX" ))
		{
			fscanf(f, "%d %f %f %f", &i, &x, &y, &z);
			//pObj->listaVertices->at(i) = nuevoVertice(x, z, y);
			if ( i > pObj->nVertices ) {
				fprintf(stderr, "Error, numero de vertice fuera de rango en %s:%i\n", pObj->nombre, i);
			}else{
				pObj->listaVertices[i] = nuevoVertice(x, z, y);	
			}
			total++;
		}
	}
	return pObj->nVertices == total;
}

/**
 * carga la lista de caras del objeto
 */
bool CargarListaCaras( FILE *f, CObjeto *pObj) {
	__debugear
	
	char palabra[255];
	int profundidad=0, i, a, b, c, total=0;
	
	fscanf(f, "%s", palabra);
	medirProfundidad(palabra, &profundidad);
	
	while( !feof(f) && profundidad > 0) {
		fscanf(f, "%s", palabra);

		if ( medirProfundidad(palabra, &profundidad)) {
			// simbolo capturado
		} else if( !strcmp(palabra, "*MESH_FACE")) {
			fscanf(f, "%d:\tA:\t%d B:\t%d C:\t%d", &i, &a, &b, &c);
			//pObj->listaCaras->at(i) = nuevaCara(a, b, c);
			if ( i > pObj->nCaras) {
				fprintf(stderr, "Error, numero de cara fuera de rango en %s:%i\n", pObj->nombre, i);
			}else{
				pObj->listaCaras[i] = nuevaCara(a, b, c);
			}
			total++;			
		}
	}
	return pObj->nCaras == total;
}

/**
 * Carga la lista de vertices sobre la textura
 */
bool CargarListaTVertices(FILE *f, CObjeto *pObj) {
	__debugear
	
	char palabra[255];
	int profundidad=0, i, total=0;
	float x,y,z;
	
	fscanf(f, "%s", palabra);
	medirProfundidad(palabra, &profundidad);
	
	while ( !feof(f) && profundidad > 0) {
		fscanf(f, "%s", palabra);
		
		if ( medirProfundidad(palabra, &profundidad)) {
			// simbolo capturado
		}else if( !strcmp( palabra, "*MESH_TVERT")) {
			fscanf(f, "%d %f %f %f", &i, &x, &y, &z);
			//pObj->listaTVertices->at(i) = nuevoVertice(x,y,z);
			if ( i > pObj->nTVertices) {
				fprintf(stderr, "Error, numero de tVertice fuera de rango en %s:%i\n", pObj->nombre, i);
			}else{
				pObj->listaTVertices[i] = nuevoVertice(x,y,z);
			}
			total++;
		}
	}
	return total == pObj->nTVertices;
}

/** 
 * Carga la lista de caras de textura
 */
bool CargarListaTCaras(FILE *f, CObjeto *pObj){
	__debugear
	
	char palabra[255];
	int profundidad=0, i, total=0, a,b,c;
	
	fscanf(f, "%s", palabra);
	medirProfundidad( palabra, &profundidad);
	
	while( !feof(f) && profundidad > 0){
		fscanf(f, "%s", palabra);
		
		if( medirProfundidad(palabra, &profundidad)) {
			// simbolo capturado
		}else if( !strcmp(palabra, "*MESH_TFACE")) {
			fscanf(f, "%d %d %d %d", &i, &a, &b, &c);
			//ponerTVerticesCara( &pObj->listaCaras->at(i), a, b, c);
			ponerTVerticesCara( pObj->listaCaras + i, a, b, c);
			total++;
		}
	}
	return true;
}

/**
 * Carga lista de materiales
 */
bool CargarListaMateriales(FILE *f, CEscena *pEscena){
	__debugear
	
	char palabra[255];
	int profundidad=0, nNumMateriales, i;
	
	fscanf(f, "%s", palabra);
	medirProfundidad(palabra, &profundidad);
	
	while ( !feof(f) && profundidad > 0) {
		fscanf(f, "%s", palabra);
		
		if( medirProfundidad(palabra, &profundidad)) {
			// simbolo capturado
		}else if( !strcmp(palabra, "*MATERIAL_COUNT")) {
			fscanf(f, "%i", &nNumMateriales);
			fprintf(stderr, "actualizado numMateriales = %i\n", nNumMateriales);
			pEscena->nMateriales = nNumMateriales;
		}else if( !strcmp(palabra, "*MATERIAL") ){
			fscanf(f, "%i", &i);
			CMaterial *pMaterial = CargarMaterial(f);
			listaAgregarElemento(pEscena->listaMateriales, pMaterial);
		}
	}
	return true;
}

CMaterial *CargarMaterial(FILE *f) {
	__debugear
	
	char palabra[255];
	int profundidad=0;
	CMaterial 	*pMaterial = new CMaterial() ;
	
	fscanf(f, "%s", palabra);
	medirProfundidad(palabra, &profundidad);
	
	while ( !feof(f) && profundidad > 0) {
		fscanf(f, "%s", palabra);
		
		if( medirProfundidad( palabra, &profundidad)) {
			// simbolo capturado
		}else if( !strcmp(palabra, "*MATERIAL_NAME")) {
			leerEntreComillas(f, palabra);
			memcpy( pMaterial->nombre, palabra, strlen(palabra) );
		}else if( !strcmp(palabra, "*BITMAP")) {
			leerEntreComillas(f, palabra);
			pMaterial->pTextura = new CTextura();
			pMaterial->pTextura->CargarBMP(palabra);
			fprintf(stdout, "cargada textura: %s\n", palabra);
		}else if ( !strcmp(palabra, "*MATERIAL_DIFFUSE")) {
			float *diffuse = pMaterial->fDifuso;
			fscanf(f, "%f %f %f", diffuse, diffuse+1, diffuse+2);
		}
	}
	return pMaterial;
}
