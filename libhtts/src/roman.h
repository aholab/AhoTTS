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
#ifndef __ROMAN_H__
#define __ROMAN_H__


/**********************************************************/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/*
Copyright: 1998 - TSR/Aholab (DET) ETSII/IT-Bilbao

Nombre fuente................ roman.h
Nombre paquete............... normalizador
Lenguaje fuente.............. C++
Estado....................... desarrollo
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Juan Luis Murugarren

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
0.0.0    28/11/00  Yon2.     Codificacion inicial.

======================== Contenido ========================

  Se da en este m¢dulo soporte para n£meros Romanos. Se da en
base a dos funciones:
  - isRoman() -> que nos dice si un n£mero es romano o no.
  - romanStr2num -> que convierte una cadena de caracteres
romanos en un n£mero decimal.
  La mayor complejidad se encuentra en saber si un n£mero es
romano. Sabido esto convertir el n£mero a decimal es
simplic¡simo.

NOTA: Actualmente, operan ambas funciones est‚ la cadena
formada por letras en mayusculas, minusculas o combinaci¢n
de ambas.

!!!!!!!!!!!!!!!!!
NOTA: Verificar que no haya caracteres en may£culas y
minusculas a la vez ya que en este caso no es un n£mero
romano. O incluso que no aparezcan en min£sculas.
!!!!!!!!!!!!!!!!!

Yon2.
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/


/**********************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************/

#include "tdef.h"

enum  {
      ROMAN_NONUM = 0,
      ROMAN_I,
      ROMAN_V,
      ROMAN_X,
      ROMAN_L,
      ROMAN_C,
      ROMAN_D,
      ROMAN_M,
      NUM_ROMAN
};

/**********************************************************/


/**********************************************************/

BOOL isRoman(pCHAR str);
UINT romanStr2num(pCHAR str);

/**********************************************************/

#ifdef __cplusplus
}
#endif

/**********************************************************/

#endif
