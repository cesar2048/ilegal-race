#include <SDL.h>
#include <gl/glew.h>
#include <map>
#include <math.h>
#include <time.h>
#include <windows.h>

#include "CObjeto.h"
#include "CTextura.h"
#include "CEscena.h"
#include "CCamara.h"
#include "ogl.h"
#include "ase.h"
#include "Juego.h"
#include "glfont.h"
#include "Menu.h"


// GLOBALES
int nAncho = 1080, nAlto = 720;
const char *estacion = "FM 90.1";
int radios = 0;
bool fullScreen = false;
std::map<int, bool> bTeclas;

CEscena	*Escena;
CCamara *miCamara;
Carro carro;
Mapa mapa;
CObjeto *path;
SDL_Window* win;

// PROTOTIPOS
void Renderiza(GLvoid);
void ControlEntrada(SDL_Event);
Carro ControladorDelCarro(Carro,Uint32);
Carro aplicaColision(Carro car, float *planoColision, float Ez, float Ex);
Mapa inicializarMapa(Mapa m);
bool Inicializa();
void Salida();
void Juego();
bool CreaVentanaGL(const char* szTitulo, int nAncho, int nAlto, int nBits, bool bPantallaCompleta);

/**
 *  Funcion M A I N
 */
int main(int argc, char* argv[]) {
	SDL_Event event;

	Inicializa();
	int value = 1;

	while (value) {
		// menu al principio ( y despues de una carrera)
		value = menu(&mapa, 640, 480, fullScreen);

		if (value == INICIA_CARRERA) {
			if (!CreaVentanaGL("Juego", nAncho, nAlto, 16, fullScreen)) {
				fprintf(stderr, "Error al crear ventana para GL\n");
			}
			mapa = inicializarMapa(mapa);
			startGameMusic();
			startCarMusic();
			mapa.timeStart = SDL_GetTicks() + 3001;

			// ciclo de vida de la carrera
			while (true) {
				SDL_PollEvent(&event);
				if (event.key.keysym.sym == SDLK_ESCAPE) break;

				mapa.timeElapsed = SDL_GetTicks() - mapa.timeStart;
				ControlEntrada(event);

				Juego();
				Renderiza();
			}

			// destruir el mapa
			SDL_DestroyWindow(win);
			delete Escena;
		}
	}

	// destruirMenu();
	Salida();
	return 0;
}

/**
 * Logica del juego
 */
void Juego() {
	static Uint32 time1=0, time2;
	Uint32 miliSeconds;
	
	time2 = SDL_GetTicks();
	miliSeconds = time2 - time1;
	time1 = time2;
	
	/** hallando donde del mapa estamos **/
	char objName[255];
	int indexAnt = carro.index;
	CObjeto *objBase;
	for(int i=0; i<mapa.nParts; i++){
		sprintf_s(objName, "Base%i", i);
		objBase = Escena->getObjectByName(objName);
		if(objBase ) {
			if ( objBase->isEncima(carro.x, carro.z, carro.y)) {
				carro.index = i;
				break;
			}
		}
	}

    glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
    drawText((char*) estacion, 50, 50);
	
	/** buscando los objetos de esta parte del mapa **/
	sprintf(objName, "Pista%i", carro.index);
	mapa.Pista = Escena->getObjectByName(objName);
	
	sprintf(objName, "Colision%i", carro.index);
	mapa.Colision = Escena->getObjectByName(objName);
	
	sprintf(objName, "Lado%i", carro.index);
	mapa.Lado = Escena->getObjectByName(objName);
	
	/** Localizacion de la camara **/
	miCamara->giroH = carro.giroH;
	miCamara->x = carro.x - 80 * sin( carro.giroH * (3.141592 / 180));
	miCamara->z = carro.z - 80 * cos( carro.giroH * (3.141592 / 180));
	miCamara->y = carro.y + 30;
	
	if ( indexAnt != carro.index ){
		if ( indexAnt == mapa.nParts - 1 && carro.index == 0 ){
			carro.lap++;
		}
	}
	
	if ( carro.yaGano != 1 && mapa.timeElapsed > mapa.datos.timeData.maxTime ) {
		carro.acelera = 0;
		carro.gira = 0;
		carro.yaGano = -1;
	}else if ( carro.lap == mapa.datos.timeData.totalLaps) {
		carro.acelera = 0;
		carro.gira = 0;
		carro.yaGano = 1;
	}
	
	/** Por cada carro, aplicar fisica **/
	carro = ControladorDelCarro(carro, miliSeconds); /** carro principal **/
}

