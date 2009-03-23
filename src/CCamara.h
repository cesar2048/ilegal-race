#ifndef _CCAMARA_H
#define _CCAMARA_H

#include "CEscena.h"
#include <math.h>

#define MATH_PI 3.14159286
#define X 0
#define Y 1
#define Z 2

class CCamara {
	
	CEscena *m_pEscena;
		
public:
	
	float x,y,z, giroH, giroV;
	
	CCamara(CEscena *pEscena);
	~CCamara();
	void ver(void);
};


#endif
