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
#ifndef __SCAPESEQ_HPP__
#define __SCAPESEQ_HPP__

/**********************************************************/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/*
Copyright: 1998 - TSR/Aholab (DET) ETSII/IT-Bilbao

Nombre fuente................ scapeseq.hpp
Nombre paquete............... normalizador
Lenguaje fuente.............. C++
Estado....................... desarrollo
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Juan Luis Murugarren

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
0.0.1    21/06/01  Yon2.     insertPuase added.
0.0.0    18/06/01  Yon2.     Codificacion inicial.

======================== Contenido ========================

Soporte para secuencias de escape sólo en el normalizador.

El formato para las secuencias de escape es:
]C0;[
El patron que genera esta secuencia en WordChop, el troceador
de palabras es:
slnps -> símbolo letra numero puntuación símbolo, todas ellas
tienen longitud uno para que la detección sea más sencilla
y para que no se confundan con posibles secuencias en el
texto original.

- Los corchetes y el punto y coma no tienen ningún significado.
- La letra indica el tipo de secuencia de escape y el número
es un parámetro para el tipo de secuencia de escape.
- Si el número es cero, indica el final de la secuencia de
escape. En generar habrá marcas de apertura y cierre de
secuencia de escape. Estas marcas afectarán a todo el texto
contenido entre ellas.

Marcas Soportadas:

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

En principio los tipos de pausa de la 3 a la 9 no tienen
mucho sentido en este módulo.

Se insertarán pausas de tipo 1 o de tipo 2 preferiblemente.

Yon2.

===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

/**********************************************************/

#include "tdef.h"
#include "t2l.hpp"
#include "scapedo.hpp"
#include "uttws.hpp"

/**********************************************************/

BOOL insertPause(UttWS *ws, UttI p, INT value);


/***********************************************************************/

#endif
