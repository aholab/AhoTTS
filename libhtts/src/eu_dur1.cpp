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

Nombre fuente................ eu_dur1.cpp
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
Metodos de {{LangEU_Prosod}} que implementan diferentes
modelos de duracion. Este es un modelo sencillo.
</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/


#include <math.h>
#include "eu_lingp.hpp"
#include "xx_uti.hpp"
#include "httsmsg.h"
#include "spl.h"

#ifdef HTTS_PROSOD_EU_DUR1
/**********************************************************/
/* modelo de duracion. Se trabaja en unidades log2 */

typedef struct {  // debe tener media 0, desviacion aprox. 1.
	DOUBLE KDurInitial;  // inicial
	DOUBLE KDurFinal;    // final
	DOUBLE KDurStress;   // incr. en acentos propios
	DOUBLE KDurEmphasis; // incr. en acentos prosodicos
	DOUBLE KDurSOU;      // incr. en Start of Utterance
	DOUBLE KDurEOU;      // incr. en End of Utterance
} UttCfg;

PRIVATE UttCfg utt_all = { 0.3, -0.3, 0.4, 0.5, 0.0, 0.3 };

/**********************************************************/

PRIVATE DOUBLE dur_pause( Phone ph, Pause pautype )
{
	DOUBLE l;

	switch (pautype) {
	case UPAUSE_UBEGIN:
	case UPAUSE_UEND: l=40; break;

	case UPAUSE_SHORT: l=70; break;
	case UPAUSE_LONG: l=90; break;

	case UPAUSE_SDECL:
	case UPAUSE_SQUEST:
	case UPAUSE_SEXCL:
	case UPAUSE_SPAUSE:
	case UPAUSE_SNEUTR: l=80; break;

	default: l=30; break;  //  aqui entran los silencios que van al rededor del PH_breath
	}

	if (ph==PH_breath) l*=2;
	return l;
}

/**********************************************************/
/* asignacion duraciones definitivas (en milisegundos). */

PRIVATE VOID utt_dur_unnorm(UttPh & ut)
{
	UttI p;
	DOUBLE d;

	for (p = ut.phoneFirst(); p != 0; p = ut.phoneNext(p)) {
		switch (ut.cell(p).getPhone()) {
		case PH_sil:
		case PH_breath: d=dur_pause(ut.cell(p).getPhone(),ut.cell(p).getPause());  break;
		case PH_p:   d=100;  break;
		case PH_B:
		case PH_b:   d= 65;  break;
		case PH_t:   d= 85;  break;
		case PH_D:
		case PH_d:   d= 60;  break;
		case PH_k:   d=100;  break;
		case PH_G:
		case PH_g:   d= 80;  break;
		case PH_m:   d= 70;  break;
		case PH_n:   d= 80;  break;
		case PH_J:   d=110;  break;
		case PH_tS:  d=135;  break;
		case PH_f:   d=100;  break;
		case PH_T:   d=100;  break;
		case PH_s:   d=110;  break;
		case PH_jj:  d=100;  break;
		case PH_x:   d=130;  break;
		case PH_l:   d= 80;  break;
		case PH_L:   d=105;  break;
		case PH_rr:  d= 80;  break;
		case PH_r:   d= 30;  break;
		case PH_i:   d= 90;  break;
		case PH_j:   d= 60;  break;
		case PH_e:   d= 90;  break;
		case PH_a:   d= 90;  break;
		case PH_o:   d= 90;  break;
		case PH_u:   d= 90;  break;
		case PH_w:   d= 60;  break;

		case PH_S:
		case PH_X:   d=110;  break;
		case PH_ts:
		case PH_tZ:  d=135;  break;
		case PH_Q:   d= 85;  break;
		case PH_dj:  d=100;  break;

		default:
		//	htts_warn("eu_dur1: Phone not known (%s).",phone_tosampa(ut.cell(p).getPhone()));
			d=90; break;
		}
		ut.cell(p).setDur(d*ut.cell(p).getDur());
	}
}

/**********************************************************/

VOID LangEU_Prosod::utt_dur1(UttPh & ut)
{
	UttI p;
	LONG N;
	DOUBLE kacc, kstep, k;

	N = ut.phoneN();	// numero de fonemas

	kacc = utt_all.KDurInitial;		// duracion inicial
	kstep = (N > 1) ? (utt_all.KDurFinal - kacc) / (N - 1) : 0;		// incr. entre fonemas

	for (p = ut.phoneFirst(); p != 0; p = ut.phoneNext(p)) {
		k = kacc;	// valor base
		// modificar segun stress y emphasis
		if (ut.cell(p).getStress() != USTRESS_NONE) k+=utt_all.KDurStress;
		if (ut.cell(p).getEmphasis() != UEMPHASIS_NONE) k+=utt_all.KDurEmphasis;
		ut.cell(p).setDur(k);
		kacc += kstep;	// valor base para siguiente fonema
	}

	// primera silaba de la utt. (realmente solo semisilaba inicial)
	for (p = ut.phoneFirst(); p != 0; p = ut.phoneNext(p, URANGE_SYLLABLE)) {
		ut.cell(p).setDur(utt_all.KDurSOU+ut.cell(p).getDur());
		// al llegar a una vocal, ya hemos procesado la semisilaba inicial
		if (LangEU_PhUti::phIsVowel(ut.cell(p).getPhone()))
			break;
	}

	// ultima silaba de la utt. (realmente solo semisilaba final)
	for (p = ut.phoneLast(); p != 0; p = ut.phonePrev(p, URANGE_SYLLABLE)) {
		ut.cell(p).setDur(utt_all.KDurEOU+ut.cell(p).getDur());
		// al llegar a una vocal, ya hemos procesado la semisilaba final
		if (LangEU_PhUti::phIsVowel(ut.cell(p).getPhone()))
			break;
	}

	for (p = ut.phoneFirst(); p != 0; p = ut.phoneNext(p)) {
		if (ut.cell(p).getPhone()==PH_sil)
			ut.cell(p).setDur(0);  // silencios: duracion media
	}

	/* pasar de db2 a lineal la duracion, aplicando media {dur_mean} y varianza {dur_dev} */
	DOUBLE durd=lin2log2(dur_dev+1.0);
	for (p=ut.phoneFirst(); p!=0; p=ut.phoneNext(p))
		ut.cell(p).setDur(dur_mean*log22lin(ut.cell(p).getDur()*durd));

	// desnormalizar a milisegundos duraciones
	utt_dur_unnorm(ut);
}

/**********************************************************/
#endif







