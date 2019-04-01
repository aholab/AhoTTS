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
(C) 1999 TSR/Aholab - ETSII/IT Bilbao (UPV/EHU) // Euskaltel

Nombre fuente................ eu_stuti.cpp
Nombre paquete............... aHoTTS
Lenguaje fuente.............. C++
Estado....................... -
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion: ............... Ana Armenta
................................... Nora Barroso

Version  dd/mm/aa  Autor     Comentario
-------		--------	--------  ----------
2.0.2	 20/10/08  Inaki      Añadir soporte para transcripción en diccionario (Nora)
2.0.1	 03/10/07  Inaki      z_T trankripzio salbuespena
2.0.0		22/02/05	Nora		Transkripzio fonetikoaren salbuespenak zein motatakoak diren jakiteko 
										behar diren funtzioak, kodearen bukaeran 
1.0.1  	06/04/00	ana		Llamadas a HDicRef	en vez de arrays
1.0.0    31/01/00	borja		Codefreeze aHoTTS v1.0
0.0.0    20/05/99	ana		Codificacion inicial

======================== Contenido ========================
<DOC>

</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include <string.h>
#include "eu_lingp.hpp"
#include "uti.h"
#include "eu_hdic.hpp"
/***********************************************************/
//#define DEBUG() fprintf (stderr,"file..: %s -- line..: %d\n",__FILE__,__LINE__);
#define DEBUG()

BOOL LangEU_PhTrans :: es_marcada(UttPh &u, UttI wordp)
{
	BOOL marcado=FALSE;
	if(u.cell(wordp).queryPOS(POS_EU_STR_MRK))
		marcado=TRUE;
	return (marcado);
}

/**********************************************************/

BOOL LangEU_PhTrans :: es_proclitico(UttPh &u,UttI wordp)
{
	BOOL proclitico=FALSE;
	if(u.cell(wordp).queryPOS(POS_EU_PROKLITIKO))
		proclitico=TRUE;
	return(proclitico);
}
/***************************************************/
BOOL LangEU_PhTrans :: es_enclitico(UttPh &u,UttI wordp)
{
	BOOL enclitico=FALSE;
	if(u.cell(wordp).queryPOS(POS_EU_ENKLITIKO))
		enclitico=TRUE;
	return(enclitico);
}

/**********************************************************/

BOOL LangEU_PhTrans :: es_lotura(UttPh &u,UttI wordp)
{
	BOOL lotura=FALSE;
	if(u.cell(wordp).queryPOS(POS_EU_LOT_JNT))
		lotura=TRUE;
	return(lotura);
}

/**********************************************************/
BOOL LangEU_PhTrans :: es_verbo_jok(UttPh &u,UttI wordp)
{
	BOOL verbo_jok=FALSE;
	DEBUG()
	if(u.cell(wordp).queryPOS(POS_EU_ADI_JOK)){
		DEBUG()
		verbo_jok=TRUE;
	}
	DEBUG()
	return(verbo_jok);
}
/***************************************************/
BOOL LangEU_PhTrans :: es_verbo_lgn(UttPh &u,UttI wordp)
{
	BOOL verbo_lgn=FALSE;
	if(u.cell(wordp).queryPOS(POS_EU_ADI_LGN))
		verbo_lgn=TRUE;
	return(verbo_lgn);
}
/***************************************************/
BOOL LangEU_PhTrans :: es_verbo_trn(UttPh &u,UttI wordp)
{
	BOOL verbo_trn=FALSE;
	if(u.cell(wordp).queryPOS(POS_EU_ADI_TRN))
		verbo_trn=TRUE;
	return(verbo_trn);
}
/**************************************************/
BOOL LangEU_PhTrans :: es_bisilabo(UttPh &u,UttI wordp)
{
	BOOL bisilabo=FALSE;
	UttI prev;
	INT numerosil;
	prev=u.wordPrev(wordp);
	if(prev) numerosil=u.syllableN(wordp,URANGE_WORD)-u.syllableN(prev,URANGE_WORD);
	else numerosil=u.syllableN(wordp,URANGE_WORD);
	if(numerosil==2) bisilabo=TRUE;
	return(bisilabo);
}
/***************************************************/
BOOL LangEU_PhTrans :: es_monosilabo(UttPh &u,UttI wordp)
{
	BOOL monosilabo=FALSE;
	UttI prev;
	INT numerosil;
	prev=u.wordPrev(wordp);
	if(prev) numerosil=u.syllableN(wordp,URANGE_WORD)-u.syllableN(prev,URANGE_WORD);
	else numerosil=u.syllableN(wordp,URANGE_WORD);
	if(numerosil==1) monosilabo=TRUE;
	return(monosilabo);
}
/***************************************************/
BOOL LangEU_PhTrans :: acento_texto(UttPh &u,UttI wordp)
{
	BOOL acentuado=FALSE;
	if(u.cell(wordp).getStress()==USTRESS_TEXT)
			acentuado=TRUE;
	return(acentuado);
}
/***************************************************/

