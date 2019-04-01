/******************************************************************************/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/

AhoTTS: A Text-To-Speech system for Basque* and Spanish*,
developed by Aholab Signal Processing Laboratory at the
University of the Basque Country (UPV/EHU). Its acoustic engine is based on
hts_engine' and it uses AhoCoder* as vocoder.
(Read COPYRIGHT_and_LICENSE_code.txt for more details)
--------------------------------------------------------------------------------

Linguistic processing for Basque and Spanish, Vocoder (Ahocoder) and
integration by Aholab UPV/EHU.

*AhoCoder is an HNM-based vocoder for Statistical Synthesizers
http://aholab.ehu.es/ahocoder/

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Copyrights:
	1997-2015  Aholab Signal Processing Laboratory, University of the Basque
	 Country (UPV/EHU)
    *2011-2015 Aholab Signal Processing Laboratory, University of the Basque
	  Country (UPV/EHU)

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Licenses:
	GPL-3.0+
	*GPL-3.0+
	'Modified BSD (Compatible with GNU GPL)

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

GPL-3.0+
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 .
 This package is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 .
 You should have received a copy of the GNU General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>.
 .
 On Debian systems, the complete text of the GNU General
 Public License version 3 can be found in /usr/share/common-licenses/GPL-3.

//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/******************************************************************************/
/**********************************************************/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/*
Copyright: 2005 - TSR/Aholab (DET) ETSII/IT-Bilbao

Nombre fuente................ numexp.cpp
Nombre paquete............... normalizador
Lenguaje fuente.............. C++
Estado....................... desarrollando
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Larraitz

Version  dd/mm/aa  Autor     Proposito de la edicion
-------------------------------------------------------------
0.0.0    19/04/05  Larraitz   koma ondorengo zenbakiak irakurri

======================== Contenido ========================



===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

/**********************************************************/

#include <stdio.h>

#include "eu_t2l.hpp"
#include "symbolexp.h"
#include "choputi.h"




/**********************************************************/

/**********************************************************/

CtI LangEU_TextToList::expnumafterpoint(CtI q)
{

	BOOL numzone[6]={FALSE, FALSE, FALSE, FALSE, FALSE, FALSE};
    BOOL etaPend = FALSE;
	INT len;
	INT i=0;
	INT numceros=0;
	pCHAR temp, input;


	input = ct.getStr(q);
	len = strlen(input);

	while(i<len && input[i]== '0')
	{
		numceros++;
		i++;
	}
	if(i != 0)
	{
		if(i == len)
			numceros--;
	}
	else
	{
		i++;
	}


	q = expnum(q);
	q = ct.del(q);

	ct.setPatternForce(q, (char *)"l");
	//ct.setStatus(q, 1);
	//ct.setTnor(q, UTYPENOR_NUMBER);

	for(i=1; i<= numceros; i++)
	{
		q = ct.insbefore(q, (char *)"zero", FALSE);  // para leer 007
	}

	ct.setPatternForce(q, (char *)"l");
	//ct.setStatus(q, 1);
	//ct.setTnor(q, UTYPENOR_NUMBER);

	return q;

}
/**********************************************************/





