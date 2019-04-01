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
#ifndef __WRKBUFF_H__
#define __WRKBUFF_H__

/**********************************************************/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/*
Copyright: 1995 - Grupo de Voz (DET) ETSII/IT-Bilbao

Nombre fuente................ wrkbuff.h
Nombre paquete............... -
Lenguaje fuente.............. C (Borland C/C++ 3.1)
Estado....................... en desarrollo
Dependencia Hard/OS.......... -
Codigo condicional........... NDEBUG, NOWRKBUFF

Codificacion................. Borja Etxebarria

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
0.0.0    07/06/95  Borja     Codificacion inicial.

======================== Contenido ========================
Cabecera de WRKBUFF.C.
Para usar un buffer de trabajo global.
Ver comentarios sobre utilizacion en WRKBUFF.C.
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include "tdef.h"
#include <assert.h>

/**********************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************/
/* esta es la longitud del buffer global. */

#define WRKBUFFLEN 10240

/**********************************************************/

/* definir NOWRKBUFF para NO utilizar un buffer global */

#define NOWRKBUFFxx

/**********************************************************/

#ifdef NOWRKBUFF   /* NOWRKBUFF esta definido */

/* usamos buffer automatico en pila */
#define WRKBUFF(n) CHAR _wrkbuff[(n)]
#define WRKBUFF_FREE() {}

#else  /* NOWRKBUFF NO esta definido */

/* asi que usamos buffer global {_wrkbuff}. WRKBUFF(n) comprueba
que {_wrkbuff} tiene elementos suficientes. */
#ifdef NDEBUG
#define WRKBUFF(n) {}
#define WRKBUFF_FREE() {}
#else
#define WRKBUFF(n) { assert((n)<=WRKBUFFLEN); assert(!_wrkbuff_used); _wrkbuff_used=TRUE; }
#define WRKBUFF_FREE() { assert(_wrkbuff_used); _wrkbuff_used=FALSE; }
#endif

#ifndef __WRKBUFF_C__

#ifdef __OS_MSDOS__
EXTERN CHAR _far *_wrkbuff;
#else
EXTERN CHAR *_wrkbuff;
#endif

#ifndef NDEBUG
EXTERN BOOL _wrkbuff_used;
#endif
#endif  /* __WRKBUFF_C */

#endif  /* NOWRKBUFF */

/**********************************************************/

#ifdef __cplusplus
}
#endif

/**********************************************************/

#endif
