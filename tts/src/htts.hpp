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
#ifndef __HTTS_HPP__
#define __HTTS_HPP__

/**********************************************************/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/*
(C) 1997 TSR/Aholab - ETSII/IT Bilbao (UPV/EHU)

Nombre fuente................ htts.hpp
Nombre paquete............... aHoTTS
Lenguaje fuente.............. C++
Estado....................... -
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Borja Etxebarria
.............................

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
1.1.0    02/10/11  inaki     add transcription API
1.0.0    31/01/00  borja     codefreeze aHoTTS v1.0
0.0.0    06/02/98  borja     Codificacion inicial.

======================== Contenido ========================
<DOC>
</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include <stdarg.h>
#include "tdef.h"
#include "htts_cfg.h"

/**********************************************************/

#ifdef HTTS_INTERFACE_WAVEMARKS
#include "mark.hpp"
#endif

#ifdef HTTS_METHVARS
extern BOOL __htts_method_formant;
extern BOOL __htts_method_franky;
extern BOOL __htts_method_tdpsola;
extern BOOL __htts_method_lppsola;
#endif

#ifdef HTTS_LANGVARS
extern BOOL __htts_lang_es;
extern BOOL __htts_lang_eu;
#endif

/**********************************************************/

enum { HTTS_CB_NOCALL=0, HTTS_CB_CALL1=1, HTTS_CB_BLOCKCALL=-1 };

/**********************************************************/

class HTTSDo;

class HTTS_DB;

/**********************************************************/

class HTTS {
private:
	HTTSDo* data;

public:
	HTTS( VOID );
	virtual ~HTTS( );
	BOOL create( VOID );
	BOOL create( HTTS_DB *db );
	HTTS_DB *getDB(VOID);

	INT input( const CHAR * str );
	BOOL input( CHAR ch );
	BOOL flush( VOID );
	/************/
	//inaki
	INT input_multilingual( const CHAR * str, const CHAR *lang , const CHAR *data_path, BOOL InputIsFile = FALSE );
	int output_multilingual(const CHAR *lang, short **samples);
	//const DOUBLE * output_multilingual();
	//BOOL outack_multilingual();
	/***********/
	const DOUBLE* output( INT *len, BOOL *flush=NULL, INT mode=0, VOID *cb_n=0 );
	BOOL isFlush( VOID );

#ifdef HTTS_INTERFACE_WAVEMARKS
	const Mark1DList & mrkget( VOID );
	VOID mrkdel( VOID );
	BOOL outack( INT n, BOOL mrkdel=TRUE );
#else
	BOOL outack( INT n );
#endif

#ifdef HTTS_TIMEEVS
	INT get_timeev( VOID );
#endif

	virtual VOID callback( VOID * /*cb_n*/ ) { };

	BOOL set( const CHAR* param, const CHAR* val );
	BOOL set( const CHAR* param, DOUBLE val );
	BOOL set( const CHAR* param, INT val );
	const CHAR* get( const CHAR* param );
	const CHAR* get( const CHAR* param, DOUBLE *val );

	DOUBLE getSRate( VOID );

	static INT16 convDOUBLE2INT16( DOUBLE d );
	static INT16 convDOUBLE2INT16_clip( DOUBLE d );
	static VOID convDOUBLE2INT16( DOUBLE d[], INT16 i16[], INT n );
	static BOOL convDOUBLE2INT16_clip( DOUBLE d[], INT16 i16[], INT n );
};

/**********************************************************/

typedef VOID HTTS_MsgFunc( const CHAR *fmt, va_list arglist );

VOID HTTS_SetErrorFunc( HTTS_MsgFunc *f );
VOID HTTS_SetWarnFunc( HTTS_MsgFunc *f );

/**********************************************************/
/* Estos defines son orientativos, pero conviene no superarlos.
Indican los limites que no deberian superarse para los
parametros PthMean, PowMean, DurMean */

#define HTTS_PTH_MIN 20
#define HTTS_PTH_MAX 500

#define HTTS_POW_MIN 0
#define HTTS_POW_MAX 10

#define HTTS_DUR_MIN 0.1
#define HTTS_DUR_MAX 10

/* Idem desviaciones estandar */
#define HTTS_DEV_MIN 0
#define HTTS_DEV_MAX 1

/**********************************************************/

#endif

