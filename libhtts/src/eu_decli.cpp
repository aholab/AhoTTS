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

Nombre fuente................ decli.cpp
Nombre paquete............... normalizador
Lenguaje fuente.............. C++
Estado....................... desarrollo
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Juan Luis Murugarren

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
0.0.6    07/05/01  Yon2.     Added isGroupDekd because of dic dependency
0.0.6    04/05/01  Yon2.     Right Warning/Error info.
0.0.4    04/05/01  Yon2.     Split in language dependent files.
0.0.3    24/04/01  Yon2.     Multiling support.
0.0.2    23/12/00  Yon2.     0.0.1 anulado.
0.0.1    13/12/00  Yon2.     Si UTYPENOR_TIME no doblar 'r'
0.0.0    09/12/00  Yon2.     Codificacion inicial.

======================== Contenido ========================

  Deklinador de palabras. La declinaci¢n se hace en dos pasos:
1 - Se detectan los grupos que tengan un tipo letra al final
de su patr¢n. Se verifica que es una declinaci¢n. Si es as¡ se
separa del grupo principal creando un grupo "dek".

2 - Una vez se ha normalizado el fragmento derecho se declina.

Si entre el grupo principal y la declinaci¢n hay un "-" se
elimina el mismo.


Se implementan los siguientes lazos entre palabra-declinaci¢n:

DEKLINABIDEA: FONETIKA-ARAUAK

1.  A + A => A
2.  A + E => E
3.  E + A => EA
4.  E + E => EE
5.  Konts. + Konts. = Konts. + E + Konts.
6.  Bokala + Bokala = Bokala + R + Bokala

Nota: Sospecho que 7. R al final se dobla. Ej: 10-etan ->hamarRetan.

Solo tienen cierta utilidad, desde el punto de vista de la
normalizaci¢n las reglas 1,2 y la 'nota'.

Las dem s o no implican ninguna actuaci¢n o se deben de
escribir con la correcci¢n mental ya hecha.

Ej ok:
   10-etan -> hamar r etan
   11-etan -> hamaik etan -> se pierde la a de hamaika
   10-ak -> hamar r ak
   15-ekin -> hamabost ekin
Ej mal:
   15-rekin -> hamabost e rekin  -> Mal escrito => fallo.

Testear un poco m s. En general y escribiendo con fundamento
va bien. Es un normalizador no un corrector ortogr fico.


Yon2.
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

/**********************************************************/

#include <stdio.h>

#include "httsmsg.h"
#include "chartype.h"

#include "eu_t2l.hpp"
#include "choputi.h"

/**********************************************************/


/**********************************************************/

BOOL LangEU_TextToList::isGroupDecd(CtI p)
{
	BOOL retval = FALSE;
	BOOL isDec = FALSE;
	UINT len, patlen;
	INT i;
	CtI q;
	HDicRef myhdic;

#ifdef HTTS_DEBUG
        if (!ct(p).pattern) htts_error("TextToList::isGroupDecd() - Trying to read null Pattern.");
#endif

	if (!ct(p).pattern) return retval;

  patlen = strlen(ct(p).pattern);
	if (patlen == 1) return retval;
	if (ct(p).pattern[patlen - 1] != 'l') return retval;

	q=p;
	//Vamos hasta la celda final del grupo.
	for (i=1 ; i < (INT)patlen ; i++) q = ct.next(q);

	//Es declinación?
	myhdic = dic->search(ct(q).str);
	isDec = dic->query(myhdic, HDIC_QUERY_EU_DEC);
	if (!isDec) return retval;
	//Coincide en su totalidad?
	len = dic->query(myhdic, HDIC_QUERY_MATCHLEN);
	if (!len) retval = TRUE;

	return retval;
}

/**********************************************************/


/**********************************************************/
//Sabemos que el grupo a declinar est  ya normalizado y que
//la declinacion esta en el grupo siguiente.
/**********************************************************/

CtI LangEU_TextToList::dekline( CtI p)
{
  CtI q;
  pCHAR dek;
  CHAR temp;
  CHAR lastDekd;  //ultimo caracter de la palabra declinada.
  CHAR firstDek;  //primer caracter de la declinaci¢n.

#ifdef HTTS_DEBUG
        if (!p) htts_error("LangEU_TextToList::dekline() - Trying to read null CtI");
#endif

	q = ct.nextGrp(p);
	dek = ct(q).str;
	q = ct.prev(q);



	temp = (ct(q).str)[strlen(ct(q).str) - 1];
	lastDekd = isVowel(temp)?uniqVowel(temp):uniqConso(temp);
	temp = dek[0];
	firstDek = isVowel(temp)?uniqVowel(temp):uniqConso(temp);


  //Doblamos la 'r' de la palabra a declinar si la tiene y ..
//  if (ct.getTnor(p) != UTYPENOR_TIME)
	if ((lastDekd == 'r') && (firstDek !='r'))
         q = ct.extendStr(q, (char *)"r");

	// a + a = a   a + e = e
	if (lastDekd == 'a')
		switch(firstDek)
	{
      case 'a':
		  dek = &(dek[1]);
		  break;
      case 'e':
		  (ct(q).str)[strlen(ct(q).str) - 1] = '\x0';
		  break;
      default:
		  break;
    }

	// e + a = ea  e + e = ee
	else if (lastDekd == 'e')
		switch(firstDek)  {
      case 'a':
		  break;
      case 'e':
		  break;
      default:
		  break;
    }
	// k + k = k + e + k
	else if (isConso(lastDekd) && isConso(firstDek))
		q = ct.extendStr(q, (char *)"e");
	// v + v = v + r + v
	//  else if (isVowel(lastDekd) && isVowel(firstDek))
	//         q = ct.extendStr(q, "r");

	q = ct.extendStr(q, dek);

	//eliminamos la celda de declinaci¢n.
	ct.del(ct.next(q));
	return p;
}

/**********************************************************/
