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

Nombre fuente................ utt.cpp
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

/**********************************************************/

#include "uti.h"
#include "utt.hpp"
#include "httsmsg.h"

/**********************************************************/

KINDOF_BDEF(UttCell)

KINDOF_BDEF(Utt)

/**********************************************************/

BOOL UttCell::isStartOf(UttLevel range) const
{
	if (range==URANGE_UTT) return FALSE;	// utt. completa, no hay fronteras.
	if (range==ULEVEL_CELL) return TRUE;

#ifdef HTTS_DEBUG
	htts_error("UttCell::isStartOf() - Unknown UttCell level set (range=%lxh)", (long)range);
#endif

	return FALSE;
}

/**********************************************************/

#ifdef HTTS_IOTXT
VOID UttCell::foutput( FILE *f ) const
{
	(void)f;
}
#endif

/**********************************************************/

#ifdef HTTS_IOTXT
BOOL UttCell::finput( FILE *f )
{
	(void)f;
	reset();
	return FALSE;
}
#endif

/**********************************************************/

VOID Utt::steal(Utt & u)
{
	ul.clear();
	ul.append_mv(u.ul);
}

/**********************************************************/

VOID Utt::clear(VOID)
{
	ul.clear();
}

/**********************************************************/

BOOL Utt::empty(VOID) const
{
	return ul.empty();
}

/**********************************************************/

UttCell *Utt::newCell(VOID)
{
	return new UttCell;
}

/**********************************************************/

UttI Utt::deleteCell(UttI p)
{
#ifdef HTTS_DEBUG
	if (!p) htts_error("Utt:deleteCell() - Trying to delete null UttCell");
#endif
	return p?ul.del(p):0;
}

/**********************************************************/

UttCell & Utt::cell(UttI p) const
{
#ifdef HTTS_DEBUG
	if (!p) htts_error("Utt::cell() - Trying to read null UttCell");
#endif
	assert(ul.item(p).isKindOf("UttCell"));
	return (UttCell &)ul.item(p);
}

/**********************************************************/

UttI Utt::cellAppend(VOID)
{
	UttCell *c=newCell();
	if (!c) htts_error("Utt::cellAppend() - Out of memory");
	return ul.append_p(c);
}

/**********************************************************/

UttI Utt::cellAddBefore(UttI p)
{
#ifdef HTTS_DEBUG
	if (!p) htts_error("Utt::cellAddBefore() - Trying to add before null UttCell");
#endif

	UttCell *c=newCell();
	if (!c) htts_error("Utt::cellAddBefore() - Out of memory");
	return ul.insbefore_p(p,c);
}

/**********************************************************/

UttI Utt::cellAddAfter(UttI p)
{
#ifdef HTTS_DEBUG
	if (!p) htts_error("Utt::cellAddAfter() - Trying to add after null UttCell");
#endif

	UttCell *c=newCell();
	if (!c) htts_error("Utt::cellAddAfter() - Out of memory");
	return ul.insafter_p(p,c);
}

/**********************************************************/

VOID Utt::cellExchange( UttI p1, UttI p2 )
{
	ul.exchange(p1,p2);
}

/**********************************************************/

Lix Utt::cellMoveAfter( UttI who, UttI where )
{
	return ul.moveafter(who,where);
}

/**********************************************************/

Lix Utt::cellMoveBefore( UttI who, UttI where )
{
	return ul.movebefore(who,where);
}

/**********************************************************/

UttI Utt::levelDel(UttLevel level, UttI p, UttLevel range)
{
#ifdef HTTS_DEBUG
	if (!p) htts_error("Utt::levelDel() - Trying to delete null UttCell");
#endif
	if (!p) return 0;

	// optimizacion/shortcut para ULEVEL_CELL. Lo de abajo funcionaria, 
	// pero hay bastante overhead inutil.
	if (level == ULEVEL_CELL) return deleteCell(p);

	UttI first = levelFirst(level, p, range);
	UttI last = levelLast(level, p, range);
	UttI next = cellNext(last);

	// Vamos borrando hacia atras para evitar arrastrar fronteras
	// inutilmente (suponiendo que las clases derivadas lo hagan).
	p = last;
	while (p) {
		UttI prev = cellPrev(p);
		deleteCell(p);
		if (p == first) break;
		p = prev;
	}

	return next;
}

/**********************************************************/

