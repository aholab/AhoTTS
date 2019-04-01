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

Nombre fuente................ uttws.cpp
Nombre paquete............... aHoTTS
Lenguaje fuente.............. C++
Estado....................... -
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Borja Etxebarria
.............................

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
1.0.4	 12/07/10	Inaki	 Añadir celdas para modificar la prosodia desde el texto (proyecto Aritz) HTTS_PROSO_VAL
1.0.3    28/09/00  Yon2.     A¤adir nuevo campo TYPENOR,
															cambio orden de campos.
1.0.2    13/05/00  borja     poner campos AGrp, FGrp
1.0.1    08/05/00  borja     activar POS
1.0.0    31/01/00  borja     codefreeze aHoTTS v1.0
0.0.0    24/11/97  borja     Codificacion inicial.

======================== Contenido ========================
<DOC>
</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include <stdlib.h>
#include "uti.h"
#include "uttws.hpp"
#include "httsmsg.h"

/**********************************************************/

KINDOF_DDEF(UttCellWS, UttCell)

KINDOF_DDEF(UttWS, Utt)

/**********************************************************/

#ifdef HTTS_IOTXT
HDicDB * UttCellWS::db = NULL;
#endif

/**********************************************************/

VOID UttCellWS::reset(VOID)
{
	UttCell::reset();
	sentence = USENTENCE_NONE;
	phrase = FALSE;
	pause = UPAUSE_NONE;
	word = NULL;
	pos = 0;
	agrp = 0;
	fgrp = 0;
	tnor = 0;
	#ifdef HTTS_PROSO_VAL
	val_break=0;//Aritz
	val_emphasis=0;//Aritz
	val_pitch=0;//Aritz
	val_range=0;//Aritz
	val_rate=0;//Aritz
	val_volume=0;//Aritz
	#endif
    emotion=UEMOTION_DEFAULT;
    emo_intensity=0;

#ifdef UTT_DUMMY
	wstatus = 0;
#endif
	dref=HDIC_REF_NULL;
#ifdef HTTS_TIMEEVS
	timeev=0;
#endif
}

/**********************************************************/

UttCellWS::UttCellWS(VOID)
{
	reset();
}

/**********************************************************/

UttCellWS::~UttCellWS(VOID)
{
	if (word) { free(word); word=NULL; }
}

/**********************************************************/

BOOL UttCellWS::isStartOf(UttLevel range) const
{
	// manejamos las fronteras conocidas...
	if (inURange(ULEVEL_SENTENCE, range) && (sentence != USENTENCE_NONE)) return TRUE;
	if (inURange(ULEVEL_PAUSE, range) && (pause != UPAUSE_NONE)) return TRUE;
	if (inURange(ULEVEL_PHRASE, range) && phrase) return TRUE;
	if (inURange(ULEVEL_AGRP, range) && agrp) return TRUE;
	if (inURange(ULEVEL_TNOR, range) && tnor) return TRUE;
	if (inURange(ULEVEL_FGRP, range) && fgrp) return TRUE;
	if (inURange(ULEVEL_WORD, range) && (word != 0)) return TRUE;

	// y llamamos a la clase base para las desconocidas. ver nota1
#ifdef HTTS_DEBUG
	return UttCell::isStartOf(delURange(addURange(ULEVEL_SENTENCE | ULEVEL_WORD | ULEVEL_PHRASE | ULEVEL_PAUSE | ULEVEL_AGRP | ULEVEL_TNOR | ULEVEL_FGRP), range));
#else
	return UttCell::isStartOf(range);
#endif

/* nota1: realmente no hace falta el delURange(), pero es que la clase
	 base si encuentra bits desconocidos por ella a ON da un error. Es
	 un debugger controlado por HTTS_DEBUG. Si se quita, ya no hace
	 falta. */
}

/**********************************************************/

VOID UttCellWS::setWord(const CHAR * code)
{
	if (word) free(word);
	word = code ? strdup(code) : (CHAR*)NULL;
}

