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

Nombre fuente................ es_w2ph.cpp
Nombre paquete............... aHoTTS
Lenguaje fuente.............. C++
Estado....................... -
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Borja Etxebarria
.............................

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
2.0.0    20/10/08  inaki     Añadir Grupos Acentuales Castellano
1.0.0    31/01/00  borja     codefreeze aHoTTS v1.0
0.0.0    24/11/97  borja     Codificacion inicial.

======================== Contenido ========================
<DOC>
Metodos de {{LangES_PhTrans}} para gestionar la conversion de palabras
a fonemas (pasando por letras).
</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include "es_lingp.hpp"
#include "xx_uti.hpp"

/**********************************************************/

VOID LangES_PhTrans::utt_w2phtr(UttPh & ut)
{
	UttI wo;

	// primero expandimos las palabras a letras
	for (wo=ut.wordFirst(); wo!=0; ) {
		UttI wn=ut.wordNext(wo);  // pre-fetch de next. mas rapido porque aun no esta expandido
		ut.word2Char(wo);
		wo=wn;
	}

	// asegurar que hay alguna sentencia
	uttDefaultSentence(ut);

	// ahora, pausa a pausa, convertir letra -> fonema
	for (UttI se=ut.pausegrFirst(); se!=0; se=ut.pausegrNext(se))
		pausegr_ch2ph(ut, se);

	// silabificar palabra a palabra
	for (wo=ut.wordFirst(); wo!=0; wo=ut.wordNext(wo))
		word_syllab(ut, wo);

	iu2jw(ut); // detecta diptongos/triptongos

	// acentuar palabras que no lo esten ya
	for (wo=ut.wordFirst(); wo!=0; wo=ut.wordNext(wo))
		if (!word_stressed(ut,wo)) word_stress(ut, wo);  //acentuar

	uttPausePh(ut);  // fonemas de silencio en pausas
	
	//ut.foutput(stderr);
	
	//establecer grupos acentuales
	/*************************
	1->Agudas
	2->Llanas
	3->Esdrújulas
	4->Sobre-esdrújulas
	las palabras átonas tienen como valor GF el de la palabra tónica siguiente + 5
	
	*************************************/
	/*for (UttI se=ut.pausegrFirst(); se!=0; se=ut.pausegrNext(se)){
		UttI p=ut.syllableFirst(se, URANGE_PAUSE);
		//mirar si es acentuada
		BOOL ACENTUADA=FALSE;
		//fprintf(stderr, "word=%s\n", ut.cell(p).getWord());
		for (UttI q=ut.phoneFirst(p,URANGE_SYLLABLE); q!=0; q=ut.phoneNext(q, URANGE_SYLLABLE)){
			//fprintf(stderr, "\tphone=%s\n", phone_tosampa(ut.cell(q).getPhone()));
			if(q==0)
				break;
			if(ut.cell(q).getStress()!=USTRESS_NONE){
				ACENTUADA=TRUE;
				break;
			}
		}
		
		if(ACENTUADA){//mirar a qué distancia está la siguiente sílaba acentuada para determinar el tipo de grupo acentual
			int contador=1;
			for (UttI p2=ut.syllableNext(p,URANGE_PAUSE); p2!=0; p2=ut.syllableNext(p2, URANGE_PAUSE)){
				if(p2==0)
					break;
				ACENTUADA=FALSE;
				for (UttI q=ut.phoneFirst(p2,URANGE_SYLLABLE); q!=0; q=ut.phoneNext(q, URANGE_SYLLABLE)){
					if(ut.cell(q).getStress()!=USTRESS_NONE){
						ACENTUADA=TRUE;
						break;
					}
				}
				if(ACENTUADA)
					break;
				++contador;
			}
			ut.cell(p).setAGrp(contador);
		}
		
		else
			ut.cell(p).setAGrp(0);
		ut.foutput(stderr);
		
	}*/
	int cont_silabas_grupo_acentual=0; //se usa para acumular silabas en palabras átonas y sumarlas en la siguiente tónica
	for (wo=ut.wordFirst(); wo!=0; wo=ut.wordNext(wo)){
		//mirar si la palabra tiene sílaba acentuada y en qué posición está
		int cont_silaba=0;
		BOOL ACENTUADA=FALSE;
	/*	for (Lix p=ut.syllableFirst(wo,URANGE_WORD); p!=0; p=ut.syllableNext(p,URANGE_WORD)){
			for(Lix q=ut.phoneFirst(p, URANGE_SYLLABLE); q!=0; q=ut.phoneNext(q,URANGE_SYLLABLE) ){			
				if (ut.cell(q).getStress()!=USTRESS_NONE){
					ACENTUADA=TRUE;
					//fprintf(stderr, "\tphone=%s\n", phone_tosampa(ut.cell(q).getPhone()));
					break;
				}
			}
			cont_silaba++;
			if(ACENTUADA)
				break;
		}
	*/	for (Lix p=ut.syllableLast(wo,URANGE_WORD); p!=0; p=ut.syllablePrev(p,URANGE_WORD)){
			for(Lix q=ut.phoneFirst(p, URANGE_SYLLABLE); q!=0; q=ut.phoneNext(q,URANGE_SYLLABLE) ){			
				if (ut.cell(q).getStress()!=USTRESS_NONE){
					ACENTUADA=TRUE;
					//fprintf(stderr, "\tphone=%s\n", phone_tosampa(ut.cell(q).getPhone()));
					break;
				}
			}
			cont_silaba++;
			if(ACENTUADA)
				break;
			
		}
		if(!ACENTUADA){
			ut.cell(wo).setAGrp(9);
			//cont_silabas_grupo_acentual+=cont_silaba;
		}
		else{
			ut.cell(wo).setAGrp(cont_silaba + cont_silabas_grupo_acentual);
			//cont_silabas_grupo_acentual=0;
		}
	}
	//para los que tienen GA=9, le ponemos el de la siguiente palabra que no tenga 9, sumando 5
	for (wo=ut.wordFirst(); wo!=0; wo=ut.wordNext(wo)){
		if(ut.cell(wo).getAGrp() == 9){
			UttI word_next=ut.wordNext(wo);
			while (word_next != NULL && ut.cell(word_next).getAGrp() == 9)
				word_next = ut.wordNext(word_next);
			if(word_next!=NULL)
				ut.cell(wo).setAGrp(5+ut.cell(word_next).getAGrp() );
		}
	}
}

/**********************************************************/
