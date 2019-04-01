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
Copyright: 1995 - Grupo de Voz (DET) ETSII/IT-Bilbao

Nombre fuente................ wrkbuff.c
Nombre paquete............... -
Lenguaje fuente.............. C (Borland C/C++ 3.1)
Estado....................... en desarrollo
Dependencia Hard/OS.......... -
Codigo condicional........... NDEBUG, NOWRKBUFF

Codificacion................. Borja Etxebarria

Version  dd/mm/aa  Autor     Comentario
-------  --------  --------  ----------
1.0.0    07/06/95  Borja     Codificacion inicial.

======================== Contenido ========================
<DOC>
Para usar un buffer de trabajo global y no tener que
andar haciendo mallocs/free, ni chupando mucha pila.

En este modulo simplemente se define una variable global
que se llama {_wrkbuff} y que es un array de CHARs de
longitud WRKBUFFLEN.

Cuando una funcion quiere usar un buffer temporal, como
suele ser normal al hacer operaciones con cadenas de
texto, podemos usar este buffer {_wrkbuff}, siempre que sea
suficientemente grande para las necesidades de la funcion.
Nos ahorramos asi el overhead de malloc/free y el chupar
demasiado espacio de pila con variables automaticas.

Conviene que la funcion haga WRKBUFF(n) justo despues de
definir las variables automaticas (es decir, ponerlo como
primera sentencia a ejecutar dentro de la funcion).
WRKBUFF(n) va a asegurarse de que el buffer global {_wrkbuff}
tenga por lo menos {n} elementos. De no ser asi, el programa
se interrumpe (se utiliza assert()). La longitud del
buffer {_wrkbuff} se selecciona a la hora de compilar, en el
fichero WRKBUFF.H, definido como WRKBUFFLEN.

Si se define NDEBUG, la sentencia assert() desaparece, por
lo que no se comprueba que {_wrkbuff} sea suficientemente
grande para las necesidades de la funcion!!! Conviene definir
NDEBUG solo al final del desarrollo, cuando estamos seguros de
que todo nuestro codigo es estable.

Si se define NOWRKBUFF, la variable global {_wrkbuff} no se
define, y la sentencia WRKBUFF(n) se sustituye por una
definicion automatica de la variable {_wrkbuff} en la pila:

CHAR _wrkbuff[n]

es decir, se usa una cadena local a la funcion en vez de
un buffer global al programa.

IMPORTANTE:  no modificar NUNCA el puntero {_wrkbuff}, por
ejemplo, no hacer _wrkbuff++ ni cosas de esas!!.
</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#define __WRKBUFF_C__

#include "wrkbuff.h"

/**********************************************************/
/* Define el buffer de trabajo, excepto si NOWRKBUFF esta
definido, en cuyo caso no se usa un buffer global, sino que
se usaran variables locales automaticas (en pila). */

#ifndef NOWRKBUFF

PUBLIC CHAR __wrkbuff[WRKBUFFLEN];
#ifdef __OS_MSDOS__
PUBLIC CHAR _far *_wrkbuff=__wrkbuff;
#else
PUBLIC CHAR *_wrkbuff=__wrkbuff;
#endif

#ifndef NDEBUG
PUBLIC BOOL _wrkbuff_used=FALSE;
#endif

#endif

/**********************************************************/
