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

Nombre fuente................ poscases.cpp
Nombre paquete............... aHoTTS
Lenguaje fuente.............. C++
Estado....................... -
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Inma Sardon
.............................

======================== Contenido ========================
<DOC>
</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include "eu_lingp.hpp"
#include "httsmsg.h"

#define DEBUGPOSx

/**********************************************************/

VOID LangEU_Categ::detaux(UttWS &u, UttI p) {
int primero=0;
BOOL excep=FALSE;
UttI p_word_prev=NULL;
if (p!=u.wordFirst())
	p_word_prev=u.wordPrev(p);
else
	primero=1;
int ultimo=0;

UttI p_word_next=NULL;
if (p!=u.wordLast())
	p_word_next=u.wordNext(p);
else
	ultimo=1;

		if (!strcmp(u.cell(p).getWord(),"dena")) {
			if ((ultimo==0) && !(u.wordIsLast(p,URANGE_SENTENCE))) {
					if ( (u.cell(p_word_next).queryPOS(POS_EU_ADI_JOK)) ||
						(u.cell(p_word_next).queryPOS(POS_EU_ADI_LGN)) ||
						(u.cell(p_word_next).queryPOS(POS_EU_ADI_TRN)) ||
						(u.wordIsFirst(p,URANGE_SENTENCE))	) {
							if (u.cell(p).queryPOS(POS_EU_ADI_LGN)) {
								u.cell(p).subPOS(POS_EU_ADI_LGN);	//quitar jok porque es ize
							}
							if ( u.cell(p).queryPOS(POS_EU_ADI_TRN))
										u.cell(p).subPOS(POS_EU_ADI_TRN);
					} else{
						u.cell(p).subPOS(POS_EU_DET);	//quitar ize porque es jok
					}
		 }
		 excep=TRUE;
		}


	if ((u.cell(p).queryPOS(POS_EU_DET)) &&
		 (u.cell(p).queryPOS(POS_EU_ADI_LGN)) &&
			!excep){

						if (primero==0) {
							if ( (u.cell(p_word_prev).queryPOS(POS_EU_ADI_JOK)) ||
								(u.cell(p_word_prev).queryPOS(POS_EU_ENKLITIKO)) ){
									u.cell(p).subPOS(POS_EU_DET);
									if (u.cell(p).queryPOS(POS_EU_IOR))
										u.cell(p).subPOS(POS_EU_IOR);
							}
							else {
									u.cell(p).subPOS(POS_EU_ADI_LGN);	//quitar det pq es ior
									if (u.cell(p).queryPOS(POS_EU_ADI_TRN))
										u.cell(p).subPOS(POS_EU_ADI_TRN);
							}
						}
						else
									if (u.cell(p).queryPOS(POS_EU_ADI_TRN))
										u.cell(p).subPOS(POS_EU_ADI_TRN);
									if (u.cell(p).queryPOS(POS_EU_ADI_LGN))
										u.cell(p).subPOS(POS_EU_ADI_LGN);


	}
}

//**********************************************************************

VOID LangEU_Categ::detior(UttWS &u, UttI p) {
int primero=0;
UttI p_word_prev=NULL;
if (p!=u.wordFirst())
	p_word_prev=u.wordPrev(p);
else
	primero=1;
	if ((u.cell(p).queryPOS(POS_EU_DET)) &&
		 (u.cell(p).queryPOS(POS_EU_IOR)) ){

						if (primero==0) {
							if ( (u.cell(p_word_prev).queryPOS(POS_EU_IZE)) ||
								(u.cell(p_word_prev).queryPOS(POS_EU_ADJ)) ||
							 (u.cell(p_word_prev).queryPOS(POS_EU_NONE)) ) {
									u.cell(p).subPOS(POS_EU_IOR);	//quitar ior pq es det
							} else
									u.cell(p).subPOS(POS_EU_DET);	//quitar det pq es ior
						}
						else
							u.cell(p).subPOS(POS_EU_DET);	//quitar det pq es ior
					}
}

