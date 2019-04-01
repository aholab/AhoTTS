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
#ifndef __LISTT_HPP__
#define __LISTT_HPP__

/**********************************************************/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/*
Copyright: 1996 - Grupo de Voz (DAET) ETSII/IT-Bilbao

Nombre fuente................ LISTT.HPP
Nombre paquete............... -
Lenguaje fuente.............. C++
Estado....................... Completado
Dependencia Hard/OS.......... -
Codigo condicional........... LIST_PTRVERIFY

Codificacion................. Borja Etxebarria

Version  dd/mm/aa  Autor     Comentario
-------  --------  --------  ----------
1.1.0    07/05/99  Borja     modif ??_mv()
1.0.0    26/03/98  Borja     recodificado, templates mas sencillas.
0.3.0    26/11/97  Borja     template ClassPtrT
0.2.1    26/08/97  Borja     uso LIINT (INT o LONG) en vez de INT
0.2.0    26/08/97  Borja     anyadir sin indicar elemento
0.1.0    12/04/97  Borja     anyadir index() y manejo con {i}, exchange()
0.0.0    06/05/96  Borja     Codificacion inicial.

======================== Contenido ========================
Manejo de listas (doblemente encadenadas) y variantes:

ListT     template para lista
SetListT  template para lista set o conjunto (elementos no repetidos)
KeyValListT   template para lista clave-valor (clave no repetida)

Ver LISTT.CPP para documentacion detallada.
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#ifndef __cplusplus
#error Must use C++ compiler
#endif

/**********************************************************/

#include <stdlib.h>
#include <assert.h>

#include "arch.h"
#include "tdef.h"

/**********************************************************/

typedef VOID * Lix;   // "List index" or cursor
typedef LONG LIINT;   // integer type used in lists (counts, indexs)

/**********************************************************/

#ifndef NDEBUG
#define LIST_PTRVERIFY
#endif

/**********************************************************/

class _PListNode {
public:
	_PListNode *b, *f;
	VOID *dp;
	_PListNode( VOID *data=NULL ) { dp=data; };
};

class _PList {
private:
	_PListNode *h;
protected:
	virtual VOID * _newData( const VOID *data ) = 0;
	virtual VOID _deleteData( VOID *data ) = 0;
private:
#ifdef LIST_PTRVERIFY
	Lix __lix_safe(LIINT i, CHAR*func, BOOL allow0) const;
#endif
	_PListNode *__newNode_d( const VOID *data=NULL );
	_PListNode *__newNode_p( VOID *data=NULL );
	virtual _PListNode *__newNode_d( const _PListNode *node );
	virtual _PListNode *__newNode_p( _PListNode *node );
	virtual VOID __deleteNode_d( _PListNode *p );
	VOID __deleteNode_p( _PListNode *p ) { delete p; }
	const _PListNode *__getNode( Lix p ) const { return (_PListNode*)p; }
protected:
	VOID ** __itemptr(Lix p) { return &(((_PListNode*)p)->dp); }

private:
	Lix __prepend(_PListNode *node);
	Lix __append(_PListNode *node);
	Lix __insbefore(Lix p, _PListNode *node);
	Lix __insafter(Lix p, _PListNode *node);
	Lix __del(Lix p, INT dir);
	VOID __clear(BOOL cdata);
protected:
	VOID _clear_p(VOID) { __clear(FALSE); }
	VOID _clear_d(VOID) { __clear(TRUE); }
private:
	_PList(_PList&) {};
public:
	_PList(VOID) { h=0; }
	virtual ~_PList() { clear(); }
protected:
	virtual _PList& _create_d(const _PList& a);
	virtual _PList& _create_p(_PList& a);

public:
	virtual BOOL OK(VOID) const;
	BOOL owns(Lix p) const;
	LIINT index (Lix p) const;
	Lix lix(LIINT i) const;
	LIINT length(VOID) const;
	BOOL empty(VOID) const { return h == 0; }
	virtual VOID clear( VOID ) { _clear_d(); };
	VOID reverse(VOID);

protected:
	Lix _prepend_d(const VOID* item=NULL) {return __prepend(__newNode_d(item));}
	Lix _append_d(const VOID* item=NULL) {return __append(__newNode_d(item));}
	Lix _insbefore_d(Lix p, const VOID* item=NULL) {return __insbefore(p,__newNode_d(item));}
	Lix _insafter_d(Lix p, const VOID* item=NULL) {return __insafter(p,__newNode_d(item));}

	Lix _prepend_p(VOID* item=NULL) {return __prepend(__newNode_p(item));}
	Lix _append_p(VOID *item=NULL) {return __append(__newNode_p(item));}
	Lix _insbefore_p(Lix p, VOID* item=NULL) {return __insbefore(p,__newNode_p(item));}
	Lix _insafter_p(Lix p, VOID* item=NULL) {return __insafter(p,__newNode_p(item));}

	Lix _prepend_d(const _PList& a);
	Lix _append_d(const _PList& a);
	Lix _insbefore_d(Lix p, const _PList& a);
	Lix _insafter_d(Lix p, const _PList& a);

