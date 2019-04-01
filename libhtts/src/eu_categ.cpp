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

Nombre fuente................ eu_categ.cpp
Nombre paquete............... aHoTTS
Lenguaje fuente.............. C++
Estado....................... -
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Inma Sardon
.............................
Version  dd/mm/aa  Autor     Comentario
-------         --------        --------  ----------
1.0.0    03/10/07  Inaki      z_T trankripzio salbuespena
0.0.0 


======================== Contenido ========================
<DOC>
</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include "eu_lingp.hpp"
#include "httsmsg.h"

//#define DEBUG() fprintf (stderr,"file..: %s -- line..: %d\n",__FILE__,__LINE__);

#define DEBUGPOSx

/**********************************************************/

LangEU_Categ::LangEU_Categ( VOID )
{
	encontrado=FALSE;
	created=FALSE;
}

/**********************************************************/

LangEU_Categ::~LangEU_Categ( VOID )
{
}


/**********************************************************/

BOOL LangEU_Categ::create( VOID )
{
	created=TRUE;
	return TRUE;
}

/**********************************************************/

BOOL LangEU_Categ::set( const CHAR *param, const CHAR * value )
{
	if (!strcmp(param,"HDicDB")&& !created) { dbName=value; return TRUE; }
	else return FALSE;
}

/**********************************************************/

const CHAR *LangEU_Categ::get( const CHAR *param )
{
#define VALRET(x) { sprintf(buf,"%g",(double)x); buf_get=buf; return buf_get; }
	if (!strcmp(param,"HDicDB")) return dbName;

	return FALSE;
}

/**********************************************************/

