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

Nombre fuente................ es_percent.cpp
Nombre paquete............... normalizador
Lenguaje fuente.............. C++
Estado....................... desarrollo
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Juan Luis Murugarren

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
0.1.4	 11/12/06  inaxio    zuzenketak n,n% edo n.n% kasuak ebazteko
0.1.3	 30/10/06  inaxio    zuzenketak "if(junto)" kasurako.
0.1.2	 24/05/05  Larraitz  Pattern Force ezarri
0.1.1	 08/05/03  inigos    Adaptación de los porcentajes
0.1.0    15/11/02  inigos    Recopia y adaptación para castellano
0.0.3    04/05/01  Yon2.     Split in language dependent files.
0.0.2    25/04/01  Yon2.     Multiling support.
0.0.1    07/04/01  Yon2.     If 2nd pattern null => need more data.
0.0.0    22/03/01  Yon2.     Codificacion inicial.

======================== Contenido ========================

	Este módulo se encarga de resolver el signo de tanto por
ciento. Si aparece solo se expandirá y se acabo.
	Si aparece pegado 10% se separa pero sólo para enteros.
	En los demás casos "10 %" o "10,01 %" se lee correctamente.

	Aunque debería detectar y tratarse el porcentaje exclusivamente
en este módulo, por ser más fácil, parte se realiza en el de números
(numhilvl), ya que se detecta un número y se busca el signo.


inigos
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include "percent.hpp"
#include "es_t2l.hpp"

#include "httsmsg.h"

/**********************************************************/

/**********************************************************/

CtI LangES_TextToList::expPercent( CtI p , BOOL junto )
{
	CtI q,r;
	INT stat;
	TNOR tnor;
	pCHAR pattern;
	INT emotion, emo_intensity,patlen;
	BOOL punto;

	emotion = ct.getEmotion(p);
	emo_intensity = ct.getEmo_int(p);

	stat = ct.getStatus(p);
	tnor = ct.getTnor(p);

	if(junto)
	{
		if (ct(p).pattern)
		{
			pattern = strdup(ct(p).pattern);
		}
		patlen = strlen(pattern);
		free(pattern);
		if (patlen>1)
		{
			if(patlen==2)
			{
				q=ct.next(p);
				r = expnum(p);
				r = ct.next(q);
				ct.setPatternForce(r,(char *) "l");

				r = ct.insbefore(r, (char *)"ciento", FALSE);
				ct.setPattern(r, (char *)"l");
				ct.setStatus(r, stat);
				ct.setTnor(r, tnor);

				r = ct.insbefore(r, (char *)"por", FALSE);
				ct.setPattern(r, (char *)"l");
				ct.setStatus(r, stat);
				ct.setTnor(r, tnor);

				r=ct.prevGrp(r);
				r=ct.delGrp(r);

				r = expnum(p);
				r = ct.del(p);
				p=r;
				return p;
			}
			else if(patlen==4)
			{
				if (strcmp(ct.getStr(ct.next(ct.next(p))), ".") == 0)
					punto=TRUE;
				else
					punto=FALSE;
				ct.del(ct.next(ct.next(ct.next(p))));
				p=ct.lastInGrp(p);
				p = ct.insafter(p, (char *)"por", FALSE, emotion, emo_intensity);
				p = ct.insafter(p, (char *)"ciento", FALSE, emotion, emo_intensity);
				p=ct.prev(ct.prev(ct.prev(p)));
				p=ct.del(p);
				p=expnum(p);
				p=ct.del(p);

				if(punto)
					p = ct.insbefore(p,(char *)"punto",FALSE, emotion, emo_intensity);
				else
					p = ct.insbefore(p,(char *)"coma",FALSE, emotion, emo_intensity);
				p=ct.prev(p);
				p=expnum(p);
				p=ct.del(p);

				ct.setPattern(p, (char *)"l");
				ct.setStatus(p, stat);
				ct.setTnor(p, tnor);

				return p;
			}
			else return p;
		}

	}

	else
	{
		q=ct.del(p);

		q = ct.insbefore(q, (char *)"por", FALSE);
		ct.setPatternForce(q, (char *)"l"); //Larraitz
		ct.setStatus(q, stat);
		ct.setTnor(q, tnor);

		q = ct.insafter(q, (char *)"ciento", FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
		ct.setPatternForce(q, (char *)"l"); //Larraitz
		ct.setStatus(q, stat);
		ct.setTnor(q, tnor);
		return q;
	}
}

/**********************************************************/

/**********************************************************/

PercentType LangES_TextToList::isPercent( CtI p)
{
	PercentType retval = PERCENT_UNKNOWN;
	pCHAR pattern;
	INT patlen;

#ifdef HTTS_DEBUG

        if (!ct(p).pattern) htts_error("TextToList::isPercent(CtI) - Trying to read null Pattern.");
#endif


	if (ct(p).pattern) pattern = strdup(ct(p).pattern);
	else return retval;

	patlen = strlen(pattern);

	//Porcentajes del tipo número%

	if (!strcmp(pattern, "ns"))
	{
		if(strcmp(ct.getStr(ct.next(p)), "%") == 0) // Si el simbolo es el '%'
		{
			free(pattern);

			retval = PERCENT_SPANISH;

			return retval;
		}
		else
		{
			free(pattern);
			return retval;
		}
	}

	//Porcentajes de tipo número(punto o coma)número%

	if (patlen == 4)
	{
		if((!strcmp(pattern, "npns")) &&
		(!(strcmp(ct.getStr(ct.next(ct.next(ct.next(p)))),"%"))) &&
		((!(strcmp(ct.getStr(ct.next(p)), ".")))||(!(strcmp(ct.getStr(ct.next(p)), ",")))))
		{
			free(pattern);
			retval = PERCENT_SPANISH;
			return retval;
		}
		else
		{
			free(pattern);
			return retval;
		}
	}

	free(pattern);

	return retval;
}

/**********************************************************/

