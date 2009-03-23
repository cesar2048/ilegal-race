#ifndef _CGL_H_
#define _CGL_H_

#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>


GLvoid			InicializaEscenaGL		(GLsizei ancho, GLsizei alto);
int				InicializaGL			(GLvoid);
bool			CreaVentanaGL			(char* szTitulo, int nAncho, int nAlto, int nBits, bool bPantallaCompleta);
SDL_Surface* 	oglGetScreen();

#endif

