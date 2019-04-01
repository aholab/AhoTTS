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

Nombre fuente................ pos1.cpp
Nombre paquete............... aHoTTS
Lenguaje fuente.............. C++
Estado....................... -
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Inma Sardon
................................... Nora Barroso

Version  dd/mm/aa    Autor     Comentario
---------  ------------  --------	  -------------
2.0.2    03/10/07    Inaki	z_T transkripzio salbuespena
2.0.1		02/03/02		Nora		Aurreko bertsioaren hobekuntzak.
2.0.0		22/02/05		Nora		Hitzegiko hitz batean transkripzio fonetikoaren salbuespena baldin badago,
											informazioa gelaxkan jartzeko.
1.0.0    XX/xx/xx  Inma Sardon     Codificacion inicial
======================== Contenido ========================
<DOC>
POS1.CPP:etiqueta POS palabras del diccionario y palabras no en el diccionario pero "conocidas"
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


VOID LangEU_Categ::posdic(UttWS &u, UttI p) {
#ifdef DEBUGPOS
	htts_warn("  Pos1: POSDIC word_act: %s",u.cell(p).getWord());
#endif

	switch (u.getHDicDB()->query(u.cell(p).getHDicRef(),HDIC_QUERY_EU_TALDE1)) {
		case HDIC_ANSWER_EU_TALDE1_ADI_JOK:
			u.cell(p).addPOS(POS_EU_ADI_JOK);
			break;
/*case honetan "u.cell(p).addPOS(POS_EU_ADB);" jarri ordez, "u.cell(p).addPOS(POS_EU_SALBTF_I_J);" jartzen badugu
guztia ondo egiten du.
Arazoa ez dago funtzioetan, baizik eta ezaugarri berriaren ezarpenean
*/
		case HDIC_ANSWER_EU_TALDE1_ADB:
			u.cell(p).addPOS(POS_EU_ADB);
			break;
		case HDIC_ANSWER_EU_TALDE1_LOT_AZK:
			u.cell(p).setPOS(POS_EU_LOT_AZK);
			break;

/*				case HDIC_ANSWER_EU_TALDE1_ATZ_ADI1:
					u.cell(p).addPOS(POS_EU_ATZ_ADI1);
					break;
*/
		case HDIC_ANSWER_EU_TALDE1_IOR:
			u.cell(p).addPOS(POS_EU_IOR);
			break;
	}

	switch (u.getHDicDB()->query(u.cell(p).getHDicRef(),HDIC_QUERY_EU_TALDE2)) {
		case HDIC_ANSWER_EU_TALDE2_ADJ:
			u.cell(p).addPOS(POS_EU_ADJ);
			break;

/*				case HDIC_ANSWER_EU_TALDE2_ADI_LGN:
					u.cell(p).addPOS(POS_EU_ADI_LGN);
					break;
*/
		case HDIC_ANSWER_EU_TALDE2_DET:
			u.cell(p).addPOS(POS_EU_DET);
			break;
		case HDIC_ANSWER_EU_TALDE2_LOT_JNT:
			u.cell(p).addPOS(POS_EU_LOT_JNT);
			break;

/*				case HDIC_ANSWER_EU_TALDE2_ATZ_IZE:
					u.cell(p).addPOS(POS_EU_ATZ_IZE);
					break;
*/
/*				case HDIC_ANSWER_EU_TALDE2_ATZ_ADI2:
					u.cell(p).addPOS(POS_EU_ATZ_ADI2);
					break;
*/
	}

	switch (u.getHDicDB()->query(u.cell(p).getHDicRef(),HDIC_QUERY_EU_TALDE3)) {
		case HDIC_ANSWER_EU_TALDE3_ADI_TRN:
			u.cell(p).addPOS(POS_EU_ADI_TRN);
			break;
		case HDIC_ANSWER_EU_TALDE3_PRT:
			u.cell(p).addPOS(POS_EU_PRT);
			break;
/*				case HDIC_ANSWER_EU_TALDE3_ATZ_ADI3:
					u.cell(p).addPOS(POS_EU_ATZ_ADI3);
					break;
*/
		case HDIC_ANSWER_EU_TALDE3_IZE:
			u.cell(p).addPOS(POS_EU_IZE);
			break;
	}


	switch (u.getHDicDB()->query(u.cell(p).getHDicRef(),HDIC_QUERY_EU_TALDE4)) {
		case HDIC_ANSWER_EU_TALDE4_ADI_LGN:
			u.cell(p).addPOS(POS_EU_ADI_LGN);
			break;
		case HDIC_ANSWER_EU_TALDE4_PROKLITIKO:
			u.cell(p).addPOS(POS_EU_PROKLITIKO);
			break;
		case HDIC_ANSWER_EU_TALDE4_ENKLITIKO:
			u.cell(p).addPOS(POS_EU_ENKLITIKO);
			break;
		case HDIC_ANSWER_EU_TALDE4_PAU_AUR:
			u.cell(p).addPOS(POS_EU_PAUSE_AURRE);
			break;
	}


	switch (u.getHDicDB()->query(u.cell(p).getHDicRef(),HDIC_QUERY_EU_STR_MRK)) {
		case 1:
			u.cell(p).addPOS(POS_EU_STR_MRK);
	}

	switch (u.getHDicDB()->query(u.cell(p).getHDicRef(),HDIC_QUERY_EU_TF_MRK)) {
		case 1:
			u.cell(p).addPOS(POS_EU_TF_MRK);
	}

/*			switch (u.getHDicDB()->query(u.cell(p).getHDicRef(),HDIC_QUERY_EU_PROKLITIKO)) {
				case 1:
					u.cell(p).addPOS(POS_EU_PROKLITIKO);
			}
			switch (u.getHDicDB()->query(u.cell(p).getHDicRef(),HDIC_QUERY_EU_ENKLITIKO)) {
				case 1:
					u.cell(p).addPOS(POS_EU_ENKLITIKO);
			}
			switch (u.getHDicDB()->query(u.cell(p).getHDicRef(),HDIC_QUERY_EU_PAU_AUR)) {
				case 1:
					u.cell(p).addPOS(POS_EU_PAUSE_AURRE);
			}
*/

	switch (u.getHDicDB()->query(u.cell(p).getHDicRef(),HDIC_QUERY_EU_PAU_ATZE)) {
		case 1:
			u.cell(p).addPOS(POS_EU_PAUSE_ATZE);
	}

/**Hitzegiko hitz batean Transkripzio fonetikoaren salbuespena baldin badu, informazioa gelaxkan jartzen dugu, 2005/02/22***/
/*KODEKETA BERRIA 2005/03/2************************************/
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
switch (u.getHDicDB()->query(u.cell(p).getHDicRef(),HDIC_QUERY_EU_SALBTF_Z_0_T)) { //INAKI salbuespena z->T (ad. Zaragoza)
		case 1:
			u.cell(p).addPOS(POS_EU_SALBTF_Z_T);

	}
/****************************************************************/

	encontrado = TRUE;

}

