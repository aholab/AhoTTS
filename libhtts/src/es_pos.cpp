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

Nombre fuente................ es_pos.cpp
Nombre paquete............... aHoTTS
Lenguaje fuente.............. C++
Estado....................... -
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Inma Sardon
Recodificacion............... inigos

Version  dd/mm/aa  Autor     Comentario
-------  --------  --------  ----------
1.0.1    03/10/07  Inaki     define MODULO_1 y _2 para dividir el sinte en 3 modulos
1.0.0    09/01/04  inigos    Adaptacion castellano
0.1.0    24/11/97  Borja     Codificacion inicial (copia euskera)

======================== Contenido ========================
<DOC>
</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/
#include "uti.h"
#include "uttws.hpp"
#include "httsmsg.h"
#include "uttws.hpp"
#include "uttph.hpp"
#include "es_lingp.hpp"
//#include "aholib.hpp"

/**********************************************************/

LangES_POS::LangES_POS( VOID )
{
	created=FALSE;
}

/**********************************************************/

LangES_POS::~LangES_POS( VOID )
{
}


/**********************************************************/

BOOL LangES_POS::create( VOID )
{
	BOOL ret=TRUE;
	created=TRUE;

	#ifndef MODULO_2 //IÑAKI
	ret = ret && categ.create();
	#endif
	#ifndef MODULO_1 //IÑAKI
	ret = ret && grupof.create();
	#endif

	return ret ;
}

/**********************************************************/

BOOL LangES_POS::set( const CHAR *param, const CHAR * value )
{
	BOOL ret=FALSE;
/*	if (!strcmp(param,"PhTSimple")) {
		phtr.setPhTSimple(str2bool(value,TRUE));
		return TRUE;
	}*/
	#ifndef MODULO_2 //IÑAKI
	ret = ret || categ.set(param,value);
	#endif
	#ifndef MODULO_1 //IÑAKI	
	ret = ret || grupof.set(param,value);
	#endif
	return ret;
}

/**********************************************************/

const CHAR *LangES_POS::get( const CHAR *param )
{
	const CHAR *s;
	#ifndef MODULO_2 //IÑAKI
	s=categ.get(param); if (s) return s;
	#endif
	#ifndef MODULO_1 //IÑAKI
	s=grupof.get(param); if (s) return s;
	#endif
	return NULL;
}

/**********************************************************/

/**********************************************************/

VOID LangES_POS::utt_pos( UttWS &ut )
{
	assert(created);
	#ifndef MODULO_2 //IÑAKI
	categ.utt_categ(ut);
	#endif
	#ifndef MODULO_1 //IÑAKI
	grupof.utt_gf(ut);
	#endif
}

/**********************************************************/

