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

Nombre fuente................ virtual.cpp
Nombre paquete............... normalizador
Lenguaje fuente.............. C++
Estado....................... desarrollo
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Juan Luis Murugarren

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
0.0.1    07/05/01  Yon2.     Added isGroupDekd.
0.0.0    04/05/01  Yon2.     Codificacion inicial.

======================== Contenido ========================
  Este archivo contiene algunas de las funciones virtuales
que se definen en la clase "TextToList". Son funciones vac¡as
en esta clase y deber n ser redefinidas en una clase
dependiente de "TextToList" y para un lenguaje espec¡fico.

  Deben existir, ya que el m‚todo Input y alg£n otro las
llama.


Yon2.
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/
 
/**********************************************************/

#include "t2l.hpp"
#include "httsmsg.h"


/***********************************************************************/


/***********************************************************************/

AbbAcrUniType TextToList::isAbbAcrUni( CtI p )
{

#ifdef HTTSDEBUG
	htts_error("TextToList::isAbbAcrUni -> You must define your own language dependent function.");
#endif

	return AAU_UNKNOWN;
}

/***********************************************************************/

DateType TextToList::isDate(CtI p)
{

#ifdef HTTSDEBUG
	htts_error("TextToList::isDate -> You must define your own language dependent function.");
#endif

	return DATE_UNKNOWN;
}


/***********************************************************************/

INT TextToList::ct2nt(const CHAR *str, BOOL flush )
{

#ifdef HTTSDEBUG
	htts_error("TextToList::ct2nt -> You must define your own language dependent function.");
#endif

	return 0;
}

/***********************************************************************/


/***********************************************************************/

INT  TextToList::ct2nt(BOOL flush)
{

#ifdef HTTSDEBUG
	htts_error("TextToList::ct2nt -> You must define your own language dependent function.");
#endif

	return 0;
}

/***********************************************************************/


/***********************************************************************/

CtI TextToList::normal(VOID)
{

#ifdef HTTSDEBUG
	htts_error("TextToList::normal -> You must define your own language dependent function.");
#endif

	return NULL;
}

/***********************************************************************/


/***********************************************************************/

BOOL TextToList::isPronun(CtI p)
{

#ifdef HTTSDEBUG
	htts_error("TextToList::isPronun -> You must define your own language dependent function.");
#endif

	return FALSE;
}

/***********************************************************************/

/***********************************************************************/

BOOL TextToList::isGroupDecd(CtI p)
{

#ifdef HTTSDEBUG
	htts_error("TextToList::isGroupDecd -> You must define your own language dependent function.");
#endif

	return FALSE;
}

/***********************************************************************/
/***********************************************************************/

INT WdChop::getchtype(CHAR c)
{

#ifdef HTTSDEBUG
	htts_error("TextToList::getchtype -> You must define your own language dependent function.");
#endif

	return 0;
}

/***********************************************************************/

