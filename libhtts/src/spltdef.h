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
#ifndef __SPLTDEF_H
#define __SPLTDEF_H

/**********************************************************/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/*
Copyright: 1994 - Grupo de Voz (DAET) ETSII/IT-Bilbao

Nombre fuente................ SPLTDEF.H
Nombre paquete............... SPL
Lenguaje fuente.............. C (Borland C/C++ 3.1)
Estado....................... Completado
Dependencia Hard/OS.......... definicion de tipos fundamentales
Codigo condicional........... -

Codificacion................. Borja Etxebarria

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
1.1.2    20/01/98  Borja     soporte MSVC++ (complex)
1.1.1    27/08/95  Borja     derivacion de tdef.h
1.1.0    08/12/94  Borja     revision general (tipos,idx,nel,tnel...)
1.0.0    06/12/94  Borja     Codificacion inicial.

======================== Contenido ========================
Definicion de tipos fundamentales para librerias SPL.

Por ejemplo:

SPL_PTR --> define el operador puntero (*)

SPL_INT --> tipo entero a utilizar en funciones SPL

SPL_pINT --> puntero a {SPL_INT}

Similar con SPL_FLOAT, SPL_VOID, etc.

En SPL se han usado asi los tipos:
SPL_INT .....  entero con signo
SPL_FLOAT ...  valor real
SPL_BOOL ....  valor booleano (SPL_TRUE, SPL_FALSE)
SPL_VOID ....  nulo
SPL_LONG ....  entero extendido; solo se usa en aquellos
.              raros casos en que es MUY evidente que un
.              INT puede sufrir overflow.
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include "tdef.h"

/**********************************************************/

#define SPL_PTR  PTR        /* Definicion de un puntero */

/**********************************************************/
/* Plantilla de definicion de un nuevo tipo SPL_NAME y puntero
al tipo (SPL_pNAME) (array), para el tipo TYPE. */

#define __SPL_TDEF(TYPE,NAME)  \
	typedef TYPE SPL_##NAME;  \
	typedef SPL_##NAME SPL_PTR SPL_p##NAME;

/**********************************************************/

__SPL_TDEF(INT, INT);   /* define SPL_INT, SPL_pINT */
__SPL_TDEF(DOUBLE, FLOAT);  /* define SPL_FLOAT, SPL_pFLOAT */
__SPL_TDEF(BOOL, BOOL);
__SPL_TDEF(VOID, VOID);
__SPL_TDEF(LONG, LONG);

#ifdef __cplusplus
__SPL_TDEF(COMPLEX, COMPLEX);  /* define SPL_COMPLEX, SPL_pCOMPLEX */
#endif

/**********************************************************/
/* valores booleanos */

#define SPL_FALSE  FALSE
#define SPL_TRUE  TRUE

/**********************************************************/

#endif

