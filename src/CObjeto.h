#ifndef _COBJETO_H_
#define _COBJETO_H_

#include "Vector.h"
#include "CMaterial.h"

class CEscena;

class CObjeto
{
	public:
		int	nCaras;
		int	nVertices;
		int	nTVertices;
		
		Vertice		*listaVertices;
		Vertice		*listaTVertices;
		Cara		*listaCaras;

		float			fColor[3];
		char			nombre[63];
		int				materialID;
		Vertice			desplazamiento;
		Vertice			centro;
		CEscena			*pEscena;
		CMaterial		*pMaterial;

		CObjeto ();
		CObjeto( const CObjeto &c);
		~CObjeto();
		void	cambiarNombre(const char*);		
		bool	usarMaterial();
		void	renderizar(void);
		float	*planoBase(float,float,bool=false);
		int		isOver(float, float);
		bool	isEncima(float, float, float);
};

#endif
