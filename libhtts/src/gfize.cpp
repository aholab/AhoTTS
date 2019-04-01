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

Nombre fuente................ gfize.cpp
Nombre paquete............... aHoTTS
Lenguaje fuente.............. C++
Estado....................... -
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Inma Sardon
.................................. Nora Barroso (2005/04/11)


======================== Contenido ==================================================
GFIZE.CPP fuente para elaborar grupos funcionales nominales

2005/04/11-an egindako aldaketak:
	Aldaketa "izeize" funtzioaren baitan egin da. Batzuetan irakurri 	ezinak ziren tokietan informazioa irakurtzen zaiatzen zen,
	ondorioz ".pho" fitxategia ez zen guztiz sortzen.
==================================================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/
#ifndef __GFIZE_CPP__
#define __GFIZE_CPP__

#include "eu_lingp.hpp"

/**********************************************************/

VOID LangEU_GF::enk(UttWS &u,UttI p,int *gf)
{
BOOL adj=FALSE; //para inf.next.p
	if (!u_p_flag.phrase_last && !u_p_flag.utt_last) {
		if (u.cell(inf.next.p).queryPOS(POS_EU_NONE)){
			switch (u.getHDicDB()->query(u.cell(inf.next.p).getHDicRef(),HDIC_QUERY_EU_TALDE2)) {
				case HDIC_ANSWER_EU_TALDE2_ADJ:
					adj=TRUE;
				break;
			}
		}
	}
		//Detectar noun o adj + enklitiko
				//*** 2.- Comprobar que existe esa palabra en el dicc no etiquetada con ize
		if (u.cell(p).queryPOS(POS_EU_NONE) ||
			 u.cell(p).queryPOS(POS_EU_IZE) ){
				//*** 3.-consideraremos la palabra como nombre
				//*** 1.- Comprobar que hay una palabra etiquetada con ize en el dicc
						//noun+bat
				u.cell(p).setFGrp(GF_EU_ARRUN);

			if (!u_p_flag.phrase_last && !u_p_flag.utt_last) {
					if ( u.cell(inf.next.p).queryPOS(POS_EU_ENKLITIKO)) { //bat
						u.cell(inf.next.p).setFGrp(GF_EU_NONE);
						*gf=1;
						encontrado=TRUE;
						indice2=2;
					}
					else {
					//noun+adj+bat
						if (!u_p_flag.phrase_prev_last && !u_p_flag.utt_prev_last) {
							if ( (u.cell(inf.next.p).queryPOS(POS_EU_ADJ)) ||
									(u.cell(inf.next.p).queryPOS(POS_EU_IZE)) ||
									(u.cell(inf.next.p).queryPOS(POS_EU_NONE))||
									(adj==TRUE) ){
										u.cell(inf.next.p).setFGrp(0);
										if ( (u.cell(inf.next.p).queryPOS(POS_EU_ADJ)) ||
											(adj==TRUE) ){
											indice2=2;
											encontrado=TRUE;
											*gf=1;
										}
										if (!u_p_flag.phrase_prev_last &&
										!u_p_flag.utt_prev_last) {
											if ( (u.cell(inf.nextn.p).queryPOS(POS_EU_ENKLITIKO)) ||
													(u.cell(inf.nextn.p).queryPOS(POS_EU_DET)) ){
													u.cell(inf.nextn.p).setFGrp(GF_EU_NONE);
													*gf=1;
													encontrado=TRUE;
													indice2=3;
											}
										}
							}//fin del if POS_EU_ADJ /NONE
						}
					}
			} //fin de comprobacion final de frase o utterance
		}//fin del if POS_EU_IZE
}

/*************************************************************************/

VOID LangEU_GF::detize(UttWS &u,UttI p,int *gf)
{
BOOL adj=FALSE; //para inf.next.p
	if (!u_p_flag.phrase_last && !u_p_flag.utt_last) {
		if (u.cell(inf.next.p).queryPOS(POS_EU_NONE)){
			switch (u.getHDicDB()->query(u.cell(inf.next.p).getHDicRef(),HDIC_QUERY_EU_TALDE2)) {
				case HDIC_ANSWER_EU_TALDE2_ADJ:
					adj=TRUE;
				break;
			}
		}
	}
	if (u.cell(p).queryPOS(POS_EU_DET)) {
		u.cell(p).setFGrp(GF_EU_ARRUN);
		if (!u_p_flag.phrase_last && !u_p_flag.utt_last) {
			if ( (u.cell(inf.next.p).queryPOS(POS_EU_NONE)) ||
				(u.cell(inf.next.p).queryPOS(POS_EU_ADJ)) ||
				(adj==TRUE) ||
				(u.cell(inf.next.p).queryPOS(POS_EU_IZE)) ){
					u.cell(inf.next.p).setFGrp(0);
					indice2=2;
					encontrado=TRUE;
//					fprintf(stderr,"detize %s encontrado %d\n",u.cell(p).getWord(),encontrado);
					*gf=1;
			}
			if (!u_p_flag.phrase_prev_last && !u_p_flag.utt_prev_last) {
				if ( u.cell(inf.nextn.p).queryPOS(POS_EU_DET) )
					if (u.wordNext(inf.nextn.p)!=NULL){
						if ( !(u.cell(u.wordNext(inf.nextn.p)).queryPOS(POS_EU_IZE)) ||
								!(u.cell(u.wordNext(inf.nextn.p)).queryPOS(POS_EU_NONE)) ||
								!(u.cell(u.wordNext(inf.nextn.p)).queryPOS(POS_EU_ADJ)) ) {
									u.cell(inf.nextn.p).setFGrp(0);
									indice2=3;
									encontrado=TRUE;
									*gf=1;
//									fprintf(stderr,"detize %s encontrado %d\n",u.cell(p).getWord(),encontrado);
						}
					}
			}
		}
	}
//fprintf(stderr,"detize %s encontrado %d\n",u.cell(p).getWord(),encontrado);
}