//**********************************************************************

VOID LangEU_Categ::trnlgn(UttWS &u, UttI p)
{
int primero=0;

UttI p_word_prev=NULL,pp_word_prev=NULL;
if (p!=u.wordFirst())
	p_word_prev=u.wordPrev(p);
else
	primero=1;
#ifdef DEBUGPOS
  htts_warn("      Poscases: TRNLGN Palabra: %s Pos: %02X",u.cell(p).getWord(), u.cell(p).getPOS());
#endif

	if ((u.cell(p).queryPOS(POS_EU_ADI_TRN)) && (u.cell(p).queryPOS(POS_EU_ADI_LGN)) )	{

		if (primero==0) {

#ifdef DEBUGPOS
  htts_warn("      Poscases: TRNLGN No es la primera palabra.");
#endif

			if ( (u.cell(p_word_prev).queryPOS(POS_EU_ATZ_ADI1)) ||	(u.cell(p_word_prev).queryPOS(POS_EU_ADI_JOK)) )	{
							u.cell(p).subPOS(POS_EU_ADI_TRN);

#ifdef DEBUGPOS
  htts_warn("      Poscases: TRNLGN La anterior es un verbo JOK.");
#endif

			}
			else	{
				if  (u.cell(p_word_prev).queryPOS(POS_EU_PRT)) //ahal se las dos cosas
					if (u.wordPrev(p_word_prev)!=NULL) {
#ifdef DEBUGPOS
  htts_warn("      Poscases: TRNLGN Por lo menos dos palabras delante de esta.");
#endif
						pp_word_prev=u.wordPrev(p_word_prev);
						if ( (u.cell(pp_word_prev).queryPOS(POS_EU_ATZ_ADI1)) || (u.cell(pp_word_prev).queryPOS(POS_EU_ADI_JOK)) )	{
							u.cell(p).subPOS(POS_EU_ADI_TRN);
#ifdef DEBUGPOS
  htts_warn("      Poscases: TRNLGN La anterior a la anterior es un verbo jok.");
#endif
						}
						else {
#ifdef DEBUGPOS
  htts_warn("      Poscases: TRNLGN Palabra: %s Pos: %02X",u.cell(p).getWord(), u.cell(p).getPOS());
#endif
							u.cell(p).subPOS(POS_EU_ADI_LGN);
#ifdef DEBUGPOS
  htts_warn("      Poscases: TRNLGN La anterior a la anterior NO es un verbo jok.");
#endif
						}
					}
				}			
				if ((u.cell(p).queryPOS(POS_EU_ADI_TRN)) && (u.cell(p).queryPOS(POS_EU_ADI_LGN)))
					if(u.wordNext(p)!=NULL)
						if (u.cell(u.wordNext(p)).queryPOS(POS_EU_ADI_JOK))
							u.cell(p).subPOS(POS_EU_ADI_TRN);
						else u.cell(p).subPOS(POS_EU_ADI_LGN);
					else u.cell(p).subPOS(POS_EU_ADI_LGN);
			}
			else u.cell(p).subPOS(POS_EU_ADI_LGN);
		}

	
#ifdef DEBUGPOS
  htts_warn("      Poscases: TRNLGN Palabra: %s Pos: %02X",u.cell(p).getWord(), u.cell(p).getPOS());
#endif
}

//**********************************************************************