	Lix _prepend_p(_PList& a);
	Lix _append_p(_PList& a);
	Lix _insbefore_p(Lix p, _PList& a);
	Lix _insafter_p(Lix p, _PList& a);

//	Lix _prepend_mv(_PList& a); // $$ quitar
	Lix _prepend_mv(_PList& a, Lix ni=0, Lix nf=0 );
//	Lix _append_mv(_PList& a);  // $$$ quitar
	Lix _append_mv(_PList& a, Lix ni=0, Lix nf=0 );
//	Lix _insbefore_mv(Lix p, _PList& a);  // $$$ quitar
//	Lix _insafter_mv(Lix p, _PList& a);   // $$$ quitar
	Lix _insbefore_mv(Lix p, _PList& a, Lix ni=0, Lix nf=0 );
	Lix _insafter_mv(Lix p, _PList& a, Lix ni=0, Lix nf=0 );

	Lix _del(Lix p, BOOL cdata, INT dir=1);
	VOID _delprev(Lix p, BOOL cdata);
	VOID _delnext(Lix p, BOOL cdata);
	Lix _delfirst(BOOL cdata);
	Lix _dellast(BOOL cdata);
public:
	Lix first(VOID) const { return Lix(h); }
	Lix last(VOID) const { return (h == 0) ? 0 : Lix(h->b); }
#ifdef LIST_PTRVERIFY
	Lix next(Lix p) const;
	Lix prev(Lix p) const;
#else
	Lix next(Lix p) const { if (p == 0 || p == h->b) return 0; return Lix(((_PListNode*)p)->f); }
	Lix prev(Lix p) const { if (p == 0 || p == h) return 0; return Lix(((_PListNode*)p)->b); }
#endif

protected:
#ifdef LIST_PTRVERIFY
	const VOID *_item(Lix p) const;
	const VOID *_item_first(VOID) const;
	const VOID *_item_last(VOID) const;
#else
	const VOID *_item(Lix p) const { if (p == 0) return 0; return ((_PListNode*)p)->dp; }
	const VOID *_item_first(VOID) const { if (h == 0) return 0; return h->dp; }
	const VOID *_item_last(VOID) const { if (h == 0) return 0; return h->b->dp; }
#endif

	virtual VOID __exchange_p( _PListNode *n1, _PListNode *n2 );
	VOID _exchange_lp( Lix p1, Lix p2 );
	VOID _exchange_p( Lix p1, Lix p2 );
	VOID _sort( INT (*comparefunc) ( const VOID *item1, const VOID *item2 ), BOOL lpmove=FALSE);
	Lix _movebefore_lp(Lix who, Lix where);
	Lix _moveafter_lp(Lix who, Lix where);


	// funcionamiento con indices
	Lix _insbefore_d(LIINT i, const VOID* item=NULL);
	Lix _insbefore_p(LIINT i, VOID* item=NULL);
	Lix _insbefore_d(LIINT i, const _PList& a);
	Lix _insbefore_p(LIINT i, _PList& a);
	Lix _insbefore_mv(LIINT i, _PList& a);
	Lix _insafter_d(LIINT i, const VOID* item=NULL);
	Lix _insafter_p(LIINT i, VOID* item=NULL);
	Lix _insafter_d(LIINT i, const _PList& a);
	Lix _insafter_p(LIINT i, _PList& a);
	Lix _insafter_mv(LIINT i, _PList& a);
	VOID _del(LIINT i, BOOL cdata);
	VOID _delprev(LIINT i, BOOL cdata);
	VOID _delnext(LIINT i, BOOL cdata);
	const VOID* _item(LIINT i) const;
	VOID _exchange_lp( LIINT i1, LIINT i2 );
	VOID _exchange_p( LIINT i1, LIINT i2 );
	Lix _movebefore_lp(LIINT who, LIINT where);
	Lix _moveafter_lp(LIINT who, LIINT where);

	friend class _SetPList;
	friend class _KVPList;
};

/**********************************************************/

class _SetPList: public _PList {
protected:
	virtual BOOL _sameData( const VOID *d1, const VOID *d2 ) const = 0;
	Lix __seek(const _PList&a, const VOID * x) const;
private:
	_SetPList(const _SetPList&) {};
public:
	_SetPList(VOID) {};

	virtual BOOL OK(VOID) const;
protected:
	_PList& _create_d(const _PList& a);
	_PList& _create_p(_PList& a);
protected:
	Lix _add_d(const VOID* x);
	Lix _add_p(VOID* x);
	VOID _add_d(const _PList& a);
	VOID _add_p(_PList& a);
	VOID _add_mv(_PList& a, Lix ni, Lix nf, BOOL ovwrt, BOOL cdata );

	Lix _seek(const VOID *x) const { return __seek(*this,x); }
	BOOL _contains(const VOID *x) const { return _seek(x)!=0; }
	BOOL _contains(const _PList&a) const;
	BOOL _equals(const _PList&a, BOOL allowrep=TRUE) const;
	BOOL _erase(const VOID* x, BOOL cdata);
	BOOL _erase(const _PList& a, BOOL cdata);
	VOID _keep(const _PList& a, BOOL cdata);
};

