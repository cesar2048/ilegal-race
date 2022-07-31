#ifndef _CESCENA_H_
#define _CESCENA_H_

#include <string.h>
#include "CObjeto.h"
#include "CMaterial.h"
#include "Lista.h"


class CEscena {
	public:
		int			nMateriales;
		int			nObjetos;
		lista_t		*listaMateriales;
		CObjeto		**listaObjetos;
		
		CEscena();
		~CEscena();
		CObjeto	*NuevoObjeto(const char *szNombre);
		CObjeto *getObjectByName(const char *);
};


#endif
