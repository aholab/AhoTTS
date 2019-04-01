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
#ifndef __UTT_HPP__
#define __UTT_HPP__

/**********************************************************/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/*
(C) 1997 TSR/Aholab - ETSII/IT Bilbao (UPV/EHU)

Nombre fuente................ utt.hpp
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
</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include <stdio.h>

#include "tdef.h"
#include "listt.hpp"
#include "kindof.hpp"

#include "htts_cfg.h"

/**********************************************************/
/* Niveles a los que se puede trabajar: celda,
fonema, silaba, palabra, frase, sentencia, todo (utterance), etc.
Son potencias de dos!!!, para poder enviar OR de varios de ellos
en un rango de busqueda (ver URANGE_???). Tienen que ser
compatibles con otros bits definidos en otros ficheros derivados
de esta clase (uttws, etc) */

typedef INT UttLevel;

enum {
	ULEVEL_UTT      =0x0000,   /* ULEVEL_UTT debe ser 0 */
	ULEVEL_CELL     =0x0001,

/* ahora vienen los rangos jerarquizados, en los que
el comienzo de una palabra implica comienzo de silaba,
el comienzo de una frase implica comienzo de palabra, etc etc. */
	URANGE_UTT = ULEVEL_UTT    /* URANGE_UTT debe ser 0 */
};

/**********************************************************/

// para comprobar si un nivel esta en un rango
inline BOOL inURange( UttLevel level, UttLevel range) { return ((level&range)!=0); }
// para eliminar un nivel o rangos de otro rango
inline UttLevel delURange( UttLevel levels, UttLevel range) { return (UttLevel)(range & (~levels)); }
// para juntar niveles o rangos en otro rango
inline UttLevel addURange( INT levels, UttLevel range=URANGE_UTT) { return (UttLevel)(range | levels); }

/**********************************************************/

class Utt;  // UttCell acepta esta clase como friend.

/* esta es la clase basica para las celdas de la lista. Solo es
necesario que nos diga si la celda es o no el comienzo de alguno
de los niveles posibles (fronteras) */
class UttCell {
public:
	UttCell( VOID ) { reset(); }
	virtual ~UttCell() { }

	virtual VOID reset( VOID ) {};

	KINDOF_DECL();

public:
#ifdef HTTS_IOTXT
	virtual VOID foutput( FILE *f ) const;
	virtual BOOL finput( FILE *f );
#endif

public:
	virtual BOOL isStartOf( UttLevel range ) const;

friend class Utt;
};

/**********************************************************/

// El tipo UttI es el "indice" para recorrer la lista.
typedef Lix UttI;

/**********************************************************/

/* Utt es la clase base que maneja la lista de celdas UttCell
a los diferentes niveles que se han definido en UttLevel.
Maneja directamente niveles UTT y CELL (que son los niveles
maximo y minimo posibles.) */
class Utt {
protected:
	typedef ListT<UttCell> UttItemList;
	UttItemList ul;

	// crear/borrar celdas: virtual para soportar clases derivadas.
	// ademas derivando deleteCell se podra manejar fronteras
	// inteligentemente (ver nota2).
	virtual UttCell * newCell( VOID );
	virtual UttI deleteCell( UttI p );
public:
	// general
	Utt( VOID ) {};
	virtual ~Utt( VOID ) {};
	BOOL create( VOID ) { return TRUE; }
	VOID steal( Utt &u );
	VOID clear( VOID );
	VOID reset( VOID ) { clear(); }
	BOOL empty( VOID ) const;

	// acceso a las celdas: dos metodos equivalentes: utt.cell(p) o utt(p)
	UttCell& cell( UttI p ) const;
	UttCell& operator() ( UttI p ) const { return cell(p); }

	// creacion de celdas
	UttI cellAppend( VOID );
	UttI cellAddBefore( UttI p );
	UttI cellAddAfter( UttI p );
	VOID cellExchange( UttI p1, UttI p2 );
	Lix cellMoveAfter( UttI who, UttI where );
	Lix cellMoveBefore( UttI who, UttI where );

