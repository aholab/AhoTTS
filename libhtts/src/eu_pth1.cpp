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

Nombre fuente................ eu_pth1.cpp
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
Metodos de {{LangEU_Prosod}} que implementan diferentes modelos
de pitch.  Este es un modelo sencillo.
</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/



#include "spl.h"
#include "uti.h"
#include "eu_lingp.hpp"
#include "xx_uti.hpp"
#include "httsmsg.h"

#ifdef HTTS_PROSOD_EU_PTH1
/**********************************************************/
/* configuracion curva pitch utterance completa */

typedef struct {
	DOUBLE dmi,dmf;  // desviacion inicial y final sobre media 0
	DOUBLE di, df;  // desviacion de pico sobre la media, inicial y final
	DOUBLE fi, ff;  // posicion inicial(final) respecto a dmi(dmf) y normalizado a di(df)
	INT r;  // silabas en caer al valle (0=punto medio)
} UttCfg;

PRIVATE UttCfg utt_decl  = { +0.3,-0.3,  +0.6,+0.3,  +0.0,-0.9,  1 };
PRIVATE UttCfg utt_pause = { +0.3,-0.3,  +0.6,+0.3,  +0.0,+0.9,  0 };
PRIVATE UttCfg utt_quest = { +0.2,-0.2,  +0.9,+0.8,  -0.2,+1.3,  0 };
PRIVATE UttCfg utt_excl  = { +0.1,-0.1,  +0.8,+0.9,  +0.0,+0.0,  0 };
PRIVATE UttCfg utt_neutr = { +0.1,-0.1,  +0.8,+0.8,  +0.0,+0.0,  0 };

/**********************************************************/
/* configuracion modificacion pitch de cada sentence */
typedef struct {
	DOUBLE slantk; // correccion de caida media: 0=plano, 1=normal, etc.
	LONG npeaks1;  // n. picos quiebro (>0 por principio, <0 por final)
	DOUBLE shift1;  // desplazamiento vertical quiebro: 0 nada, 1=primer seg. horiz, -1=segundo seg. horiz)
	DOUBLE kdf, kdi;  // correccion desviacion de pico: 0=normal, -1/1=inicial/final
	DOUBLE ff, fi;  // posicion final/inicial (centrado en media final/inicial y normalizado a desviacion (df/di que toque, corregido con kdf,kdi)
/* nota: kdi y fi afectan a la sentencia *siguiente*! */
} SentenceCfg;

PRIVATE SentenceCfg sent_decl  = { 1.0, 1, +1.0, +0.1,-0.1, 0.0,0.0 };
PRIVATE SentenceCfg sent_pause = { 0.9, 1, +1.0, +0.1,-0.1, 0.5,0.0 };
PRIVATE SentenceCfg sent_quest = { 1.0, 2, +2.0, +0.3,-0.3, 1.3,0.0 };
PRIVATE SentenceCfg sent_excl  = { 1.1, 1, +2.0, +0.6,-0.6, 0.0,0.0 };
PRIVATE SentenceCfg sent_neutr = { 0.4, 0, +1.0, +0.1,-0.1, 0.0,0.0 };

/* Esta estructura se rellena en funcion de los parametros anteriores,
media y desviacion, y es la que se utiliza para generar la curva
de pitch en cada sentence */
typedef struct {
	DOUBLE m0, m1, m2; // valores medios
	DOUBLE d0, d2;    // desviacion de pico sobre valor medio
	DOUBLE f0, f2;   // posicion extremos (centrado en media, relativo a d0,d2
	LONG s1;        // posicion de media m1 (en silabas)
	LONG r;         // silabas en caer al fondo del valle
} Pth1CfgSent;

/**********************************************************/

PRIVATE VOID sentence_pth1(UttPh& ut, UttI sentp, Pth1CfgSent& prm);

/**********************************************************/

