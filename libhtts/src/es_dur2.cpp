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
/**********************************************************/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/*//MODELO DE DURACION BASADO EN ARBOLES PARA CASTELLANO PARA LOCUTORA MARTA
(C) 1999 TSR/Aholab - ETSII/IT Bilbao (UPV/EHU) // Euskaltel

Nombre fuente................ es_dur2.cpp
Nombre paquete............... aHoTTS
Lenguaje fuente.............. C++
Estado....................... -
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Borja Etxebarria
............................. Ana Armenta

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
2.0.0    31/01/00  inaki     Añadir Modelo duracion castellano (Eva)
1.0.0    31/01/00  borja     codefreeze aHoTTS v1.0
0.0.0    24/11/97  borja     Codificacion inicial.

======================== Contenido ========================
<DOC>
Metodos de {{LangEU_Prosod}} que implementan diferentes
modelos de duracion. Este es un modelo estadistico en
base a CART. Se usan tres árboles, uno para las vocales,
otro para las consonantes sonoras y el ultimo para las sordas
</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include <math.h>
#include "phone.h"
#include "es_lingp.hpp"
#include "xx_uti.hpp"
#include "httsmsg.h"

#ifdef HTTS_PROSOD_ES_DUR2
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
	INT nsilword;
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
	INT n_nsilword;

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
	INT nn_nsilword;
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
	INT p_nsilword;
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
	INT pp_nsilword;
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
	CartCfg ccfg={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
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
//Realmente los arboles predicen la raiz cuadrada de la duracion, luego tenemos que
//deshacer la cuenta
		//aux=d*datos_f[ccfg.numfon][1]+datos_f[ccfg.numfon][0];
		d=d*d;

		if((ut.cell(p).getPhone()==PH_sil)){//&&(ut.cell(p).getPause()==UPAUSE_SHORT))
			if(ut.cell(p).getPause()==UPAUSE_UBEGIN || ut.cell(p).getPause()==UPAUSE_UEND  )		
				d=200;
			else if(ut.cell(p).getPause()==UPAUSE_SHORT)
				d=140;
			else
				d=140;
		}		


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
	INT initial[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0};
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
		ccfg.n_begend=1;
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
		ccfg.nn_begend=1;
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
	if(ut.wordPrev(ut.wordThis(p)))
 		ccfg.nsilword=ut.syllableN(p,URANGE_WORD)-ut.syllableN(ut.wordPrev(ut.wordThis(p)),URANGE_WORD);
	else
		ccfg.nsilword=ut.syllableN(p,URANGE_WORD);
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
	STATIC 	INT ph_m[10]={2,0,0,0,4,1,1,0,0,12};//m      15
	STATIC 	INT ph_n[10]={2,0,0,0,4,2,1,0,0,13};//n      16
	STATIC 	INT ph_J[10]={2,0,0,0,4,7,1,0,0,14};//J      17
	STATIC 	INT ph_tS[10]={2,0,0,0,3,7,2,0,0,15};//tS     18
	STATIC 	INT ph_s[10]={2,0,0,0,2,2,2,0,0,16};//s      22
	STATIC 	INT ph_jj[10]={2,0,0,0,2,3,1,0,0,17};//jj     24
	STATIC 	INT ph_B[10]={2,0,0,0,6,1,1,0,0,18};//B      26
	STATIC 	INT ph_f[10]={2,0,0,0,2,4,2,0,0,19};//f      27
	STATIC 	INT ph_D[10]={2,0,0,0,6,2,1,0,0,20};//D      28
	STATIC 	INT ph_T[10]={2,0,0,0,2,8,2,0,0,21};//T      29
	STATIC 	INT ph_x[10]={2,0,0,0,2,6,2,0,0,22};//x      30
	STATIC 	INT ph_G[10]={2,0,0,0,6,6,1,0,0,23};//G      31
	STATIC 	INT ph_l[10]={2,0,0,0,5,2,1,0,0,24};//l      32
	STATIC 	INT ph_L[10]={2,0,0,0,5,3,1,0,0,25};//L      33
	STATIC 	INT ph_r[10]={2,0,0,0,5,2,1,0,0,26};//r      34
	STATIC 	INT ph_rr[10]={2,0,0,0,7,2,1,0,0,27};//rr     35

	STATIC 	INT ph_j[10]={1,1,1,2,0,0,1,0,0,30};//j      6
	STATIC 	INT ph_w[10]={1,1,3,2,0,0,1,0,0,31};//w      7

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
		case PH_m: return ph_m;
		case PH_n: return ph_n;
		case PH_J: return ph_J;
		case PH_tS: return ph_tS;
		case PH_s: return ph_s;
		case PH_jj: return ph_jj;
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
/*	long nfonsil;
	if(ut.syllablePrev(ut.syllableThis(p))){
	 	nfonsil=ut.phoneN(p,URANGE_SYLLABLE)-ut.phoneN(ut.syllablePrev(ut.syllableThis(p)),URANGE_SYLLABLE);
		if(!ut.syllableNext(ut.syllableThis(p),URANGE_SENTENCE))
			nfonsil=nfonsil-1;
	}
	else {
		nfonsil=ut.phoneN(p,URANGE_SYLLABLE)-1;
	}
	return nfonsil;
*/}
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
	DOUBLE mean=0;

/*****************************/	
//convertir beg y end a _
if(cartcfg.numfon == 28 || cartcfg.numfon == 29)
	cartcfg.numfon = 0;
if(cartcfg.n_numfon == 28 || cartcfg.n_numfon == 29)
	cartcfg.n_numfon = 0;
if(cartcfg.nn_numfon == 28 || cartcfg.nn_numfon == 29)
	cartcfg.nn_numfon = 0;
if(cartcfg.p_numfon == 28 || cartcfg.p_numfon == 29)
	cartcfg.p_numfon = 0;
if(cartcfg.pp_numfon == 28 || cartcfg.pp_numfon == 29)
	cartcfg.pp_numfon = 0;
/*****************************/	

