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

Nombre fuente................ datehilvl.cpp
Nombre paquete............... normalizador
Lenguaje fuente.............. C++
Estado....................... desarrollo
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Juan Luis Murugarren

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
0.0.2	 27/11/06  inaxio    No aceptar 29 de febrero en años no bisiestos.
0.0.1    25/04/01  Yon2.     Multiling support.
0.0.0    29/11/00  Yon2.     Codificacion inicial.

======================== Contenido ========================

	Detección del formato de fecha y funciones para
para la adaptación de ese formato al único expandible
que es YMD propio de euskara.
	Si la detección falla devuelve DATE_UNKNOWN.

        Forma parte de la clase WordChop.
Yon2.
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/



#include "stdlib.h"
#include "stdio.h"

#include "httsmsg.h"

#include "eu_t2l.hpp"
#include "roman.h"
#include "datehilvl.hpp"

/**********************************************************/


/**********************************************************/

CtI LangEU_TextToList::dateFromSpanish(CtI p)
{
	CtI pp, qq;
	pCHAR day;
	pCHAR year;

	pp = p;
	qq = ct.next(pp);
	qq = ct.next(qq);
	qq = ct.next(qq);
	qq = ct.next(qq);

	day = strdup(ct(pp).str);
	year = strdup(ct(qq).str);

	ct.setStr(pp, year);
	ct.setStr(qq, day);

	free(day);
	free(year);

	return p;
}

/**********************************************************/


/**********************************************************/

CtI LangEU_TextToList::dateFromRoman(CtI p)
{
	CtI q;
	UINT num;
	CHAR temp[80];

	q = ct.next(p);
	q = ct.next(q);

	num = romanStr2num(ct(q).str);
	sprintf(temp, "%d", num);
	ct.setStr(q, temp);

	(ct(p).pattern)[2] = 'n';

	return p;
}

/**********************************************************/


/**********************************************************/

CtI LangEU_TextToList::dateFromRomanSpanish(CtI p)
{
	p = dateFromRoman(p);
	p = dateFromSpanish(p);

	return p;
}

/**********************************************************/

/**********************************************************/

DateType LangEU_TextToList::isDate(CtI p)
{
	DateType retval = DATE_UNKNOWN;
	DateType dateTemp;
	pCHAR pattern;
	CtI q;
	CHAR separator;
	BOOL lettMonth = FALSE;
	BOOL roman = FALSE;
	UINT num1, num2, num3;
        UINT daysInMonth[]={31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

#ifdef HTTS_DEBUG
        if (!ct(p).pattern) htts_error("TextToList::isDate() - Trying to read null Pattern.");
#endif

	if (ct(p).pattern) pattern = strdup(ct(p).pattern);
  else return retval;

	//Si no es uno de los patrones de fecha.
	if ( (strlen(pattern) != 5) ||
		(pattern[1] != pattern[3]) ||
		(pattern[0] != 'n') || (pattern[4] != 'n') ||
		(!((pattern[2] == 'n')||(pattern[2] == 'l'))) ) {
		free(pattern);
		return retval;
	}
	if (pattern[2] == 'l') lettMonth = TRUE;

	free(pattern);


	//Chequeamos que los separadores sean válidos.
	q=ct.next(p);
	if (strlen(ct(q).str) != 1) return retval;
	separator = (ct(q).str)[0];
	q=ct.next(q);
	q=ct.next(q);
	if (strlen(ct(q).str) != 1) return retval;
	if (separator != (ct(q).str)[0]) return retval;
	if (!((separator == '/') || (separator == '-'))) return retval;


	//Recogemos los números implicados.
	q=p;
	sscanf(ct(q).str, "%d", &num1);
	q=ct.next(q);
	q=ct.next(q);
	if (lettMonth) {
		if (isRoman(ct(q).str)) {
			num2 = romanStr2num(ct(q).str);
			roman = TRUE;
		}
		else return retval;
	}
	else sscanf(ct(q).str, "%d", &num2);
	q=ct.next(q);
	q=ct.next(q);
	sscanf(ct(q).str, "%d", &num3);

	//Buscamos el formato de fecha;
	if ( (num2 < 1) || (num2 > 12) ) return retval;
	if ((num1 > 31) && (num3 > 31)) return retval;
	dateTemp = DATE_BASQUE;
	if (num1 > 31) dateTemp = DATE_BASQUE;
	if (num3 > 31) dateTemp = DATE_SPANISH;

	if (dateTemp == DATE_BASQUE)
		// inaxio-> no aceptar 29 de febrero si no es bisiesto;
		if(daysInMonth[num2 - 1] < num3 ||
		((num2 == 2) && (num1 % 4 != 0) && (num3 > 28))) return retval;
	if (dateTemp == DATE_SPANISH)
		// inaxio-> no aceptar 29 de febrero si no es bisiesto;
		if(daysInMonth[num2 - 1] < num1 ||
		((num2 == 2) && (num3 % 4 != 0) && (num1 > 28))) return retval;

	if (roman)  {
		if  (dateTemp == DATE_BASQUE) retval = DATE_ROMAN_BASQUE;
		if  (dateTemp == DATE_SPANISH) retval = DATE_ROMAN_SPANISH;
	}
	else  retval = dateTemp;


	return retval;
}
/**********************************************************/
