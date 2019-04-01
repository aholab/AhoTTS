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

Nombre fuente................ eu_dur2.cpp
Nombre paquete............... aHoTTS
Lenguaje fuente.............. C++
Estado....................... -
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Borja Etxebarria
............................. Ana Armenta

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
1.0.0    31/01/00  borja     codefreeze aHoTTS v1.0
0.0.0    24/11/97  borja     Codificacion inicial.

======================== Contenido ========================
<DOC>
Metodos de {{LangEU_Prosod}} que implementan diferentes
modelos de duracion. Este es un modelo estadistico en
base a CART.
</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include <math.h>
#include "phone.h"
#include "eu_lingp.hpp"
#include "xx_uti.hpp"
#include "httsmsg.h"

#ifdef HTTS_PROSOD_EU_DUR2
/**********************************************************/
/*modelo de duraci¢n*/

typedef struct {  // iformaci¢n que se le suministra al cart
//ver si realmente necesito enteros ¢ me vale con booleanos
//o alguna forma de simplificar esta estructura

	INT numfon;//n£mero que caracteriza al fonema
	INT vc;    //vocal ¢ consonante
	INT vhgt; //altura de la vocal
	INT vfrt; //frontness
	INT vrnd; //roundness
	INT ct;    //tipo de consonante
	INT art;   //lugar de articulaci¢n
	INT s_n;   //sorda ¢ sonora
	INT sigpau; //despu‚s hay pausa?
	INT begend; //es ppio o fin de frase.
	INT npospal;     //posici¢n en la palabra
	INT npossil;   //posici¢n en la s¡laba
	INT ultimapal;
	INT ultimasil;
	INT npalsen;
	INT nsilsen;
	INT npalutt;
	INT nsilutt;
	INT str;		//acento del sonido


	INT n_numfon;//n£mero que caracteriza al fonema
	INT n_vc;    //vocal ¢ consonante
	INT n_vhgt; //altura de la vocal
	INT n_vfrt; //frontness
	INT n_vrnd; //roundness
	INT n_ct;    //tipo de consonante
	INT n_art;   //lugar de articulaci¢n
	INT n_s_n;   //sorda ¢ sonora
	INT n_sigpau; //despu‚s hay pausa?
	INT n_begend; //es ppio o fin de frase.

//****************************************************

	INT nn_numfon;//n£mero que caracteriza al fonema
	INT nn_vc;    //vocal ¢ consonante
	INT nn_vhgt; //altura de la vocal
	INT nn_vfrt; //frontness
	INT nn_vrnd; //roundness
	INT nn_ct;    //tipo de consonante
	INT nn_art;   //lugar de articulaci¢n
	INT nn_s_n;   //sorda ¢ sonora
	INT nn_sigpau; //despu‚s hay pausa?
	INT nn_begend; //es ppio o fin de frase.

//****************************************************

	INT p_numfon;//n£mero que caracteriza al fonema
	INT p_vc;    //vocal ¢ consonante
	INT p_vhgt; //altura de la vocal
	INT p_vfrt; //frontness
	INT p_vrnd; //roundness
	INT p_ct;    //tipo de consonante
	INT p_art;   //lugar de articulaci¢n
	INT p_s_n;   //sorda ¢ sonora
	INT p_sigpau; //despu‚s hay pausa?
	INT p_begend; //es ppio o fin de frase.

//****************************************************

	INT pp_numfon;//n£mero que caracteriza al fonema
	INT pp_vc;    //vocal ¢ consonante
	INT pp_vhgt; //altura de la vocal
	INT pp_vfrt; //frontness
	INT pp_vrnd; //roundness
	INT pp_ct;    //tipo de consonante
	INT pp_art;   //lugar de articulaci¢n
	INT pp_s_n;   //sorda ¢ sonora
	INT pp_sigpau; //despu‚s hay pausa?
	INT pp_begend; //es ppio o fin de frase.

//****************************************************


}CartCfg;

typedef INT* CartFeats;

PRIVATE CartFeats Feats(Phone ph);
PRIVATE VOID Utt_Cart_Cfg(UttPh &ut,CartCfg &ccfg,UttI p);
PRIVATE DOUBLE Cart_Dur(CartCfg cartcfg);
PRIVATE INT Nrel_Pal(UttPh &ut,UttI p);
PRIVATE INT Nrel_Sil(UttPh &ut,UttI p);
PRIVATE INT Stress(UttPh &ut,UttI p);

