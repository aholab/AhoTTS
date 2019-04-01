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
#ifndef __UTTDPH_HPP__
#define __UTTDPH_HPP__

/**********************************************************/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/*
(C) 1997 TSR/Aholab - ETSII/IT Bilbao (UPV/EHU)

Nombre fuente................ uttdph.hpp
Nombre paquete............... aHoTTS
Lenguaje fuente.............. C++
Estado....................... -
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Borja Etxebarria
.............................

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
1.0.1    13/05/00  borja     update ULEVEL, docs
1.0.0    31/01/00  borja     codefreeze aHoTTS v1.0
0.0.0    24/11/97  borja     Codificacion inicial.

======================== Contenido ========================
<DOC>
</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include "uttph.hpp"
#include "dphdb.hpp"

/**********************************************************/
/* diferentes niveles a los que se puede trabajar:  Tienen que ser
compatibles con otros bits definidos en otros ficheros derivados
de {{utt}} (utt, uttph, etc)  */

enum {
	ULEVEL_DIPHONE  =0x1000,

	URANGE_DIPHONE = URANGE_UTT | ULEVEL_DIPHONE,
};

/**********************************************************/
/* La clase UttDph esta pensada para sintesis de difonemas
(polifonemas): Aparte de lo manejado por la clase de
fonemas UttPh tiene ademas un campo con el codigo de difonema,
y la utterance puede almacenar un puntero a la base de datos
de difonemas.

El campo Diphone *no* se translada automaticamente cuando se borra
una celda!

Un difonema siempre debe comenzar en la celda de su primer fonema,
o en celdas posteriores (logicamente antes del fonema siguiente). */

class UttCellDph: public UttCellPh {
public:
	BOOL isStartOf( UttLevel range ) const;
private:
	DiphoneRef dph;       // codigo difonema

public:
	UttCellDph ( VOID );
	VOID setDiphoneRef( DiphoneRef code ) { dph = code; }
	DiphoneRef getDiphoneRef( VOID ) const { return dph; }

	VOID setDummyDiphoneRef( VOID ) { dph = DphDB::getDummy(); }
	BOOL isDummyDiphoneRef( VOID ) const { return DphDB::isDummy(dph); }

	virtual VOID reset( VOID );

public:
#ifdef HTTS_IOTXT
	virtual VOID __foutput(FILE *f) const;
	virtual CHAR *__finput( CHAR *buf );

	static VOID setGlobalDphDB( DphDB * dbase ) { db=dbase; }
protected:
	static DphDB* db;
#endif

public:
	KINDOF_DECL();

};

/**********************************************************/
/* Esta clasecilla agrupa datos necesarios para recorrer
la lista UttDph segmento a segmento (un fonema se forma
con varios segmentos, que pueden estar en difonemas diferentes).
Si {phn} es -1 y {dph} no es 0, significa que el difonema es
dummy (de relleno, cuando no hay un difonema adecuado), y {ph}
apunta al fonema en cuestion.
{dph} o {ph} pueden ser 0 (NULL) en cuyo caso phn==-1 (realmente
vale con verificar si {dph} es o no 0, {ph} solo tiene
sentido si {dph} no es 0. */

class DphPhSeg {
public:
	UttI ph;   // fonema actual
	UttI dph;  // difonema actual
	INT phn;   // numero de fonema dentro del difonema
};

/**********************************************************/

class UttDph : public UttPh {
protected:
	// al crear nuevas celdas, que sean del tipo adecuado (Dph)
	UttCell * newCell( VOID );

	DphDB* dphdb;  // puntero a base de datos

public:
	UttDph( VOID );
	// Cast automatico de las celdas al tipo adecuado
	UttCellDph& cell( UttI p ) const { assert(Utt::cell(p).isKindOf("UttCellDph")); return (UttCellDph&)(Utt::cell(p)); }
	UttCellDph& operator()( UttI p ) const { return cell(p); }

	VOID setDphDB( DphDB * db ) { dphdb=db; } // acceso a base de datos
	DphDB *getDphDB( VOID ) { return dphdb; }

	UTT_LEVELSHORTCUTS(diphone,ULEVEL_DIPHONE);

	KINDOF_DECL();

#ifdef HTTS_IOTXT
	virtual VOID foutput( FILE *f ) const;
	virtual BOOL finput( FILE *f );
#endif

	VOID segFirst( DphPhSeg & si );
	VOID segNext( DphPhSeg & si );

};

/**********************************************************/

#endif

