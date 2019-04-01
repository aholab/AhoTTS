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
#ifndef __NUMHILVL_HPP__
#define __NUMHILVL_HPP__

/**********************************************************/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/*
Copyright: 1998 - TSR/Aholab (DET) ETSII/IT-Bilbao

Nombre fuente................ numhilvl.hpp
Nombre paquete............... normalizador
Lenguaje fuente.............. C++
Estado....................... desarrollo
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Juan Luis Murugarren

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
0.0.7	 02/01/07  inaxio    Añadido NUM_RESTA y NUM_SUMA para restas y sumas de números
0.0.6	 29/12/06  inaxio    Añadido NUM_NEGATIVE para números negativos
0.0.5	 20/11/06  inaxio    Añadido NUM_UNIT para unidades monetarias
0.0.4	 18/10/06  inaxio    Añadidos NUM_ORD_ES_OS (1os) y el femenino _AS (1as) para el plural
0.0.3    16/11/02  inigos    Añadidos NUM_ORD_ES_O (º) y el femenino _A (ª)
0.0.3    07/03/01  Yon2.     Too Large Number (>18 chars) -> spell.
0.0.2    22/03/01  Yon2.     Need More Data.
0.0.1    09/12/00  Yon2.     Tipo de numero.
0.0.0    19/11/00  Yon2.     Codificacion inicial.

======================== Contenido ========================

M¢dulo expansor de n£meros a nivel alto. Expande cardinales
y ordinales. Se apoya en el m¢dulo de expansi¢n num‚rica de
nivel bajo.

Yon2.

===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/


/**********************************************************/

enum NumType	{
	NUM_UNKNOWN=0,
	NUM_CARD,
	NUM_PUNT,
	NUM_COMA,
	NUM_NEGATIVE,
	NUM_RESTA,
	NUM_SUMA,
	NUM_ORD_ES_O,
	NUM_ORD_ES_A,
	NUM_ORD_ES_OS,
	NUM_ORD_ES_AS,
	NUM_ORD,
	NUM_ORD_PUNT,
	NUM_CARD_DEC,
	NUM_TOO_LARGE,
	NUM_NEEDMOREDATA,
// Los porcentajes
	NUM_CARD_PER,
	NUM_PUNT_PER,
	NUM_COMA_PER,
// Las unidades monetarias
	NUM_UNIT
};

/**********************************************************/

#endif

