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
#ifndef __ES_LINGP_HPP__
#define __ES_LINGP_HPP__

/**********************************************************/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/*
(C) 1997 TSR/Aholab - ETSII/IT Bilbao (UPV/EHU)

Nombre fuente................ es_lingp.hpp
Nombre paquete............... aHoTTS
Lenguaje fuente.............. C++
Estado....................... -
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Borja Etxebarria
.............................

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
1.0.5	 23/02/12  Inaki	speaker dependent transcription
1.0.4	 13/12/11  Inaki	añadir opcion phtkatamotz para no pronunciar como rr las r al principio de una palabra
1.0.3	 12/07/10  Inaki	 Añadir celdas para modificar la prosodia desde el texto (proyecto Aritz) HTTS_PROSO_VAL
1.0.2    20/10/08  Inaki     Añadir soporte para modelo pth3 (pitch por corpus)
1.0.1    03/10/07  Inaki     pause_syllab_stress silabificacion no coarticulada (XML)
1.0.0    31/01/00  borja     codefreeze aHoTTS v1.0
0.0.0    24/11/97  borja     Codificacion inicial.

======================== Contenido ========================
<DOC>
</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/
#include "htts_cfg.h" //INAKI
#include "lingp.hpp"
#include "phone.h"
#include "phmap.hpp"
#include "es_pos.hpp"

/**********************************************************/
/* fonemas del castellano, nombres mas faciles de recordar
que SAMPA  (pero realmente mapeados a los SAMPA
correspondientes) */
/* $$$ duda: jj  hielo  rayo:  se supone que es lo mismo? */

#define PHES_p  PH_p
#define PHES_b  PH_b
#define PHES_t  PH_t
#define PHES_d  PH_d
#define PHES_k  PH_k
#define PHES_g  PH_g
#define PHES_m  PH_m
#define PHES_n  PH_n
#define PHES_ntilde PH_J
#define PHES_ch PH_tS
#define PHES_f  PH_f
#define PHES_z  PH_T
#define PHES_s  PH_s
#define PHES_y  PH_jj
#define PHES_j  PH_x
#define PHES_l  PH_l
#define PHES_ll PH_L
#define PHES_rr PH_rr
#define PHES_r  PH_r
#define PHES_i  PH_i
#define PHES_e  PH_e
#define PHES_a  PH_a
#define PHES_o  PH_o
#define PHES_u  PH_u
#define PHES_baprox  PH_B
#define PHES_daprox  PH_D
#define PHES_gaprox  PH_G
#define PHES_iaprox  PH_j
#define PHES_uaprox  PH_w

#define PHES_E	PH_V  //lo usamos para el gallego, es un parche
#define PHES_O	PH_Q  //lo usamos para el gallego, es un parche
#define PHES_N	PH_N  //lo usamos para el gallego, es un parche
#define PHES_h	PH_h  //lo usamos para el gallego, es un parche

/**********************************************************/

class LangES_PhUti {
public:
	static UttI syllable_vowel( UttPh &u, UttI syl );
	static UttI syllable_xvowel( UttPh &u, UttI syl );
	static BOOL phIsVowel( Phone ph );
	static BOOL phIsValidCC( Phone ph1, Phone ph2 );
	static BOOL phIsDiptongo( Phone ph1, Phone ph2, Stress acc1, Stress acc2 );
	static BOOL phIsTriptongo( Phone ph1, Phone ph2, Phone ph3,
			Stress acc1, Stress acc3 );
};

/**********************************************************/
class LangES_POS {
	private:
		BOOL created;
		LangES_Categ categ;
		LangES_GF	grupof;

public:
	LangES_POS( VOID );
	~LangES_POS();
	BOOL create(VOID);

	VOID utt_pos( UttWS & u );
	VOID utt_categ( UttWS & u );
	VOID utt_gf( UttWS & u );
	BOOL set( const CHAR *param, const CHAR *val );
	const CHAR *get( const CHAR *param );
};
/**********************************************************/

class LangES_Pauses {
private:
	BOOL created;
	String buf_get;

public:
	LangES_Pauses( VOID );
	~LangES_Pauses();
	BOOL create(VOID);

	BOOL set( const CHAR *param, const CHAR *val );
	const CHAR *get( const CHAR *param );

	VOID utt_pauses(UttWS & ut);
	#ifdef HTTS_PROSO_VAL
	VOID utt_n_val_pause(UttPh& ut);//Aritz
	#endif

private:
#ifdef HTTS_ES_PAU2
	VOID utt_pau2( UttWS &ut );
//	VOID *pau2_dummy_ptr;
	/* Busca "dummy" para ir viendo donde se puede ir metiendo codigo
	especifico para un metodo pausador . */
#endif

protected:
	DOUBLE pau_nsilab;
	DOUBLE durmean;
	DOUBLE pbreath; // probabilidad breath

	String pau_model;
};

/**********************************************************/

