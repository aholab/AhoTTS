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
(C) 1999 TSR/Aholab - ETSII/IT Bilbao (UPV/EHU) // Euskaltel

Nombre fuente................ eu_stre.cpp
Nombre paquete............... aHoTTS
Lenguaje fuente.............. C++
Estado....................... -
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion: ............... Antonio Aparicio
............................. Ana Armenta
............................. Borja Etxebarria

Version  dd/mm/aa  Autor     Comentario
-------  --------  --------  ----------
1.0.0    31/01/00  borja     codefreeze aHoTTS v1.0
0.0.0    18/05/99  Antonio   Codificacion inicial

======================== Contenido ========================
<DOC>
Metodos de {{LangEU_PhTrans}} que analizan las palabras para saber
si son agudas, llanas o esdrujulas y poner el acento donde corresponda.
</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

# include <string.h>
# include "eu_lingp.hpp"
# include "uti.h"
//#define DEBUG() fprintf (stderr,"file..: %s -- line..: %d\n",__FILE__,__LINE__);
#define DEBUG()

/************************************************************************/
VOID LangEU_PhTrans::fgrp2agrp(UttPh &u,UttI fg)
{
	//primero ponemos frontera y tipo de AGRP
	for(UttI p=u.wordFirst(fg,URANGE_FGRP);p!=0;p=u.wordNext(p,URANGE_FGRP)){
		//por defecto frontera acentual de tipo 2
		DEBUG()
		u.cell(p).setAGrp(AGRP_EU_OROK);
		//y si cumple alguno de los siguientes casos la cambiamos
		DEBUG()
		if(acento_texto(u,p)){
			u.cell(p).setAGrp(AGRP_EU_TXT);
		}
/*EVA!!! Los enclíticos sólo tienen que ir no acentuados si hay alguna
palabra delante en su mismo FGrp, a la que se 'pegan'*/
		if(es_enclitico(u,p)&&(u.cell(p).getFGrp()==GF_EU_NONE)){
			u.cell(p).setAGrp(AGRP_EU_NONE);
		}
		DEBUG()
		if(es_marcada(u,p)){
			DEBUG()
			u.cell(p).setAGrp(AGRP_EU_MRK);
		}
		DEBUG()
		if(es_verbo_jok(u,p)&&es_bisilabo(u,p)&&es_ko_go_ten_tzen(u,p)){
			//tb que es verbo
			//hay que verificar que la raiz es monosilabica
			DEBUG()
			u.cell(p).setAGrp(AGRP_EU_MRK);
		}
		DEBUG()
		if(es_lotura(u,p)){
			u.cell(p).setAGrp(AGRP_EU_GABE);
		}
		DEBUG()
		if(es_proclitico(u,p)){//pero solo en caso de que el siguiente sea lgn o trn
			//u.cell(p).setAGrp(AGRP_EU_OROK);
			if((p=u.wordNext(p,URANGE_FGRP))!=NULL){
				if(es_verbo_trn(u,p)||es_verbo_lgn(u,p))
					u.cell(p).setAGrp(AGRP_EU_NONE);
				else
					u.cell(p).setAGrp(AGRP_EU_OROK);
			}
		}
		else if(es_verbo_jok(u,p)){
			DEBUG()
			//u.cell(p).setAGrp(AGRP_EU_OROK);
			if((p=u.wordNext(p,URANGE_FGRP))!=NULL){
				DEBUG()
				if(es_monosilabo(u,p)&&es_verbo_lgn(u,p)){
					DEBUG()
					u.cell(p).setAGrp(AGRP_EU_NONE);
				}
				else
					u.cell(p).setAGrp(AGRP_EU_OROK);

			}
		}
		DEBUG()
	}
	DEBUG()
}
/***************************************************************************/
VOID LangEU_PhTrans::agrp_stress(UttPh &u,UttI ag)
{
UttI p;//apuntador a palabra
UttI s;
INT tipo_agrp;
	p=u.wordFirst(ag,URANGE_AGRP);
	tipo_agrp=u.cell(p).getAGrp();
	DEBUG()
	//acentuamos convenientemente cada tipo de AGRP
	switch(tipo_agrp){
		case AGRP_EU_MRK:
					DEBUG()
				//acento en la primera
					s=u.syllableFirst(ag,URANGE_AGRP);
					syllable_stress(u,s);
					break;
		case AGRP_EU_OROK:
					DEBUG()
				//acento en la segunda
					s=u.syllableFirst(ag,URANGE_AGRP);
					s=u.syllableNext(s,URANGE_AGRP);
					if(s)
						syllable_stress(u,s);
					else{
						s=u.syllableFirst(ag,URANGE_AGRP);
						syllable_stress(u,s);
					}
					break;
		case AGRP_EU_GABE:
					DEBUG()
		//sin acento
		case AGRP_EU_NONE:
					DEBUG()
					break;
		//no hay frontera de grupo acentual
	}
}
/***************************************************************************/
VOID LangEU_PhTrans::word_stress(UttPh &u,UttI wordp)
{
UttI i;//apuntador a la s¡laba
UttI p;//apuntador a palabra
const char *hitza;
char x;
LONG n;
//siempre tengo la palabra actual en hitza
//apuntador a palabra actual en wordp
	hitza=u.cell(wordp).getWord();
//apuntador a palabra anterior en p
	if(!u.wordIsFirst(wordp,URANGE_PHRASE))
			p=u.wordPrev (wordp, URANGE_PHRASE);
	else p=0;//Cuando no hay palabra anterior p vale 0.

//si es edo,eta,ala,baino en caso de que vayan al final de frase se acent£an
//en la segunda s¡laba, resto de casos NO se acent£an.
	if (es_sin_acento(u,wordp)){
		if(u.wordIsLast (wordp,URANGE_PHRASE)){
			i=u.syllableLast (wordp,URANGE_WORD);
			syllable_stress (u,i);
			return;
		}
		return;
	}
	//si es palabra marcada ser¡a m s sencillo pasarle hitza, pero:
	//le pasamos u y wordp con vistas a que esta informaci¢n pueda venir dada
	//por el etiquetador POS
	//salbuespena: funci¢n que devuelve TRUE cuando la palabra hay que
	//acentuarla en la primera s¡laba(es una palabra marcada)
	if(salbuespena(u,wordp)){
		i=u.syllableFirst(wordp,URANGE_WORD);
		syllable_stress(u,i);
		return;
	}
	//cuando un verbo auxiliar o sintetico va precedido de 'ez','bait' ¢ 'ba'
	//se acent£an en la primera s¡laba y la part¡cula [ez,bait,ba] queda sin
	//acentuar
	x=hitza[0];
	if(es_aux_o_sint(u,wordp,x)){
		//faltan los que empiezan por h
		if(p){
			if (es_ez_ba_bait(u,p)){
				i=u.syllableFirst (wordp, URANGE_WORD);
				syllable_stress (u, i);
				return;
			}
		}
	}

	//averiguo el n£mero de s¡labas de la palabra
	i=u.phoneLast(wordp,URANGE_WORD);
	n=u.syllablePos(i,URANGE_WORD);
	i=u.syllableFirst(wordp,URANGE_WORD);
	//si es verbo perifr stico con aspecto imperfecto o futuro, y el n£mero
	//de s¡labas "raiz+sufijo"=dos el acento se sit£a sobre la primera s¡laba.
/************hay que preguntar si es verbo*****************/
//si no, lo hacemos para todas las palabras bis¡labas con esa terminaci¢n
	if((n==1)&&es_ko_go_ten_tzen(u,wordp)){
		syllable_stress(u,i);
		return;
	}

	//Si es monosilabo se acent£a,excepto cuando se trate de [ez,bait,ba]
	//que no los acentuaremos por si detr s va un auxiliar
	if(n==0){
		if(es_ez_ba_bait(u,wordp)&&!u.wordIsLast(wordp,URANGE_SENTENCE))
			return;
		syllable_stress(u,i);
		return;
	}
	//si una palabra llega hasta aqu¡, no es auxiliar. Luego, si la palabra
	//anterior es [ez,ba,bait] habr  que acentuar aurre.
	if(p){
		if(es_ez_ba_bait(u,p))
				syllable_stress(u,p);
	}
	/**********caso general*************/
	//el resto de palabras se acent£an en la 2¦s¡laba
	i=u.syllableNext (i,URANGE_WORD);
	syllable_stress(u,i);
 }
/**********************************************************************/
VOID LangEU_PhTrans::syllable_stress(UttPh &u,UttI syl)
{
	UttI v = LangEU_PhUti::syllable_vowel (u, syl);
	if (v)
		u.cell(v).setStress (USTRESS_AUTO);
}
/***************************************************************************/
BOOL LangEU_PhTrans::agrp_stressed (UttPh &u, UttI ag)
{
	for (UttI p=u.phoneFirst(ag,URANGE_AGRP); p!=0; p=u.phoneNext(p,URANGE_AGRP))
		if (u.cell(p).getStress()!=USTRESS_NONE) return (TRUE);
	return (FALSE);
}
/************************************************************************/
BOOL LangEU_PhTrans::word_stressed (UttPh &u, UttI wordp)
{

for (Lix p=u.phoneFirst(wordp,URANGE_WORD); p!=0; p=u.phoneNext(p,URANGE_WORD))
	if (u.cell(p).getStress()!=USTRESS_NONE) return TRUE;
return FALSE;

}
/************************************************************************/