//**********************************************************************

VOID LangEU_Categ::aditudu(UttWS &u,UttI p,char *word_act)
{
	INT len_string1=0;
	CHAR atzizki[MAX_TAM_WORD]="\0";

#ifdef DEBUGPOS
	htts_warn("  Pos1: ADITUDU: word_act: %s",u.cell(p).getWord());
#endif
	len_string1=strlen(word_act);
	if (len_string1>2) {
		strcpy(atzizki,&word_act[len_string1-2]);
		if (!strcmp(atzizki,"du") || !strcmp(atzizki,"tu")){
			u.cell(p).setPOS(POS_EU_ADI_JOK);
			encontrado = TRUE;
		}
	}
}

//**********************************************************************

VOID LangEU_Categ::babait(UttWS &u,UttI p, char *word_act,LangEU_HDicDB &db)
{
	INT len_adi=0,len_aux_temp=0;
	CHAR adi[MAX_TAM_WORD]="\0";
	CHAR atz1[MAX_TAM_WORD]="\0", atz2[MAX_TAM_WORD]="\0";
	CHAR word_act2[MAX_TAM_WORD]="\0",aux_temp[MAX_TAM_WORD]="\0";
	INT fin=0;
	HDicRef hDicRef;

	strcpy(adi,word_act);
	len_adi=strlen(adi);

	if (len_adi>=2) {
		strncpy(atz1,adi,2);	//copia 2 letras
		atz1[2]='\0';
	}

	if (len_adi>=3) {
		strncpy(atz2,adi,3);	//copia 3 letras
		atz2[3]='\0';
	}

	if (!strcmp(atz1,"ba")) {
		strcpy(word_act2,&adi[2]);
		fin=strlen(word_act2);
		word_act[fin]='\0';
		hDicRef=db.search(word_act2);
		switch (db.query(hDicRef,HDIC_QUERY_EU_TALDE4)) {
			case HDIC_ANSWER_EU_TALDE4_ADI_LGN:
				u.cell(p).setPOS(POS_EU_ADI_LGN);
				encontrado = TRUE;
				break;
		}
		switch (db.query(hDicRef,HDIC_QUERY_EU_TALDE3)) {
			case HDIC_ANSWER_EU_TALDE3_ADI_TRN:
				u.cell(p).addPOS(POS_EU_ADI_TRN);
				encontrado = TRUE;
				break;
		}
	}

	if (encontrado == FALSE) {
		if (!strcmp(atz2,"bai"))
			switch(word_act[3]) {
				case 't':
					if (len_adi>=3) {
						strcpy(aux_temp,&adi[3]);
						len_aux_temp=strlen(aux_temp);
						aux_temp[len_aux_temp]='\0';
						aux_temp[0]='d';
						hDicRef=db.search(aux_temp);
						switch (db.query(hDicRef,HDIC_QUERY_EU_TALDE4)) {
							case HDIC_ANSWER_EU_TALDE4_ADI_LGN:
								u.cell(p).setPOS(POS_EU_ADI_LGN);
								encontrado = TRUE;
							break;
						}
						switch (db.query(hDicRef,HDIC_QUERY_EU_TALDE3)) {
							case HDIC_ANSWER_EU_TALDE3_ADI_TRN:
								u.cell(p).addPOS(POS_EU_ADI_TRN);
								encontrado = TRUE;
							break;
						}
					}
					if (encontrado) break;

					if (len_adi>=4) {
						strcpy(aux_temp,&adi[4]);
						len_aux_temp=strlen(aux_temp);
						aux_temp[len_aux_temp]='\0';
						hDicRef=db.search(aux_temp);
						switch (db.query(hDicRef,HDIC_QUERY_EU_TALDE4)) {
							case HDIC_ANSWER_EU_TALDE4_ADI_LGN:
								u.cell(p).setPOS(POS_EU_ADI_LGN);
								encontrado = TRUE;
							break;
						}
						switch (db.query(hDicRef,HDIC_QUERY_EU_TALDE3)) {
							case HDIC_ANSWER_EU_TALDE3_ADI_TRN:
								u.cell(p).addPOS(POS_EU_ADI_TRN);
								encontrado = TRUE;
							break;
						}
					}
					break;

				case 'k':
					if (len_adi>=3) {
						strcpy(aux_temp,&adi[3]);
						len_aux_temp=strlen(aux_temp);
						aux_temp[len_aux_temp]='\0';
						aux_temp[0]='g';
						hDicRef=db.search(aux_temp);
						switch (db.query(hDicRef,HDIC_QUERY_EU_TALDE4)) {
							case HDIC_ANSWER_EU_TALDE4_ADI_LGN:
								u.cell(p).setPOS(POS_EU_ADI_LGN);
								encontrado = TRUE;
							break;
						}
						switch (db.query(hDicRef,HDIC_QUERY_EU_TALDE3)) {
							case HDIC_ANSWER_EU_TALDE3_ADI_TRN:
								u.cell(p).addPOS(POS_EU_ADI_TRN);
								encontrado = TRUE;
							break;
						}
					}

				default:
					if (len_adi>=4) {
						strcpy(aux_temp,&adi[4]);
						len_aux_temp=strlen(aux_temp);
						aux_temp[len_aux_temp]='\0';
						hDicRef=db.search(aux_temp);
						switch (db.query(hDicRef,HDIC_QUERY_EU_TALDE4)) {
							case HDIC_ANSWER_EU_TALDE4_ADI_LGN:
								u.cell(p).setPOS(POS_EU_ADI_LGN);
								encontrado = TRUE;
							break;
						}
						switch (db.query(hDicRef,HDIC_QUERY_EU_TALDE3)) {
							case HDIC_ANSWER_EU_TALDE3_ADI_TRN:
								u.cell(p).addPOS(POS_EU_ADI_TRN);
								encontrado = TRUE;
							break;
						}
					}
			}
	}

}

