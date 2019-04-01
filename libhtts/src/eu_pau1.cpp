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

Nombre fuente................ eu_pau1.cpp
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
Metodos de {{LangEU_Pauses}} que implementan el modelo "pau1" de
pausador. Es un modelo sencillo.
</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/


#include <stdlib.h>
#include <math.h>
#include "eu_lingp.hpp"
#include "xx_uti.hpp"
#include "httsmsg.h"

#ifdef HTTS_EU_PAU1

/**********************************************************/
/* aprox. debe haber una pausa cada pau_nsilab, pero
si hablamos rapido, seran mas silabas, por eso dividimos
entre el factor de estiramiento de duracion (durmean).
Ademas suponemos que aprox. una silaba son 2.2 letras
de media para hacer una estimacion tosca de las silabas a
partir de las palabras...
En resumen, aprox. cada {nchars} (=2.2*pau_nsilab/durmean)
letras, toca pausa.

Ademas se sustituyen de forma aleatoria algunas pausas
por breath, en funcion de la probabilidad de breath (
variable miembro {pbreath}), y el tipo de pausa. Habria
que tener en cuenta n. silabas desde ultimo breath para
que quedara mejor... */

VOID LangEU_Pauses::utt_pau1(UttWS & ut)
{
	// primero ponemos las pausas de sentencia normales.
	uttSentencePauses(ut);

	// Si au_nsilab se pone a 0, no se meten pausas automaticas.
	if (!pau_nsilab) return;

	// cada cuantas letras debe haber pausa?
	DOUBLE nchars= (2.2*pau_nsilab/(durmean?durmean:1));

	if (!(INT)nchars) return;

	Lix p, w;

	/* recorremos cada bloque de pausas actuales (son las de sentencia)
	y si son muy largos, insertamos mas pausas */
	for (p=ut.pausegrFirst(); p!=0; p=ut.pausegrNext(p)) {
		// contar letras
		INT nch=0;
		for (w=ut.wordFirst(p,URANGE_PAUSE); w!=0; w=ut.wordNext(w,URANGE_PAUSE)) {
			const CHAR *word=ut.cell(w).getWord(); assert(word);
			if (word) nch += strlen(word);
		}
		if (nch > (INT)(nchars*1.5)) { // si es un segmento muy largo insertamos pausas
			INT i = (INT)(nch/nchars); // n. pausas que entran (redondeado hacia abajo)
			if (!i) i=1;
			INT blkch= (INT)(nch/i); // cada cuantos chars metemos pausa
			INT nch=0;
			for (w=ut.wordFirst(p,URANGE_PAUSE); w!=0; w=ut.wordNext(w,URANGE_PAUSE)) {
				if (nch>=blkch) {  // si hay suficientes letras en las palabras acumuladas, metemos pausa
					nch=0;
					if (ut.cell(w).getPause()==UPAUSE_NONE) ut.cell(w).setPause(UPAUSE_SHORT);  // ponemos pausa!
				}
				const CHAR *word=ut.cell(w).getWord(); assert(word);
				if (word) nch += strlen(word);  // vamos acumulando letras de palabras
			}
		}
	}

	/* recorremos todas las pausas, y aleatoriamente metemos algun
	breath */

	for (p=ut.pausegrFirst(); p!=0; p=ut.pausegrNext(p)) {
		Pause pau=ut.cell(p).getPause();
		DOUBLE prob=0;
		switch (pau) {  // seleccionamos probabilidad base (multiplica a {pbreath})
			case UPAUSE_UBEGIN: prob=0.9; break;
			case UPAUSE_UEND: prob=0; break;
			case UPAUSE_SHORT: prob=0.5; break;
			case UPAUSE_LONG: prob=0.8; break;
			case UPAUSE_SDECL: prob=0.6; break;
			case UPAUSE_SQUEST: prob=0.6; break;
			case UPAUSE_SEXCL: prob=0.6; break;
			case UPAUSE_SPAUSE: prob=0.9; break;
			case UPAUSE_SNEUTR: prob=0.6; break;
			default: prob=0; break;
		}

		// loteria
		if (rand()<pbreath*prob*RAND_MAX)  ut.cell(p).setPause(-pau);
	}

}

/**********************************************************/
#endif
