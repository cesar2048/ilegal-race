#include "CCamara.h"

CCamara::CCamara(CEscena *pEscena) {
	x = y = z = giroV = giroH = 0;
	m_pEscena = pEscena;
}

CCamara::~CCamara(){
}

//--------------------------------------------------------------------------------
// NOMBRE: ver
// DESCRIPCION: Ajusta la vision a los parametros de la camara
//--------------------------------------------------------------------------------

void CCamara::ver(void) {

	glRotatef( giroV,	1.0f,	0.0f,	0.0f);
	glRotatef( giroH,	0.0f,	1.0f,	0.0f);
	
	glScalef(1.0f, 1.0f, -1.0f);
	glTranslatef( -x, - y, -z);
}

