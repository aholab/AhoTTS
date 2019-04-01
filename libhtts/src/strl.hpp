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
#ifndef __STRL_HPP__
#define __STRL_HPP__

/**********************************************************/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/*
Copyright: 1996 - Grupo de Voz (DAET) ETSII/IT-Bilbao

Nombre fuente................ STRL.HPP
Nombre paquete............... -
Lenguaje fuente.............. C++
Estado....................... Completado
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Borja Etxebarria

Version  dd/mm/aa  Autor     Comentario
-------  --------  --------  ----------
1.3.0    07/08/97  Borja     nuevo metodo: bbval()
1.2.0    25/04/97  Borja     usar String en vez de char*
1.1.0    14/01/97  Borja     add/remove_prefix() como en CSTR SpeechTools
1.0.0    06/05/96  Borja     Codificacion inicial.

======================== Contenido ========================
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include <stdarg.h>

#include "string.hpp"
#include "listt.hpp"

/**********************************************************/

class StrList;
class StrSet;
class KVStrList;

/**********************************************************/

class StrList {
private:
	ListT<String> l;

public:
	BOOL owns(Lix p) const { return l.owns(p); }
	LIINT index (Lix p) const { return l.index(p); }
	Lix lix(LIINT i) const { return l.lix(i); }
	LIINT length(VOID) const { return l.length(); }
	BOOL empty(VOID) const { return l.empty(); }
	VOID clear( VOID ) { l.clear(); };
	VOID reverse(VOID) { l.reverse(); }

	Lix first(VOID) const { return l.first(); }
	Lix last(VOID) const { return l.last(); }
	Lix next(Lix p) const { return l.next(p); }
	Lix prev(Lix p) const { return l.prev(p); }

	StrList& operator = (const StrList& a) { l.operator=(a.l); return *this; }

	Lix prepend(VOID) { return l.prepend(); }
	Lix append(VOID) { return l.append(); }
	Lix insbefore(Lix p) { return l.insbefore(p); }
	Lix insafter(Lix p) { return l.insafter(p); }
	Lix prepend(const String& d) { return l.prepend(d); }
	Lix append(const String& d) { return l.append(d); }
	Lix insbefore(Lix p, const String& d) { return l.insbefore(p,d); }
	Lix insafter(Lix p, const String& d) { return l.insafter(p,d); }
	Lix prepend(const StrList& a) { return l.prepend(a.l); }
	Lix append(const StrList& a) { return l.append(a.l); }
	Lix insbefore(Lix p, const StrList& a) { return l.insbefore(p,a.l); }
	Lix insafter(Lix p, const StrList& a) { return l.insafter(p,a.l); }
	Lix prepend_mv(StrList& a) { return l.prepend_mv(a.l); }
	Lix append_mv(StrList& a) { return l.append_mv(a.l); }
	Lix insbefore_mv(Lix p, StrList& a) { return l.insbefore_mv(p,a.l); }
	Lix insafter_mv(Lix p, StrList& a) { return l.insafter_mv(p,a.l); }
	Lix prepend(const char *d) { return l.prepend(String(d)); }
	Lix append(const char *d) { return l.append(String(d)); }
	Lix insbefore(Lix p, const char *d) { return l.insbefore(p,String(d)); }
	Lix insafter(Lix p, const char *d) { return l.insafter(p,String(d)); }

	Lix del(Lix p, INT dir = 1) { return l.del(p,dir); }
	VOID delprev(Lix p) { l.delprev(p); }
	VOID delnext(Lix p) { l.delnext(p); }
	Lix delfirst(VOID) { return l.delfirst(); }
	Lix dellast(VOID) { return l.dellast(); }

	const String& item(Lix p) const { return l.item(p); }
	const String& operator () (Lix p) const { return l.operator()(p); }
	const String& item_first(VOID) const { return l.item_first(); }
	const String& item_last(VOID) const { return l.item_last(); }
	String& item(Lix p) { return l.item(p); }
	String& item_first(VOID) { return l.item_first(); }
	String& item_last(VOID) { return l.item_last(); }