/**********************************************************/

VOID UttCellWS::setPOS(INT code)
{
	pos=code;
#ifdef HTTS_DEBUG
	if (pos!=code) htts_error("UttCellWS::pos field too short");
#endif
}

/**********************************************************/

VOID UttCellWS::addPOS(INT pos2)
{
unsigned int posword;
posword=pos;
pos=posword+pos2;
}

/**********************************************************/

VOID UttCellWS::subPOS(INT pos2)
{
unsigned int posword;
posword=pos;
pos=posword-pos2;
}

/**********************************************************/

INT UttCellWS::queryPOS(INT pos2)
{
INT rdo1;
INT posword;
posword=pos;
rdo1=posword&pos2;

if ((posword==0) && (pos2==0))
	return 1;
else
	if (rdo1==0)
		return 0;
	else
		return 1;
}

/**********************************************************/

VOID UttCellWS::setFGrp(INT code )
{
	fgrp = code;
}

/**********************************************************/

VOID UttCellWS::setAGrp(INT code)
{
	agrp = code;
}

/**********************************************************/

VOID UttCellWS::setTNor(INT code)
{
	tnor = code;
}
#ifdef HTTS_PROSO_VAL
/***********************************************************/
VOID UttCellWS::setProso_val_break(int code)//Aritz
{
	val_break=code;
}
/********************************************************/
VOID UttCellWS::setProso_val_emphasis(int code)//Aritz
{
	val_emphasis=code;
}
/********************************************************/
VOID UttCellWS::setProso_val_pitch(int code)//Aritz
{
	val_pitch=code;
}
/*******************************************************/
VOID UttCellWS::setProso_val_range(int code)//Aritz
{
	val_range=code;
}
/*******************************************************/
VOID UttCellWS::setProso_val_rate(int code)//Aritz
{
	val_rate=code;
}
/***********************************************************/
VOID UttCellWS::setProso_val_volume(int code)//Aritz
{
	val_volume=code;
}
/**********************************************************/
INT UttCellWS::getProso_val_break(VOID) const//Aritz
{
	return val_break;
}
/******************************************************/
INT UttCellWS::getProso_val_emphasis(VOID) const//Aritz
{
	return val_emphasis;
}
/******************************************************/
INT UttCellWS::getProso_val_pitch(VOID) const//Aritz
{
	return val_pitch;
}
/*****************************************************/
INT UttCellWS::getProso_val_range(VOID) const//Aritz
{
	return val_range;
}
/****************************************************/
INT UttCellWS::getProso_val_rate(VOID) const//Aritz
{
	return val_rate;
}
/****************************************************/
INT UttCellWS::getProso_val_volume(VOID) const//Aritz
{
	return val_volume;
}
/**********************************************************/
#endif
#ifdef UTT_DUMMY
VOID UttCellWS::setWStatus(INT code)
{
	wstatus = code;
#ifdef HTTS_DEBUG
	if (wstatus!=code) htts_error("UttCellWS::wstatus field too short");
#endif
}
#endif

/**********************************************************/