BOOL LangEU_PhTrans :: salbuespena (UttPh &u, UttI wordp)
{

BOOL encontrado=FALSE;
const char * palabra;
const char * excepciones[]=
	{
		"aurreratu",
		"aurreratuko",
		"aurreratzen",
		"bana",
		"bina",
		"bosna",
		"bota",
		"botako",
		"botatzen",
		"dela",
		"dena",
		"ehunta",
		"haiei",
		"haiek",
		"hauei",
		"hauek",
		"launa",
		"seina",
		"zela",
		"zena",
		"zeuek",
		"zuei",
		"zuek",
		"zuen",
		"zuenzat",
		NULL
		};
		palabra = u.cell(wordp).getWord();
		if(str_match(excepciones, nptrs((const void * const *)excepciones), palabra, 0, 0)!=-1){
			 encontrado=TRUE;
		}
return (encontrado);
}

/**********************************************************/

BOOL LangEU_PhTrans ::es_ez_ba_bait(UttPh &u,UttI wordp)
{
BOOL encontrado=FALSE;
const char *palabra;
const char *lista[]=
	{
	"ez",
	"ba",
	"bait",
	NULL
	};
	palabra=u.cell(wordp).getWord();
	if(str_match(lista, nptrs((const void * const *)lista), palabra, 0, 0)!=-1){
			 encontrado=TRUE;
	}
	return(encontrado);
}

/**********************************************************/

BOOL LangEU_PhTrans ::es_sin_acento(UttPh &u,UttI wordp)
{
BOOL encontrado=FALSE;
const char *palabra;
const char *lista[]=
	{
	"eta",
	"ta",
	"edo",
	"ala",
	"baina",
	"baino",
	NULL
	};
	palabra=u.cell(wordp).getWord();
	if(str_match(lista, nptrs((const void * const *)lista), palabra, 0, 0)!=-1){
			 encontrado=TRUE;
	}
	return(encontrado);
}

/**********************************************************/

BOOL LangEU_PhTrans ::es_ko_go_ten_tzen(UttPh &u,UttI wordp)
{
const CHAR *palabra;
INT n;
	palabra=u.cell(wordp).getWord();
		n=strlen(palabra);
		if(palabra[n-1]=='o')
			if((palabra[n-2]=='k')||(palabra[n-2]=='g'))
				return TRUE;
		if(palabra[n-1]=='n'){
			if(palabra[n-2]=='e'){
				if(palabra[n-3]=='t')  return TRUE;
				else if((palabra[n-3]=='z')&&(palabra[n-4]=='t')) return TRUE;
			}
		}
	return FALSE;
}

/**********************************************************/

BOOL LangEU_PhTrans :: es_ten_tzen(UttPh &u, UttI wordp)
{
BOOL encontrado=FALSE;
const char * palabra;
	const char *tentzen[]=
	{
				"ahitzen",
				"ahultzen",
				"bahitzen",
				"batzen",
				"biltzen",
				"biltzen",
				"bitzen",
				"bizten",
				"bizten",
				"blaitzen",
				"busttzen",
				"deitzen",
				"dotzen",
				"doitzen",
				"ehuntzen",
				"ertzen",
				"esttzen",
				"esten",
				"esten",
				"eurtzen",
				"eurttzen",
				"eusten",
				"fauntzen",
				"fintzen",
				"fintzen",
				"gatzen",
				"gaintzen",
				"gaitzen",
				"gaitztzen",
				"gaizten",
				"gaizten",
				"galtzen",
				"gartzen",
				"gehitzen",
				"geltzen",
				"gertzen",
				"geuntzen",
				"goitzen",
				"goizten",
				"gortzen",
				"gurtzen",
				"hantzen",
				"hartzen",
				"harzten",
				"hasten",
				"hasten",
				"hatstzen",
				"haurtzen",
				"hausten",
				"hausten",
				"hautzen",
				"hazten",
				"heltzen",
				"hersten",
				"hersten",
				"hertzen",
				"hesten",
				"hezten",
				"hiltzen",
				"hiltzen",
				"histen",
				"hozten",
				"husten",
				"ixten",
				"jaten",
				"jartzen",
				"jausten",
				"jausten",
				"jauzten",
				"jezten",
				"jiten",
				"jotzen",
				"joaten",
				"josten",
				"kentzen",
				"ketzen",
				"kitzen",
				"kosktzen",
				"lantzen",
				"lautzen",
				"lazten",
				"lehertzen",
				"leitzen",
				"lohitzen",
				"lortzen",
				"lotzen",
				"lurtzen",
				"maizten",
				"mentzen",
				"mensten",
				"mentstzen",
				"mintzen",
				"mozten",
				"nahtzen",
				"nahitzen",
				"neurtzen",
				"neurtzen",
				"nortzen",
				"nortzen",
				"ohiltzen",
				"ohitzen",
				"ontzen",
				"osten",
				"otzen",
				"peitzen",
				"pizten",
				"pleintzen",
				"pozten",
				"puzten",
				"saltzen",
				"sartzen",
				"sasten",
				"soiltzen",
				"sortzen",
				"sutzen",
				"toiltzen",
				"txartzen",
				"urtzen",
				"uzten",
				"xitzen",
				"zailtzen",
				"zaintzen",
				"zaurtzen",
				"zentzen",
        "zertzen",
				"ziltzen",
        "ziltzen",
        "zoltzen",
				"zortzen",
        "zurtzen",
	 NULL
	 };
		palabra = u.cell(wordp).getWord();
		/*int str_match( const char * const *strarr, int nstrs,const char * str, int lcode, int case_sensitive )*/

		if(str_match(tentzen, nptrs((const void * const *)tentzen), palabra, 0, 0)!=-1){
			 encontrado=TRUE;
			}
return (encontrado);
}


