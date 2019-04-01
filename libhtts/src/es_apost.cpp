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
Copyright: 1998 - TSR/Aholab (DET) ETSII/IT-Bilbao

Nombre fuente................ es_apost.cpp
Nombre paquete............... normalizador
Lenguaje fuente.............. C++
Estado....................... desarrollo
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Juan Luis Murugarren

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
0.1.0    11/11/02  inigos	 Recopia para castellano
0.0.0    07/05/01  Yon2.     Codificacion inicial.

======================== Contenido ========================
	Modulo para normalizar palabras con ap¢strofes en
su interior.  Ej: t'erdi -> terdi


Yon2.

  En castellano NO debería darse.

inigos
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include "httsmsg.h"
#include "es_t2l.hpp"


/**********************************************************/

BOOL LangES_TextToList::isApost( CtI p )
{
	BOOL retval = FALSE;
	pCHAR pattern;

	CtI q;

	CHAR separator;



#ifdef HTTS_DEBUG

        if (!ct(p).pattern) htts_error("LangES_TextToList::isApost() - Trying to read null Pattern.");
#endif


	if (ct(p).pattern) pattern = strdup(ct(p).pattern);
	else return retval;



	//Buscamos patrones de tipo compuesto validos.
	switch (strlen(pattern))	{
		case 3:
			if ( ( pattern[0] == 'l' ) && ( pattern[1] == 'p' ) &&
				( pattern[2] == 'l' ) ) {
					q=ct.next(p);
					if (strlen(ct(q).str) != 1) break;
					separator = (ct(q).str)[0];
					if (separator == '\'') retval = TRUE;
			}
			break;

			default:
			break;
	}

	free(pattern);

	return retval;


}

/**********************************************************/


/**********************************************************/

CtI LangES_TextToList::expApost( CtI p )
{
	CtI q;
	INT nint = 0 ,i;
	INT patlen;
	INT stat, emotion, emo_intensity;

	stat = ct.getStatus(p);
	emotion = ct.getEmotion(p);
	emo_intensity = ct.getEmo_int(p);

	patlen = strlen(ct(p).pattern);
	switch (patlen)	{
		case 3:
			nint = 1;
			break;
		default:
#ifdef HTTS_DEBUG
			htts_error("LangEU_TextToList::expApost() No valid pattern length detected.");
#endif
			break;
	}

	q=p;
	p=ct.lastInGrp(q);

	for (i = 0; i < nint; i++)	{
		p=ct.insafter(p, ct.getStr(q), FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
		q=ct.next(q);
		q=ct.next(q);
	}
	p=ct.extendStr(p, ct.getStr(q));

	//Trasladamos la frontera y eliminamos sobrantes.

	q=ct.next(q);
	ct.setPatternForce(q,(char*) "l");
	ct.setStatus(q, stat);
	ct.setTnor(q, UTYPENOR_APOST);

	q=ct.prevGrp(q);
	q=ct.delGrp(q);

	return q;

}

/**********************************************************/