/**********************************************************/

class _KVPListNode : public _PListNode {
public:
	VOID *vp;
	_KVPListNode( VOID *key=NULL, VOID *val=NULL ) { dp=key; vp=val; };
};

class _KVPList: public _SetPList {
protected:
	virtual VOID * _newVal( const VOID *val) = 0;
	virtual VOID _deleteVal( VOID *val) = 0;
	virtual VOID _copyVal( VOID *dest, const VOID *src ) = 0;
private:
	_PListNode *__newNode_d_kv( const VOID *data=NULL, const VOID *val=NULL );
	_PListNode *__newNode_p_kv( VOID *data=NULL, VOID *val=NULL );
	_PListNode *__newNode_d( const _PListNode *node ) { return __newNode_d_kv(((_KVPListNode*)node)->dp,((_KVPListNode*)node)->vp); }
	_PListNode *__newNode_p( _PListNode *node ) {	return __newNode_p_kv(((_KVPListNode*)node)->dp,((_KVPListNode*)node)->vp); }
	VOID __deleteNode_d( _PListNode *p ) { _deleteData(((_KVPListNode*)p)->dp); _deleteVal(((_KVPListNode*)p)->vp); delete p; }
protected:
	VOID ** __itemvalptr(Lix p) { return &(((_KVPListNode*)p)->vp); }

private:
	_KVPList(_KVPList&) {};
public:
	_KVPList(VOID) { }

protected:
	Lix _prepend_d_kv(const VOID* key=NULL, const VOID* val=NULL) {return __prepend(__newNode_d_kv(key,val));}
	Lix _append_d_kv(const VOID* key=NULL, const VOID* val=NULL) {return __append(__newNode_d_kv(key,val));}
	Lix _insbefore_d_kv(Lix p, const VOID* key=NULL, const VOID* val=NULL) {return __insbefore(p,__newNode_d_kv(key,val));}
	Lix _insafter_d_kv(Lix p, const VOID* key=NULL, const VOID* val=NULL) {return __insafter(p,__newNode_d_kv(key,val));}

	Lix _prepend_p_kv(VOID* item=NULL, VOID* val=NULL) {return __prepend(__newNode_p_kv(item,val));}
	Lix _append_p_kv(VOID *item=NULL, VOID* val=NULL) {return __append(__newNode_p_kv(item,val));}
	Lix _insbefore_p_kv(Lix p, VOID* item=NULL, VOID* val=NULL) {return __insbefore(p,__newNode_p_kv(item,val));}
	Lix _insafter_p_kv(Lix p, VOID* item=NULL, VOID* val=NULL) {return __insafter(p,__newNode_p_kv(item,val));}

protected:
#ifdef LIST_PTRVERIFY
	const VOID *_itemval(Lix p) const;
	const VOID *_itemval_first(VOID) const;
	const VOID *_itemval_last(VOID) const;
#else
	const VOID *_itemval(Lix p) const { if (p == 0) return 0; return ((_KVPListNode*)p)->vp; }
	const VOID *_itemval_first(VOID) const { if (h == 0) return 0; return ((_KVPListNode*)h)->vp; }
	const VOID *_itemval_last(VOID) const { if (h == 0) return 0; return ((_KVPListNode*)h->b)->vp; }
#endif
	VOID __exchange_p( _PListNode *n1, _PListNode *n2 );

	// funcionamiento con indices
	Lix _insbefore_d_kv(LIINT i, const VOID* key=NULL, const VOID* val=NULL);
	Lix _insbefore_p_kv(LIINT i, VOID* key=NULL, VOID* val=NULL);
	Lix _insafter_d_kv(LIINT i, const VOID* key=NULL, const VOID* val=NULL);
	Lix _insafter_p_kv(LIINT i, VOID* key, VOID* val);
	const VOID* _itemval(LIINT i) const;

	const VOID * _val( const VOID *key ) const;
	const VOID * _val( const VOID *key, const VOID *defval ) const;

	Lix _add(const VOID* k, const VOID* v,
			BOOL rewrite, BOOL ovwrt, BOOL dup, BOOL cdata );
	VOID _add(const _PList& a, BOOL rewrite, BOOL ovwrt, BOOL dup, BOOL cdata );
};

/**********************************************************/

template <class T>
class ListT: public _PList {
protected:
	virtual VOID * _newData( const VOID *data ) { T *t=new T; if (data) *t=(*(T*)data); return t; }
	virtual VOID _deleteData( VOID *data ) { if (data) delete (T*)data; }
private:
	ListT(const ListT<T> &) {};
public:
	ListT() {};
	~ListT() { clear(); }

	ListT<T>& create(VOID) { return *this; }
	ListT<T>& create(const ListT<T>& a) { return (ListT<T>&)_create_d(a); }
	ListT<T>& operator = (const ListT<T>& a) { return (ListT<T>&)_create_d(a); }