VOID LangEU_Prosod::utt_pth1( UttPh &ut )
{
	Pth1CfgSent prms;
	UttI last, first;
	UttCfg * cfg=0;
	LONG nsil;
	UttI p;

	for (p=ut.phoneFirst(); p!=0; p=ut.phoneNext(p))  // inicializamos a valor invalido
		ut.cell(p).setPth(FillValue);

	first=ut.sentenceFirst();
	last=ut.sentenceLast();
	nsil=ut.syllableN();
	if (!last) return;

	switch (ut.cell(last).getSentence()) {
		case USENTENCE_DECL: cfg = &utt_decl; break;
		case USENTENCE_EXCL: cfg = &utt_excl; break;
		case USENTENCE_QUEST: cfg = &utt_quest; break;
		case USENTENCE_PAUSE: cfg = &utt_pause; break;
		case USENTENCE_NEUTR: cfg = &utt_neutr; break;
		case USENTENCE_COLON: cfg = &utt_decl; break;
		case USENTENCE_SCOLON: cfg = &utt_decl; break;
		case USENTENCE_SUSPEN: cfg = &utt_decl; break;
		case USENTENCE_PAR: cfg = &utt_pause; break;
		case USENTENCE_PAROPEN: cfg = &utt_pause; break;
		default:htts_error("LangEU_Prosod: Unknown sentence type"); break;
	}

	SentenceCfg * scfg=0;
	SentenceCfg * oscfg=0;

	for (p=first; p!=0; p=ut.sentenceNext(p)) {
		LONG pos=ut.syllablePos(p);
		LONG snsil=ut.syllableN(p,URANGE_SENTENCE);

		//posicion media en utterance (inicial, intermedio, final)
		prms.m0=cfg->dmi;
		prms.m2=cfg->dmf;

		prms.s1=0;	// quiebro intermedio desactivado, luego activamos
		prms.m1=0;

		switch (ut.cell(p).getSentence()) {
			case USENTENCE_DECL: scfg = &sent_decl; break;
			case USENTENCE_EXCL: scfg = &sent_excl; break;
			case USENTENCE_QUEST: scfg = &sent_quest; break;
			case USENTENCE_PAUSE: scfg = &sent_pause; break;
			case USENTENCE_NEUTR: scfg = &sent_neutr; break;
			case USENTENCE_COLON: scfg = &sent_decl; break;
			case USENTENCE_SCOLON: scfg = &sent_decl; break;
			case USENTENCE_SUSPEN: scfg = &sent_decl; break;
			case USENTENCE_PAR: scfg = &sent_pause; break;
			case USENTENCE_PAROPEN: scfg = &sent_pause; break;
			default:htts_error("LangEU_Prosod: Unknown sentence type"); break;
		}
		if (scfg->npeaks1) {  // quiebro intermedio
			UttI q;
			if (scfg->npeaks1>0) q=ut.emphasisGotoPos(scfg->npeaks1-1,p,URANGE_SENTENCE);
			else q=ut.emphasisGotoPos(ut.emphasisN(p,URANGE_SENTENCE)-scfg->npeaks1,p,URANGE_SENTENCE);
			if (q) prms.s1=ut.syllablePos(q,URANGE_SENTENCE);
		}

		// desviacion sobre la media (inicial y final)
		prms.d0=linterpol(pos+1,1,nsil,cfg->di,cfg->df);
		prms.d2=linterpol(pos+snsil,1,nsil,cfg->di,cfg->df);
		prms.f0=cfg->fi;
		prms.f2=cfg->ff;
		prms.r=cfg->r;

		if (p!=last) {  // entonacion final de pausa intermedia
			if (scfg->kdf<0) prms.d2=linterpol(scfg->kdf,-1,0,cfg->di,prms.d2);
			else prms.d2=linterpol(scfg->kdf,0,1,prms.d2,cfg->df);
			prms.f2=scfg->ff;
		}
		if (p!=first) {  // entonacion inicial tras pausa intermedia
			if (oscfg->kdi<0) prms.d0=linterpol(oscfg->kdi,-1,0,cfg->di,prms.d0);
			else prms.d0=linterpol(oscfg->kdi,0,1,prms.d0,cfg->df);
			prms.f0=oscfg->fi;
		}

		prms.m2=prms.m0+scfg->slantk*(prms.m2-prms.m0);

		prms.m1=linterpol(prms.s1+1,1,snsil,prms.m0,prms.m2);
		if (prms.s1) { // corregir posicion quiebro intermedio si lo hay
			if (scfg->shift1>0)
				prms.m1=linterpol(scfg->shift1,0,+1,prms.m1,prms.m0);
			else
				prms.m1=linterpol(scfg->shift1,0,-1,prms.m1,prms.m2);
		}

		sentence_pth1(ut,p,prms);
		oscfg=scfg;
	}

	uttPthLin(ut);  // interpolacion lineal entre los puntos ya definidos.

	/* pasar de db2 a lineal el pitch, aplicando media {pth_mean} y varianza {pth_dev} */
	DOUBLE pthd=lin2log2(pth_dev+1.0);
	for (p=ut.phoneFirst(); p!=0; p=ut.phoneNext(p))
		ut.cell(p).setPth(pth_mean*log22lin(ut.cell(p).getPth()*pthd));
}

/**********************************************************/

PRIVATE VOID sentence_pth1( UttPh & ut, UttI sentp, Pth1CfgSent & prm )
{
#define MEAN(s) ((scorner>=0)? \
	((s<=scorner)? \
		linterpol(s,0,scorner,prm.m0,prm.m1): \
		linterpol(s,scorner,nsil-1,prm.m1,prm.m2)): \
	linterpol(s,0,nsil-1,prm.m0,prm.m2))