Carro ControladorDelCarro(Carro car, Uint32 miliSeconds) {
	CObjeto *objCaida = Escena->getObjectByName("Caida");
	bool estaCayendo = (!objCaida) ? false: objCaida->isOver(car.x, car.z) != -1;
	if (estaCayendo) {
		car.y -= 9.8 * miliSeconds / 100;
		car.giroV += 0.5;
		car.acelera = 0;
	}
	
	/** calculo de VELOCIDAD **/
	if	( car.velocidad > 0 ) {
		if 		( car.acelera ==  1) car.velocidad += ((ACEL * (float)miliSeconds) / 100)   ; /* aceleracion */
		else if ( car.acelera == -1) car.velocidad += (FRENO * (float)miliSeconds) / 100; /* frenado 	*/
	} else{
		if 		( car.acelera == -1) car.velocidad += ((RETRO * (float)miliSeconds) / 100)   ; /* aceleracion */
		else if ( car.acelera ==  1) car.velocidad -= (FRENO * (float)miliSeconds) / 100; /* frenado 	*/
	}
	if ( car.acelera == 0) {
		if ( car.velocidad > 0) {
			car.velocidad += (DACEL * (float)miliSeconds) / 100;
			if ( car.velocidad < 0) car.velocidad = 0;
		}
		if ( car.velocidad < 0) {
			car.velocidad -= (DACEL * (float)miliSeconds) / 100;
			if ( car.velocidad > 0) car.velocidad = 0;
		}
	}
	if ( car.brake ){
		if ( car.velocidad > 0) {
			car.velocidad += (BRAKE * (float)miliSeconds) / 100;
			if ( car.velocidad < 0) car.velocidad = 0;
		}
		if ( car.velocidad < 0) {
			car.velocidad -= (BRAKE * (float)miliSeconds) / 100;
			if ( car.velocidad > 0) car.velocidad = 0;
		}		
	}
	if ( car.velocidad > MAX) car.velocidad = MAX;
	if ( car.velocidad < MIN) car.velocidad = MIN;
	
	if ( SDL_GetTicks() < mapa.timeStart ) {
		car.velocidad = 0;
	}
	
	
	
	/* componentes del movimiento, y E.C. del plano sobre el que esta */
	float zComp  = cos( car.giroH * (3.141592 / 180));
	float xComp  = sin( car.giroH * (3.141592 / 180));
	float *base = mapa.Pista->planoBase(car.x, car.z);

	/* coordenadas del frente (xF,zF), y E.C. del plano */	
	float xF = car.x + xComp * car.zTop;
	float zF = car.z + zComp * car.zTop;
	float *base2 = mapa.Pista->planoBase(xF, zF);
	float yF = (base2) ? base2[0] * xF + base2[1] * zF + base2[2] : (car.y > 500 ) ? 0 : car.y;
	
	/* ajustando coordenadas del carro */
	car.x += xComp * car.velocidad;
	car.z += zComp * car.velocidad;
	car.y = (base && !estaCayendo) ? base[0] * car.x + base[1] * car.z + base[2] : car.y;
	
	/* detectando colisiones */
	float Ex = xComp * car.zTop - xComp * car.xLeft + car.x;
	float Ez = zComp * car.zTop + zComp * car.xLeft + car.z;
	float *base3 = mapa.Colision->planoBase(Ex, Ez); // esquina 1
	if ( base3) car = aplicaColision(car, base3, Ez, Ex);
	
	Ex = xComp * car.zTop - xComp * car.xRight + car.x;
	Ez = zComp * car.zTop + zComp * car.xRight + car.z;
	base3 = mapa.Colision->planoBase(Ex, Ez); // esquina 2
	if ( base3) car = aplicaColision(car, base3, Ez, Ex);
	
	Ex = xComp * car.zBottom - xComp * car.xRight + car.x;
	Ez = zComp * car.zBottom + zComp * car.xRight + car.z;
	base3 = mapa.Colision->planoBase(Ex, Ez);// esquina 3
	if ( base3) car = aplicaColision(car, base3, Ez, Ex);
	
	Ex = xComp * car.zBottom - xComp * car.xLeft + car.x;
	Ez = zComp * car.zBottom + zComp * car.xLeft + car.z;
	base3 = mapa.Colision->planoBase(Ex, Ez);// esquina 4
	if ( base3) car = aplicaColision(car, base3, Ez, Ex);

	/**  giro del carro, horizontal y vertical **/
	car.giroV = - atan( (yF - car.y) / car.zTop ) * (180 / 3.141692);
	if ( car.velocidad == 0) {
	}else if ( car.velocidad > 0) {
		car.giroH += (car.gira * 0.4 *(-1/((car.velocidad+2.2)/10) - sqrt(car.velocidad/5) + 5)/2.84)/(2 - car.brake);
	}else{
		car.giroH -= (car.gira * 0.4 *(-1/((-car.velocidad+2.2)/10) - sqrt(-car.velocidad/5) + 5)/2.84)/(2 - car.brake);
	}
	
	return car;
}