	Lix prepend(VOID) {return _prepend_d(); }
	Lix append(VOID) {return _append_d(); }
	Lix insbefore(Lix p) {return _insbefore_d(p); }
	Lix insafter(Lix p) {return _insafter_d(p); }
	Lix prepend(const T& d) {return _prepend_d((VOID*)&d); }
	Lix append(const T& d) {return _append_d((VOID*)&d); }
	Lix insbefore(Lix p, const T& d) {return _insbefore_d(p,(VOID*)&d); }
	Lix insafter(Lix p, const T& d) {return _insafter_d(p,(VOID*)&d); }
	Lix prepend(const ListT<T>& a) { return _prepend_d(a); }
	Lix append(const ListT<T>& a) { return _append_d(a); }
	Lix insbefore(Lix p, const ListT<T>& a) { return _insbefore_d(p,a); }
	Lix insafter(Lix p, const ListT<T>& a) { return _insafter_d(p,a); }
	Lix prepend_p(T* d) {return _prepend_p((VOID*)d); }
	Lix append_p(T* d) {return _append_p((VOID*)d); }
	Lix insbefore_p(Lix p, T* d) {return _insbefore_p(p,(VOID*)d); }
	Lix insafter_p(Lix p, T* d) {return _insafter_p(p,(VOID*)d); }
	Lix prepend_mv(ListT<T>& a, Lix ni=0, Lix nf=0 ) { return _prepend_mv(a,ni,nf); }
	Lix append_mv(ListT<T>& a, Lix ni=0, Lix nf=0) { return _append_mv(a,ni,nf); }
	Lix insbefore_mv(Lix p, ListT<T>& a, Lix ni=0, Lix nf=0) { return _insbefore_mv(p,a,ni,nf); }
	Lix insafter_mv(Lix p, ListT<T>& a, Lix ni=0, Lix nf=0) { return _insafter_mv(p,a,ni,nf); }

	Lix del(Lix p, INT dir = 1) { return _del(p,TRUE,dir); }
	VOID delprev(Lix p) { _delprev(p,TRUE); }
	VOID delnext(Lix p) { _delnext(p,TRUE); }
	Lix delfirst(VOID) { return _delfirst(TRUE); }
	Lix dellast(VOID) { return _dellast(TRUE); }

	const T& item(Lix p) const { return *(const T*)_item(p); }
	const T& operator () (Lix p) const { return *(const T*)_item(p); }
	const T& item_first(VOID) const { return *(const T*)_item_first(); }
	const T& item_last(VOID) const { return *(const T*)_item_last(); }
	T& item(Lix p) { return *(T*)_item(p); }
	//T& operator () (Lix p) { return *(T*)_item(p); }
	T& item_first(VOID) { return *(T*)_item_first(); }
	T& item_last(VOID) { return *(T*)_item_last(); }
	T* &item_p(Lix p) { return *(T**)__itemptr(p); }

	VOID exchange( Lix p1, Lix p2 ) { _exchange_p(p1,p2 ); }
	VOID sortf( INT (*comparefunc) ( const T*item1, const T*item2 ))
			{ _sort((INT (*)(const VOID*,const VOID*))comparefunc); }
	Lix movebefore(Lix who, Lix where) { return _movebefore_lp(who,where); }
	Lix moveafter(Lix who, Lix where) { return _moveafter_lp(who,where); }

