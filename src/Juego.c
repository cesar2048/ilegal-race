#include "Juego.h"

CObjeto *tacometer;
GLFONT  guiFont;
void	(*gameType) ();
static int nAlto, nAncho;

void startMask() {
	// HABILITACION DEL BUFFER STENCIL
	glClearStencil(0);
	glClear( GL_STENCIL_BUFFER_BIT );
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glEnable( GL_STENCIL_TEST );
	glStencilOp( GL_KEEP, GL_KEEP, GL_REPLACE );
	glStencilFunc( GL_ALWAYS, 1, 1 ); 
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);	
}

void endMask() {
	// REGRESO AL ORIGEN
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glStencilFunc( GL_EQUAL, 1, 1 ); 
	glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );
}

void beginOrtographic() {
	/** SET TO ORTOGRAPHIC PROJECTION **/
	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, 0);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glLoadIdentity();
		glOrtho(0, nAncho, 0, nAlto, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
}


void endOrtographic() {
		/* RETURN TO PERSPECTIVE PROJECTION */
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void guiInitialize(CEscena *pEscena, int alto, int ancho) {
	tacometer = pEscena->getObjectByName("tacometer");
	GLuint i;
	glGenTextures(1, &i);
	glFontCreate( &guiFont, "calibri.glf", i);
	gameType = NULL;
	nAlto = alto;
	nAncho = ancho;
}


void Controles(Carro car, Mapa m) {
	char msg[255];
	char msg2[255];
	msg[0] = msg2[0] = 0;

	Uint32 now = SDL_GetTicks();
	Uint32 left10 = (m.datos.timeData.maxTime > m.timeElapsed) ? m.datos.timeData.maxTime - m.timeElapsed : 10001 ;

	if ( car.yaGano == 1) {
		sprintf(msg, "Ganaste");
		sprintf(msg2, "");
	}else if ( m.timeStart >= now ) {
		sprintf(msg2, "%i", (int)(m.timeStart - now + 1000)/1000 );
		sprintf(msg, "");
	}else if ( m.timeElapsed < m.datos.timeData.maxTime ) {
		sprintf(msg2, "tiempo: %4.2f", ((float)(m.datos.timeData.maxTime - m.timeElapsed))/1000 );
		sprintf(msg,  "vuelta: %i / %i", car.lap, m.datos.timeData.totalLaps);
	} else {
		left10 = 1;
		sprintf(msg2, "Tiempo fuera");
		sprintf(msg, "");
	}
	
	/** drawing **/
	
	GLfloat mat_emission[]={1.0, 1.0, 1.0, 0.0};
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission);
	
	beginOrtographic();

		glPushMatrix();
			glTranslatef(nAncho - 75, 75, 0);
			
			// tacometro
			tacometer->renderizar();
	
			// aguja
			glColor4f(1.0f, 0.5f, 0.5f, 1.0f);
			glBindTexture(GL_TEXTURE_2D, 0);
			glRotatef( - car.velocidad * (170/50) + 135, 0, 0, 1);
			glBegin(GL_QUADS);
				glVertex2f(-3.0f, 0.0f);
				glVertex2f( 3.0f, 0.0f);
				glVertex2f( 2.5f, 35.0f);
				glVertex2f(-2.5f, 35.0f);
			glEnd();
		glPopMatrix();
		
		//glDisable (GL_BLEND);
		glColor4f(0.0f, 0.7f, 0.0f, 1.0f);
		drawText(msg, 10, nAlto-10, 20);
		if( left10 < 10000) glColor4f(0.9f, 0.0f, 0.0f, 1.0f);
		drawText(msg2, nAncho/2 - strlen(msg2)*6, nAlto - nAlto/4);
		

	
	endOrtographic();
	
	
	GLfloat mat_specular[]={1.0, 1.0, 1.0, 1.0};
	GLfloat mat2_emission[]={0.0, 0.0, 0.0, 0.0};
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat2_emission);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
}


void drawText(char *txt, int x, int y) {
	drawText(txt, x, y, 18);
}

void drawText(char *txt, int x, int y, int size) {
	GLfloat mat_emission[]={1.0, 1.0, 1.0, 0.0};
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission);
	
	beginOrtographic();
	
	glPushMatrix();
		glTranslatef(x, y, 0.0f);
		glScalef(size, size, 1.0f);
		glFontBegin(&guiFont);
			glFontTextOut(txt, 0, 0, 0);
		glFontEnd();
	glPopMatrix();
	
	endOrtographic();
	
	GLfloat mat_specular[]={1.0, 1.0, 1.0, 1.0};
	GLfloat mat2_emission[]={0.0, 0.0, 0.0, 0.0};
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat2_emission);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
}




