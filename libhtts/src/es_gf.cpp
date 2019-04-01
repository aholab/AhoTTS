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
(C) 2004 TSR/Aholab - ETSII/IT Bilbao (UPV/EHU)

Nombre fuente................ eu_gf.cpp
Nombre paquete............... aHoTTS
Lenguaje fuente.............. C++
Estado....................... -
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Inma Sardon
............................. inigos

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
1.1.0    24/03/04  inigos     adaptacion al castellano
1.0.0			   inmas     Codificacion inicial.

======================== Contenido ========================
<DOC>
eu_gf.cpp:programa para separar una utterance en grupos funcionales
</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#ifndef __ES_GF_CPP__
#define __ES_GF_CPP__
#include "es_lingp.hpp"
#include "es_pos.hpp"
#include "tnor.h"

/**********************************************************/

LangES_GF::LangES_GF( VOID )
{
	created=FALSE;
}

/**********************************************************/

LangES_GF::~LangES_GF( VOID )
{
}

/**********************************************************/

BOOL LangES_GF::create( VOID )
{
	created=TRUE;
	return TRUE;
}

/**********************************************************/

BOOL LangES_GF::set( const CHAR *param, const CHAR * value )
{
	if (!strcmp(param,"HDicDBName")&& !created) { dbName=value; return TRUE; }
	else return FALSE;
}

/**********************************************************/

const CHAR *LangES_GF::get( const CHAR *param )
{
#define VALRET(x) { sprintf(buf,"%g",(double)x); buf_get=buf; return buf_get; }
	if (!strcmp(param,"HDicDBName")) return dbName;

	return FALSE;
}

/**********************************************************/

VOID LangES_GF::utt_gf(UttWS &u)
{
	INT gf=7;
	INT previo = 0, invitacion = 0;
	INT tnor;

	// ############################################################################
	// # previo | utilización 
	// # --------------------------------------------------------------------------
	// #    0   | Por defecto
	// #    1   | Preposición o Demostrativo  + Relativo ("el que", "por quien")
	// #    2   | Verbo conjugado (aumenta la posibilidad de pausa)
	// #    3   | SPI: "la", "las", o "los"  o  SPV: "lo" para los RELA ("las que")
	// ############################################################################

	UttI p=NULL;	//indice para recorrer las celdas

	//recorrido palabra por palabra
	for(p=u.wordFirst();p!=0;p=u.wordNext(p))
	{
		if(previo ==2) invitacion=1;

		switch(u.cell(p).getPOS())
		{

		// ##########
		// # gf = 7 #
		// ##########

		case POS_ES_CONJ:
			gf=7;	
			previo = 0;
			break;
		// ##########
		// # gf = 6 #
		// ##########
		case POS_ES_RELA:
			if(previo == 1 || previo == 3)
			{
				u.cell(u.wordPrev(p)).setFGrp(6);
			}
		case POS_ES_SUSP_VERB:
			if(!strcmp("lo", u.cell(p).getWord()))
			{
				previo = 3;
			}
		case POS_ES_VERB_ENCL:
		case POS_ES_VERB_COMP:
			gf=6;
			previo = 0;
			break;
		case POS_ES_VERB_SIMP:
		case POS_ES_VERB_IICC:
			if(previo == 1)
			{
				gf = 5;
			}
			else
			{
				gf=6;
			}
			previo = 2;
			break;
		// ##########
		// # gf = 5 #
		// ##########
		case POS_ES_ADVE:
			if(!strcmp("no", u.cell(p).getWord()))
			{
				gf=6;
				previo = 0;
				break;
			}
			if(previo == 2)
			{
				gf = 6;
				previo = 2;
				break;
			}
		case POS_ES_VERB_GERU:
			previo = 0;
			gf=5;
			break;
		case POS_ES_VERB_SIMP_DEBIL:  // lo metemos pero hay que andarse con cuidado
			if(previo == 1)
			{
				gf = 4;
			}
			else
			{
				gf=5;
			}
			previo = 2;
			break;

		case POS_ES_DEMO_NOVERB:
			if(p != u.wordFirst())
			{
				switch(u.cell(u.wordPrev(p)).getPOS())
				{
				case POS_ES_ADVE:
				case POS_ES_DEMO_IND:
					gf = 6;
					break;
				case POS_ES_SUSP_IND:
					gf = 3;
					break;
				default:
					gf = 5;
					break;
				}
			}
			else gf = 5;
		case POS_ES_PREP:
			previo = 1;
			gf=5;
			break;
		// ##########
		// # gf = 4 #
		// ##########
		case POS_ES_SUSP_IND:
			gf = 4;			
			previo = 0;
			if(!strcmp("la", u.cell(p).getWord()) || !strcmp("las", u.cell(p).getWord()) ||
				!strcmp("los", u.cell(p).getWord()))
			{
				previo = 3;
			}
			if(p != u.wordFirst() && u.cell(u.wordPrev(p)).getPOS() == POS_ES_SUSP_IND)
			{
				gf = 5;
			}
			break;
		// ##########
		// # gf = 3 #
		// ##########
		case POS_ES_VERB_IR1C:
		case POS_ES_VERB_IR2C:
		case POS_ES_VERB_IR3C:
		case POS_ES_CARD_NOVERB:
		case POS_ES_POSE_NOVERB:
		case POS_ES_INDNUM_NOVERB:
			previo = 0;
			gf=3;
			break;
		case POS_ES_DEMO_IND:
			previo = 1;
			gf=3;
		// #########
		// # gf -- #
		// #########
		case POS_ES_VERB_PPIO:
		case POS_ES_OTRO_SUST:
		case POS_ES_RELIE:
		case POS_ES_NONE:
		case POS_ES_SUSP_NPV:
		case POS_ES_POSE_IND:
		case POS_ES_INDNUM_IND:
		case POS_ES_CARD_IND:
		default:
			if(gf>0) gf--;
			previo = 0;
			break;
		}

		tnor = u.cell(p).getTNor();

		// Si tenemos algo que puede ser nombre y no es comienzo de sentencia
		if(tnor == UTYPENOR_NAME && p!=u.wordFirst())
		{
			gf=5;
		}


		if(invitacion)
		{
			gf += 3;
			if(gf > 6) gf = 6;
			if(previo==2) gf=5;
		}
		invitacion = 0;
		u.cell(p).setFGrp(gf);
	}
}
#endif