class LangES_PhTrans {
private:
	String pht_speaker; //INAKI: speaker dependent transcriptions
public:
	LangES_PhTrans( VOID ) { phtkatamotz=0; }
	VOID setPhTKatamotz( BOOL katamotz ) { phtkatamotz=katamotz; }
	BOOL getPhTKatamotz( VOID ) { return phtkatamotz; }
	VOID utt_w2phtr(UttPh & ut);
	//----------------------------------------------------------------------------------
	VOID setPhTSpeaker( const char *speaker ) { pht_speaker=speaker; }//INAKI:
	const char *getPhTSpeaker( VOID ) { return pht_speaker; }//INAKI: speaker dependent transcriptions
private:
	BOOL phtkatamotz; //INAKI: para no pronunciar como rr las r a principio de palabra
	VOID pausegr_ch2ph(UttPh &u, UttI senp );
	VOID word_syllab( UttPh &u, UttI wordp );
	VOID iu2jw( UttPh &u );

	VOID word_stress( UttPh &u, UttI wordp );
	VOID syllable_stress( UttPh &u, UttI syl );
	BOOL word_stressed( UttPh &u, UttI wordp );
};

/**********************************************************/

class LangES_Prosod {
private:
	String buf_get;
  BOOL created;

public:
	LangES_Prosod( VOID );
	~LangES_Prosod();
	BOOL create(VOID);

	BOOL set( const CHAR *param, const CHAR *val );
	const CHAR *get( const CHAR *param );
	#ifdef HTTS_PROSO_VAL
	VOID utt_n_val_pitch(UttPh &ut);//Aritz
	VOID utt_n_val_rate(UttPh & ut);//Aritz
	VOID utt_n_val_volume(UttPh & ut);//Aritz
	VOID utt_n_val_emphasis(UttPh & ut);//Aritz
	VOID utt_n_val_range(UttPh & ut);//Aritz
	#endif

	VOID utt_prosody(UttPh & ut);

private:
	VOID utt_emphasis( UttPh &u );

protected:
	DOUBLE pth_mean;
	DOUBLE pth_dev;
	DOUBLE dur_mean;
	DOUBLE dur_dev;
	DOUBLE pow_mean;
	DOUBLE pow_dev;

	String pth_model;
	String dur_model;
	String pow_model;
	String prosdbname; //EVA Para que el castellano pueda tener tambien modelo 3 si hace falta...

};

/**********************************************************/

class LangES_LingP : public LingP {
private:
	LangES_POS	pos;
	LangES_Pauses  pauses;
	LangES_PhTrans phtr;
	//LangES_Prosod  prosod;
	#ifdef HTTS_PROSO_VAL
	LangES_Prosod  pros;//Aritz
	LangES_Pauses  pau;//Aritz
	#endif
	PhMap map;

public:
	virtual ~LangES_LingP() {}; //needed to remove a warning
	BOOL create( VOID );

	VOID utt_lingp( Utt *u );
	VOID utt_pos( Utt *u );
	VOID utt_pauses( Utt *u );
	VOID utt_phtrans( Utt *u );
	//VOID utt_prosod( Utt *u );
	VOID utt_map( Utt *u );
	#ifdef HTTS_PROSO_VAL
	VOID utt_new_val(Utt *u);//Aritz
	VOID utt_n_val_pause(Utt *u);//Aritz
	#endif

	BOOL set( const CHAR *param, const CHAR *val );
	const CHAR *get( const CHAR *param );
};

int quitar_acentos(CHAR *word_act);

/**********************************************************/
/* tipos de Part of Speech: */
enum {
	POS_ES_NONE				=	0x00000,
	POS_ES_SUSP_VERB		=	0x00001,
	POS_ES_SUSP_IND			=	0x00002,
	POS_ES_SUSP_NPV			=	0x00004,
	POS_ES_DEMO_NOVERB		=	0x00008,
	POS_ES_DEMO_IND			=	0x00010,
	POS_ES_POSE_NOVERB		=	0x00020,
	POS_ES_POSE_IND			=	0x00040,
	POS_ES_RELA				=	0x00080,
	POS_ES_RELIE			=	0x00100,
	POS_ES_INDNUM_NOVERB	 =	0x00200,
	POS_ES_INDNUM_IND		=	0x00400,
	POS_ES_CARD_NOVERB		=	0x00800,
	POS_ES_CARD_IND			=	0x01000,
	POS_ES_ADVE				=	0x02000,
	POS_ES_PREP				=	0x04000,
	POS_ES_CONJ				=	0x08000,
	POS_ES_VERB_SIMP		=	0x10000,
	POS_ES_VERB_SIMP_DEBIL	=	0x10001,
	POS_ES_VERB_COMP		=	0x10010,
	POS_ES_VERB_GERU		=	0x10020,
	POS_ES_VERB_PPIO		=	0x10040,
	POS_ES_VERB_ENCL		=	0x10080,
	POS_ES_VERB_IR1C		=	0x10100,
	POS_ES_VERB_IR2C		=	0x10200,
	POS_ES_VERB_IR3C		=	0x10400,
	POS_ES_VERB_IICC		=	0x10800,
	POS_ES_OTRO_SUST		=	0x20000,
	POS_ES_ADJ				=	0x40000, //se le añade el POS de adjetivo
};
/**********************************************************/

#endif

/*tipos de grupos funcionales*/
/*enum {
	GF_ES_NONE=0,
	GF_ES_MUY_BAJO,
	GF_ES_BAJO, // 2
	GF_ES_MEDIO,
	GF_ES_NORMAL, // 4
	GF_ES_ALTO,
	GF_ES_MUY_ALTO,
	GF_ES_EXTREMO	//7
};*/
