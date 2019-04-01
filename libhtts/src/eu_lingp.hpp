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
#ifndef __EU_LINGP_HPP__
#define __EU_LINGP_HPP__

/**********************************************************/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/*
(C) 1997 TSR/Aholab - ETSII/IT Bilbao (UPV/EHU)

Nombre fuente................ eu_lingp.hpp
Nombre paquete............... aHoTTS
Lenguaje fuente.............. C++
Estado....................... -
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Borja Etxebarria
................................... Nora Barroso

Version  dd/mm/aa		 Autor     Proposito de la edicion
--------------------------------------------------------------
2.1.7	 13/12/11			Inaki	añadir opcion phtkatamotz para no pronunciar como rr las r al principio de una palabra
2.1.6	 12/07/10			Inaki	Añadir celdas para modificar la prosodia desde el texto (proyecto Aritz) HTTS_PROSO_VAL
2.1.5    05/01/10           Inaki	Añadir modelos de duracion y pitch para amaia (dur_amaia, pth3_amaia)
2.1.4    20/03/09           Inaki	Añadir modelos de duracion para emociones karolina (dur_emo)
2.1.3    20/10/08           Inaki	Añadir modelos de duracion para emociones (dur_sad y dur_happy)	(Eva)
2.1.2    20/10/08           Inaki	Añadir soporte para transcripción en diccionario (Nora)
2.1.1    03/10/07           Inaki	Define MODULO_1 y _2, ExternPOS, PhTSpeaker y z_T salbuespena
2.1.0		16/03/05		Nora		Hitzen transkripzio fonetikoa hitzegian agertu ahal izateko (EZ EZARRITA)
2.0.0		22/02/05		Nora		Transkripzio fonetikoaren salbuespen motak POS etiketen zerrendan gehitu.
											Transkripzio fonetikoaren salbuspena zein den jakiteko behar diren funtzioen definizioa.
1.0.1    08/05/00		borja     codigos POS_EU_??
1.0.0    31/01/00		borja     codefreeze aHoTTS v1.0
0.0.0    24/11/97		borja     Codificacion inicial.

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
#include "eu_pos.hpp"

/**********************************************************/
/* fonemas del euskara, nombres mas faciles de recordar
que SAMPA  (pero realmente mapeados a los SAMPA
correspondientes) */

#define PHEU_p  PH_p
#define PHEU_b  PH_b
#define PHEU_t  PH_t
#define PHEU_d  PH_d
#define PHEU_k  PH_k
#define PHEU_g  PH_g
#define PHEU_m  PH_m
#define PHEU_n  PH_n
#define PHEU_ntilde PH_J
#define PHEU_f  PH_f
#define PHEU_s  PH_s
#define PHEU_jj PH_jj
#define PHEU_l  PH_l
#define PHEU_ll PH_L
#define PHEU_rr PH_rr
#define PHEU_r  PH_r
#define PHEU_i  PH_i
#define PHEU_e  PH_e
#define PHEU_a  PH_a
#define PHEU_o  PH_o
#define PHEU_u  PH_u

#define PHEU_z  PH_X
#define PHEU_tZ PH_tZ  //(antes era PH_P)
#define PHEU_tt PH_Q
#define PHEU_x  PH_S
#define PHEU_ts PH_ts
#define PHEU_dj PH_dj
#define PHEU_tx PH_tS
#define PHEU_T  PH_T
#define PHEU_j  PH_x

#define PHEU_dZ PH_dZ

#define PHEU_iaprox PH_j
#define PHEU_uaprox PH_w

#define PHEU_E	PH_V  //lo usamos para el gallego, es un parche
#define PHEU_O	PH_Q  //lo usamos para el gallego, es un parche
#define PHEU_N	PH_N  //lo usamos para el gallego, es un parche
#define PHEU_h	PH_h  //lo usamos para el gallego, es un parche

//otros que por ahora no usamos
#define PHEU_baprox  PH_B
#define PHEU_daprox  PH_D
#define PHEU_gaprox  PH_G

/**********************************************************/