/*************************************************************************/
VOID LangEU_GF::izedet(UttWS &u,UttI p,int *gf)
{
	if ( (u.cell(p).queryPOS(POS_EU_ATZ_IZE)) ||
		 (u.cell(p).queryPOS(POS_EU_NONE)) ||
			(u.cell(p).queryPOS(POS_EU_IZE)) ){
		u.cell(p).setFGrp(GF_EU_ARRUN);

		if (!u_p_flag.phrase_last && !u_p_flag.utt_last) {
			if (u.cell(inf.next.p).queryPOS(POS_EU_DET)) {
				u.cell(inf.next.p).setFGrp(GF_EU_NONE);
				indice2=2;
				encontrado=TRUE;
//				fprintf(stderr,"izedet %s encontrado %d\n",u.cell(p).getWord(),encontrado);
				*gf=1;
			}
		}
	}
//fprintf(stderr,"izedet %s encontrado %d\n",u.cell(p).getWord(),encontrado);
}

/*************************************************************************/
VOID LangEU_GF::izeize(UttWS &u,UttI p,int *gf)
{
	if (u.cell(p).queryPOS(POS_EU_ATZ_IZE))
	{
		u.cell(p).setFGrp(GF_EU_ARRUN);
		if (!u_p_flag.phrase_last && !u_p_flag.utt_last)
		{

			if ( (u.cell(inf.next.p).queryPOS(POS_EU_NONE)) ||
				(u.cell(inf.next.p).queryPOS(POS_EU_IZE)) )
			{
				u.cell(inf.next.p).setFGrp(GF_EU_NONE);
				indice2=2;
				encontrado=TRUE;
				*gf=1;
			}

			if (!u_p_flag.phrase_prev_last && !u_p_flag.utt_prev_last)
			{
				if (u.cell(inf.next.p).queryPOS(POS_EU_ATZ_IZE))
				{
					 if ( (u.cell(inf.nextn.p).queryPOS(POS_EU_NONE)) ||
					(u.cell(inf.nextn.p).queryPOS(POS_EU_DET)) ||
					(u.cell(inf.nextn.p).queryPOS(POS_EU_ENKLITIKO)) ||
					(u.cell(inf.nextn.p).queryPOS(POS_EU_IZE)) )
					{
							u.cell(inf.next.p).setFGrp(GF_EU_NONE);
							u.cell(inf.nextn.p).setFGrp(GF_EU_NONE);
							indice2=3;
							encontrado=TRUE;
							*gf=1;
					}
				}
			}

		}

	}
}

/*************************************************************************/

VOID LangEU_GF::izeadb(UttWS &u,UttI p,int *gf)
{
BOOL adj=FALSE; //para p actual
	if (u.cell(p).queryPOS(POS_EU_NONE)){
		switch (u.getHDicDB()->query(u.cell(p).getHDicRef(),HDIC_QUERY_EU_TALDE2)) {
			case HDIC_ANSWER_EU_TALDE2_ADJ:
				adj=TRUE;
				break;
		}
	}
	if ( (u.cell(p).queryPOS(POS_EU_ATZ_IZE)) ||
		 (u.cell(p).queryPOS(POS_EU_NONE)) ||
			(u.cell(p).queryPOS(POS_EU_IZE)) ||
			(u.cell(p).queryPOS(POS_EU_ADJ)) ||
			(adj==TRUE) ){
					u.cell(p).setFGrp(GF_EU_ARRUN);
					if (!u_p_flag.phrase_last && !u_p_flag.utt_last) {
						if (u.cell(inf.next.p).queryPOS(POS_EU_ADB)) {
							u.cell(inf.next.p).setFGrp(GF_EU_NONE);
							indice2=2;
							encontrado=TRUE;
							*gf=1;
//							fprintf(stderr,"izeadb %s encontrado %d\n",u.cell(p).getWord(),encontrado);
						}
					}
	}
//fprintf(stderr,"izeadb %s encontrado %d\n",u.cell(p).getWord(),encontrado);
}

/*************************************************************************/

VOID LangEU_GF::adbadj(UttWS &u,UttI p,int *gf)
{
BOOL adj=FALSE; //para p actual
	if (u.cell(p).queryPOS(POS_EU_NONE)){
		switch (u.getHDicDB()->query(u.cell(p).getHDicRef(),HDIC_QUERY_EU_TALDE2)) {
			case HDIC_ANSWER_EU_TALDE2_ADJ:
				adj=TRUE;
				break;
		}
	}
	if ( u.cell(p).queryPOS(POS_EU_ADB) ) {
					u.cell(p).setFGrp(GF_EU_ARRUN);
					if (!u_p_flag.phrase_last && !u_p_flag.utt_last) {
						if (u.cell(inf.next.p).queryPOS(POS_EU_ADJ)) {
							u.cell(inf.next.p).setFGrp(GF_EU_NONE);
							indice2=2;
							encontrado=TRUE;
							*gf=1;
//							fprintf(stderr,"izeadb %s encontrado %d\n",u.cell(p).getWord(),encontrado);
						}
					}
	}
//fprintf(stderr,"izeadb %s encontrado %d\n",u.cell(p).getWord(),encontrado);
}

#endif