//**********************************************************************

VOID LangEU_Categ::atzize(UttWS &u,UttI p, char *word_act,LangEU_HDicDB &db)
{
	CHAR atzizki[MAX_TAM_WORD];
	INT len_string1,len_atz,tam,len_atz2;
	HDicRef hDicRef;

	len_string1=strlen(word_act);
	len_atz=0;
	while ((len_atz!=len_string1) && (!encontrado)) {
		strcpy(atzizki,&word_act[len_atz]);
		len_atz2=strlen(atzizki);
		atzizki[++len_atz2]='\0';
		hDicRef=db.search(atzizki);
		tam=db.query(hDicRef,HDIC_QUERY_MATCHLEN);
#ifdef DEBUGPOS
					htts_warn("  Pos1: ATZIZE Buscando [%s], encontrado HDicRef %s", atzizki, db.hDicRefToTxt(hDicRef));
#endif

		switch (db.query(hDicRef,HDIC_QUERY_EU_TALDE2)) {
			case HDIC_ANSWER_EU_TALDE2_ATZ_IZE:
				if (tam==0) {
						u.cell(p).addPOS(POS_EU_ATZ_IZE);
						encontrado = TRUE;

#ifdef DEBUGPOS
					htts_warn("  Pos1: ATZIZE Buscando [%s], encontrado? %d", atzizki, encontrado);
#endif

					}
				break;
			}
			len_atz++;
	}
}

//**********************************************************************

VOID LangEU_Categ::atzadi(UttWS &u,UttI p, char *word_act,LangEU_HDicDB &db)
{
	CHAR atzizki[MAX_TAM_WORD]="\0";
	CHAR string_tmp[MAX_TAM_WORD]="\0";
	INT len_string1,len_atz2,len_string_tmp;
	HDicRef hDicRef,hDicRef2;
	INT num_eq=0;

/*******************************************/
/* 3.- Comprobar atzizki de verbo (ko, go) */
/*******************************************/

#ifdef DEBUGPOS
	htts_warn("  Pos1: Compruebo atzizki de verbo ko,go: [%s]", word_act);
#endif
	len_string1=strlen(word_act);
	if (len_string1>=2){
		strcpy(atzizki,&word_act[len_string1-2]);
		len_atz2=strlen(atzizki);
		atzizki[len_atz2]='\0';
		hDicRef=db.search(atzizki);
		switch (db.query(hDicRef,HDIC_QUERY_EU_TALDE1)) {
			case HDIC_ANSWER_EU_TALDE1_ATZ_ADI1:
				strncpy(string_tmp,word_act,len_string1-2);
				len_string_tmp=len_string1-2;	//2 del ko/go
				atzizki[len_string_tmp]='\0';
				hDicRef2=db.search(string_tmp);
				switch (db.query(hDicRef2,HDIC_QUERY_EU_TALDE1)) {
					case HDIC_ANSWER_EU_TALDE1_ADI_JOK:
						num_eq=u.getHDicDB()->query(hDicRef2,HDIC_QUERY_MATCHLEN);
						if (num_eq==0) {
							u.cell(p).addPOS(POS_EU_ADI_JOK);
							u.cell(p).addPOS(POS_EU_ATZ_ADI1);
							encontrado = TRUE;
#ifdef DEBUGPOS
							htts_warn("  Pos1: Verbo ko,go encontrado.");
#endif

							}
					}
			}
		}

/*******************************************************************/
/* 2.- Comprobar atzizki de verbo, nominalizacion--> empieza por t */
/*******************************************************************/

		if (!encontrado)	{
#ifdef DEBUGPOS
		htts_warn("  Pos1: Compruebo atzizki de verbo tea, tzea: [%s] con ADIT", word_act);
#endif
		adit(u,p,word_act,db);
#ifdef DEBUGPOS
		if (encontrado) htts_warn("  Pos1: Verbo tea, tzea encontrado con ADIT.");
#endif
		}

		if (!encontrado)	{
#ifdef DEBUGPOS
		htts_warn("  Pos1: Compruebo atzizki de verbo tea, tzea: [%s] con AUXT", word_act);
#endif
		auxt(u,p,word_act,db);
#ifdef DEBUGPOS
		if (encontrado) htts_warn("  Pos1: Verbo tea, tzea encontrado con AUXT.");
#endif
		}

}

/*********************************************************/


/*********************************************************/