UttI Utt::levelNext(UttLevel level, UttI p, UttLevel range) const
{
	if (!p) return 0;
	do {	// forward search for start of next level-cell
		p = ul.next(p);
		if (!p) return 0;
		// chequeamos fronteras, salvo si estamos a rango maximo (por eficiencia)
		if ((range != URANGE_UTT)&&(cell(p).isStartOf(range))) return 0;
		if (level==ULEVEL_CELL) return p;
	} while (!cell(p).isStartOf(level));
	return p;
}

/**********************************************************/

UttI Utt::levelPrev(UttLevel level, UttI p, UttLevel range) const
{
	if (!p) return 0;

	if (inURange(ULEVEL_CELL, range)) return 0;	// en el rango de las celdas, las celdas son unicas

	if (level == ULEVEL_CELL) {		// short-cut para ULEVEL_CELL. lo de abajo seria suficiente...
		if (range == URANGE_UTT) return ul.prev(p);
		return cell(p).isStartOf(range) ? 0 : ul.prev(p);
	}

	if (level == ULEVEL_UTT) return 0;	// only one utterance in the utterance :)

	while (!cell(p).isStartOf(level)) {		// backward search for start of this level
		if (range != URANGE_UTT) {
			if (cell(p).isStartOf(range)) return 0;
		}
		p = ul.prev(p);
		if (!p) return 0;
	}

	do {	// backward search for start of previous level
		if (range != URANGE_UTT) {
			if (cell(p).isStartOf(range))	return 0;
		}
		p = ul.prev(p);
		if (!p)	return 0;
	} while (!cell(p).isStartOf(level));

	return p;
}

/**********************************************************/

UttI Utt::levelThis(UttLevel level, UttI p, UttLevel range) const
{
	if (level == ULEVEL_CELL) return p;
	if (level == ULEVEL_UTT) return ul.first();
	while (p) {
		if (cell(p).isStartOf(level)) break;
		p = cellPrev(p, range);
	}
	return p;
}

/**********************************************************/

UttI Utt::levelFirst(UttLevel level, UttI p, UttLevel range) const
{
	if ((!p) && (range == URANGE_UTT)) {
		p = levelThis(level, ul.first(), range);
		return p ? p : levelNext(level, ul.first(), range);
	}

	UttI q = levelThis(level, p, range);

	if (!q) return levelNext(level, p, range);
	while (q) {
		p = q;
		q = levelPrev(level, p, range);
	}
	return p;
}

/**********************************************************/

UttI Utt::levelLast(UttLevel level, UttI p, UttLevel range) const
{
	if ((!p) && (range == URANGE_UTT))
		return levelThis(level, ul.last(), range);

	UttI q = levelThis(level, p, range);
	if (!q) q = levelNext(level, p, range);
	while (q) {
		p = q;
		q = levelNext(level, p, range);
		if (!q) return p;
	}
	return 0;
}

/**********************************************************/

BOOL Utt::levelIsFirst(UttLevel level, UttI p, UttLevel range) const
{
	return (levelPrev(level, p, range) == 0);
}

/**********************************************************/

BOOL Utt::levelIsLast(UttLevel level, UttI p, UttLevel range) const
{
	return (levelNext(level, p, range) == 0);
}

/**********************************************************/

LONG Utt::levelPos(UttLevel level, UttI p, UttLevel range) const
{
	LONG n = 0;
	while ((p = levelPrev(level, p, range)) != 0)  n++;
	return n;
}

/**********************************************************/

LONG Utt::levelN(UttLevel level, UttI p, UttLevel range) const
{
	p = levelLast(level, p, range);
	return p ? levelPos(level, p) + 1 : 0;
}

/**********************************************************/

UttI Utt::levelGotoPos(UttLevel level, LONG pos, UttI p, UttLevel range) const
{
	p = levelFirst(level, p, range);
	while (p && (pos--))  p = levelNext(level, p, range);
	return p;
}

/**********************************************************/

#ifdef HTTS_IOTXT
VOID Utt::foutput( FILE *f ) const
{
	for (UttI p = cellFirst(); p!=0; p=cellNext(p)) cell(p).foutput(f);
	fprintf(f,"\n");
}
#endif

/**********************************************************/

#ifdef HTTS_IOTXT
BOOL Utt::finput( FILE *f )
{
	reset();

	UttI p;
	do { p=cellAppend(); } while (cell(p).finput(f));
	cellDel(p);
	if (empty()) return FALSE;

#define NREAD 10
	CHAR buf[NREAD];
	if (fgetln_filt(buf, NREAD, f,0,1,0,NULL))
		if (*buf!='\0') htts_error("Utt:finput() - Invalid UttCell entry (%s...)",buf);
#undef NREAD

	return TRUE;
}
#endif

/**********************************************************/