	// funcionamiento con indices
	Lix insbefore(LIINT i) { return _insbefore_d(i); }
	Lix insbefore(LIINT i, const T& item) { return _insbefore_d(i,(VOID*)&item); }
	Lix insbefore(LIINT i, const ListT<T>& a) { return _insbefore_d(i,a); }
	Lix insbefore_mv(LIINT i, ListT<T>& a) { return _insbefore_mv(i,a); }
	Lix insafter(LIINT i) { return _insafter_d(i); }
	Lix insafter(LIINT i, const T& item) { return _insafter_d(i,(VOID*)&item); }
	Lix insafter(LIINT i, const ListT<T>& a) { return _insafter_d(i,a); }
	Lix insafter_mv(LIINT i, ListT<T>& a) { return _insafter_mv(i,a); }
	VOID del(LIINT i) { _del(i,TRUE); }
	VOID delprev(LIINT i) { _delprev(i,TRUE); }
	VOID delnext(LIINT i) { _delnext(i,TRUE); }
	const T& item(LIINT i) const { return *(const T*)_item(i); }
	const T& operator () (LIINT i) const { return *(const T*)_item(i); }
	T& item(LIINT i) { return *(T*)_item(i); }
	//T& operator () (LIINT i) { return *(T*)_item(i); }
	VOID exchange( LIINT i1, LIINT i2 ) { _exchange_p(i1,i2 ); }
	Lix movebefore(LIINT who, LIINT where) { return _movebefore_lp(who,where); }
	Lix moveafter(LIINT who, LIINT where) { return _moveafter_lp(who,where); }

#ifdef KK // $$$ da problemas
	// Para que listas SetListT tambien se puedan usar con listas ListT
	ListT<T>& create(const SetListT<T>& a) { return (ListT<T>&)_create_d(a); };
	ListT<T>& operator = (const SetListT<T>& a) { return (ListT<T>&)_create_d(a); }
	Lix prepend(const SetListT<T>& a) { return _prepend_d(a); }
	Lix append(const SetListT<T>& a) { return _append_d(a); }
	Lix insbefore(Lix p, const SetListT<T>& a) { return _insbefore_d(p,a); }
	Lix insafter(Lix p, const SetListT<T>& a) { return _insafter_d(p,a); }
	Lix prepend_mv(SetListT<T>& a, Lix ni=0, Lix nf=0) { return _prepend_mv(a,ni,nf); }
	Lix append_mv(SetListT<T>& a, Lix ni=0, Lix nf=0) { return _append_mv(a,ni,nf); }
	Lix insbefore_mv(Lix p, SetListT<T>& a, Lix ni=0, Lix nf=0) { return _insbefore_mv(p,a,ni,nf); }
	Lix insafter_mv(Lix p, SetListT<T>& a, Lix ni=0, Lix nf=0) { return _insafter_mv(p,a,ni,nf); }
	Lix insbefore(LIINT i, const SetListT<T>& a) { return _insbefore_d(i,a); }
	Lix insbefore_mv(LIINT i, SetListT<T>& a) { return _insbefore_mv(i,a); }
	Lix insafter(LIINT i, const SetListT<T>& a) { return _insafter_d(i,a); }
	Lix insafter_mv(LIINT i, SetListT<T>& a) { return _insafter_mv(i,a); }

	SetListT<T>& toSetList(VOID) { return *(SetListT<T>*)((_PList*)this); }
#endif
};

/**********************************************************/

template <class T>
class SetListT: public _SetPList {
protected:
	virtual VOID * _newData( const VOID *data ) { T *t=new T; if (data) *t=(*(T*)data); return t; }
	virtual VOID _deleteData( VOID *data ) { if (data) delete (T*)data; }
	virtual BOOL _sameData( const VOID *d1, const VOID *d2 ) const { return (*(const T*)d1) == (*(const T*)d2); }
private:
	SetListT(const SetListT<T> &) {};
public:
	SetListT() {};
	~SetListT() { clear(); }

	SetListT<T>& create(VOID) { return *this; }
	SetListT<T>& create(const SetListT<T>& a) { return (SetListT<T>&)_create_d(a); }
	SetListT<T>& operator = (const SetListT<T>& a) { return (SetListT<T>&)_create_d(a); }

	Lix _prepend(VOID) {return _prepend_d(); }
	Lix _append(VOID) {return _append_d(); }
	Lix _insbefore(Lix p) {return _insbefore_d(p); }
	Lix _insafter(Lix p) {return _insafter_d(p); }
	Lix _prepend(const T& d) {return _prepend_d((VOID*)&d); }
	Lix _append(const T& d) {return _append_d((VOID*)&d); }
	Lix _insbefore(Lix p, const T& d) {return _insbefore_d(p,(VOID*)&d); }
	Lix _insafter(Lix p, const T& d) {return _insafter_d(p,(VOID*)&d); }
	Lix _prepend(const SetListT<T>& a) { return _prepend_d(a); }
	Lix _append(const SetListT<T>& a) { return _append_d(a); }
	Lix _insbefore(Lix p, const SetListT<T>& a) { return _insbefore_d(p,a); }
	Lix _insafter(Lix p, const SetListT<T>& a) { return _insafter_d(p,a); }
	Lix _prepend_mv(SetListT<T>& a, Lix ni=0, Lix nf=0) { return _SetPList::_prepend_mv(a,ni,nf); }
	Lix _append_mv(SetListT<T>& a, Lix ni=0, Lix nf=0) { return _SetPList::_append_mv(a,ni,nf); }
	Lix _insbefore_mv(Lix p, SetListT<T>& a, Lix ni=0, Lix nf=0) { return _SetPList::_insbefore_mv(p,a,ni,nf); }
	Lix _insafter_mv(Lix p, SetListT<T>& a, Lix ni=0, Lix nf=0) { return _SetPList::_insafter_mv(p,a,ni,nf); }

	Lix del(Lix p, INT dir = 1) { return _del(p,TRUE,dir); }
	VOID delprev(Lix p) { _delprev(p,TRUE); }
	VOID delnext(Lix p) { _delnext(p,TRUE); }
	Lix delfirst(VOID) { return _delfirst(TRUE); }
	Lix dellast(VOID) { return _dellast(TRUE); }

	const T& item(Lix p) const { return *(const T*)_SetPList::_item(p); }
	const T& operator () (Lix p) const { return *(const T*)_SetPList::_item(p); }
	const T& item_first(VOID) const { return *(const T*)_SetPList::_item_first(); }
	const T& item_last(VOID) const { return *(const T*)_SetPList::_item_last(); }
	T& _item(Lix p) { return *(T*)_SetPList::_item(p); }
	//T& operator () (Lix p) { return *(T*)_SetPList::_item(p); }
	T& _item_first(VOID) { return *(T*)_SetPList::_item_first(); }
	T& _item_last(VOID) { return *(T*)_SetPList::_item_last(); }