	VOID exchange( Lix p1, Lix p2 ) { l.exchange(p1,p2); }
	VOID sortf( INT (*comparefunc) ( const String*item1, const String*item2 ))
			{ l.sortf(comparefunc); }

	Lix insbefore(LIINT i) { return l.insbefore(i); }
	Lix insbefore(LIINT i, const String& item) { return l.insbefore(i,item); }
	Lix insbefore(LIINT i, const StrList& a) { return l.insbefore(i,a.l); }
	Lix insbefore_mv(LIINT i, StrList& a) { return l.insbefore_mv(i,a.l); }
	Lix insafter(LIINT i) { return l.insafter(i); }
	Lix insafter(LIINT i, const String& item) { return l.insafter(i,item); }
	Lix insafter(LIINT i, const StrList& a) { return l.insafter(i,a.l); }
	Lix insafter_mv(LIINT i, StrList& a) { return l.insafter_mv(i,a.l); }
	VOID del(LIINT i) { l.del(i); }
	VOID delprev(LIINT i) { l.delprev(i); }
	VOID delnext(LIINT i) { l.delnext(i); }
	const String& item(LIINT i) const { return l.item(i); }
	const String& operator () (LIINT i) const { return l.operator()(i); }
	String& item(LIINT i) { return l.item(i); }
	VOID exchange( LIINT i1, LIINT i2 ) { l.exchange(i1,i2); }

#ifdef KK // $$$
	inline StrList& operator = (const StrSet& a);
	inline Lix prepend(const StrSet& a);
	inline Lix append(const StrSet& a);
	inline Lix insbefore(Lix p, const StrSet& a);
	inline Lix insafter(Lix p, const StrSet& a);
	inline Lix prepend_mv(StrSet& a);
	inline Lix append_mv(StrSet& a);
	inline Lix insbefore_mv(Lix p, StrSet& a);
	inline Lix insafter_mv(Lix p, StrSet& a);
	inline Lix insbefore(LIINT i, const StrSet& a);
	inline Lix insbefore_mv(LIINT i, StrSet& a);
	inline Lix insafter(LIINT i, const StrSet& a);
	inline Lix insafter_mv(LIINT i, StrSet& a);
#endif

public:
	StrList( ) {};
	StrList( const StrList &a ) { l=a.l; };
	inline StrList( const StrSet &a );
	StrList( const char *words ) { append_s(words); }
	StrList( int dummy, const char *fwords, ... ) { (void)dummy; va_list v; va_start(v,fwords); append_s(fwords,v); va_end(v); }
	StrList( const char *fwords, va_list v ) { append_s(fwords,v); }
	StrList( const char *wordarray[] ) { append_s(wordarray); }
	StrList( char *wordarray[] ) { append_s(wordarray); }

	VOID append_s( const char *words );
	VOID append_sf( const char *fwords, ... );
	VOID append_s( const char *fwords, va_list v );
	VOID append_s( const char *wordarray[] );
	VOID append_s( char* wordarray[]) {append_s((const char**)wordarray);}

	VOID sort( BOOL reverse=FALSE );
	BOOL OK( VOID ) const;

	friend class StrSet;
	friend class KVStrList;
};

std::ostream& operator << (std::ostream &st, const StrList &l);

/**********************************************************/

class StrSet {
private:
	SetListT<String> l;

public:
	BOOL owns(Lix p) const { return l.owns(p); }
	LIINT index (Lix p) const { return l.index(p); }
	Lix lix(LIINT i) const { return l.lix(i); }
	LIINT length(VOID) const { return l.length(); }
	BOOL empty(VOID) const { return l.empty(); }
	VOID clear( VOID ) { l.clear(); };
	VOID reverse(VOID) { l.reverse(); }

