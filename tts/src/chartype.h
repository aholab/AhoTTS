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
#ifndef __CHARTYPE_H__
#define __CHARTYPE_H__

/**********************************************************/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/*
Copyright: 1998 - TSR/Aholab (DET) ETSII/IT-Bilbao

Nombre fuente................ chartype.h
Nombre paquete............... normalizador
Lenguaje fuente.............. C
Estado....................... desarrollo
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Juan Luis Murugarren

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
0.0.3    30/10/00  Yon2.     Generador de tipo para patrones.
0.0.2    28/10/00  Yon2.     Funcion para leer el tipo de caracter.
0.0.1    23/10/00  Yon2.     Archivo de cabecera.
0.0.0    17/10/00  Yon2.     Codificacion inicial.

======================== Contenido ========================

Matriz de 256 elementos que clasifica los caracteres de la tabla
ISO 8859/Latin 1 en grupos según su función.

Pueden ser:
       - Letras
       - Numeros
       - Simbolos
       - Delimitadores -> Espacio, tabulador, CR, LF

Esta tabla es aplicable a caracteres ya filtrados. Ver "isofilt.c".

No editar este archivo desde DOS con BC la página de códigos
es totalmente diferente.

Cuidado!! Tabla exportada electronicamente de los fuentes de
Oscar!!!
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/


#include "tdef.h"

/**********************************************************/

enum ChType {
	CHTYPE_NULL=0,
	CHTYPE_DELIM,
	CHTYPE_PUNTT,
	CHTYPE_SYMBL,
	CHTYPE_DIGIT,
	CHTYPE_LETTR
};

enum SyChType	{
	SYCHTYPE_NONE='X',
	SYCHTYPE_DELIM = 'd',
	SYCHTYPE_PUNTT = 'p',
	SYCHTYPE_SYMBL = 's',
	SYCHTYPE_DIGIT = 'n',
	SYCHTYPE_LETTR = 'l'
};

/***********************************************************************/




#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************/

extern INT chtype[256]; //La tabla que mapea caracteres a "tipo de caracter"

/**********************************************************/

#ifdef __cplusplus
}
#endif

/**********************************************************/

#endif
