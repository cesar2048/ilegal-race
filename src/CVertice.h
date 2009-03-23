#ifndef _CVERTICE_H_
#define _CVERTICE_H_

#include "CVector.h"
#include <stdio.h>
#include <string.h>

class CVertice
{
	public:
		float		m_fCoords[3];

		inline void PonCoordenadas(float x, float y, float z) {
			m_fCoords[0] = x;
			m_fCoords[1] = y;
			m_fCoords[2] = z;
		};
};


#endif