	// ver nota2, muy importante!!!
	virtual UttI levelDel( UttLevel level, UttI p, UttLevel range=URANGE_UTT );

	// movimiento por la lista
	UttI levelFirst( UttLevel level, UttI p=0, UttLevel range=URANGE_UTT ) const;
	UttI levelLast( UttLevel level, UttI p=0, UttLevel range=URANGE_UTT ) const;
	UttI levelNext( UttLevel level, UttI p, UttLevel range=URANGE_UTT ) const;
	UttI levelPrev( UttLevel level, UttI p, UttLevel range=URANGE_UTT ) const;
	UttI levelThis( UttLevel level, UttI p, UttLevel range=URANGE_UTT ) const;
	LONG levelPos( UttLevel level, UttI p, UttLevel range=URANGE_UTT ) const;
	LONG levelN( UttLevel level, UttI p=0, UttLevel range=URANGE_UTT ) const;
	UttI levelGotoPos( UttLevel level, LONG pos, UttI p=0, UttLevel range=URANGE_UTT ) const;
	BOOL levelIsFirst( UttLevel level, UttI p, UttLevel range=URANGE_UTT ) const;
	BOOL levelIsLast( UttLevel level, UttI p, UttLevel range=URANGE_UTT ) const;

	// esto define wordFirst, syllableFirst, wordLast, etc.
	// (como levelFirst pero el parametro {level} esta prefijado)
#define UTT_LEVELSHORTCUTS(lvl,lvlk) \
	UttI lvl##First( UttI p=0, UttLevel r=URANGE_UTT ) const { return levelFirst(lvlk,p,r); } \
	UttI lvl##Last( UttI p=0, UttLevel r=URANGE_UTT ) const { return levelLast(lvlk,p,r); } \
	UttI lvl##Next( UttI p, UttLevel r=URANGE_UTT ) const { return levelNext(lvlk,p,r); } \
	UttI lvl##Prev( UttI p, UttLevel r=URANGE_UTT ) const { return levelPrev(lvlk,p,r); } \
	UttI lvl##This( UttI p, UttLevel r=URANGE_UTT ) const { return levelThis(lvlk,p,r); } \
	LONG lvl##Pos( UttI p, UttLevel r=URANGE_UTT ) const { return levelPos(lvlk,p,r); } \
	LONG lvl##N( UttI p=0, UttLevel r=URANGE_UTT ) const { return levelN(lvlk,p,r); } \
	UttI lvl##GotoPos( LONG pos, UttI p=0, UttLevel r=URANGE_UTT ) const { return levelGotoPos(lvlk,pos,p,r); } \
	BOOL lvl##IsFirst( UttI p, UttLevel r=URANGE_UTT ) const { return levelIsFirst(lvlk,p,r); } \
	BOOL lvl##IsLast( UttI p, UttLevel r=URANGE_UTT ) const { return levelIsLast(lvlk,p,r); } \
	UttI lvl##Del( UttI p, UttLevel r=URANGE_UTT ) { return levelDel(lvlk,p,r); }

	UTT_LEVELSHORTCUTS(cell,ULEVEL_CELL);

#ifdef HTTS_IOTXT
	virtual VOID foutput( FILE *f ) const;
	virtual BOOL finput( FILE *f );
#endif

	KINDOF_DECL();
};

/**********************************************************/
/*
nota1: se podia haber hecho la lista directamente de
elementos UttCell, pero entonces al crear/borrar/mover elementos
andariamos copiando de aqui para alli TODA la celda. Por eso
hacemos listas de objeto CellPtr que solo contienen
un puntero a la celda real.
Esta nota es obsoleta.
---
nota2: Es cosa tuya si se pierden fronteras,
redefine deleteCell para arreglarlo. Y si range
contiene un nivel inferior a level, la funcion no
borrara el level completo, ni devolvera un puntero a la
siguiente entrada de tipo level, sino que te devolvera
referencia al siguiente range.
---
*/
/**********************************************************/

#endif

