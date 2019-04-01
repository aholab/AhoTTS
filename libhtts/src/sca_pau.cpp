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

Nombre fuente................ sca_pau.cpp
Nombre paquete............... normalizador
Lenguaje fuente.............. C++
Estado....................... desarrollo
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Juan Luis Murugarren

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
0.0.1    22/06/01  Yon2.     Pause location change. From PrevCell
                             to actual Cell.
0.0.0    21/06/01  Yon2.     Codificacion inicial.

======================== Contenido ========================

Inserción de pausas.

Caracter 'P' -> indica insertar pausa. La pausa se asocia a la
palabra inmediatamente anterior a la marca de comienzo. El
tipo de secuencia de escape "Pausa" no tiene marca de final.
El valor del parámetro puede ser:
(ver scapesec.hpp)
Aunque de momento sólo se insertan pausas de tipo UPAUSE_SHORT y
UPAUSE_LONG, correspondientes al valor 1 y 2 respectivamente.
El resto de pausas se mapean a UPAUSE_SHORT. Aunque no hay
ningún problema para soportarlas todas.

Caracter 'P' -> indica insertar pausa. La pausa se asocia a la
palabra inmediatamente anterior a la marca de comienzo. El
tipode secuencia de escape "Pausa" no tiene marca de final.
El valor del parámetro puede ser:
1 UPAUSE_SHORT
2 UPAUSE_LONG
3 UPAUSE_UBEGIN
4 UPAUSE_UEND
5 UPAUSE_SDECL
6 UPAUSE_SQUEST
7 UPAUSE_SEXCL
8 UPAUSE_SPAUSE
9 UPAUSE_SNEUTR


Yon2.

===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

/**********************************************************/

#include "tdef.h"
#include "scapeseq.hpp"
#include "uttws.hpp"

#include "httsmsg.h"

/**********************************************************/


/**********************************************************/

BOOL insertPause(UttWS *ws, UttI p, INT value)
{
	BOOL retval = FALSE;
	if (value)	{
		switch(value)	{
			case 1:
				if (p) {
					ws->cell(p).setPause(UPAUSE_SHORT);
					retval = TRUE;
				}
				break;
			case 2:
				if (p) {
					ws->cell(p).setPause(UPAUSE_LONG);
					retval = TRUE;
				}
				break;
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
				if (p) {
					ws->cell(p).setPause(UPAUSE_SHORT);
					retval = TRUE;
				}
				break;
			default:
#ifdef DEBUG_SCA
				htts_warn("Sca_pau.cpp: inserPause Value out of range %d", value);
#endif
				break;
		}
	}
	return retval;
}

/**********************************************************/