/* asignacion duraciones definitivas (en milisegundos). */
PRIVATE VOID utt_dur_unnorm(UttPh & ut)
{
	UttI p;
	DOUBLE d=0;
	DOUBLE aux=0;
	CartCfg ccfg={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	DOUBLE datos_f[][2]={	{229.137221,133.828502},
												{70.248063,21.001539},
												{63.539889,22.279934},
												{67.435684,24.777966},
												{72.022443,24.691409},
												{69.735279,24.617134},
												{97.978745,28.646198},
												{77.944932,39.245416},
												{105.806223,23.096225},
												{51.287230,22.147693},
												{93.778523,25.464593},
												{74.748634,40.754888},
												{122.848201,31.334981},
												{76.535220,18.176976},
												{71.001005,20.375406},
												{88.509857,22.993946},
												{153.987216,25.738410},
												{149.456928,28.952119},
												{143.068807,24.054341},
												{136.449612,28.976724},
												{103.031137,33.870058},
												{110.571153,32.671685},
												{83.682145,21.643905},
												{72.045905,35.412627},
												{63.422185,20.070097},
												{110.876579,27.694032},
												{59.110162,17.911968},
												{126.344828,23.986743},
												{136.897059,27.381370},
												{65.624588,23.004537},
												{67.740748,21.414369},
												{74.098812,24.086442},
												{38.394793,11.136886},
												{58.101275,21.748189},
												{0,0},
												{0,0},
												{0,0},
												{0,0},
	};


	for (p=ut.phoneFirst();p!=0;p=ut.phoneNext(p)){
		Utt_Cart_Cfg(ut,ccfg,p);//obtengo la informaci¢n necesaria

#ifdef NOTDEFINED      // debug
		fprintf(stderr,"0 %d 1 %d 2 %d 3 %d 4 %d 5 %d 6 %d 7 %d 8 %d 9 %d\n",ccfg.pp_vc,ccfg.pp_vhgt,ccfg.pp_vfrt,ccfg.pp_vrnd,ccfg.pp_ct,ccfg.pp_art,ccfg.pp_s_n,ccfg.pp_sigpau,ccfg.pp_begend,ccfg.pp_numfon);
		fprintf(stderr,"0 %d 1 %d 2 %d 3 %d 4 %d 5 %d 6 %d 7 %d 8 %d 9 %d\n",ccfg.p_vc,ccfg.p_vhgt,ccfg.p_vfrt,ccfg.p_vrnd,ccfg.p_ct,ccfg.p_art,ccfg.p_s_n,ccfg.p_sigpau,ccfg.p_begend,ccfg.p_numfon);
		fprintf(stderr,"0 %d 1 %d 2 %d 3 %d 4 %d 5 %d 6 %d 7 %d 8 %d 9 %d\n",ccfg.vc,ccfg.vhgt,ccfg.vfrt,ccfg.vrnd,ccfg.ct,ccfg.art,ccfg.s_n,ccfg.sigpau,ccfg.begend,ccfg.numfon);
		fprintf(stderr,"0 %d 1 %d 2 %d 3 %d 4 %d 5 %d 6 %d 7 %d 8 %d 9 %d\n",ccfg.n_vc,ccfg.n_vhgt,ccfg.n_vfrt,ccfg.n_vrnd,ccfg.n_ct,ccfg.n_art,ccfg.n_s_n,ccfg.n_sigpau,ccfg.n_begend,ccfg.n_numfon);
		fprintf(stderr,"0 %d 1 %d 2 %d 3 %d 4 %d 5 %d 6 %d 7 %d 8 %d 9 %d\n",ccfg.nn_vc,ccfg.nn_vhgt,ccfg.nn_vfrt,ccfg.nn_vrnd,ccfg.nn_ct,ccfg.nn_art,ccfg.nn_s_n,ccfg.nn_sigpau,ccfg.nn_begend,ccfg.nn_numfon);
#endif

		d=Cart_Dur(ccfg);//asignamos la duraci¢n de cada fonema
//Realmente los arboles predicen la z-score, no la duracion, luego tenemos que
//deshacer la cuenta
		aux=d*datos_f[ccfg.numfon][1]+datos_f[ccfg.numfon][0];
		d=aux;

		if((ut.cell(p).getPhone()==PH_sil)&&(ut.cell(p).getPause()==UPAUSE_SHORT))
			d=100;

//Los del ABATEUS quieren una pausa final larga para que les de tiempo a
//cerra la boca!!!
//		if((ut.cell(p).getPhone()==PH_sil)&&(ut.cell(p).getPause()==UPAUSE_UEND))
//			d=500;

#ifdef NOTDEFINED       //debug
		fprintf(stderr,"%d %d %d %d %d %d\n",ccfg.numfon,ccfg.vc,ccfg.vhgt,ccfg.vfrt,ccfg.vrnd,ccfg.ct);
		fprintf(stderr,"fonema que dura: %f\n",d);
		fprintf(stderr,"ccfg.numfon: %d\n",ccfg.numfon);
#endif

		ut.cell(p).setDur(d*ut.cell(p).getDur());
	}
}
/*********************************************************/
PRIVATE VOID Utt_Cart_Cfg(UttPh &ut,CartCfg &ccfg,UttI p){
//rellena la estructura ccfg
	Phone ph;
	UttI next,prev,next_next,prev_prev;
	INT initial[]={0,0,0,0,0,0,0,0,0,0,0};
	INT *arrai=initial;

	if(ut.wordIsLast(ut.wordThis(p),URANGE_UTT))
		ccfg.ultimapal=1;
	if(ut.syllableIsLast(ut.syllableThis(p),URANGE_UTT))
		ccfg.ultimasil=1;

	if(ut.sentencePrev(ut.sentenceThis(p))){
		ccfg.nsilsen=ut.syllableN(p,URANGE_SENTENCE)-ut.syllableN(ut.sentencePrev(ut.sentenceThis(p)),URANGE_SENTENCE);
		ccfg.npalsen=ut.wordN(p,URANGE_SENTENCE)-ut.wordN(ut.sentencePrev(ut.sentenceThis(p)),URANGE_SENTENCE);
	}
	else {
		ccfg.nsilsen=ut.syllableN(p,URANGE_SENTENCE);
		ccfg.npalsen=ut.wordN(p,URANGE_SENTENCE);
	}
	ccfg.nsilutt=ut.syllableN(p,URANGE_UTT);
	ccfg.npalutt=ut.wordN(p,URANGE_UTT);

	//INT i;
	ph=ut.cell(p).getPhone();
	arrai=Feats(ph);
	ccfg.vc=arrai[0];
	ccfg.vhgt=arrai[1];
	ccfg.vfrt=arrai[2];
	ccfg.vrnd=arrai[3];
	ccfg.ct=arrai[4];
	ccfg.art=arrai[5];
	ccfg.s_n=arrai[6];
	ccfg.sigpau=arrai[7];
	ccfg.begend=arrai[8];
	ccfg.numfon=arrai[9];
	ccfg.npospal=Nrel_Pal(ut,p);     //posici¢n en la palabra
	ccfg.npossil=Nrel_Sil(ut,p);   //posici¢n en la silaba
	ccfg.str=Stress(ut,p);
#ifdef NOTDEFINED  	//debug
	fprintf(stderr,"actual:");
	for(i=0;i<11;i++){fprintf(stderr," %d %d",i,arrai[i]);}
	fprintf(stderr,"\n");
	fprintf(stderr,"0 %d 1 %d 2 %d 3 %d 4 %d 5 %d 6 %d 7 %d 8 %d 9 %d\n",ccfg.vc,ccfg.vhgt,ccfg.vfrt,ccfg.vrnd,ccfg.ct,ccfg.art,ccfg.s_n,ccfg.sigpau,ccfg.begend,ccfg.numfon);
#endif
	//resto de los campos....
	next=ut.phoneNext(p);
	if(next!=0){
		ph=ut.cell(next).getPhone();
		arrai=Feats(ph);
		ccfg.n_vc=arrai[0];
		ccfg.n_vhgt=arrai[1];
		ccfg.n_vfrt=arrai[2];
		ccfg.n_vrnd=arrai[3];
		ccfg.n_ct=arrai[4];
		ccfg.n_art=arrai[5];
		ccfg.n_s_n=arrai[6];
		ccfg.n_sigpau=arrai[7];
		ccfg.n_begend=arrai[8];
		ccfg.n_numfon=arrai[9];
#ifdef NOTDEFINED  //debug
	fprintf(stderr,"siguiente:");
	for(i=0;i<11;i++){fprintf(stderr," %d %d",i,arrai[i]);}
	fprintf(stderr,"\n");
	fprintf(stderr,"0 %d 1 %d 2 %d 3 %d 4 %d 5 %d 6 %d 7 %d 8 %d 9 %d\n",ccfg.n_vc,ccfg.n_vhgt,ccfg.n_vfrt,ccfg.n_vrnd,ccfg.n_ct,ccfg.n_art,ccfg.n_s_n,ccfg.n_sigpau,ccfg.n_begend,ccfg.n_numfon);
#endif
	}
	else{
		ccfg.n_vc=0;
		ccfg.n_vhgt=0;
		ccfg.n_vfrt=0;
		ccfg.n_vrnd=0;
		ccfg.n_ct=0;
		ccfg.n_art=0;
		ccfg.n_s_n=0;
		ccfg.n_sigpau=0;
		ccfg.n_begend=0;
		ccfg.n_numfon=0;
	}
// fonema nnext next
	next_next=ut.phoneNext(next);
	if(next_next!=0){
		ph=ut.cell(next_next).getPhone();
		arrai=Feats(ph);
		ccfg.nn_vc=arrai[0];
		ccfg.nn_vhgt=arrai[1];
		ccfg.nn_vfrt=arrai[2];
		ccfg.nn_vrnd=arrai[3];
		ccfg.nn_ct=arrai[4];
		ccfg.nn_art=arrai[5];
		ccfg.nn_s_n=arrai[6];
		ccfg.nn_sigpau=arrai[7];
		ccfg.nn_begend=arrai[8];
		ccfg.nn_numfon=arrai[9];
#ifdef NOTDEFINED  //debug
	fprintf(stderr,"siguiente:");
	for(i=0;i<11;i++){fprintf(stderr," %d %d",i,arrai[i]);}
	fprintf(stderr,"\n");
	fprintf(stderr,"0 %d 1 %d 2 %d 3 %d 4 %d 5 %d 6 %d 7 %d 8 %d 9 %d\n",ccfg.n_vc,ccfg.n_vhgt,ccfg.n_vfrt,ccfg.n_vrnd,ccfg.n_ct,ccfg.n_art,ccfg.n_s_n,ccfg.n_sigpau,ccfg.n_begend,ccfg.n_numfon);
#endif
	}
	else{
		ccfg.nn_vc=0;
		ccfg.nn_vhgt=0;
		ccfg.nn_vfrt=0;
		ccfg.nn_vrnd=0;
		ccfg.nn_ct=0;
		ccfg.nn_art=0;
		ccfg.nn_s_n=0;
		ccfg.nn_sigpau=0;
		ccfg.nn_begend=0;
		ccfg.nn_numfon=0;
	}

//.....

	prev=ut.phonePrev(p);
	if(prev!=0){
		ph=ut.cell(prev).getPhone();
		arrai=Feats(ph);
		ccfg.p_vc=arrai[0];
		ccfg.p_vhgt=arrai[1];
		ccfg.p_vfrt=arrai[2];
		ccfg.p_vrnd=arrai[3];
		ccfg.p_ct=arrai[4];
		ccfg.p_art=arrai[5];
		ccfg.p_s_n=arrai[6];
		ccfg.p_sigpau=arrai[7];
		ccfg.p_begend=arrai[8];
		ccfg.p_numfon=arrai[9];
#ifdef NOTDEFINED  //debug
	fprintf(stderr,"anterior:");
	for(i=0;i<11;i++) fprintf(stderr," %d %d",i,arrai[i]);
	fprintf(stderr,"\n");
	fprintf(stderr,"0 %d 1 %d 2 %d 3 %d 4 %d 5 %d 6 %d 7 %d 8 %d 9 %d\n",ccfg.p_vc,ccfg.p_vhgt,ccfg.p_vfrt,ccfg.p_vrnd,ccfg.p_ct,ccfg.p_art,ccfg.p_s_n,ccfg.p_sigpau,ccfg.p_begend,ccfg.p_numfon);
#endif
	}
	else{
		ccfg.p_vc=0;
		ccfg.p_vhgt=0;
		ccfg.p_vfrt=0;
		ccfg.p_vrnd=0;
		ccfg.p_ct=0;
		ccfg.p_art=0;
		ccfg.p_s_n=0;
		ccfg.p_sigpau=0;
		ccfg.p_begend=0;
		ccfg.p_numfon=0;
	}
// fonema prev prev
	prev_prev=ut.phonePrev(prev);
	if(prev_prev!=0){
		ph=ut.cell(prev_prev).getPhone();
		arrai=Feats(ph);
		ccfg.pp_vc=arrai[0];
		ccfg.pp_vhgt=arrai[1];
		ccfg.pp_vfrt=arrai[2];
		ccfg.pp_vrnd=arrai[3];
		ccfg.pp_ct=arrai[4];
		ccfg.pp_art=arrai[5];
		ccfg.pp_s_n=arrai[6];
		ccfg.pp_sigpau=arrai[7];
		ccfg.pp_begend=arrai[8];
		ccfg.pp_numfon=arrai[9];
#ifdef NOTDEFINED  //debug
	fprintf(stderr,"siguiente:");
	for(i=0;i<11;i++){fprintf(stderr," %d %d",i,arrai[i]);}
	fprintf(stderr,"\n");
	fprintf(stderr,"0 %d 1 %d 2 %d 3 %d 4 %d 5 %d 6 %d 7 %d 8 %d 9 %d\n",ccfg.n_vc,ccfg.n_vhgt,ccfg.n_vfrt,ccfg.n_vrnd,ccfg.n_ct,ccfg.n_art,ccfg.n_s_n,ccfg.n_sigpau,ccfg.n_begend,ccfg.n_numfon);
#endif
	}
	else{
		ccfg.pp_vc=0;
		ccfg.pp_vhgt=0;
		ccfg.pp_vfrt=0;
		ccfg.pp_vrnd=0;
		ccfg.pp_ct=0;
		ccfg.pp_art=0;
		ccfg.pp_s_n=0;
		ccfg.pp_sigpau=0;
		ccfg.pp_begend=0;
		ccfg.pp_numfon=0;
	}

	//....
}

PRIVATE INT * Feats(Phone ph)
{
	STATIC 	INT ph_default[10]={0,0,0,0,0,0,0,0,0,0};
	STATIC 	INT ph_sil[10]={0,0,0,0,0,0,0,0,0,0};
	STATIC 	INT ph_a[10]={1,1,2,2,0,0,1,0,0,1};//a      1
	STATIC  INT ph_e[10]={1,2,1,2,0,0,1,0,0,2};//e      2
	STATIC 	INT ph_i[10]={1,3,1,2,0,0,1,0,0,3};//i      3
	STATIC 	INT ph_o[10]={1,2,3,1,0,0,1,0,0,4};//o      4
	STATIC 	INT ph_u[10]={1,3,3,1,0,0,1,0,0,5};//u      5
	STATIC 	INT ph_p[10]={2,0,0,0,1,1,2,0,0,6};//p      8
	STATIC 	INT ph_b[10]={2,0,0,0,1,1,1,0,0,7};//b      9
	STATIC 	INT ph_t[10]={2,0,0,0,1,2,2,0,0,8};//t      10
	STATIC 	INT ph_d[10]={2,0,0,0,1,2,1,0,0,9};//d      11
	STATIC 	INT ph_k[10]={2,0,0,0,1,6,2,0,0,10};//k      12
	STATIC 	INT ph_g[10]={2,0,0,0,1,6,1,0,0,11};//g      13
	STATIC 	INT ph_Q[10]={2,0,0,0,1,3,2,0,0,12};//Q      14
	STATIC 	INT ph_m[10]={2,0,0,0,4,1,1,0,0,13};//m      15
	STATIC 	INT ph_n[10]={2,0,0,0,4,2,1,0,0,14};//n      16
	STATIC 	INT ph_J[10]={2,0,0,0,4,7,1,0,0,15};//J      17
	STATIC 	INT ph_tS[10]={2,0,0,0,3,7,2,0,0,16};//tS     18
	STATIC 	INT ph_ts[10]={2,0,0,0,3,2,2,0,0,17};//ts     19
	STATIC 	INT ph_tZ[10]={2,0,0,0,3,5,2,0,0,18};//tZ      21
	STATIC 	INT ph_S[10]={2,0,0,0,2,3,2,0,0,19};//S      20
	STATIC 	INT ph_s[10]={2,0,0,0,2,2,2,0,0,20};//s      22
	STATIC 	INT ph_X[10]={2,0,0,0,2,5,2,0,0,21};//X      23
	STATIC 	INT ph_jj[10]={2,0,0,0,2,3,1,0,0,22};//jj     24
	STATIC 	INT ph_dj[10]={2,0,0,0,3,3,1,0,0,23};//dj     25
	STATIC 	INT ph_B[10]={2,0,0,0,6,1,1,0,0,24};//B      26
	STATIC 	INT ph_f[10]={2,0,0,0,2,4,2,0,0,25};//f      27
	STATIC 	INT ph_D[10]={2,0,0,0,6,2,1,0,0,26};//D      28
	STATIC 	INT ph_T[10]={2,0,0,0,2,8,2,0,0,27};//T      29
	STATIC 	INT ph_x[10]={2,0,0,0,2,6,2,0,0,28};//x      30
	STATIC 	INT ph_G[10]={2,0,0,0,6,6,1,0,0,29};//G      31
	STATIC 	INT ph_l[10]={2,0,0,0,5,2,1,0,0,30};//l      32
	STATIC 	INT ph_L[10]={2,0,0,0,5,3,1,0,0,31};//L      33
	STATIC 	INT ph_r[10]={2,0,0,0,5,2,1,0,0,32};//r      34
	STATIC 	INT ph_rr[10]={2,0,0,0,7,2,1,0,0,33};//rr     35

	STATIC 	INT ph_j[10]={1,1,1,2,0,0,1,0,0,6};//j      6
	STATIC 	INT ph_w[10]={1,1,3,2,0,0,1,0,0,7};//w      7

	switch (ph) {
		case PH_sil:return ph_sil; //_ pause 0
		case PH_a: return ph_a;
		case PH_e: return ph_e;
		case PH_i: return ph_i;
		case PH_o: return ph_o;
		case PH_u: return ph_u;
		case PH_j: return ph_j;
		case PH_w: return ph_w;
		case PH_p: return ph_p;
		case PH_b: return ph_b;
		case PH_t: return ph_t;
		case PH_d: return ph_d;
		case PH_k: return ph_k;
		case PH_g: return ph_g;
		case PH_Q: return ph_Q;
		case PH_m: return ph_m;
		case PH_n: return ph_n;
		case PH_J: return ph_J;
		case PH_tS: return ph_tS;
		case PH_ts: return ph_ts;
		case PH_S: return ph_S;
		case PH_tZ: return ph_tZ;
		case PH_s: return ph_s;
		case PH_X: return ph_X;
		case PH_jj: return ph_jj;
		case PH_dj: return ph_dj;
		case PH_B: return ph_B;
		case PH_f: return ph_f;
		case PH_D: return ph_D;
		case PH_T: return ph_T;
		case PH_x: return ph_x;
		case PH_G: return ph_G;
		case PH_l: return ph_l;
		case PH_L: return ph_L;
		case PH_r: return ph_r;
		case PH_rr: return ph_rr;
	}
	return ph_default;
}
PRIVATE INT Nrel_Pal(UttPh &ut,UttI p){
LONG n,w;
	w=ut.phoneN(p,URANGE_WORD);
	n=ut.phonePos(p,URANGE_WORD);//empieza a contar por 0!
	if(w==1) return 0;//0:single
	if(n==0) return 1;//1:beg
	if((n+1)==w) return 3;//3:end
	return 2;	//2:mid
}
PRIVATE INT Nrel_Sil(UttPh &ut,UttI p){
	LONG n,s;
	s=ut.phoneN(p,URANGE_SYLLABLE);
	n=ut.phonePos(p,URANGE_SYLLABLE);//empieza a contar por 0!
	if(s==1) return 0;//0:single
	if(n==0) return 1;//1:beg
	if((n+1)==s) return 3;//3:end
	return 2;	//2:mid
}
PRIVATE INT Stress(UttPh &ut,UttI p){
	int acento;
	if ( ((ut.cell(p).getStress())==USTRESS_TEXT) || ((ut.cell(p).getStress())==USTRESS_AUTO))
						acento=1;
	if ( (ut.cell(p).getStress())==USTRESS_NONE)
						acento=0;
	if(acento==0) return 0;//0:no acento
	return 1;//1:acento
}



PRIVATE DOUBLE Cart_Dur(CartCfg cartcfg) {
	INT terminalNode=0;
	DOUBLE mean=0;
/*Terminal Node 1*/
if
(
  (
      cartcfg.n_numfon == 6 ||
      cartcfg.n_numfon == 8 ||
      cartcfg.n_numfon == 10 ||
      cartcfg.n_numfon == 13 ||
      cartcfg.n_numfon == 16 ||
      cartcfg.n_numfon == 17 ||
      cartcfg.n_numfon == 18 ||
      cartcfg.n_numfon == 23 
  ) &&
  (
      cartcfg.p_numfon == 0 ||
      cartcfg.p_numfon == 1 ||
      cartcfg.p_numfon == 2 ||
      cartcfg.p_numfon == 3 ||
			cartcfg.p_numfon == 4 ||
			cartcfg.p_numfon == 5 ||
      cartcfg.p_numfon == 6 ||
      cartcfg.p_numfon == 7 ||
      cartcfg.p_numfon == 8 ||
      cartcfg.p_numfon == 9 ||
      cartcfg.p_numfon == 10 ||
      cartcfg.p_numfon == 11 ||
      cartcfg.p_numfon == 12 ||
      cartcfg.p_numfon == 13 ||
      cartcfg.p_numfon == 14 ||
      cartcfg.p_numfon == 15 ||
      cartcfg.p_numfon == 16 ||
      cartcfg.p_numfon == 17 ||
      cartcfg.p_numfon == 18 ||
      cartcfg.p_numfon == 19 ||
      cartcfg.p_numfon == 20 ||
      cartcfg.p_numfon == 21 ||
      cartcfg.p_numfon == 24 ||
      cartcfg.p_numfon == 25 ||
			cartcfg.p_numfon == 27 ||
			cartcfg.p_numfon == 28 ||
      cartcfg.p_numfon == 29 ||
      cartcfg.p_numfon == 30 
  ) 
)
{
		mean = -0.459925;
}

/*Terminal Node 2*/
if
(
	(
			cartcfg.p_numfon == 6 ||
			cartcfg.p_numfon == 7 ||
			cartcfg.p_numfon == 8 ||
			cartcfg.p_numfon == 10 ||
			cartcfg.p_numfon == 13 ||
			cartcfg.p_numfon == 14 ||
			cartcfg.p_numfon == 16 ||
			cartcfg.p_numfon == 17 ||
			cartcfg.p_numfon == 18 ||
			cartcfg.p_numfon == 19 ||
			cartcfg.p_numfon == 20 ||
			cartcfg.p_numfon == 21 ||
			cartcfg.p_numfon == 25 ||
			cartcfg.p_numfon == 27 ||
			cartcfg.p_numfon == 28
	) &&
	(
			cartcfg.n_numfon == 1 ||
			cartcfg.n_numfon == 2 ||
			cartcfg.n_numfon == 3 ||
			cartcfg.n_numfon == 4 ||
			cartcfg.n_numfon == 5 ||
			cartcfg.n_numfon == 7 ||
			cartcfg.n_numfon == 9 ||
			cartcfg.n_numfon == 11 ||
			cartcfg.n_numfon == 12 ||
			cartcfg.n_numfon == 14 ||
			cartcfg.n_numfon == 15 ||
			cartcfg.n_numfon == 19 ||
			cartcfg.n_numfon == 20 ||
			cartcfg.n_numfon == 21 ||
			cartcfg.n_numfon == 22 ||
      cartcfg.n_numfon == 24 ||
			cartcfg.n_numfon == 25 ||
			cartcfg.n_numfon == 26 ||
			cartcfg.n_numfon == 28 ||
			cartcfg.n_numfon == 29 ||
			cartcfg.n_numfon == 30 ||
			cartcfg.n_numfon == 31 ||
			cartcfg.n_numfon == 32 ||
			cartcfg.n_numfon == 33 ||
			cartcfg.n_numfon == 34
	)
)
{
		mean = -0.24868;
}

/*Terminal Node 3*/
if
(
  (
      cartcfg.n_numfon == 1 ||
      cartcfg.n_numfon == 2 ||
      cartcfg.n_numfon == 3 ||
      cartcfg.n_numfon == 4 ||
      cartcfg.n_numfon == 11 ||
      cartcfg.n_numfon == 14 ||
      cartcfg.n_numfon == 19 ||
      cartcfg.n_numfon == 20 ||
      cartcfg.n_numfon == 21 ||
      cartcfg.n_numfon == 24 ||
      cartcfg.n_numfon == 28 ||
      cartcfg.n_numfon == 29 
  ) &&
	(
			cartcfg.numfon == 9 ||
			cartcfg.numfon == 12 ||
      cartcfg.numfon == 14 ||
      cartcfg.numfon == 15 ||
      cartcfg.numfon == 16 ||
      cartcfg.numfon == 26 ||
      cartcfg.numfon == 28 ||
      cartcfg.numfon == 29 ||
      cartcfg.numfon == 30 ||
      cartcfg.numfon == 32 
  ) &&
  (
      cartcfg.nn_numfon == 1 ||
      cartcfg.nn_numfon == 2 ||
      cartcfg.nn_numfon == 3 ||
      cartcfg.nn_numfon == 4 ||
      cartcfg.nn_numfon == 5 ||
      cartcfg.nn_numfon == 6 ||
      cartcfg.nn_numfon == 8 ||
			cartcfg.nn_numfon == 9 ||
			cartcfg.nn_numfon == 10 ||
			cartcfg.nn_numfon == 11 ||
      cartcfg.nn_numfon == 12 ||
      cartcfg.nn_numfon == 13 ||
      cartcfg.nn_numfon == 14 ||
      cartcfg.nn_numfon == 15 ||
      cartcfg.nn_numfon == 16 ||
      cartcfg.nn_numfon == 18 ||
      cartcfg.nn_numfon == 19 ||
      cartcfg.nn_numfon == 20 ||
      cartcfg.nn_numfon == 21 ||
      cartcfg.nn_numfon == 22 ||
      cartcfg.nn_numfon == 23 ||
      cartcfg.nn_numfon == 24 ||
      cartcfg.nn_numfon == 25 ||
      cartcfg.nn_numfon == 26 ||
      cartcfg.nn_numfon == 27 ||
      cartcfg.nn_numfon == 29 ||
      cartcfg.nn_numfon == 30 ||
			cartcfg.nn_numfon == 31 ||
			cartcfg.nn_numfon == 32 ||
			cartcfg.nn_numfon == 33 ||
      cartcfg.nn_numfon == 34 
  ) &&
  (
      cartcfg.p_numfon == 0 ||
      cartcfg.p_numfon == 1 ||
      cartcfg.p_numfon == 2 ||
      cartcfg.p_numfon == 3 ||
      cartcfg.p_numfon == 4 ||
      cartcfg.p_numfon == 5 ||
      cartcfg.p_numfon == 9 ||
      cartcfg.p_numfon == 11 ||
      cartcfg.p_numfon == 12 ||
      cartcfg.p_numfon == 15 ||
      cartcfg.p_numfon == 24 ||
      cartcfg.p_numfon == 29 ||
      cartcfg.p_numfon == 30 
  ) 
)
{
		mean = -0.319498;
}

/*Terminal Node 4*/
if
(
  (
      cartcfg.n_numfon == 5 ||
      cartcfg.n_numfon == 7 ||
      cartcfg.n_numfon == 9 ||
      cartcfg.n_numfon == 12 ||
      cartcfg.n_numfon == 15 ||
      cartcfg.n_numfon == 22 ||
      cartcfg.n_numfon == 25 ||
      cartcfg.n_numfon == 26 ||
      cartcfg.n_numfon == 30 ||
      cartcfg.n_numfon == 31 ||
      cartcfg.n_numfon == 32 ||
      cartcfg.n_numfon == 33 ||
			cartcfg.n_numfon == 34
	) &&
  (
      cartcfg.numfon == 9 ||
      cartcfg.numfon == 12 ||
      cartcfg.numfon == 14 ||
      cartcfg.numfon == 15 ||
      cartcfg.numfon == 16 ||
      cartcfg.numfon == 26 ||
      cartcfg.numfon == 28 ||
      cartcfg.numfon == 29 ||
      cartcfg.numfon == 30 ||
      cartcfg.numfon == 32 
  ) &&
  (
      cartcfg.nn_numfon == 1 ||
      cartcfg.nn_numfon == 2 ||
      cartcfg.nn_numfon == 3 ||
      cartcfg.nn_numfon == 4 ||
      cartcfg.nn_numfon == 5 ||
			cartcfg.nn_numfon == 6 ||
			cartcfg.nn_numfon == 8 ||
      cartcfg.nn_numfon == 9 ||
      cartcfg.nn_numfon == 10 ||
      cartcfg.nn_numfon == 11 ||
      cartcfg.nn_numfon == 12 ||
      cartcfg.nn_numfon == 13 ||
      cartcfg.nn_numfon == 14 ||
      cartcfg.nn_numfon == 15 ||
      cartcfg.nn_numfon == 16 ||
      cartcfg.nn_numfon == 18 ||
      cartcfg.nn_numfon == 19 ||
      cartcfg.nn_numfon == 20 ||
      cartcfg.nn_numfon == 21 ||
      cartcfg.nn_numfon == 22 ||
      cartcfg.nn_numfon == 23 ||
      cartcfg.nn_numfon == 24 ||
      cartcfg.nn_numfon == 25 ||
      cartcfg.nn_numfon == 26 ||
      cartcfg.nn_numfon == 27 ||
			cartcfg.nn_numfon == 29 ||
			cartcfg.nn_numfon == 30 ||
      cartcfg.nn_numfon == 31 ||
      cartcfg.nn_numfon == 32 ||
      cartcfg.nn_numfon == 33 ||
      cartcfg.nn_numfon == 34 
  ) &&
  (
      cartcfg.p_numfon == 0 ||
      cartcfg.p_numfon == 1 ||
      cartcfg.p_numfon == 2 ||
      cartcfg.p_numfon == 3 ||
      cartcfg.p_numfon == 4 ||
      cartcfg.p_numfon == 5 ||
      cartcfg.p_numfon == 9 ||
      cartcfg.p_numfon == 11 ||
      cartcfg.p_numfon == 12 ||
      cartcfg.p_numfon == 15 ||
      cartcfg.p_numfon == 24 ||
      cartcfg.p_numfon == 29 ||
			cartcfg.p_numfon == 30
	)
)
{
    terminalNode = -4;
		mean = 0.342646;
}

/*Terminal Node 5*/
if
(
  (
      cartcfg.numfon == 1 ||
      cartcfg.numfon == 2 ||
      cartcfg.numfon == 3 ||
      cartcfg.numfon == 4 ||
      cartcfg.numfon == 5 ||
      cartcfg.numfon == 6 ||
      cartcfg.numfon == 7 ||
      cartcfg.numfon == 8 ||
      cartcfg.numfon == 10 ||
      cartcfg.numfon == 11 ||
      cartcfg.numfon == 13 ||
      cartcfg.numfon == 17 ||
      cartcfg.numfon == 18 ||
      cartcfg.numfon == 19 ||
      cartcfg.numfon == 20 ||
      cartcfg.numfon == 21 ||
      cartcfg.numfon == 22 ||
      cartcfg.numfon == 23 ||
      cartcfg.numfon == 24 ||
      cartcfg.numfon == 25 ||
      cartcfg.numfon == 27 ||
      cartcfg.numfon == 31 ||
      cartcfg.numfon == 33 
  ) &&
  (
      cartcfg.nn_numfon == 1 ||
      cartcfg.nn_numfon == 2 ||
      cartcfg.nn_numfon == 3 ||
      cartcfg.nn_numfon == 4 ||
      cartcfg.nn_numfon == 5 ||
      cartcfg.nn_numfon == 6 ||
      cartcfg.nn_numfon == 8 ||
      cartcfg.nn_numfon == 9 ||
      cartcfg.nn_numfon == 10 ||
      cartcfg.nn_numfon == 11 ||
      cartcfg.nn_numfon == 12 ||
      cartcfg.nn_numfon == 13 ||
      cartcfg.nn_numfon == 14 ||
      cartcfg.nn_numfon == 15 ||
      cartcfg.nn_numfon == 16 ||
      cartcfg.nn_numfon == 18 ||
      cartcfg.nn_numfon == 19 ||
      cartcfg.nn_numfon == 20 ||
      cartcfg.nn_numfon == 21 ||
      cartcfg.nn_numfon == 22 ||
      cartcfg.nn_numfon == 23 ||
      cartcfg.nn_numfon == 24 ||
      cartcfg.nn_numfon == 25 ||
      cartcfg.nn_numfon == 26 ||
      cartcfg.nn_numfon == 27 ||
      cartcfg.nn_numfon == 29 ||
      cartcfg.nn_numfon == 30 ||
      cartcfg.nn_numfon == 31 ||
      cartcfg.nn_numfon == 32 ||
      cartcfg.nn_numfon == 33 ||
      cartcfg.nn_numfon == 34 
  ) &&
  (
      cartcfg.p_numfon == 0 ||
      cartcfg.p_numfon == 1 ||
      cartcfg.p_numfon == 2 ||
      cartcfg.p_numfon == 3 ||
      cartcfg.p_numfon == 4 ||
      cartcfg.p_numfon == 5 ||
      cartcfg.p_numfon == 9 ||
      cartcfg.p_numfon == 11 ||
      cartcfg.p_numfon == 12 ||
      cartcfg.p_numfon == 15 ||
      cartcfg.p_numfon == 24 ||
      cartcfg.p_numfon == 29 ||
      cartcfg.p_numfon == 30 
  ) &&
  (
      cartcfg.n_numfon == 1 ||
      cartcfg.n_numfon == 2 ||
      cartcfg.n_numfon == 3 ||
      cartcfg.n_numfon == 4 ||
      cartcfg.n_numfon == 5 ||
      cartcfg.n_numfon == 7 ||
      cartcfg.n_numfon == 9 ||
      cartcfg.n_numfon == 11 ||
      cartcfg.n_numfon == 12 ||
      cartcfg.n_numfon == 14 ||
      cartcfg.n_numfon == 15 ||
      cartcfg.n_numfon == 19 ||
      cartcfg.n_numfon == 20 ||
      cartcfg.n_numfon == 21 ||
      cartcfg.n_numfon == 22 ||
      cartcfg.n_numfon == 24 ||
      cartcfg.n_numfon == 25 ||
      cartcfg.n_numfon == 26 ||
      cartcfg.n_numfon == 28 ||
      cartcfg.n_numfon == 29 ||
      cartcfg.n_numfon == 30 ||
      cartcfg.n_numfon == 31 ||
      cartcfg.n_numfon == 32 ||
      cartcfg.n_numfon == 33 ||
      cartcfg.n_numfon == 34 
  ) 
)
{
    terminalNode = -5;
		mean = 0.134152;
}

/*Terminal Node 6*/
if
(
  (
      cartcfg.nn_numfon == 0 ||
      cartcfg.nn_numfon == 7 ||
      cartcfg.nn_numfon == 17 ||
      cartcfg.nn_numfon == 28 ||
      cartcfg.nn_numfon == 35 
  ) &&
  (
      cartcfg.p_numfon == 0 ||
      cartcfg.p_numfon == 1 ||
      cartcfg.p_numfon == 2 ||
      cartcfg.p_numfon == 3 ||
      cartcfg.p_numfon == 4 ||
      cartcfg.p_numfon == 5 ||
      cartcfg.p_numfon == 9 ||
      cartcfg.p_numfon == 11 ||
      cartcfg.p_numfon == 12 ||
      cartcfg.p_numfon == 15 ||
      cartcfg.p_numfon == 24 ||
      cartcfg.p_numfon == 29 ||
      cartcfg.p_numfon == 30 
  ) &&
  (
      cartcfg.n_numfon == 1 ||
      cartcfg.n_numfon == 2 ||
      cartcfg.n_numfon == 3 ||
      cartcfg.n_numfon == 4 ||
      cartcfg.n_numfon == 5 ||
      cartcfg.n_numfon == 7 ||
      cartcfg.n_numfon == 9 ||
      cartcfg.n_numfon == 11 ||
      cartcfg.n_numfon == 12 ||
      cartcfg.n_numfon == 14 ||
      cartcfg.n_numfon == 15 ||
      cartcfg.n_numfon == 19 ||
      cartcfg.n_numfon == 20 ||
      cartcfg.n_numfon == 21 ||
      cartcfg.n_numfon == 22 ||
      cartcfg.n_numfon == 24 ||
      cartcfg.n_numfon == 25 ||
      cartcfg.n_numfon == 26 ||
      cartcfg.n_numfon == 28 ||
      cartcfg.n_numfon == 29 ||
      cartcfg.n_numfon == 30 ||
      cartcfg.n_numfon == 31 ||
      cartcfg.n_numfon == 32 ||
      cartcfg.n_numfon == 33 ||
      cartcfg.n_numfon == 34 
  ) 
)
{
    terminalNode = -6;
		mean = 0.71947;
}

/*Terminal Node 7*/
if
(
  (
      cartcfg.p_numfon == 22 ||
      cartcfg.p_numfon == 23 ||
      cartcfg.p_numfon == 26 ||
      cartcfg.p_numfon == 31 ||
      cartcfg.p_numfon == 32 ||
      cartcfg.p_numfon == 33 ||
      cartcfg.p_numfon == 34 
  ) &&
  (
      cartcfg.n_numfon == 1 ||
      cartcfg.n_numfon == 2 ||
      cartcfg.n_numfon == 3 ||
      cartcfg.n_numfon == 4 ||
      cartcfg.n_numfon == 5 ||
      cartcfg.n_numfon == 6 ||
      cartcfg.n_numfon == 7 ||
      cartcfg.n_numfon == 8 ||
      cartcfg.n_numfon == 9 ||
      cartcfg.n_numfon == 10 ||
      cartcfg.n_numfon == 11 ||
      cartcfg.n_numfon == 12 ||
      cartcfg.n_numfon == 13 ||
      cartcfg.n_numfon == 14 ||
      cartcfg.n_numfon == 15 ||
      cartcfg.n_numfon == 16 ||
      cartcfg.n_numfon == 17 ||
      cartcfg.n_numfon == 18 ||
      cartcfg.n_numfon == 19 ||
      cartcfg.n_numfon == 20 ||
      cartcfg.n_numfon == 21 ||
      cartcfg.n_numfon == 22 ||
      cartcfg.n_numfon == 23 ||
      cartcfg.n_numfon == 24 ||
      cartcfg.n_numfon == 25 ||
      cartcfg.n_numfon == 26 ||
      cartcfg.n_numfon == 28 ||
      cartcfg.n_numfon == 29 ||
      cartcfg.n_numfon == 30 ||
      cartcfg.n_numfon == 31 ||
      cartcfg.n_numfon == 32 ||
      cartcfg.n_numfon == 33 ||
      cartcfg.n_numfon == 34 
  ) 
)
{
    terminalNode = -7;
		mean = 0.479791;
}

/*Terminal Node 8*/
if
(
  (
      cartcfg.numfon == 6 ||
      cartcfg.numfon == 7 ||
      cartcfg.numfon == 9 ||
      cartcfg.numfon == 10 ||
      cartcfg.numfon == 11 ||
      cartcfg.numfon == 12 ||
      cartcfg.numfon == 13 ||
      cartcfg.numfon == 14 ||
      cartcfg.numfon == 15 ||
      cartcfg.numfon == 16 ||
      cartcfg.numfon == 17 ||
      cartcfg.numfon == 18 ||
      cartcfg.numfon == 19 ||
      cartcfg.numfon == 20 ||
      cartcfg.numfon == 21 ||
      cartcfg.numfon == 22 ||
      cartcfg.numfon == 23 ||
      cartcfg.numfon == 24 ||
      cartcfg.numfon == 25 ||
      cartcfg.numfon == 26 ||
      cartcfg.numfon == 27 ||
      cartcfg.numfon == 28 ||
      cartcfg.numfon == 29 ||
      cartcfg.numfon == 30 ||
      cartcfg.numfon == 31 ||
      cartcfg.numfon == 32 
  ) &&
  (
      cartcfg.n_numfon == 0 ||
      cartcfg.n_numfon == 27 ||
      cartcfg.n_numfon == 35 
  ) 
)
{
    terminalNode = -8;
		mean = 0.293523;
}

/*Terminal Node 9*/
if
(
  (
      cartcfg.pp_numfon == 0 ||
      cartcfg.pp_numfon == 1 ||
      cartcfg.pp_numfon == 2 ||
      cartcfg.pp_numfon == 3 ||
      cartcfg.pp_numfon == 4 ||
      cartcfg.pp_numfon == 5 ||
      cartcfg.pp_numfon == 6 ||
      cartcfg.pp_numfon == 7 ||
      cartcfg.pp_numfon == 8 ||
      cartcfg.pp_numfon == 9 ||
      cartcfg.pp_numfon == 10 ||
      cartcfg.pp_numfon == 11 ||
      cartcfg.pp_numfon == 12 ||
      cartcfg.pp_numfon == 13 ||
      cartcfg.pp_numfon == 14 ||
      cartcfg.pp_numfon == 15 ||
      cartcfg.pp_numfon == 16 ||
      cartcfg.pp_numfon == 17 ||
      cartcfg.pp_numfon == 18 ||
      cartcfg.pp_numfon == 19 ||
      cartcfg.pp_numfon == 20 ||
      cartcfg.pp_numfon == 21 ||
      cartcfg.pp_numfon == 22 ||
      cartcfg.pp_numfon == 23 ||
      cartcfg.pp_numfon == 25 ||
      cartcfg.pp_numfon == 26 ||
      cartcfg.pp_numfon == 27 ||
      cartcfg.pp_numfon == 28 ||
      cartcfg.pp_numfon == 29 ||
      cartcfg.pp_numfon == 31 ||
      cartcfg.pp_numfon == 32 ||
      cartcfg.pp_numfon == 33 ||
      cartcfg.pp_numfon == 34 
  ) &&
  (
      cartcfg.nn_numfon == 0 ||
      cartcfg.nn_numfon == 3 ||
      cartcfg.nn_numfon == 5 ||
      cartcfg.nn_numfon == 6 ||
      cartcfg.nn_numfon == 8 ||
      cartcfg.nn_numfon == 10 ||
      cartcfg.nn_numfon == 13 ||
      cartcfg.nn_numfon == 14 ||
      cartcfg.nn_numfon == 15 ||
      cartcfg.nn_numfon == 16 ||
      cartcfg.nn_numfon == 17 ||
      cartcfg.nn_numfon == 18 ||
      cartcfg.nn_numfon == 19 ||
      cartcfg.nn_numfon == 20 ||
      cartcfg.nn_numfon == 22 ||
      cartcfg.nn_numfon == 23 ||
      cartcfg.nn_numfon == 24 ||
      cartcfg.nn_numfon == 25 ||
      cartcfg.nn_numfon == 26 ||
      cartcfg.nn_numfon == 27 ||
      cartcfg.nn_numfon == 28 ||
      cartcfg.nn_numfon == 30 ||
      cartcfg.nn_numfon == 31 ||
      cartcfg.nn_numfon == 32 ||
      cartcfg.nn_numfon == 33 ||
      cartcfg.nn_numfon == 34 ||
      cartcfg.nn_numfon == 35 
  ) &&
  (
      cartcfg.numfon == 1 ||
      cartcfg.numfon == 2 ||
      cartcfg.numfon == 3 ||
      cartcfg.numfon == 4 ||
      cartcfg.numfon == 5 ||
      cartcfg.numfon == 8 ||
      cartcfg.numfon == 33 
  ) &&
  (
      cartcfg.n_numfon == 0 ||
      cartcfg.n_numfon == 27 ||
      cartcfg.n_numfon == 35 
  ) 
)
{
    terminalNode = -9;
		mean = 0.871955;
}

/*Terminal Node 10*/
if
(
  (
      cartcfg.pp_numfon == 24 ||
      cartcfg.pp_numfon == 30 
  ) &&
  (
      cartcfg.nn_numfon == 0 ||
      cartcfg.nn_numfon == 3 ||
      cartcfg.nn_numfon == 5 ||
      cartcfg.nn_numfon == 6 ||
      cartcfg.nn_numfon == 8 ||
      cartcfg.nn_numfon == 10 ||
      cartcfg.nn_numfon == 13 ||
      cartcfg.nn_numfon == 14 ||
      cartcfg.nn_numfon == 15 ||
      cartcfg.nn_numfon == 16 ||
      cartcfg.nn_numfon == 17 ||
      cartcfg.nn_numfon == 18 ||
      cartcfg.nn_numfon == 19 ||
      cartcfg.nn_numfon == 20 ||
      cartcfg.nn_numfon == 22 ||
      cartcfg.nn_numfon == 23 ||
      cartcfg.nn_numfon == 24 ||
      cartcfg.nn_numfon == 25 ||
      cartcfg.nn_numfon == 26 ||
      cartcfg.nn_numfon == 27 ||
      cartcfg.nn_numfon == 28 ||
      cartcfg.nn_numfon == 30 ||
      cartcfg.nn_numfon == 31 ||
      cartcfg.nn_numfon == 32 ||
      cartcfg.nn_numfon == 33 ||
      cartcfg.nn_numfon == 34 ||
      cartcfg.nn_numfon == 35 
  ) &&
  (
      cartcfg.numfon == 1 ||
      cartcfg.numfon == 2 ||
      cartcfg.numfon == 3 ||
      cartcfg.numfon == 4 ||
      cartcfg.numfon == 5 ||
      cartcfg.numfon == 8 ||
      cartcfg.numfon == 33 
  ) &&
  (
      cartcfg.n_numfon == 0 ||
      cartcfg.n_numfon == 27 ||
      cartcfg.n_numfon == 35 
  ) 
)
{
    terminalNode = -10;
		mean = 5.59063;
}

/*Terminal Node 11*/
if
(
  (
      cartcfg.pp_numfon == 0 ||
      cartcfg.pp_numfon == 3 ||
      cartcfg.pp_numfon == 4 ||
      cartcfg.pp_numfon == 6 ||
      cartcfg.pp_numfon == 7 ||
      cartcfg.pp_numfon == 8 ||
      cartcfg.pp_numfon == 9 ||
      cartcfg.pp_numfon == 10 ||
      cartcfg.pp_numfon == 11 ||
      cartcfg.pp_numfon == 12 ||
      cartcfg.pp_numfon == 13 ||
      cartcfg.pp_numfon == 15 ||
      cartcfg.pp_numfon == 16 ||
      cartcfg.pp_numfon == 17 ||
      cartcfg.pp_numfon == 19 ||
      cartcfg.pp_numfon == 20 ||
      cartcfg.pp_numfon == 22 ||
      cartcfg.pp_numfon == 23 ||
      cartcfg.pp_numfon == 24 ||
      cartcfg.pp_numfon == 25 ||
      cartcfg.pp_numfon == 26 ||
      cartcfg.pp_numfon == 27 ||
      cartcfg.pp_numfon == 28 ||
      cartcfg.pp_numfon == 30 ||
      cartcfg.pp_numfon == 31 ||
      cartcfg.pp_numfon == 32 ||
      cartcfg.pp_numfon == 34 
  ) &&
  (
      cartcfg.nn_numfon == 1 ||
      cartcfg.nn_numfon == 2 ||
      cartcfg.nn_numfon == 4 ||
      cartcfg.nn_numfon == 7 ||
      cartcfg.nn_numfon == 9 ||
      cartcfg.nn_numfon == 11 ||
      cartcfg.nn_numfon == 12 ||
      cartcfg.nn_numfon == 21 ||
      cartcfg.nn_numfon == 29 
  ) &&
  (
      cartcfg.numfon == 1 ||
      cartcfg.numfon == 2 ||
      cartcfg.numfon == 3 ||
      cartcfg.numfon == 4 ||
      cartcfg.numfon == 5 ||
      cartcfg.numfon == 8 ||
      cartcfg.numfon == 33 
  ) &&
  (
      cartcfg.n_numfon == 0 ||
      cartcfg.n_numfon == 27 ||
      cartcfg.n_numfon == 35 
  ) 
)
{
    terminalNode = -11;
		mean = 1.23785;
}

/*Terminal Node 12*/
if
(
  (
      cartcfg.pp_numfon == 1 ||
      cartcfg.pp_numfon == 2 ||
      cartcfg.pp_numfon == 5 ||
      cartcfg.pp_numfon == 14 ||
      cartcfg.pp_numfon == 18 ||
      cartcfg.pp_numfon == 21 ||
      cartcfg.pp_numfon == 29 ||
      cartcfg.pp_numfon == 33 
  ) &&
  (
      cartcfg.nn_numfon == 1 ||
      cartcfg.nn_numfon == 2 ||
      cartcfg.nn_numfon == 4 ||
      cartcfg.nn_numfon == 7 ||
      cartcfg.nn_numfon == 9 ||
      cartcfg.nn_numfon == 11 ||
      cartcfg.nn_numfon == 12 ||
      cartcfg.nn_numfon == 21 ||
      cartcfg.nn_numfon == 29 
  ) &&
  (
      cartcfg.numfon == 1 ||
      cartcfg.numfon == 2 ||
      cartcfg.numfon == 3 ||
      cartcfg.numfon == 4 ||
      cartcfg.numfon == 5 ||
      cartcfg.numfon == 8 ||
      cartcfg.numfon == 33 
  ) &&
  (
      cartcfg.n_numfon == 0 ||
      cartcfg.n_numfon == 27 ||
      cartcfg.n_numfon == 35 
  ) 
)
{
    terminalNode = -12;
		mean = 3.79816;
}


	return mean;
}
/**********************************************************/

VOID LangEU_Prosod::utt_dur2(UttPh & ut)
{
	UttI p;

  dur_dev=0; //Porque en este modelo la desviacion estandar no tiene
             //sentido. Solo hay un valor de duracion al que modificar 
             //asi que con la media basta.

	//Pongo la media en tanto por uno en cada fonema y luego se multiplica
//por lo que dice el arbol

	for (p=ut.phoneFirst(); p!=0; p=ut.phoneNext(p)) {
		ut.cell(p).setDur(dur_mean);
	}

	utt_dur_unnorm(ut); // desnormalizar a milisegundos duraciones
}

/**********************************************************/
#endif
