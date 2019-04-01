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

Nombre fuente................ eu_uti.cpp
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
Metodos de {{LangEU_PhUti}}, que se utilizan por otros
metodos de las clases {{LangEU_???}}: detectan grupos consonanticos
validos, detectan diptongos, etc.
</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include "xx_uti.hpp"

#include "eu_lingp.hpp"

/**********************************************************/

BOOL LangEU_PhUti::phIsVowel(Phone ph)
{
	return ((ph == PHEU_a) || (ph == PHEU_e) || (ph == PHEU_i)
		|| (ph == PHEU_o) || (ph == PHEU_u)
		|| (ph == PHEU_iaprox)|| (ph == PHEU_uaprox));
}

/**********************************************************/

BOOL LangEU_PhUti::phIsValidCC(Phone ph1, Phone ph2)
{
	static Phone lset[] =
	{PHEU_b, PHEU_baprox, PHEU_f, PHEU_g,PHEU_gaprox, PHEU_k, PHEU_p, PHEU_t, 0};
	static Phone rset[] =
	{PHEU_b, PHEU_baprox, PHEU_d,PHEU_daprox, PHEU_f, PHEU_g,PHEU_gaprox, PHEU_k, PHEU_p, PHEU_t, 0};

	if (ph2 == PHEU_l)
		return phSetContains(lset, ph1);	// grupos con la l

	if ((ph2 == PHEU_r)||(ph2 ==PHEU_rr))
		return phSetContains(rset, ph1);	// grupos con la r y con la rr

	return FALSE;
}

/**********************************************************/
/* diptongos: fuerte-debil, debil-fuerte, debil-debil,
	 [aeo+iu] [iu+eao] [i+u] [u+i], salvo si la debil esta
	 acentuada (iu, ui, si la primera esta acentuada no lo
	 tomo como diptongo). */

BOOL LangEU_PhUti::phIsDiptongo(Phone ph1, Phone ph2, Stress acc1, Stress acc2)
{
	BOOL iu1, iu2;

	iu1 = ((ph1 == PHEU_iaprox) || (ph1 == PHEU_uaprox));
	iu2 = ((ph2 == PHEU_iaprox) || (ph2 == PHEU_uaprox));

	if (iu1 && !iu2 && (acc1==USTRESS_NONE))
		return FALSE;	// df o df'

	if (!iu1 && iu2 && (acc2==USTRESS_NONE))
		return TRUE;	// fd o f'd

	/*if (iu1 && iu2 && (acc1==USTRESS_NONE))
		return TRUE;	//  dd o dd'*/

	return FALSE;;
}

/**********************************************************/
/* triptongos: debil-fuerte-debil: [iu+aeo+iu], salvo si
	 alguna debil esta acentuada */

/*BOOL LangEU_PhUti::phIsTriptongo(Phone ph1, Phone ph2, Phone ph3,
	Stress acc1, Stress acc3)
{
	BOOL iu1, iu2, iu3;

	iu1 = (ph1 == PHEU_i) || (ph1 == PHEU_u);
	iu2 = (ph2 == PHEU_i) || (ph2 == PHEU_u);
	iu3 = (ph3 == PHEU_i) || (ph3 == PHEU_u);

	if (iu1 && !iu2 && iu3 && (acc1==USTRESS_NONE) && (acc3==USTRESS_NONE))
		return TRUE;

	return FALSE;
}*/

/**********************************************************/
/* {devuelve} lindice a  vocal de la silaba. En diptongos y
	 demas, a la vocal fuerte, o a la acentuada si todas son debiles (iu),
	 o a la segunda si ninguna esta acentuada todavia */

UttI LangEU_PhUti::syllable_vowel(UttPh & u, UttI syl)
{
	UttI i, v;
	Phone p;

	i = u.phoneFirst(syl, URANGE_SYLLABLE);		// vamos al principio de silaba

	v = 0;

	while (i) {
		p = u.cell(i).getPhone();
		if ((p == PHEU_a) || (p == PHEU_e) || (p == PHEU_o))
			return i;	// si la vocal es fuerte, ya esta!

		if (((p == PHEU_i)/*||(p == PHEU_iaprox)*/||(p == PHEU_u)/*||(p == PHEU_uaprox)*/) &&		// si es debil y...
			 ((!v) || (u.cell(v).getStress() == USTRESS_NONE)))		// ... es la primera o la primera no estaba acentuada

			v = i;	// posicion de ultima vocal (debil) encontrada

		i = u.phoneNext(i, URANGE_SYLLABLE);
	}
	return v;
}

/**********************************************************/
/* como syllable_vowel, pero cuando no hay vocal, en vez de
NULL devuelve el mismo lindice {syl} */

UttI LangEU_PhUti::syllable_xvowel(UttPh & u, UttI syl)
{
	UttI v =syllable_vowel(u,syl);
	return v?v:syl;
}

/**********************************************************/
