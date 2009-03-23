#ifndef _CESCENA_H_
#define _CESCENA_H_

#include "CObjeto.h"
#include "CMaterial.h"
#include "Lista.h"
#include "string.h"
//#include <vector>

class CEscena {
	public:
		int			nMateriales;
		int			nObjetos;
		lista_t		*listaMateriales;
		CObjeto		**listaObjetos;
		
		CEscena();
		~CEscena();
		CObjeto	*NuevoObjeto(char *szNombre);
		CObjeto *getObjectByName(char *);
};


#endif
