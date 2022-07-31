#include "CObjeto.h"


/**
 * Constructor
 */
CObjeto::CObjeto () {
	nCaras = nVertices = nTVertices = 0;
	fColor[0]=1.0f;
	fColor[1]=1.0f;
	fColor[2]=1.0f;
	materialID=-1;
	pEscena=NULL;
	pMaterial=NULL;
	nombre[0]='\0';
}



/**
 * Destructor
 */
CObjeto::~CObjeto() {
	free(listaCaras);
	free(listaVertices),
	free(listaTVertices);
}



/**
 * Cambia el nombre de este objeto
 */
void CObjeto::cambiarNombre(const char *nuevoNombre) {
	strcpy( nombre, nuevoNombre);
}



/**
 * Indica a openGL que use la textura que tiene relacionada,
 * o que seleccione la textura default (0, sin textura), y que
 * use su color de material en el canal difuso, o el color de malla
 *
 * Devuelve:  true, si usa textura
 *			  false, si usa color
 */
bool CObjeto::usarMaterial() {
	if ( pMaterial != NULL) {
		return pMaterial->seleccionar();
	}
	// seleccionando color de malla
	glBindTexture(GL_TEXTURE_2D, 0);
	glColor3fv( this->fColor );
	return false;
}



/**
 * Envia a openGL las coordenadas de tu textura para renderizarse
 */
void CObjeto::renderizar() {
	bool t;
	Cara *pCara;
	
	t = usarMaterial();
	for( int j=0; j < nCaras; j++) {
		pCara = listaCaras + j;
		glBegin(GL_TRIANGLES);
		for( int k=0; k<3; k++){
			if (t) glTexCoord2fv( listaTVertices[ pCara->TVertices[k] ].coords );
			glVertex3fv( listaVertices[ pCara->Vertices[k] ].coords  );
		}
		glEnd();
	}
}


/*
								   (())))
								   (()))
  +---------------------------------+))
  |									|)
  +---------------------------------+

*/

/**
 * Halla la E.C. del plano base sobre el que se encuentra
 * el punto (x,z)
 *
 * devuelve un puntero a 3 floats, coeficientes de la E.C 
 *     y = mx + nz + b    devuelve [m,n,b]
 */
#define X 0
#define Z 2
float *CObjeto::planoBase(float x, float z, bool bla) {
	float *vertice1, *vertice2, *vertice3;
	float *devolver = ( float *) malloc ( sizeof(float) * 3);
	Cara *pCara;
	int nCara;
	
	if( devolver == NULL){
		fprintf(stderr, "No se pudo alojar memoria: CCamara.h:%s\n", __LINE__);
	}
	
	if (  (nCara = isOver(x, z)) == -1) {
		return NULL;
	}
	if(nombre[0] == 'C'){
		nCara++;
		nCara--;
	}
	
	pCara = listaCaras + nCara;
	vertice1 = listaVertices[ pCara->Vertices[0] ].coords;
	vertice2 = listaVertices[ pCara->Vertices[1] ].coords;
	vertice3 = listaVertices[ pCara->Vertices[2] ].coords;
	
	if (bla) {
		devolver = 0;
	}
	
	// hallando la ecuacion del plano
	float ab[] = {
		vertice2[0] - vertice1[0],
		vertice2[1] - vertice1[1],
		vertice2[2] - vertice1[2] 
	};
	float ac[] = {
		vertice3[0] - vertice1[0],
		vertice3[1] - vertice1[1],
		vertice3[2] - vertice1[2] 
	};
	float N[] = {
		  ab[1]*ac[2] - ab[2]*ac[1],
		-(ab[0]*ac[2] - ab[2]*ac[0]),
		  ab[0]*ac[1] - ab[1]*ac[0] 
	};
	if( N[1] == 0){
		//fprintf(stderr, "Error Div by Zero, en cara %i, Objeto %s\n", nCara, this->nombre);
		return NULL;
	}
	devolver[0] = - N[0] / N[1];
	devolver[1] = - N[2] / N[1];
	devolver[2] = (vertice1[0]*N[0] + vertice1[1]*N[1] + vertice1[2]*N[2] ) / N[1];
	
	return devolver;
}






int CObjeto::isOver(float x, float z) {
	float *vertice1, *vertice2, *vertice3;
	float m1,m2,m3,b1,b2,b3;
	Cara *pCara;
	//z *= -1;
	
	for( unsigned int j=0; j < nCaras; j++){
		pCara = listaCaras + j;
		vertice1 = listaVertices[ pCara->Vertices[0] ].coords;
		vertice2 = listaVertices[ pCara->Vertices[1] ].coords;
		vertice3 = listaVertices[ pCara->Vertices[2] ].coords;
		
		// linea 1
		m1 = (vertice1[Z] - vertice2[Z]) / (vertice1[X] - vertice2[X]);
		b1 = vertice1[Z] - m1 * vertice1[X];
		// validacion linea1
		if ( vertice1[X] != vertice2[X] ) {
			if( vertice3[Z] < ( m1 * vertice3[X] + b1) ){
				if( z > ( m1 * x + b1) ) continue;
			}else{
				if( z < ( m1 * x + b1) ) continue;
			}
		}else{
			if ( vertice3[X] < vertice2[X] ){
				if ( x > vertice2[X] ) continue;
			}else{
				if ( x < vertice2[X] ) continue;
			}
		}
		
		// linea2
		m2 = (vertice1[Z] - vertice3[Z]) / (vertice1[X] - vertice3[X]);
		b2 = vertice1[Z] - m2 * vertice1[X];
		// validacion linea2
		if ( vertice1[X] != vertice3[X] ) {
			if( vertice2[Z] < ( m2 * vertice2[X] + b2) ){
				if( z > ( m2 * x + b2) ) continue;
			}else{
				if( z < ( m2 * x + b2) ) continue;
			}
		}else{
			if ( vertice2[X] < vertice3[X] ){
				if ( x > vertice3[X] ) continue;
			}else{
				if ( x < vertice3[X] ) continue;
			}
		}

		// lina3
		m3 = (vertice2[Z] - vertice3[Z]) / (vertice2[X] - vertice3[X]);
		b3 = vertice2[Z] - m3 * vertice2[X];
		// validacion linea3
		if( vertice2[X] != vertice3[X] ) {
			if( vertice1[Z] < ( m3 * vertice1[X] + b3) ){
				if( z > ( m3 * x + b3) ) continue;
			}else{
				if( z < ( m3 * x + b3) ) continue;
			}
		}else{
			if ( vertice1[X] < vertice3[X] ){
				if ( x > vertice3[X] ) continue;
			}else{
				if ( x < vertice3[X] ) continue;
			}
		}
		return j;
	}
	return -1;
}

bool CObjeto::isEncima(float x, float z, float y) {
	float *base = planoBase(x, z);
	if ( base) {
		if ( x* base[0] + z * base[1] + base[2]  <  y) {
			return true;
		}
	}
	return false;
}