VOID LangEU_Categ::adit(UttWS &u, UttI p, char *word_act, LangEU_HDicDB
&db)
{
char atzizki[MAX_TAM_WORD]="\0";
char adi_ref[MAX_TAM_WORD]="\0";
char adi[MAX_TAM_WORD]="\0";
strcpy(adi,word_act);

HDicRef hDicRef;
unsigned int tam=0,len_atz=0;

char adi_temp[MAX_TAM_WORD]="\0";
int len_adi=0;
int t=0, i=0,z=0;
BOOL atz_flag=FALSE,atz_adi1=FALSE,atz_adi2=FALSE;

len_adi=strlen(adi);
strcpy(adi_temp,"\0");

t=0;
i=len_adi;
		while( (t==0) && (i!=0)){
			i--;
			if (adi[i]=='t')
				switch (adi[i+1]) {
					case	'z':	//tzek...
						t=1;
						z=1;
					break;
					case	'e':	//tek....
						t=1;
						z=0;
					break;
					default:
						t=0;
					break;
				}
		} //fin del while
		strcpy(atzizki,&adi[i]);
		len_atz=len_adi-i;
		atzizki[len_atz]='\0';

		hDicRef=db.search(atzizki);
		tam=db.query(hDicRef,HDIC_QUERY_MATCHLEN);
		strcpy(adi_ref,u.getHDicDB()->hDicRefToTxt(hDicRef));

#ifdef DEBUGPOS
		htts_warn("  Pos1: ADIT atzizki %s",atzizki);
#endif

		if (tam==0){
			switch(db.query(hDicRef,HDIC_QUERY_EU_TALDE1)){
				case HDIC_ANSWER_EU_TALDE1_ATZ_ADI1:
					atz_flag=TRUE;
					atz_adi1=TRUE;
#ifdef DEBUGPOS
		htts_warn("  Pos1: ADIT atz_adi1?  %d",atz_adi1);
#endif
				break;
			}
			switch(db.query(hDicRef,HDIC_QUERY_EU_TALDE2)){
				case HDIC_ANSWER_EU_TALDE2_ATZ_ADI2:
					atz_flag=TRUE;
					atz_adi2=TRUE;
#ifdef DEBUGPOS
		htts_warn("  Pos1: ADIT atz_adi2?  %d",atz_adi2);
#endif
				break;
			}
		}


/* Busca si es un verbo acabado en tea, tekotan, teko... */
		if ((z==0)&&(t==1)&&(atz_flag==TRUE)){
			strncpy(adi_temp,adi,i);
			adi_temp[i]='\0';
			hDicRef=db.search(adi_temp);
			strcpy(adi_ref,u.getHDicDB()->hDicRefToTxt(hDicRef));
			tam=db.query(hDicRef,HDIC_QUERY_MATCHLEN);
			switch(db.query(hDicRef,HDIC_QUERY_EU_TALDE1)){
				case HDIC_ANSWER_EU_TALDE1_ADI_JOK:
					if (tam==0)
							encontrado = TRUE;
#ifdef DEBUGPOS
					htts_warn("  Pos1: ADIT Lo de delante de tea es un verbo principal. [%s]", adi_temp);
#endif

				break;
			}

			if (!encontrado) {
				switch (adi[i-1]) {
					case 'a':
					case 'e':
					case 'i':
					case 'o':
					case 'u':
						adi_temp[i]='n';
						adi_temp[i+1]='\0';
						hDicRef=db.search(adi_temp);
						tam=db.query(hDicRef,HDIC_QUERY_MATCHLEN);
						strcpy(adi_ref,u.getHDicDB()->hDicRefToTxt(hDicRef));
						switch(db.query(hDicRef,HDIC_QUERY_EU_TALDE1)){
							case HDIC_ANSWER_EU_TALDE1_ADI_JOK:
								if (tam==0)
									encontrado = TRUE;
#ifdef DEBUGPOS
					htts_warn("  Pos1: ADIT Lo de delante de tea-tzea es un verbo principal acabado en n. [%s]", adi_temp);
#endif
							break;
						}
						break;

					case 'z':
						strcpy(adi_temp,adi);
						adi_temp[i+1]='u';
						adi_temp[i+2]='\0';
						hDicRef=db.search(adi_temp);
						tam=db.query(hDicRef,HDIC_QUERY_MATCHLEN);
						strcpy(adi_ref,u.getHDicDB()->hDicRefToTxt(hDicRef));
						switch(db.query(hDicRef,HDIC_QUERY_EU_TALDE1)){
							case HDIC_ANSWER_EU_TALDE1_ADI_JOK:
								if (tam==0)
									encontrado = TRUE;
#ifdef DEBUGPOS
          htts_warn("  Pos1: ADIT Lo de delante de tea-tzea+tu es un verbo principal tipo orraztu (con z antes de t). [%s]", adi_temp);
#endif

							break;
						}
						if (encontrado) break;

						strcpy(adi_temp,adi);
						adi_temp[i]='i';
						adi_temp[i+1]='\0';
						hDicRef=db.search(adi_temp);
						tam=db.query(hDicRef,HDIC_QUERY_MATCHLEN);
						strcpy(adi_ref,u.getHDicDB()->hDicRefToTxt(hDicRef));

						switch(db.query(hDicRef,HDIC_QUERY_EU_TALDE1)){
							case HDIC_ANSWER_EU_TALDE1_ADI_JOK:
								if (tam==0)
										encontrado = TRUE;
#ifdef DEBUGPOS
          htts_warn("  Pos1: ADIT Lo de delante de tea+i es un verbo principal tipo adierazi. [%s]", adi_temp);
#endif

								break;
						}
						if (encontrado) break;

						strcpy(adi_temp,adi);
						adi_temp[i]='z';   //parece que no puede ser otra...
						adi_temp[i-1]='t';
						adi_temp[i+1]='i';
						adi_temp[i+2]='\0';
						hDicRef=db.search(adi_temp);
						tam=db.query(hDicRef,HDIC_QUERY_MATCHLEN);
						strcpy(adi_ref,u.getHDicDB()->hDicRefToTxt(hDicRef));
						switch(db.query(hDicRef,HDIC_QUERY_EU_TALDE1)){
							case HDIC_ANSWER_EU_TALDE1_ADI_JOK:
									if (tam==0)
										encontrado = TRUE;
#ifdef DEBUGPOS
          htts_warn("  Pos1: ADIT Lo de delante de tea es un verbo principal tipo jantzi. [%s]", adi_temp);
#endif

						}

					break;

					case 's':
						strcpy(adi_temp,adi);
						adi_temp[i]='i';
						adi_temp[i+1]='\0';
						hDicRef=db.search(adi_temp);
						tam=db.query(hDicRef,HDIC_QUERY_MATCHLEN);
						strcpy(adi_ref,u.getHDicDB()->hDicRefToTxt(hDicRef));

						switch(db.query(hDicRef,HDIC_QUERY_EU_TALDE1)){
							case HDIC_ANSWER_EU_TALDE1_ADI_JOK:
								if (tam==0)
									encontrado = TRUE;
#ifdef DEBUGPOS
          htts_warn("  Pos1: ADIT Lo de delante de tea-tzea+i es un verbo principal tipo erosi. [%s]", adi_temp);
#endif

							break;
						}
						if (encontrado) break;

						strcpy(adi_temp,adi);
						adi_temp[i]='s';   //parece que no puede ser otra...
						adi_temp[i-1]='t';
						adi_temp[i+1]='i';
						adi_temp[i+2]='\0';
						hDicRef=db.search(adi_temp);
						tam=db.query(hDicRef,HDIC_QUERY_MATCHLEN);
						strcpy(adi_ref,u.getHDicDB()->hDicRefToTxt(hDicRef));
						switch(db.query(hDicRef,HDIC_QUERY_EU_TALDE1)){
							case HDIC_ANSWER_EU_TALDE1_ADI_JOK:
									if (tam==0)
										encontrado = TRUE;
#ifdef DEBUGPOS
          htts_warn("  Pos1: ADIT Lo de delante de tea es un verbo principal tipo erantsi. [%s]", adi_temp);
#endif

						}

						if (encontrado) break;

						strcpy(adi_temp,adi);
						len_adi=strlen(adi_temp);
						if (len_adi>=3) {
							adi_temp[i+1]='u';
							adi_temp[i+2]='\0';
							hDicRef=db.search(adi_temp);
							strcpy(adi_ref,u.getHDicDB()->hDicRefToTxt(hDicRef));
							tam=db.query(hDicRef,HDIC_QUERY_MATCHLEN);
							switch(db.query(hDicRef,HDIC_QUERY_EU_TALDE1)){
								case HDIC_ANSWER_EU_TALDE1_ADI_JOK:
									if (tam==0)
										encontrado = TRUE;
#ifdef DEBUGPOS
          htts_warn("  Pos1: ADIT Lo de delante de tea+tu es un verbo principal tipo aberastu. [%s]", adi_temp);
#endif
								break;
							}
						}

						break;

					case 'x':
						strcpy(adi_temp,adi);
						adi_temp[i]='x';   //parece que no puede ser otra...
						adi_temp[i-1]='t';
						adi_temp[i+1]='i';
						adi_temp[i+2]='\0';
						hDicRef=db.search(adi_temp);
						tam=db.query(hDicRef,HDIC_QUERY_MATCHLEN);
						strcpy(adi_ref,u.getHDicDB()->hDicRefToTxt(hDicRef));
						switch(db.query(hDicRef,HDIC_QUERY_EU_TALDE1)){
							case HDIC_ANSWER_EU_TALDE1_ADI_JOK:
									if (tam==0)
										encontrado = TRUE;
#ifdef DEBUGPOS
          htts_warn("  Pos1: ADIT Lo de delante de tea-tzea es un verbo principal tipo itxi. [%s]", adi_temp);
#endif

							break;
						}
						break;

					default:
						if (!encontrado) {
							strcpy(adi_temp,adi);
							len_adi=strlen(adi_temp);
							if (len_adi>=3) {
								adi_temp[i+1]='u';
								adi_temp[i+2]='\0';
								hDicRef=db.search(adi_temp);
								strcpy(adi_ref,u.getHDicDB()->hDicRefToTxt(hDicRef));
								tam=db.query(hDicRef,HDIC_QUERY_MATCHLEN);
								switch(db.query(hDicRef,HDIC_QUERY_EU_TALDE1)){
									case HDIC_ANSWER_EU_TALDE1_ADI_JOK:
										if (tam==0)
											encontrado = TRUE;
#ifdef DEBUGPOS
          htts_warn("  Pos1: ADIT Lo de delante de tea+tu es un verbo principal tipo inexistente. [%s]", adi_temp);
#endif
									break;
								}
							}
						}

						if (!encontrado) {
							strcpy(adi_temp,adi);
							len_adi=strlen(adi_temp);
							if (len_adi>=3) {
								adi_temp[i]='d';
								adi_temp[i+1]='u';
								adi_temp[i+2]='\0';
								hDicRef=db.search(adi_temp);
								strcpy(adi_ref,u.getHDicDB()->hDicRefToTxt(hDicRef));
								tam=db.query(hDicRef,HDIC_QUERY_MATCHLEN);
								switch(db.query(hDicRef,HDIC_QUERY_EU_TALDE1)){
									case HDIC_ANSWER_EU_TALDE1_ADI_JOK:
										if (tam==0)
											encontrado = TRUE;
#ifdef DEBUGPOS
          htts_warn("  Pos1: ADIT Lo de delante de tea+du es un verbo principal tipo inexistente. [%s]", adi_temp);
#endif

									break;
								}
							}
						} //findel is encontrado
				}//fin del switch
			} //findel is encontrado
		}
/* Hemos acabado de buscar los de tea, tekotan,... Si era uno de esos tam
sera 0 y encontrado TRUE */


/* Ahora buscamos los acabados en tzea, tzekotan, tzeko... */
		if (!encontrado) {
			if ((z==1)&&(t==1)&&(atz_flag==TRUE)){			//caso tzen, tzekotan,...
				strncpy(adi_temp,adi,i);
				adi_temp[i]='\0';
				hDicRef=db.search(adi_temp);
				tam=db.query(hDicRef,HDIC_QUERY_MATCHLEN);
				strcpy(adi_ref,u.getHDicDB()->hDicRefToTxt(hDicRef));
				switch(db.query(hDicRef,HDIC_QUERY_EU_TALDE1)){
					case HDIC_ANSWER_EU_TALDE1_ADI_JOK:
						if (tam==0)
							encontrado = TRUE;
#ifdef DEBUGPOS
          htts_warn("  Pos1: ADIT Lo de delante de tzea es un verbo principal. [%s]", adi_temp);
#endif
					break;
				}
				if (!encontrado) {
					switch (adi[i-1]) {
						case 'r':
						case 'n':
						case 'l':
							strcpy(adi_temp,adi);
							adi_temp[i]='i';
							adi_temp[i+1]='\0';
							hDicRef=db.search(adi_temp);
							tam=db.query(hDicRef,HDIC_QUERY_MATCHLEN);
							strcpy(adi_ref,u.getHDicDB()->hDicRefToTxt(hDicRef));
							switch(db.query(hDicRef,HDIC_QUERY_EU_TALDE1)){
								case HDIC_ANSWER_EU_TALDE1_ADI_JOK:
									if (tam==0)
										encontrado = TRUE;
#ifdef DEBUGPOS
          htts_warn("  Pos1: ADIT Lo de delante de tzea+i es un verbo principal tipo erori. [%s]", adi_temp);
#endif
								break;
							}
							if (encontrado) 	break;

							strcpy(adi_temp,adi);

							adi_temp[i]='r';
							adi_temp[i+1]='i';
							adi_temp[i+2]='\0';
							hDicRef=db.search(adi_temp);
							strcpy(adi_ref,u.getHDicDB()->hDicRefToTxt(hDicRef));
							tam=db.query(hDicRef,HDIC_QUERY_MATCHLEN);
							switch(db.query(hDicRef,HDIC_QUERY_EU_TALDE1)){
								case HDIC_ANSWER_EU_TALDE1_ADI_JOK:
									if (tam==0) encontrado = TRUE;
#ifdef DEBUGPOS
          htts_warn("  Pos1: ADIT Lo de delante de tzea+ri es un verbo principal tipo ekarri. [%s]", adi_temp);
#endif
								break;

							}
							if (encontrado) 	break;

							strcpy(adi_temp,adi);

							adi_temp[i]='t';
							adi_temp[i+1]='u';
							adi_temp[i+2]='\0';
							hDicRef=db.search(adi_temp);
							strcpy(adi_ref,u.getHDicDB()->hDicRefToTxt(hDicRef));
							tam=db.query(hDicRef,HDIC_QUERY_MATCHLEN);
							switch(db.query(hDicRef,HDIC_QUERY_EU_TALDE1)){
								case HDIC_ANSWER_EU_TALDE1_ADI_JOK:
									if (tam==0) encontrado = TRUE;
#ifdef DEBUGPOS
          htts_warn("  Pos1: ADIT Lo de delante de tzea+ri es un verbo principal tipo sartu. [%s]", adi_temp);
#endif
								break;

							}

							if (encontrado) 	break;

							strcpy(adi_temp,adi);

							adi_temp[i]='d';
							adi_temp[i+1]='u';
							adi_temp[i+2]='\0';
							hDicRef=db.search(adi_temp);
							strcpy(adi_ref,u.getHDicDB()->hDicRefToTxt(hDicRef));
							tam=db.query(hDicRef,HDIC_QUERY_MATCHLEN);
							switch(db.query(hDicRef,HDIC_QUERY_EU_TALDE1)){
								case HDIC_ANSWER_EU_TALDE1_ADI_JOK:
									if (tam==0) encontrado = TRUE;
#ifdef DEBUGPOS
          htts_warn("  Pos1: ADIT Lo de delante de tzea+ri es un verbo principal tipo saldu. [%s]", adi_temp);
#endif
								break;

							}

						break;
					 default:
							if (len_adi>=4) {

								strcpy(adi_temp,adi);
								adi_temp[i+1]='u';
								adi_temp[i+2]='\0';
								hDicRef=db.search(adi_temp);
								strcpy(adi_ref,u.getHDicDB()->hDicRefToTxt(hDicRef));
								tam=db.query(hDicRef,HDIC_QUERY_MATCHLEN);
								switch(db.query(hDicRef,HDIC_QUERY_EU_TALDE1)){
									case HDIC_ANSWER_EU_TALDE1_ADI_JOK:
										if (tam==0) encontrado = TRUE;
#ifdef DEBUGPOS
          htts_warn("  Pos1: ADIT Lo de delante de tzea+tu es un verbo principal tipo kontatu. [%s]", adi_temp);
#endif
									break;
								}
							}

							if (!encontrado) {
								if (len_adi>=4) {

									strcpy(adi_temp,adi);
									adi_temp[i]='d';
									adi_temp[i+1]='u';
									adi_temp[i+2]='\0';
									hDicRef=db.search(adi_temp);
									strcpy(adi_ref,u.getHDicDB()->hDicRefToTxt(hDicRef));
									tam=db.query(hDicRef,HDIC_QUERY_MATCHLEN);
									switch(db.query(hDicRef,HDIC_QUERY_EU_TALDE1)){
										case HDIC_ANSWER_EU_TALDE1_ADI_JOK:
											encontrado = TRUE;
#ifdef DEBUGPOS
          htts_warn("  Pos1: ADIT Lo de delante de tzea+du es un verbo principal tipo saldu. [%s]", adi_temp);
#endif
										break;

									}
								}
							}//fin de if encontrado
						}//r
					}//encontrado
				}//caso tzen
			}//no encontrado

#ifdef DEBUGPOS
		htts_warn("  Pos1: ADIT encontrado %d, tam %d, atz_adi1 %d, atz_adi2 %d", encontrado, tam, atz_adi1, atz_adi2);
#endif

			if ((encontrado)&&(tam==0)){

				if (atz_adi1==TRUE)	{
					u.cell(p).setPOS(POS_EU_ATZ_ADI1);	//ten o tzen
					u.cell(p).addPOS(POS_EU_ADI_JOK);
				}
				if (atz_adi2==TRUE)
						u.cell(p).setPOS(POS_EU_ATZ_ADI2);
			}
}