class LangEU_PhUti {
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

class LangEU_Categ;
class LangEU_GF;

class LangEU_POS {
	private:
		BOOL created;
		LangEU_Categ categ;
		LangEU_GF	grupof;

public:
	LangEU_POS( VOID );
	~LangEU_POS();
	BOOL create(VOID);

	VOID utt_pos( UttWS & u );

	VOID utt_categ( UttWS & u );
	VOID utt_gf( UttWS & u );
	BOOL set( const CHAR *param, const CHAR *val );
	const CHAR *get( const CHAR *param );
};

/**********************************************************/

class LangEU_Pauses {
private:
	BOOL created;
	String buf_get;

public:
	LangEU_Pauses( VOID );
	~LangEU_Pauses();
	BOOL create(VOID);

	BOOL set( const CHAR *param, const CHAR *val );
	const CHAR *get( const CHAR *param );

	VOID utt_pauses(UttWS & ut);
	#ifdef HTTS_PROSO_VAL
	VOID utt_n_val_pause(UttPh & ut);//Aritz
	#endif

private:
#ifdef HTTS_EU_PAU1
	VOID utt_pau1( UttWS &ut );
#endif

protected:
	DOUBLE pau_nsilab;
	DOUBLE durmean;
	DOUBLE pbreath; // probabilidad breath

	String pau_model;
};

/**********************************************************/

class LangEU_PhTrans {
private:
	BOOL phtsimple;
	BOOL phtkatamotz; //INAKI: para no pronunciar como rr las r a principio de palabra
	String pht_speaker;//INAKI: KAROLINAREN TRANSKRIPZIORAKO
	String pht_hdic_name; //INAKI, para transcripcion desde diccionario
public:
	LangEU_PhTrans( VOID ) { phtsimple=0; phtkatamotz=0; }
	VOID utt_w2phtr(UttPh &ut);
	VOID setPhTSimple( BOOL simple ) { phtsimple=simple; }
	VOID setPhTKatamotz( BOOL katamotz ) { phtkatamotz=katamotz; }
	BOOL getPhTSimple( VOID ) { return phtsimple; }
	BOOL getPhTKatamotz( VOID ) { return phtkatamotz; }
	VOID setPhTSpeaker( const char *speaker ) { pht_speaker=speaker; }//INAKI: KAROLINAREN TRANSKRIPZIORAKO
	const char *getPhTSpeaker( VOID ) { return pht_speaker; }//INAKI: KAROLINAREN TRANSKRIPZIORAKO
	VOID setPhTHDicName( const char *dic ) { pht_hdic_name=dic; }//INAKI: transcripcion desde diccionario
	const char *getPhTHDicName( VOID ) { return pht_hdic_name; }//INAKI: transcripcion desde diccionario

private:
	VOID pausegr_ch2ph(UttPh &u, UttI senp );
	VOID word_syllab( UttPh &u, UttI wordp );
	VOID iu2jw( UttPh &u );
	VOID utt_silph( UttPh &u );
	VOID fgrp2agrp(UttPh &u,UttI fg);
 /***********Hitzaren transkripzio fonetikoa hiztegian agertzen bada  2005/03/16********************/
 	UttI     tf_mrk_ch2ph(UttPh &u, UttI senp );
/***************************************************************************************/
//	BOOL es_excepcion(UttPh &u,UttI p, char caso);
	BOOL es_bai_aditz(UttPh &u,UttI p);
	VOID agrp_stress( UttPh &u, UttI agrp);
	VOID syllable_stress( UttPh &u, UttI syl );
	BOOL agrp_stressed( UttPh &u, UttI agrp);
	BOOL es_marcada(UttPh &u, UttI wordp);
	BOOL acento_texto(UttPh &u,UttI wordp);
	BOOL es_ko_go_ten_tzen(UttPh &u,UttI wordp);
	BOOL es_ten_tzen(UttPh &u,UttI wordp);
	BOOL es_lotura(UttPh &u,UttI wordp);
	BOOL es_enclitico(UttPh &u,UttI wordp);
	BOOL es_proclitico(UttPh &u,UttI wordp);
	BOOL es_verbo_jok(UttPh &u,UttI wordp);
	BOOL es_bisilabo(UttPh &u,UttI wordp);
	BOOL es_monosilabo(UttPh &u,UttI wordp);
	BOOL es_verbo_lgn(UttPh &u,UttI wordp);
	BOOL es_verbo_trn(UttPh &u,UttI wordp);