	VOID exchange( Lix p1, Lix p2 ) { _exchange_p(p1,p2 ); }
	VOID sortf( INT (*comparefunc) ( const T*item1, const T*item2 ))
			{ _sort((INT (*)(const VOID*,const VOID*))comparefunc); }
	Lix movebefore(Lix who, Lix where) { return _movebefore_lp(who,where); }
	Lix moveafter(Lix who, Lix where) { return _moveafter_lp(who,where); }

	Lix add(const T& x) { return _add_d((const VOID*)&x); }
	VOID add(const SetListT<T>& a) { _add_d(a); }
	VOID add_mv(SetListT<T>& a, Lix ni, Lix nf, BOOL ovwrt) { _add_mv(a,ni,nf,ovwrt,TRUE); }

	Lix seek(const T& x) const { return _seek((const VOID*)&x); }
	BOOL contains(const T& x) const { return _contains((const VOID*)&x); }
	BOOL contains(const SetListT<T>&a) const { return _contains(a); }
	BOOL equals(const SetListT<T>&a, BOOL allowrep=TRUE) const { return _equals(a,allowrep); }
	BOOL erase(const T & x) { return _erase((const VOID *)&x,TRUE); }
	BOOL erase(const SetListT<T>& a) { return _erase(a,TRUE); }
	VOID keep(const SetListT<T>& a) { _keep(a,TRUE); }

	// funcionamiento con indices
	Lix _insbefore(LIINT i) { return _insbefore_d(i); }
	Lix _insbefore(LIINT i, const T& item) { return _insbefore_d(i,(VOID*)&item); }
	Lix _insbefore(LIINT i, const SetListT<T>& a) { return _insbefore_d(i,a); }
	Lix _insbefore_mv(LIINT i, SetListT<T>& a) { return _SetPList::_insbefore_mv(i,a); }
	Lix _insafter(LIINT i) { return _insafter_d(i); }
	Lix _insafter(LIINT i, const T& item) { return _insafter_d(i,(VOID*)&item); }
	Lix _insafter(LIINT i, const SetListT<T>& a) { return _insafter_d(i,a); }
	Lix _insafter_mv(LIINT i, SetListT<T>& a) { return _SetPList::_insafter_mv(i,a); }
	VOID del(LIINT i) { _del(i,TRUE); }
	VOID delprev(LIINT i) { _delprev(i,TRUE); }
	VOID delnext(LIINT i) { _delnext(i,TRUE); }
	const T& item(LIINT i) const { return *(const T*)_SetPList::_item(i); }
	const T& operator () (LIINT i) const { return *(const T*)_SetPList::_item(i); }
	T& _item(LIINT i) { return *(T*)_SetPList::_item(i); }
	//T& operator () (LIINT i) { return *(T*)_item(i); }
	VOID exchange( LIINT i1, LIINT i2 ) { _exchange_p(i1,i2 ); }
	Lix movebefore(LIINT who, LIINT where) { return _movebefore_lp(who,where); }
	Lix moveafter(LIINT who, LIINT where) { return _moveafter_lp(who,where); }

	// Para que listas ListT tambien se puedan usar con listas SetListT
	SetListT<T>& create(const ListT<T>& a) { return (SetListT<T>&)_create_d(a); }
	SetListT<T>& operator = (const ListT<T>& a) { return (SetListT<T>&)_create_d(a); }
	Lix _prepend(const ListT<T>& a) { return _prepend_d(a); }
	Lix _append(const ListT<T>& a) { return _append_d(a); }
	Lix _insbefore(Lix p, const ListT<T>& a) { return _insbefore_d(p,a); }
	Lix _insafter(Lix p, const ListT<T>& a) { return _insafter_d(p,a); }
	Lix _prepend_mv(ListT<T>& a, Lix ni=0, Lix nf=0) { return _SetPList::_prepend_mv(a,ni,nf); }
	Lix _append_mv(ListT<T>& a, Lix ni=0, Lix nf=0) { return _SetPList::_append_mv(a,ni,nf); }
	Lix _insbefore_mv(Lix p, ListT<T>& a, Lix ni=0, Lix nf=0) { return _SetPList::_insbefore_mv(p,a,ni,nf); }
	Lix _insafter_mv(Lix p, ListT<T>& a, Lix ni=0, Lix nf=0) { return _SetPList::_insafter_mv(p,a,ni,nf); }
	VOID add(const ListT<T>& a) { _add_d(a); }
	VOID add_mv(ListT<T>& a, Lix ni, Lix nf, BOOL ovwrt) { _add_mv(a,ni,nf,ovwrt,TRUE); }
	BOOL contains(const ListT<T>&a) const { return _contains(a); }
	BOOL equals(const ListT<T>&a, BOOL allowrep=TRUE) const { return _equals(a,allowrep); }
	BOOL erase(const ListT<T>& a) { return _erase(a,TRUE); }
	VOID keep(const ListT<T>& a) { _keep(a,TRUE); }
	Lix _insbefore(LIINT i, const ListT<T>& a) { return _insbefore_d(i,a); }
	Lix _insbefore_mv(LIINT i, ListT<T>& a) { return _SetPList::_insbefore_mv(i,a); }
	Lix _insafter(LIINT i, const ListT<T>& a) { return _insafter_d(i,a); }
	Lix _insafter_mv(LIINT i, ListT<T>& a) { return _SetPList::_insafter_mv(i,a); }

