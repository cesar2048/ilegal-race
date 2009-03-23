#include <stdlib.h>
#include <string.h>
#include <GL/gl.h>
#include <SDL/SDL_image.h>
#include "CTextura.h"

/**
 * Constructor
 */
CTextura::CTextura() {
	ID=0;
	isAlpha = false;
}

/**
 * Destructor
 */
CTextura::~CTextura() {
	Elimina();
}

//---------------------------------------------------------------
// Nombre: Elimina
// Descripcion: Elimina la textura
//---------------------------------------------------------------
void CTextura::Elimina() {
	if( ID ) {
		glDeleteTextures(1, &ID);
		ID=0;
	}
}

/**
 * Usar la textura
 */
void CTextura::Usa() {
	if(isAlpha) {
		glEnable (GL_BLEND); 
	}else{
		glDisable (GL_BLEND); 
	}
	glBindTexture(GL_TEXTURE_2D, ID);
}

/**
 * Carga un archivo BMP
 */
bool CTextura::CargarBMP(char *file) {
	GLenum format;
	int bpp;
	
	
	// verify file existence
	FILE* f = fopen(file, "r");
	if ( !f ) {
		fprintf(stderr, "[Error] %s():%i %s File doesn't exists '%s'\n",__FILE__,__LINE__,__FUNCTION__,file);
		return false;
	}
	fclose(f);
	
	strcpy( this->nombre, file);
    SDL_Surface* tex = IMG_Load(file); //SDL_LoadBMP(file);	
	SDL_LockSurface( tex );

	// ajustes de la imagen    
	bpp = tex->format->BytesPerPixel;
	format = GL_RGB;
	if ( bpp == 4 ){
		isAlpha = true;
		format = GL_RGBA;
	}
	
	// convert from bgr to rgb
	if ( strstr( file, ".bmp") ) {
	    for(int j=0; j<tex->h; j++){
			GLubyte *b = 0, aux;
			for(int i=0; i < tex->w; i++) {
				b = ((GLubyte*)tex->pixels) + j*tex->w*bpp + i*bpp;
				aux = b[0];
				b[0] = b[2];
				b[2] = aux;
			}
		}
	}
	
	// flipping image
    for(int j=0; j<tex->h/2; j++){
		GLubyte *ba, *bb;
		ba  = ((GLubyte*) tex->pixels) + j*tex->w*bpp;
		bb  = ((GLubyte*) tex->pixels) + tex->w*(tex->h-1)*bpp - j*tex->w*bpp;

		for(int i=0; i < tex->w*bpp; i++) {
			GLubyte aux = *ba;
			*(ba++) = *bb;
			*(bb++) = aux;
		}
	}
	
	// loading image into gl
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexImage2D(GL_TEXTURE_2D, 0, bpp, tex->w, tex->h, 0, format, GL_UNSIGNED_BYTE, tex->pixels);

	SDL_UnlockSurface(tex);
    SDL_FreeSurface(tex);
	
	return true;
}








