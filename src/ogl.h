#ifndef _CGL_H_
#define _CGL_H_

// #include <GL/gl.h>
#include <SDL.h>
#include <GL/glew.h>


GLvoid			InicializaEscenaGL		(GLsizei ancho, GLsizei alto);
int				InicializaGL			(GLvoid);
SDL_Surface* 	oglGetScreen();

#endif

