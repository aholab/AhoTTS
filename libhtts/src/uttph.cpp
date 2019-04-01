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

Nombre fuente................ uttph.cpp
Nombre paquete............... aHoTTS
Lenguaje fuente.............. C++
Estado....................... -
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Borja Etxebarria
.............................

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
1.0.0    31/01/00  borja     codefreeze aHoTTS v1.0
0.0.0    24/11/97  borja     Codificacion inicial.

======================== Contenido ========================
<DOC>
</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include "uti.h"
#include "uttph.hpp"
#include "httsmsg.h"
#include "strl.hpp"

/**********************************************************/


KINDOF_DDEF(UttCellPh, UttCellWS)
KINDOF_DDEF(UttPh, UttWS)

/**********************************************************/

VOID UttCellPh::reset(VOID)
{
	UttCellWS::reset();
	ch = '\0';
	ph = PH_none;
	stress = USTRESS_NONE;
	emph = UEMPHASIS_NONE;
	syllable = FALSE;
	group1 = FALSE;
	group2 = FALSE;
	dur = pow = 0;

#ifdef HTTS_MULTIPITCH
	if (pth) free(pth);
	npth = 1;
	pth = (DOUBLE*)malloc(2*sizeof(DOUBLE));
	pth[0]=0; pth[1]=0;
#else
	pth = 0;
#endif
}

/**********************************************************/

UttCellPh::UttCellPh(VOID)
{
#ifdef HTTS_MULTIPITCH
	pth = NULL;
#endif

	reset();
}

/**********************************************************/

UttCellPh::~UttCellPh(VOID)
{
#ifdef HTTS_MULTIPITCH
	if (pth) { free(pth); pth=NULL; }
#endif
}

/**********************************************************/

BOOL UttCellPh::isStartOf(UttLevel range) const
{
	// manejamos las fronteras conocidas
	if (inURange(ULEVEL_PHONE, range) && (ph != PH_none)) return TRUE;
	if (inURange(ULEVEL_CHAR, range) && ch) return TRUE;
	if (inURange(ULEVEL_SYLLABLE, range) && syllable) return TRUE;
	if (inURange(ULEVEL_EMPHASIS, range) && emph != UEMPHASIS_NONE) return TRUE;

	// resto de niveles en clase base (ver nota1)
#ifdef HTTS_DEBUG
	return UttCellWS::isStartOf(delURange(addURange(ULEVEL_PHONE |
				ULEVEL_SYLLABLE | ULEVEL_CHAR | ULEVEL_EMPHASIS), range));
#else
	return UttCellWS::isStartOf(range);
#endif

/*nota1: realmente no hace falta el delURange, pero es que la clase
   base si encuentra bits desconocidos por ella a ON da un error. Es 
	 un debugger controlado por HTTS_DEBUG. Si se quita, ya no hace
   falta. */
}

/**********************************************************/

VOID UttCellPh::setStress(Stress code)
{
	stress = code;
#ifdef HTTS_DEBUG
	if (stress!=code) htts_error("UttCellPh::stress field too short");
#endif
}

/**********************************************************/

VOID UttCellPh::setEmphasis(Emphasis code)
{
	emph = code;
#ifdef HTTS_DEBUG
	if (emph!=code) htts_error("UttCellPh::emphasis field too short");
#endif
}

/**********************************************************/

#ifdef HTTS_MULTIPITCH
VOID UttCellPh::set_PthN( INT n )
{
	assert(n);
	if (pth) { free(pth); pth=NULL; }
	npth=n;
	if (npth) pth=(DOUBLE*)malloc(2*n*sizeof(DOUBLE));
}
#endif

/**********************************************************/