	BOOL es_aux_o_sint(UttPh &u,UttI p, char caso);

	VOID word_stress( UttPh &u, UttI wordp );
	BOOL word_stressed( UttPh &u, UttI wordp );
	BOOL salbuespena (UttPh &u, UttI wordp);
	BOOL es_sin_acento(UttPh &u,UttI wordp);
	BOOL es_ez_ba_bait(UttPh &u,UttI wordp);

	BOOL set( const CHAR *param, const CHAR *val ); //INAKI: KAROLINAREN TRANSKRIPZIORAKO
	const CHAR *get( const CHAR *param ); //INAKI: KAROLINAREN TRANSKRIPZIORAKO

/*Transkripzio fonetikoaren salbuspena zein den jakiteko behar diren funtzioak 2005/02/22******************/
//	BOOL trans_fonet_salb_none(UttPh &u,UttI wordp);    //TRUE=Hitz horrek ez du Transkripzio Fonetikoaren salbuespenik
	BOOL trans_fonet_salb_i_0_j(UttPh &u,UttI wordp); 		//TRUE="i" grafiaren transkripzioa [0] izan ordez [j] da
	BOOL trans_fonet_salb_j_0_x(UttPh &u,UttI wordp); 	//TRUE="j" grafiaren transkripzioa [0] izan ordez [x] da
	BOOL trans_fonet_salb_l_L_l(UttPh &u,UttI wordp);		//TRUE="l" grafiaren transkripzioa [L] izan ordez [l] da
	BOOL trans_fonet_salb_n_J_n(UttPh &u,UttI wordp);		//TRUE="n" grafiaren transkripzioa [J] izan ordez [n] da
	BOOL trans_fonet_salb_z_0_t(UttPh &u,UttI wordp);	//INAKI: salbuespena z->T (ad. Zaragoza)	TRUE="z" grafiaren transkripzioa [S`] izan ordez [T] da
/*******************************************************************************************/

/***********Hitzaren transkripzio fonetikoa hiztegian agertzen bada************************/
	BOOL trans_fonet_hitza(UttPh &u,UttI wordp); //TRUE=TF hitzegian agertzen bada. Hitza markatua egon behar du.
/********************************************************************************/

};



class LangEU_Prosod {
private:
	String buf_get;
	BOOL created;

public:
	LangEU_Prosod( VOID );
	~LangEU_Prosod();
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
#ifdef HTTS_PROSOD_EU_DUR1
	VOID utt_dur1( UttPh &ut );
#endif
#ifdef HTTS_PROSOD_EU_DUR2
	VOID utt_dur2( UttPh &ut );
#endif
#ifdef HTTS_PROSOD_EU_PTH1
	VOID utt_pth1( UttPh &ut );
//	VOID *pth1_dummy_ptr;
	/* Busca "dummy" para ir viendo donde se puede ir metiendo codigo
	especifico para un metodo prosodico. */
#endif
#ifdef HTTS_PROSOD_EU_POW1
	VOID utt_pow1( UttPh &ut );
#endif
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
	String prosdbname;
};

/**********************************************************/

class LangEU_LingP : public LingP {
private:
	LangEU_POS     pos;
	LangEU_Pauses  pauses;
	LangEU_PhTrans phtr;
	LangEU_Prosod  prosod;
	#ifdef HTTS_PROSO_VAL
	LangEU_Prosod  pros;//Aritz
	LangEU_Pauses  pau;//Aritz
	#endif
	PhMap map;

public:
	virtual ~LangEU_LingP() {}; //needed to remove a warning
	BOOL create( VOID );

	VOID utt_lingp( Utt *u );

	VOID utt_pos( Utt *u );
	VOID utt_pauses( Utt *u );
	VOID utt_phtrans( Utt *u );
	VOID utt_prosod( Utt *u );
	VOID utt_map( Utt *u );
	#ifdef HTTS_PROSO_VAL
	VOID utt_new_val(Utt *u);//Aritz
	VOID utt_n_val_pause(Utt *u);//Aritz
	#endif