#define DESV(s) linterpol(s,0,nsil-1,prm.d0,prm.d2)

	LONG nsil = ut.syllableN(sentp,URANGE_SENTENCE);	// numero de silabas
	if (!nsil) return;  // si no hay silabas, nada que hacer

	UttI first = ut.emphasisFirst(sentp,URANGE_SENTENCE);  // primer pico
	UttI last = ut.emphasisLast(sentp,URANGE_SENTENCE);  // ultimo pico
	UttI begin = ut.syllableFirst(sentp,URANGE_SENTENCE);	 // ultima silaba
	UttI end = ut.syllableLast(sentp,URANGE_SENTENCE);	// ultima silaba
	LONG sfirst = ut.syllablePos(first,URANGE_SENTENCE);
	LONG slast = ut.syllablePos(last,URANGE_SENTENCE);
	LONG sbegin = ut.syllablePos(begin,URANGE_SENTENCE);
	LONG send = ut.syllablePos(end,URANGE_SENTENCE);
	UttI p;

	LONG scorner = prm.s1;  // silaba de quiebro (m1)
	if ((scorner<=0) || (scorner>=nsil-1)) // fuera de rango...
		scorner = -1; // ...desactivar quiebro

	/* pitch para cada pico de la sentence */
	for (p = first; p != 0; p = ut.emphasisNext(p,URANGE_SENTENCE)) {
		LONG s = ut.syllablePos(p,URANGE_SENTENCE);  // num. silaba pico
		ut.cell(LangEU_PhUti::syllable_xvowel(ut,p)).setPth(MEAN(s)+DESV(s));
	}

	/* ponemos el valor inicial (cabeza) */
	UttI vbegin = LangEU_PhUti::syllable_xvowel(ut,begin);  // vocal en la silaba
	if (!first || (sfirst>sbegin))  // hay silabas antes del primer pico
		ut.cell(vbegin).setPth(prm.m0+prm.f0*prm.d0);  // valor inicial
	else  // primera silaba == primer pico => media de pico y valor inicial
		ut.cell(vbegin).setPth(0.5*(ut.cell(vbegin).getPth()+prm.m0+prm.f0*prm.d0));

	/* ponemos el valor final (cola) */
	UttI vend = LangEU_PhUti::syllable_xvowel(ut,end);  // vocal en la silaba
	if (!last || (slast<send))  // hay silabas despues del ultimo pico
		ut.cell(vend).setPth(prm.m2+prm.f2*prm.d2);  // valor final
	else  // ultima silaba == ultimo pico => media de pico y valor final
		ut.cell(vend).setPth(0.5*(ut.cell(vend).getPth()+prm.m2+prm.f2*prm.d2));

	/* pitch en cada valle (entre 2 picos) */
	for (p = first; p!= last; ) {
		UttI next = ut.emphasisNext(p,URANGE_SENTENCE);
		LONG s = ut.syllablePos(p,URANGE_SENTENCE);
		LONG snext = ut.syllablePos(next,URANGE_SENTENCE);
		if (snext-s>1) {  // solo si hay alguna silaba intermedia
			if (prm.r) {  // caida a valor medio en {prm.r} silabas
				LONG s1=s+prm.r;
				LONG s2=snext-prm.r;
				DOUBLE p1=MEAN(s1);
				DOUBLE p2=MEAN(s2);
				if (s1<=s2) {
					UttI pv1 = ut.syllableGotoPos(s1,p,URANGE_SENTENCE);
					UttI pv2 = ut.syllableGotoPos(s2,p,URANGE_SENTENCE);
#ifdef HTTS_DEBUG
					if (!pv1||!pv2) { show_srcpos(); htts_error("error going to valley!"); }
#endif
					if (pv1) ut.cell(LangEU_PhUti::syllable_xvowel(ut,pv1)).setPth(p1);
					if (pv2) ut.cell(LangEU_PhUti::syllable_xvowel(ut,pv2)).setPth(p2);
				}
				else {
					LONG ss=(s1+s2+1)/2;
					UttI pss = ut.syllableGotoPos(ss,p,URANGE_SENTENCE);
#ifdef HTTS_DEBUG
					if (!pss) { show_srcpos(); htts_error("error going to valley!"); }
#endif
					DOUBLE pp1=linterpol(ss,s,s1,ut.cell(LangEU_PhUti::syllable_xvowel(ut,p)).getPth(),p1);
					DOUBLE pp2=linterpol(ss,s2,snext,p2,ut.cell(LangEU_PhUti::syllable_xvowel(ut,next)).getPth());
					if (pp1<pp2) pp1=pp2;
					if (pss) ut.cell(LangEU_PhUti::syllable_xvowel(ut,pss)).setPth(pp1);
				}
			}
			else {  // caida a MEAN-DESV en el centro
				LONG sv = (s + snext + 1) / 2;		// n. silaba del valle
				UttI pv = ut.syllableGotoPos(sv,p,URANGE_SENTENCE);	// lindice de la silaba valle
#ifdef HTTS_DEBUG
				if (!pv) { show_srcpos(); htts_error("error going to valley!"); }
#endif
				if (pv) ut.cell(LangEU_PhUti::syllable_xvowel(ut,pv)).setPth(MEAN(sv)-DESV(sv));
			}
		}  // fin silaba intermedia
		p=next;  // siguiente pico
	}
}

/**********************************************************/

#endif
