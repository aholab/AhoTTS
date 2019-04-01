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

Nombre fuente................ comp.cpp
Nombre paquete............... normalizador
Lenguaje fuente.............. C++
Estado....................... desarrollo
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Juan Luis Murugarren

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
0.0.4    21/10/11  Inaki     la funcion normal de eu_normal.cpp se encargara de hacerlas pronunciables (se pierde la marca UTYPENOR_COMP, aunque no la usamos para nada)
			     para mantenerla se puede poner una excepción en la función isNorm() para que no considere como normalizada la palabra que contenga dicha marca
0.0.4    04/05/01  Yon2.     Right Warning/Error info.
0.0.3    25/04/01  Yon2.     Multiling support.
0.0.2    08/04/01  Yon2.     Avoid Warnings.
0.0.1    31/03/01  Yon2.     If a word in comp group is non pron -> spell it.
0.0.0    22/03/01  Yon2.     Codificacion inicial.

======================== Contenido ========================
  Normaliza Grupos de tipo compuesto, es decir, con la
estructura:
		lpl
		lplpl
  Letras, un signo de puntuacion, que debe ser un guion,
y nuevamente letras, y como mucho se repite otra vez esa estructura.
	El resultado de la normalización son dos o tres grupos nuevos,
cada uno con una palabra, y con un tipo TNOR compuesto.


Yon2.
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include "httsmsg.h"
#include "eu_t2l.hpp"


/**********************************************************/

CtI LangEU_TextToList::expComp( CtI p )
{
	CtI q;
	INT nint = 0 ,i;
	INT patlen;
	INT stat, emotion, emo_intensity;

	stat = ct.getStatus(p);
	emotion = ct.getEmotion(p);
	emo_intensity = ct.getEmo_int(p);
	pCHAR word1 = NULL;
        pCHAR word2 = NULL;

	patlen = strlen(ct(p).pattern);
	switch (patlen)	
	{
		case 3:
			///////////////////////////////////////
			//INAKI
			nint = 1;
			// Copiamos las dos partes
                        word1 = strdup(ct(p).str);
                        q=ct.next(p); //-
                        q=ct.next(q); //2.hitza
                        word2 = strdup(ct(q).str);
				
			q=ct.del(q);
                        q=ct.prev(q);
			if(q == NULL){
				q=ct.insafter(ct.next(p) , word2, FALSE, emotion, emo_intensity);
				break;
			}
			q=ct.del(q);
                        q=ct.prev(q);
			q=ct.del(q);
                        q=ct.insbefore(q, word2, FALSE, emotion, emo_intensity);
                        // Patrón, estado y tipo de normalización de la palabra (al número se lo ponen)
                        ct.setPatternForce(q, (char *)"l");
                        ct.setStatus(q, stat);

	                q=ct.insbefore(q, word1, FALSE, emotion, emo_intensity);
                     
                        // Patrón, estado y tipo de normalización de la palabra (al número se lo ponen)
                        ct.setPatternForce(q, (char *)"l");
                        ct.setStatus(q, stat);
                       // ct.setTnor(q, UTYPENOR_COMP);

                        free(word1);
                        free(word2);
			return q;
////////////////////////////////////////////////////////////
			break;
		case 5:
			nint = 2;
			break;
		default:
#ifdef HTTS_DEBUG
			htts_error("LangEU_TextToList::expComp() No valid pattern length detected.");
#endif
			break;
	}


		
	q=p;
	p=ct.lastInGrp(q);

	for (i = 0; i < nint; i++)	{
		if (isPronun(q))  p=ct.insafter(p, ct.getStr(q), FALSE, emotion, emo_intensity);//}
		else p = spellCell(q, p);
		q=ct.next(q);
		q=ct.next(q);
	}


	if (isPronun(q)) p=ct.insafter(p, ct.getStr(q), FALSE, emotion, emo_intensity);
	else p = spellCell(q, p);
	
  //Trasladamos la frontera y eliminamos sobrantes.
	if(nint != 1){
		if(!strcmp(ct.getStr(ct.next(q)), "-" ))
			ct.del(ct.next(q));
	}
  q=ct.next(q);
  ct.setPatternForce(q, (char *)"l");
  ct.setStatus(q, stat);
 ct.setTnor(q, UTYPENOR_COMP);
      if(nint != 1){
		if(!strcmp(ct.getStr(ct.next(q)), "-" ))
			ct.del(ct.next(q));
	}
  q=ct.prevGrp(q);
  q=ct.delGrp(q);

  return q;
	
}

/**********************************************************/

