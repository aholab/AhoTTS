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

Nombre fuente................ es_dateexp.cpp
Nombre paquete............... normalizador
Lenguaje fuente.............. C++
Estado....................... desarrollo
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Juan Luis Murugarren

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
0.1.0    15/11/02  inigos    Recopia y adaptación para castellano
0.0.1    11/12/00  Yon2.     Soporte tipo de normalizacion.
0.0.0    25/10/00  Yon2.     Correcci¢n de las cadenas implicadas.
0.0.0    23/10/00  Yon2.     Codificacion inicial.

======================== Contenido ========================

  M‚todo para expandir fechas. S¢lo expande fechas en los
los formatos DD/MM/YYYY o DD/MM/YY. Es trabajo de capas
superiores la conversi¢n a alguno de estos formatos si se
encuentran expresadas en otro formato o si el mes se
encuentra en n£meros romanos o combinacion de ambas.

  ejemplo: 26 de abril de 2000

inigos
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/


#include "stdlib.h"
#include "stdio.h"

#include "es_t2l.hpp"
#include "symbolexp.h"
#include "string.h"
#include "choputi.h"

/**********************************************************/

CHAR *es_date_monthStr[]={
     (char*)"enero", (char*)"febrero", (char*)"marzo", (char*)"abril",
     (char*)"mayo", (char*)"junio", (char*)"julio",(char*) "agosto",
    (char*) "septiembre", (char*)"octubre", (char*)"noviembre", (char*)"diciembre"
};

CHAR es_date_deStr[]="de";

/**********************************************************/


/**********************************************************/

CtI LangES_TextToList::expdateYMD(CtI p)
{
  UINT year;
  UINT month;
  UINT day;
  CtI yy;
  CtI q, r;
  INT stat, emotion, emo_intensity;

	stat = ct.getStatus(p);
	emotion = ct.getEmotion(p);
	emo_intensity = ct.getEmo_int(p);

	r=p;
	year = chu_str2num(ct.cell(p).str);
	p = ct.next(p);
	p = ct.next(p);
	month = chu_str2num(ct.cell(p).str);
	p = ct.next(p);
	p = ct.next(p);
	day = chu_str2num(ct.cell(p).str);

	yy = p;

	q = p;

	p = upTo99(day, p, FALSE);

  // !!!!! Esta comprobaci¢n es redundante, capas superiores lo deber¡an
  // chequear antes.
  if ((month > 0) && (month < 13))  {
    p = ct.insafter(p, es_date_deStr, FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
    p = ct.insafter(p, es_date_monthStr[month - 1], FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
    p = ct.insafter(p, es_date_deStr, FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
  }
  else p = ct.insafter(p,(char*) "kaka", FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu

  p = expnum(r, p);

  //Trasladamos la frontera y eliminamos sobrantes.

  q=ct.next(q);
  ct.setPatternForce(q,(char*) "l");
  ct.setStatus(q, stat);
  ct.setTnor(q, UTYPENOR_DATE);

  q=ct.prevGrp(q);
  q=ct.delGrp(q);

  return q;


}

/**********************************************************/

