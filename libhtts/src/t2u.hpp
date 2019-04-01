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
#ifndef __T2U_HPP__
#define __T2U_HPP__

/**********************************************************/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/*
(C) 1997 TSR/Aholab - ETSII/IT Bilbao (UPV/EHU)

Nombre fuente................ t2u.hpp
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
0.1.0    --/--/97  borja     codificacion inicial

======================== Contenido ========================
<DOC>
modulo de paso de texto a lista Utt
</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include "tdef.h"
#include "uttws.hpp"
#include "htts_cfg.h"

/**********************************************************/

#define T2ULST_MAXWORDLEN 40

/**********************************************************/

enum { T2U_CB_NOCALL=0, T2U_CB_CALL1=1, T2U_CB_BLOCKCALL=-1 };

/**********************************************************/

class T2ULst {
private:
	INT flushing;
	BOOL espera_outack;
	BOOL uttcompleta;
	enum { ModeUtt, ModeWord, ModeSpell } txtMode;

	CHAR buffin[T2ULST_MAXWORDLEN+1]; // +1 para 0 final
	INT punc;
	UttI sentence;

	UttWS *utt;

	VOID reset( VOID );
	INT entrada_cadena( const CHAR * );
	INT selectpunc( INT, INT, INT * );
	VOID updatepunc( UttI, INT );
	UttI uttappend( CHAR *, const CHAR *, INT, INT );

public:
	T2ULst( VOID );
	virtual ~T2ULst( );
	BOOL create( UttWS *ut );

	INT input( const CHAR * str );
    BOOL input( CHAR ch );
	BOOL flush( VOID );
	Utt * output( BOOL *flush, INT mode=0, VOID *cb_n=0 );
    VOID outack( VOID );
	virtual VOID callback( VOID * /*cb_x*/ ) { };

	BOOL set( const CHAR* param, const CHAR* val );
	const CHAR* get( const CHAR* param );
};

/**********************************************************/

#endif

