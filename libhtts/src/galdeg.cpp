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

Nombre fuente................ galdeg.cpp
Nombre paquete............... aHoTTS
Lenguaje fuente.............. C++
Estado....................... -
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Inma Sardon
				 ................. Nora Barroso (2005/04/11)

======================== Contenido ==================================================
<DOC>
GALDEG.CPP fuente para calificar grupos funcionales como galdegaia

2005/04/11-an egindako aldaketak:
	set_up_flag funtzioaren baitan aldaketa batzuk egin dira. "flag"-en ezarpena ez zen guztiz ondo egiten eta batzuetan irakurri
	ezinak ziren tokietan informazioa irakurtzen zaiatzen zen, ondorioz ".pho" fitxategia ez zen guztiz sortzen.
</DOC>
==================================================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/
#ifndef __GALDEG_CPP__
#define __GALDEG_CPP__
#include "eu_lingp.hpp"

/**********************************************************/

VOID LangEU_GF::set_inf(UttWS &u, UttI p)
{
UttI p_word_next=NULL,p_word_nextn=NULL;
	strcpy(inf.act.word,u.cell(p).getWord());
	inf.act.word_POS=u.cell(p).getPOS();
	if (p!=u.wordLast()) {
		p_word_next=u.wordNext(p);
		strcpy(inf.next.word,u.cell(p_word_next).getWord());
		inf.next.word_POS=u.cell(p_word_next).getPOS();
	}

	if ((p!=u.wordLast()) && (p_word_next!=u.wordLast())) {
		p_word_nextn=u.wordNext(p_word_next);
		strcpy(inf.nextn.word,u.cell(p_word_nextn).getWord());
		inf.nextn.word_POS=u.cell(p_word_nextn).getPOS();
	}
}

//**********************************************************************
VOID LangEU_GF::set_up_flag(UttWS &u, UttI p)
{
	if (p==u.wordLast())
	{
		u_p_flag.utt_last=1;	//detecta final de frase
		u_p_flag.utt_prev_last=0; //NORA

	}
	else	{//si no es ultimo
		u_p_flag.utt_last=0;
		inf.next.p=u.wordNext(p);	//evitar el null
		if (u.wordNext(p)==u.wordLast()) //si es penultimo
			u_p_flag.utt_prev_last=1;
		else {
			u_p_flag.utt_prev_last=0;
			inf.nextn.p=u.wordNext(u.wordNext(p)); 	//evitar el null
		}
	}

	if (u.wordIsLast(p,URANGE_PHRASE))
	{
		u_p_flag.phrase_last=1;	//detecta final de utterance
		u_p_flag.phrase_prev_last=0; //NORA
	}
	else {
		u_p_flag.phrase_last=0;
		if (u.wordIsLast(u.wordNext(p),URANGE_PHRASE))
			u_p_flag.phrase_prev_last=1;
		else
			u_p_flag.phrase_prev_last=0;
	}

	if (p==u.wordFirst())
		u_p_flag.utt_first=1;
	else
		u_p_flag.utt_first=0;

	if (u.wordIsFirst(u.wordNext(p),URANGE_PHRASE))
		u_p_flag.phrase_first=1; //detecta comienzo de frase
	else {
		u_p_flag.phrase_first=0;
		inf.prev.p=u.wordNext(p);	//evitar el null
	}
}

//**********************************************************************

VOID LangEU_GF::galdeg(UttWS &u)
{
UttI p_FGrp_last=NULL,p_FGrp_next=NULL;
UttI p=NULL;
int FGrp_next=0,FGrp_act=0;
	p_FGrp_last=u.fgrpLast();
	for (p=u.fgrpFirst();p!=0;p=u.fgrpNext(p)) {
		if (p != p_FGrp_last) {
 //			if (p!=u.wordLast()) {
//		if (u.cell(p).fgrpIsLast(p,URANGE_PHRASE)) {//INMASS ????
			if (!u.wordIsLast(p,URANGE_PHRASE)) {
				p_FGrp_next=u.fgrpNext(p);
				FGrp_next = u.cell(p_FGrp_next).getFGrp();
				FGrp_act = u.cell(p).getFGrp();
/* EVA -> Si la frase empieza por verbo, esa es la galdegaia*/
			if (p==u.fgrpFirst())
						if (FGrp_act==GF_EU_ADI)
							u.cell(p).setFGrp(GF_EU_GALDE);

				if ( (FGrp_next==GF_EU_ADI) && (FGrp_act==GF_EU_ARRUN))
					if ( !(u.cell(p).queryPOS(POS_EU_LOT_JNT)) ||
						!(u.cell(p).queryPOS(POS_EU_LOT_AZK)) )
						u.cell(p).setFGrp(GF_EU_GALDE);
			}
		}
	}
}
#endif