#ifdef HTTS_IOTXT
VOID UttCellPh::__foutput( FILE *f ) const
{
	UttCellWS::__foutput(f);

	fprintf(f,"%s/", getSyllable()?"s":" ");

	switch (getStress()) {
	case USTRESS_NONE:fprintf(f," /"); break;
	case USTRESS_TEXT:fprintf(f,"'/"); break;
	case USTRESS_AUTO:fprintf(f,"`/"); break;
	default:htts_error("Invalid stress type (%d)",(INT)getStress());
	}

	switch (getEmphasis()) {
	case UEMPHASIS_NONE:fprintf(f," /"); break;
	case UEMPHASIS_STRESS:fprintf(f,"^/"); break;
	case UEMPHASIS_STRONG:fprintf(f,"*/"); break;
	default:htts_error("Invalid emphasis type (%d)",(INT)getEmphasis());
	}

	CHAR ch=getChar();
	fprintf(f,"%c/",ch ? ch : ' ');
	fprintf(f,"%s/",phone_tosampa(getPhone()));

#ifdef HTTS_MULTIPITCH
	if ((getPhone()==PH_none)&&(getPth()==0)) fprintf(f,"%4s %6s","","");
	else for (INT i=0; i<get_PthN(); i++)
		fprintf(f,"%s%4.2f,%6.2f",i?", ":"",(double)get_PthX(i),(double)get_Pth(i));
	fprintf(f,"/");
#else
	if ((getPhone()==PH_none)&&(getPth()==0)) fprintf(f,"%6s/","");
	else fprintf(f,"%6.2f/",(double)getPth());
#endif

	if ((getPhone()==PH_none)&&(getDur()==0)) fprintf(f,"%6s/","");
	else fprintf(f,"%6.2f/",(double)getDur());

	if ((getPhone()==PH_none)&&(getPow()==0)) fprintf(f,"%6s/","");
	else fprintf(f,"%6.2f/",(double)getPow());
}
#endif

/**********************************************************/

#ifdef HTTS_IOTXT
CHAR *UttCellPh::__finput( CHAR *s )
{
#define NEXT() s=next; next=strchr(s,'/'); if (!next) return s; *next='\0'; next++;
	CHAR *next;

	next=UttCellWS::__finput(s);
	if (!next) return NULL;

	NEXT(); if (*s=='s') setSyllable(TRUE);

	NEXT();
	switch (*s) {
	case '\0':
	case ' ': setStress(USTRESS_NONE); break;
	case '\'':setStress(USTRESS_TEXT); break;
	case '`': setStress(USTRESS_AUTO); break;
	default: return NULL;
	}

	NEXT();
	switch (*s) {
	case '\0':
	case ' ': setEmphasis(UEMPHASIS_NONE); break;
	case '^': setEmphasis(UEMPHASIS_STRESS); break;
	case '*': setEmphasis(UEMPHASIS_STRONG); break;
	default: return NULL;
	}

	NEXT(); if (*s!=' ') setChar(*s);

	NEXT(); if (*s) setPhone(phone_fromsampa(s));

#ifdef HTTS_MULTIPITCH
	NEXT();
	if (*s) {
		char *ss=s;
		while ((ss=strchr(ss,','))!=NULL) { *ss=' '; }
		StrList l(s);

		if (l.length()==1) { DOUBLE d; if (str2d(s,&d)) return NULL; setPth(d); }
		else {
			set_PthN((INT)(l.length()/2));
			for (INT i=0; i<get_PthN(); i++) {
				DOUBLE d;
				if (str2d(l.item(i*2),&d)) return NULL; set_PthX(i,d);
				if (str2d(l.item(i*2+1),&d)) return NULL; set_Pth(i,d);
			}
		}
	}
#else
	NEXT(); if (*s) { DOUBLE d; if (str2d(s,&d)) return NULL; setPth(d); };
#endif

	NEXT(); if (*s) { DOUBLE d; if (str2d(s,&d)) return NULL; setDur(d); };

	NEXT(); if (*s) { DOUBLE d; if (str2d(s,&d)) return NULL; setPow(d); };

	return next;
}
#endif

/**********************************************************/

UttCell *UttPh::newCell(VOID)
{
	return new UttCellPh;
}

/**********************************************************/

UttI UttPh::deleteCell(UttI p)
{
	UttI next = cellNext(p);

	if (next) {
		// traspasa fronteras si se puede
		if (cell(p).getSyllable() && !cell(next).getSyllable())
			cell(next).setSyllable(cell(p).getSyllable());

		if (cell(p).getGroup1() && !cell(next).getGroup1())
			cell(next).setGroup1(cell(p).getGroup1());

		if (cell(p).getGroup2() && !cell(next).getGroup2())
			cell(next).setGroup2(cell(p).getGroup2());
	}

	return UttWS::deleteCell(p);
					// encadena a clase base

	// nota: el caracter, fonema, y acento, no se transpasan!
}

/**********************************************************/
/* expande una palabra. Se supone que no esta expandida,
y que logicamente wordp es el principio de la palabra.
Si no, malo.*/

VOID UttPh::word2Char(UttI wordp)
{
	const CHAR *w;

	w = cell(wordp).getWord();
	if (!w) return;

	cell(wordp).setChar(*w);
	while (*w) {
		if (!(*(++w))) break;
		wordp = cellAddAfter(wordp);
		cell(wordp).setChar(*w);
	}
}

/**********************************************************/
