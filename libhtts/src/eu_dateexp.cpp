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

Nombre fuente................ ctlist.hpp
Nombre paquete............... normalizador
Lenguaje fuente.............. C++
Estado....................... desarrollo
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Juan Luis Murugarren

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
0.0.1    11/12/00  Yon2.     Soporte tipo de normalizacion.
0.0.0    25/10/00  Yon2.     Correcci¢n de las cadenas implicadas.
0.0.0    23/10/00  Yon2.     Codificacion inicial.

======================== Contenido ========================

  M‚todo para expandir fechas. S¢lo expande fechas en los
los formatos YYYY/MM/DD o YY/MM/DD. Siendo trabajo de capas
superiores la conversi¢n a alguno de estos formatos si se
encuentran expresadas en formato castellano o si el mes
se encuentra en n£meros romanos o combinacion de ambas.

  Formato utilizado para la expansi¢n:

  xxx-EKO xxx-AREN xxx-A

  Ej: Mila eta bostehunEKO otsaliAREN hamabostA

NOTA: Al declinar los numeros se a¤ade "a" si no la tienen
ya, y si el n£mero termina en "r" se dobla esta misma antes
de a¤adirle la "a".

Yon2.
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/


#include "stdlib.h"
#include "stdio.h"

#include "eu_t2l.hpp"
#include "symbolexp.h"
#include "string.h"
#include "choputi.h"

/**********************************************************/
CHAR *eu_date_monthStr[]={
     (char *)"urtarril", (char *)"otsail", (char *)"martxo", (char *)"apiril",
     (char *)"maiatz", (char *)"ekain", (char *)"uztail", (char *)"abuztu",
     (char *)"irail", (char *)"urri", (char *)"azaro", (char *)"abendu"
};

CHAR eu_date_rekoStr[]="reko";
CHAR eu_date_ekoStr[]="eko";
CHAR eu_date_koStr[]="ko";
CHAR eu_date_renStr[]="aren";

/**********************************************************/

CHAR eu_date_aStr[]="a";
CHAR eu_date_raStr[]="ra";

/**********************************************************/

/**********************************************************/

CtI LangEU_TextToList::expdateYMD(CtI p)
{
  UINT year;
  UINT month;
  UINT day;
  CtI yy;
  CHAR last;
  CtI q;
	INT stat, emotion, emo_intensity;

	stat = ct.getStatus(p);
	emotion = ct.getEmotion(p);
	emo_intensity = ct.getEmo_int(p);


	year = chu_str2num(ct.cell(p).str);
  yy = p;
	p = ct.next(p);
	p = ct.next(p);
	month = chu_str2num(ct.cell(p).str);
	p = ct.next(p);
	p = ct.next(p);
	day = chu_str2num(ct.cell(p).str);
  q = p;

  p = expnum(yy, p);

  last = ct(p).str[strlen(ct(p).str) - 1];
  switch(last)  {
    case 'a':
    case 'e':
    case 'i':
    case 'o':
    case 'u':
             p = ct.extendStr(p, eu_date_koStr);
             break;
    case 'r':
             p = ct.extendStr(p, eu_date_rekoStr);
             break;
    default:
             p = ct.extendStr(p, eu_date_ekoStr);
             break;
  }

  // !!!!! Esta comprobaci¢n es redundante, capas superiores lo deber¡an
  // chequear antes.
  if ((month > 0) && (month < 13))  {
    p = ct.insafter(p, eu_date_monthStr[month - 1], FALSE, emotion, emo_intensity);
    p = ct.extendStr(p, eu_date_renStr);
  }
  else p = ct.insafter(p,(char *) "kaka", FALSE, emotion, emo_intensity);

  p = upTo99(day, p);

  last = ct(p).str[strlen(ct(p).str) - 1];
  if (!(last == 'a')) p = ct.extendStr(p, (last == 'r')?eu_date_raStr:eu_date_aStr);

  //Trasladamos la frontera y eliminamos sobrantes.

  q=ct.next(q);
  ct.setPatternForce(q, (char *)"l");
  ct.setStatus(q, stat);
  ct.setTnor(q, UTYPENOR_DATE);

  q=ct.prevGrp(q);
  q=ct.delGrp(q);

  return q;


}

/**********************************************************/

