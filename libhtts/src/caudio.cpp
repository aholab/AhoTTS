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

Nombre fuente................ caudio.cpp
Nombre paquete............... caudio
Lenguaje fuente.............. C++ (Borland C/C++ 3.1)
Estado....................... desarrollo
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Borja Etxebarria

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
1.0.1    09/04/99  Borja     uso tipos UINT
0.0.0    01/07/97  Borja     codificacion inicial.

======================== Contenido ========================
Funcioncillas sobre objetos CAudio en general
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include <stdlib.h>
#include "caudiox.hpp"

/**********************************************************/

VOID CAudio::minmax( LONG len, DOUBLE *min, DOUBLE *max )
{
	DOUBLE d, mi, ma;
	LONG i;

	mi=ma=0;

	if (len>0) {
		get(d);
		ma=mi=d;
		for (i=1; i<len; i++) {
			get(d);
			if (d>ma) ma=d;
			if (d<mi) mi=d;
		}
	}
	if (min) *min=mi;
	if (max) *max=ma;
}

/**********************************************************/

VOID CAudio::minmax( LONG len, INT16 *min, INT16 *max )
{
	INT16 d, mi, ma;
	LONG i;

	mi=ma=0;

	if (len>0) {
		get(d);
		ma=mi=d;
		for (i=1; i<len; i++) {
			get(d);
			if (d>ma) ma=d;
			if (d<mi) mi=d;
		}
	}
	if (min) *min=mi;
	if (max) *max=ma;
}

/**********************************************************/

LONG CAudio::maxfr( UINT framelen, UINT winlen, LONG nframes )
{
	LONG pos0 = getPos();
	INT ofs;
	LONG max, posmax;

	if (nframes<0) {
		LONG len=getNSamples()-pos0;
		nframes=(LONG)((len+framelen-1)/framelen);
	}

	if (framelen<=winlen)
		ofs = -((INT)(winlen-framelen+1)/2);
	else
		ofs = ((INT)(framelen-winlen)/2);

	max=0;
	posmax=0;
	for ( LONG fr=0; fr<nframes; fr++) {
		setPos(pos0+fr*framelen+ofs);
		INT16 *v = get_i16(winlen);
		LONG sum=0;
		for (UINT i=0; i<winlen; i++)
			sum += abs(v[i]);
		if (sum>max) {
			max=sum;
			posmax = fr;
		}
	}
	return posmax;
}


/**********************************************************/