//SONORAS
if( cartcfg.numfon == 7 || cartcfg.numfon == 9 || cartcfg.numfon == 11 || cartcfg.numfon == 12 || cartcfg.numfon == 13 || cartcfg.numfon == 14 
|| cartcfg.numfon == 17 || cartcfg.numfon == 18 || cartcfg.numfon == 20 || cartcfg.numfon == 23 || cartcfg.numfon == 24 || cartcfg.numfon == 25 
|| cartcfg.numfon == 26 || cartcfg.numfon == 27 ){
	/*CLASSIFICATION AND REGRESSION TREE*/
	//Node1
	if( cartcfg.numfon == 18  || cartcfg.numfon == 20  || cartcfg.numfon == 23  || cartcfg.numfon == 7  || cartcfg.numfon == 9  || cartcfg.numfon == 11  || cartcfg.numfon == 24  || cartcfg.numfon == 13  || cartcfg.numfon == 26 )
	{
		//Node2
		if( cartcfg.numfon == 18  || cartcfg.numfon == 20  || cartcfg.numfon == 23  || cartcfg.numfon == 7  || cartcfg.numfon == 9  || cartcfg.numfon == 11  || cartcfg.numfon == 26 )
		{
			//Node3
			if( cartcfg.n_numfon == 20  || cartcfg.n_numfon == 14  || cartcfg.n_numfon == 1  || cartcfg.n_numfon == 7  || cartcfg.n_numfon == 2  || cartcfg.n_numfon == 11  || cartcfg.n_numfon == 3  || cartcfg.n_numfon == 30  || cartcfg.n_numfon == 4  || cartcfg.n_numfon == 26  || cartcfg.n_numfon == 15  || cartcfg.n_numfon == 5 )
			{
				//Node4
				if( cartcfg.p_numfon == 18  || cartcfg.p_numfon == 1  || cartcfg.p_numfon == 7  || cartcfg.p_numfon == 2  || cartcfg.p_numfon == 19  || cartcfg.p_numfon == 12  || cartcfg.p_numfon == 13  || cartcfg.p_numfon == 4  || cartcfg.p_numfon == 6  || cartcfg.p_numfon == 8 )
				{
					//Node5
					if( cartcfg.numfon == 18  || cartcfg.numfon == 7  || cartcfg.numfon == 9  || cartcfg.numfon == 11  || cartcfg.numfon == 26 )
					{
						//Node6
						if( cartcfg.p_numfon == 1  || cartcfg.p_numfon == 2  || cartcfg.p_numfon == 12  || cartcfg.p_numfon == 13 )
						{
							//Node7
							if( cartcfg.n_vhgt == 1 || cartcfg.n_vhgt == 2)
							{
								//Node8
								if( cartcfg.pp_numfon == 23  || cartcfg.pp_numfon == 0  || cartcfg.pp_numfon == 1  || cartcfg.pp_numfon == 7  || cartcfg.pp_numfon == 28  || cartcfg.pp_numfon == 2  || cartcfg.pp_numfon == 19  || cartcfg.pp_numfon == 11  || cartcfg.pp_numfon == 3  || cartcfg.pp_numfon == 30  || cartcfg.pp_numfon == 12  || cartcfg.pp_numfon == 4  || cartcfg.pp_numfon == 6  || cartcfg.pp_numfon == 26  || cartcfg.pp_numfon == 5 )
								{
									//TerminalNode5
									mean=5.845000;
								}
								else
								{
									//TerminalNode6
									mean=6.565000;
								}
							}
							else
							{
								//Node11
								if( cartcfg.pp_numfon == 14  || cartcfg.pp_numfon == 25  || cartcfg.pp_numfon == 1  || cartcfg.pp_numfon == 28  || cartcfg.pp_numfon == 9  || cartcfg.pp_numfon == 2  || cartcfg.pp_numfon == 3  || cartcfg.pp_numfon == 17  || cartcfg.pp_numfon == 10  || cartcfg.pp_numfon == 12  || cartcfg.pp_numfon == 13  || cartcfg.pp_numfon == 4  || cartcfg.pp_numfon == 27  || cartcfg.pp_numfon == 15  || cartcfg.pp_numfon == 5 )
								{
									//TerminalNode27
									mean=6.606000;
								}
								else
								{
									//TerminalNode28
									mean=7.229000;
								}
							}
						}
						else
						{
							//Node12
							if( cartcfg.n_numfon == 14  || cartcfg.n_numfon == 1  || cartcfg.n_numfon == 7  || cartcfg.n_numfon == 2  || cartcfg.n_numfon == 11  || cartcfg.n_numfon == 3  || cartcfg.n_numfon == 30  || cartcfg.n_numfon == 4  || cartcfg.n_numfon == 26  || cartcfg.n_numfon == 5 )
							{
								//TerminalNode7
								mean=6.187000;
							}
							else
							{
								//TerminalNode8
								mean=6.939000;
							}
						}
					}
					else
					{
						//Node13
						if(cartcfg.npalutt <= 12.50)
						{
							//Node14
							if( cartcfg.n_numfon == 20  || cartcfg.n_numfon == 14  || cartcfg.n_numfon == 1  || cartcfg.n_numfon == 7  || cartcfg.n_numfon == 2  || cartcfg.n_numfon == 11  || cartcfg.n_numfon == 3  || cartcfg.n_numfon == 30  || cartcfg.n_numfon == 15 )
							{
								//Node15
								if( cartcfg.nn_numfon == 18  || cartcfg.nn_numfon == 20  || cartcfg.nn_numfon == 23  || cartcfg.nn_numfon == 25  || cartcfg.nn_numfon == 21  || cartcfg.nn_numfon == 1  || cartcfg.nn_numfon == 7  || cartcfg.nn_numfon == 9  || cartcfg.nn_numfon == 2  || cartcfg.nn_numfon == 29  || cartcfg.nn_numfon == 3  || cartcfg.nn_numfon == 17  || cartcfg.nn_numfon == 10  || cartcfg.nn_numfon == 24  || cartcfg.nn_numfon == 12  || cartcfg.nn_numfon == 4  || cartcfg.nn_numfon == 6  || cartcfg.nn_numfon == 26  || cartcfg.nn_numfon == 16  || cartcfg.nn_numfon == 8  || cartcfg.nn_numfon == 15  || cartcfg.nn_numfon == 5  || cartcfg.nn_numfon == 22 )
								{
									//TerminalNode23
									mean=6.299000;
								}
								else
								{
									//TerminalNode24
									mean=6.720000;
								}
							}
							else
							{
								//TerminalNode14
								mean=7.122000;
							}
						}
						else
						{
							//Node19
							if( cartcfg.n_numfon == 20  || cartcfg.n_numfon == 14  || cartcfg.n_numfon == 7  || cartcfg.n_numfon == 11  || cartcfg.n_numfon == 15 )
							{
								//TerminalNode15
								mean=3.447000;
							}
							else
							{
								//Node20
								if( cartcfg.n_numfon == 1  || cartcfg.n_numfon == 2  || cartcfg.n_numfon == 3 )
								{
									//TerminalNode50
									mean=7.279000;
								}
								else
								{
									//Node55
									if( cartcfg.npossil == 1  || cartcfg.npossil == 0 )
									{
										//Node56
										if( cartcfg.pp_numfon == 25  || cartcfg.pp_numfon == 0  || cartcfg.pp_numfon == 28  || cartcfg.pp_numfon == 2  || cartcfg.pp_numfon == 11  || cartcfg.pp_numfon == 3  || cartcfg.pp_numfon == 17  || cartcfg.pp_numfon == 10  || cartcfg.pp_numfon == 4  || cartcfg.pp_numfon == 26  || cartcfg.pp_numfon == 16  || cartcfg.pp_numfon == 15  || cartcfg.pp_numfon == 5  || cartcfg.pp_numfon == 31  || cartcfg.pp_numfon == 22 )
										{
											//TerminalNode51
											mean=7.053000;
										}
										else
										{
											//TerminalNode52
											mean=7.749000;
										}
									}
									else
									{
										//Node57
										if(cartcfg.ultimapal == 0)
										{
											//TerminalNode53
											mean=7.875000;
										}
										else
										{
											//TerminalNode54
											mean=8.771000;
										}
									}
								}
							}
						}
					}
				}
				else
				{
					//Node26
					if( cartcfg.p_numfon == 20  || cartcfg.p_numfon == 23  || cartcfg.p_numfon == 0  || cartcfg.p_numfon == 9  || cartcfg.p_numfon == 11  || cartcfg.p_numfon == 3  || cartcfg.p_numfon == 10  || cartcfg.p_numfon == 24  || cartcfg.p_numfon == 16  || cartcfg.p_numfon == 5  || cartcfg.p_numfon == 31 )
					{
						//Node27
						if( cartcfg.n_numfon == 14  || cartcfg.n_numfon == 1  || cartcfg.n_numfon == 7  || cartcfg.n_numfon == 2  || cartcfg.n_numfon == 11  || cartcfg.n_numfon == 3  || cartcfg.n_numfon == 30  || cartcfg.n_numfon == 15 )
						{
							//Node28
							if( cartcfg.pp_numfon == 18  || cartcfg.pp_numfon == 20  || cartcfg.pp_numfon == 23  || cartcfg.pp_numfon == 14  || cartcfg.pp_numfon == 21  || cartcfg.pp_numfon == 7  || cartcfg.pp_numfon == 9  || cartcfg.pp_numfon == 2  || cartcfg.pp_numfon == 19  || cartcfg.pp_numfon == 11  || cartcfg.pp_numfon == 10  || cartcfg.pp_numfon == 12  || cartcfg.pp_numfon == 6  || cartcfg.pp_numfon == 26  || cartcfg.pp_numfon == 16  || cartcfg.pp_numfon == 8  || cartcfg.pp_numfon == 15  || cartcfg.pp_numfon == 22 )
							{
								//TerminalNode47
								mean=6.716000;
							}
							else
							{
								//Node52
								if( cartcfg.n_numfon == 14  || cartcfg.n_numfon == 25  || cartcfg.n_numfon == 21  || cartcfg.n_numfon == 1  || cartcfg.n_numfon == 3  || cartcfg.n_numfon == 30  || cartcfg.n_numfon == 12  || cartcfg.n_numfon == 6  || cartcfg.n_numfon == 27  || cartcfg.n_numfon == 8  || cartcfg.n_numfon == 15  || cartcfg.n_numfon == 31  || cartcfg.n_numfon == 22 )
								{
									//Node21
									if( cartcfg.pp_numfon == 14  || cartcfg.pp_numfon == 21  || cartcfg.pp_numfon == 0  || cartcfg.pp_numfon == 7  || cartcfg.pp_numfon == 28  || cartcfg.pp_numfon == 2  || cartcfg.pp_numfon == 19  || cartcfg.pp_numfon == 11  || cartcfg.pp_numfon == 10  || cartcfg.pp_numfon == 13  || cartcfg.pp_numfon == 4  || cartcfg.pp_numfon == 6  || cartcfg.pp_numfon == 16  || cartcfg.pp_numfon == 8  || cartcfg.pp_numfon == 5  || cartcfg.pp_numfon == 22 )
									{
										//Node22
										if( cartcfg.nn_numfon == 18  || cartcfg.nn_numfon == 20  || cartcfg.nn_numfon == 14  || cartcfg.nn_numfon == 7  || cartcfg.nn_numfon == 9  || cartcfg.nn_numfon == 2  || cartcfg.nn_numfon == 29  || cartcfg.nn_numfon == 30  || cartcfg.nn_numfon == 17  || cartcfg.nn_numfon == 24  || cartcfg.nn_numfon == 12  || cartcfg.nn_numfon == 13  || cartcfg.nn_numfon == 4  || cartcfg.nn_numfon == 27  || cartcfg.nn_numfon == 16  || cartcfg.nn_numfon == 8  || cartcfg.nn_numfon == 5  || cartcfg.nn_numfon == 31 )
										{
											//TerminalNode16
											mean=5.561000;
										}
										else
										{
											//TerminalNode17
											mean=6.212000;
										}
									}
									else
									{
										//Node23
										if( cartcfg.nn_numfon == 18  || cartcfg.nn_numfon == 20  || cartcfg.nn_numfon == 14  || cartcfg.nn_numfon == 7  || cartcfg.nn_numfon == 9  || cartcfg.nn_numfon == 29  || cartcfg.nn_numfon == 19  || cartcfg.nn_numfon == 3  || cartcfg.nn_numfon == 30  || cartcfg.nn_numfon == 17  || cartcfg.nn_numfon == 24  || cartcfg.nn_numfon == 12  || cartcfg.nn_numfon == 6  || cartcfg.nn_numfon == 26  || cartcfg.nn_numfon == 16  || cartcfg.nn_numfon == 8  || cartcfg.nn_numfon == 31  || cartcfg.nn_numfon == 22 )
										{
											//TerminalNode18
											mean=6.046000;
										}
										else
										{
											//TerminalNode19
											mean=6.687000;
										}
									}
								}
								else
								{
									//Node24
									if(cartcfg.p_vrnd == 1)
									{
										//Node25
										if( cartcfg.nn_numfon == 18  || cartcfg.nn_numfon == 23  || cartcfg.nn_numfon == 14  || cartcfg.nn_numfon == 25  || cartcfg.nn_numfon == 21  || cartcfg.nn_numfon == 7  || cartcfg.nn_numfon == 9  || cartcfg.nn_numfon == 29  || cartcfg.nn_numfon == 3  || cartcfg.nn_numfon == 17  || cartcfg.nn_numfon == 10  || cartcfg.nn_numfon == 13  || cartcfg.nn_numfon == 4  || cartcfg.nn_numfon == 6  || cartcfg.nn_numfon == 26  || cartcfg.nn_numfon == 27  || cartcfg.nn_numfon == 8  || cartcfg.nn_numfon == 15  || cartcfg.nn_numfon == 5  || cartcfg.nn_numfon == 31 )
										{
											//TerminalNode20
											mean=4.176000;
										}
										else
										{
											//TerminalNode21
											mean=6.239000;
										}
									}
									else
									{
										//TerminalNode22
										mean=6.750000;
									}
								}
							}
						}
						else
						{
							//Node31
							if( cartcfg.nn_numfon == 18  || cartcfg.nn_numfon == 20  || cartcfg.nn_numfon == 23  || cartcfg.nn_numfon == 14  || cartcfg.nn_numfon == 25  || cartcfg.nn_numfon == 21  || cartcfg.nn_numfon == 7  || cartcfg.nn_numfon == 9  || cartcfg.nn_numfon == 2  || cartcfg.nn_numfon == 29  || cartcfg.nn_numfon == 19  || cartcfg.nn_numfon == 30  || cartcfg.nn_numfon == 17  || cartcfg.nn_numfon == 10  || cartcfg.nn_numfon == 12  || cartcfg.nn_numfon == 13  || cartcfg.nn_numfon == 4  || cartcfg.nn_numfon == 6  || cartcfg.nn_numfon == 26  || cartcfg.nn_numfon == 27  || cartcfg.nn_numfon == 16  || cartcfg.nn_numfon == 15  || cartcfg.nn_numfon == 5  || cartcfg.nn_numfon == 22 )
							{
								//Node32
								if( cartcfg.pp_numfon == 20  || cartcfg.pp_numfon == 21  || cartcfg.pp_numfon == 1  || cartcfg.pp_numfon == 7  || cartcfg.pp_numfon == 3  || cartcfg.pp_numfon == 30  || cartcfg.pp_numfon == 17  || cartcfg.pp_numfon == 10  || cartcfg.pp_numfon == 13  || cartcfg.pp_numfon == 6  || cartcfg.pp_numfon == 26  || cartcfg.pp_numfon == 27  || cartcfg.pp_numfon == 8  || cartcfg.pp_numfon == 5  || cartcfg.pp_numfon == 31  || cartcfg.pp_numfon == 22 )
								{
									//Node59
									if( cartcfg.nn_numfon == 14  || cartcfg.nn_numfon == 25  || cartcfg.nn_numfon == 1  || cartcfg.nn_numfon == 7  || cartcfg.nn_numfon == 2  || cartcfg.nn_numfon == 3  || cartcfg.nn_numfon == 30  || cartcfg.nn_numfon == 4  || cartcfg.nn_numfon == 15  || cartcfg.nn_numfon == 5  || cartcfg.nn_numfon == 31 )
									{
										//Node60
										if( cartcfg.n_numfon == 14  || cartcfg.n_numfon == 25  || cartcfg.n_numfon == 21  || cartcfg.n_numfon == 1  || cartcfg.n_numfon == 2  || cartcfg.n_numfon == 19  || cartcfg.n_numfon == 10  || cartcfg.n_numfon == 12  || cartcfg.n_numfon == 4  || cartcfg.n_numfon == 6  || cartcfg.n_numfon == 27  || cartcfg.n_numfon == 8  || cartcfg.n_numfon == 22 )
										{
											//TerminalNode55
											mean=6.813000;
										}
										else
										{
											//TerminalNode56
											mean=7.260000;
										}
									}
									else
									{
										//TerminalNode57
										mean=7.926000;
									}
								}
								else
								{
									//TerminalNode58
									mean=7.347000;
								}
							}
							else
							{
								//TerminalNode29
								mean=7.512000;
							}
						}
					}
					else
					{
						//TerminalNode30
						mean=7.453000;
					}
				}
			}
			else
			{
				//Node33
				if( cartcfg.numfon == 20  || cartcfg.numfon == 23  || cartcfg.numfon == 7  || cartcfg.numfon == 9  || cartcfg.numfon == 11 )
				{
					//Node34
					if( cartcfg.pp_numfon == 20  || cartcfg.pp_numfon == 23  || cartcfg.pp_numfon == 14  || cartcfg.pp_numfon == 25  || cartcfg.pp_numfon == 28  || cartcfg.pp_numfon == 2  || cartcfg.pp_numfon == 19  || cartcfg.pp_numfon == 11  || cartcfg.pp_numfon == 3  || cartcfg.pp_numfon == 30  || cartcfg.pp_numfon == 17  || cartcfg.pp_numfon == 12  || cartcfg.pp_numfon == 4  || cartcfg.pp_numfon == 26  || cartcfg.pp_numfon == 16  || cartcfg.pp_numfon == 8  || cartcfg.pp_numfon == 15  || cartcfg.pp_numfon == 31 )
					{
						//Node35
						if( cartcfg.n_numfon == 25  || cartcfg.n_numfon == 21  || cartcfg.n_numfon == 9  || cartcfg.n_numfon == 13  || cartcfg.n_numfon == 27  || cartcfg.n_numfon == 16 )
						{
							//TerminalNode31
							mean=4.327000;
						}
						else
						{
							//Node36
							if( cartcfg.pp_ct == 3 || cartcfg.pp_ct == 4 || cartcfg.pp_ct == 6 || cartcfg.pp_ct == 7)
							{
								//Node39
								if( cartcfg.p_numfon == 18  || cartcfg.p_numfon == 20  || cartcfg.p_numfon == 23  || cartcfg.p_numfon == 21  || cartcfg.p_numfon == 0  || cartcfg.p_numfon == 7  || cartcfg.p_numfon == 9  || cartcfg.p_numfon == 19  || cartcfg.p_numfon == 11  || cartcfg.p_numfon == 30  || cartcfg.p_numfon == 10  || cartcfg.p_numfon == 24  || cartcfg.p_numfon == 13  || cartcfg.p_numfon == 6  || cartcfg.p_numfon == 26  || cartcfg.p_numfon == 8  || cartcfg.p_numfon == 31 )
								{
									//TerminalNode35
									mean=5.229000;
								}
								else
								{
									//TerminalNode36
									mean=6.830000;
								}
							}
							else
							{
								//Node37
								if( cartcfg.nn_numfon == 18  || cartcfg.nn_numfon == 20  || cartcfg.nn_numfon == 23  || cartcfg.nn_numfon == 14  || cartcfg.nn_numfon == 25  || cartcfg.nn_numfon == 0  || cartcfg.nn_numfon == 7  || cartcfg.nn_numfon == 9  || cartcfg.nn_numfon == 29  || cartcfg.nn_numfon == 19  || cartcfg.nn_numfon == 30  || cartcfg.nn_numfon == 17  || cartcfg.nn_numfon == 24  || cartcfg.nn_numfon == 12  || cartcfg.nn_numfon == 13  || cartcfg.nn_numfon == 26  || cartcfg.nn_numfon == 27  || cartcfg.nn_numfon == 16  || cartcfg.nn_numfon == 8  || cartcfg.nn_numfon == 15  || cartcfg.nn_numfon == 5  || cartcfg.nn_numfon == 31  || cartcfg.nn_numfon == 22 )
								{
									//TerminalNode43
									mean=3.880000;
								}
								else
								{
									//TerminalNode44
									mean=7.410000;
								}
							}
						}
					}
					else
					{
						//Node40
						if( cartcfg.p_numfon == 18  || cartcfg.p_numfon == 23  || cartcfg.p_numfon == 21  || cartcfg.p_numfon == 0  || cartcfg.p_numfon == 7  || cartcfg.p_numfon == 9  || cartcfg.p_numfon == 2  || cartcfg.p_numfon == 19  || cartcfg.p_numfon == 11  || cartcfg.p_numfon == 30  || cartcfg.p_numfon == 10  || cartcfg.p_numfon == 24  || cartcfg.p_numfon == 12  || cartcfg.p_numfon == 13  || cartcfg.p_numfon == 6  || cartcfg.p_numfon == 26  || cartcfg.p_numfon == 16  || cartcfg.p_numfon == 8  || cartcfg.p_numfon == 31 )
						{
							//TerminalNode37
							mean=6.398000;
						}
						else
						{
							//Node41
							if(cartcfg.npalutt <= 12.50)
							{
								//TerminalNode38
								mean=8.300000;
							}
							else
							{
								//TerminalNode39
								mean=6.811000;
							}
						}
					}
				}
				else
				{
					//Node42
					if( cartcfg.n_numfon == 25  || cartcfg.n_numfon == 21  || cartcfg.n_numfon == 19  || cartcfg.n_numfon == 10  || cartcfg.n_numfon == 24  || cartcfg.n_numfon == 6  || cartcfg.n_numfon == 27  || cartcfg.n_numfon == 16  || cartcfg.n_numfon == 8  || cartcfg.n_numfon == 31  || cartcfg.n_numfon == 22 )
					{
						//TerminalNode40
						mean=7.123000;
					}
					else
					{
						//TerminalNode41
						mean=7.820000;
					}
				}
			}
		}
		else
		{
			//Node43
			if( cartcfg.n_numfon == 18  || cartcfg.n_numfon == 20  || cartcfg.n_numfon == 23  || cartcfg.n_numfon == 14  || cartcfg.n_numfon == 25  || cartcfg.n_numfon == 21  || cartcfg.n_numfon == 1  || cartcfg.n_numfon == 7  || cartcfg.n_numfon == 2  || cartcfg.n_numfon == 19  || cartcfg.n_numfon == 3  || cartcfg.n_numfon == 30  || cartcfg.n_numfon == 10  || cartcfg.n_numfon == 24  || cartcfg.n_numfon == 12  || cartcfg.n_numfon == 4  || cartcfg.n_numfon == 6  || cartcfg.n_numfon == 26  || cartcfg.n_numfon == 27  || cartcfg.n_numfon == 16  || cartcfg.n_numfon == 8  || cartcfg.n_numfon == 15  || cartcfg.n_numfon == 5  || cartcfg.n_numfon == 31  || cartcfg.n_numfon == 22 )
			{
				//Node44
				if( cartcfg.n_numfon == 20  || cartcfg.n_numfon == 7  || cartcfg.n_numfon == 24  || cartcfg.n_numfon == 26 )
				{
					//Node45
					if( cartcfg.numfon == 13 )
					{
						//Node46
						if(cartcfg.nsilutt <= 24.50)
						{
							//TerminalNode42
							mean=5.110000;
						}
						else
						{
							//Node47
							if( cartcfg.pp_numfon == 18  || cartcfg.pp_numfon == 20  || cartcfg.pp_numfon == 23  || cartcfg.pp_numfon == 14  || cartcfg.pp_numfon == 25  || cartcfg.pp_numfon == 21  || cartcfg.pp_numfon == 0  || cartcfg.pp_numfon == 1  || cartcfg.pp_numfon == 7  || cartcfg.pp_numfon == 28  || cartcfg.pp_numfon == 9  || cartcfg.pp_numfon == 2  || cartcfg.pp_numfon == 19  || cartcfg.pp_numfon == 11  || cartcfg.pp_numfon == 3  || cartcfg.pp_numfon == 30  || cartcfg.pp_numfon == 17  || cartcfg.pp_numfon == 10  || cartcfg.pp_numfon == 12  || cartcfg.pp_numfon == 13  || cartcfg.pp_numfon == 4  || cartcfg.pp_numfon == 6  || cartcfg.pp_numfon == 26  || cartcfg.pp_numfon == 27  || cartcfg.pp_numfon == 16  || cartcfg.pp_numfon == 8  || cartcfg.pp_numfon == 15  || cartcfg.pp_numfon == 5  || cartcfg.pp_numfon == 31  || cartcfg.pp_numfon == 22 )
							{
								//TerminalNode59
								mean=7.305000;
							}
							else
							{
								//TerminalNode60
								mean=7.829000;
							}
						}
					}
					else
					{
						//TerminalNode46
						mean=7.529000;
					}
				}
				else
				{
					//Node49
					if( cartcfg.p_numfon == 18  || cartcfg.p_numfon == 1  || cartcfg.p_numfon == 7  || cartcfg.p_numfon == 9  || cartcfg.p_numfon == 2  || cartcfg.p_numfon == 19  || cartcfg.p_numfon == 11  || cartcfg.p_numfon == 3  || cartcfg.p_numfon == 10  || cartcfg.p_numfon == 24  || cartcfg.p_numfon == 4  || cartcfg.p_numfon == 6  || cartcfg.p_numfon == 5 )
					{
						//Node50
						if( cartcfg.nn_numfon == 18  || cartcfg.nn_numfon == 20  || cartcfg.nn_numfon == 23  || cartcfg.nn_numfon == 21  || cartcfg.nn_numfon == 9  || cartcfg.nn_numfon == 29  || cartcfg.nn_numfon == 19  || cartcfg.nn_numfon == 17  || cartcfg.nn_numfon == 10  || cartcfg.nn_numfon == 24  || cartcfg.nn_numfon == 12  || cartcfg.nn_numfon == 13  || cartcfg.nn_numfon == 6  || cartcfg.nn_numfon == 26  || cartcfg.nn_numfon == 27  || cartcfg.nn_numfon == 16  || cartcfg.nn_numfon == 8  || cartcfg.nn_numfon == 22 )
						{
							//Node51
							if( cartcfg.pp_numfon == 18  || cartcfg.pp_numfon == 20  || cartcfg.pp_numfon == 23  || cartcfg.pp_numfon == 28  || cartcfg.pp_numfon == 9  || cartcfg.pp_numfon == 19  || cartcfg.pp_numfon == 11  || cartcfg.pp_numfon == 3  || cartcfg.pp_numfon == 17  || cartcfg.pp_numfon == 10  || cartcfg.pp_numfon == 12  || cartcfg.pp_numfon == 4  || cartcfg.pp_numfon == 15  || cartcfg.pp_numfon == 31 )
							{
								//Node9
								if( cartcfg.nn_numfon == 18  || cartcfg.nn_numfon == 20  || cartcfg.nn_numfon == 23  || cartcfg.nn_numfon == 14  || cartcfg.nn_numfon == 1  || cartcfg.nn_numfon == 7  || cartcfg.nn_numfon == 9  || cartcfg.nn_numfon == 2  || cartcfg.nn_numfon == 29  || cartcfg.nn_numfon == 19  || cartcfg.nn_numfon == 17  || cartcfg.nn_numfon == 10  || cartcfg.nn_numfon == 24  || cartcfg.nn_numfon == 12  || cartcfg.nn_numfon == 13  || cartcfg.nn_numfon == 4  || cartcfg.nn_numfon == 6  || cartcfg.nn_numfon == 27  || cartcfg.nn_numfon == 16  || cartcfg.nn_numfon == 8  || cartcfg.nn_numfon == 15  || cartcfg.nn_numfon == 5  || cartcfg.nn_numfon == 31  || cartcfg.nn_numfon == 22 )
								{
									//TerminalNode1
									mean=5.378000;
								}
								else
								{
									//TerminalNode2
									mean=5.824000;
								}
							}
							else
							{
								//Node10
								if( cartcfg.nn_numfon == 18  || cartcfg.nn_numfon == 20  || cartcfg.nn_numfon == 21  || cartcfg.nn_numfon == 7  || cartcfg.nn_numfon == 9  || cartcfg.nn_numfon == 29  || cartcfg.nn_numfon == 19  || cartcfg.nn_numfon == 10  || cartcfg.nn_numfon == 24  || cartcfg.nn_numfon == 12  || cartcfg.nn_numfon == 13  || cartcfg.nn_numfon == 6  || cartcfg.nn_numfon == 16  || cartcfg.nn_numfon == 15  || cartcfg.nn_numfon == 22 )
								{
									//TerminalNode3
									mean=5.672000;
								}
								else
								{
									//TerminalNode4
									mean=6.179000;
								}
							}
						}
						else
						{
							//Node53
							if(cartcfg.nsilutt <= 26.50)
							{
								//Node54
								if( cartcfg.n_numfon == 18  || cartcfg.n_numfon == 14  || cartcfg.n_numfon == 21  || cartcfg.n_numfon == 1  || cartcfg.n_numfon == 2  || cartcfg.n_numfon == 19  || cartcfg.n_numfon == 3  || cartcfg.n_numfon == 30  || cartcfg.n_numfon == 12  || cartcfg.n_numfon == 6  || cartcfg.n_numfon == 27  || cartcfg.n_numfon == 16  || cartcfg.n_numfon == 5  || cartcfg.n_numfon == 31  || cartcfg.n_numfon == 22 )
								{
									//TerminalNode48
									mean=6.854000;
								}
								else
								{
									//TerminalNode49
									mean=7.170000;
								}
							}
							else
							{
								//Node58
								if( cartcfg.pp_numfon == 20  || cartcfg.pp_numfon == 25  || cartcfg.pp_numfon == 0  || cartcfg.pp_numfon == 1  || cartcfg.pp_numfon == 7  || cartcfg.pp_numfon == 28  || cartcfg.pp_numfon == 9  || cartcfg.pp_numfon == 2  || cartcfg.pp_numfon == 11  || cartcfg.pp_numfon == 3  || cartcfg.pp_numfon == 30  || cartcfg.pp_numfon == 10  || cartcfg.pp_numfon == 12  || cartcfg.pp_numfon == 13  || cartcfg.pp_numfon == 4  || cartcfg.pp_numfon == 6  || cartcfg.pp_numfon == 31 )
								{
									//Node29
									if( cartcfg.nn_numfon == 20  || cartcfg.nn_numfon == 23  || cartcfg.nn_numfon == 14  || cartcfg.nn_numfon == 21  || cartcfg.nn_numfon == 1  || cartcfg.nn_numfon == 7  || cartcfg.nn_numfon == 9  || cartcfg.nn_numfon == 2  || cartcfg.nn_numfon == 29  || cartcfg.nn_numfon == 19  || cartcfg.nn_numfon == 17  || cartcfg.nn_numfon == 10  || cartcfg.nn_numfon == 24  || cartcfg.nn_numfon == 12  || cartcfg.nn_numfon == 13  || cartcfg.nn_numfon == 6  || cartcfg.nn_numfon == 26  || cartcfg.nn_numfon == 5 )
									{
										//TerminalNode32
										mean=3.345000;
									}
									else
									{
										//Node38
										if( cartcfg.n_numfon == 18  || cartcfg.n_numfon == 23  || cartcfg.n_numfon == 17  || cartcfg.n_numfon == 10  || cartcfg.n_numfon == 24  || cartcfg.n_numfon == 12  || cartcfg.n_numfon == 6  || cartcfg.n_numfon == 22 )
										{
											//TerminalNode33
											mean=4.723000;
										}
										else
										{
											//TerminalNode34
											mean=6.685000;
										}
									}
								}
								else
								{
									//Node30
									if( cartcfg.numfon == 9  || cartcfg.numfon == 11  || cartcfg.numfon == 26 )
									{
										//TerminalNode25
										mean=6.487000;
									}
									else
									{
										//TerminalNode26
										mean=6.916000;
									}
								}
							}
						}
					}
					else
					{
						//Node61
						if( cartcfg.pp_numfon == 18  || cartcfg.pp_numfon == 20  || cartcfg.pp_numfon == 23  || cartcfg.pp_numfon == 14  || cartcfg.pp_numfon == 25  || cartcfg.pp_numfon == 21  || cartcfg.pp_numfon == 0  || cartcfg.pp_numfon == 7  || cartcfg.pp_numfon == 9  || cartcfg.pp_numfon == 19  || cartcfg.pp_numfon == 11  || cartcfg.pp_numfon == 30  || cartcfg.pp_numfon == 17  || cartcfg.pp_numfon == 10  || cartcfg.pp_numfon == 24  || cartcfg.pp_numfon == 13  || cartcfg.pp_numfon == 4  || cartcfg.pp_numfon == 6  || cartcfg.pp_numfon == 26  || cartcfg.pp_numfon == 27  || cartcfg.pp_numfon == 16  || cartcfg.pp_numfon == 8  || cartcfg.pp_numfon == 15  || cartcfg.pp_numfon == 22 )
						{
							//Node48
							if( cartcfg.nn_numfon == 18  || cartcfg.nn_numfon == 20  || cartcfg.nn_numfon == 23  || cartcfg.nn_numfon == 14  || cartcfg.nn_numfon == 25  || cartcfg.nn_numfon == 21  || cartcfg.nn_numfon == 0  || cartcfg.nn_numfon == 1  || cartcfg.nn_numfon == 7  || cartcfg.nn_numfon == 9  || cartcfg.nn_numfon == 2  || cartcfg.nn_numfon == 29  || cartcfg.nn_numfon == 19  || cartcfg.nn_numfon == 17  || cartcfg.nn_numfon == 10  || cartcfg.nn_numfon == 24  || cartcfg.nn_numfon == 12  || cartcfg.nn_numfon == 13  || cartcfg.nn_numfon == 4  || cartcfg.nn_numfon == 6  || cartcfg.nn_numfon == 26  || cartcfg.nn_numfon == 27  || cartcfg.nn_numfon == 16  || cartcfg.nn_numfon == 8  || cartcfg.nn_numfon == 15  || cartcfg.nn_numfon == 5  || cartcfg.nn_numfon == 31  || cartcfg.nn_numfon == 22 )
							{
								//Node16
								if( cartcfg.npossil == 2 )
								{
									//Node17
									if( cartcfg.pp_numfon == 20  || cartcfg.pp_numfon == 23  || cartcfg.pp_numfon == 14  || cartcfg.pp_numfon == 7  || cartcfg.pp_numfon == 28  || cartcfg.pp_numfon == 9  || cartcfg.pp_numfon == 2  || cartcfg.pp_numfon == 19  || cartcfg.pp_numfon == 11  || cartcfg.pp_numfon == 30  || cartcfg.pp_numfon == 10  || cartcfg.pp_numfon == 12  || cartcfg.pp_numfon == 4  || cartcfg.pp_numfon == 6  || cartcfg.pp_numfon == 26  || cartcfg.pp_numfon == 16  || cartcfg.pp_numfon == 15  || cartcfg.pp_numfon == 5  || cartcfg.pp_numfon == 31 )
									{
										//Node18
										if( cartcfg.nn_numfon == 20  || cartcfg.nn_numfon == 23  || cartcfg.nn_numfon == 7  || cartcfg.nn_numfon == 9  || cartcfg.nn_numfon == 2  || cartcfg.nn_numfon == 29  || cartcfg.nn_numfon == 3  || cartcfg.nn_numfon == 17  || cartcfg.nn_numfon == 10  || cartcfg.nn_numfon == 12  || cartcfg.nn_numfon == 26  || cartcfg.nn_numfon == 8  || cartcfg.nn_numfon == 15 )
										{
											//TerminalNode9
											mean=5.019000;
										}
										else
										{
											//TerminalNode10
											mean=6.249000;
										}
									}
									else
									{
										//TerminalNode11
										mean=6.579000;
									}
								}
								else
								{
									//TerminalNode12
									mean=6.666000;
								}
							}
							else
							{
								//TerminalNode13
								mean=7.350000;
							}
						}
						else
						{
							//TerminalNode45
							mean=10.198000;
						}
					}
				}
			}
			else
			{
				//Node62
				if( cartcfg.nn_numfon == 18  || cartcfg.nn_numfon == 20  || cartcfg.nn_numfon == 23  || cartcfg.nn_numfon == 14  || cartcfg.nn_numfon == 25  || cartcfg.nn_numfon == 21  || cartcfg.nn_numfon == 0  || cartcfg.nn_numfon == 7  || cartcfg.nn_numfon == 2  || cartcfg.nn_numfon == 19  || cartcfg.nn_numfon == 3  || cartcfg.nn_numfon == 12  || cartcfg.nn_numfon == 4  || cartcfg.nn_numfon == 26  || cartcfg.nn_numfon == 16  || cartcfg.nn_numfon == 15  || cartcfg.nn_numfon == 31  || cartcfg.nn_numfon == 22 )
				{
					//TerminalNode61
					mean=8.449000;
				}
				else
				{
					//TerminalNode62
					mean=8.971000;
				}
			}
		}
	}


	else
	{
		//Node63
		if( cartcfg.numfon == 12 )
		{
			//Node64
			if( cartcfg.n_numfon == 18  || cartcfg.n_numfon == 20  || cartcfg.n_numfon == 23  || cartcfg.n_numfon == 14  || cartcfg.n_numfon == 25  || cartcfg.n_numfon == 21  || cartcfg.n_numfon == 1  || cartcfg.n_numfon == 9  || cartcfg.n_numfon == 2  || cartcfg.n_numfon == 19  || cartcfg.n_numfon == 11  || cartcfg.n_numfon == 3  || cartcfg.n_numfon == 17  || cartcfg.n_numfon == 10  || cartcfg.n_numfon == 12  || cartcfg.n_numfon == 13  || cartcfg.n_numfon == 6  || cartcfg.n_numfon == 26  || cartcfg.n_numfon == 27  || cartcfg.n_numfon == 15  || cartcfg.n_numfon == 22 )
			{
				//Node65
				if( cartcfg.n_vhgt == 0 || cartcfg.n_vhgt == 2)
				{
					//Node66
					if( cartcfg.p_numfon == 18  || cartcfg.p_numfon == 20  || cartcfg.p_numfon == 23  || cartcfg.p_numfon == 0  || cartcfg.p_numfon == 1  || cartcfg.p_numfon == 7  || cartcfg.p_numfon == 9  || cartcfg.p_numfon == 2  || cartcfg.p_numfon == 19  || cartcfg.p_numfon == 11  || cartcfg.p_numfon == 3  || cartcfg.p_numfon == 10  || cartcfg.p_numfon == 12  || cartcfg.p_numfon == 13  || cartcfg.p_numfon == 4  || cartcfg.p_numfon == 6  || cartcfg.p_numfon == 16  || cartcfg.p_numfon == 8  || cartcfg.p_numfon == 5 )
					{
						//TerminalNode63
						mean=7.451000;
					}
					else
					{
						//TerminalNode64
						mean=8.199000;
					}
				}
				else
				{
					//Node67
					if( cartcfg.p_numfon == 20  || cartcfg.p_numfon == 23  || cartcfg.p_numfon == 21  || cartcfg.p_numfon == 1  || cartcfg.p_numfon == 7  || cartcfg.p_numfon == 9  || cartcfg.p_numfon == 2  || cartcfg.p_numfon == 19  || cartcfg.p_numfon == 11  || cartcfg.p_numfon == 3  || cartcfg.p_numfon == 30  || cartcfg.p_numfon == 10  || cartcfg.p_numfon == 12  || cartcfg.p_numfon == 13  || cartcfg.p_numfon == 4  || cartcfg.p_numfon == 6  || cartcfg.p_numfon == 16  || cartcfg.p_numfon == 8  || cartcfg.p_numfon == 31 )
					{
						//TerminalNode65
						mean=7.864000;
					}
					else
					{
						//TerminalNode66
						mean=8.378000;
					}
				}
			}
			else
			{
				//Node68
				if( cartcfg.nn_numfon == 18  || cartcfg.nn_numfon == 14  || cartcfg.nn_numfon == 25  || cartcfg.nn_numfon == 21  || cartcfg.nn_numfon == 7  || cartcfg.nn_numfon == 9  || cartcfg.nn_numfon == 2  || cartcfg.nn_numfon == 29  || cartcfg.nn_numfon == 19  || cartcfg.nn_numfon == 30  || cartcfg.nn_numfon == 17  || cartcfg.nn_numfon == 10  || cartcfg.nn_numfon == 12  || cartcfg.nn_numfon == 6  || cartcfg.nn_numfon == 27  || cartcfg.nn_numfon == 16  || cartcfg.nn_numfon == 8  || cartcfg.nn_numfon == 5 )
				{
					//TerminalNode67
					mean=8.189000;
				}
				else
				{
					//TerminalNode68
					mean=8.702000;
				}
			}
		}
		else
		{
			//Node69
			if( cartcfg.n_numfon == 18  || cartcfg.n_numfon == 20  || cartcfg.n_numfon == 23  || cartcfg.n_numfon == 14  || cartcfg.n_numfon == 25  || cartcfg.n_numfon == 21  || cartcfg.n_numfon == 0  || cartcfg.n_numfon == 1  || cartcfg.n_numfon == 7  || cartcfg.n_numfon == 9  || cartcfg.n_numfon == 2  || cartcfg.n_numfon == 19  || cartcfg.n_numfon == 11  || cartcfg.n_numfon == 30  || cartcfg.n_numfon == 17  || cartcfg.n_numfon == 10  || cartcfg.n_numfon == 24  || cartcfg.n_numfon == 12  || cartcfg.n_numfon == 13  || cartcfg.n_numfon == 6  || cartcfg.n_numfon == 26  || cartcfg.n_numfon == 27  || cartcfg.n_numfon == 16  || cartcfg.n_numfon == 8  || cartcfg.n_numfon == 15  || cartcfg.n_numfon == 22 )
			{
				//Node70
				if( cartcfg.nn_numfon == 18  || cartcfg.nn_numfon == 20  || cartcfg.nn_numfon == 23  || cartcfg.nn_numfon == 14  || cartcfg.nn_numfon == 25  || cartcfg.nn_numfon == 21  || cartcfg.nn_numfon == 1  || cartcfg.nn_numfon == 7  || cartcfg.nn_numfon == 2  || cartcfg.nn_numfon == 29  || cartcfg.nn_numfon == 19  || cartcfg.nn_numfon == 30  || cartcfg.nn_numfon == 10  || cartcfg.nn_numfon == 24  || cartcfg.nn_numfon == 12  || cartcfg.nn_numfon == 4  || cartcfg.nn_numfon == 6  || cartcfg.nn_numfon == 26  || cartcfg.nn_numfon == 27  || cartcfg.nn_numfon == 16  || cartcfg.nn_numfon == 8  || cartcfg.nn_numfon == 15  || cartcfg.nn_numfon == 5  || cartcfg.nn_numfon == 31  || cartcfg.nn_numfon == 22 )
				{
					//Node71
					if( cartcfg.pp_numfon == 20  || cartcfg.pp_numfon == 14  || cartcfg.pp_numfon == 25  || cartcfg.pp_numfon == 0  || cartcfg.pp_numfon == 1  || cartcfg.pp_numfon == 7  || cartcfg.pp_numfon == 19  || cartcfg.pp_numfon == 11  || cartcfg.pp_numfon == 3  || cartcfg.pp_numfon == 17  || cartcfg.pp_numfon == 10  || cartcfg.pp_numfon == 24  || cartcfg.pp_numfon == 12  || cartcfg.pp_numfon == 4  || cartcfg.pp_numfon == 27  || cartcfg.pp_numfon == 16  || cartcfg.pp_numfon == 8  || cartcfg.pp_numfon == 15  || cartcfg.pp_numfon == 22 )
					{
						//TerminalNode69
						mean=8.503000;
					}
					else
					{
						//TerminalNode70
						mean=9.059000;
					}
				}
				else
				{
					//TerminalNode71
					mean=9.609000;
				}
			}
			else
			{
				//Node72
				if( cartcfg.nn_numfon == 18  || cartcfg.nn_numfon == 20  || cartcfg.nn_numfon == 23  || cartcfg.nn_numfon == 25  || cartcfg.nn_numfon == 21  || cartcfg.nn_numfon == 1  || cartcfg.nn_numfon == 7  || cartcfg.nn_numfon == 9  || cartcfg.nn_numfon == 2  || cartcfg.nn_numfon == 29  || cartcfg.nn_numfon == 30  || cartcfg.nn_numfon == 17  || cartcfg.nn_numfon == 10  || cartcfg.nn_numfon == 24  || cartcfg.nn_numfon == 12  || cartcfg.nn_numfon == 13  || cartcfg.nn_numfon == 6  || cartcfg.nn_numfon == 26  || cartcfg.nn_numfon == 27  || cartcfg.nn_numfon == 16  || cartcfg.nn_numfon == 8  || cartcfg.nn_numfon == 15  || cartcfg.nn_numfon == 5  || cartcfg.nn_numfon == 31  || cartcfg.nn_numfon == 22 )
				{
					//Node73
					if( cartcfg.numfon == 25  || cartcfg.numfon == 17 )
					{
						//TerminalNode72
						mean=9.084000;
					}
					else
					{
						//TerminalNode73
						mean=9.711000;
					}
				}
				else
				{
					//TerminalNode74
					mean=10.477000;
				}
			}
		}
	}
}
//VOCALES
else if( cartcfg.numfon == 1 ||  cartcfg.numfon == 2 || cartcfg.numfon == 3 || cartcfg.numfon == 4 || cartcfg.numfon == 5 /*|| cartcfg.numfon == 30 || cartcfg.numfon == 31*/)
{
/*CLASSIFICATION AND REGRESSION TREE*/
//Node1
if( cartcfg.nn_numfon == 18  || cartcfg.nn_numfon == 20  || cartcfg.nn_numfon == 23  || cartcfg.nn_numfon == 14  || cartcfg.nn_numfon == 25  || cartcfg.nn_numfon == 21  || cartcfg.nn_numfon == 1  || cartcfg.nn_numfon == 7  || cartcfg.nn_numfon == 9  || cartcfg.nn_numfon == 2  || cartcfg.nn_numfon == 19  || cartcfg.nn_numfon == 11  || cartcfg.nn_numfon == 3  || cartcfg.nn_numfon == 30  || cartcfg.nn_numfon == 17  || cartcfg.nn_numfon == 10  || cartcfg.nn_numfon == 24  || cartcfg.nn_numfon == 12  || cartcfg.nn_numfon == 13  || cartcfg.nn_numfon == 4  || cartcfg.nn_numfon == 6  || cartcfg.nn_numfon == 26  || cartcfg.nn_numfon == 27  || cartcfg.nn_numfon == 16  || cartcfg.nn_numfon == 8  || cartcfg.nn_numfon == 15  || cartcfg.nn_numfon == 5  || cartcfg.nn_numfon == 31  || cartcfg.nn_numfon == 22 )
{
	//Node2
	if( cartcfg.n_numfon == 1  || cartcfg.n_numfon == 2  || cartcfg.n_numfon == 19  || cartcfg.n_numfon == 3  || cartcfg.n_numfon == 30  || cartcfg.n_numfon == 17  || cartcfg.n_numfon == 10  || cartcfg.n_numfon == 24  || cartcfg.n_numfon == 12  || cartcfg.n_numfon == 13  || cartcfg.n_numfon == 6  || cartcfg.n_numfon == 26  || cartcfg.n_numfon == 16  || cartcfg.n_numfon == 5  || cartcfg.n_numfon == 31  || cartcfg.n_numfon == 22 )
	{
		//Node3
		if(cartcfg.str == 0)
		{
			//Node4
			if( cartcfg.p_numfon == 20  || cartcfg.p_numfon == 14  || cartcfg.p_numfon == 1  || cartcfg.p_numfon == 9  || cartcfg.p_numfon == 2  || cartcfg.p_numfon == 19  || cartcfg.p_numfon == 3  || cartcfg.p_numfon == 30  || cartcfg.p_numfon == 17  || cartcfg.p_numfon == 10  || cartcfg.p_numfon == 24  || cartcfg.p_numfon == 4  || cartcfg.p_numfon == 6  || cartcfg.p_numfon == 26  || cartcfg.p_numfon == 27  || cartcfg.p_numfon == 16  || cartcfg.p_numfon == 8  || cartcfg.p_numfon == 15  || cartcfg.p_numfon == 31  || cartcfg.p_numfon == 22 )
			{
				//Node5
				if( cartcfg.numfon == 2 )
				{
					//Node6
					if( cartcfg.pp_numfon == 18  || cartcfg.pp_numfon == 20  || cartcfg.pp_numfon == 23  || cartcfg.pp_numfon == 14  || cartcfg.pp_numfon == 21  || cartcfg.pp_numfon == 28  || cartcfg.pp_numfon == 9  || cartcfg.pp_numfon == 19  || cartcfg.pp_numfon == 17  || cartcfg.pp_numfon == 10  || cartcfg.pp_numfon == 6  || cartcfg.pp_numfon == 16  || cartcfg.pp_numfon == 8  || cartcfg.pp_numfon == 15  || cartcfg.pp_numfon == 31  || cartcfg.pp_numfon == 22 )
					{
						//Node68
						if(cartcfg.vfrt == 1)
						{
							//TerminalNode64
							mean=7.708000;
						}
						else
						{
							//TerminalNode65
							mean=8.170000;
						}
					}
					else
					{
						//Node69
						if(cartcfg.npalutt <= 9.50)
						{
							//73
							if( cartcfg.pp_numfon == 18  || cartcfg.pp_numfon == 20  || cartcfg.pp_numfon == 23  || cartcfg.pp_numfon == 14  || cartcfg.pp_numfon == 21  || cartcfg.pp_numfon == 1  || cartcfg.pp_numfon == 7  || cartcfg.pp_numfon == 28  || cartcfg.pp_numfon == 9  || cartcfg.pp_numfon == 19  || cartcfg.pp_numfon == 11  || cartcfg.pp_numfon == 10  || cartcfg.pp_numfon == 12  || cartcfg.pp_numfon == 13  || cartcfg.pp_numfon == 4  || cartcfg.pp_numfon == 6  || cartcfg.pp_numfon == 16  || cartcfg.pp_numfon == 8  || cartcfg.pp_numfon == 15  || cartcfg.pp_numfon == 5  || cartcfg.pp_numfon == 31  || cartcfg.pp_numfon == 22 )
							{
								//Node28
								if( cartcfg.n_numfon == 1  || cartcfg.n_numfon == 2  || cartcfg.n_numfon == 3  || cartcfg.n_numfon == 24  || cartcfg.n_numfon == 12  || cartcfg.n_numfon == 26  || cartcfg.n_numfon == 5  || cartcfg.n_numfon == 31 )
								{
									//Node29
									if(cartcfg.vhgt == 2)
									{
										//Node30
										if( cartcfg.nn_numfon == 14  || cartcfg.nn_numfon == 21  || cartcfg.nn_numfon == 11  || cartcfg.nn_numfon == 24  || cartcfg.nn_numfon == 12  || cartcfg.nn_numfon == 13  || cartcfg.nn_numfon == 15  || cartcfg.nn_numfon == 22 )
										{
											//Node31
											if( cartcfg.pp_numfon == 23  || cartcfg.pp_numfon == 14  || cartcfg.pp_numfon == 9  || cartcfg.pp_numfon == 19  || cartcfg.pp_numfon == 30  || cartcfg.pp_numfon == 24  || cartcfg.pp_numfon == 13  || cartcfg.pp_numfon == 27  || cartcfg.pp_numfon == 15  || cartcfg.pp_numfon == 31  || cartcfg.pp_numfon == 22 )
											{
												//TerminalNode24
												mean=5.039000;
											}
											else
											{
												//Node32
												if( cartcfg.n_numfon == 2  || cartcfg.n_numfon == 3  || cartcfg.n_numfon == 26 )
												{
													//TerminalNode25
													mean=5.886000;
												}
												else
												{
													//TerminalNode26
													mean=6.813000;
												}
											}
										}
										else
										{
											//Node33
											if( cartcfg.p_numfon == 14  || cartcfg.p_numfon == 9  || cartcfg.p_numfon == 3  || cartcfg.p_numfon == 30  || cartcfg.p_numfon == 10  || cartcfg.p_numfon == 4  || cartcfg.p_numfon == 6  || cartcfg.p_numfon == 16  || cartcfg.p_numfon == 15  || cartcfg.p_numfon == 31 )
											{
												//TerminalNode27
												mean=6.595000;
											}
											else
											{
												//Node34
												if( cartcfg.n_numfon == 1  || cartcfg.n_numfon == 2  || cartcfg.n_numfon == 3  || cartcfg.n_numfon == 24  || cartcfg.n_numfon == 12  || cartcfg.n_numfon == 5  || cartcfg.n_numfon == 31 )
												{
													//TerminalNode28
													mean=6.986000;
												}
												else
												{
													//TerminalNode29
													mean=7.902000;
												}
											}
										}
									}
									else
									{
										//Node35
										if( cartcfg.p_numfon == 1  || cartcfg.p_numfon == 3  || cartcfg.p_numfon == 30  || cartcfg.p_numfon == 17  || cartcfg.p_numfon == 24  || cartcfg.p_numfon == 8 )
										{
											//Node36
											if( cartcfg.n_numfon == 1  || cartcfg.n_numfon == 2 )
											{
												//Node37
												if( cartcfg.numfon == 1  || cartcfg.numfon == 4  || cartcfg.numfon == 5 )
												{
													//TerminalNode30
													mean=5.774000;
												}
												else
												{
													//TerminalNode31
													mean=8.936000;
												}
											}
											else
											{
												//Node38
												if( cartcfg.pp_numfon == 18  || cartcfg.pp_numfon == 20  || cartcfg.pp_numfon == 23  || cartcfg.pp_numfon == 0  || cartcfg.pp_numfon == 1  || cartcfg.pp_numfon == 9  || cartcfg.pp_numfon == 3  || cartcfg.pp_numfon == 30  || cartcfg.pp_numfon == 24  || cartcfg.pp_numfon == 13  || cartcfg.pp_numfon == 26  || cartcfg.pp_numfon == 27  || cartcfg.pp_numfon == 15  || cartcfg.pp_numfon == 31  || cartcfg.pp_numfon == 22 )
												{
													//TerminalNode32
													mean=6.772000;
												}
												else
												{
													//TerminalNode33
													mean=7.420000;
												}
											}
										}
										else
										{
											//Node39
											if(cartcfg.ultimasil == 0)
											{
												//Node40
												if( cartcfg.p_numfon == 9  || cartcfg.p_numfon == 2  || cartcfg.p_numfon == 19  || cartcfg.p_numfon == 10  || cartcfg.p_numfon == 6  || cartcfg.p_numfon == 26  || cartcfg.p_numfon == 22 )
												{
													//Node41
													if(cartcfg.n_vhgt == 2)
													{
														//TerminalNode34
														mean=5.608000;
													}
													else
													{
														//TerminalNode35
														mean=6.894000;
													}
												}
												else
												{
													//Node42
													if( cartcfg.nn_numfon == 18  || cartcfg.nn_numfon == 20  || cartcfg.nn_numfon == 23  || cartcfg.nn_numfon == 14  || cartcfg.nn_numfon == 1  || cartcfg.nn_numfon == 9  || cartcfg.nn_numfon == 2  || cartcfg.nn_numfon == 19  || cartcfg.nn_numfon == 11  || cartcfg.nn_numfon == 3  || cartcfg.nn_numfon == 10  || cartcfg.nn_numfon == 12  || cartcfg.nn_numfon == 13  || cartcfg.nn_numfon == 4  || cartcfg.nn_numfon == 6  || cartcfg.nn_numfon == 27  || cartcfg.nn_numfon == 8  || cartcfg.nn_numfon == 15  || cartcfg.nn_numfon == 5  || cartcfg.nn_numfon == 31  || cartcfg.nn_numfon == 22 )
													{
														//Node43
														if(cartcfg.vrnd == 2)
														{
															//TerminalNode36
															mean=6.951000;
														}
														else
														{
															//Node44
															if(cartcfg.nsilutt <= 19.50)
															{
																//TerminalNode37
																mean=6.027000;
															}
															else
															{
																//TerminalNode38
																mean=8.608000;
															}
														}
													}
													else
													{
														//TerminalNode39
														mean=8.140000;
													}
												}
											}
											else
											{
												//Node45
												if( cartcfg.nn_numfon == 18  || cartcfg.nn_numfon == 23  || cartcfg.nn_numfon == 14  || cartcfg.nn_numfon == 21  || cartcfg.nn_numfon == 1  || cartcfg.nn_numfon == 7  || cartcfg.nn_numfon == 2  || cartcfg.nn_numfon == 19  || cartcfg.nn_numfon == 11  || cartcfg.nn_numfon == 3  || cartcfg.nn_numfon == 10  || cartcfg.nn_numfon == 12  || cartcfg.nn_numfon == 4  || cartcfg.nn_numfon == 6  || cartcfg.nn_numfon == 26  || cartcfg.nn_numfon == 15  || cartcfg.nn_numfon == 5  || cartcfg.nn_numfon == 31 )
												{
													//Node46
													if( cartcfg.pp_numfon == 18  || cartcfg.pp_numfon == 20  || cartcfg.pp_numfon == 23  || cartcfg.pp_numfon == 0  || cartcfg.pp_numfon == 1  || cartcfg.pp_numfon == 3  || cartcfg.pp_numfon == 12  || cartcfg.pp_numfon == 27  || cartcfg.pp_numfon == 15  || cartcfg.pp_numfon == 31  || cartcfg.pp_numfon == 22 )
													{
														//TerminalNode40
														mean=7.033000;
													}
													else
													{
														//TerminalNode41
														mean=7.678000;
													}
												}
												else
												{
													//Node47
													if( cartcfg.n_numfon == 2  || cartcfg.n_numfon == 12  || cartcfg.n_numfon == 5  || cartcfg.n_numfon == 31 )
													{
														//TerminalNode42
														mean=7.370000;
													}
													else
													{
														//Node48
														if( cartcfg.p_numfon == 14  || cartcfg.p_numfon == 19  || cartcfg.p_numfon == 10  || cartcfg.p_numfon == 6  || cartcfg.p_numfon == 16  || cartcfg.p_numfon == 15  || cartcfg.p_numfon == 31  || cartcfg.p_numfon == 22 )
														{
															//TerminalNode43
															mean=7.801000;
														}
														else
														{
															//TerminalNode44
															mean=9.016000;
														}
													}
												}
											}
										}
									}
								}
								else
								{
									//Node49
									if( cartcfg.numfon == 3  || cartcfg.numfon == 4 )
									{
										//Node50
										if( cartcfg.p_numfon == 14  || cartcfg.p_numfon == 9  || cartcfg.p_numfon == 19  || cartcfg.p_numfon == 30  || cartcfg.p_numfon == 10  || cartcfg.p_numfon == 24  || cartcfg.p_numfon == 4  || cartcfg.p_numfon == 16  || cartcfg.p_numfon == 8  || cartcfg.p_numfon == 15  || cartcfg.p_numfon == 31  || cartcfg.p_numfon == 22 )
										{
											//Node51
											if( cartcfg.nn_numfon == 23  || cartcfg.nn_numfon == 21  || cartcfg.nn_numfon == 1  || cartcfg.nn_numfon == 7  || cartcfg.nn_numfon == 9  || cartcfg.nn_numfon == 2  || cartcfg.nn_numfon == 19  || cartcfg.nn_numfon == 11  || cartcfg.nn_numfon == 3  || cartcfg.nn_numfon == 30  || cartcfg.nn_numfon == 10  || cartcfg.nn_numfon == 24  || cartcfg.nn_numfon == 4  || cartcfg.nn_numfon == 6  || cartcfg.nn_numfon == 26  || cartcfg.nn_numfon == 27  || cartcfg.nn_numfon == 16  || cartcfg.nn_numfon == 8  || cartcfg.nn_numfon == 5 )
											{
												//Node52
												if( cartcfg.n_numfon == 30  || cartcfg.n_numfon == 10  || cartcfg.n_numfon == 13  || cartcfg.n_numfon == 6 )
												{
													//Node53
													if( cartcfg.p_numfon == 14  || cartcfg.p_numfon == 4  || cartcfg.p_numfon == 16  || cartcfg.p_numfon == 31  || cartcfg.p_numfon == 22 )
													{
														//TerminalNode45
														mean=6.438000;
													}
													else
													{
														//TerminalNode46
														mean=6.966000;
													}
												}
												else
												{
													//TerminalNode47
													mean=7.153000;
												}
											}
											else
											{
												//TerminalNode48
												mean=7.645000;
											}
										}
										else
										{
											//TerminalNode49
											mean=7.589000;
										}
									}
									else
									{
										//Node54
										if( cartcfg.p_numfon == 1  || cartcfg.p_numfon == 9  || cartcfg.p_numfon == 2  || cartcfg.p_numfon == 3  || cartcfg.p_numfon == 30  || cartcfg.p_numfon == 24  || cartcfg.p_numfon == 4  || cartcfg.p_numfon == 27  || cartcfg.p_numfon == 8  || cartcfg.p_numfon == 15  || cartcfg.p_numfon == 31  || cartcfg.p_numfon == 22 )
										{
											//Node55
											if( cartcfg.p_numfon == 1 )
											{
												//Node56
												if( cartcfg.numfon == 1 )
												{
													//TerminalNode50
													mean=5.865000;
												}
												else
												{
													//Node57
													if( cartcfg.nn_numfon == 18  || cartcfg.nn_numfon == 20  || cartcfg.nn_numfon == 23  || cartcfg.nn_numfon == 14  || cartcfg.nn_numfon == 25  || cartcfg.nn_numfon == 21  || cartcfg.nn_numfon == 1  || cartcfg.nn_numfon == 7  || cartcfg.nn_numfon == 9  || cartcfg.nn_numfon == 2  || cartcfg.nn_numfon == 19  || cartcfg.nn_numfon == 11  || cartcfg.nn_numfon == 17  || cartcfg.nn_numfon == 10  || cartcfg.nn_numfon == 12  || cartcfg.nn_numfon == 13  || cartcfg.nn_numfon == 6  || cartcfg.nn_numfon == 26  || cartcfg.nn_numfon == 27  || cartcfg.nn_numfon == 16  || cartcfg.nn_numfon == 8  || cartcfg.nn_numfon == 15  || cartcfg.nn_numfon == 5  || cartcfg.nn_numfon == 31  || cartcfg.nn_numfon == 22 )
													{
														//TerminalNode51
														mean=6.553000;
													}
													else
													{
														//TerminalNode52
														mean=9.761000;
													}
												}
											}
											else
											{
												//Node58
												if( cartcfg.pp_numfon == 18  || cartcfg.pp_numfon == 20  || cartcfg.pp_numfon == 23  || cartcfg.pp_numfon == 0  || cartcfg.pp_numfon == 9  || cartcfg.pp_numfon == 19  || cartcfg.pp_numfon == 3  || cartcfg.pp_numfon == 13  || cartcfg.pp_numfon == 4  || cartcfg.pp_numfon == 26  || cartcfg.pp_numfon == 22 )
												{
													//TerminalNode53
													mean=7.174000;
												}
												else
												{
													//TerminalNode54
													mean=7.565000;
												}
											}
										}
										else
										{
											//Node59
											if( cartcfg.pp_numfon == 18  || cartcfg.pp_numfon == 20  || cartcfg.pp_numfon == 14  || cartcfg.pp_numfon == 1  || cartcfg.pp_numfon == 7  || cartcfg.pp_numfon == 9  || cartcfg.pp_numfon == 19  || cartcfg.pp_numfon == 24  || cartcfg.pp_numfon == 13  || cartcfg.pp_numfon == 4  || cartcfg.pp_numfon == 27  || cartcfg.pp_numfon == 15  || cartcfg.pp_numfon == 31  || cartcfg.pp_numfon == 22 )
											{
												//TerminalNode55
												mean=7.732000;
											}
											else
											{
												//Node60
												if( cartcfg.nn_numfon == 18  || cartcfg.nn_numfon == 20  || cartcfg.nn_numfon == 23  || cartcfg.nn_numfon == 14  || cartcfg.nn_numfon == 25  || cartcfg.nn_numfon == 21  || cartcfg.nn_numfon == 1  || cartcfg.nn_numfon == 9  || cartcfg.nn_numfon == 2  || cartcfg.nn_numfon == 11  || cartcfg.nn_numfon == 3  || cartcfg.nn_numfon == 30  || cartcfg.nn_numfon == 17  || cartcfg.nn_numfon == 10  || cartcfg.nn_numfon == 4  || cartcfg.nn_numfon == 6  || cartcfg.nn_numfon == 26  || cartcfg.nn_numfon == 16  || cartcfg.nn_numfon == 8  || cartcfg.nn_numfon == 15  || cartcfg.nn_numfon == 5  || cartcfg.nn_numfon == 31 )
												{
													//TerminalNode56
													mean=8.095000;
												}
												else
												{
													//TerminalNode57
													mean=9.814000;
												}
											}
										}
									}
								}
							}
							else
							{
								//TerminalNode72
								mean=8.448000;
							}
						}
						else
						{
							//TerminalNode70
							mean=5.854000;
						}
					}
				}
				else
				{
					//Node24
					if( cartcfg.pp_numfon == 25  || cartcfg.pp_numfon == 21  || cartcfg.pp_numfon == 28  || cartcfg.pp_numfon == 11  || cartcfg.pp_numfon == 17  || cartcfg.pp_numfon == 10  || cartcfg.pp_numfon == 6  || cartcfg.pp_numfon == 16  || cartcfg.pp_numfon == 8 )
					{
						//TerminalNode71
						mean=7.134000;
					}
					else
					{
						//Node7
						if( cartcfg.p_numfon == 20  || cartcfg.p_numfon == 14  || cartcfg.p_numfon == 1  || cartcfg.p_numfon == 2  || cartcfg.p_numfon == 30  || cartcfg.p_numfon == 10  || cartcfg.p_numfon == 31 )
						{
							//Node8
							if( cartcfg.n_numfon == 1  || cartcfg.n_numfon == 2  || cartcfg.n_numfon == 17  || cartcfg.n_numfon == 24  || cartcfg.n_numfon == 13  || cartcfg.n_numfon == 26  || cartcfg.n_numfon == 31 )
							{
								//Node9
								if( cartcfg.p_numfon == 14  || cartcfg.p_numfon == 2  || cartcfg.p_numfon == 31 )
								{
									//TerminalNode1
									mean=4.960000;
								}
								else
								{
									//Node10
									if( cartcfg.nn_numfon == 18  || cartcfg.nn_numfon == 20  || cartcfg.nn_numfon == 14  || cartcfg.nn_numfon == 25  || cartcfg.nn_numfon == 1  || cartcfg.nn_numfon == 7  || cartcfg.nn_numfon == 9  || cartcfg.nn_numfon == 2  || cartcfg.nn_numfon == 19  || cartcfg.nn_numfon == 11  || cartcfg.nn_numfon == 30  || cartcfg.nn_numfon == 17  || cartcfg.nn_numfon == 10  || cartcfg.nn_numfon == 24  || cartcfg.nn_numfon == 12  || cartcfg.nn_numfon == 13  || cartcfg.nn_numfon == 4  || cartcfg.nn_numfon == 6  || cartcfg.nn_numfon == 27  || cartcfg.nn_numfon == 16  || cartcfg.nn_numfon == 8  || cartcfg.nn_numfon == 15  || cartcfg.nn_numfon == 5  || cartcfg.nn_numfon == 22 )
									{
										//TerminalNode2
										mean=5.503000;
									}
									else
									{
										//TerminalNode3
										mean=6.631000;
									}
								}
							}
							else
							{
								//Node11
								if( cartcfg.nn_numfon == 18  || cartcfg.nn_numfon == 23  || cartcfg.nn_numfon == 14  || cartcfg.nn_numfon == 25  || cartcfg.nn_numfon == 21  || cartcfg.nn_numfon == 9  || cartcfg.nn_numfon == 19  || cartcfg.nn_numfon == 11  || cartcfg.nn_numfon == 30  || cartcfg.nn_numfon == 24  || cartcfg.nn_numfon == 6  || cartcfg.nn_numfon == 8  || cartcfg.nn_numfon == 15 )
								{
									//TerminalNode4
									mean=5.508000;
								}
								else
								{
									//TerminalNode5
									mean=6.308000;
								}
							}
						}
						else
						{
							//Node12
							if( cartcfg.n_numfon == 2  || cartcfg.n_numfon == 17  || cartcfg.n_numfon == 24  || cartcfg.n_numfon == 6  || cartcfg.n_numfon == 5 )
							{
								//Node13
								if( cartcfg.nn_numfon == 20  || cartcfg.nn_numfon == 23  || cartcfg.nn_numfon == 14  || cartcfg.nn_numfon == 21  || cartcfg.nn_numfon == 1  || cartcfg.nn_numfon == 7  || cartcfg.nn_numfon == 2  || cartcfg.nn_numfon == 19  || cartcfg.nn_numfon == 11  || cartcfg.nn_numfon == 30  || cartcfg.nn_numfon == 17  || cartcfg.nn_numfon == 10  || cartcfg.nn_numfon == 24  || cartcfg.nn_numfon == 12  || cartcfg.nn_numfon == 13  || cartcfg.nn_numfon == 4  || cartcfg.nn_numfon == 6  || cartcfg.nn_numfon == 27  || cartcfg.nn_numfon == 16  || cartcfg.nn_numfon == 15  || cartcfg.nn_numfon == 5  || cartcfg.nn_numfon == 31  || cartcfg.nn_numfon == 22 )
								{
									//TerminalNode6
									mean=5.341000;
								}
								else
								{
									//TerminalNode7
									mean=6.844000;
								}
							}
							else
							{
								//Node14
								if( cartcfg.nn_numfon == 20  || cartcfg.nn_numfon == 14  || cartcfg.nn_numfon == 25  || cartcfg.nn_numfon == 21  || cartcfg.nn_numfon == 7  || cartcfg.nn_numfon == 2  || cartcfg.nn_numfon == 19  || cartcfg.nn_numfon == 11  || cartcfg.nn_numfon == 3  || cartcfg.nn_numfon == 30  || cartcfg.nn_numfon == 17  || cartcfg.nn_numfon == 10  || cartcfg.nn_numfon == 24  || cartcfg.nn_numfon == 12  || cartcfg.nn_numfon == 13  || cartcfg.nn_numfon == 15  || cartcfg.nn_numfon == 5  || cartcfg.nn_numfon == 31 )
								{
									//TerminalNode8
									mean=6.280000;
								}
								else
								{
									//TerminalNode9
									mean=6.890000;
								}
							}
						}
					}
				}
			}
			else
			{
				//Node61
				if( cartcfg.p_numfon == 18  || cartcfg.p_numfon == 25  || cartcfg.p_numfon == 21  || cartcfg.p_numfon == 7  || cartcfg.p_numfon == 11  || cartcfg.p_numfon == 12  || cartcfg.p_numfon == 13  || cartcfg.p_numfon == 5 )
				{
					//Node62
					if( cartcfg.n_numfon == 2  || cartcfg.n_numfon == 3  || cartcfg.n_numfon == 30  || cartcfg.n_numfon == 5  || cartcfg.n_numfon == 22 )
					{
						//Node63
						if( cartcfg.nn_numfon == 23  || cartcfg.nn_numfon == 14  || cartcfg.nn_numfon == 7  || cartcfg.nn_numfon == 9  || cartcfg.nn_numfon == 2  || cartcfg.nn_numfon == 19  || cartcfg.nn_numfon == 11  || cartcfg.nn_numfon == 3  || cartcfg.nn_numfon == 30  || cartcfg.nn_numfon == 17  || cartcfg.nn_numfon == 10  || cartcfg.nn_numfon == 24  || cartcfg.nn_numfon == 12  || cartcfg.nn_numfon == 13  || cartcfg.nn_numfon == 16  || cartcfg.nn_numfon == 5 )
						{
							//TerminalNode58
							mean=6.383000;
						}
						else
						{
							//TerminalNode59
							mean=7.446000;
						}
					}
					else
					{
						//Node64
						if( cartcfg.nn_numfon == 23  || cartcfg.nn_numfon == 14  || cartcfg.nn_numfon == 25  || cartcfg.nn_numfon == 21  || cartcfg.nn_numfon == 1  || cartcfg.nn_numfon == 19  || cartcfg.nn_numfon == 11  || cartcfg.nn_numfon == 3  || cartcfg.nn_numfon == 30  || cartcfg.nn_numfon == 17  || cartcfg.nn_numfon == 10  || cartcfg.nn_numfon == 6  || cartcfg.nn_numfon == 16  || cartcfg.nn_numfon == 15  || cartcfg.nn_numfon == 5  || cartcfg.nn_numfon == 22 )
						{
							//Node65
							if( cartcfg.p_numfon == 25  || cartcfg.p_numfon == 21  || cartcfg.p_numfon == 7  || cartcfg.p_numfon == 11  || cartcfg.p_numfon == 12  || cartcfg.p_numfon == 13 )
							{
								//TerminalNode60
								mean=7.275000;
							}
							else
							{
								//TerminalNode61
								mean=7.708000;
							}
						}
						else
						{
							//Node66
							if( cartcfg.numfon == 2  || cartcfg.numfon == 5 )
							{
								//TerminalNode62
								mean=7.460000;
							}
							else
							{
								//TerminalNode63
								mean=7.791000;
							}
						}
					}
				}
				else
				{
					//Node67
					if( cartcfg.pp_numfon == 20  || cartcfg.pp_numfon == 23  || cartcfg.pp_numfon == 14  || cartcfg.pp_numfon == 25  || cartcfg.pp_numfon == 21  || cartcfg.pp_numfon == 0  || cartcfg.pp_numfon == 1  || cartcfg.pp_numfon == 7  || cartcfg.pp_numfon == 9  || cartcfg.pp_numfon == 2  || cartcfg.pp_numfon == 19  || cartcfg.pp_numfon == 11  || cartcfg.pp_numfon == 3  || cartcfg.pp_numfon == 30  || cartcfg.pp_numfon == 17  || cartcfg.pp_numfon == 24  || cartcfg.pp_numfon == 12  || cartcfg.pp_numfon == 13  || cartcfg.pp_numfon == 4  || cartcfg.pp_numfon == 6  || cartcfg.pp_numfon == 26  || cartcfg.pp_numfon == 27  || cartcfg.pp_numfon == 16  || cartcfg.pp_numfon == 8  || cartcfg.pp_numfon == 15  || cartcfg.pp_numfon == 5  || cartcfg.pp_numfon == 31  || cartcfg.pp_numfon == 22 )
					{
						//Node15
						if( cartcfg.n_numfon == 2  || cartcfg.n_numfon == 17  || cartcfg.n_numfon == 24 )
						{
							//Node16
							if( cartcfg.n_ct == 0 || cartcfg.n_ct == 1 || cartcfg.n_ct == 3 || cartcfg.n_ct == 4 || cartcfg.n_ct == 6 || cartcfg.n_ct == 7)
							{
								//TerminalNode10
								mean=5.341000;
							}
							else
							{
								//Node17
								if( cartcfg.p_numfon == 14  || cartcfg.p_numfon == 1  || cartcfg.p_numfon == 2  || cartcfg.p_numfon == 10  || cartcfg.p_numfon == 4  || cartcfg.p_numfon == 27  || cartcfg.p_numfon == 16  || cartcfg.p_numfon == 8  || cartcfg.p_numfon == 15 )
								{
									//Node18
									if( cartcfg.pp_numfon == 25  || cartcfg.pp_numfon == 0  || cartcfg.pp_numfon == 1  || cartcfg.pp_numfon == 7  || cartcfg.pp_numfon == 2  || cartcfg.pp_numfon == 11  || cartcfg.pp_numfon == 3  || cartcfg.pp_numfon == 30  || cartcfg.pp_numfon == 24  || cartcfg.pp_numfon == 12  || cartcfg.pp_numfon == 13  || cartcfg.pp_numfon == 4  || cartcfg.pp_numfon == 26  || cartcfg.pp_numfon == 5 )
									{
										//TerminalNode11
										mean=5.798000;
									}
									else
									{
										//TerminalNode12
										mean=8.364000;
									}
								}
								else
								{
									//TerminalNode13
									mean=6.505000;
								}
							}
						}
						else
						{
							//Node19
							if( cartcfg.p_numfon == 1  || cartcfg.p_numfon == 9  || cartcfg.p_numfon == 2  || cartcfg.p_numfon == 19  || cartcfg.p_numfon == 3  || cartcfg.p_numfon == 30  || cartcfg.p_numfon == 17  || cartcfg.p_numfon == 10  || cartcfg.p_numfon == 4  || cartcfg.p_numfon == 6  || cartcfg.p_numfon == 27  || cartcfg.p_numfon == 16  || cartcfg.p_numfon == 8  || cartcfg.p_numfon == 31 )
							{
								//Node20
								if( cartcfg.nn_numfon == 23  || cartcfg.nn_numfon == 25  || cartcfg.nn_numfon == 2  || cartcfg.nn_numfon == 19  || cartcfg.nn_numfon == 10  || cartcfg.nn_numfon == 24  || cartcfg.nn_numfon == 12  || cartcfg.nn_numfon == 6  || cartcfg.nn_numfon == 15  || cartcfg.nn_numfon == 5  || cartcfg.nn_numfon == 22 )
								{
									//Node21
									if( cartcfg.p_vfrt == 1 || cartcfg.p_vfrt == 2)
									{
										//Node22
										if(cartcfg.nsilsen <= 6.50)
										{
											//TerminalNode14
											mean=7.967000;
										}
										else
										{
											//TerminalNode15
											mean=5.543000;
										}
									}
									else
									{
										//TerminalNode16
										mean=6.435000;
									}
								}
								else
								{
									//TerminalNode17
									mean=6.858000;
								}
							}
							else
							{
								//Node23
								if( cartcfg.nn_numfon == 14  || cartcfg.nn_numfon == 25  || cartcfg.nn_numfon == 9  || cartcfg.nn_numfon == 17  || cartcfg.nn_numfon == 12  || cartcfg.nn_numfon == 13  || cartcfg.nn_numfon == 15  || cartcfg.nn_numfon == 5  || cartcfg.nn_numfon == 31  || cartcfg.nn_numfon == 22 )
								{
									//TerminalNode18
									mean=6.713000;
								}
								else
								{
									//TerminalNode19
									mean=7.271000;
								}
							}
						}
					}
					else
					{
						//Node25
						if( cartcfg.p_numfon == 14  || cartcfg.p_numfon == 9  || cartcfg.p_numfon == 30  || cartcfg.p_numfon == 26  || cartcfg.p_numfon == 31 )
						{
							//TerminalNode20
							mean=6.224000;
						}
						else
						{
							//Node26
							if( cartcfg.n_numfon == 2  || cartcfg.n_numfon == 3  || cartcfg.n_numfon == 30  || cartcfg.n_numfon == 10  || cartcfg.n_numfon == 24  || cartcfg.n_numfon == 12  || cartcfg.n_numfon == 26 )
							{
								//Node27
								if( cartcfg.nn_numfon == 14  || cartcfg.nn_numfon == 7  || cartcfg.nn_numfon == 2  || cartcfg.nn_numfon == 11  || cartcfg.nn_numfon == 30  || cartcfg.nn_numfon == 10  || cartcfg.nn_numfon == 4  || cartcfg.nn_numfon == 6  || cartcfg.nn_numfon == 26  || cartcfg.nn_numfon == 27  || cartcfg.nn_numfon == 15  || cartcfg.nn_numfon == 22 )
								{
									//TerminalNode21
									mean=6.319000;
								}
								else
								{
									//TerminalNode22
									mean=6.866000;
								}
							}
							else
							{
								//TerminalNode23
								mean=7.120000;
							}
						}
					}
				}
			}
		}
		else
		{
			//Node70
			if( cartcfg.p_numfon == 11  || cartcfg.p_numfon == 30  || cartcfg.p_numfon == 31 )
			{
				//Node71
				if(cartcfg.vhgt == 2)
				{
					//Node72
					if(cartcfg.npalutt <= 22.50)
					{
						//TerminalNode66
						mean=9.114000;
					}
					else
					{
						//TerminalNode67
						mean=8.167000;
					}
				}
				else
				{
					//Node74
					if( cartcfg.pp_numfon == 23  || cartcfg.pp_numfon == 14  || cartcfg.pp_numfon == 21  || cartcfg.pp_numfon == 0  || cartcfg.pp_numfon == 1  || cartcfg.pp_numfon == 28  || cartcfg.pp_numfon == 9  || cartcfg.pp_numfon == 2  || cartcfg.pp_numfon == 19  || cartcfg.pp_numfon == 11  || cartcfg.pp_numfon == 3  || cartcfg.pp_numfon == 30  || cartcfg.pp_numfon == 17  || cartcfg.pp_numfon == 24  || cartcfg.pp_numfon == 12  || cartcfg.pp_numfon == 13  || cartcfg.pp_numfon == 4  || cartcfg.pp_numfon == 6  || cartcfg.pp_numfon == 5  || cartcfg.pp_numfon == 31  || cartcfg.pp_numfon == 22 )
					{
						//Node87
						if( cartcfg.numfon == 1  || cartcfg.numfon == 2  || cartcfg.numfon == 4 )
						{
							//Node88
							if(cartcfg.nsilutt <= 34.00)
							{
								//TerminalNode82
								mean=6.683000;
							}
							else
							{
								//TerminalNode83
								mean=5.748000;
							}
						}
						else
						{
							//TerminalNode84
							mean=7.381000;
						}
					}
					else
					{
						//Node89
						if( cartcfg.numfon == 2  || cartcfg.numfon == 4 )
						{
							//Node90
							if( cartcfg.p_numfon == 25  || cartcfg.p_numfon == 9  || cartcfg.p_numfon == 3  || cartcfg.p_numfon == 30  || cartcfg.p_numfon == 17  || cartcfg.p_numfon == 10  || cartcfg.p_numfon == 4  || cartcfg.p_numfon == 16  || cartcfg.p_numfon == 8  || cartcfg.p_numfon == 15  || cartcfg.p_numfon == 31  || cartcfg.p_numfon == 22 )
							{
								//Node91
								if( cartcfg.pp_numfon == 20  || cartcfg.pp_numfon == 23  || cartcfg.pp_numfon == 9  || cartcfg.pp_numfon == 19  || cartcfg.pp_numfon == 11  || cartcfg.pp_numfon == 30  || cartcfg.pp_numfon == 10  || cartcfg.pp_numfon == 24  || cartcfg.pp_numfon == 26  || cartcfg.pp_numfon == 16  || cartcfg.pp_numfon == 15 )
								{
									//TerminalNode85
									mean=6.637000;
								}
								else
								{
									//Node92
									if( cartcfg.nn_numfon == 18  || cartcfg.nn_numfon == 23  || cartcfg.nn_numfon == 14  || cartcfg.nn_numfon == 25  || cartcfg.nn_numfon == 21  || cartcfg.nn_numfon == 1  || cartcfg.nn_numfon == 7  || cartcfg.nn_numfon == 9  || cartcfg.nn_numfon == 2  || cartcfg.nn_numfon == 19  || cartcfg.nn_numfon == 11  || cartcfg.nn_numfon == 30  || cartcfg.nn_numfon == 17  || cartcfg.nn_numfon == 10  || cartcfg.nn_numfon == 24  || cartcfg.nn_numfon == 12  || cartcfg.nn_numfon == 13  || cartcfg.nn_numfon == 27  || cartcfg.nn_numfon == 16  || cartcfg.nn_numfon == 15  || cartcfg.nn_numfon == 5  || cartcfg.nn_numfon == 22 )
									{
										//TerminalNode86
										mean=7.045000;
									}
									else
									{
										//TerminalNode87
										mean=7.508000;
									}
								}
							}
							else
							{
								//TerminalNode88
								mean=7.719000;
							}
						}
						else
						{
							//Node93
							if( cartcfg.nn_numfon == 18  || cartcfg.nn_numfon == 14  || cartcfg.nn_numfon == 21  || cartcfg.nn_numfon == 7  || cartcfg.nn_numfon == 19  || cartcfg.nn_numfon == 11  || cartcfg.nn_numfon == 17  || cartcfg.nn_numfon == 10  || cartcfg.nn_numfon == 12  || cartcfg.nn_numfon == 13  || cartcfg.nn_numfon == 6  || cartcfg.nn_numfon == 27  || cartcfg.nn_numfon == 15  || cartcfg.nn_numfon == 5  || cartcfg.nn_numfon == 22 )
							{
								//Node94
								if( cartcfg.pp_numfon == 20  || cartcfg.pp_numfon == 23  || cartcfg.pp_numfon == 25  || cartcfg.pp_numfon == 19  || cartcfg.pp_numfon == 11  || cartcfg.pp_numfon == 30  || cartcfg.pp_numfon == 24  || cartcfg.pp_numfon == 27  || cartcfg.pp_numfon == 8  || cartcfg.pp_numfon == 15 )
								{
									//TerminalNode89
									mean=5.852000;
								}
								else
								{
									//TerminalNode90
									mean=7.088000;
								}
							}
							else
							{
								//Node95
								if( cartcfg.pp_numfon == 18  || cartcfg.pp_numfon == 20  || cartcfg.pp_numfon == 10  || cartcfg.pp_numfon == 24  || cartcfg.pp_numfon == 26  || cartcfg.pp_numfon == 16  || cartcfg.pp_numfon == 31 )
								{
									//Node96
									if( cartcfg.n_numfon == 14  || cartcfg.n_numfon == 0  || cartcfg.n_numfon == 7  || cartcfg.n_numfon == 9  || cartcfg.n_numfon == 8 )
									{
										//TerminalNode91
										mean=6.729000;
									}
									else
									{
										//Node97
										if( cartcfg.nn_numfon == 20  || cartcfg.nn_numfon == 23  || cartcfg.nn_numfon == 25  || cartcfg.nn_numfon == 9  || cartcfg.nn_numfon == 2  || cartcfg.nn_numfon == 3  || cartcfg.nn_numfon == 30  || cartcfg.nn_numfon == 16  || cartcfg.nn_numfon == 8  || cartcfg.nn_numfon == 31 )
										{
											//TerminalNode92
											mean=7.205000;
										}
										else
										{
											//TerminalNode93
											mean=7.864000;
										}
									}
								}
								else
								{
									//Node98
									if( cartcfg.n_numfon == 25  || cartcfg.n_numfon == 21  || cartcfg.n_numfon == 0  || cartcfg.n_numfon == 7  || cartcfg.n_numfon == 9  || cartcfg.n_numfon == 27  || cartcfg.n_numfon == 8 )
									{
										//Node99
										if( cartcfg.p_numfon == 1  || cartcfg.p_numfon == 9  || cartcfg.p_numfon == 3  || cartcfg.p_numfon == 17  || cartcfg.p_numfon == 10  || cartcfg.p_numfon == 4  || cartcfg.p_numfon == 16  || cartcfg.p_numfon == 8  || cartcfg.p_numfon == 15  || cartcfg.p_numfon == 31  || cartcfg.p_numfon == 22 )
										{
											//TerminalNode94
											mean=7.348000;
										}
										else
										{
											//TerminalNode95
											mean=8.028000;
										}
									}
									else
									{
										//Node100
										if(cartcfg.ultimasil == 0)
										{
											//TerminalNode96
											mean=7.684000;
										}
										else
										{
											//TerminalNode97
											mean=8.220000;
										}
									}
								}
							}
						}
					}
				}
			}
			else
			{
				//Node75
				if(cartcfg.nsilword <= 2.50)
				{
					//Node76
					if( cartcfg.p_numfon == 9  || cartcfg.p_numfon == 2  || cartcfg.p_numfon == 19  || cartcfg.p_numfon == 3  || cartcfg.p_numfon == 17  || cartcfg.p_numfon == 10  || cartcfg.p_numfon == 12  || cartcfg.p_numfon == 13  || cartcfg.p_numfon == 4  || cartcfg.p_numfon == 6  || cartcfg.p_numfon == 26  || cartcfg.p_numfon == 27  || cartcfg.p_numfon == 16  || cartcfg.p_numfon == 8  || cartcfg.p_numfon == 15  || cartcfg.p_numfon == 22 )
					{
						//Node77
						if( cartcfg.vfrt == 1 || cartcfg.vfrt == 3)
						{
							//Node78
							if( cartcfg.nn_numfon == 23  || cartcfg.nn_numfon == 14  || cartcfg.nn_numfon == 25  || cartcfg.nn_numfon == 19  || cartcfg.nn_numfon == 11  || cartcfg.nn_numfon == 30  || cartcfg.nn_numfon == 10  || cartcfg.nn_numfon == 12  || cartcfg.nn_numfon == 6  || cartcfg.nn_numfon == 15  || cartcfg.nn_numfon == 22 )
							{
								//Node79
								if( cartcfg.pp_numfon == 23  || cartcfg.pp_numfon == 14  || cartcfg.pp_numfon == 25  || cartcfg.pp_numfon == 21  || cartcfg.pp_numfon == 7  || cartcfg.pp_numfon == 28  || cartcfg.pp_numfon == 9  || cartcfg.pp_numfon == 2  || cartcfg.pp_numfon == 19  || cartcfg.pp_numfon == 11  || cartcfg.pp_numfon == 3  || cartcfg.pp_numfon == 30  || cartcfg.pp_numfon == 17  || cartcfg.pp_numfon == 10  || cartcfg.pp_numfon == 12  || cartcfg.pp_numfon == 4  || cartcfg.pp_numfon == 6  || cartcfg.pp_numfon == 27  || cartcfg.pp_numfon == 16  || cartcfg.pp_numfon == 15  || cartcfg.pp_numfon == 5  || cartcfg.pp_numfon == 31  || cartcfg.pp_numfon == 22 )
								{
									//TerminalNode73
									mean=6.480000;
								}
								else
								{
									//TerminalNode74
									mean=7.430000;
								}
							}
							else
							{
								//TerminalNode75
								mean=7.598000;
							}
						}
						else
						{
							//TerminalNode76
							mean=8.164000;
						}
					}
					else
					{
						//Node80
						if( cartcfg.pp_numfon == 18  || cartcfg.pp_numfon == 20  || cartcfg.pp_numfon == 23  || cartcfg.pp_numfon == 25  || cartcfg.pp_numfon == 1  || cartcfg.pp_numfon == 7  || cartcfg.pp_numfon == 9  || cartcfg.pp_numfon == 2  || cartcfg.pp_numfon == 19  || cartcfg.pp_numfon == 11  || cartcfg.pp_numfon == 3  || cartcfg.pp_numfon == 30  || cartcfg.pp_numfon == 17  || cartcfg.pp_numfon == 10  || cartcfg.pp_numfon == 13  || cartcfg.pp_numfon == 6  || cartcfg.pp_numfon == 26  || cartcfg.pp_numfon == 16  || cartcfg.pp_numfon == 8  || cartcfg.pp_numfon == 15  || cartcfg.pp_numfon == 5  || cartcfg.pp_numfon == 31  || cartcfg.pp_numfon == 22 )
						{
							//TerminalNode77
							mean=8.241000;
						}
						else
						{
							//TerminalNode78
							mean=8.909000;
						}
					}
				}
				else
				{
					//Node81
					if( cartcfg.p_numfon == 14  || cartcfg.p_numfon == 21  || cartcfg.p_numfon == 7  || cartcfg.p_numfon == 9  || cartcfg.p_numfon == 19  || cartcfg.p_numfon == 17  || cartcfg.p_numfon == 10  || cartcfg.p_numfon == 24  || cartcfg.p_numfon == 12  || cartcfg.p_numfon == 6  || cartcfg.p_numfon == 27  || cartcfg.p_numfon == 16  || cartcfg.p_numfon == 8  || cartcfg.p_numfon == 15  || cartcfg.p_numfon == 22 )
					{
						//Node82
						if( cartcfg.nn_numfon == 18  || cartcfg.nn_numfon == 23  || cartcfg.nn_numfon == 14  || cartcfg.nn_numfon == 25  || cartcfg.nn_numfon == 1  || cartcfg.nn_numfon == 7  || cartcfg.nn_numfon == 9  || cartcfg.nn_numfon == 19  || cartcfg.nn_numfon == 11  || cartcfg.nn_numfon == 3  || cartcfg.nn_numfon == 10  || cartcfg.nn_numfon == 12  || cartcfg.nn_numfon == 13  || cartcfg.nn_numfon == 6  || cartcfg.nn_numfon == 27  || cartcfg.nn_numfon == 16  || cartcfg.nn_numfon == 8  || cartcfg.nn_numfon == 15  || cartcfg.nn_numfon == 5  || cartcfg.nn_numfon == 22 )
						{
							//TerminalNode79
							mean=7.986000;
						}
						else
						{
							//TerminalNode80
							mean=8.508000;
						}
					}
					else
					{
						//TerminalNode81
						mean=8.753000;
					}
				}
			}
		}
	}
	else
	{
		//Node83
		if( cartcfg.n_s_n == 1 || cartcfg.n_s_n == 2)
		{
			//Node84
			if(cartcfg.str == 0)
			{
				//Node85
				if( cartcfg.p_numfon == 25  || cartcfg.p_numfon == 1  || cartcfg.p_numfon == 9  || cartcfg.p_numfon == 2  || cartcfg.p_numfon == 3  || cartcfg.p_numfon == 30  || cartcfg.p_numfon == 17  || cartcfg.p_numfon == 10  || cartcfg.p_numfon == 4  || cartcfg.p_numfon == 26  || cartcfg.p_numfon == 16  || cartcfg.p_numfon == 8  || cartcfg.p_numfon == 15  || cartcfg.p_numfon == 31  || cartcfg.p_numfon == 22 )
				{
					//Node86
					if( cartcfg.pp_numfon == 14  || cartcfg.pp_numfon == 21  || cartcfg.pp_numfon == 7  || cartcfg.pp_numfon == 28  || cartcfg.pp_numfon == 17  || cartcfg.pp_numfon == 12  || cartcfg.pp_numfon == 6  || cartcfg.pp_numfon == 22 )
					{
						//TerminalNode68
						mean=6.643000;
					}
					else
					{
						//TerminalNode69
						mean=8.048000;
					}
				}
				else
				{
					//Node101
					if( cartcfg.npossil == 3 )
					{
						//TerminalNode98
						mean=7.681000;
					}
					else
					{
						//Node102
						if( cartcfg.p_numfon == 20  || cartcfg.p_numfon == 14  || cartcfg.p_numfon == 21  || cartcfg.p_numfon == 7  || cartcfg.p_numfon == 19  || cartcfg.p_numfon == 24  || cartcfg.p_numfon == 12  || cartcfg.p_numfon == 13  || cartcfg.p_numfon == 6  || cartcfg.p_numfon == 27  || cartcfg.p_numfon == 5 )
						{
							//Node103
							if( cartcfg.nn_numfon == 18  || cartcfg.nn_numfon == 23  || cartcfg.nn_numfon == 14  || cartcfg.nn_numfon == 25  || cartcfg.nn_numfon == 21  || cartcfg.nn_numfon == 7  || cartcfg.nn_numfon == 9  || cartcfg.nn_numfon == 2  || cartcfg.nn_numfon == 19  || cartcfg.nn_numfon == 11  || cartcfg.nn_numfon == 3  || cartcfg.nn_numfon == 17  || cartcfg.nn_numfon == 13  || cartcfg.nn_numfon == 6  || cartcfg.nn_numfon == 16  || cartcfg.nn_numfon == 15  || cartcfg.nn_numfon == 5  || cartcfg.nn_numfon == 22 )
							{
								//Node104
								if( cartcfg.p_numfon == 14 )
								{
									//TerminalNode99
									mean=4.315000;
								}
								else
								{
									//TerminalNode100
									mean=7.728000;
								}
							}
							else
							{
								//Node105
								if( cartcfg.numfon == 2  || cartcfg.numfon == 4  || cartcfg.numfon == 5 )
								{
									//Node106
									if( cartcfg.pp_numfon == 20  || cartcfg.pp_numfon == 23  || cartcfg.pp_numfon == 14  || cartcfg.pp_numfon == 25  || cartcfg.pp_numfon == 21  || cartcfg.pp_numfon == 7  || cartcfg.pp_numfon == 28  || cartcfg.pp_numfon == 9  || cartcfg.pp_numfon == 19  || cartcfg.pp_numfon == 11  || cartcfg.pp_numfon == 30  || cartcfg.pp_numfon == 17  || cartcfg.pp_numfon == 10  || cartcfg.pp_numfon == 12  || cartcfg.pp_numfon == 13  || cartcfg.pp_numfon == 6  || cartcfg.pp_numfon == 26  || cartcfg.pp_numfon == 27  || cartcfg.pp_numfon == 8  || cartcfg.pp_numfon == 15  || cartcfg.pp_numfon == 31  || cartcfg.pp_numfon == 22 )
									{
										//TerminalNode101
										mean=6.867000;
									}
									else
									{
										//TerminalNode102
										mean=7.948000;
									}
								}
								else
								{
									//Node107
									if( cartcfg.n_numfon == 14  || cartcfg.n_numfon == 21  || cartcfg.n_numfon == 0  || cartcfg.n_numfon == 7  || cartcfg.n_numfon == 9  || cartcfg.n_numfon == 27  || cartcfg.n_numfon == 8 )
									{
										//TerminalNode103
										mean=8.051000;
									}
									else
									{
										//TerminalNode104
										mean=8.684000;
									}
								}
							}
						}
						else
						{
							//Node108
							if( cartcfg.pp_numfon == 18  || cartcfg.pp_numfon == 23  || cartcfg.pp_numfon == 14  || cartcfg.pp_numfon == 25  || cartcfg.pp_numfon == 0  || cartcfg.pp_numfon == 1  || cartcfg.pp_numfon == 7  || cartcfg.pp_numfon == 9  || cartcfg.pp_numfon == 2  || cartcfg.pp_numfon == 19  || cartcfg.pp_numfon == 11  || cartcfg.pp_numfon == 3  || cartcfg.pp_numfon == 30  || cartcfg.pp_numfon == 17  || cartcfg.pp_numfon == 10  || cartcfg.pp_numfon == 24  || cartcfg.pp_numfon == 12  || cartcfg.pp_numfon == 13  || cartcfg.pp_numfon == 4  || cartcfg.pp_numfon == 6  || cartcfg.pp_numfon == 26  || cartcfg.pp_numfon == 27  || cartcfg.pp_numfon == 16  || cartcfg.pp_numfon == 8  || cartcfg.pp_numfon == 15  || cartcfg.pp_numfon == 5  || cartcfg.pp_numfon == 31  || cartcfg.pp_numfon == 22 )
							{
								//TerminalNode105
								mean=8.501000;
							}
							else
							{
								//TerminalNode106
								mean=9.745000;
							}
						}
					}
				}
			}
			else
			{
				//Node109
				if( cartcfg.p_numfon == 30  || cartcfg.p_numfon == 16  || cartcfg.p_numfon == 15  || cartcfg.p_numfon == 31 )
				{
					//Node110
					if( cartcfg.numfon == 2  || cartcfg.numfon == 3  || cartcfg.numfon == 5 )
					{
						//TerminalNode107
						mean=7.175000;
					}
					else
					{
						//TerminalNode108
						mean=8.505000;
					}
				}
				else
				{
					//Node111
					if( cartcfg.vhgt == 2 || cartcfg.vhgt == 3)
					{
						//Node112
						if( cartcfg.p_numfon == 25  || cartcfg.p_numfon == 21  || cartcfg.p_numfon == 9  || cartcfg.p_numfon == 2  || cartcfg.p_numfon == 19  || cartcfg.p_numfon == 17  || cartcfg.p_numfon == 10  || cartcfg.p_numfon == 12  || cartcfg.p_numfon == 13  || cartcfg.p_numfon == 4  || cartcfg.p_numfon == 6  || cartcfg.p_numfon == 26  || cartcfg.p_numfon == 27  || cartcfg.p_numfon == 8  || cartcfg.p_numfon == 22 )
						{
							//Node113
							if( cartcfg.pp_numfon == 14  || cartcfg.pp_numfon == 25  || cartcfg.pp_numfon == 7  || cartcfg.pp_numfon == 28  || cartcfg.pp_numfon == 9  || cartcfg.pp_numfon == 10  || cartcfg.pp_numfon == 12  || cartcfg.pp_numfon == 6  || cartcfg.pp_numfon == 27  || cartcfg.pp_numfon == 15  || cartcfg.pp_numfon == 22 )
							{
								//TerminalNode109
								mean=7.212000;
							}
							else
							{
								//TerminalNode110
								mean=8.407000;
							}
						}
						else
						{
							//TerminalNode111
							mean=8.833000;
						}
					}
					else
					{
						//TerminalNode112
						mean=8.921000;
					}
				}
			}
		}
		else
		{
			//Node114
			if( cartcfg.p_numfon == 14  || cartcfg.p_numfon == 0  || cartcfg.p_numfon == 1  || cartcfg.p_numfon == 2  || cartcfg.p_numfon == 11  || cartcfg.p_numfon == 30  || cartcfg.p_numfon == 4  || cartcfg.p_numfon == 22 )
			{
				//TerminalNode113
				mean=8.171000;
			}
			else
			{
				//TerminalNode114
				mean=9.290000;
			}
		}
	}
}


else
{
	//Node115
	if( cartcfg.n_numfon == 14  || cartcfg.n_numfon == 25  || cartcfg.n_numfon == 0  || cartcfg.n_numfon == 7  || cartcfg.n_numfon == 9  || cartcfg.n_numfon == 3  || cartcfg.n_numfon == 17  || cartcfg.n_numfon == 12  || cartcfg.n_numfon == 13  || cartcfg.n_numfon == 6  || cartcfg.n_numfon == 27  || cartcfg.n_numfon == 8  || cartcfg.n_numfon == 15  || cartcfg.n_numfon == 31 )
	{
		//Node116
		if( cartcfg.p_numfon == 14  || cartcfg.p_numfon == 0  || cartcfg.p_numfon == 1  || cartcfg.p_numfon == 7  || cartcfg.p_numfon == 2  || cartcfg.p_numfon == 11  || cartcfg.p_numfon == 17  || cartcfg.p_numfon == 4  || cartcfg.p_numfon == 5  || cartcfg.p_numfon == 31 )
		{
			//TerminalNode115
			mean=3.968000;
		}
		else
		{
			//TerminalNode116
			mean=8.204000;
		}
	}
	else
	{
		//Node117
		if( cartcfg.p_numfon == 14  || cartcfg.p_numfon == 25  || cartcfg.p_numfon == 0  || cartcfg.p_numfon == 1  || cartcfg.p_numfon == 11  || cartcfg.p_numfon == 30  || cartcfg.p_numfon == 17  || cartcfg.p_numfon == 4  || cartcfg.p_numfon == 15  || cartcfg.p_numfon == 5  || cartcfg.p_numfon == 31 )
		{
			//TerminalNode117
			mean=8.710000;
		}
		else
		{
			//TerminalNode118
			mean=9.487000;
		}
	}
}
}
//semivocales j w
else if(cartcfg.numfon == 30 || cartcfg.numfon == 31){
/*CLASSIFICATION AND REGRESSION TREE*/
//Node1
if( cartcfg.p_numfon == 23  || cartcfg.p_numfon == 25  || cartcfg.p_numfon == 21  || cartcfg.p_numfon == 1  || cartcfg.p_numfon == 7  || cartcfg.p_numfon == 9  || cartcfg.p_numfon == 2  || cartcfg.p_numfon == 19  || cartcfg.p_numfon == 11  || cartcfg.p_numfon == 30  || cartcfg.p_numfon == 10  || cartcfg.p_numfon == 24  || cartcfg.p_numfon == 12  || cartcfg.p_numfon == 13  || cartcfg.p_numfon == 4  || cartcfg.p_numfon == 6  || cartcfg.p_numfon == 16  || cartcfg.p_numfon == 8  || cartcfg.p_numfon == 15  || cartcfg.p_numfon == 22 )
{
	//Node2
	if( cartcfg.p_numfon == 21  || cartcfg.p_numfon == 11  || cartcfg.p_numfon == 10  || cartcfg.p_numfon == 16  || cartcfg.p_numfon == 15  || cartcfg.p_numfon == 22 )
	{
		//Node3
		if(cartcfg.nsilutt <= 21.50)
		{
			//Node4
			if(cartcfg.ultimapal == 0)
			{
				//TerminalNode1
				mean=6.093000;
			}
			else
			{
				//TerminalNode2
				mean=6.784000;
			}
		}
		else
		{
			//Node5
			if( cartcfg.nn_numfon == 20  || cartcfg.nn_numfon == 23  || cartcfg.nn_numfon == 14  || cartcfg.nn_numfon == 21  || cartcfg.nn_numfon == 1  || cartcfg.nn_numfon == 9  || cartcfg.nn_numfon == 2  || cartcfg.nn_numfon == 19  || cartcfg.nn_numfon == 11  || cartcfg.nn_numfon == 3  || cartcfg.nn_numfon == 30  || cartcfg.nn_numfon == 12  || cartcfg.nn_numfon == 4  || cartcfg.nn_numfon == 6  || cartcfg.nn_numfon == 8  || cartcfg.nn_numfon == 15  || cartcfg.nn_numfon == 5  || cartcfg.nn_numfon == 31 )
			{
				//TerminalNode3
				mean=6.515000;
			}
			else
			{
				//Node6
				if( cartcfg.p_numfon == 10  || cartcfg.p_numfon == 15  || cartcfg.p_numfon == 22 )
				{
					//TerminalNode4
					mean=6.670000;
				}
				else
				{
					//TerminalNode5
					mean=7.343000;
				}
			}
		}
	}
	else
	{
		//Node7
		if( cartcfg.n_numfon == 14  || cartcfg.n_numfon == 2  || cartcfg.n_numfon == 3  || cartcfg.n_numfon == 17  || cartcfg.n_numfon == 10  || cartcfg.n_numfon == 12  || cartcfg.n_numfon == 13  || cartcfg.n_numfon == 16  || cartcfg.n_numfon == 8  || cartcfg.n_numfon == 5  || cartcfg.n_numfon == 22 )
		{
			//Node8
			if( cartcfg.nn_numfon == 20  || cartcfg.nn_numfon == 23  || cartcfg.nn_numfon == 25  || cartcfg.nn_numfon == 1  || cartcfg.nn_numfon == 9  || cartcfg.nn_numfon == 2  || cartcfg.nn_numfon == 19  || cartcfg.nn_numfon == 10  || cartcfg.nn_numfon == 12  || cartcfg.nn_numfon == 13  || cartcfg.nn_numfon == 4  || cartcfg.nn_numfon == 16  || cartcfg.nn_numfon == 8  || cartcfg.nn_numfon == 15  || cartcfg.nn_numfon == 5  || cartcfg.nn_numfon == 31 )
			{
				//Node9
				if( cartcfg.nn_numfon == 9  || cartcfg.nn_numfon == 10  || cartcfg.nn_numfon == 8  || cartcfg.nn_numfon == 15  || cartcfg.nn_numfon == 5  || cartcfg.nn_numfon == 31 )
				{
					//TerminalNode6
					mean=6.183000;
				}
				else
				{
					//Node10
					if( cartcfg.p_numfon == 25  || cartcfg.p_numfon == 13 )
					{
						//Node11
						if( cartcfg.numfon == 31 )
						{
							//TerminalNode7
							mean=5.716000;
						}
						else
						{
							//TerminalNode8
							mean=8.872000;
						}
					}
					else
					{
						//TerminalNode9
						mean=7.251000;
					}
				}
			}
			else
			{
				//TerminalNode10
				mean=7.838000;
			}
		}
		else
		{
			//Node12
			if( cartcfg.nn_numfon == 14  || cartcfg.nn_numfon == 21  || cartcfg.nn_numfon == 19  || cartcfg.nn_numfon == 11  || cartcfg.nn_numfon == 30  || cartcfg.nn_numfon == 15  || cartcfg.nn_numfon == 5 )
			{
				//Node13
				if( cartcfg.pp_numfon == 18  || cartcfg.pp_numfon == 20  || cartcfg.pp_numfon == 23  || cartcfg.pp_numfon == 21  || cartcfg.pp_numfon == 0  || cartcfg.pp_numfon == 7  || cartcfg.pp_numfon == 9  || cartcfg.pp_numfon == 19  || cartcfg.pp_numfon == 11  || cartcfg.pp_numfon == 3  || cartcfg.pp_numfon == 24  || cartcfg.pp_numfon == 12  || cartcfg.pp_numfon == 13  || cartcfg.pp_numfon == 4  || cartcfg.pp_numfon == 6  || cartcfg.pp_numfon == 27  || cartcfg.pp_numfon == 16  || cartcfg.pp_numfon == 8  || cartcfg.pp_numfon == 15  || cartcfg.pp_numfon == 5  || cartcfg.pp_numfon == 31  || cartcfg.pp_numfon == 22 )
				{
					//TerminalNode11
					mean=5.351000;
				}
				else
				{
					//TerminalNode12
					mean=7.756000;
				}
			}
			else
			{
				//Node14
				if( cartcfg.p_numfon == 23  || cartcfg.p_numfon == 9  || cartcfg.p_numfon == 30  || cartcfg.p_numfon == 6 )
				{
					//TerminalNode13
					mean=7.558000;
				}
				else
				{
					//Node15
					if( cartcfg.nn_numfon == 18  || cartcfg.nn_numfon == 20  || cartcfg.nn_numfon == 23  || cartcfg.nn_numfon == 1  || cartcfg.nn_numfon == 2  || cartcfg.nn_numfon == 10  || cartcfg.nn_numfon == 12  || cartcfg.nn_numfon == 13  || cartcfg.nn_numfon == 4  || cartcfg.nn_numfon == 26  || cartcfg.nn_numfon == 27  || cartcfg.nn_numfon == 22 )
					{
						//TerminalNode14
						mean=8.122000;
					}
					else
					{
						//TerminalNode15
						mean=8.913000;
					}
				}
			}
		}
	}
}


else
{
	//Node16
	if( cartcfg.numfon == 31 )
	{
		//Node17
		if( cartcfg.p_numfon == 18  || cartcfg.p_numfon == 26  || cartcfg.p_numfon == 27 )
		{
			//TerminalNode16
			mean=7.335000;
		}
		else
		{
			//TerminalNode17
			mean=8.839000;
		}
	}
	else
	{
		//Node18
		if( cartcfg.pp_numfon == 18  || cartcfg.pp_numfon == 25  || cartcfg.pp_numfon == 21  || cartcfg.pp_numfon == 0  || cartcfg.pp_numfon == 7  || cartcfg.pp_numfon == 9  || cartcfg.pp_numfon == 30  || cartcfg.pp_numfon == 12  || cartcfg.pp_numfon == 26  || cartcfg.pp_numfon == 27  || cartcfg.pp_numfon == 16  || cartcfg.pp_numfon == 8  || cartcfg.pp_numfon == 15  || cartcfg.pp_numfon == 31  || cartcfg.pp_numfon == 22 )
		{
			//TerminalNode18
			mean=7.952000;
		}
		else
		{
			//TerminalNode19
			mean=9.067000;
		}
	}
}

}
else{
//SORDAS
/*CLASSIFICATION AND REGRESSION TREE*/
//Node1
if( cartcfg.n_numfon == 18  || cartcfg.n_numfon == 20  || cartcfg.n_numfon == 23  || cartcfg.n_numfon == 25  || cartcfg.n_numfon == 21  || cartcfg.n_numfon == 7  || cartcfg.n_numfon == 19  || cartcfg.n_numfon == 11  || cartcfg.n_numfon == 10  || cartcfg.n_numfon == 12  || cartcfg.n_numfon == 13  || cartcfg.n_numfon == 6  || cartcfg.n_numfon == 27  || cartcfg.n_numfon == 16  || cartcfg.n_numfon == 8  || cartcfg.n_numfon == 15  || cartcfg.n_numfon == 5 )
{
	//Node2
	if( cartcfg.nn_numfon == 18  || cartcfg.nn_numfon == 23  || cartcfg.nn_numfon == 25  || cartcfg.nn_numfon == 21  || cartcfg.nn_numfon == 1  || cartcfg.nn_numfon == 7  || cartcfg.nn_numfon == 9  || cartcfg.nn_numfon == 2  || cartcfg.nn_numfon == 19  || cartcfg.nn_numfon == 11  || cartcfg.nn_numfon == 3  || cartcfg.nn_numfon == 30  || cartcfg.nn_numfon == 10  || cartcfg.nn_numfon == 24  || cartcfg.nn_numfon == 12  || cartcfg.nn_numfon == 4  || cartcfg.nn_numfon == 6  || cartcfg.nn_numfon == 26  || cartcfg.nn_numfon == 27  || cartcfg.nn_numfon == 16  || cartcfg.nn_numfon == 8  || cartcfg.nn_numfon == 5  || cartcfg.nn_numfon == 31  || cartcfg.nn_numfon == 22 )
	{
		//Node3
		if( cartcfg.numfon == 16 )
		{
			//Node4
			if(cartcfg.npalutt <= 14.50)
			{
				//Node5
				if( cartcfg.npossil == 3  || cartcfg.npossil == 0 )
				{
					//Node6
					if( cartcfg.n_numfon == 20  || cartcfg.n_numfon == 25  || cartcfg.n_numfon == 11  || cartcfg.n_numfon == 27  || cartcfg.n_numfon == 16  || cartcfg.n_numfon == 15  || cartcfg.n_numfon == 5 )
					{
						//Node7
						if(cartcfg.ultimasil == 1)
						{
							//TerminalNode1
							mean=6.330000;
						}
						else
						{
							//Node8
							if( cartcfg.npossil == 3 )
							{
								//Node9
								if( cartcfg.n_numfon == 25  || cartcfg.n_numfon == 11  || cartcfg.n_numfon == 27  || cartcfg.n_numfon == 15 )
								{
									//Node10
									if(cartcfg.nsilutt <= 18.00)
									{
										//TerminalNode2
										mean=8.322000;
									}
									else
									{
										//TerminalNode3
										mean=4.032000;
									}
								}
								else
								{
									//TerminalNode4
									mean=7.318000;
								}
							}
							else
							{
								//TerminalNode5
								mean=10.488000;
							}
						}
					}
					else
					{
						//Node11
						if(cartcfg.ultimasil == 1)
						{
							//Node12
							if( cartcfg.p_numfon == 18  || cartcfg.p_numfon == 20  || cartcfg.p_numfon == 23  || cartcfg.p_numfon == 21  || cartcfg.p_numfon == 0  || cartcfg.p_numfon == 1  || cartcfg.p_numfon == 2  || cartcfg.p_numfon == 19  || cartcfg.p_numfon == 11  || cartcfg.p_numfon == 30  || cartcfg.p_numfon == 24  || cartcfg.p_numfon == 12  || cartcfg.p_numfon == 4  || cartcfg.p_numfon == 6  || cartcfg.p_numfon == 26  || cartcfg.p_numfon == 16  || cartcfg.p_numfon == 8  || cartcfg.p_numfon == 5  || cartcfg.p_numfon == 31 )
							{
								//TerminalNode6
								mean=7.394000;
							}
							else
							{
								//TerminalNode7
								mean=8.220000;
							}
						}
						else
						{
							//Node13
							if( cartcfg.n_numfon == 18  || cartcfg.n_numfon == 23  || cartcfg.n_numfon == 6  || cartcfg.n_numfon == 8 )
							{
								//TerminalNode8
								mean=8.043000;
							}
							else
							{
								//TerminalNode9
								mean=8.533000;
							}
						}
					}
				}
				else
				{
					//Node14
					if(cartcfg.nsilutt <= 21.50)
					{
						//TerminalNode10
						mean=9.847000;
					}
					else
					{
						//Node15
						if( cartcfg.pp_numfon == 18  || cartcfg.pp_numfon == 23  || cartcfg.pp_numfon == 14  || cartcfg.pp_numfon == 25  || cartcfg.pp_numfon == 21  || cartcfg.pp_numfon == 0  || cartcfg.pp_numfon == 7  || cartcfg.pp_numfon == 9  || cartcfg.pp_numfon == 19  || cartcfg.pp_numfon == 11  || cartcfg.pp_numfon == 3  || cartcfg.pp_numfon == 30  || cartcfg.pp_numfon == 17  || cartcfg.pp_numfon == 10  || cartcfg.pp_numfon == 24  || cartcfg.pp_numfon == 12  || cartcfg.pp_numfon == 4  || cartcfg.pp_numfon == 6  || cartcfg.pp_numfon == 26  || cartcfg.pp_numfon == 27  || cartcfg.pp_numfon == 8  || cartcfg.pp_numfon == 15  || cartcfg.pp_numfon == 5  || cartcfg.pp_numfon == 31  || cartcfg.pp_numfon == 22 )
						{
							//TerminalNode11
							mean=6.299000;
						}
						else
						{
							//TerminalNode12
							mean=9.372000;
						}
					}
				}
			}
			else
			{
				//Node16
				if( cartcfg.n_numfon == 20  || cartcfg.n_numfon == 23  || cartcfg.n_numfon == 25  || cartcfg.n_numfon == 7  || cartcfg.n_numfon == 27  || cartcfg.n_numfon == 16  || cartcfg.n_numfon == 5 )
				{
					//Node33
					if(cartcfg.npalutt <= 32.50)
					{
						//TerminalNode29
						mean=7.873000;
					}
					else
					{
						//TerminalNode30
						mean=8.404000;
					}
				}
				else
				{
					//TerminalNode31
					mean=8.512000;
				}
			}
		}
		else
		{
			//Node25
			if( cartcfg.n_numfon == 18  || cartcfg.n_numfon == 20  || cartcfg.n_numfon == 23  || cartcfg.n_numfon == 25  || cartcfg.n_numfon == 21  || cartcfg.n_numfon == 7  || cartcfg.n_numfon == 11  || cartcfg.n_numfon == 10  || cartcfg.n_numfon == 13  || cartcfg.n_numfon == 6  || cartcfg.n_numfon == 16  || cartcfg.n_numfon == 8 )
			{
				//Node27
				if( cartcfg.numfon == 8 )
				{
					//TerminalNode25
					mean=8.774000;
				}
				else
				{
					//TerminalNode26
					mean=9.588000;
				}
			}
			else
			{
				//Node26
				if( cartcfg.nn_numfon == 18  || cartcfg.nn_numfon == 23  || cartcfg.nn_numfon == 25  || cartcfg.nn_numfon == 21  || cartcfg.nn_numfon == 7  || cartcfg.nn_numfon == 9  || cartcfg.nn_numfon == 19  || cartcfg.nn_numfon == 11  || cartcfg.nn_numfon == 10  || cartcfg.nn_numfon == 12  || cartcfg.nn_numfon == 6  || cartcfg.nn_numfon == 27  || cartcfg.nn_numfon == 16  || cartcfg.nn_numfon == 8  || cartcfg.nn_numfon == 22 )
				{
					//TerminalNode32
					mean=8.512000;
				}
				else
				{
					//TerminalNode33
					mean=9.115000;
				}
			}
		}
	}
	else
	{
		//Node28
		if( cartcfg.nn_ct == 1 || cartcfg.nn_ct == 2 || cartcfg.nn_ct == 3 || cartcfg.nn_ct == 4 || cartcfg.nn_ct == 5 || cartcfg.nn_ct == 6 || cartcfg.nn_ct == 7)
		{
			//TerminalNode27
			mean=9.640000;
		}
		else
		{
			//TerminalNode28
			mean=11.819000;
		}
	}
}


else if(cartcfg.numfon != 0 )
{
	//Node29
	if( cartcfg.numfon == 10  || cartcfg.numfon == 6  || cartcfg.numfon == 8 )
	{
		//Node30
		if( cartcfg.p_numfon == 23  || cartcfg.p_numfon == 0  || cartcfg.p_numfon == 11  || cartcfg.p_numfon == 10  || cartcfg.p_numfon == 12  || cartcfg.p_numfon == 13  || cartcfg.p_numfon == 6 )
		{
			//Node31
			if(cartcfg.ultimasil == 0)
			{
				//Node32
				if( cartcfg.n_numfon == 14  || cartcfg.n_numfon == 1  || cartcfg.n_numfon == 9  || cartcfg.n_numfon == 2  || cartcfg.n_numfon == 30  || cartcfg.n_numfon == 17  || cartcfg.n_numfon == 26  || cartcfg.n_numfon == 22 )
				{
					//Node17
					if( cartcfg.n_ct == 1 || cartcfg.n_ct == 3 || cartcfg.n_ct == 4 || cartcfg.n_ct == 5 || cartcfg.n_ct == 7)
					{
						//Node18
						if( cartcfg.pp_numfon == 18  || cartcfg.pp_numfon == 20  || cartcfg.pp_numfon == 23  || cartcfg.pp_numfon == 14  || cartcfg.pp_numfon == 25  || cartcfg.pp_numfon == 21  || cartcfg.pp_numfon == 0  || cartcfg.pp_numfon == 1  || cartcfg.pp_numfon == 7  || cartcfg.pp_numfon == 28  || cartcfg.pp_numfon == 9  || cartcfg.pp_numfon == 2  || cartcfg.pp_numfon == 19  || cartcfg.pp_numfon == 11  || cartcfg.pp_numfon == 3  || cartcfg.pp_numfon == 30  || cartcfg.pp_numfon == 17  || cartcfg.pp_numfon == 10  || cartcfg.pp_numfon == 24  || cartcfg.pp_numfon == 12  || cartcfg.pp_numfon == 13  || cartcfg.pp_numfon == 4  || cartcfg.pp_numfon == 6  || cartcfg.pp_numfon == 26  || cartcfg.pp_numfon == 27  || cartcfg.pp_numfon == 16  || cartcfg.pp_numfon == 8  || cartcfg.pp_numfon == 15  || cartcfg.pp_numfon == 5  || cartcfg.pp_numfon == 22 )
						{
							//TerminalNode13
							mean=3.315000;
						}
						else
						{
							//TerminalNode14
							mean=11.489000;
						}
					}
					else
					{
						//Node19
						if( cartcfg.p_numfon == 18  || cartcfg.p_numfon == 20  || cartcfg.p_numfon == 23  || cartcfg.p_numfon == 21  || cartcfg.p_numfon == 0  || cartcfg.p_numfon == 1  || cartcfg.p_numfon == 2  || cartcfg.p_numfon == 19  || cartcfg.p_numfon == 11  || cartcfg.p_numfon == 3  || cartcfg.p_numfon == 30  || cartcfg.p_numfon == 10  || cartcfg.p_numfon == 12  || cartcfg.p_numfon == 13  || cartcfg.p_numfon == 4  || cartcfg.p_numfon == 6  || cartcfg.p_numfon == 26  || cartcfg.p_numfon == 16  || cartcfg.p_numfon == 8  || cartcfg.p_numfon == 5 )
						{
							//Node20
							if( cartcfg.p_numfon == 18  || cartcfg.p_numfon == 20  || cartcfg.p_numfon == 23  || cartcfg.p_numfon == 21  || cartcfg.p_numfon == 19  || cartcfg.p_numfon == 11  || cartcfg.p_numfon == 3  || cartcfg.p_numfon == 30  || cartcfg.p_numfon == 10  || cartcfg.p_numfon == 12  || cartcfg.p_numfon == 4  || cartcfg.p_numfon == 6  || cartcfg.p_numfon == 26  || cartcfg.p_numfon == 16  || cartcfg.p_numfon == 8 )
							{
								//Node21
								if( cartcfg.nn_numfon == 18  || cartcfg.nn_numfon == 23  || cartcfg.nn_numfon == 25  || cartcfg.nn_numfon == 21  || cartcfg.nn_numfon == 1  || cartcfg.nn_numfon == 7  || cartcfg.nn_numfon == 9  || cartcfg.nn_numfon == 2  || cartcfg.nn_numfon == 19  || cartcfg.nn_numfon == 11  || cartcfg.nn_numfon == 30  || cartcfg.nn_numfon == 10  || cartcfg.nn_numfon == 6  || cartcfg.nn_numfon == 27  || cartcfg.nn_numfon == 16  || cartcfg.nn_numfon == 8  || cartcfg.nn_numfon == 22 )
								{
									//TerminalNode15
									mean=5.326000;
								}
								else
								{
									//TerminalNode16
									mean=6.542000;
								}
							}
							else
							{
								//TerminalNode17
								mean=6.394000;
							}
						}
						else
						{
							//TerminalNode18
							mean=10.360000;
						}
					}
				}
				else
				{
					//Node22
					if( cartcfg.n_numfon == 18  || cartcfg.n_numfon == 13  || cartcfg.n_numfon == 6  || cartcfg.n_numfon == 8  || cartcfg.n_numfon == 15 )
					{
						//Node23
						if( cartcfg.p_numfon == 18  || cartcfg.p_numfon == 20  || cartcfg.p_numfon == 23  || cartcfg.p_numfon == 21  || cartcfg.p_numfon == 0  || cartcfg.p_numfon == 1  || cartcfg.p_numfon == 2  || cartcfg.p_numfon == 19  || cartcfg.p_numfon == 11  || cartcfg.p_numfon == 10  || cartcfg.p_numfon == 24  || cartcfg.p_numfon == 12  || cartcfg.p_numfon == 4  || cartcfg.p_numfon == 6  || cartcfg.p_numfon == 26  || cartcfg.p_numfon == 16  || cartcfg.p_numfon == 8  || cartcfg.p_numfon == 5 )
						{
							//TerminalNode19
							mean=6.975000;
						}
						else
						{
							//TerminalNode20
							mean=7.724000;
						}
					}
					else
					{
						//Node24
						if( cartcfg.p_numfon == 20  || cartcfg.p_numfon == 23  || cartcfg.p_numfon == 21  || cartcfg.p_numfon == 0  || cartcfg.p_numfon == 19  || cartcfg.p_numfon == 11  || cartcfg.p_numfon == 30  || cartcfg.p_numfon == 24  || cartcfg.p_numfon == 12  || cartcfg.p_numfon == 4  || cartcfg.p_numfon == 6  || cartcfg.p_numfon == 26  || cartcfg.p_numfon == 16  || cartcfg.p_numfon == 8  || cartcfg.p_numfon == 5  || cartcfg.p_numfon == 31 )
						{
							//TerminalNode21
							mean=7.140000;
						}
						else
						{
							//TerminalNode22
							mean=7.965000;
						}
					}
				}
			}
			else
			{
				//Node34
				if( cartcfg.nn_numfon == 18  || cartcfg.nn_numfon == 20  || cartcfg.nn_numfon == 14  || cartcfg.nn_numfon == 1  || cartcfg.nn_numfon == 9  || cartcfg.nn_numfon == 2  || cartcfg.nn_numfon == 29  || cartcfg.nn_numfon == 19  || cartcfg.nn_numfon == 11  || cartcfg.nn_numfon == 30  || cartcfg.nn_numfon == 17  || cartcfg.nn_numfon == 24  || cartcfg.nn_numfon == 12  || cartcfg.nn_numfon == 13  || cartcfg.nn_numfon == 4  || cartcfg.nn_numfon == 6  || cartcfg.nn_numfon == 26  || cartcfg.nn_numfon == 16  || cartcfg.nn_numfon == 8  || cartcfg.nn_numfon == 5  || cartcfg.nn_numfon == 31  || cartcfg.nn_numfon == 22 )
				{
					//TerminalNode38
					mean=7.522000;
				}
				else
				{
					//Node40
					if( cartcfg.pp_vfrt == 0 || cartcfg.pp_vfrt == 2 || cartcfg.pp_vfrt == 3)
					{
						//TerminalNode39
						mean=8.396000;
					}
					else
					{
						//TerminalNode40
						mean=10.420000;
					}
				}
			}
		}
		else
		{
			//Node35
			if( cartcfg.nn_numfon == 18  || cartcfg.nn_numfon == 20  || cartcfg.nn_numfon == 23  || cartcfg.nn_numfon == 14  || cartcfg.nn_numfon == 25  || cartcfg.nn_numfon == 21  || cartcfg.nn_numfon == 1  || cartcfg.nn_numfon == 7  || cartcfg.nn_numfon == 2  || cartcfg.nn_numfon == 29  || cartcfg.nn_numfon == 19  || cartcfg.nn_numfon == 11  || cartcfg.nn_numfon == 3  || cartcfg.nn_numfon == 30  || cartcfg.nn_numfon == 10  || cartcfg.nn_numfon == 24  || cartcfg.nn_numfon == 12  || cartcfg.nn_numfon == 13  || cartcfg.nn_numfon == 4  || cartcfg.nn_numfon == 6  || cartcfg.nn_numfon == 26  || cartcfg.nn_numfon == 27  || cartcfg.nn_numfon == 16  || cartcfg.nn_numfon == 8  || cartcfg.nn_numfon == 15  || cartcfg.nn_numfon == 5  || cartcfg.nn_numfon == 31  || cartcfg.nn_numfon == 22 )
			{
				//Node36
				if( cartcfg.p_numfon == 18  || cartcfg.p_numfon == 20  || cartcfg.p_numfon == 21  || cartcfg.p_numfon == 1  || cartcfg.p_numfon == 2  || cartcfg.p_numfon == 19  || cartcfg.p_numfon == 3  || cartcfg.p_numfon == 4  || cartcfg.p_numfon == 16  || cartcfg.p_numfon == 5 )
				{
					//Node37
					if( cartcfg.n_numfon == 14  || cartcfg.n_numfon == 1  || cartcfg.n_numfon == 9  || cartcfg.n_numfon == 2  || cartcfg.n_numfon == 3  || cartcfg.n_numfon == 17  || cartcfg.n_numfon == 26  || cartcfg.n_numfon == 22 )
					{
						//TerminalNode34
						mean=8.848000;
					}
					else
					{
						//TerminalNode35
						mean=9.123000;
					}
				}
				else
				{
					//TerminalNode36
					mean=9.389000;
				}
			}
			else
			{
				//TerminalNode37
				mean=10.017000;
			}
		}
	}
	else
	{
		//Node38
		if( cartcfg.p_numfon == 19  || cartcfg.p_numfon == 11  || cartcfg.p_numfon == 16 )
		{
			//Node39
			if( cartcfg.nn_numfon == 18  || cartcfg.nn_numfon == 23  || cartcfg.nn_numfon == 14  || cartcfg.nn_numfon == 25  || cartcfg.nn_numfon == 21  || cartcfg.nn_numfon == 0  || cartcfg.nn_numfon == 1  || cartcfg.nn_numfon == 7  || cartcfg.nn_numfon == 9  || cartcfg.nn_numfon == 29  || cartcfg.nn_numfon == 11  || cartcfg.nn_numfon == 3  || cartcfg.nn_numfon == 17  || cartcfg.nn_numfon == 10  || cartcfg.nn_numfon == 24  || cartcfg.nn_numfon == 12  || cartcfg.nn_numfon == 13  || cartcfg.nn_numfon == 6  || cartcfg.nn_numfon == 26  || cartcfg.nn_numfon == 27  || cartcfg.nn_numfon == 16  || cartcfg.nn_numfon == 8  || cartcfg.nn_numfon == 15  || cartcfg.nn_numfon == 5  || cartcfg.nn_numfon == 31  || cartcfg.nn_numfon == 22 )
			{
				//Node42
				if( cartcfg.n_numfon == 14  || cartcfg.n_numfon == 1  || cartcfg.n_numfon == 9  || cartcfg.n_numfon == 2  || cartcfg.n_numfon == 24  || cartcfg.n_numfon == 26  || cartcfg.n_numfon == 22 )
				{
					//Node43
					if( cartcfg.npossil == 3  || cartcfg.npossil == 0 )
					{
						//Node44
						if( cartcfg.numfon == 21  || cartcfg.numfon == 19  || cartcfg.numfon == 16 )
						{
							//Node45
							if( cartcfg.n_numfon == 24  || cartcfg.n_numfon == 22 )
							{
								//TerminalNode41
								mean=8.371000;
							}
							else
							{
								//Node46
								if( cartcfg.nn_numfon == 20  || cartcfg.nn_numfon == 21  || cartcfg.nn_numfon == 7  || cartcfg.nn_numfon == 9  || cartcfg.nn_numfon == 29  || cartcfg.nn_numfon == 19  || cartcfg.nn_numfon == 11  || cartcfg.nn_numfon == 17  || cartcfg.nn_numfon == 24  || cartcfg.nn_numfon == 12  || cartcfg.nn_numfon == 13  || cartcfg.nn_numfon == 6  || cartcfg.nn_numfon == 27  || cartcfg.nn_numfon == 16  || cartcfg.nn_numfon == 8  || cartcfg.nn_numfon == 15 )
								{
									//TerminalNode42
									mean=8.806000;
								}
								else
								{
									//TerminalNode43
									mean=9.216000;
								}
							}
						}
						else
						{
							//TerminalNode44
							mean=9.991000;
						}
					}
					else
					{
						//Node47
						if(cartcfg.nsilutt <= 26.50)
						{
							//Node48
							if( cartcfg.pp_numfon == 20  || cartcfg.pp_numfon == 7  || cartcfg.pp_numfon == 9  || cartcfg.pp_numfon == 2  || cartcfg.pp_numfon == 19  || cartcfg.pp_numfon == 11  || cartcfg.pp_numfon == 13  || cartcfg.pp_numfon == 4  || cartcfg.pp_numfon == 6  || cartcfg.pp_numfon == 16  || cartcfg.pp_numfon == 8  || cartcfg.pp_numfon == 5 )
							{
								//TerminalNode45
								mean=9.700000;
							}
							else
							{
								//TerminalNode46
								mean=10.197000;
							}
						}
						else
						{
							//Node49
							if( cartcfg.numfon == 21  || cartcfg.numfon == 19  || cartcfg.numfon == 16 )
							{
								//TerminalNode47
								mean=9.103000;
							}
							else
							{
								//TerminalNode48
								mean=9.924000;
							}
						}
					}
				}
				else
				{
					//Node50
					if(cartcfg.npalutt <= 12.50)
					{
						//Node51
						if( cartcfg.npossil == 3  || cartcfg.npossil == 0 )
						{
							//Node52
							if( cartcfg.n_numfon == 3  || cartcfg.n_numfon == 17  || cartcfg.n_numfon == 4  || cartcfg.n_numfon == 31 )
							{
								//TerminalNode49
								mean=9.694000;
							}
							else
							{
								//TerminalNode50
								mean=10.222000;
							}
						}
						else
						{
							//Node53
							if( cartcfg.p_numfon == 10  || cartcfg.p_numfon == 12  || cartcfg.p_numfon == 13  || cartcfg.p_numfon == 8 )
							{
								//TerminalNode51
								mean=9.791000;
							}
							else
							{
								//TerminalNode52
								mean=10.531000;
							}
						}
					}
					else
					{
						//Node54
						if( cartcfg.p_numfon == 23  || cartcfg.p_numfon == 21  || cartcfg.p_numfon == 0  || cartcfg.p_numfon == 10  || cartcfg.p_numfon == 13  || cartcfg.p_numfon == 4  || cartcfg.p_numfon == 8 )
						{
							//TerminalNode53
							mean=9.336000;
						}
						else
						{
							//Node55
							if( cartcfg.nn_numfon == 18  || cartcfg.nn_numfon == 20  || cartcfg.nn_numfon == 14  || cartcfg.nn_numfon == 25  || cartcfg.nn_numfon == 21  || cartcfg.nn_numfon == 7  || cartcfg.nn_numfon == 9  || cartcfg.nn_numfon == 29  || cartcfg.nn_numfon == 19  || cartcfg.nn_numfon == 3  || cartcfg.nn_numfon == 30  || cartcfg.nn_numfon == 17  || cartcfg.nn_numfon == 10  || cartcfg.nn_numfon == 13  || cartcfg.nn_numfon == 6  || cartcfg.nn_numfon == 8  || cartcfg.nn_numfon == 15  || cartcfg.nn_numfon == 31 )
							{
								//TerminalNode54
								mean=9.480000;
							}
							else
							{
								//TerminalNode55
								mean=9.908000;
							}
						}
					}
				}
			}
			else
			{
				//Node56
				if( cartcfg.ct == 1 || cartcfg.ct == 2)
				{
					//TerminalNode56
					mean=10.972000;
				}
				else
				{
					//TerminalNode57
					mean=12.367000;
				}
			}
		}
		else
		{
			//Node41
			if( cartcfg.nn_numfon == 18  || cartcfg.nn_numfon == 20  || cartcfg.nn_numfon == 23  || cartcfg.nn_numfon == 14  || cartcfg.nn_numfon == 25  || cartcfg.nn_numfon == 21  || cartcfg.nn_numfon == 1  || cartcfg.nn_numfon == 7  || cartcfg.nn_numfon == 9  || cartcfg.nn_numfon == 2  || cartcfg.nn_numfon == 29  || cartcfg.nn_numfon == 19  || cartcfg.nn_numfon == 11  || cartcfg.nn_numfon == 3  || cartcfg.nn_numfon == 30  || cartcfg.nn_numfon == 17  || cartcfg.nn_numfon == 10  || cartcfg.nn_numfon == 24  || cartcfg.nn_numfon == 12  || cartcfg.nn_numfon == 13  || cartcfg.nn_numfon == 4  || cartcfg.nn_numfon == 6  || cartcfg.nn_numfon == 26  || cartcfg.nn_numfon == 27  || cartcfg.nn_numfon == 16  || cartcfg.nn_numfon == 8  || cartcfg.nn_numfon == 15  || cartcfg.nn_numfon == 5  || cartcfg.nn_numfon == 31  || cartcfg.nn_numfon == 22 )
			{
				//TerminalNode23
				mean=7.224000;
			}
			else
			{
				//TerminalNode24
				mean=8.400000;
			}
		}
	}
}
}
	return mean;
}
/**********************************************************/

VOID LangES_Prosod::utt_dur2(UttPh & ut)
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