Carro aplicaColision(Carro car, float *planoColision, float Ez, float Ex) {
	if(!planoColision) {
		return car;
	}
	if( planoColision[0] > 0.3 || planoColision[0] < -0.3 || planoColision[1] > 0.3 || planoColision[1] < -0.3 ) {
		if(planoColision[1] != 0) {
			// E.C. de la linea recta en interseccion con el plano de colision
			float m = - planoColision[0] / planoColision[1];
			float b = (car.y - planoColision[2]) / planoColision[1];
			float Px = (Ez + Ex/m - b) / (m + 1/m);
			float Pz = m * Px + b;
			
			float cCompare = m * car.x + b;
			float pCompare = m * Px + b;
			
			if ( (cCompare > 0 && pCompare > 0) || (cCompare < 0 && pCompare < 0)  ) {
				car.x += Px - Ex;
				car.z += Pz - Ez;	
			}
		}else{
			if (
				car.z > planoColision[2] && Ez > planoColision[2] ||
				car.z < planoColision[2] && Ez < planoColision[2]
			)
			{
				car.x  += ( car.y - planoColision[2] ) / planoColision[0] - car.x;
			}
		}
		car.velocidad *= 0.75;
	}
	return car;
}


/** 
 * Inicia las librerias, carga los objetos, etc
 */
bool Inicializa() {
	// inicializa video y audio
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == -1) {
            fprintf(stderr, "Error: %s\n", SDL_GetError());
            return 1;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
            fprintf(stderr, "Error en Mix_OpenAudio: %s\n", Mix_GetError());
            return 1;
    }
    
	return true;
}

Carro inicializaCarro(Carro carro) {
	/*** INICIALIZACION DEL CARRO **/
	
	/** Estado inicial del carro */
	carro.index = carro.lap = carro.yaGano = 0;
	carro.gira = carro.acelera = carro.brake = 0;
	carro.giroH = carro.giroV = carro.velocidad = 0;
	carro.x = carro.y = carro.z = 0;
	carro.pObj = Escena->getObjectByName("carro1");
	/** proporciones del carro */
	for(int i=0; i < carro.pObj->nVertices; i++) {
		float *coords = carro.pObj->listaVertices[i].coords;
		
		if ( coords[2] > carro.zTop ) 	carro.zTop 	  = coords[2];
		if ( coords[2] < carro.zBottom) carro.zBottom = coords[2];

		if ( coords[0] > carro.xRight) 	carro.xRight = coords[0];
		if ( coords[0] < carro.xLeft)	carro.xLeft  = coords[0];
	}
	return carro;
}

