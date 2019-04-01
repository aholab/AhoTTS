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
Copyright: 1996 - Grupo de Voz (DAET) ETSII/IT-Bilbao

Nombre fuente................ -
Nombre paquete............... -
Lenguaje fuente.............. C++ (Borland C/C++ 3.1)
Estado....................... desarrollo
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Borja Etxebarria

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
1.0.1    09/04/99  Borja     uso tipos UINT
0.0.0    30/07/96  Borja     codificacion inicial.

======================== Contenido ========================
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include "uti.h"
#include "caudiox.hpp"

/**********************************************************/
/* Lee la cabecera. La posicion en el fichero es indeterminada,
al terminar de leer la cabecera debe dejarse el fichero en la
posicion de la primera muestra. {devuelve} el n. de muestras */

LONG AFRaw::HdrR( FILE* f, KVStrList &def, BOOL override )
{
	/* el tipo de muestra y la frecuencia de muestreo, se dejan
	por defecto (previamente definidas, o variables de entorno, o
	valor por defecto compilado). */
	ADDIFNOV(CAUDIO_SRATE,def.dval(CAUDIO_SRATE,8000));
	ADDIFNOV(CAUDIO_SAMPTYPE,def.val(CAUDIO_SAMPTYPE,SAMPTYPE_STR_PCM16));
	ADDIFNOV(CAUDIO_NCHAN,def.lval(CAUDIO_NCHAN,1));
	// endian por defecto
	ADDIFNOV(CAUDIO_BIGENDIAN,def.val(CAUDIO_BIGENDIAN,CAUDIO_BIGENDIAN_DEFAULT?"yes":"no"));

	xfseek(f,0,SEEK_SET);  // posiciona en la primera muestra
	return -1;  // auto
}

/**********************************************************/
/* Se inventa y rellena los campos que quiera de una cabecera
por defecto para un fichero nuevo. */

VOID AFRaw::HdrG( KVStrList &def, BOOL override )
{
	ADD(CAUDIO_NSAMPLES,0);
	ADDIFNOV(CAUDIO_SRATE,def.dval(CAUDIO_SRATE,8000));
	ADDIFNOV(CAUDIO_SAMPTYPE,def.val(CAUDIO_SAMPTYPE,SAMPTYPE_STR_PCM16));
	ADDIFNOV(CAUDIO_NCHAN,def.lval(CAUDIO_NCHAN,1));

  // endian por defecto
	ADDIFNOV(CAUDIO_BIGENDIAN,def.val(CAUDIO_BIGENDIAN,CAUDIO_BIGENDIAN_DEFAULT?"yes":"no"));
}

/**********************************************************/
/* Escribe la cabecera. La posicion inicial en el fichero esta
indefinida.
Una vez escrita la cabecera, debe ir a la primera muestra
de audio. */

VOID AFRaw::HdrW( FILE* f, LONG )
{
	xfseek(f,0,SEEK_SET);  // vamos a la primera muestra
}

/**********************************************************/

BOOL AFRaw::testFile( FILE * )
{
	return FALSE;  // un raw no es detectable
}

/**********************************************************/