	Lix first(VOID) const { return l.first(); }
	Lix last(VOID) const { return l.last(); }
	Lix next(Lix p) const { return l.next(p); }
	Lix prev(Lix p) const { return l.prev(p); }

	StrSet& operator = (const StrSet& a) { l.operator=(a.l); return *this; }

	Lix _prepend(VOID) {return l._prepend();}
	Lix _append(VOID) {return l._append();}
	Lix _insbefore(Lix p) {return l._insbefore(p);}
	Lix _insafter(Lix p) {return l._insafter(p);}
	Lix _prepend(const String& d) {return l._prepend(d);}
	Lix _append(const String& d) {return l._append(d);}
	Lix _insbefore(Lix p, const String& d) {return l._insbefore(p,d);}
	Lix _insafter(Lix p, const String& d) {return l._insafter(p,d);}
	Lix _prepend(const StrSet& a) {return l._prepend(a.l);}
	Lix _append(const StrSet& a) {return l._append(a.l);}
	Lix _insbefore(Lix p, const StrSet& a) {return l._insbefore(p,a.l);}
	Lix _insafter(Lix p, const StrSet& a) {return l._insafter(p,a.l);}
	Lix _prepend_mv(StrSet& a) {return l._prepend_mv(a.l);}
	Lix _append_mv(StrSet& a) {return l._append_mv(a.l);}
	Lix _insbefore_mv(Lix p, StrSet& a) {return l._insbefore_mv(p,a.l);}
	Lix _insafter_mv(Lix p, StrSet& a) {return l._insafter_mv(p,a.l);}
	Lix _prepend(const char *d) { return l._prepend(String(d)); }
	Lix _append(const char *d) { return l._append(String(d)); }
	Lix _insbefore(Lix p, const char *d) { return l._insbefore(p,String(d)); }
	Lix _insafter(Lix p, const char *d) { return l._insafter(p,String(d)); }

	Lix del(Lix p, INT dir = 1) {return l.del(p,dir);}
	VOID delprev(Lix p) {l.delprev(p);}
	VOID delnext(Lix p) {l.delnext(p);}
	Lix delfirst(VOID) {return l.delfirst();}
	Lix dellast(VOID) {return l.dellast();}

	const String& item(Lix p) const {return l.item(p);}
	const String& operator () (Lix p) const {return l.operator()(p);}
	const String& item_first(VOID) const {return l.item_first();}
	const String& item_last(VOID) const {return l.item_last();}
	String& _item(Lix p) {return l._item(p);}
	String& _item_first(VOID) {return l._item_first();}
	String& _item_last(VOID) {return l._item_last();}

	VOID exchange( Lix p1, Lix p2 ) {l.exchange(p1,p2);}
	VOID sortf( INT (*comparefunc) ( const String*item1, const String*item2 ))
			{l.sortf(comparefunc);}

	Lix add(const String& x) {return l.add(x);}
	VOID add(const StrSet& a) {l.add(a.l);}
	VOID add_mv(StrSet& a, BOOL ovwrt) {l.add_mv(a.l,0,0,ovwrt);}
	Lix add(const char * x) {return l.add(String(x));}

	Lix seek(const String& x) const {return l.seek(x);}
	BOOL contains(const String& x) const {return l.contains(x);}
	BOOL contains(const StrSet&a) const {return l.contains(a.l);}
	BOOL contains(const char * x) const {return l.contains(String(x));}
	BOOL equals(const StrSet&a, BOOL allowrep=TRUE) const {return l.equals(a.l,allowrep);}
	BOOL erase(const String & x) {return l.erase(x);}
	BOOL erase(const StrSet& a) {return l.erase(a.l);}
	BOOL erase(const char * x) {return l.erase(String(x));}
	VOID keep(const StrSet& a) {l.keep(a.l);}

