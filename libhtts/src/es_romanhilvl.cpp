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

Nombre fuente................ es_romanhilvl.cpp
Nombre paquete............... normalizador
Lenguaje fuente.............. C++
Estado....................... desarrollo
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Juan Luis Murugarren

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
0.0.7	 16/11/06  inaxio    Leer como cardinales nº>1000 y no deletrear.
0.0.6    15/11/02  inigos    Recopia y adaptación para castellano
0.0.5    04/05/01  Yon2.     Right Warning/Error info.
0.0.4    25/04/01  Yon2.     Multiling support.
0.0.3    08/04/01  Yon2.     Avoid Warnings.
0.0.2    29/03/00  Yon2.     Fix I. bug.
0.0.1    16/03/00  Yon2.     Romanos fuera de fechas solo en mayusculas.
0.0.0    28/11/00  Yon2.     Codificacion inicial.

======================== Contenido ========================

!!!!!!!!!!!!!!!!!
NOTA: Verificar que no haya caracteres en may£culas y
minusculas a la vez ya que en este caso no es un n£mero
romano. O incluso que no aparezcan en min£sculas.
!!!!!!!!!!!!!!!!!

Los numeros romanos son siempre ordinales. Hacer.
Y si van acompa¤ados de "." fuera, fin frase ?

Yon2.
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/

/**********************************************************/


#include "t2l.hpp"
#include "es_t2l.hpp"
#include "httsmsg.h"

#include "romanhilvl.hpp"
#include "roman.h"
#include "chset.h"

/**********************************************************/

#define DEBUG_ROMANx


pCHAR es_romanTable[] = {
	(char *)"primer", (char *) "segund", (char *)"tercer", (char *)"cuart", (char *)"quint",
		(char *)"sext", (char *)"septim", (char *)"octav", (char *)"noven", (char *)"décim",
		(char *)"undecim", (char *)"duodécim"
	};

pCHAR es_tensRomanTable[] = {
	(char *)"décim", (char *)"vigésim", (char *)"trigésim", (char *)"cuadragésim", (char *)"quincuagésim",
		(char *)"sexagésim", (char *)"septuagésim", (char *)"octogésim", (char *)"nonagésim"
	};

pCHAR es_hundredsRomanTable[] = {
	(char *)"centésim", (char *)"ducentésim", (char *)"tricentésim", (char *)"cuadrigentésim", (char *)"quingentésim",
		(char *)"sexcentésim", (char *)"septingentésim", (char *)"octingentésim", (char *)"noningentésim"
	};


CHAR es_masculinoRomanStr[]="o";
CHAR es_femeninoRomanStr[]="a";

/**********************************************************/

RomanType LangES_TextToList::isRomanN(CtI p)
{
	RomanType retval = ROMAN_UNKNOWN;
	pCHAR pattern;
	INT patlen;
	pCHAR temp;

#ifdef HTTS_DEBUG
	if (!ct(p).pattern) htts_error("LangES_TextToList::isRoman(CtI) - Trying to read null Pattern.");
#endif

	if (ct(p).pattern) pattern = strdup(ct(p).pattern);
  else return retval;


  patlen = strlen(pattern);

	//Si es patron de numero romano.
 	if ((pattern[0] == 'l') && (patlen == 1)) retval = ROMAN_ROMAN;

  //Chequeamos que las letras correspondan a un n£mero romano.
  if (!isRoman(ct.getStr(p))) {
    free(pattern);
    return ROMAN_UNKNOWN;
  }

  //Si no están en mayusculas no son numeros romanos.
  if (retval) {
	  temp = strdup(ct.getStr(p));
  	chset_StrUpper(temp);
  	if (strcmp(temp, ct.getStr(p))) retval = ROMAN_UNKNOWN;
    free(temp);
  }

	free(pattern);

#ifdef HTTS_DEBUG
#ifdef DEBUG_ROMAN
  switch(retval)  {
    case ROMAN_ROMAN:
         htts_warn("Type ROMAN_ROMAN");
         break;
    case ROMAN_ROMAN_P:
         htts_warn("Type ROMAN_ROMAN_P");
         break;
    case ROMAN_ROMAN_PUNT:
         htts_warn("Type ROMAN_ROMAN_PUNT");
         break;
    case ROMAN_NEEDMOREDATA:
         htts_warn("Type ROMAN_NEEDMOREDATA");
         break;
    default:
         htts_warn("Type ROMAN_UNKNOWN");
         break;
  }
#endif
#endif
	return retval;
}

/**********************************************************/

CtI LangES_TextToList::expRoman(CtI p, BOOL nearPoint, BOOL farPoint)
{
	CtI q;
	INT stat, emotion, emo_intensity;
	INT num,r;

	q=p;

	//convertimos el n£mero romano a decimal
	stat = ct.getStatus(q);
	emotion = ct.getEmotion(q);
	emo_intensity = ct.getEmo_int(q);
	num = romanStr2num(ct.getStr(q));

	if(num>999)//inaxio--> Si es mayor de 1000 lo leerá como número cardinal.
	{
		r=num%1000;
		num=num/1000;
		if(num!=1) q=upTo999(num,q);
		q=ct.insafter(q, (char *)"mil", FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu

		if(r!=0) q = upTo999(r,q);

		q=ct.next(p);
		ct.setPatternForce(q, (char *)"l");
		ct.setStatus(q, stat);
		ct.setTnor(q, UTYPENOR_NUMBER);

		q=ct.prevGrp(q);
		q=ct.delGrp(q);

	return q;
	}

	while (num)	{  // QUE PASA SI ES MAYOR DE 1000???
		if (num<=12)
		{
			q = ct.insafter(q, es_romanTable[num-1], FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
			num = 0;
		}
		else
			if (num<100) {
				q = ct.insafter(q, es_tensRomanTable[(UINT)(num/10 - 1)], FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
				num = num%10;
				if (num) q = ct.extendStr(q, es_masculinoRomanStr); // DEBE SER APPEND!!!
			}

			else {
				q = ct.insafter(q, es_hundredsRomanTable[(UINT)(num/100 - 1)], FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
				num = num%100;
				if (num) q = ct.extendStr(q, es_masculinoRomanStr);
			}
	}
	if (0) // HAY QUE SABER SI LA PALABRA ANTERIOR ES FEMENINO O MASCULINO
	{
		q = ct.extendStr(q, es_femeninoRomanStr);
	}
	else
	{
		q = ct.extendStr(q, es_masculinoRomanStr);
	}

	  //Trasladamos la frontera y eliminamos sobrantes.
	q=ct.next(p);
	ct.setPatternForce(q, (char *)"l");
	ct.setStatus(q, stat);
	ct.setTnor(q, UTYPENOR_NUMBER);

	q=ct.prevGrp(q);
	q=ct.delGrp(q);

	return q;
}

/**********************************************************/
