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

Nombre fuente................ eu_hdic.cpp
Nombre paquete............... aHoTTS
Lenguaje fuente.............. C++
Estado....................... -
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion: ............... Borja Etxebarria
.............................

Version  dd/mm/aa  Autor     Comentario
-------  --------  --------  ----------
2.1.1 		03/10/07	Inaki		z_T salbuespena gehitu
2.1.0		18/03/05	Nora		Hitzaren trankripzio fonetiko osoa hiztegitik hartzeko.
2.0.0		15/02/05	Nora		Transkripzio fonettikoaren salbuespenak hiztegian kodetzeko.
1.1.1    2004      Inakis     Introduccion del searchbin para el euskara
1.0.0    31/01/00  borja     codefreeze aHoTTS v1.0
0.1.0    15/12/99  borja     version inicial

======================== Contenido ========================
<DOC>
</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include <string.h>
#include "eu_hdic.hpp"
#include "uti.h"
#include "chset.h"

/**********************************************************/

KINDOF_DDEF(LangEU_HDicDB,HDicDB);

/**********************************************************/

BOOL LangEU_HDicDB::flagParse( const CHAR *s, HDicRef &r )
{
	/*  estos son de pega */
// n = nombre
//	if (!strcmp(s,"ize")) r.setquery(HDIC_QUERY_EU_TALDE1,HDIC_ANSWER_EU_TALDE1_IZE);

	if (!strcmp(s,"jok")) r.setquery(HDIC_QUERY_EU_TALDE1,HDIC_ANSWER_EU_TALDE1_ADI_JOK);
	else if (!strcmp(s,"adb")) r.setquery(HDIC_QUERY_EU_TALDE1,HDIC_ANSWER_EU_TALDE1_ADB);
	else if (!strcmp(s,"lot_azk")) r.setquery(HDIC_QUERY_EU_TALDE1,HDIC_ANSWER_EU_TALDE1_LOT_AZK);
	else if (!strcmp(s,"atz_adi1")) r.setquery(HDIC_QUERY_EU_TALDE1,HDIC_ANSWER_EU_TALDE1_ATZ_ADI1);
	else if (!strcmp(s,"ior")) r.setquery(HDIC_QUERY_EU_TALDE1,HDIC_ANSWER_EU_TALDE1_IOR);

	else if (!strcmp(s,"adj")) r.setquery(HDIC_QUERY_EU_TALDE2,HDIC_ANSWER_EU_TALDE2_ADJ);
	else if (!strcmp(s,"det")) r.setquery(HDIC_QUERY_EU_TALDE2,HDIC_ANSWER_EU_TALDE2_DET);
	else if (!strcmp(s,"lot_jnt")) r.setquery(HDIC_QUERY_EU_TALDE2,HDIC_ANSWER_EU_TALDE2_LOT_JNT);
	else if (!strcmp(s,"atz_ize")) r.setquery(HDIC_QUERY_EU_TALDE2,HDIC_ANSWER_EU_TALDE2_ATZ_IZE);
	else if (!strcmp(s,"atz_adi2")) r.setquery(HDIC_QUERY_EU_TALDE2,HDIC_ANSWER_EU_TALDE2_ATZ_ADI2);

	else if (!strcmp(s,"trn")) r.setquery(HDIC_QUERY_EU_TALDE3,HDIC_ANSWER_EU_TALDE3_ADI_TRN);
	else if (!strcmp(s,"prt")) r.setquery(HDIC_QUERY_EU_TALDE3,HDIC_ANSWER_EU_TALDE3_PRT);
	else if (!strcmp(s,"atz_adi3")) r.setquery(HDIC_QUERY_EU_TALDE3,HDIC_ANSWER_EU_TALDE3_ATZ_ADI3);
	else if (!strcmp(s,"ize")) r.setquery(HDIC_QUERY_EU_TALDE3,HDIC_ANSWER_EU_TALDE3_IZE);

	else if (!strcmp(s,"lgn")) r.setquery(HDIC_QUERY_EU_TALDE4,HDIC_ANSWER_EU_TALDE4_ADI_LGN);
	else if (!strcmp(s,"proklitiko")) r.setquery(HDIC_QUERY_EU_TALDE4,HDIC_ANSWER_EU_TALDE4_PROKLITIKO);
	else if (!strcmp(s,"enklitiko")) r.setquery(HDIC_QUERY_EU_TALDE4,HDIC_ANSWER_EU_TALDE4_ENKLITIKO);
	else if (!strcmp(s,"pau_aur")) r.setquery(HDIC_QUERY_EU_TALDE4,HDIC_ANSWER_EU_TALDE4_PAU_AUR);

	else if (!strcmp(s,"str_mrk")) r.setquery(HDIC_QUERY_EU_STR_MRK,1);
//  tf_mrk = maracadas para la transcripcion
	else if (!strcmp(s,"tf_mrk")) r.setquery(HDIC_QUERY_EU_TF_MRK,1);
//invitacion a pausa por detras
	else if (!strcmp(s,"pau_atze")) r.setquery(HDIC_QUERY_EU_PAU_ATZE,1);

/*Transkripzio fonetikoaren salbuespen hitzak sartzen,2005/02/15****************************/
/*Salbuespenen kodeketa hitzegian:
		"i" grafiaren transkripzioa [0] izan ordez [j] baldin bada = "i_j"
		"j" grafiaren transkripzioa [0] izan ordez [x] baldin bada = "j_x"
		"l" grafiaren transkripzioa [L] izan ordez [l] baldin bada = "l_l"
		"n" grafiaren transkripzioa [J] izan ordez [n] baldin bada = "n_n"*/
	else if (!strcmp(s,"i_j")) r.setquery(HDIC_QUERY_EU_SALBTF_I_0_J,1);
	else if (!strcmp(s,"j_x")) r.setquery(HDIC_QUERY_EU_SALBTF_J_0_X,1);
	else if (!strcmp(s,"l_l")) r.setquery(HDIC_QUERY_EU_SALBTF_L_l_L,1);
	else if (!strcmp(s,"n_n")) r.setquery(HDIC_QUERY_EU_SALBTF_N_J_N,1);
	else if (!strcmp(s,"z_T")) r.setquery(HDIC_QUERY_EU_SALBTF_Z_0_T,1); //INAKI salbuespena z->T (ad. Zaragoza)
/*************************************************************************************/


	/* estos los necesita el normalizador de texto */
	else if (!strcmp(s,"abb")) r.setquery(HDIC_QUERY_EU_NOR,HDIC_ANSWER_EU_NOR_ABB);
	else if (!strcmp(s,"unit")) r.setquery(HDIC_QUERY_EU_NOR,HDIC_ANSWER_EU_NOR_UNIT);
	else if (!strcmp(s,"acr")) r.setquery(HDIC_QUERY_EU_NOR,HDIC_ANSWER_EU_NOR_ACR);
	else if (!strcmp(s,"dec")) r.setquery(HDIC_QUERY_EU_DEC,1);
	else return FALSE;
	return TRUE;
}