	BOOL set( const CHAR *param, const CHAR *val );
	const CHAR *get( const CHAR *param );
};
/**********************************************************/
/* tipos de Part of Speech: ize, lgn, jok*/
enum {
	POS_EU_NONE=0x0,
	POS_EU_IZE=0x01,			//nombre	izena
	POS_EU_ADI_JOK=0x002,	//verbo conjugado aditz jokatua
	POS_EU_ADI_LGN=0x004, //verbo auxiliar	aditz laguntzaile
	POS_EU_ADI_TRN=0x008,	//verbo perifrasico aditz trinkoa
	POS_EU_ADJ=0x010,			//adjetivo adjetibo
	POS_EU_DET=0x020,			//determinante determinatzaile
	POS_EU_ADB=0x040,			//adverbio adberbioa
	POS_EU_LOT_JNT=0x080,	//conjuncion copulativa lotura_juntura
	POS_EU_LOT_AZK=0x0100,	//conjuncion final lotura_azkena
	POS_EU_PRT=0x0200,		//particulas partikulak
	POS_EU_IOR=0x0400,		//pronombre izenordea
	POS_EU_ATZ_IZE=0x0800,	//sufijo nombre atzizki izena
	POS_EU_ATZ_ADI1=0x01000, //sufijo verbo 1 atzizki aditza 1
	POS_EU_ATZ_ADI2=0x02000, //sufijo verbo 1 (nominalizacion) atzizki aditza 2
	POS_EU_ATZ_ADI3=0x04000, //sufijo verbo 1 (para auxiliares)
	POS_EU_STR_MRK=0x08000,	//marcadas de acento
	POS_EU_TF_MRK=0x010000,	//marcadas de transcripcion fonetica
	POS_EU_PROKLITIKO=0x020000,	//proclitico (se pega por detras)
	POS_EU_ENKLITIKO=0x040000,		//enclitico (se pega por delante)
	POS_EU_PAUSE_AURRE=0x080000,	//invitacion a pausa por detras
	POS_EU_PAUSE_ATZE=0x0100000,  //invitacion a pausa por delante
	POS_EU_PAUSE_ARTEAN=0x0200000,  //invitacion a pausa por detras y delante
/*Transkripzio fonetikoaren salbuespen motak 2005/02/14******************/
	POS_EU_SALBTF_I_J= 0x0400000,	 	//"i" grafiaren transkripzioa [0] izan ordez [j] da
	POS_EU_SALBTF_J_X= 0x0800000, 	//"j" grafiaren transkripzioa [0] izan ordez [x] da
	POS_EU_SALBTF_l_l= 0x01000000, 	//"l" grafiaren transkripzioa [L] izan ordez [l] da
	POS_EU_SALBTF_N_N= 0x02000000,	//"n" grafiaren transkripzioa [J] izan ordez [n] da
	POS_EU_SALBTF_Z_T= 0x04000000   //INAKI salbuespena z->T (ad. Zaragoza) //"z" grafiaren transkripzioa [s`] izan ordez [T] da
/*****************************************************************/
};

/*********************************************************/


/*tipos de grupos funcionales*/
enum {
	GF_EU_NONE=0,
	GF_EU_ARRUN, 			//normal
	GF_EU_ADI,				//verbal
	GF_EU_ADI_EZ,			//verbal negativo
	GF_EU_GALDE, 			//foco
	GF_EU_PAU_AURRE,  //invitacion de pausa por delante
	GF_EU_PAU_ATZE,    //invitacion de pausa por detras
	GF_EU_PAU_ARTEAN	//invitacion a puasa por ambos lados
};

/**********************************************************/
/*tipos de grupos acentuales*/
enum {
	AGRP_EU_NONE=0,
	AGRP_EU_MRK, //en la primera
	AGRP_EU_OROK,//acento en la segunda silaba
	AGRP_EU_GABE, //sin acento
	AGRP_EU_TXT
};

#endif

