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

Nombre fuente................ es_stre.cpp
Nombre paquete............... aHoTTS
Lenguaje fuente.............. C++
Estado....................... -
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Borja Etxebarria
.............................

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
2.0.1    09/04/13  iñaki     Corregir acentuación de llanas acabadas en "n" pero convertidas en "m" por coarticulacion que se ponían como agudas
2.0.0    10/10/08  iñaki     Corregir acentuación monosilabos y añadir excepciones para átonas
1.0.0    31/01/00  borja     codefreeze aHoTTS v1.0
0.0.0    24/11/97  borja     Codificacion inicial.

======================== Contenido ========================
<DOC>
Metodos de {{LangES_PhTrans}} que analizan las palabras para saber
si son agudas, llanas o esdrujulas y poner el acento donde corresponda.
</DOC>

Pendiente:
$$$ falta iu ui. Y que les pasa a las palabras que acaban en mente?
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include <string.h>

#include "es_lingp.hpp"

/**********************************************************/
/* Acentua una silaba {syl} en la vocal mas fuerte (aeo) o en
   la segunda debil (iu). */

VOID LangES_PhTrans::syllable_stress(UttPh & u, UttI syl)
{
	UttI v = LangES_PhUti::syllable_vowel(u, syl);

	if (v)
		u.cell(v).setStress(USTRESS_AUTO);
}

/**********************************************************/

VOID LangES_PhTrans::word_stress(UttPh & u, UttI wordp)
{
	UttI i, i2;
	LONG n;
	Phone p, p2;

	i = u.phoneLast(wordp, URANGE_WORD);  // vamos al ultimo fonema

	if (!i) return;  // si no hay fonemas, adios!
	//Inaki acentuar salvo excepciones: http://www.salonhogar.com/espanol/lenguaje/gramatica/acentuacion.htm
	/*if(u.cell(wordp).getPOS() == POS_ES_PREP || u.cell(wordp).getPOS() == POS_ES_CONJ)
		return;
	if(u.getHDicDB()->query(u.cell(wordp).getHDicRef(),HDIC_QUERY_ES_PREP) == HDIC_QUERY_ES_CONJPOS)
		return;*/
	
	const char * word = u.cell(wordp).getWord();

	n = u.syllablePos(i, URANGE_WORD);
	if (!n) {	// monosilabos, son agudos
		//syllable_stress(u, i);
		//inaki: excepciones
		if(wordp != u.wordThis(u.phoneLast(wordp, URANGE_PAUSE)) &&(!strcmp(word, "el") || !strcmp(word, "la") || !strcmp(word, "las") || !strcmp(word, "los") || !strcmp(word, "lo")
				||!strcmp(word, "mi") || !strcmp(word, "tu") || !strcmp(word, "su") || !strcmp(word, "mis") || !strcmp(word, "tus") || !strcmp(word, "sus")
				|| !strcmp(word, "me") || !strcmp(word, "nos") || !strcmp(word, "te") || !strcmp(word, "os") || !strcmp(word, "le") || !strcmp(word, "les") || !strcmp(word, "se")	
				|| !strcmp(word,"que") ||!strcmp(word,"tan")  
				|| !strcmp(word,"a") ||!strcmp(word,"al") ||!strcmp(word,"con") ||!strcmp(word,"de") ||!strcmp(word,"del") ||!strcmp(word,"en") ||!strcmp(word,"por") ||!strcmp(word,"sin") ||!strcmp(word,"tras") ||!strcmp(word,"tan")
				||!strcmp(word,"y") ||!strcmp(word,"e") ||!strcmp(word,"ni") ||!strcmp(word,"o") ||!strcmp(word,"u") ||!strcmp(word,"mas") ||!strcmp(word,"si") || !strcmp(word,"pues")))		
			return;
		//el resto se acentuan
		syllable_stress(u, i);
	}
	//inaki: excepciones
	if(wordp != u.wordThis(u.phoneLast(wordp, URANGE_PAUSE)) && ( !strcmp(word, "donde") || !strcmp(word, "como") || !strcmp(word, "cuando") || !strcmp(word, "cuanto") ||!strcmp(word, "quien") || !strcmp(word, "cuyo")
		||!strcmp(word,"ante") ||!strcmp(word,"bajo") || !strcmp(word, "contra") || !strcmp(word,"desde") ||!strcmp(word,"entre") ||!strcmp(word,"hacia") 
		||!strcmp(word,"hasta") ||!strcmp(word,"para") ||!strcmp(word,"sobre") ||!strcmp(word,"durante") ||!strcmp(word,"mediante")
		||!strcmp(word,"pero") ||!strcmp(word,"sino") || !strcmp(word,"aunque") ))  
		return;
	//inaki, si no son la última palabra no llevan acento
	//es nuestra casa -> nuestra sin acento
	//esa casa es nuestra --> nuestra con acento
	if(wordp != u.wordThis(u.phoneLast(wordp, URANGE_PAUSE)) && (!strcmp(word, "nuestro") || !strcmp(word, "nuestra") || !strcmp(word, "nuestros") || !strcmp(word, "nuestras")
		|| !strcmp(word, "vuestro") || !strcmp(word, "vuestra") || !strcmp(word, "vuestros") || !strcmp(word, "vuestras")))
		return;
		


	p = u.cell(i).getPhone();
	i2 = u.phonePrev(i, URANGE_SYLLABLE);
	p2 = i2 ? u.cell(i2).getPhone() : 0;

	// palabra acabada en [aeo]i es aguda y con acento en la [aeo]
	if ((p == PHES_i) && ((p2 == PHES_a) || (p2 == PHES_e) || (p2 == PHES_o))) {
		u.cell(i2).setStress(USTRESS_AUTO);
		return;
	}

	// palabra acabada en [nkp]s es aguda 
	if ((p == PHES_s) && ((p2 == PHES_n) || (p2 == PHES_k) || (p2 == PHES_p))) {
		syllable_stress(u, i2);
		return;
	}

	// palabra acabada en [vocal,n,s] es llana
	if (LangES_PhUti::phIsVowel(p) || (p == PHES_n) || (p == PHES_s)) {
		syllable_stress(u, u.syllablePrev(i, URANGE_WORD));
		return;
	}
	//inaki hay casos en los que la n se convierte en m (antes de p,m,b,f) y deberían ser llanas
	if(p == PHES_m && u.cell(i).getChar() == 'n'){
		syllable_stress(u, u.syllablePrev(i, URANGE_WORD));
		return;
	}

	syllable_stress(u, i);	// resto, agudas
}

/**********************************************************/

BOOL LangES_PhTrans::word_stressed(UttPh & u, UttI wordp)
{
	for (Lix p=u.phoneFirst(wordp,URANGE_WORD); p!=0; p=u.phoneNext(p,URANGE_WORD))
		if (u.cell(p).getStress()!=USTRESS_NONE) return TRUE;
	return FALSE;
}

/**********************************************************/
