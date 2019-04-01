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

Nombre fuente................ eu_emph.cpp
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
<DOC>
Metodos de {{LangEU_Prosod}} que buscan los puntos de enfasis en los
que se debe poner el acento prosodico.
</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include <string.h>

#include "chset.h"
#include "eu_lingp.hpp"

/**********************************************************/
// $$$ buscar mas palabritas de estas...
// $$$ habria que usar el campo POS en vez de buscar en {atonas}...

PRIVATE CHAR *atonas[] =
{
	(char *)"el", (char *)"la", (char *)"lo", (char *)"los", (char *)"las",
	(char *)"un", (char *)"una", (char *)"unos", (char *)"unas",

	(char *)"mi", (char *)"tu", (char *)"su", (char *)"mis", (char *)"tus", (char *)"sus",
	(char *)"nuestra", (char *)"vuestra", (char *)"nuestras", (char *)"vuestras",
	(char *)"nuestro", (char *)"vuestro", (char *)"nuestros", (char *)"vuestros",

	(char *)"me", (char *)"te", (char *)"le", (char *)"nos", (char *)"os", (char *)"les", (char *)"se",
	(char *)"al", (char *)"del",

	(char *)"a", (char *)"ante", (char *)"bajo", (char *)"cabe", (char *)"con", (char *)"contra", (char *)"de",
	(char *)"desde", (char *)"en", (char *)"entre", (char *)"hacia", (char *)"hasta", (char *)"para", (char *)"por",
	(char *)"sin", (char *)"sobre", (char *)"tras", (char *)"mediante",

	(char *)"y", (char *)"e", (char *)"ni", (char *)"mas", (char *)"o", (char *)CS_S_otilde, (char *)"u", (char *)"pero", (char *)"aunque", (char *)"si",
	(char *)"porque", (char *)"que", (char *)"quien", (char *)"cuando", (char *)"como", (char *)"donde", (char *)"cual", (char *)"cuan",
	(char *)"aun", (char *)"pues", (char *)"tan", (char *)"mientras", (char *)"sino", (char *)"cada",

	NULL
};

/**********************************************************/

PRIVATE BOOL is_atona(const CHAR * word)
{
	CHAR **s = atonas;

	while (*s) {
		if (!strcmp(word, *s)) return TRUE;
		s++;
	}
	return FALSE;
}

/**********************************************************/

PRIVATE VOID word_emphasis(UttPh & u, UttI wordp)
{
	UttI w, p;

	w = u.cellFirst(wordp, URANGE_WORD); // vamos a la primera celda de la palabra

	// a las palabras atonas no les ponemos acento prosodico
	if (is_atona(u.cell(w).getWord())) return;

	// buscamos los fonemas acentuados (vocal) de esta palabra.
	// Normalmente solo habra uno, pero hacemos que puede funcionar con varios
	for (p=u.phoneFirst(w,URANGE_WORD); p!=0; p=u.phoneNext(p,URANGE_WORD)) {
		if (u.cell(p).getStress() != USTRESS_NONE)
			u.cell(p).setEmphasis(UEMPHASIS_STRESS);  // marcamos el punto de acento prosodico
	}
}

/**********************************************************/
/* Recorre la lista de palabras, marcando los focos de enfasis: todas
las palabras salvo las atonas. */

VOID LangEU_Prosod::utt_emphasis(UttPh & ut)
{
	for (UttI p=ut.wordFirst(); p!=0; p=ut.wordNext(p)) word_emphasis(ut,p);
}

/**********************************************************/