Mapa inicializarMapa(Mapa mapa) {
	/*** CARGA DE DATOS DEL MAPA **/
	Escena = new CEscena;
	miCamara = new CCamara( Escena );
	
	CargarModeloASE(mapa.fileName, Escena );
	CargarModeloASE(mapa.carro, Escena);
	CargarModeloASE("assets/maps/controles.ASE", Escena);
	
	guiInitialize(Escena, nAlto, nAncho);
	path = Escena->getObjectByName("path");
	
	/** inicializacion del carro **/
	carro = inicializaCarro(carro);
	
	/*** INICIALIZACION DEL MAPA **/
	
	// hallando automaticamente la cantidad de partes del mapa
	// de acuerdo al numero de elementos "Base%i" en la lista
	mapa.nParts = 0;
	mapa.day = 1;
	char objName[32];
	for(int i=0; i < Escena->nObjetos; i++ ) {
		sprintf(objName, "Base%i", i);
		CObjeto *pObj = Escena->getObjectByName(objName);
		if(pObj){
			mapa.nParts = i+1;
		}
	}
	
	char infoFile[255], word[255];
	sprintf(infoFile, "%s.info", mapa.fileName);
	FILE *f = fopen(infoFile, "r");
	if(f) while( !feof(f)) {
		fscanf(f, "%s", word);
		if ( !strcmp( word, "LAP_TIME")) {
			fscanf(f, "%i", &mapa.datos.timeData.maxTime);
		} else if ( !strcmp( word, "LAPS")) {
			fscanf(f, "%i", &mapa.datos.timeData.totalLaps);
		}
	}
	
	if(f) fclose(f);
	// color del cielo
	switch( mapa.dificultad ){
		case 1:
			glClearColor(0.8f, 0.8f, 1.0f, 1.0f);
			glDisable(GL_LIGHTING);
		break;
		case 2:
			glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
			glEnable(GL_LIGHTING);
		break;
		case 3:
			glClearColor(0.8f, 0.8f, 1.0f, 1.0f);
			glDisable(GL_LIGHTING);
			mapa.datos.timeData.maxTime *= 0.75;
		break;
		case 4:
			glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
			glEnable(GL_LIGHTING);
			mapa.datos.timeData.maxTime *= 0.5;
		break;
	}
	
	fprintf(stderr, "Numero de bases hallado: %i\n", mapa.nParts);
	return mapa;
}
void radios_control(){
	switch(radios){
           case 0: estacion = "fm 91.1";      	radios++;break;
           case 1: estacion = "fm 93.3";      	radios++;break;
           case 2: estacion = "fm 94.5";      	radios++;break;
           case 3: estacion = "fm 96.7";      	radios++;break;
           case 4: estacion = "fm 97.3";      	radios++;break;
           case 5: estacion = "fm 98.3";      	radios++;break;
           case 6: estacion = "fm 99.0";      	radios++;break;
           case 7: estacion = "fm 101.1";      	radios++;break;
           case 8: estacion = "fm 105.3";      	radios++;break;                                                                                        
    	   default:         radios = 0;
}}

/**
 * Controla la entrada del teclado
 * ajusta los parametros de control del carro de acuerdo a la entrada
 */
void ControlEntrada(SDL_Event ev) {
	switch (ev.type) {
	case SDL_KEYDOWN:
		bTeclas[ev.key.keysym.sym] = true;
		break;
	case SDL_KEYUP:
		bTeclas[ev.key.keysym.sym] = false;
	}

	if (bTeclas[SDLK_LEFT] && !bTeclas[SDLK_RIGHT]) {
		carro.gira -= 0.5;
		if (carro.gira < -5) carro.gira = -5;
	} else if (bTeclas[SDLK_RIGHT] && !bTeclas[SDLK_LEFT]) {
		carro.gira += 0.5;
		if (carro.gira > 5) carro.gira = 5;
	} else {
		if (carro.gira > 0) carro.gira -= 0.5;
		else if (carro.gira < 0) carro.gira += 0.5;
	}

	if (bTeclas[SDLK_UP] && !bTeclas[SDLK_DOWN]) {
		carro.acelera = 1;
	} else if (!bTeclas[SDLK_UP] && bTeclas[SDLK_DOWN]) {
		carro.acelera = -1;
	} else {
		carro.acelera = 0;
	}

	if (bTeclas[SDLK_SPACE]) {
		carro.brake = 1;
	} else {
		carro.brake = 0;
	}

	if (bTeclas[SDLK_a]) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	if (bTeclas[SDLK_s]) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (bTeclas[SDLK_r]) {
		bTeclas[SDLK_r] = false;
		startGameMusic();
		radios_control();
	}
};