	ListT<T>& toList(VOID) { return *(ListT<T>*)((_PList*)this); }
};

/**********************************************************/

template <class K, class V>
class KeyValListT: public _KVPList {
protected:
	virtual VOID * _newData( const VOID *data ) { K *t=new K; if (data) *t=(*(K*)data); return t; }
	virtual VOID _deleteData( VOID *data ) { if (data) delete (K*)data; }
	virtual BOOL _sameData( const VOID *d1, const VOID *d2 ) const { return *(K*)d1 == *(K*)d2; }
	virtual VOID * _newVal( const VOID *val) { V *t=new V; if (val) *t=(*(V*)val); return t; }
	virtual VOID _deleteVal( VOID *val) { if (val) delete (V*)val; }
	virtual VOID _copyVal( VOID *dest, const VOID *src ) { if (src && dest) *(V*)dest = *(V*)src; }
private:
	KeyValListT(const KeyValListT<K,V> &) {};
public:
	KeyValListT() {};
	~KeyValListT() { clear(); }

	KeyValListT<K,V>& create(VOID) { return *this; }
	KeyValListT<K,V>& create(const KeyValListT<K,V>& a) { return (KeyValListT<K,V>&)_create_d(a); }
	KeyValListT<K,V>& operator = (const KeyValListT<K,V>& a) { return (KeyValListT<K,V>&)_create_d(a); }

	Lix _prepend(VOID) {return _prepend_d(); }
	Lix _append(VOID) {return _append_d(); }
	Lix _insbefore(Lix p) {return _insbefore_d(p); }
	Lix _insafter(Lix p) {return _insafter_d(p); }
	Lix _prepend(const K& d, const V& v) {return _prepend_d_kv((VOID*)&d,(VOID*)&v); }
	Lix _append(const K& d, const V& v) {return _append_d_kv((VOID*)&d,(VOID*)&v); }
	Lix _insbefore(Lix p, const K& d, const V& v) {return _insbefore_d_kv(p,(VOID*)&d,(VOID*)&v); }
	Lix _insafter(Lix p, const K& d, const V& v) {return _insafter_d_kv(p,(VOID*)&d,(VOID*)&v); }
	Lix _prepend(const KeyValListT<K,V>& a) { return _prepend_d(a); }
	Lix _append(const KeyValListT<K,V>& a) { return _append_d(a); }
	Lix _insbefore(Lix p, const KeyValListT<K,V>& a) { return _insbefore_d(p,a); }
	Lix _insafter(Lix p, const KeyValListT<K,V>& a) { return _insafter_d(p,a); }
	Lix _prepend_mv(KeyValListT<K,V>& a, Lix ni=0, Lix nf=0) { return _KVPList::_prepend_mv(a,ni,nf); }
	Lix _append_mv(KeyValListT<K,V>& a, Lix ni=0, Lix nf=0) { return _KVPList::_append_mv(a,ni,nf); }
	Lix _insbefore_mv(Lix p, KeyValListT<K,V>& a, Lix ni=0, Lix nf=0) { return _KVPList::_insbefore_mv(p,a,ni,nf); }
	Lix _insafter_mv(Lix p, KeyValListT<K,V>& a, Lix ni=0, Lix nf=0) { return _KVPList::_insafter_mv(p,a,ni,nf); }

	Lix del(Lix p, INT dir = 1) { return _del(p,TRUE,dir); }
	VOID delprev(Lix p) { _delprev(p,TRUE); }
	VOID delnext(Lix p) { _delnext(p,TRUE); }
	Lix delfirst(VOID) { return _delfirst(TRUE); }
	Lix dellast(VOID) { return _dellast(TRUE); }

	const K& itemkey(Lix p) const { return *(const K*)_item(p); }
	const K& itemkey_first(VOID) const { return *(const K*)_item_first(); }
	const K& itemkey_last(VOID) const { return *(const K*)_item_last(); }
	K& _itemkey(Lix p) { return *(K*)_item(p); }
	K& _itemkey_first(VOID) { return *(K*)_item_first(); }
	K& _itemkey_last(VOID) { return *(K*)_item_last(); }

	const V& itemval(Lix p) const { return *(const V*)_itemval(p); }
	const V& itemval_first(VOID) const { return *(const V*)_itemval_first(); }
	const V& itemval_last(VOID) const { return *(const V*)_itemval_last(); }
	V& itemval(Lix p) { return *(V*)_itemval(p); }
	V& itemval_first(VOID) { return *(V*)_itemval_first(); }
	V& itemval_last(VOID) { return *(V*)_itemval_last(); }

