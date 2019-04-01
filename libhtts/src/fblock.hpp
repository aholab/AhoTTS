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
#ifndef __FBLOCK_HPP__
#define __FBLOCK_HPP__

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
1.0.1    09/04/99  Borja     uso tipos UINT
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

/**********************************************************/

#define FBLOCK_BP_IGNORE 0
#define FBLOCK_BP_BEFORE 1
#define FBLOCK_BP_AFTER 2
#define FBLOCK_BP_BOTH 4

/**********************************************************/

class FBlock {
	private:
		FILE* f;

		LONG bPos0;  // bytes delante del bloque
		LONG bTail;  // bytes detras del bloque
		LONG bPos;  // posicion actual
		LONG bTotSize;  // numero de bytes
		BOOL writeMode;  // TRUE si en modo Write
		UINT borderpolicy;  // b0->begin  b1->end
		BOOL stdinout; // TRUE si se esta en stdin o stdout;

		VOID setRW( BOOL write );

	public:
		FBlock( VOID );

		VOID attach( FILE* fBin, LONG N=-1, UINT borderpolicy=4 );

		VOID setPos( LONG pos );
		LONG getPos( VOID ) const { return (LONG)bPos; }
		LONG getN( VOID ) const {	return (LONG)bTotSize; }

		UINT get( BYTE& byte );
		UINT set( BYTE byte ) { return set(&byte,1); }
		UINT ins( BYTE byte ) { return ins(&byte,1); }
		UINT del( VOID ) { return del(1); }

		UINT get( BYTE* block, UINT len );
		UINT set( const BYTE* block, UINT len );
		UINT ins( const BYTE* block, UINT len );
		UINT del( UINT len );

		VOID toggleRW( VOID );

		BOOL OK ( VOID ) const;

		~FBlock( VOID ) { f=0; }
};

/**********************************************************/

#endif
