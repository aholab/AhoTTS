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

Nombre fuente................ gfadi.cpp
Nombre paquete............... aHoTTS
Lenguaje fuente.............. C++
Estado....................... -
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Inma Sardon
.............................

======================== Contenido ========================
<DOC>
GFADI.CPP para elaborar grupos funcionales verbales
</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/
#ifndef __GFADI_CPP__
#define __GFADI_CPP__
#include "eu_lingp.hpp"

/**********************************************************/

VOID LangEU_GF::baitadi(UttWS &u,UttI p,int *gf)
{
	if (u.cell(p).queryPOS(POS_EU_PROKLITIKO)) {
		if (!u_p_flag.phrase_last && !u_p_flag.utt_last) {
			if ( (u.cell(inf.next.p).queryPOS(POS_EU_ADI_LGN)) ||
				 (u.cell(inf.next.p).queryPOS(POS_EU_ADI_TRN))  ||
				 (u.cell(inf.next.p).queryPOS(POS_EU_ADI_JOK))  ||
				 (u.cell(inf.next.p).queryPOS(POS_EU_ATZ_ADI1))  ||
      			(u.cell(inf.next.p).queryPOS(POS_EU_ATZ_ADI2))  ||
				 (u.cell(inf.next.p).queryPOS(POS_EU_ATZ_ADI3)) ) {
						if (!strcmp(inf.act.word,"ez")) {
							if (u.cell(inf.next.p).queryPOS(POS_EU_ATZ_ADI2))
								u.cell(p).setFGrp(GF_EU_ADI);
							else
								u.cell(p).setFGrp(GF_EU_ADI_EZ);
						}
						else
							u.cell(p).setFGrp(GF_EU_ADI);
						u.cell(inf.next.p).setFGrp(GF_EU_NONE);
						indice2=2;
						encontrado=TRUE;
						*gf=u.cell(p).getFGrp();
						if (!u_p_flag.phrase_prev_last && !u_p_flag.utt_prev_last) {
							if ((u.cell(inf.nextn.p).queryPOS(POS_EU_ADI_JOK)) ||
							 (u.cell(inf.nextn.p).queryPOS(POS_EU_ATZ_ADI1)) ){
										u.cell(inf.nextn.p).setFGrp(GF_EU_NONE);
										indice2=3;
										encontrado=TRUE;
										*gf=u.cell(p).getFGrp();
							}
						} //fin comprobacion
			}
		}//fin de comprobacion
	} //fin de enclitico
}

/*********************************************************************/
VOID LangEU_GF::joklgn(UttWS &u,UttI p,int *gf)
{
	if ( (u.cell(p).queryPOS(POS_EU_ADI_JOK)) ||
	 u.cell(p).queryPOS(POS_EU_ATZ_ADI1)){
			u.cell(p).setFGrp(GF_EU_ADI);
			if (!u_p_flag.phrase_last && !u_p_flag.utt_last) {
				if ( (u.cell(inf.next.p).queryPOS(POS_EU_ADI_LGN)) ||
				 (u.cell(inf.next.p).queryPOS(POS_EU_ATZ_ADI3)) ){
							u.cell(inf.next.p).setFGrp(GF_EU_NONE);
							indice2=2;
							encontrado=TRUE;
							*gf=2;
				}
				else
						if ( (u.cell(inf.next.p).queryPOS(POS_EU_PROKLITIKO))||
						 (u.cell(inf.next.p).queryPOS(POS_EU_PRT)) ){
							if (!u_p_flag.phrase_prev_last && !u_p_flag.utt_prev_last) {
								if ( (u.cell(inf.nextn.p).queryPOS(POS_EU_ADI_LGN)) ||
									(u.cell(inf.nextn.p).queryPOS(POS_EU_ATZ_ADI3)) ){
										if (!strcmp(u.cell(inf.next.p).getWord(),"ez")) {
											u.cell(p).setFGrp(GF_EU_ADI_EZ);
											u.cell(inf.next.p).setFGrp(GF_EU_NONE);
											u.cell(inf.nextn.p).setFGrp(GF_EU_NONE);
											indice2=3;
											encontrado=TRUE;
											*gf=3;	//GF verbal negativo
										}
										else{
											u.cell(p).setFGrp(GF_EU_ADI);
											u.cell(inf.next.p).setFGrp(GF_EU_NONE);
											u.cell(inf.nextn.p).setFGrp(GF_EU_NONE);
											indice2=3;
											encontrado=TRUE;
											*gf=2;	//GF verbal no negativo

										}
								}
							}
						}
			 }
	}
//fprintf(stderr,"joklgn %s encontrado %d\n",u.cell(p).getWord(),encontrado);
}

/*********************************************************************/

VOID LangEU_GF::trn(UttWS &u,UttI p,int *gf)
{
	if ( (u.cell(p).queryPOS(POS_EU_ADI_TRN)) ||
		(u.cell(p).queryPOS(POS_EU_ATZ_ADI1)) ){
				u.cell(p).setFGrp(GF_EU_ADI);
				indice2=1;
				encontrado=TRUE;
	}
}

/*********************************************************************/

VOID LangEU_GF::jokjok(UttWS &u,UttI p,int *gf)
{
	if ( (u.cell(p).queryPOS(POS_EU_ADI_JOK)) ||
	 u.cell(p).queryPOS(POS_EU_ATZ_ADI1)){
			u.cell(p).setFGrp(GF_EU_ADI);
			if (!u_p_flag.phrase_last && !u_p_flag.utt_last) {
				if ( (u.cell(inf.next.p).queryPOS(POS_EU_ADI_JOK)) ||
				(u.cell(inf.next.p).queryPOS(POS_EU_ATZ_ADI1)) ){
						u.cell(inf.next.p).setFGrp(GF_EU_NONE);
						indice2=2;
						encontrado=TRUE;
						*gf=2;
				if (!u_p_flag.phrase_prev_last && !u_p_flag.utt_prev_last) {
						if ((u.cell(inf.nextn.p).queryPOS(POS_EU_ADI_LGN)) ||
						 (u.cell(inf.nextn.p).queryPOS(POS_EU_ATZ_ADI3)) ){
									u.cell(inf.nextn.p).setFGrp(GF_EU_NONE);
									indice2=3;
									encontrado=TRUE;
									*gf=2;
						}
					} //fin de comprobacion
				}
			} //fin de comprobacion
	}
}
#endif