#ifdef HTTS_IOTXT
VOID UttCellWS::__foutput( FILE *f ) const
{
	fprintf(f,"/");
	fprintf(f,"%c/",(CHAR)getSentence());

	const CHAR *w=getWord(); if (!w) w="";
	int l=UTT_DEF_WORD_WIDTH;
	char *env=getenv("WORDWIDTH"); if (env) l=atoi(env);
	fprintf(f,"%-*s/",l,w);


    if (getEmotion()) fprintf(f,"%02X/",(unsigned)(getEmotion()));
	else fprintf(f,"/");

     if (getEmoInt()) fprintf(f,"%02X/",(unsigned)(getEmoInt()));
	else fprintf(f,"/");

	const CHAR *s=NULL;
	if (db && getHDicRef()!=HDIC_REF_NULL) s=db->hDicRefToTxt(getHDicRef());
	fprintf(f,"%s/",s?s:"");

	if (getTNor()) fprintf(f,"%01X/",(unsigned)(getTNor()));
	else fprintf(f," /");

	fprintf(f,"%s/", getPhrase()?"p":" ");

	if (getPause()!=UPAUSE_NONE)
		fprintf(f,"%s%c/",(getPause()<0)?"~":"",(CHAR)abs(getPause()));
	else
		fprintf(f," /");

	if (getPOS()) fprintf(f,"%02X/",(unsigned)(getPOS()));
	else fprintf(f,"  /");

	if (getFGrp()) fprintf(f,"%01X/",(unsigned)(getFGrp()));
	else fprintf(f," /");

	if (getAGrp()) fprintf(f,"%01X/",(unsigned)(getAGrp()));
	else fprintf(f," /");
	#ifdef HTTS_PROSO_VAL
	//Aritz Se crea una celda para cada tipo de etiqueta y se introduce su valor correspondiente

	if(getProso_val_break()) fprintf(f,"%02d/",(unsigned)(getProso_val_break()));//Aritz
	else fprintf(f,"/");
	if(getProso_val_emphasis()) fprintf(f,"%02d/",(unsigned)(getProso_val_emphasis()));//Aritz
	else fprintf(f,"/");
	if(getProso_val_pitch()) fprintf(f,"%02d/",(unsigned)(getProso_val_pitch()));//Aritz
	else fprintf(f,"/");
	if(getProso_val_range()) fprintf(f,"%02d/",(unsigned)(getProso_val_range()));//Aritz
	else fprintf(f,"/");
	if(getProso_val_rate()) fprintf(f,"%02d/",(unsigned)(getProso_val_rate()));//Aritz
	else fprintf(f,"/");
	if(getProso_val_volume()) fprintf(f,"%02d/",(unsigned)(getProso_val_volume()));//Aritz
	else fprintf(f,"/");
	#endif

#ifdef HTTS_TIMEEVS
	if (getTimeEv()) fprintf(f,"%u/",(unsigned int)getTimeEv());
	else fprintf(f,"/");
#endif

}
#endif

/**********************************************************/

#ifdef HTTS_IOTXT
VOID UttCellWS::foutput( FILE *f ) const
{
	__foutput(f);
	fprintf(f,"\n");
}
#endif

/**********************************************************/