/**********************************************************/


/**********************************************************/
/* Busqueda en fichero compilado.

hacemos 4 busquedas binarias por este orden:
1. casesens con sustituciones
2. casesens sin sustituciones
3. caseinsens con sustituciones
4. caseinsens sin sustituciones
Cada una debe ser mas larga que la anterior para desbancarla.
Para caseinsens se utiliza {tokl} (lowercase) en vez de {tok}.
*/

HDicRef LangEU_HDicDB::searchBin(char **exp)
{
	long hit[4];
	BOOL notFoundFr;
	size_t toklen_old;

	/* buscamos en los 4 bloques del fichero */
	hit[0]=tokbsearch(tok, b_base[0],b_n[0],b_blen[0]);
	hit[1]=tokbsearch(tok, b_base[1],b_n[1],b_blen[1]);
	hit[2]=tokbsearch(tokl,b_base[2],b_n[2],b_blen[2]);
	hit[3]=tokbsearch(tokl,b_base[3],b_n[3],b_blen[3]);

	INT i=0;

	/* nos quedamos con el mas largo (o sea empezamos por el ultimo bloque) */
	notFoundFr = FALSE;
	if (hit[3]>=0) i=3;
	else if (hit[2]>=0) i=2;
	else if (hit[1]>=0) i=1;
	else if (hit[0]>=0) i=0;
	else notFoundFr = TRUE;

	if (notFoundFr || (toklen != hitlen)) {
//Yon2. $$$$$ Aqui hacemos búsqueda parcial.
		pCHAR partialStr;
		pCHAR partialStrl;
		size_t parLen=toklen - 1;
		BOOL contSrch=TRUE;

#ifdef DEBUGDIC
		htts_warn ("Entro en la búsqueda parcial.");
#endif
		toklen_old = toklen;
		if(!parLen) contSrch=FALSE; //No buscar si no hay nada que buscar
		while (contSrch)	{
			partialStr = strdup(tok);
			partialStr[parLen]='\0';
			partialStrl = strdup(partialStr);
			chset_StrLower(partialStrl); // pasar a minusculas
			toklen = parLen;
			hitlen = 0;


			/* buscamos en los 4 bloques del fichero */
			hit[0]=tokbsearch(partialStr, b_base[0],b_n[0],b_blen[0]);
			hit[1]=tokbsearch(partialStr, b_base[1],b_n[1],b_blen[1]);
			hit[2]=tokbsearch(partialStrl, b_base[2],b_n[2],b_blen[2]);
			hit[3]=tokbsearch(partialStrl,b_base[3],b_n[3],b_blen[3]);

			/* nos quedamos con el mas largo (o sea empezamos por el ultimo bloque) */
			notFoundFr = FALSE;
			if (hit[3]>=0) i=3;
			else if (hit[2]>=0) i=2;
			else if (hit[1]>=0) i=1;
			else if (hit[0]>=0) i=0;
			else notFoundFr = TRUE;

			/* continuamos buscando? Solo nos sirve match total */
#ifdef DEBUGDIC
			htts_warn("Buscando: %s Len: %d Len: %d i: %d", partialStr, parLen, hitlen, i);
#endif
			/* liberamos las cadenas parciales */
			free(partialStr);
			free(partialStrl);

			if (hitlen == parLen) {
				notFoundFr = FALSE;
				contSrch = FALSE;
#ifdef DEBUGDIC
				htts_warn("Hemos encontrado un match parcial.");
				htts_warn("Found: %d %d %d %d", hit[3], hit[2],hit[1],hit[0]);
#endif
			}
			else {
				notFoundFr = TRUE;
#ifdef DEBUGDIC
				htts_warn("Not found: %d %d %d %d", hit[3], hit[2],hit[1],hit[0]);
#endif
			}
			parLen--;
			if (parLen == 0) contSrch = FALSE;
		}
		toklen = toklen_old;
		if (notFoundFr) {
#ifdef DEBUGDIC
			htts_warn("Nos salimos porque no encuentra nada.");
#endif
			return HDIC_REF_NULL;  // si no hay ningun hit, fuera!
		}
	}

	if (exp&&(*exp)) { free(*exp); *exp=NULL; } // liberar expansion antigua si la hay

	BOOL casesen[4]={TRUE,TRUE,FALSE,FALSE};
	hitlonger=casesen[i];
	// saltamos al campo HDicRef de la linea del hit
	fseek(fileBin,b_base[i]+b_blen[i]*hit[i]+sizeof(INT16)+b_slen[i],SEEK_SET);

	// Leemos el campo ref.
	HDicRef ref=HDIC_REF_NULL;
	UINT32 u32;
	if (!fread(&u32,sizeof(u32),1,fileBin)) return HDIC_REF_NULL;
	endian_fromlittle32(&u32); ref.bits=u32;

#ifdef DEBUGDIC
	htts_warn("HitLen: %d TokLen: %d Ref: %ud i: %d", hitlen, toklen, u32, i);
#endif
	ref.__setbits(HDIC_QUERY_CASE,casesen[i]); // metemos el campo casesens
	// salvar longitud del match. 0 si es full
	if (hitlen==toklen) ref.__setbits(HDIC_QUERY_MATCHLEN,0);
	else {
		ref.__setbits(HDIC_QUERY_MATCHLEN,hitlen);
		// si no entra en el campo de bits, lo ponemos a full (mejor que dejarlo indeterminado...)
		if (hitlen!=ref.__getbits(HDIC_QUERY_MATCHLEN)) ref.__setbits(HDIC_QUERY_MATCHLEN,0);
	}

	/* si el hit era en el primer o tercer bloque, hay sustitucion */
	if (exp&&((i==0)||(i==2))) {
		UINT16 u16;
		size_t len;
		if (fread(&u16,sizeof(u16),1,fileBin)) {
			endian_fromlittle16(&u16); len=u16;
			*exp=(CHAR*)malloc(sizeof(CHAR)*(len+1));
			if (!fread(*exp,sizeof(CHAR),len+1,fileBin)) {
				free(*exp); *exp=NULL;
			}
		}
	}
	return ref;
}

