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
(C) 1997 TSR/Aholab - ETSII/IT Bilbao (UPV/EHU)

Nombre fuente................ httsmsg.c
Nombre paquete............... aHoTTS
Lenguaje fuente.............. C++
Estado....................... -
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Borja Etxebarria
.............................

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
1.0.0    31/01/00  borja     codefreeze aHoTTS v1.0
0.0.0    24/11/97  borja     Codificacion inicial.

======================== Contenido ========================
Gestion de mensajes de error, warning, debug.
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include <stdio.h>
#include <stdlib.h>

#define __HTTSMSG_NOEXTERNS__
#include "httsmsg.h"

/**********************************************************/

HTTSMsgFunc * __htts_error=NULL;
HTTSMsgFunc * __htts_warn=NULL;

/**********************************************************/

VOID htts_set_error_func( HTTSMsgFunc *f )
{
	__htts_error=f;
}

/**********************************************************/

VOID htts_set_warn_func( HTTSMsgFunc *f )
{
	__htts_warn=f;
}

/**********************************************************/

VOID htts_error( const CHAR *fmt, ... )
{
	va_list argptr;
	va_start(argptr, fmt);

	if (__htts_error) __htts_error(fmt,argptr);
	else {
		fprintf(stderr,"[aHoTTS error]: ");
		vfprintf(stderr, fmt, argptr);
		fprintf(stderr,"\n");
	}

	va_end(argptr);
	exit(1);
}

/**********************************************************/

VOID htts_cerror( INT condition, const CHAR *fmt, ... )
{
	if (condition) {
		va_list argptr;
		va_start(argptr, fmt);

		if (__htts_error) __htts_error(fmt,argptr);
		else {
			fprintf(stderr,"[aHoTTS error]: ");
			vfprintf(stderr, fmt, argptr);
			fprintf(stderr,"\n");
		}

		va_end(argptr);
		exit(1);
	}
}

/**********************************************************/

VOID htts_warn( const CHAR *fmt, ... )
{
	va_list argptr;
	va_start(argptr, fmt);

	if (__htts_warn) __htts_warn(fmt,argptr);
	else {
		fprintf(stderr,"[aHoTTS warn]: ");
		vfprintf(stderr, fmt, argptr);
		fprintf(stderr,"\n");
	}

	va_end(argptr);
}

/**********************************************************/

#ifdef HTTS_DEBUGxxx
VOID htts_debug( INT lvl, const CHAR *fmt, ... )
{
	va_list argptr;

	fprintf(stderr,"[aHoTTS debug %ld]: ",(long)lvl);
	va_start(argptr, fmt);
	vfprintf(stderr, fmt, argptr);
	va_end(argptr);
	fprintf(stderr,"\n");
}
#endif

/**********************************************************/

