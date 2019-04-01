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

Nombre fuente................ eu_w2ph.cpp
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
Metodos de {{LangEU_PhTrans}} para gestionar la conversion de palabras
a fonemas (pasando por letras).
</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

//#define DEBUG() fprintf (stderr,"file..: %s -- line..: %d\n",__FILE__,__LINE__);
#define DEBUG()
#include "eu_lingp.hpp"
#include "xx_uti.hpp"

/**********************************************************/

VOID LangEU_PhTrans::utt_w2phtr(UttPh & ut)
{
	UttI wo;
	DEBUG()
	// primero expandimos las palabras a letras
	for (wo=ut.wordFirst(); wo!=0; ) {
		UttI wn=ut.wordNext(wo);  // pre-fetch de next. mas rapido porque aun no esta expandido
		ut.word2Char(wo);
		wo=wn;
	}
	DEBUG()
	// asegurar que hay alguna sentencia
	uttDefaultSentence(ut);
	DEBUG()
	// ahora, pausa a pausa, convertir letra -> fonema
	for (UttI se=ut.pausegrFirst(); se!=0; se=ut.pausegrNext(se))
		pausegr_ch2ph(ut, se);
	DEBUG()
	// silabificar palabra a palabra
	for (wo=ut.wordFirst(); wo!=0; wo=ut.wordNext(wo))
		word_syllab(ut, wo);
	DEBUG()
	iu2jw(ut); // detecta diptongos/triptongos
	DEBUG()
	//construir grupos acentuales,se trabaja de fgrp en fgrp
	for (UttI fg=ut.fgrpFirst(); fg!=0; fg=ut.fgrpNext(fg,URANGE_UTT))
		fgrp2agrp(ut,fg);
	//acentuar los grupos acentuales que no lo esten ya
	for (UttI ag=ut.agrpFirst(); ag!=0; ag=ut.agrpNext(ag)){
		DEBUG()
		if (!agrp_stressed(ut,ag)) agrp_stress(ut, ag);  //acentuar
		//no se else agrp_stress_text(ut,ag);//
		DEBUG()
	}
	uttPausePh(ut);  // fonemas de silencio en pausas
}

/**********************************************************/
