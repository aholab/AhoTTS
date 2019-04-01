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
#include "t2l.hpp"


/**********************************************************/

BOOL TextToList::isComp( CtI p )
{
	BOOL retval = FALSE;
	pCHAR pattern;

	CtI q;

	CHAR separator;



#ifdef HTTS_DEBUG

        if (!ct(p).pattern) htts_error("TextToList::isComp() - Trying to read null Pattern.");
#endif



	if (ct(p).pattern) pattern = strdup(ct(p).pattern);

  else return retval;



  //Buscamos patrones de tipo compuesto validos.
  switch (strlen(pattern))	{
  	case 3:
    	if ( ( pattern[0] == 'l' ) && ( pattern[1] == 'p' ) &&
        ( pattern[2] == 'l' ) ) {
				q=ct.next(p);
      	if (strlen(ct(q).str) != 1) break;
      	separator = (ct(q).str)[0];
      	if (separator == '-') retval = TRUE;
      }
      break;
    case 5:
    	if ( ( pattern[0] == 'l' ) && ( pattern[1] == 'p' ) &&
        ( pattern[2] == 'l' ) && ( pattern[3] == 'p' ) &&
        ( pattern[4] == 'l' )) {
				q=ct.next(p);
      	if (strlen(ct(q).str) != 1) break;
      	separator = (ct(q).str)[0];
      	if (separator != '-') break;
				q=ct.next(q);
				q=ct.next(q);
				if (strlen(ct(q).str) != 1) break;
				if (separator != (ct(q).str)[0]) break;
				if ( (separator == '-') ) retval = TRUE;
      }
      break;
    default:
    	break;
  }
  
	free(pattern);

	

	return retval;


}

/**********************************************************/