/***Funtzio berriak: transkripzio fonetikoaren salbuespenak zeintzuk diren jakiteko, 2005/02/22*******/ 
/*
BOOL LangEU_PhTrans :: trans_fonet_salb_none(UttPh &u,UttI wordp) 
{ 

	BOOL salb_none =FALSE; 	
	if(!u.cell(wordp).queryPOS(POS_EU_SALBTF_I_J)&&!u.cell(wordp).queryPOS(POS_EU_SALBTF_J_X)&&!u.cell(wordp).queryPOS(POS_EU_SALBTF_l_l)&&!u.cell(wordp).queryPOS(POS_EU_SALBTF_N_N))
	salb_none =TRUE; 	
	return(salb_none); 
}  
*/

BOOL LangEU_PhTrans :: trans_fonet_salb_i_0_j (UttPh &u,UttI wordp)
{	
	BOOL salb_i_0_j =FALSE;
	if(u.cell(wordp).queryPOS(POS_EU_SALBTF_I_J))
		salb_i_0_j =TRUE;
	return(salb_i_0_j);
}


BOOL LangEU_PhTrans :: trans_fonet_salb_j_0_x (UttPh &u,UttI wordp)
{
	BOOL salb_j_0_x =FALSE;
	if(u.cell(wordp).queryPOS(POS_EU_SALBTF_J_X))
		salb_j_0_x =TRUE;
	return(salb_j_0_x);
}


BOOL LangEU_PhTrans :: trans_fonet_salb_l_L_l (UttPh &u,UttI wordp)
{
	BOOL salb_l_L_l =FALSE;
	if(u.cell(wordp).queryPOS(POS_EU_SALBTF_l_l))
		salb_l_L_l =TRUE;
	return(salb_l_L_l);
}

BOOL LangEU_PhTrans :: trans_fonet_salb_n_J_n (UttPh &u,UttI wordp)
{
	BOOL salb_n_J_n =FALSE;
	if(u.cell(wordp).queryPOS(POS_EU_SALBTF_N_N))
		salb_n_J_n =TRUE;
	return(salb_n_J_n);
}
BOOL LangEU_PhTrans :: trans_fonet_salb_z_0_t (UttPh &u,UttI wordp)// INAKI, salbuespena z->T (ad. Zaragoza)
{
	BOOL salb_z_0_t =FALSE;
	//mirar tamaño del match con la entrada del dicc. tam==0 --> match total; tam>0  --> longitud del match parcial
	INT tam=u.getHDicDB()->query(u.cell(wordp).getHDicRef(),HDIC_QUERY_MATCHLEN);
	//calculamos posicion del fonema en la palabra
	INT pos= u.phoneN(wordp,URANGE_SENTENCE)+1 - u.phonePos(wordp, URANGE_SENTENCE);
	//solo si hay match total (tam==0) o si el fonema está dentro del match parcial (para que no altere atzizkiz tipo -rentzat...)
	if(u.cell(wordp).queryPOS(POS_EU_SALBTF_Z_T)&&(tam>=pos||tam==0))
		salb_z_0_t =TRUE;
	return(salb_z_0_t);
}

BOOL LangEU_PhTrans :: trans_fonet_hitza (UttPh &u,UttI wordp)
{
	BOOL hizt_tf_mrk =FALSE;
	if(u.cell(wordp).queryPOS(POS_EU_TF_MRK))
		hizt_tf_mrk =TRUE;
	return(hizt_tf_mrk);
}
