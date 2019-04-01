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

Nombre fuente................ eu_ling.cpp
Nombre paquete............... aHoTTS
Lenguaje fuente.............. C++
Estado....................... -
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Borja Etxebarria
.............................

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
1.0.2    12/12/11  Inaki     Añadir opcion phtkatamotz para que las r al principio de palabras las pronuncie con r suave
1.0.1    20/10/08  Inaki     Añadir soporte para transcripción en diccionario (Nora)
1.0.1    03/10/07  Inaki     Define MODULO_1 y _2, ExternPOS y PhTSpeaker
1.0.0    31/01/00  borja     codefreeze aHoTTS v1.0
0.0.0    24/11/97  borja     Codificacion inicial.

======================== Contenido ========================
<DOC>
Metodos de {{LangEU_LingP}} que gestionan el preprocesado linguistico
completo (llamando a metodos de {{LangEU_Pauses}}, {{LangEU_PhTrans}} y
{{LangEU_Prosod}}).
</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include "eu_lingp.hpp"
#include "uti.h"
//#define DEBUG() fprintf (stderr,"file..: %s -- line..: %d\n",__FILE__,__LINE__);
#define DEBUG()
/**********************************************************/

BOOL LangEU_LingP::create( VOID )
{
	BOOL ret=TRUE;
	ret = ret && pos.create();
	#ifndef MODULE_1
	ret = ret && pauses.create();
	ret = ret && prosod.create();
	ret = ret && map.create();
	#endif
	return ret ;
}

/**********************************************************/

VOID LangEU_LingP::utt_lingp( Utt *ut)
{
	DEBUG()
	assert(ut->isKindOf("UttWS"));
	DEBUG()
	#ifdef IXA_POS_GF	//INAKI
	#ifndef MODULE_2
	if(str2bool(pos.get("EXTERN_POS_GF"), TRUE)){
		pos.utt_extern_pos(*(UttWS*)ut);
	}
	else
	#endif
	#ifndef MODULE_1
	if(str2bool(pos.get("EXTERN_POS_GF"), FALSE))
		pos.utt_extern_gf(*(UttWS*)ut);
	else
	#endif
	#endif
		pos.utt_pos(*(UttWS*)ut);
	#ifndef MODULE_1
	DEBUG()
	pauses.utt_pauses(*(UttWS*)ut);
	DEBUG()
	#ifdef HTTS_PROSO_VAL
	pau.utt_n_val_pause(*(UttPh*)ut);//Aritz
	DEBUG()
	#endif
	assert(ut->isKindOf("UttPh"));
	phtr.utt_w2phtr(*(UttPh*)ut);	//CON XML SALTAR; CORREGIR
	DEBUG()
	prosod.utt_prosody(*(UttPh*)ut);
	DEBUG()
	map.utt_phmap(*(UttPh*)ut);
	#ifdef HTTS_PROSO_VAL
	DEBUG()
	pros.utt_n_val_pitch(*(UttPh*)ut);//Aritz
	DEBUG()
	pros.utt_n_val_rate(*(UttPh*)ut);//Aritz
	DEBUG()
	pros.utt_n_val_volume(*(UttPh*)ut);//Aritz
	DEBUG()
	pros.utt_n_val_emphasis(*(UttPh*)ut);//Aritz
	DEBUG()
	pros.utt_n_val_range(*(UttPh*)ut);//Aritz
	#endif
	#endif
}
/**********************************************************/

VOID LangEU_LingP::utt_pos( Utt *ut )
{
	assert(ut->isKindOf("UttWS"));//INMASS
	#ifndef MODULE_2
	#ifdef IXA_POS_GF //INAKI
	if(str2bool(pos.get("EXTERN_POS_GF"), FALSE))
		pos.utt_extern_pos(*(UttWS*)ut);
	else
	#endif
	#endif
	#ifndef MODULE_1
	#ifdef IXA_POS_GF
	if(str2bool(pos.get("EXTERN_POS_GF"), FALSE))
		pos.utt_extern_gf(*(UttWS*)ut);
	else
	#endif
	#endif
		pos.utt_pos(*(UttWS*)ut);

}
#ifdef HTTS_PROSO_VAL
/******************************************************/
VOID LangEU_LingP::utt_n_val_pause(Utt * ut)//Aritz
{
	assert(ut->isKindOf("UttPh"));
	pau.utt_n_val_pause(*(UttPh*)ut);
}
#endif
/******************************************************/
VOID LangEU_LingP::utt_pauses( Utt *ut )
{
	assert(ut->isKindOf("UttWS"));
	pauses.utt_pauses(*(UttWS*)ut);
}
#ifdef HTTS_PROSO_VAL
/*********************************************************/
/*Aritz*/VOID LangEU_LingP::utt_new_val(Utt *ut)
{
	assert(ut->isKindOf("UttPh"));
	pros.utt_n_val_pitch(*(UttPh*)ut);
	pros.utt_n_val_rate(*(UttPh*)ut);
	pros.utt_n_val_volume(*(UttPh*)ut);
	pros.utt_n_val_emphasis(*(UttPh*)ut);
	pros.utt_n_val_range(*(UttPh*)ut);
}
#endif
/**********************************************************/

VOID LangEU_LingP::utt_phtrans( Utt *ut )
{
	assert(ut->isKindOf("UttPh"));
	phtr.utt_w2phtr(*(UttPh*)ut);
}

/**********************************************************/

VOID LangEU_LingP::utt_prosod( Utt *ut )
{
	assert(ut->isKindOf("UttPh"));
	prosod.utt_prosody(*(UttPh*)ut);
}

/**********************************************************/

VOID LangEU_LingP::utt_map( Utt *ut )
{
	assert(ut->isKindOf("UttPh"));
	map.utt_phmap(*(UttPh*)ut);
}

/**********************************************************/

BOOL LangEU_LingP::set( const CHAR *param, const CHAR * value )
{
	BOOL ret=FALSE;
	if (!strcmp(param,"PhTSimple")) {
		phtr.setPhTSimple(str2bool(value,TRUE));
		return TRUE;
	}
	if (!strcmp(param,"PhTKatamotz")) {
		phtr.setPhTKatamotz(str2bool(value,TRUE));
		return TRUE;
	}
	ret = ret || pos.set(param,value);
	#ifndef MODULE_1
	ret = ret || pauses.set(param,value);
	ret = ret || prosod.set(param,value);
	ret = ret || map.set(param,value);
	if (!strcmp(param,"PhTSpeaker")) {//INAKI: para transcripcion de karolina
		phtr.setPhTSpeaker(value);
			return TRUE;
	}
	if (!strcmp(param,"HDicDB")) {//INAKI: para transcripcion desde diccionario
		phtr.setPhTHDicName(value);
			return TRUE;
	}
	#endif
	return ret;
}

/**********************************************************/

const CHAR *LangEU_LingP::get( const CHAR *param )
{
	const CHAR *s;
	s=pos.get(param); if (s) return s;
	#ifndef MODULE_1
	if (!strcmp(param,"PhTSpeaker")) {//INAKI: para transcripcion de karolina
		s=phtr.getPhTSpeaker();
		if(s) return s;
	}
	s=pauses.get(param); if (s) return s;
	s=prosod.get(param); if (s) return s;  
	s=map.get(param); if (s) return s;
	#endif
	return NULL;
}

/**********************************************************/
