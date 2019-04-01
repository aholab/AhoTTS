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

Nombre fuente................ eu_syl.cpp
Nombre paquete............... aHoTTS
Lenguaje fuente.............. C++
Estado....................... -
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Borja Etxebarria
............................. Ana Armentia

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
1.0.0    31/01/00  borja     codefreeze aHoTTS v1.0
0.0.0    24/11/97  borja     Codificacion inicial.

======================== Contenido ========================
<DOC>
Metodos de {{LangEU_PhTrans}} que silabifican las palabras. Hace falta
para luego poner el acento en la silaba adecuada, detectar diptongos, etc.
</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include <string.h>

#include "eu_lingp.hpp"

/**********************************************************/
/* {wordp} debe ser el primer fonema de la palabra */

VOID LangEU_PhTrans::word_syllab(UttPh & u, UttI wordp)
{
#define SETSYL(p) u.cell(p).setSyllable(TRUE)
#define PH(p)  u.cell(p).getPhone()
#define ACC(p) u.cell(p).getStress()
#define ESVOCAL(ch) (LangEU_PhUti::phIsVowel(ch))
#define NOESVOCAL(ch) (! LangEU_PhUti::phIsVowel(ch))
#define CCVALIDO(c1,c2) (LangEU_PhUti::phIsValidCC(c1,c2))
#define NEXT(p) u.phoneNext(p,URANGE_WORD)
// SPREV: anterior, pero dentro de la silaba
#define SPREV(p) u.phonePrev(p,URANGE_SYLLABLE)

	UttI i, i2, i3;
	Phone ph, ph2, ph3;

	wordp = u.phoneFirst(wordp,URANGE_WORD);  // primer fonema

	for (i = wordp; i != 0; i = NEXT(i)) {
		ph = PH(i);		// fonema actual

		if (NOESVOCAL(ph))
			continue;	// mientras no sea vocal, avanza

		i2 = SPREV(i);	// fonema anterior en silaba

		if (!i2) {
			SETSYL(i);
			continue;
		}	// #V -> #-V  (#=comienzo palabra/silaba)

		ph2 = PH(i2);	// fonema anterior

		i3 = SPREV(i2);		// fonema anterior de anterior en silaba    

		if (NOESVOCAL(ph2)) {	// CV

			if (!i3) {
				SETSYL(i2);
				continue;
			}	// #CV -> #-CV

			ph3 = PH(i3);
			if (ESVOCAL(ph3)) {
				SETSYL(i2);
				continue;
			}	// VCV -> V-CV
			// CCV: comprobamos si el grupo CC es valido

			if (CCVALIDO(ph3, ph2)) {
				SETSYL(i3);
				continue;
			}	// CCV -> -CCV

			SETSYL(i2);
			continue;	// CCV -> C-CV  (ver nota1 abajo)

		}
		else {	// VV

			if (i3) {
				ph3 = PH(i3);
				if (ESVOCAL(ph3)) {		// VVV

					/* Ana: comentado el bloque de triptongos, que no hay. */
					/*
						if (LangEU_PhUti::phIsTriptongo(ph3, ph2, ph, ACC(i3), ACC(i))) {		// VVV triptongo

						i3 = SPREV(i3);		// retrocedemos uno mas en la silaba...

						if (!i3)
							continue;	// #VVV  es triptongo

						if (NOESVOCAL(PH(i3)))
							continue;	// CVVV  es triptongo
						// VVVV -> VVV-V  salimos del if de triptongo, para evitar cuatrongos :)

					}*/
					SETSYL(i);
					continue;	// no era triptongo: VVV -> VV-V

				}
			}	// llegamos aqui si #VV o CVV. comprobamos diptongos

			if (LangEU_PhUti::phIsDiptongo(ph2, ph, ACC(i2), ACC(i)))
				continue;	// VV es diptongo

			SETSYL(i);
			continue;	// no era diptongo: VV -> V-V

		}
	}	// end for

	if (!wordp)  // puede haber palabras sin fonemas (ej: "h")
		return;

	// ver nota2
	if (!u.cell(wordp).getSyllable()) {		// no hay silaba al comienzo?

		i = u.syllableNext(wordp, URANGE_WORD);		// vamos al comienzo de la siguiente (que sera la "primera")

		if (i)
			u.cell(i).setSyllable(FALSE);	// quitamos la frontera de silaba (si la hay)

		SETSYL(wordp);	// y la corremos al principio de la palabra (1er fonema).

	}

#undef SETSYL
#undef PH
#undef ACC
#undef ESVOCAL
#undef NOESVOCAL
#undef NEXT
#undef SPREV

/* nota1: el resto de casos, las consonantes son de silabas diferentes,
	 se podria comprobar grupos CC especiales validos al principio de palabra,
   pero la regla que hay al final ya fuera del bucle principal 
   une ristras iniciales de consonantes a la primera silaba, asi que 
   no hace falta.
   ---
   nota2: si se ha dado un caso como este: #CCCCCV... se habra 
	 silabificado asi #CCCC-CV (o asi #CCC-CCV si CC es valido),
   y la ristra inicial de consonantes no pertenecen a ninguna
   silaba, asi que ahora lo arreglamos metiendo todo el paquete
   en la primera silaba. tambien manejamos el caso #CCCCC#: */
}

/**********************************************************/
