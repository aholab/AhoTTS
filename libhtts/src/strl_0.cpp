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
Copyright: 1996 - Grupo de Voz (DET) ETSII/IT-Bilbao

Nombre fuente................ STRL.CPP
Nombre paquete............... -
Lenguaje fuente.............. C++
Estado....................... Completado
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Borja Etxebarria

Version  dd/mm/aa  Autor     Comentario
-------  --------  --------  ----------
1.2.5    30/08/98  Borja     split en varios modulos strl_?.cpp

======================== Contenido ========================
Ver strl.cpp para doc.
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include <stdio.h>

#include "strl.hpp"
#include "uti.h"
#include "wrkbuff.h"

/**********************************************************/

INT _strl_cmp(const String *s1, const String *s2);
INT _strl_rcmp(const String *s1, const String *s2);

/**********************************************************/

VOID StrList::append_s(const char *words )
{
	static const char *separator=" \t\n";

	if (!words) return;

	size_t l;
	while (words) {
		words += strspn(words,separator);
		if (!(*words)) break;
		BOOL cute=FALSE;
		if (*words=='"') {
			words++;
			cute=TRUE;
			l=strcspn(words,"\"");
		}
		else l = strcspn(words,separator);
		if (l) append(String(words,l));
		words+=l;
		if (cute && (*words)) words++;
	}
}

/**********************************************************/

VOID StrList::append_sf(const char *fwords, ... )
{
	va_list argptr;
	va_start(argptr, fwords);
	append_s(fwords, argptr);
	va_end(argptr);
}

/**********************************************************/

VOID StrList::append_s(const char *fwords, va_list v )
{
	WRKBUFF(2048);
	if (!fwords) return;
	vsprintf(_wrkbuff, fwords, v);
	append_s(_wrkbuff);
	WRKBUFF_FREE();
}

/**********************************************************/

VOID StrList::append_s( const char *wordarray[] )
{
	if (!wordarray) return;
	while (*wordarray) append(String(*(wordarray++)));
}

/**********************************************************/

VOID StrList::sort( BOOL reverse )
{
	if (reverse) sortf(_strl_rcmp);
	else sortf(_strl_cmp);
}

/**********************************************************/

BOOL StrList::OK ( VOID ) const
{
	if (!(l.OK())) return FALSE;
	for (Lix p=first(); p!=0; p=next(p)) if (!item(p).OK()) return FALSE;
	return TRUE;
}

/**********************************************************/

std::ostream& operator << (std::ostream &st, const StrList &l)
{
	for (Lix p=l.first(); p!=0; p=l.next(p))
		st<< "\"" << l(p) << "\"" << std::endl;
	return st;
}

/**********************************************************/