	Lix _insbefore(LIINT i) {return l._insbefore(i);}
	Lix _insbefore(LIINT i, const String& item) {return l._insbefore(i,item);}
	Lix _insbefore(LIINT i, const StrSet& a) {return l._insbefore(i,a.l);}
	Lix _insbefore_mv(LIINT i, StrSet& a) {return l._insbefore_mv(i,a.l);}
	Lix _insbefore(LIINT i, const char * item) {return l._insbefore(i,String(item));}
	Lix _insafter(LIINT i) {return l._insafter(i);}
	Lix _insafter(LIINT i, const String& item) {return l._insafter(i,item);}
	Lix _insafter(LIINT i, const StrSet& a) {return l._insafter(i,a.l);}
	Lix _insafter_mv(LIINT i, StrSet& a) {return l._insafter_mv(i,a.l);}
	Lix _insafter(LIINT i, const char *item) {return l._insafter(i,String(item));}
	VOID del(LIINT i) {l.del(i);}
	VOID delprev(LIINT i) {l.delprev(i);}
	VOID delnext(LIINT i) {l.delnext(i);}
	const String& item(LIINT i) const {return l.item(i);}
	const String& operator () (LIINT i) const {return l.operator()(i);}
	String& _item(LIINT i) {return l._item(i);}
	VOID exchange( LIINT i1, LIINT i2 ) {l.exchange(i1,i2);}

	StrSet& operator = (const StrList& a) {l.operator=(a.l); return *this;}
	Lix _prepend(const StrList& a) {return l._prepend(a.l);}
	Lix _append(const StrList& a) {return l._append(a.l);}
	Lix _insbefore(Lix p, const StrList& a) {return l._insbefore(p,a.l);}
	Lix _insafter(Lix p, const StrList& a) {return l._insafter(p,a.l);}
	Lix _prepend_mv(StrList& a) {return l._prepend_mv(a.l);}
	Lix _append_mv(StrList& a) {return l._append_mv(a.l);}
	Lix _insbefore_mv(Lix p, StrList& a) {return l._insbefore_mv(p,a.l);}
	Lix _insafter_mv(Lix p, StrList& a) {return l._insafter_mv(p,a.l);}
	VOID add(const StrList& a) {l.add(a.l);}
	VOID add_mv(StrList& a, BOOL ovwrt) {l.add_mv(a.l,0,0,ovwrt);}
	BOOL contains(const StrList&a) const {return l.contains(a.l);}
	BOOL equals(const StrList&a, BOOL allowrep=TRUE) const {return l.equals(a.l,allowrep);}
	BOOL erase(const StrList& a) {return l.erase(a.l);}
	VOID keep(const StrList& a) {l.keep(a.l);}
	Lix _insbefore(LIINT i, const StrList& a) {return l._insbefore(i,a.l);}
	Lix _insbefore_mv(LIINT i, StrList& a) {return l._insbefore_mv(i,a.l);}
	Lix _insafter(LIINT i, const StrList& a) {return l._insafter(i,a.l);}
	Lix _insafter_mv(LIINT i, StrList& a) {return l._insafter_mv(i,a.l);}

public:
	StrSet() {};
	StrSet( const StrList &a ) { l=a.l; };
	StrSet( const StrSet &a ) { l=a.l; };
	StrSet( const char *words ) { add_s(words); }
	StrSet( int dummy, const char *fwords, ... ) { (void)dummy; va_list v; va_start(v,fwords); add_s(fwords,v); va_end(v); }
	StrSet( const char *fwords, va_list v ) { add_s(fwords,v); }
	StrSet( const char *wordarray[] ) { add_s(wordarray); }
	StrSet( char *wordarray[] ) { add_s(wordarray); }

	VOID add_s( const char *fwords );
	VOID add_sf( const char *fwords, ... );
	VOID add_s( const char *fwords, va_list v );
	VOID add_s( const char *wordarray[] );
	VOID add_s( char* wordarray[] ) {add_s((const char**)wordarray);}

	VOID sort( BOOL reverse=FALSE );
	BOOL OK( VOID ) const;