	VOID exchange( Lix p1, Lix p2 ) { _exchange_p(p1,p2 ); }
	VOID sortf( INT (*comparefunc) ( const K*item1, const K*item2 ))
			{ _sort((INT (*)(const VOID*,const VOID*))comparefunc); }
	Lix movebefore(Lix who, Lix where) { return _movebefore_lp(who,where); }
	Lix moveafter(Lix who, Lix where) { return _moveafter_lp(who,where); }

	Lix add(const K& x,const V& v, BOOL rewrite=TRUE) { return _add((const VOID*)&x,(const VOID*)&v,rewrite,TRUE,TRUE,TRUE); }
	VOID add(const KeyValListT<K,V>& a, BOOL rewrite=TRUE) { _add((const _PList&)a,rewrite,TRUE,TRUE,TRUE); }
	VOID add_mv(KeyValListT<K,V>& a, Lix ni, Lix nf, BOOL rewrite=TRUE) { _add_mv(a,ni,nf,rewrite,TRUE); }

	Lix seek(const K& x) const { return _seek((const VOID*)&x); }
	BOOL contains(const K& x) const { return _contains((const VOID*)&x); }
	BOOL contains(const KeyValListT<K,V>&a) const { return _contains(a); }
	BOOL equals(const KeyValListT<K,V>&a, BOOL allowrep=TRUE) const { return _equals(a,allowrep); }
	BOOL erase(const K & x) { return _erase((const VOID *)&x,TRUE); }
	BOOL erase(const KeyValListT<K,V>& a) { return _erase(a,TRUE); }
	VOID keep(const KeyValListT<K,V>& a) { _keep(a,TRUE); }

	const V& val( const K& key ) const { return *(const V*)_val((const VOID*)&key); }
	const V& val( const K& key, const V& defval ) const { return *(const V*)_val((const VOID*)&key,(const VOID*)&defval); }

	// funcionamiento con indices
	Lix _insbefore(LIINT i) { return _insbefore_d(i); }
	Lix _insbefore(LIINT i, const K& item, const V&val) { return _insbefore_d_kv(i,(VOID*)&item,(VOID*)&val); }
	Lix _insbefore(LIINT i, const KeyValListT<K,V>& a) { return _insbefore_d(i,a); }
	Lix _insbefore_mv(LIINT i, KeyValListT<K,V>& a) { return _KVPList::_insbefore_mv(i,a); }
	Lix _insafter(LIINT i) { return _insafter_d(i); }
	Lix _insafter(LIINT i, const K& item, const V&val) { return _insafter_d_kv(i,(VOID*)&item,(VOID*)&val); }
	Lix _insafter(LIINT i, const KeyValListT<K,V>& a) { return _insafter_d(i,a); }
	Lix _insafter_mv(LIINT i, KeyValListT<K,V>& a) { return _KVPList::_insafter_mv(i,a); }
	VOID del(LIINT i) { _del(i,TRUE); }
	VOID delprev(LIINT i) { _delprev(i,TRUE); }
	VOID delnext(LIINT i) { _delnext(i,TRUE); }
	const K& itemkey(LIINT i) const { return *(const K*)_item(i); }
	K& _itemkey(LIINT i) { return *(K*)_item(i); }
	VOID exchange( LIINT i1, LIINT i2 ) { _exchange_p(i1,i2); }
	const V& itemval(LIINT i) const { return *(const V*)_itemval(i); }
	V& itemval(LIINT i) { return *(V*)_itemval(i); }
	Lix movebefore(LIINT who, LIINT where) { return _movebefore_lp(who,where); }
	Lix moveafter(LIINT who, LIINT where) { return _moveafter_lp(who,where); }

	// Para que listas ListT y SetListT tambien se puedan usar con KeyValListT
	BOOL contains(const ListT<K>&a) const { return _contains(a); }
	BOOL equals(const ListT<K>&a, BOOL allowrep=TRUE) const { return _equals(a,allowrep); }
	BOOL erase(const ListT<K>& a) { return _erase(a,TRUE); }
	VOID keep(const ListT<K>& a) { _keep(a,TRUE); }
	BOOL contains(const SetListT<K>&a) const { return _contains(a); }
	BOOL equals(const SetListT<K>&a, BOOL allowrep=TRUE) const { return _equals(a,allowrep); }
	BOOL erase(const SetListT<K>& a) { return _erase(a,TRUE); }
	VOID keep(const SetListT<K>& a) { _keep(a,TRUE); }
};

/**********************************************************/

#ifdef kk
template<class T>
class ClassPtrT {
public:
	T* ptr;
	ClassPtrT(VOID) { ptr=0; }
	ClassPtrT(T *t) { ptr=t; }
	~ClassPtrT(VOID) { if (ptr) delete ptr; }
};
#endif

/**********************************************************/

#endif
