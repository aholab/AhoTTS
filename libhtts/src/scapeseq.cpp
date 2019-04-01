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

Nombre fuente................ scapeseq.cpp
Nombre paquete............... normalizador
Lenguaje fuente.............. C++
Estado....................... desarrollo
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Juan Luis Murugarren

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
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
(ver scapesec.hpp)

Yon2.

===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

/**********************************************************/

#include "tdef.h"
#include "scapeseq.hpp"

#include "httsmsg.h"

/**********************************************************/

BOOL TextToList::isScapeSeq( CtI p)
{
	BOOL retval = TRUE;
	pCHAR pattern;
	CtI q;

#ifdef HTTS_DEBUG
        if (!ct(p).pattern) htts_error("TextToList::isScapeSec() - Trying to read null Pattern.");
#endif

	if (ct(p).pattern) pattern = strdup(ct(p).pattern);
	else return retval;

	//Buscamos patrones de tipo secuencia de escape válidos. "slnps"
	if (strlen(pattern) != 5) retval = FALSE;
	else if (strcmp(pattern, "slnps")) retval = FALSE;
	else {
		q = p;
		if (strlen(ct.getStr(q)) != 1 ) retval = FALSE;
		q = ct.next(q);
		if (strlen(ct.getStr(q)) != 1 ) retval = FALSE;
		q = ct.next(q);
		if (strlen(ct.getStr(q)) != 1 ) retval = FALSE;
		q = ct.next(q);
		if (strlen(ct.getStr(q)) != 1 ) retval = FALSE;
		q = ct.next(q);
		if (strlen(ct.getStr(q)) != 1 ) retval = FALSE;
	}

	//Chequeamos que los caracteres sean correctos.
	if (retval)	{
		q=p;
		if (ct.getStr(q)[0] != ']') retval = FALSE;
		q = ct.next(q);
		q = ct.next(q);
		q = ct.next(q);
		if (ct.getStr(q)[0] != ';') retval = FALSE;
		q = ct.next(q);
		if (ct.getStr(q)[0] != '[') retval = FALSE;

	}

	//Se podría verificar también que la letra corresponde a un
	//tipo de secuencia conocido y el rango del número.
	//De momento como sólo hay pausas na de na.

#ifdef DEBUG_SCA
#ifdef HTTS_DEBUG
        if (retval) htts_warn("TextToList::isScapeSec() - Scape sequence detected %s.", pattern);
#endif
#endif

	free(pattern);

	return retval;


}


/***********************************************************************/

CtI TextToList::markScapeSeq( CtI p)
{

#ifdef HTTS_DEBUG
        if (!ct(p).pattern) htts_error("TextToList::markScapeSec() - Trying to read null Pattern.");
#endif
	ct.setPattern(p, (char*)"q");
	ct.setEscSeq(p, TRUE);
	ct.setTnor(p, UTYPENOR_UNKGROUP);

	return (p);
}


/***********************************************************************/


