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

Nombre fuente................ eu_pause.cpp
Nombre paquete............... aHoTTS
Lenguaje fuente.............. C++
Estado....................... -
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Borja Etxebarria
.............................

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
1.1.0 	 12/07/10  Inaki	 Añadir celdas para modificar la prosodia desde el texto (proyecto Aritz) HTTS_PROSO_VAL
1.0.1    13/05/00  borja     pau_nsillab=0
1.0.0    31/01/00  borja     codefreeze aHoTTS v1.0
0.0.0    24/11/97  borja     Codificacion inicial.

======================== Contenido ========================
<DOC>
Metodos de {{LangEU_Pauses}} para gestionar los modelos de
colocacion de las pausas.

Si PauNSilab se pone a 0, no se meten pausas automaticas.
</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include <assert.h>
#include "httsmsg.h"
#include "eu_lingp.hpp"
#include "xx_uti.hpp"
#include "uti.h"

/**********************************************************/

LangEU_Pauses::LangEU_Pauses( VOID )
{
	created=FALSE;

	pau_nsilab=0;
	durmean=1;
	pbreath=0;

/* seleccion de modelos por defecto. El ultimo de cada tipo
es el que se selecciona. */
#ifdef HTTS_EU_PAU2
	pau_model="Pau2";
#endif
#ifdef HTTS_EU_PAU1
	pau_model="Pau1";
#endif

#ifdef HTTS_EU_PAU2
//	pau2_dummy_ptr=NULL;
#endif
}

/**********************************************************/

LangEU_Pauses::~LangEU_Pauses( VOID )
{
#ifdef HTTS_EU_PAU2
//	if (pau2_dummy_ptr) { xfree(pau2_dummy_ptr); pau2_dummy_ptr=NULL; }
#endif
}

/**********************************************************/

BOOL LangEU_Pauses::create( VOID )
{
#ifdef HTTS_EU_PAU2
//	pau2_dummy_ptr=xmalloc(10);
#endif

	created=TRUE;
	return TRUE;
}

/**********************************************************/

VOID LangEU_Pauses::utt_pauses(UttWS & ut)
{
	assert(created);

	uttDefaultSentence(ut);  // aseguramos que hay alguna sentence inicial

#ifdef HTTS_EU_PAU1
	if (!strcmp(pau_model,"Pau1")) utt_pau1(ut); else
#endif
#ifdef HTTS_EU_PAU2
	if (!strcmp(pau_model,"Pau2")) utt_pau2(ut); else
#endif
	htts_error("Invalid LangEU_Pauses::pau_model (%s)",(const CHAR*)pau_model);
}

/**********************************************************/

BOOL LangEU_Pauses::set( const CHAR *param, const CHAR * value )
{
	double d;
	int code = str2d(value,&d);

	if (!strcmp(param,"PauModel")&& !created) { pau_model=value; return TRUE; }

	else if (!strcmp(param,"PauNSilab")) pau_nsilab=d;

	// DurMean realmente se gestiona en la prosodia. Aqui espiamos su valor :)
	else if (!strcmp(param,"DurMean")) {
		durmean=d;
//EVA: anadido para que el parametro DurMean llegue hasta el modelo de
//duracion
		return FALSE;
}

	else if (!strcmp(param,"BreathProb")) pbreath=d;

#ifdef HTTS_EU_PAU2
//	else if (!strcmp(param,"pau2_dummy")) {
//		if (!created) return FALSE;  // if needed
//		// do whatever
//		return TRUE;
//	}
#endif

	else return FALSE;

	if (code) htts_error("LangEU_Pauses: invalid value for %s (%s)",param,value);
	return TRUE;
}

/**********************************************************/

const CHAR *LangEU_Pauses::get( const CHAR *param )
{
#define VALRET(x) { sprintf(buf,"%g",(double)x); buf_get=buf; return buf_get; }
	char buf[20];

	if (!strcmp(param,"PauModel")) return pau_model;
	if (!strcmp(param,"PauNSilab")) VALRET(pau_nsilab);
	if (!strcmp(param,"BreathProb")) VALRET(pbreath);

#ifdef HTTS_EU_PAU2
//	if (!strcmp(param,"pau2_dummy")) return "dummy";
#endif

	return FALSE;
}

/**********************************************************/
#ifdef HTTS_PROSO_VAL
VOID LangEU_Pauses::utt_n_val_pause(UttPh & ut)//Aritz
{
	UttI q,r,s;

	int val_etiq;



	for(q=ut.wordFirst();q!=0;q=ut.wordNext(q))
		{
			//Hay que añadirle la etiqueta a la palabra anterior
			if(ut.cell(q).val_break)
			{
					val_etiq=ut.cell(q).val_break;
					r=ut.cellAddBefore(q);
					ut.cell(r).setPhone('_');
					//ut.cell(r).setPause(UPAUSE_SNEUTR);//Para que la pausa sea UPAUSE_SNEUTR
					ut.cell(r).setDur(val_etiq);

			}

		}


}
#endif
/************************************************************/
