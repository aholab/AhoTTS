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

Nombre fuente................ xx_uti.cpp
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
</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include "xx_uti.hpp"
#include "uti.h"

/**********************************************************/

BOOL phSetContains(Phone * lset, Phone ph)
{
	while (*lset) {
		if (*lset == ph) return TRUE;
		lset++;
	}
	return FALSE;
}

/**********************************************************/
/* Interpola los valores de pitch linealmente entre los puntos
definidos (en los puntos indefinidos el pitch vale {FillValue}). */

VOID uttPthLin( UttPh &ut )
{
	UttI sentfirst=ut.sentenceFirst();
	UttI sentlast=ut.sentenceLast();
	for (UttI sentp=sentfirst; sentp!=0; sentp=ut.sentenceNext(sentp)) {
		UttI p;

		//buscamos primer fonema con pitch
		for (p=ut.phoneFirst(sentp,URANGE_SENTENCE); p!=0; p=ut.phoneNext(p,URANGE_SENTENCE))
			if (ut.cell(p).getPth()!=FillValue) break;

		if (!p) {// si no hay, ponemos valores medios
			for (p=ut.phoneFirst(sentp,URANGE_SENTENCE); p!=0; p=ut.phoneNext(p,URANGE_SENTENCE))
				ut.cell(p).setPth(0);  // valor medio en escala log2
			continue;
		 }

		UttI first = p;

		//ponemos pitch constante=inicial a todo lo que este delante del primer valor con pitch
		for (p=ut.phoneFirst(sentp,(sentp==sentfirst)?(INT)URANGE_UTT:(INT)URANGE_SENTENCE); p!=first; p=ut.phoneNext(p))
			ut.cell(p).setPth(ut.cell(first).getPth());

		//buscamos ultimo fonema con pitch
		for (p=ut.phoneLast(sentp,URANGE_SENTENCE); p!=0; p=ut.phonePrev(p,URANGE_SENTENCE))
			if (ut.cell(p).getPth()!=FillValue) break;
		assert(p);  // tiene que haber algun fonema con pitch si hemos llegado aqui.

		UttI last = p;

		//ponemos pitch constante=final a todo lo que este detras del ultimo valor con pitch
		for (p =ut.phoneLast(sentp,(sentp==sentlast)?(INT)URANGE_UTT:(INT)URANGE_SENTENCE); p!=last; p=ut.phonePrev(p))
			ut.cell(p).setPth(ut.cell(last).getPth());

		// para cada {beg} (fonema con valor de pitch)
		for (UttI beg = first; beg != last;) {
			UttI end;
			LONG n, N;

			// buscamos siguiente punto con valor de pitch
			for (end=ut.phoneNext(beg,URANGE_SENTENCE), N=1; end!=0; end=ut.phoneNext(end,URANGE_SENTENCE), N++)
				if (ut.cell(end).getPth()!=FillValue) break;
			assert(end); // tiene que haber algun fonema con pitch si hemos llegado aqui.

			// para cada fonema entre beg y end, interpola pitch linealmente.
			for (p=ut.phoneNext(beg,URANGE_SENTENCE), n=1; p!=end; p=ut.phoneNext(p,URANGE_SENTENCE), n++)
				ut.cell(p).setPth(linterpol(n,0,N, ut.cell(beg).getPth(), ut.cell(end).getPth()));

			beg = end;	// siguiente fonema con pitch pasa a ser el actual
		}
	}
}

/**********************************************************/
/* insertamos fonemas de silencio en pausas (entre sentences,
etc etc. ), y traslada la frontera de pausa a estos fonemas.

La ultima frontera de pausa normalmente es una celda tonta
que ya existe y solo se usa para mantener ese campo, asi que
se verifica si es el caso y en vez de anyadir una celda mas
se pone el fonema directamente en ella.

Cuando los codigos de pausa son negativos, se considera
que representan un breath (pausa con respiracion audible)
y en vez de silencio se usa el fonema breath rodeado de dos
silencios (la frontera de pausa esta en el breath, los
silencios no tienen frontera asignada). */

VOID uttPausePh(UttPh & u)
{
	UttI p, psil;

	if (u.phoneN()==0) return;  // solo ponemos silencios si hay algun fonema

	for (p=u.pausegrFirst(); p!=0; p=u.pausegrNext(p)) {
		if ((u.cell(p).getWord()==NULL) && (u.cell(p).getPhone()==PH_none))
			psil=p;  // es una celda "tonta"
		else
			psil=u.cellAddBefore(p);	// creamos nueva celda para fonema silencio
		BOOL breath = (u.cell(p).getPause()<0);
		u.cell(psil).setPhone( breath ? PH_breath : PH_sil);
		if (psil!=p) {  // trasladar frontera pausa si hemos creado celda nueva
			u.cell(psil).setPause(u.cell(p).getPause());
			u.cell(p).setPause(UPAUSE_NONE);
		}
		if (breath) {  // rodeamos al breath con dos silencios
			u.cell(u.cellAddBefore(psil)).setPhone(PH_sil);
			u.cell(u.cellAddAfter(psil)).setPhone(PH_sil);
		}
	}
}

/**********************************************************/
/* Aseguramos que la primera palabra tiene un tipo de sentencia
y ponemos una por defecto si no la hay. */

VOID uttDefaultSentence(UttWS & u)
{
	UttI w=u.wordFirst();
	if (w && (u.cell(w).getSentence()==USENTENCE_NONE))
		u.cell(w).setSentence(UTT_DEF_SENTENCE);
}

/**********************************************************/
/* Ponemos las pausas debidas a los limites de sentencias,
incluyendo la inicial (tipo especial BEGIN) y la final (para esta
hay que anyadir una celda para poner la pausa y ademas no es
del tipo de la ultima sentencia, sino tipo END).

Cuando en una palabra hay frontera de tipo pausa significa
que DELANTE de esa palabra hay una pausa. Por eso hace falta
anyadir una celda al final, detras de la ultima palabra,
para poder meter la pausa final.

Ademas el tipo de pausa se obtendra del tipo de la sentencia
anterior. */

VOID uttSentencePauses( UttWS &ut )
{
	UttI p, w;
	Sentence s=USENTENCE_NONE;
	BOOL first=TRUE;

	for (p=ut.sentenceFirst(); p!=0; p=ut.sentenceNext(p)) {
		w=ut.wordFirst(p,URANGE_SENTENCE);  // primera palabra de la sentencia
		if (first) { ut.cell(w).setPause(UPAUSE_UBEGIN); first=FALSE; }
		else ut.cell(w).setPause((INT)s);

		s=ut.cell(p).getSentence();  // salvar tipo de la sentencia para iteracion siguiente
		assert(s!=USENTENCE_NONE);
	}
	p=ut.cellAppend();
	ut.cell(p).setPause(UPAUSE_UEND);

	/* verificacion: la primera palabra de la lista debe ser la que tiene la pausa BEGIN */
	assert(!(ut.wordFirst()) || (ut.cell(ut.wordFirst()).getPause()==UPAUSE_UBEGIN));
}

/**********************************************************/
