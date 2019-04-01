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

Nombre fuente................ gfpau.cpp
Nombre paquete............... aHoTTS
Lenguaje fuente.............. C++
Estado....................... -
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Inma Sardon
.............................


======================== Contenido ========================
<DOC>
GFPAU.CPP fuente para considerar grupos funcionales pausales
</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/
#ifndef __GFPAU_CPP__
#define __GFPAU_CPP__
#include "eu_lingp.hpp"

/**********************************************************/


VOID LangEU_GF::invpau(UttWS &u,UttI p,int *gf)
{
int fgrp,pau=0;
					if (u.cell(p).queryPOS(POS_EU_PAUSE_AURRE)) {
						u.cell(p).setFGrp(GF_EU_PAU_AURRE);
										fgrp=u.cell(p).getFGrp();
										pau=1;
					}
					if (u.cell(p).queryPOS(POS_EU_PAUSE_ATZE)) {
							u.cell(p).setFGrp(GF_EU_PAU_ATZE);
							fgrp=u.cell(p).getFGrp();
							pau=1;
								}
				if (u.cell(p).queryPOS(POS_EU_PAUSE_AURRE) &&
				 u.cell(p).queryPOS(POS_EU_PAUSE_ATZE)) {
						u.cell(p).setFGrp(GF_EU_PAU_ARTEAN);
						fgrp=u.cell(p).getFGrp();
						pau=1;
				 }
				 if (pau==1){
						indice2=1;
						encontrado=TRUE;
						*gf=fgrp;

	if (!u_p_flag.phrase_first && !u_p_flag.utt_first) {
					if (u.cell(inf.prev.p).queryPOS(POS_EU_PAUSE_AURRE) ||
					 u.cell(inf.prev.p).queryPOS(POS_EU_PAUSE_ATZE)) {
						u.cell(inf.prev.p).setFGrp(fgrp);
						u.cell(p).setFGrp(GF_EU_NONE);
						//*indice=1; //no, en realidad es dos OJOOO!!!
						indice2=1;
						encontrado=TRUE;
						*gf=fgrp;
					}
				}
		if (!u_p_flag.phrase_last && !u_p_flag.utt_last) {
					if (u.cell(inf.next.p).queryPOS(POS_EU_PAUSE_AURRE) ||
					 u.cell(inf.next.p).queryPOS(POS_EU_PAUSE_ATZE)) {
						u.cell(p).setFGrp(fgrp);
						u.cell(inf.next.p).setFGrp(GF_EU_NONE);
						indice2=2;
						encontrado=TRUE;
						*gf=fgrp;
					}
				}
		}//fin de pau==1
}
/*******************************************************************/
VOID LangEU_GF::jnt(UttWS &u,UttI p,int *gf)
{
		//Detectar noun o adj + enklitiko
				//*** 2.- Comprobar que existe esa palabra en el dicc no etiquetada con ize
				if (u.cell(p).queryPOS(POS_EU_NONE) || u.cell(p).queryPOS(POS_EU_IZE)) {
				//*** 3.-consideraremos la palabra como nombre
				//*** 1.- Comprobar que hay una palabra etiquetada con ize en el dicc
						//noun+bat
						u.cell(p).setFGrp(GF_EU_ARRUN);

		if (!u_p_flag.phrase_last && !u_p_flag.utt_last) {
									if ( u.cell(inf.next.p).queryPOS(POS_EU_LOT_JNT)) {
										u.cell(inf.next.p).setFGrp(0);
		if (!u_p_flag.phrase_prev_last && !u_p_flag.utt_prev_last) {
												if (u.cell(inf.nextn.p).queryPOS(POS_EU_NONE) ||
												 u.cell(inf.nextn.p).queryPOS(POS_EU_IZE)) {
													u.cell(inf.nextn.p).setFGrp(0);
													indice2=3;
													encontrado=TRUE;
													*gf=1;
												 }
											}
										}
						}
				}
}
#endif