#ifdef HTTS_IOTXT
CHAR *UttCellWS::__finput( CHAR *s )
{
#define NEXT() s=next; next=strchr(s,'/'); if (!next) return s; *next='\0'; next++;
	CHAR *next;

	next=s;

	NEXT();
	if (*s) {
		if (!strchr(USENTENCE_ALLCODES,*s))  htts_error("Invalid UttCellWS::sentence (%s)",s);
		setSentence((Sentence)*s);
	}
	else setSentence(USENTENCE_NONE);

	NEXT(); if (*s) setWord(s);

	NEXT();
	if (*s) {
		char *tmp;
		int x=(int)strtol(s,&tmp,16);
		if (tmp!=s+strlen(s) || x < MIN_EMOTION || x > MAX_EMOTION) htts_error("Invalid UttCelWS::emotion (%s)",s);
		setEmotion(x);
	}

	NEXT();
	if (*s) {
		char *tmp;
		int x=(int)strtol(s,&tmp,16);
		if (tmp!=s+strlen(s) || x < MIN_INTENSITY  || x > MAX_INTENSITY) htts_error("Invalid UttCelWS::emo_intesity (%s)",s);
		setEmoInt(x);
	}


	NEXT(); if (db && (*s)) setHDicRef(db->txtToHDicRef(s));

	NEXT();
	if (*s) {
		char *tmp;
		int x=(int)strtol(s,&tmp,16);
		if (tmp!=s+strlen(s)) htts_error("Invalid UttCelWS::tnor (%s)",s);
		setTNor(x);
	}

	NEXT(); if (*s=='p') setPhrase(TRUE);

	NEXT();
	BOOL breath = (*s=='~')?TRUE:FALSE;
	if (breath) s++;
	if (*s) {
		if (!strchr(UPAUSE_ALLCODES,*s))  htts_error("Invalid UttCellWS::pause(%s)",s);
		setPause((breath?-1:+1) * (INT)(Pause)*s);
	}
	else setPause(UPAUSE_NONE);

	NEXT();
	if (*s) {
		char *tmp;
		int x=(int)strtol(s,&tmp,16);
		if (tmp!=s+strlen(s)) htts_error("Invalid UttCelWS::pos (%s)",s);
		setPOS(x);
	}

	NEXT();
	if (*s) {
		char *tmp;
		int x=(int)strtol(s,&tmp,16);
		if (tmp!=s+strlen(s)) htts_error("Invalid UttCelWS::fgrp (%s)",s);
		setFGrp(x);
	}

	NEXT();
	if (*s) {
		char *tmp;
		int x=(int)strtol(s,&tmp,16);
		if (tmp!=s+strlen(s)) htts_error("Invalid UttCelWS::agrp (%s)",s);
		setAGrp(x);
	}
	#ifdef HTTS_PROSO_VAL
	NEXT();//Aritz
	if (*s) 
	{
		char *tmp;
		int x=(int)strtol(s,&tmp,10);
		
		if (tmp!=s+strlen(s)/*|| x < MIN_VAL_BREAK  || x > MAX_VAL_BREAK*/) htts_error("Invalid UttCelWS::proso_val_break (%s)",s);
		else
		{
			
			setProso_val_break(x);
		}
	

		
	}
	NEXT();//Aritz
	if (*s) {
		char *tmp;
		int x=(int)strtol(s,&tmp,10);
		if (tmp!=s+strlen(s) || x < MIN_VAL_EMPHASIS  || x > MAX_VAL_EMPHASIS) htts_error("Invalid UttCelWS::proso_val_emphasis (%s)",s);
		setProso_val_emphasis(x);
		
	}
	NEXT();//Aritz
	if (*s) {
		char *tmp;
		int x=(int)strtol(s,&tmp,10);
		if (tmp!=s+strlen(s) || x < MIN_VAL_PITCH  || x > MAX_VAL_PITCH) htts_error("Invalid UttCelWS::proso_val_pitch (%s)",s);
		else
		{
			
			setProso_val_pitch(x);
		}
	}
	NEXT();//Aritz
	if (*s) {
		char *tmp;
		int x=(int)strtol(s,&tmp,10);
		if (tmp!=s+strlen(s) || x < MIN_VAL_PITCH  || x > MAX_VAL_PITCH) htts_error("Invalid UttCelWS::proso_val_range (%s)",s);
		setProso_val_range(x);
		
	}
	NEXT();//Aritz
	if (*s) {
		char *tmp;
		int x=(int)strtol(s,&tmp,10);
//		if (tmp!=s+strlen(s) || x < MIN_VAL_PITCH || x > MAX_VAL_PITCH) htts_error("Invalid UttCelWS::proso_val_rate (%s)",s);
		setProso_val_rate(x);
		
	}
	NEXT();//Aritz
	if (*s) {
		char *tmp;
		int x=(int)strtol(s,&tmp,10);
		if (tmp!=s+strlen(s) || x < MIN_VAL_PITCH  || x > MAX_VAL_PITCH) htts_error("Invalid UttCelWS::proso_val_volume (%s)",s);
		setProso_val_volume(x);
		
	}
	#endif

#ifdef HTTS_TIMEEVS
	NEXT();
	if (*s) setTimeEv(atoi(s));
#endif

	return next;
}
#endif

/**********************************************************/

