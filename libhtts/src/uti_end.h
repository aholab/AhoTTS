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
#ifndef __UTI_H
#define __UTI_H

/**********************************************************/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/*
Copyright: 1993 - Grupo de Voz (DAET) ETSII/IT-Bilbao

Nombre fuente................ uti.h
Nombre paquete............... AhoLib
Lenguaje fuente.............. C (BC31/GCC)
Estado....................... Utilizable
Dependencia Hard/OS.......... SI
Codigo condicional........... XFILE_???, STR???, etc

Codificacion................. Borja Etxebarria

Version  dd/mm/aa  Autor     Comentario
-------  --------  --------  ----------
2.2.4    21/04/00  Borja     fgetln_filt() -> comillas dobles (macro fgetlnf)
2.2.3    21/04/00  Borja     soporte DJGPP
2.2.2    06/08/97  Borja     bug en macros endian_???
2.2.1    06/08/97  Borja     bug en define XFILES_?? -> XFILE_??
2.2.0    22/07/97  Borja     show_srcpos()
2.1.0    22/07/97  Borja     little/big endian management
2.0.1    01/07/97  Borja     uti_math
2.0.0    01/05/97  Borja     mix de xfiles, syst, xsyst, fname, vstr...
1.2.0    31/07/96  Borja     die
1.1.0    30/07/96  Borja     ftrunc, fmoven, finsn, fdeln.
1.0.0    11/01/93  Borja     Codificacion inicial.

======================== Contenido ========================
<DOC>
Funciones para ficheros, cadenas, etc. ver los
ficheros fuentes UTI_????.C
</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "arch.h"
#include "tdef.h"

/**********************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************/
/* Manejo little/big endian */

void endian_swap16( VOID * word16 );
void endian_swap32( VOID * word32 );
void endian_swap64( VOID * word32 );
void endian_swap16_n( VOID * word16, int n );
void endian_swap32_n( VOID * word32, int n );
void endian_swap64_n( VOID * word32, int n );

#ifdef __LITTLE_ENDIAN__
#define endian_tolittle16(x) ((void)0)
#define endian_tolittle32(x) ((void)0)
#define endian_tolittle64(x) ((void)0)
#define endian_fromlittle16(x) ((void)0)
#define endian_fromlittle32(x) ((void)0)
#define endian_fromlittle64(x) ((void)0)
#define endian_tolittle16_n(x,n) ((void)0)
#define endian_tolittle32_n(x,n) ((void)0)
#define endian_tolittle64_n(x,n) ((void)0)
#define endian_fromlittle16_n(x,n) ((void)0)
#define endian_fromlittle32_n(x,n) ((void)0)
#define endian_fromlittle64_n(x,n) ((void)0)
#else
#define endian_tolittle16(w) endian_swap16(w)
#define endian_tolittle32(w) endian_swap32(w)
#define endian_tolittle64(w) endian_swap64(w)
#define endian_fromlittle16(w) endian_swap16(w)
#define endian_fromlittle32(w) endian_swap32(w)
#define endian_fromlittle64(w) endian_swap64(w)
#define endian_tolittle16_n(w,n) endian_swap16_n(w,n)
#define endian_tolittle32_n(w,n) endian_swap32_n(w,n)
#define endian_tolittle64_n(w,n) endian_swap64_n(w,n)
#define endian_fromlittle16_n(w,n) endian_swap16_n(w,n)
#define endian_fromlittle32_n(w,n) endian_swap32_n(w,n)
#define endian_fromlittle64_n(w,n) endian_swap64_n(w,n)
#endif

#ifdef __BIG_ENDIAN__
#define endian_tobig16(x) ((void)0)
#define endian_tobig32(x) ((void)0)
#define endian_tobig64(x) ((void)0)
#define endian_frombig16(x) ((void)0)
#define endian_frombig32(x) ((void)0)
#define endian_frombig64(x) ((void)0)
#define endian_tobig16_n(x,n) ((void)0)
#define endian_tobig32_n(x,n) ((void)0)
#define endian_tobig64_n(x,n) ((void)0)
#define endian_frombig16_n(x,n) ((void)0)
#define endian_frombig32_n(x,n) ((void)0)
#define endian_frombig64_n(x,n) ((void)0)
#else
#define endian_tobig16(w) endian_swap16(w)
#define endian_tobig32(w) endian_swap32(w)
#define endian_tobig64(w) endian_swap64(w)
#define endian_frombig16(w) endian_swap16(w)
#define endian_frombig32(w) endian_swap32(w)
#define endian_frombig64(w) endian_swap64(w)
#define endian_tobig16_n(w,n) endian_swap16_n(w,n)
#define endian_tobig32_n(w,n) endian_swap32_n(w,n)
#define endian_tobig64_n(w,n) endian_swap64_n(w,n)
#define endian_frombig16_n(w,n) endian_swap16_n(w,n)
#define endian_frombig32_n(w,n) endian_swap32_n(w,n)
#define endian_frombig64_n(w,n) endian_swap64_n(w,n)
#endif

/**********************************************************/

#ifdef __cplusplus
}  /* extern "C" */
#endif

/**********************************************************/

#endif