	friend class StrList;
	friend class KVStrList;
};

std::ostream& operator << (std::ostream &st, const StrSet &l);

/**********************************************************/

class KVStrList  {
private:
	KeyValListT<String,String> l;

public:
	BOOL owns(Lix p) const { return l.owns(p); }
	LIINT index (Lix p) const { return l.index(p); }
	Lix lix(LIINT i) const { return l.lix(i); }
	LIINT length(VOID) const { return l.length(); }
	BOOL empty(VOID) const { return l.empty(); }
	VOID clear( VOID ) { l.clear(); };
	VOID reverse(VOID) { l.reverse(); }

	Lix first(VOID) const { return l.first(); }
	Lix last(VOID) const { return l.last(); }
	Lix next(Lix p) const { return l.next(p); }
	Lix prev(Lix p) const { return l.prev(p); }

	KVStrList & operator = (const KVStrList & a) { l.operator=(a.l); return *this; }

	Lix _prepend(VOID) {return l._prepend();}
	Lix _append(VOID) {return l._append();}
	Lix _insbefore(Lix p) {return l._insbefore(p);}
	Lix _insafter(Lix p) {return l._insafter(p);}
	Lix _prepend(const String& k, const String &v) {return l._prepend(k,v);}
	Lix _append(const String& k, const String &v) {return l._append(k,v);}
	Lix _insbefore(Lix p, const String& k, const String &v) {return l._insbefore(p,k,v);}
	Lix _insafter(Lix p, const String& k, const String &v) {return l._insafter(p,k,v);}
	Lix _prepend(const KVStrList & a) {return l._prepend(a.l);}
	Lix _append(const KVStrList & a) {return l._append(a.l);}
	Lix _insbefore(Lix p, const KVStrList & a) {return l._insbefore(p,a.l);}
	Lix _insafter(Lix p, const KVStrList & a) {return l._insafter(p,a.l);}
	Lix _prepend_mv(KVStrList & a) {return l._prepend_mv(a.l);}
	Lix _append_mv(KVStrList & a) {return l._append_mv(a.l);}
	Lix _insbefore_mv(Lix p, KVStrList & a) {return l._insbefore_mv(p,a.l);}
	Lix _insafter_mv(Lix p, KVStrList & a) {return l._insafter_mv(p,a.l);}

	Lix del(Lix p, INT dir = 1) {return l.del(p,dir);}
	VOID delprev(Lix p) {l.delprev(p);}
	VOID delnext(Lix p) {l.delnext(p);}
	Lix delfirst(VOID) {return l.delfirst();}
	Lix dellast(VOID) {return l.dellast();}

	const String& itemkey(Lix p) const {return l.itemkey(p);}
	const String& itemkey_first(VOID) const {return l.itemkey_first();}
	const String& itemkey_last(VOID) const {return l.itemkey_last();}
	String& _itemkey(Lix p) {return l._itemkey(p);}
	String& _itemkey_first(VOID) {return l._itemkey_first();}
	String& _itemkey_last(VOID) {return l._itemkey_last();}

	const String& itemval(Lix p) const {return l.itemval(p);}
	const String& itemval_first(VOID) const {return l.itemval_first();}
	const String& itemval_last(VOID) const {return l.itemval_last();}
	String& itemval(Lix p) {return l.itemval(p);}
	String& itemval_first(VOID) {return l.itemval_first();}
	String& itemval_last(VOID) {return l.itemval_last();}

	VOID exchange( Lix p1, Lix p2 ) {l.exchange(p1,p2);}
	VOID sortf( INT (*comparefunc) ( const String*item1, const String*item2 ))
			{l.sortf(comparefunc);}

	Lix add(const String& k, const String &v) {return l.add(k,v);}
	VOID add(const KVStrList & a) {l.add(a.l);}
	VOID add_mv(KVStrList & a, BOOL ovwrt) {l.add_mv(a.l,0,0,ovwrt);}