VOID LangEU_Categ::jntazk(UttWS &u, UttI p,int numWord) {
		if ((u.cell(p).queryPOS(POS_EU_LOT_AZK)) &&
		 (u.cell(p).queryPOS(POS_EU_LOT_JNT)) ){
//			if (p==u.wordLast()){	//esta al final
/* EVA: cambiado el rango que usaba inmass por URANGE_SENTENCE que es el 
que deberia haber usado */
			if (u.wordIsLast(p,URANGE_SENTENCE)){	//esta al final de frase, no utte
				u.cell(p).subPOS(POS_EU_LOT_JNT);  //quitar LOT_JNT,es LOT_AZK
				if (u.cell(p).queryPOS(POS_EU_PAUSE_AURRE))
					u.cell(p).subPOS(POS_EU_PAUSE_AURRE);
				if (u.cell(p).queryPOS(POS_EU_PAUSE_ATZE))
					u.cell(p).subPOS(POS_EU_PAUSE_ATZE);
			}

			else //puede ser juntura o invitacion a pausa
			//si es inv pausa por delante y por detras
				u.cell(p).subPOS(POS_EU_LOT_AZK);
				if ((u.cell(p).queryPOS(POS_EU_PAUSE_AURRE)) ||
					(u.cell(p).queryPOS(POS_EU_PAUSE_ATZE)) ) {
					//si n<=2 => juntura sin pausa
					if (numWord<=2)         {
									if (u.cell(p).queryPOS(POS_EU_PAUSE_AURRE))
										u.cell(p).subPOS(POS_EU_PAUSE_AURRE);
									if (u.cell(p).queryPOS(POS_EU_PAUSE_ATZE))
										u.cell(p).subPOS(POS_EU_PAUSE_ATZE);

					//	u.cell(p).setPOS(POS_EU_LOT_JNT);
					}
//					 else {	//si num>2 => invitar a pausa por delante
//							if (!strcmp(u.cell(p).getWord(),"eta") ||
//							!strcmp(u.cell(p).getWord(),"edo") )
//								u.cell(p).subPOS(POS_EU_LOT_JNT);
//						}
				}
		}
}

//**********************************************************************
VOID LangEU_Categ::izejok(UttWS &u, UttI p) {
int ultimo=0;

UttI p_word_next=NULL;
if (p!=u.wordLast())
	p_word_next=u.wordNext(p);
else
	ultimo=1;

		if ((u.cell(p).queryPOS(POS_EU_ADI_JOK)) &&
		 (u.cell(p).queryPOS(POS_EU_IZE)) ){

							if ((ultimo==0) && !(u.wordIsLast(p,URANGE_SENTENCE))) {
								if ( (u.cell(p_word_next).queryPOS(POS_EU_DET)) ||
								 (u.cell(p_word_next).queryPOS(POS_EU_ADJ)) ) {
										u.cell(p).subPOS(POS_EU_ADI_JOK);	//quitar jok porque es ize
								} else
									u.cell(p).subPOS(POS_EU_IZE);	//quitar ize porque es jok
							}
							else
								u.cell(p).subPOS(POS_EU_IZE); //al final de frase, es aditza
					}
}
//**********************************************************************
VOID LangEU_Categ::adjjok(UttWS &u, UttI p) {
int ultimo=0;

UttI p_word_next=NULL;

if (p!=u.wordLast())
	p_word_next=u.wordNext(p);
else
	ultimo=1;

		if ((u.cell(p).queryPOS(POS_EU_ADI_JOK)) &&
		 (u.cell(p).queryPOS(POS_EU_ADJ)) ){

							if ((ultimo==0) && !(u.wordIsLast(p,URANGE_SENTENCE))) {
								if ( (u.cell(p_word_next).queryPOS(POS_EU_ADI_TRN)) ||
								 (u.cell(p_word_next).queryPOS(POS_EU_ADI_LGN)) ) {
										u.cell(p).subPOS(POS_EU_ADJ);	//quitar adj porque es jok
								}
								else
								if ( (u.cell(p_word_next).queryPOS(POS_EU_DET)) ||
								 (u.cell(p_word_next).queryPOS(POS_EU_ADJ)) ) {
										u.cell(p).subPOS(POS_EU_ADI_JOK);	//quitar jok porque es adj
								} else
									u.cell(p).subPOS(POS_EU_ADJ);	//quitar adj porque es jok
							}
							else
								u.cell(p).subPOS(POS_EU_ADJ); //al final de frase, es aditza
					}
}