VOID LangEU_Categ::utt_categ( UttWS & u )
{
	UttI p=NULL;		//indice para recorrer las celdas
	//caracteristicas de palabras
	char word_act[MAX_TAM_WORD]="\0";
	int len_string1=0;
	int numWord=0;	//numero de palabra en la phrase (entre signos puntuac.)

	HDicRef hDicRef;
	char ref[MAX_TAM_WORD]="\0";
	int num_eq=0,found=0;

	LangEU_HDicDB db;
	db.create(dbName);	//dbName contiene el nombre del diccionario

/*---------------------------------------------------------------*/
/*  Recorrido palabra por palabra de cada frase, de cada u.      */
/*---------------------------------------------------------------*/
	for(p=u.wordFirst();p!=NULL;p=u.wordNext(p)){
		encontrado=FALSE;
		strcpy(word_act,u.cell(p).getWord());

/*================================================================*/
/*  Analisis POS: Part Of Speech                                  */
/*================================================================*/
		u.cell(p).setPOS(POS_EU_NONE);
		hDicRef=u.cell(p).getHDicRef();
		strcpy(ref,u.getHDicDB()->hDicRefToTxt(hDicRef));
		num_eq=u.getHDicDB()->query(hDicRef,HDIC_QUERY_MATCHLEN);
		len_string1=strlen(word_act);

//Yon2. !!!!! Solo vale num_eq si hDicRef != 0
		if ((!num_eq) && (hDicRef != HDIC_REF_NULL)) found = TRUE;
		else found = FALSE;

		if (found) {
#ifdef DEBUGPOS
			htts_warn("Eu_categ: Paso a POSDIC. found es TRUE");
#endif
			posdic(u,p);		//asignar POS a palabra encontrada en diccionario
		}
		else	{
#ifdef DEBUGPOS
			htts_warn("Eu_categ: No paso a POSDIC. found es FALSE");
#endif
		}	
		//no se ha encontrado en el diccionario

/* Eva 4/12/2006
Si no se ha encontrado, pero hay un match parcial con una palabra que tiene excepciones a la transcripcion
fonetica, se heredan esas excepciones. Asi no hay que meter en el diccionario las palabras con excepciones declinadas
en todas sus formas*/
	if (encontrado==FALSE) {
		switch (u.getHDicDB()->query(u.cell(p).getHDicRef(),HDIC_QUERY_EU_SALBTF_I_0_J)) {
			case 1:
				u.cell(p).addPOS(POS_EU_SALBTF_I_J);
		}
		switch (u.getHDicDB()->query(u.cell(p).getHDicRef(),HDIC_QUERY_EU_SALBTF_J_0_X)) {
			case 1:
				u.cell(p).addPOS(POS_EU_SALBTF_J_X);
		}
		switch (u.getHDicDB()->query(u.cell(p).getHDicRef(),HDIC_QUERY_EU_SALBTF_L_l_L)) {
			case 1:
				u.cell(p).addPOS(POS_EU_SALBTF_l_l);
		}
		switch (u.getHDicDB()->query(u.cell(p).getHDicRef(),HDIC_QUERY_EU_SALBTF_N_J_N)) {
			case 1:
				u.cell(p).addPOS(POS_EU_SALBTF_N_N);
		}
		switch (u.getHDicDB()->query(u.cell(p).getHDicRef(),HDIC_QUERY_EU_SALBTF_Z_0_T)) {//INAKI salbuespena z->T (ad. Zaragoza)
			case 1:
				u.cell(p).addPOS(POS_EU_SALBTF_Z_T);
		}
	}
	
#ifdef DEBUGPOS
			htts_warn("Eu_categ: En la palabra [%s] el flag de encontrado vale %d",u.cell(p).getWord(), encontrado);
#endif

	if (encontrado==FALSE) {
		//las palabras marcadas, las marcaremos igual
		switch (u.getHDicDB()->query(u.cell(p).getHDicRef(),HDIC_QUERY_EU_STR_MRK)) {
			case 1:
					u.cell(p).addPOS(POS_EU_STR_MRK);
#ifdef DEBUGPOS
			htts_warn("Eu_categ: Ha añadido el flag de marcado para el acento en la palabra [%s]",u.cell(p).getWord());
#endif
					switch (u.getHDicDB()->query(u.cell(p).getHDicRef(),HDIC_QUERY_EU_TALDE3) ) {
						case HDIC_ANSWER_EU_TALDE3_IZE:
						u.cell(p).addPOS(POS_EU_IZE);
						break;
					}
			break;

		}
	}
		if (encontrado==FALSE)  {
#ifdef DEBUGPOS
			htts_warn("Eu_categ: Paso por ADITUDU");
#endif
			aditudu(u,p,word_act);			//analizar si palabra no encontrada es verbo
#ifdef DEBUGPOS
		htts_warn("Eu_categ: ADITUDU Word [%s] Found? %d",u.cell(p).getWord(),encontrado);
#endif

		}
		if (encontrado==FALSE) {
#ifdef DEBUGPOS
			htts_warn("Eu_categ: Paso por BABAIT");
#endif
			babait(u,p,word_act,db);		//analizar si es un aux o trn
#ifdef DEBUGPOS
		htts_warn("Eu_categ: BABAIT Word [%s] Found? %d",u.cell(p).getWord(),encontrado);
#endif

		}
		if (encontrado==FALSE) {
#ifdef DEBUGPOS
			htts_warn("Eu_categ: Paso por ATZADI");
#endif
			atzadi(u,p,word_act,db);		//buscar sufijo de verbo
#ifdef DEBUGPOS
		htts_warn("Eu_categ: ATZADI Word [%s] Found? %d",u.cell(p).getWord(),encontrado);
#endif

		}		
		if (encontrado==FALSE)	{
#ifdef DEBUGPOS
			htts_warn("Eu_categ: Paso por ATZIZE");
#endif
			atzize(u,p,word_act,db); 	//buscar sufijo de nombre
#ifdef DEBUGPOS
		htts_warn("Eu_categ: ATZIZE Word [%s] Found? %d",u.cell(p).getWord(),encontrado);
#endif

		}
	}


	encontrado=FALSE;	//resetear flag encontrado

/************  poscases  ***************/
	for(p=u.wordFirst();p!=0;p=u.wordNext(p)){
		detaux(u,p);
		detior(u,p);					// ize +det   ez_ize+prn

		if (u.cell(p).isStartOf(URANGE_SENTENCE))	//num relativo
			numWord=1;
		else
			numWord++;

		jntazk(u,p,numWord);	//lot o invitacion a pausa
		izejok(u,p);	//distinguir cuando es nombre y cuando verbo
		adjjok(u,p);	//distinguir cuando es adjetivo y cuando verbo
		trnlgn(u,p);					// trn != lgn

	}
}





