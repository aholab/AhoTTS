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

Nombre fuente................ t2u_do.cpp
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
0.1.0    --/--/97  borja     codificacion inicial

======================== Contenido ========================
<DOC>
modulo de paso de texto a lista Utt
</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include "uti.h"
#include "chset.h"
#include "t2u.hpp"

/**********************************************************/

BOOL istext(CHAR ch)
{
	CHAR *masletras = (char*)CS_S_atilde CS_S_etilde CS_S_itilde
		CS_S_otilde CS_S_utilde CS_S_uuml CS_S_ntilde;

	if ((ch >= 'a') && (ch <= 'z'))
		return TRUE;
	if (strchr(masletras, ch))
		return TRUE;
	return FALSE;
}

/**********************************************************/

VOID T2ULst::reset( VOID )
{
	buffin[0] = '\0';
	punc = 0;
	sentence = 0;
}

/**********************************************************/

INT T2ULst::entrada_cadena(const CHAR * str)
{
	INT pos;
	UttI p;
	BOOL inword;	// estado (TRUE si estamos dentro de palabra)
	INT wstart;		// comienzo de la palabra actual
	BOOL eou;	// marca fin de utterance

	inword = (buffin[0] != '\0');  //si teniamos un trozo de palabra

	if (!str) {  // flush!
		if (inword) {
			p = uttappend(buffin, "", 0, 0);  // salva posibles despojos
			if (!sentence)
				sentence = p;  // coge frase si no la teniamos
		}
		if (!punc)  // puntuacion por defecto: punto
			punc = '.';
		updatepunc(sentence, punc);	// actualiza punc de frase
		if (sentence)
			uttcompleta = TRUE;  // siempre que realmente tengamos algo, tenemos una utt completa!!
		punc = 0;  // reinicializamos para proxima utt
		sentence = 0;
		return 0;
	}

	eou = FALSE;
	pos = 0;
	wstart = 0;
	while ((!eou) && (str[pos])) {
		if (istext(str[pos])) {		// es una letra?
			if (!inword) {	// si encontramos palabra nueva
				wstart = pos;	// salvamos posicion de comienzo
				inword = TRUE;	// marca que estamos en una palabra
				if (punc) {		// si antes ha habido algun signo de puntuacion
					updatepunc(sentence, punc); // actualiza sintagma anterior
					punc = 0;	// y empezamos una nueva
					sentence = 0;
				}
			}
		}
		else {	// es un espacio o signo de puntuacion
			punc = selectpunc(punc, str[pos], &eou);	// refresca punc
			if (inword) {	// si estabamos en palabra => fin palabra
				p = uttappend(buffin, str, wstart, pos);	// salva palabra
				if (!sentence)
					sentence = p;	// recuerda frase si no la teniamos ya
				inword = FALSE;		// marca que ya no estamos en una palabra
			}
		}
		pos++;	// sig. caracter
	}

	if (inword) {	// si hemos quedado dentro de una palabra
		INT lb = strlen(buffin);
		if (lb < T2ULST_MAXWORDLEN) {	// si se puede, salva lo que entre
			INT li = strlen(str + wstart);
			if (li > T2ULST_MAXWORDLEN - lb)
				li = T2ULST_MAXWORDLEN - lb;
			strncpy(buffin + lb, str + wstart, li);
			buffin[lb + li] = '\0';
			pos = wstart + li;
		}
		else {	// si no se puede salvar mas ==> palabra dividida!!!
			p = uttappend(buffin, str, wstart, pos);	// salva "cachopalabra"
			if (!sentence)
				sentence = p;	// recuerda frase si no la teniamos ya
		}
	}
	else if (eou) {
		updatepunc(sentence, punc);		// actualiza punc de ultima frase
		if (sentence)
			uttcompleta = TRUE;	 // siempre que realmente tengamos algo, tenemos una utt completa!!
		punc = 0;	// reinicializamos para proxima utt
		sentence = 0;
	}

	return pos;
}

/**********************************************************/

UttI T2ULst::uttappend(CHAR * buffin, const CHAR * str, INT strstart,
	INT strendplus1)
{
	INT lb = strlen(buffin);
	INT n = strendplus1 - strstart;

	if (n + lb == 0)
		return 0;

	/* formar palabra {buffin}+{cacho_de_str} */
	CHAR *s = (CHAR *) malloc(sizeof(CHAR) * (lb + n + 1));
	strcpy(s, buffin);
	if (n)
		strncpy(s + lb, str + strstart, n);
	s[lb + n] = '\0';

	/* anyadir la palabra a la utterance */
	UttI c = utt->cellAppend();
	utt->cell(c).setWord(s);
	free(s);
	buffin[0] = '\0';

	return c;
}

/**********************************************************/

VOID T2ULst::updatepunc(UttI sentc, INT punc)
{
	if (!sentc) return;
	Sentence s;

	switch (punc) {
	case '.': s=USENTENCE_DECL; break;
	case '?': s=USENTENCE_QUEST; break;
	case '!': s=USENTENCE_EXCL; break;
	case ':': s=USENTENCE_DECL; break;
	case ';': s=USENTENCE_DECL; break;
	case ',': s=USENTENCE_PAUSE; break;
	case '(': s=USENTENCE_PAUSE; break;
	case ')': s=USENTENCE_PAUSE; break;
	case '-': s=USENTENCE_PAUSE; break;
	case CS_iquest: s=USENTENCE_PAUSE; break;
	case CS_iexcl: s=USENTENCE_PAUSE; break;
	default:
		/* Lo normal es no llegar aqui, salvo en modo palabra,
		en el que los espacios marcan fin de 'frase' y se pone
		entonacion neutra. */
		if (txtMode==ModeWord) s=USENTENCE_NEUTR;
		else s=USENTENCE_NONE;
		break;
	}

	utt->cell(sentc).setSentence(s);
}

/**********************************************************/

INT T2ULst::selectpunc(INT punc, INT newpunc, INT * eof)
{
	// todos ordenados por prioridad (para machacarse mutuamente si hace falta)
	CHAR *sortedall = (char *)".?!:;,()-" CS_S_iquest CS_S_iexcl;
	// los que provocan fin de utterance
	CHAR *endofutt = (char *)".:;!?" CS_S_iexcl CS_S_iquest;
	CHAR *s;

	s = strchr(sortedall, newpunc);  // a ver si {newpunc} vale
	if (s && strchr(s, punc))  // si vale y es mas prioritario que {punc}
		punc = newpunc;

	/* modo palabra: cualquier signo es eof. Puntuacion neutra (' ')
	en caso de no haber otra valida. */
	if (txtMode==ModeWord) {
		if (!punc) punc=' ';
		*eof = TRUE;
	}
	else  /* detectar eof en otros modos */
		*eof = punc && (strchr(endofutt, punc) != NULL);
	return punc;
}

/**********************************************************/