/**
 * Motor grafico
 */
void Renderiza(GLvoid) {
	// swaping
	SDL_GL_SwapWindow(win);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
    CObjeto *pObj;

	GLfloat specularlight[] = { 1.0f ,1.0f ,1.0f ,1.0f};
	GLfloat  ambientlight[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat  diffuselight[] = { 0.7f, 0.7f, 0.7f, 1.0f };

	glLightfv(GL_LIGHT0,GL_AMBIENT,ambientlight);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuselight);
	glLightfv(GL_LIGHT0,GL_SPECULAR,specularlight);
	glLighti(GL_LIGHT0, GL_SPOT_CUTOFF, 45);
	
	GLfloat	lightPos[] = { 0.0f,  0.0f, 0.0f, 1.0f };
	GLfloat lightDir[] = { 0.0f , 0.0f, 30.0f};
	
	glPushMatrix();
		miCamara->ver();
		
		// dibujando el carro con su transformacion especial (translates, rotates)
		glPushMatrix();
			glTranslatef( carro.x, carro.y, carro.z);
			glRotatef( carro.giroH, 0, 1, 0);
			glRotatef( carro.giroV, 1, 0, 0);
			
			glLightfv(GL_LIGHT0, GL_POSITION,		lightPos);
			glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightDir);		
			
			carro.pObj->renderizar();
		glPopMatrix();
		
		
		CObjeto *super = Escena->getObjectByName("Super0");
		if(super)  super->renderizar();
		
		// dibuja objetos del segmento principal
		if( mapa.Colision ) mapa.Colision->renderizar();
		if( mapa.Pista    ) mapa.Pista->renderizar();
		if( mapa.Lado     ) mapa.Lado->renderizar();
	
		// dibuja objetos del segmento anterior y siguiente	
		char objName[255];
		const char *names[] = {"Colision%i", "Lado%i", "Pista%i"};
		int nNext = ( carro.index + mapa.nParts + 1 ) % mapa.nParts;
		int nPrev = ( carro.index + mapa.nParts - 1) % mapa.nParts;
		for(int i=0; i<3; i++){
			sprintf(objName, names[i], nNext);
			pObj = Escena->getObjectByName(objName);
			if(pObj) pObj->renderizar();
			
			sprintf(objName, names[i], nPrev);
			pObj = Escena->getObjectByName(objName);
			if(pObj) pObj->renderizar();
		}
		
	glPopMatrix();
	
	// dibujando los controles
	Controles(carro, mapa);


}


/**
 * Crea una ventana
 */
bool CreaVentanaGL(const char* Titulo, int ancho, int alto, int bits, bool fullscreen) {
	// fprintf(stderr, "window %ix%i a %i bits\n", ancho, alto, bits);
	int flags = SDL_WINDOW_BORDERLESS | SDL_WINDOW_OPENGL;
	if (fullscreen) flags |= SDL_WINDOW_FULLSCREEN;

	win = SDL_CreateWindow("Ilegal race", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ancho, alto, flags);
	SDL_GLContext glContext = SDL_GL_CreateContext(win);

	GLenum error = glewInit();
	if (error != GLEW_OK) {
		char* msg = (char*)glewGetErrorString(error);
		MessageBoxA(NULL, msg, "Error caption", MB_ICONERROR);
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	InicializaEscenaGL(ancho, alto);

	if (!InicializaGL()) {
		SDL_Quit();
		// fprintf(stderr,"%s() -- Fallo en la inicialización.\n", __FUNCTION__);
		return 0;
	}
	return 1;
}



/**
 * Libera objetos
 * deinicializa las librerias
 */
void Salida() {
    Mix_CloseAudio();
	SDL_Quit();
}