#ifdef HTTS_IOTXT
BOOL UttCellWS::finput( FILE *f )
{
#define NREAD 200
	CHAR buf[NREAD];
	CHAR *wrk;
	BOOL err;
	int tl;

	reset();

	int ch=fgetc(f);
	if (ch==EOF) return FALSE;
	if (ch!='/') { ungetc(ch,f); return FALSE; }

	buf[0]='/';
	if (fgetln_filt(buf+1, NREAD-1, f,0,1,0,&tl)==NULL) return FALSE;
	if (tl) htts_error("UttCellWS::finput() - line too long [%s]",buf);

	wrk=strdup(buf+1);
	err= (__finput(wrk)==NULL);
	free(wrk);
	if (err) htts_error("Invalid %s (%s)",className(),buf);
	return TRUE;
}
#endif

/**********************************************************/

UttWS::UttWS(VOID)
{
	dicdb=NULL;
}

/**********************************************************/

UttCell *UttWS::newCell(VOID)
{
	return new UttCellWS;
}

/**********************************************************/

UttI UttWS::deleteCell(UttI p)
{
	UttI next = cellNext(p);

	if (next) {
		// traspasa frontera palabra (palabra) POS y wstatus si se debe.
		if (cell(p).getWord() && !cell(next).getWord()) {
			cell(next).setWord_ptr((CHAR *) (cell(p).getWord()));
			cell(p).setWord_ptr(NULL);	// para que no haga free
			cell(next).setPOS(cell(p).getPOS());
#ifdef UTT_DUMMY
			cell(next).setWStatus(cell(p).getWStatus());
#endif
		}
		// traspasa codigo sentencia si se debe.
		if ((cell(p).getSentence()!=USENTENCE_NONE) &&
				(cell(next).getSentence()==USENTENCE_NONE))
			cell(next).setSentence(cell(p).getSentence());

		// traspasa codigo pause si se debe.
		if ((cell(p).getPause()!=UPAUSE_NONE) &&
				(cell(next).getPause()==UPAUSE_NONE))
			cell(next).setPause(cell(p).getPause());

		// traspasa hdicref si se debe.
		if ((cell(p).getHDicRef()!=HDIC_REF_NULL) &&
				(cell(next).getHDicRef()==HDIC_REF_NULL))
			cell(next).setHDicRef(cell(p).getHDicRef());

		// traspasa frontera phrase (sintagma) si se puede
		if (cell(p).getPhrase() && !cell(next).getPhrase())
			cell(next).setPhrase(cell(p).getPhrase());

		// traspasa frontera fgrp si se puede
		if (cell(p).getFGrp() && !cell(next).getFGrp())
			cell(next).setFGrp(cell(p).getFGrp());

		// traspasa frontera agrp si se puede
		if (cell(p).getAGrp() && !cell(next).getAGrp())
			cell(next).setAGrp(cell(p).getAGrp());

		// traspasa frontera tnor si se puede
		if (cell(p).getTNor() && !cell(next).getTNor())
			cell(next).setTNor(cell(p).getTNor());

#ifdef HTTS_TIMEEVS
		// traspasa time ev. si se debe
		if ((cell(p).getTimeEv()) &&
				(cell(next).getTimeEv()==0))
			cell(next).setTimeEv(cell(p).getTimeEv());
#endif
	}

	return Utt::deleteCell(p); // encadena proceso a clase base
}

/**********************************************************/

#ifdef HTTS_TIMEEVS
VOID UttWS::expandTimeEvs( VOID )
{
	Lix p;
	INT curtev=0;

	for (p=cellFirst(); p!=0; p=cellNext(p))
		if (cell(p).getTimeEv()) curtev=cell(p).getTimeEv();
		else cell(p).setTimeEv(curtev);
}
#endif

/**********************************************************/

#ifdef HTTS_IOTXT
VOID UttWS::foutput( FILE *f ) const
{
	if (dicdb) UttCellWS::setGlobalHDicDB(dicdb);
	Utt::foutput(f);
}
#endif

/**********************************************************/

#ifdef HTTS_IOTXT
BOOL UttWS::finput( FILE *f )
{
	if (dicdb) UttCellWS::setGlobalHDicDB(dicdb);
	return Utt::finput(f);
}
#endif

/**********************************************************/
