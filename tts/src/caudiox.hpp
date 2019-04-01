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
#ifndef __CAUDIOX_HPP__
#define __CAUDIOX_HPP__

/**********************************************************/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/*
Copyright: 1996 - Grupo de Voz (DAET) ETSII/IT-Bilbao

Nombre fuente................ -
Nombre paquete............... -
Lenguaje fuente.............. C++ (Borland C/C++ 3.1)
Estado....................... desarrollo
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Borja Etxebarria

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
0.0.0    15/07/96  Borja     codificacion inicial.

======================== Contenido ========================
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

/**********************************************************/

#ifndef __cplusplus
#error Must use C++ compiler
#endif

/**********************************************************/

#include <stdio.h>

#include "tdef.h"
#include "caudio.hpp"

/**********************************************************/

class AFFormat {
	protected:
		CAudioFile *fa;
	public:
		AFFormat( CAudioFile *ca ) { fa = ca; };
		virtual ~AFFormat( VOID ) {};
		virtual BOOL OK( VOID ) = 0;
		virtual const CHAR* fFormat( VOID ) = 0;

		virtual LONG HdrR( FILE *f, KVStrList &def, BOOL override ) = 0;
		virtual VOID HdrG( KVStrList &def, BOOL override ) = 0;
		virtual VOID HdrW( FILE *f, LONG nSamp ) = 0;
};

/**********************************************************/

#define ADD(p,v) fa->info().add(p,v)
#define NOV(p) ((!fa->info().contains(p))|| override)
#define ADDIFNOV(p,v) if (NOV(p)) ADD(p,v)

/**********************************************************/

class AFRaw : public AFFormat {
	public:
		AFRaw( CAudioFile *ca ) : AFFormat(ca) {};
		BOOL OK( VOID ) {return TRUE;};
		static const CHAR* formatStr( VOID ) { return "Raw"; };
		const CHAR* fFormat( VOID ) { return formatStr(); };
		static BOOL testFile( FILE *f );

		LONG HdrR( FILE *f, KVStrList &def, BOOL override );
		VOID HdrG( KVStrList &def, BOOL override );
		VOID HdrW( FILE *f, LONG nSamp );
};

/**********************************************************/

class AFAho2 : public AFFormat {
	protected:
		LONG lcgs;  /* longitud de la ultima cabecera leida/escrita */

	public:
		AFAho2( CAudioFile *ca ) : AFFormat(ca) {};
		BOOL OK( VOID ) {return TRUE;};
		static const CHAR* formatStr( VOID ) { return "Aho2"; };
		const CHAR* fFormat( VOID ) { return formatStr(); };
		static BOOL testFile( FILE *f );

		LONG HdrR( FILE *f, KVStrList &def, BOOL override );
		VOID HdrG( KVStrList &def, BOOL override );
		VOID HdrW( FILE *f, LONG nSamp );
};

/**********************************************************/

class AFTei : public AFFormat {
	public:
		AFTei( CAudioFile *ca ) : AFFormat(ca) {};
		BOOL OK( VOID ) {return TRUE;};
		static const CHAR* formatStr( VOID ) { return "Tei"; };
		const CHAR* fFormat( VOID ) { return formatStr(); };
		static BOOL testFile( FILE *f );

		LONG HdrR( FILE *f, KVStrList &def, BOOL override );
		VOID HdrG( KVStrList &def, BOOL override );
		VOID HdrW( FILE *f, LONG nSamp );
};

/**********************************************************/

class AFWav : public AFFormat {
		long datapos;
	public:
		AFWav( CAudioFile *ca ) : AFFormat(ca) {};
		BOOL OK( VOID ) {return TRUE;};
		static const CHAR* formatStr( VOID ) { return "Wav"; };
		const CHAR* fFormat( VOID ) { return formatStr(); };
		static BOOL testFile( FILE *f );

		LONG HdrR( FILE *f, KVStrList &def, BOOL override );
		VOID HdrG( KVStrList &def, BOOL override );
		VOID HdrW( FILE *f, LONG nSamp );
};


/**********************************************************/

class AFAho1 : public AFFormat{
	public:
		AFAho1( CAudioFile *ca ) : AFFormat(ca) {};
		BOOL OK( VOID ) {return TRUE;};
		static const CHAR* formatStr( VOID ) { return "Aho1"; };
		const CHAR* fFormat( VOID ) { return formatStr(); };
		static BOOL testFile( FILE *f );

		LONG HdrR( FILE *f, KVStrList &def, BOOL override );
		VOID HdrG( KVStrList &def, BOOL override );
		VOID HdrW( FILE *f, LONG nSamp );
};

/**********************************************************/

class AFNist : public AFFormat {
	public:
		AFNist( CAudioFile *ca ) : AFFormat(ca) {};
		BOOL OK( VOID ) {return TRUE;};
		static const CHAR* formatStr( VOID ) { return "Nist"; };
		const CHAR* fFormat( VOID ) { return formatStr(); };
		static BOOL testFile( FILE *f );

		LONG HdrR( FILE *f, KVStrList &def, BOOL override );
		VOID HdrG( KVStrList &def, BOOL override );
		VOID HdrW( FILE *f, LONG nSamp );
};

/**********************************************************/

class AFHak : public AFFormat {
		LONG lc;  /* longitud de la ultima cabecera leida/escrita */
	public:
		AFHak( CAudioFile *ca ) : AFFormat(ca) {};
		BOOL OK( VOID ) {return TRUE;};
		static const CHAR* formatStr( VOID ) { return "Hak"; };
		const CHAR* fFormat( VOID ) { return formatStr(); };
		static BOOL testFile( FILE *f );

		LONG HdrR( FILE *f, KVStrList &def, BOOL override );
		VOID HdrG( KVStrList &def, BOOL override );
		VOID HdrW( FILE *f, LONG nSamp );
};

/**********************************************************/

#endif
