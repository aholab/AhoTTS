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

Nombre fuente................ percent.cpp
Nombre paquete............... normalizador
Lenguaje fuente.............. C++
Estado....................... desarrollo
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Juan Luis Murugarren

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
0.0.4    04/05/01  Yon2.     Right Warning/Error info.
0.0.3    04/05/01  Yon2.     Split in language dependent files.
0.0.2    25/04/01  Yon2.     Multiling support.
0.0.1    07/04/01  Yon2.     If 2nd pattern null => need more data.
0.0.0    22/03/01  Yon2.     Codificacion inicial.

======================== Contenido ========================

	Este módulo se encarga de resolver el signo de tanto por
ciento. Si aparece solo se expandirá y se acabo.
	Si aparece con un número por delante (forma castellana)
se pasará a la forma en euskara. Si aparece detrás el número
se expandirá como corresponda, y si aparece un número detrás
y otro delante será preferida la forma en euskara, es decir,
se tomarña el número que va por detrás.

		% 100 -> ehuneko hamarra

Se declina el numero.  XX-a

Nota: Puede que estemos buscando el número siguiente y este todavía
no haber entrado => falla detección. => WorkArround
Solo números enteros.

Yon2.
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include "percent.hpp"
#include "t2l.hpp"

#include "httsmsg.h"

/**********************************************************/


/**********************************************************/

/*PercentType TextToList::isPercent( CtI p)
{
	PercentType retval = PERCENT_UNKNOWN;
	pCHAR pattern;
  INT patlen;

	CtI q;

#ifdef HTTS_DEBUG

        if (!ct(p).pattern) htts_error("TextToList::isPercent(CtI) - Trying to read null Pattern.");
#endif



	if (ct(p).pattern) pattern = xstrdup(ct(p).pattern);

  else return retval;



  patlen = strlen(pattern);



	//Si es patron de signo de porciento -> Simbolo.
 	if (!((pattern[0] == 's') && (patlen == 1))) {
    free(pattern);

    return retval;

  }

	if (!((ct.getStr(p)[0] == '%') && (strlen(ct.getStr(p)) == 1))) {
    free(pattern);

    return retval;

  }


	free(pattern);





	//Verificamos que hay un número en el entorno.

	//Primero en Euskara, el número detrás.
	if (ct.nextGrp(p)) {
		q = ct.nextGrp(p);

#ifdef HTTS_DEBUG
    if (!ct(q).pattern) htts_warn("TextToList::isPercent(CtI) - Trying to read null 2nd Pattern.");
#endif



		if (!ct(q).pattern) {
			if (moreDataNeedAllowed) retval =	PERCENT_NEEDMOREDATA;
		}
		else	{
			pattern = xstrdup(ct(q).pattern);
			patlen = strlen(pattern);

			//Si es patron de numero tenemos un porcentaje en euskara.
			if (((pattern[0] == 'n') && (patlen == 1))) retval = PERCENT_BASQUE;
			free(pattern);
		}
  }

	else if (moreDataNeedAllowed) retval =	PERCENT_NEEDMOREDATA;

	if (!retval) if (ct.prevGrp(p)) {
		q = ct.prevGrp(p);


		if ((ct.getTnor(q) == UTYPENOR_NUMBER) && !(ct.isDekd(q)) )	retval = PERCENT_SPANISH;
  }






	return retval;


}
*/
/**********************************************************/

