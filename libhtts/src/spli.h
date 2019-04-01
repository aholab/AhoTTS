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
#ifndef __SPLI_H
#define __SPLI_H

/**********************************************************/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/*
Copyright: 1994 - Grupo de Voz (DAET) ETSII/IT-Bilbao

Nombre fuente................ SPLI.H
Nombre paquete............... SPL
Lenguaje fuente.............. C (Borland C/C++ 3.1)
Estado....................... Completado
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Borja Etxebarria

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
1.1.1    30/07/95  Borja     scope funciones explicito
1.1.0    08/12/94  Borja     revision general (tipos,idx,nel,tnel...)
1.0.0    06/07/93  Borja     Codificacion inicial.

======================== Contenido ========================
Definiciones y macros de uso interno comun a todos los
modulos SPL.

Si NDEBUG NO esta definido, todos los chequeos assert() se activan,
reduciendo un poco la velocidad de las funciones ya que se chequea
la validez de los parametros, pero es mas seguro en fase de
desarrollo. Si se pasa algun parametro mal, el programa casca,
informando del fichero fuente y de la linea en la que se detecto
la anomalia.

Si NDEBUG SI se define, los chequeos se desconectan. Si se envian
a las funciones SPL valores fuera de los rangos indicados, no
funcionaran correctamente (resultados indefinidos, escritura fuera
de vectores...

El define NDEBUG es C estandar (assert.h), y puede haber otros modulos
que lo utilicen. Si se define globalmente, afectara a todos ellos y no
solo a los modulos SPL?.C
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "spl.h"

/**********************************************************/
/* suma en {k}, desde {from} hasta {to}, la expresion {expr} acumulando
el sumatorio en {sum}, que se inicializa a {isum} */

#define __sum(k,from,to,expr,sum,isum)  {  \
	(sum) = (isum);  \
	for ((k)=(from); (k)<=(to); (k)++)  \
		(sum) += (expr);  \
}

/**********************************************************/
/* suma en {k}, desde {from} hasta el elemento anterior a {upto}, la
expresion {expr} acumulando el resultado en {sum}, que se inicializa
a {isum} */

#define __xsum(k,from,upto,expr,sum,isum)  {  \
	(sum) = (isum);  \
	for ((k)=(from); (k)<(upto); (k)++)  \
		(sum) += (expr);  \
}

/**********************************************************/
/* resta en {k}, desde from hasta {to}, la expresion {expr} acumulando
el sumatorio en {sub}, que se inicializa a {isub} */

#define __sub(k,from,to,expr,sub,isub)  {  \
	(sub) = (isub);  \
	for ((k)=(from); (k)<=(to); (k)++)  \
		(sub) -= (expr);  \
}

/**********************************************************/
/* resta en {k}, desde {from} hasta el elemento anterior a {upto}, la
expresion {expr} acumulando el resultado en {sub}, que se inicializa
a {isub} */

#define __xsub(k,from,upto,expr,sub,isub)  {  \
	(sub) = (isub);  \
	for ((k)=(from); (k)<(upto); (k)++)  \
		(sub) -= (expr);  \
}

/**********************************************************/
/* intercambia {a} y {b}. {tmp} es una variable temporal
del mismo tipo que {a} y {b} */

#define __swap(a,b,tmp) { (tmp)=(a); (a)=(b); (b)=(tmp); }

/**********************************************************/

#endif