	Lix seek(const String& k) const {return l.seek(k);}
	BOOL contains(const String& k) const {return l.contains(k);}
	BOOL contains(const KVStrList &a) const {return l.contains(a.l);}
	BOOL contains(const char *k) const {return l.contains(String(k));}
	BOOL equals(const KVStrList &a, BOOL allowrep=TRUE) const {return l.equals(a.l,allowrep);}
	BOOL erase(const String & k) {return l.erase(k);}
	BOOL erase(const KVStrList & a) {return l.erase(a.l);}
	BOOL erase(const char * k) {return l.erase(String(k));}
	VOID keep(const KVStrList & a) {l.keep(a.l);}

	const String& val( const String& key ) const; /* { return l.val(key); } */
	const String& val( const String& key, const String& defval ) const { return l.val(key,defval); }

	Lix _insbefore(LIINT i) {return l._insbefore(i);}
	Lix _insbefore(LIINT i, const String& k, const String& v) {return l._insbefore(i,k,v);}
	Lix _insbefore(LIINT i, const KVStrList & a) {return l._insbefore(i,a.l);}
	Lix _insbefore_mv(LIINT i, KVStrList & a) {return l._insbefore_mv(i,a.l);}
	Lix _insafter(LIINT i) {return l._insafter(i);}
	Lix _insafter(LIINT i, const String& k, const String& v) {return l._insafter(i,k,v);}
	Lix _insafter(LIINT i, const KVStrList & a) {return l._insafter(i,a.l);}
	Lix _insafter_mv(LIINT i, KVStrList & a) {return l._insafter_mv(i,a.l);}
	VOID del(LIINT i) {l.del(i);}
	VOID delprev(LIINT i) {l.delprev(i);}
	VOID delnext(LIINT i) {l.delnext(i);}
	const String& itemkey(LIINT i) const {return l.itemkey(i);}
	String& _itemkey(LIINT i) {return l._itemkey(i);}
	const String& itemval(LIINT i) const {return l.itemval(i);}
	String& itemval(LIINT i) {return l.itemval(i);}
	VOID exchange( LIINT i1, LIINT i2 ) {l.exchange(i1,i2);}

	BOOL contains(const StrList&a) const {return l.contains(a.l);}
	BOOL equals(const StrList&a, BOOL allowrep=TRUE) const {return l.equals(a.l,allowrep);}
	BOOL erase(const StrList& a) {return l.erase(a.l);}
	VOID keep(const StrList& a) {l.keep(a.l);}
	BOOL contains(const StrSet&a) const {return l.contains(a.l);}
	BOOL equals(const StrSet&a, BOOL allowrep=TRUE) const {return l.equals(a.l,allowrep);}
	BOOL erase(const StrSet& a) {return l.erase(a.l);}
	VOID keep(const StrSet& a) {l.keep(a.l);}

protected:
	static int toi( const String &k, const String &v );
	static long tol( const String &k, const String &v );
	static float tof( const String &k, const String &v);
	static double tod( const String &k, const String &v );
	static BOOL tob( const String &k, const String &v );
public:
	KVStrList ( ) {};
	KVStrList ( const KVStrList &p) { l=p.l; }
	KVStrList ( const char *words, const char* sep="=" ) { add_s(words,sep); }
	KVStrList ( int dummy, const char *fwords, const char* sep="=", ... ) { (void)dummy; va_list v; va_start(v,sep); add_s(fwords,sep,v); va_end(v); }
	KVStrList ( const char *fwords, const char* sep, va_list v ) { add_s(fwords,sep,v); }
	KVStrList ( const char *wordarray[], const char*sep=" =\t\n") { add_s(wordarray,sep); }
	KVStrList ( char* wordarray[], const char*sep=" =\t\n") { add_s(wordarray,sep); }

