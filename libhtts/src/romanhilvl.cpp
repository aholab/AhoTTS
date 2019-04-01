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

Nombre fuente................ romanhilvl.cpp
Nombre paquete............... normalizador
Lenguaje fuente.............. C++
Estado....................... desarrollo
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Juan Luis Murugarren

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
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
#include "httsmsg.h"

#include "romanhilvl.hpp"
#include "roman.h"
#include "chset.h"

/**********************************************************/

#define DEBUG_ROMANx


/**********************************************************/

RomanType TextToList::isRomanN(CtI p)
{
	RomanType retval = ROMAN_UNKNOWN;
	pCHAR pattern;
  INT patlen;
  pCHAR temp;
  CtI q;

#ifdef HTTS_DEBUG
        if (!ct(p).pattern) htts_error("TextToList::isRoman(CtI) - Trying to read null Pattern.");
#endif

	if (ct(p).pattern) pattern = strdup(ct(p).pattern);
  else return retval;


  patlen = strlen(pattern);

	//Si es patron de numero romano.
 	if ((pattern[0] == 'l') && (patlen == 1)) retval = ROMAN_ROMAN;
  if (patlen == 2)
    if ((pattern[0] == 'l') && (pattern[1] == 'p')) retval = ROMAN_ROMAN_P;

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



  if (retval == ROMAN_ROMAN)	{
  		if ( ct.nextGrp(p) )
      	if (ct.isRightFrg(ct.nextGrp(p))) {
	      	q = ct.nextGrp(p);
	        if ((strlen(ct(q).pattern) == 1) && ((ct(q).pattern)[0] == 'p')){
	        	if (((ct(q).str)[0] == '.') && (strlen(ct(q).str) == 1)) {
							if (ct.nextGrp(q))	{
								q=ct.nextGrp(q);
								if (ct(q).pattern)	{
								if ((ct(q).pattern)[0] == 'l')
									if (ct.getStr(q)[0] == chset_ToLower(ct.getStr(q)[0]) )
										retval = ROMAN_ROMAN_PUNT;
								}
								else if (moreDataNeedAllowed) if (!ct.nextGrp(q)) retval = ROMAN_NEEDMOREDATA;
							}
							else if (moreDataNeedAllowed) if (!ct.nextGrp(q)) retval = ROMAN_NEEDMOREDATA;

          	}
	        	if (((ct(q).str)[0] == '.') && (strlen(ct(q).str) > 1)) retval = ROMAN_ROMAN_PUNT;

					}
					else if ((strlen(ct(q).pattern) > 1) && ((ct(q).pattern)[0] == 'p'))
	        	if ((ct(q).str)[0] == '.')	retval = ROMAN_ROMAN_PUNT;
      	}
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





/**********************************************************/


