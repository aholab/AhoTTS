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

Nombre fuente................ phone.c
Nombre paquete............... aHoTTS
Lenguaje fuente.............. C++
Estado....................... -
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Borja Etxebarria
.............................

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
1.1.0    20/10/08  inaki     Añadir fonemas Inglés
1.0.0    31/01/00  borja     codefreeze aHoTTS v1.0
0.0.0    24/11/97  borja     Codificacion inicial.

======================== Contenido ========================
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include "phone.h"
#include "uti.h"
#include "httsmsg.h"

/**********************************************************/

typedef struct {
	Phone phone;
	const CHAR *sampa;
} PhoneInfo;

/**********************************************************/

PhoneInfo phinfo[] = {
	{ PH_sil, "_" },
	{ PH_sic, "+" },
	{ PH_breath, "~" },
	{ PH_p, "p" },
	{ PH_b, "b" },
	{ PH_t, "t" },
	{ PH_d, "d" },
	{ PH_k, "k" },
	{ PH_g, "g" },
	{ PH_m, "m" },
	{ PH_n, "n" },
	{ PH_J, "J" },
	{ PH_tS,"tS" },
	{ PH_B, "B" },
	{ PH_f, "f" },
	{ PH_T, "T" },
	{ PH_D, "D" },
	{ PH_s, "s" },
	{ PH_jj,"jj" },
	{ PH_x, "x" },
	{ PH_G, "G" },
	{ PH_l, "l" },
	{ PH_L, "L" },
	{ PH_r, "r" },
	{ PH_rr,"rr" },
	{ PH_i, "i" },
	{ PH_j, "j" },
	{ PH_e, "e" },
	{ PH_a, "a" },
	{ PH_o, "o" },
	{ PH_u, "u" },
	{ PH_w, "w" },
	{ PH_S, "S" },
	{ PH_ts,"ts" },
	{ PH_dj,"gj" },
	{ PH_X, "s`" },
	{ PH_tZ,"ts`" },
	{ PH_Q, "c" },
	//{ PH_dZ,"Z" }, //BLIZZARD
	
	/* maria */	
	{ PH_dZ_, "dZ" },
	{ PH_v, "v" },
	{ PH_z, "z" },
	{ PH_Z, "Z" },
	{ PH_h, "h" },
	{ PH_N, "N" },
	{ PH_I, "I" },
	{ PH_IA, "I@" },
	{ PH_eA, "e@" },
	{ PH_a_, "{" },
	{ PH_Q_, "Q" },
	{ PH_A_, "A:" },
	{ PH_V, "V" },
	{ PH_U, "U" },
	{ PH_UA, "U@" },
	{ PH_A, "@" },
	{ PH_i_, "i:" },
	{ PH_eI, "eI" },
	{ PH_aI, "aI" },
	{ PH_OI, "OI" },
	{ PH_AU, "@U" },
	{ PH_aU, "aU" },
	{ PH_3_, "3:" },
	{ PH_O_, "O:" },
	{ PH_u_, "u:" },
	/*********** unilex BLIZZARD2008*/
	{ PH_lw, "lw" },
	{ PH_nx, "nx" },
	{ PH_uw, "uw" },
	{ PH_lx, "lx" },
	{ PH_mx, "mx" },
	{ PH_y, "y" }, //iparralde, u afrancesada
	/* ****** */
	{ PH_none, " " }  /* este siempre el ultimo */
};

/**********************************************************/

const CHAR * phone_tosampa( Phone ph )
{
	INT i=-1;

	do {
		i++;
		if (phinfo[i].phone==ph) return phinfo[i].sampa;
	} while (phinfo[i].phone!=PH_none);
	htts_error("Phone code not known (%ld [%c])",(long)ph,(char)ph);
	return NULL;
}

/**********************************************************/

Phone phone_fromsampa( const CHAR *sampa )
{
	INT i=-1;

	do {
		i++;
		if (!strcmp(phinfo[i].sampa,sampa)) return phinfo[i].phone;
	} while (phinfo[i].phone!=PH_none);
	htts_error("SAMPA phone name not known (%s)",sampa);
	return PH_none;
}

/**********************************************************/