	const char *cval( const String &k ) const { return val(k); }
	int ival( const String &k ) const { return toi(k,val(k)); }
	long lval( const String &k ) const { return tol(k,val(k)); }
	float fval( const String &k ) const { return tof(k,val(k)); }
	double dval( const String &k ) const { return tod(k,val(k)); }
	BOOL bval( const String &k ) const { return tob(k,val(k)); }

	const char *val( const String &k, const char *vdef ) const;
	const char *cval( const String &k, const char *vdef ) const { return val(k,vdef); }
	int ival( const String &k, int def ) const;
	long lval( const String &k, long def ) const;
	float fval( const String &k, float def ) const;
	double dval( const String &k, double def ) const;
	BOOL bval( const String &k, BOOL def ) const;
	BOOL bbval( const String &k ) const { return bval(k,FALSE); }

	Lix addc( const String &k, const char* s ) { return add(k,s); }
	Lix addi( const String &k, int val );
	Lix addui( const String &k, unsigned int val );
	Lix addl( const String &k, long val );
	Lix addf( const String &k, float val );
	Lix addd( const String &k, double val );
	Lix addb( const String &k, BOOL val );

	Lix add( const String &k, int val ) { return addi(k,val); }
	Lix add( const String &k, unsigned int val ) { return addui(k,val); }
	Lix add( const String &k, long val ) { return addl(k,val); }
	Lix add( const String &k, float val ) { return addf(k,val); }
	Lix add( const String &k, double val ) { return addd(k,val); }

	VOID add_prefix( const String &prefix );
	VOID remove_prefix( const String &prefix );

	VOID add_s( const char *words, const char* sep="=" );
	VOID add_sf( const char *fwords, const char* sep="=", ... );
	VOID add_s( const char *fwords, const char* sep, va_list v );
	VOID add_s( const char *wordarray[], const char*sep=" =\t\n");
	VOID add_s( char* wordarray[], const char*sep=" =\t\n") {add_s((const char**)wordarray,sep);}

	VOID sort( BOOL reverse=FALSE );
	BOOL OK( VOID ) const;

	friend VOID clargs2props( int argc, char *argv[], KVStrList  &props,
		 StrList &files, const char *format, const char *options,
		 const char *date );
};

std::ostream& operator << (std::ostream &st, const KVStrList  &l);

/**********************************************************/

VOID clargs2props( int argc, char *argv[], KVStrList  &props,
		StrList &files, const char *format, const char *options="",
		const char *date=__DATE__ );

/**********************************************************/

#ifdef KK  // $$$
inline StrList::StrList( const StrSet &a ) { l=a.l; };
inline StrList& StrList::operator = (const StrSet& a) { l.operator=(a.l); return *this; }
inline Lix StrList::prepend(const StrSet& a) { return l.prepend(a.l); }
inline Lix StrList::append(const StrSet& a) { return l.append(a.l); }
inline Lix StrList::insbefore(Lix p, const StrSet& a) { return l.insbefore(p,a.l); }
inline Lix StrList::insafter(Lix p, const StrSet& a) { return l.insafter(p,a.l); }
inline Lix StrList::prepend_mv(StrSet& a) { return l.prepend_mv(a.l); }
inline Lix StrList::append_mv(StrSet& a) { return l.append_mv(a.l); }
inline Lix StrList::insbefore_mv(Lix p, StrSet& a) { return l.insbefore_mv(p,a.l); }
inline Lix StrList::insafter_mv(Lix p, StrSet& a) { return l.insafter_mv(p,a.l); }
inline Lix StrList::insbefore(LIINT i, const StrSet& a) { return l.insbefore(i,a.l); }
inline Lix StrList::insbefore_mv(LIINT i, StrSet& a) { return l.insbefore_mv(i,a.l); }
inline Lix StrList::insafter(LIINT i, const StrSet& a) { return l.insafter(i,a.l); }
inline Lix StrList::insafter_mv(LIINT i, StrSet& a) { return l.insafter_mv(i,a.l); }
#endif

/**********************************************************/

#endif
