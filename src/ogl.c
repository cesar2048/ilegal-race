#include <SDL/SDL.h>
#include "ogl.h"

SDL_Surface	*screen;

SDL_Surface* oglGetScreen() {
	return screen;
}


/**
 * Ajusta los parametros de perspectiva
 */
GLvoid InicializaEscenaGL(GLsizei ancho, GLsizei alto) {

	glViewport(0,0,ancho,alto);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f,(GLfloat)ancho/(GLfloat)alto,1.0f,4000.0f);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/**
 * EStablece el estado inicial de openGL
 */
int InicializaGL(GLvoid) {
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_TEXTURE_2D);
	glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glCullFace(GL_BACK);
	glEnable(GL_BLEND);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	
	glClearColor(0.8f, 0.8f, 1.0f, 0.6f);
	return true;
}


/**
 * Crea una ventana
 */
bool CreaVentanaGL(char* Titulo, int ancho, int alto, int bits, bool fullscreen) {
	fprintf(stderr, "window %ix%i a %i bits\n", ancho, alto, bits);
	int flags = SDL_OPENGL | SDL_HWSURFACE;
	if ( fullscreen) flags |= SDL_FULLSCREEN;

	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	if (  (screen = SDL_SetVideoMode(ancho, alto, bits, flags)) == 0 ){
		  fprintf( stderr, "Video mode set failed: %s\n", SDL_GetError( ) );
	}
	
	InicializaEscenaGL(ancho, alto);

	if (!InicializaGL()) {
		SDL_Quit();
		fprintf(stderr,"%s() -- Fallo en la inicialización.\n", __FUNCTION__);
		return false;					
	}
	return true;
}


