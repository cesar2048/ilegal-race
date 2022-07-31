#include <windows.h>
#include <SDL.h>
#include <GL/glew.h>
#include "ogl.h"


SDL_Surface	*screen;

SDL_Surface* oglGetScreen() {
	return screen;
}


/**
 * Ajusta los parametros de perspectiva
 */
GLvoid InicializaEscenaGL(GLsizei width, GLsizei height) {

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float fieldOfView = 75.0f;
	// The following code is a fancy bit of math that is eqivilant to calling:
	// gluPerspective( fieldOfView/2.0f, width/height , 0.1f, 255.0f )
	// We do it this way simply to avoid requiring glu.h
	GLfloat zNear = 0.5f;
	GLfloat zFar = 5000.0f;
	GLfloat aspect = float(width) / float(height);
	GLfloat fH = tan(fieldOfView / 360.0 * 3.14159) * zNear;
	GLfloat fW = fH * aspect;
	glFrustum(-fW, fW, -fH, fH, zNear, zFar);
	
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
	return 1;
}



