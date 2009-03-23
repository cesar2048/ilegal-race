#ifndef _CMATERIAL_H_
#define _CMATERIAL_H_

#include "CTextura.h"

class CMaterial {
	
	public:
		float 		fDifuso[4];
		char		nombre[32];
		CTextura	*pTextura;
		//bool		bitmap;

		bool seleccionar() {
			if(pTextura) {
				// usando la textura bitmap
				glColor3f(1.0f, 1.0f, 1.0f);
				pTextura->Usa();
				return true;
			}
			// usando color definido en material
			glBindTexture(GL_TEXTURE_2D, 0);
			glColor3fv(fDifuso);
			return false;
		}
		
		CMaterial(){
			fprintf(stdout, "Material construido\n");
			pTextura=NULL;
		}
		
		~CMaterial() {
            if(pTextura) delete pTextura;
			fprintf(stdout, "Material destruido\n");
		}
};

#endif
