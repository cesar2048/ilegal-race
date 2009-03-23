#ifndef __CTEXTURA_H_
#define __CTEXTURA_H_

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <stdio.h>
#include <string.h>

class CTextura {
	unsigned int	ID;
	bool			isAlpha;
	
  	public:
			CTextura	();
			~CTextura	();
	bool	CargarBMP	(char*);
	void 	Usa			();
	void 	Elimina		();
	char	nombre[255];
};

#endif 