/*********************************************************/

VOID LangEU_Categ::auxt(UttWS &u, UttI p, char *word_act, LangEU_HDicDB &db)
{
char atzizki[MAX_TAM_WORD]="\0";
char adi[MAX_TAM_WORD]="\0";
char adi_ref[MAX_TAM_WORD]="\0";

HDicRef hDicRef;
unsigned int tam=0;

char adi_temp[MAX_TAM_WORD]="\0";
unsigned int len_adi=0,len_atz=0;
int l=0,n=0,i=0;	//l atz en la,lako,etc
							//atz en narena, narekin, etc


strcpy(adi,word_act);
len_adi=strlen(adi);


i=len_adi;
		while( ((l==0)&&(n==0)) && (i!=0)){
			i--;
			if (adi[i]=='l') {
				strcpy(atzizki,&adi[i]);
#ifdef DEBUGPOS
		htts_warn("  Pos1: AUXT palabra %s, atzizki %s", adi,atzizki);
#endif
				len_atz=len_adi-i;
				atzizki[len_atz]='\0';

				hDicRef=db.search(atzizki);
				tam=db.query(hDicRef,HDIC_QUERY_MATCHLEN);
				strcpy(adi_ref,u.getHDicDB()->hDicRefToTxt(hDicRef));
				if (tam==0){
					switch(db.query(hDicRef,HDIC_QUERY_EU_TALDE3)){
						case HDIC_ANSWER_EU_TALDE3_ATZ_ADI3:
							l=1;
						break;
					}
				}
			}

			if (adi[i]=='n') {
				strcpy(atzizki,&adi[i]);
#ifdef DEBUGPOS
		htts_warn("  Pos1: AUXT palabra %s, atzizki %s", adi,atzizki);
#endif
				len_atz=len_adi-i;
				atzizki[len_atz]='\0';

				hDicRef=db.search(atzizki);
				tam=db.query(hDicRef,HDIC_QUERY_MATCHLEN);
				strcpy(adi_ref,u.getHDicDB()->hDicRefToTxt(hDicRef));

				if (tam==0){
					switch(db.query(hDicRef,HDIC_QUERY_EU_TALDE3)){
						case HDIC_ANSWER_EU_TALDE3_ATZ_ADI3:
							n=1;
						break;
					}
				}

			}
		} //fin del while

		if ((n==1)||(l==1)){
			strncpy(adi_temp,adi,i);
			adi_temp[i]='\0';
			hDicRef=db.search(adi_temp);
			strcpy(adi_ref,u.getHDicDB()->hDicRefToTxt(hDicRef));
			tam=db.query(hDicRef,HDIC_QUERY_MATCHLEN);
			if (tam==0){
				switch(db.query(hDicRef,HDIC_QUERY_EU_TALDE4)){
					case HDIC_ANSWER_EU_TALDE4_ADI_LGN:
						u.cell(p).setPOS(POS_EU_ATZ_ADI3);
						u.cell(p).addPOS(POS_EU_ADI_LGN);
						encontrado = TRUE;
#ifdef DEBUGPOS
		htts_warn("  Pos1: AUXT Buscando %s encontrado? %d", adi_temp,encontrado);
#endif

					break;
				}
				switch(db.query(hDicRef,HDIC_QUERY_EU_TALDE3)){
					case HDIC_ANSWER_EU_TALDE3_ADI_TRN:
						if (!u.cell(p).queryPOS(POS_EU_ATZ_ADI3))
							u.cell(p).addPOS(POS_EU_ATZ_ADI3);
						u.cell(p).addPOS(POS_EU_ADI_TRN);
						encontrado = TRUE;
#ifdef DEBUGPOS
		htts_warn("  Pos1: AUXT Buscando %s encontrado? %d", adi_temp,encontrado);
#endif

					break;
				}
			}

			if (!encontrado) {
				switch (adi[i-1]) {
					case 'e':
						if (len_adi>=1) {
							strncpy(adi_temp,adi,i-1);
							adi_temp[i-1]='\0';
							hDicRef=db.search(adi_temp);
							tam=db.query(hDicRef,HDIC_QUERY_MATCHLEN);
							strcpy(adi_ref,u.getHDicDB()->hDicRefToTxt(hDicRef));
							if (tam==0){
								switch(db.query(hDicRef,HDIC_QUERY_EU_TALDE4)){
									case HDIC_ANSWER_EU_TALDE4_ADI_LGN:
										u.cell(p).setPOS(POS_EU_ATZ_ADI3);
										u.cell(p).addPOS(POS_EU_ADI_LGN);
										encontrado = TRUE;
#ifdef DEBUGPOS
		htts_warn("  Pos1: AUXT Buscando %s encontrado? %d", adi_temp,encontrado);
#endif
									break;
								}
								switch(db.query(hDicRef,HDIC_QUERY_EU_TALDE3)){
									case HDIC_ANSWER_EU_TALDE3_ADI_TRN:
										if (!u.cell(p).queryPOS(POS_EU_ATZ_ADI3))
											u.cell(p).addPOS(POS_EU_ATZ_ADI3);
										u.cell(p).addPOS(POS_EU_ADI_TRN);
										encontrado = TRUE;
#ifdef DEBUGPOS
		htts_warn("  Pos1: AUXT Buscando %s encontrado? %d", adi_temp,encontrado);
#endif

									break;
								}
							}
							if (!encontrado == 0) {
								strncpy(adi_temp,adi,i);
								if (i<MAX_TAM_WORD){
									adi_temp[i]='n';
									adi_temp[i+1]='\0';
								}
								hDicRef=db.search(adi_temp);
								tam=db.query(hDicRef,HDIC_QUERY_MATCHLEN);
								strcpy(adi_ref,u.getHDicDB()->hDicRefToTxt(hDicRef));
								if (tam==0){
									switch(db.query(hDicRef,HDIC_QUERY_EU_TALDE4)){
										case HDIC_ANSWER_EU_TALDE4_ADI_LGN:
											u.cell(p).setPOS(POS_EU_ATZ_ADI3);
											u.cell(p).addPOS(POS_EU_ADI_LGN);
											encontrado = TRUE;
#ifdef DEBUGPOS
		htts_warn("  Pos1: AUXT Buscando %s encontrado? %d", adi_temp,encontrado);
#endif
										break;
									}
									switch(db.query(hDicRef,HDIC_QUERY_EU_TALDE3)){
										case HDIC_ANSWER_EU_TALDE3_ADI_TRN:
											if (!u.cell(p).queryPOS(POS_EU_ATZ_ADI3))
												u.cell(p).addPOS(POS_EU_ATZ_ADI3);
											u.cell(p).addPOS(POS_EU_ADI_TRN);
											encontrado = TRUE;
#ifdef DEBUGPOS
		htts_warn("  Pos1: AUXT Buscando %s encontrado? %d", adi_temp,encontrado);
#endif
										break;
									}
								}
							}

							if (!encontrado) {
								if (adi[i-2]=='r') {
									if (len_adi>=1) {
										strncpy(adi_temp,adi,i-1);
										adi_temp[i-2]='\0';
										hDicRef=db.search(adi_temp);
										tam=db.query(hDicRef,HDIC_QUERY_MATCHLEN);
										strcpy(adi_ref,u.getHDicDB()->hDicRefToTxt(hDicRef));
									}

									if (tam==0){
										switch(db.query(hDicRef,HDIC_QUERY_EU_TALDE4)){
											case HDIC_ANSWER_EU_TALDE4_ADI_LGN:
												u.cell(p).setPOS(POS_EU_ATZ_ADI3);
												u.cell(p).addPOS(POS_EU_ADI_LGN);
												encontrado = TRUE;
#ifdef DEBUGPOS
		htts_warn("  Pos1: AUXT Buscando %s encontrado? %d", adi_temp,encontrado);
#endif
											break;
										}
										switch(db.query(hDicRef,HDIC_QUERY_EU_TALDE3)){
											case HDIC_ANSWER_EU_TALDE3_ADI_TRN:
												if (!u.cell(p).queryPOS(POS_EU_ATZ_ADI3))
													u.cell(p).addPOS(POS_EU_ATZ_ADI3);
												u.cell(p).addPOS(POS_EU_ADI_TRN);
												encontrado = TRUE;
#ifdef DEBUGPOS
		htts_warn("  Pos1: AUXT Buscando %s encontrado? %d", adi_temp,encontrado);
#endif
											break;
										}
									}
								}
							}
							if (!encontrado) {
								strncpy(adi_temp,adi,i-1);
								adi_temp[i-1]='a';
								adi_temp[i]='\0';
								hDicRef=db.search(adi_temp);
								tam=db.query(hDicRef,HDIC_QUERY_MATCHLEN);
								strcpy(adi_ref,u.getHDicDB()->hDicRefToTxt(hDicRef));
								if (tam==0){
									switch(db.query(hDicRef,HDIC_QUERY_EU_TALDE4)){
										case HDIC_ANSWER_EU_TALDE4_ADI_LGN:
											u.cell(p).setPOS(POS_EU_ATZ_ADI3);
											u.cell(p).addPOS(POS_EU_ADI_LGN);
											encontrado = TRUE;
#ifdef DEBUGPOS
		htts_warn("  Pos1: AUXT Buscando %s encontrado? %d", adi_temp,encontrado);
#endif
										break;
									}
									switch(db.query(hDicRef,HDIC_QUERY_EU_TALDE3)){
										case HDIC_ANSWER_EU_TALDE3_ADI_TRN:
											if (!u.cell(p).queryPOS(POS_EU_ATZ_ADI3))
												u.cell(p).addPOS(POS_EU_ATZ_ADI3);
											u.cell(p).addPOS(POS_EU_ADI_TRN);
											encontrado = TRUE;
#ifdef DEBUGPOS
		htts_warn("  Pos1: AUXT Buscando %s encontrado? %d", adi_temp,encontrado);
#endif
										break;
									}
								}
//								fprintf(stderr,"\nadi_ref %s atzizki %s\n",adi_ref,atzizki);
							}
						}//fin del if>1
					break;

					case 'a':
						if (len_adi>=2) {
							if(adi[i-2]=='d') {
								strncpy(adi_temp,adi,i);
								adi_temp[i-2]='t';
								adi_temp[i-1]='\0';
								hDicRef=db.search(adi_temp);
								tam=db.query(hDicRef,HDIC_QUERY_MATCHLEN);
								strcpy(adi_ref,u.getHDicDB()->hDicRefToTxt(hDicRef));

								if (tam==0){
									switch(db.query(hDicRef,HDIC_QUERY_EU_TALDE4)){
										case HDIC_ANSWER_EU_TALDE4_ADI_LGN:
											u.cell(p).setPOS(POS_EU_ATZ_ADI3);
											u.cell(p).addPOS(POS_EU_ADI_LGN);
											encontrado = TRUE;
#ifdef DEBUGPOS
		htts_warn("  Pos1: AUXT Buscando %s encontrado? %d", adi_temp,encontrado);
#endif
										break;
									}
									switch(db.query(hDicRef,HDIC_QUERY_EU_TALDE3)){
										case HDIC_ANSWER_EU_TALDE3_ADI_TRN:
											if (!u.cell(p).queryPOS(POS_EU_ATZ_ADI3))
												u.cell(p).addPOS(POS_EU_ATZ_ADI3);
											u.cell(p).addPOS(POS_EU_ADI_TRN);
											encontrado = TRUE;
#ifdef DEBUGPOS
		htts_warn("  Pos1: AUXT Buscando %s encontrado? %d", adi_temp,encontrado);
#endif
										break;
									}
								}
							}	else {//fin del if d
								adi_temp[i]='n';
								adi_temp[i+1]='\0';
								hDicRef=db.search(adi_temp);
								tam=db.query(hDicRef,HDIC_QUERY_MATCHLEN);
								strcpy(adi_ref,u.getHDicDB()->hDicRefToTxt(hDicRef));

								if (tam==0){
									switch(db.query(hDicRef,HDIC_QUERY_EU_TALDE4)){
										case HDIC_ANSWER_EU_TALDE4_ADI_LGN:
											u.cell(p).setPOS(POS_EU_ATZ_ADI3);
											u.cell(p).addPOS(POS_EU_ADI_LGN);
											encontrado = TRUE;
										break;
#ifdef DEBUGPOS
		htts_warn("  Pos1: AUXT Buscando %s encontrado? %d", adi_temp,encontrado);
#endif
									}
									switch(db.query(hDicRef,HDIC_QUERY_EU_TALDE3)){
										case HDIC_ANSWER_EU_TALDE3_ADI_TRN:
											if (!u.cell(p).queryPOS(POS_EU_ATZ_ADI3))
												u.cell(p).addPOS(POS_EU_ATZ_ADI3);
											u.cell(p).addPOS(POS_EU_ADI_TRN);
											encontrado = TRUE;
#ifdef DEBUGPOS
		htts_warn("  Pos1: AUXT Buscando %s encontrado? %d", adi_temp,encontrado);
#endif
										break;
									}
								}
							}
						} //fin del if >2
						if (encontrado) break;
				}//fin del switch
		}
	}
}
