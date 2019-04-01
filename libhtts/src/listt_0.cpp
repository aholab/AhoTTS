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
Copyright: 1996 - Grupo de Voz (DAET) ETSII/IT-Bilbao

Nombre fuente................ LISTT.CPP
Nombre paquete............... -
Lenguaje fuente.............. C++
Estado....................... Completado
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Borja Etxebarria

Version  dd/mm/aa  Autor     Comentario
-------  --------  --------  ----------
1.0.1    30/08/98  Borja     split varios modulos listt_?.cpp

======================== Contenido ========================
ver doc. en listt.cpp
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include <limits.h>

#include "listt_i.hpp"

/**********************************************************/

_PList& _PList::_create_d(const _PList& a)
{
	if (h != a.h) { clear(); _append_d(a); }
	return *this;
}

/**********************************************************/

_PList& _PList::_create_p(_PList& a)
{
	if (h != a.h) { clear(); _append_p(a); }
	return *this;
}

/**********************************************************/

BOOL _PList::OK(VOID) const
{
	BOOL v = TRUE;
	if (h != 0) {
		_PListNode* t = h;
		long count = LONG_MAX;
		do {
			count--;
			v &= t->b->f == t;
			v &= t->f->b == t;
			t = t->f;
		} while (v && count > 0 && t != h);
		v &= count > 0;
	}
	return v;
}

/**********************************************************/

BOOL _PList::owns(Lix p) const
{
	_PListNode* t = h;
	if (t != 0 && p != 0) {
		do {
			if (Lix(t) == p)
				return TRUE;
			t = t->f;
		} while (t != h);
	}
	return FALSE;
}

/**********************************************************/

LIINT _PList::index (Lix p) const
{
	_PListNode* t = h;
	if ((p!=0) && (t!=0)) {
		LIINT l = 0;
		do {
			if (Lix(t) == p)
				return l;
			l++;
			t = t->f;
		} while (t != h);
	}
	LISTVERIFY(p,"index");
	return -1;
}

/**********************************************************/

Lix _PList::lix( LIINT i ) const
{
	_PListNode* t = h;
	if (t != 0) do { if ((i--)==0) return t; t = t->f; } while (t != h);
	return 0;
}

/**********************************************************/

LIINT _PList::length(VOID) const
{
	LIINT l = 0;
	_PListNode* t = h;
	if (t != 0) do { ++l; t = t->f; } while (t != h);
	return l;
}

/**********************************************************/

VOID _PList::reverse(VOID)
{
	if (h != 0) {
		_PListNode* w;
		_PListNode* t = h;
		do { __SWAP__(t->b,t->f,w); t=t->b; } while (t!=h);
		h = t->f;
	}
}

/**********************************************************/

Lix _PList::_prepend_d(const _PList& a)
{
	for (Lix p=a.last(); p!=0; p=a.prev(p))
		__prepend(__newNode_d(a.__getNode(p)));
	return first();
}

/**********************************************************/

Lix _PList::_append_d(const _PList& a)
{
	for (Lix p=a.first(); p!=0; p=a.next(p))
		__append(__newNode_d(a.__getNode(p)));
	return last();
}

/**********************************************************/

Lix _PList::_insbefore_d(Lix p, const _PList& a)
{
	if (p == 0) return _append_d(a);
	LISTVERIFY(p,"_insbefore_d");
	for (Lix q=a.last(); q!=0; q=a.prev(q))
		p=__insbefore(p,__newNode_d(a.__getNode(q)));
	return p;
}

/**********************************************************/

Lix _PList::_insafter_d(Lix p, const _PList& a)
{
	if (p == 0) return _prepend_d(a);
	LISTVERIFY(p,"_insafter_d");
	for (Lix q=a.first(); q!=0; q=a.next(q))
		p=__insafter(p,__newNode_d(a.__getNode(q)));
	return p;
}

/**********************************************************/

Lix _PList::_prepend_p(_PList& a)
{
	for (Lix p=a.last(); p!=0; p=a.prev(p))
		__prepend(__newNode_p((_PListNode*)a.__getNode(p)));
	return first();
}

/**********************************************************/

Lix _PList::_append_p(_PList& a)
{
	for (Lix p=a.first(); p!=0; p=a.next(p))
		__append(__newNode_p((_PListNode*)a.__getNode(p)));
	return last();
}

/**********************************************************/

Lix _PList::_insbefore_p(Lix p, _PList& a)
{
	if (p == 0) return _append_p(a);
	LISTVERIFY(p,"_insbefore_p");
	for (Lix q=a.last(); q!=0; q=a.prev(q))
		p=__insbefore(p,__newNode_p((_PListNode*)a.__getNode(q)));
	return p;
}

/**********************************************************/

Lix _PList::_insafter_p(Lix p, _PList& a)
{
	if (p == 0) return _prepend_p(a);
	LISTVERIFY(p,"_insafter_p");
	for (Lix q=a.first(); q!=0; q=a.next(q))
		p=__insafter(p,__newNode_p((_PListNode*)a.__getNode(q)));
	return p;
}

/**********************************************************/

Lix _PList::_prepend_mv(_PList& a, Lix ni, Lix nf )
{
	_PListNode* t1 = (_PListNode*)(ni?ni:a.h);
	_PListNode* t2 = (_PListNode*)(nf?nf: a.h?a.h->b:0);
#ifdef LIST_PTRVERIFY
	XLISTVERIFY(a,(Lix)t1,"_prepend_mv");
	XLISTVERIFY(a,(Lix)t2,"_prepend_mv");
	if (t1 && t2) assert(a.index(t1)<=a.index(t2));
#endif
	if (t1->b == t2) a.h = 0;
	else {
		t1->b->f = t2->f;
		t2->f->b = t1->b;
		if (a.h==t1) a.h=t2->f;
	}

	if (t1 && t2) {
		if (h==0) {
			h = t1;
			t1->b=t2;
			t2->f=t1;
		}
		else {
			t1->b = h->b;
			h->b->f=t1;
			t2->f = h;
			h->b=t2;
			h=t1;
		}
	}
	return Lix(h);
}

#ifdef ORI
Lix _PList::_prepend_mv(_PList& a )
{
	_PListNode* t = a.h;
	a.h = 0;
	if (h == 0)
		h = t;
	else if (t != 0) {
		_PListNode* l = h->b;
		t->b->f = h;
		h->b = t->b;
		t->b = l;
		l->f = t;
		h = t;
	}
	return Lix(h);
}
#endif

/**********************************************************/

Lix _PList::_append_mv(_PList& a, Lix ni, Lix nf )
{
	_PListNode* t1 = (_PListNode*)(ni?ni:a.h);
	_PListNode* t2 = (_PListNode*)(nf?nf: a.h?a.h->b:0);
#ifdef LIST_PTRVERIFY
	XLISTVERIFY(a,(Lix)t1,"_append_mv");
	XLISTVERIFY(a,(Lix)t2,"_append_mv");
	if (t1 && t2) assert(a.index(t1)<=a.index(t2));
#endif
	if (t1->b == t2) a.h = 0;
	else {
		t1->b->f = t2->f;
		t2->f->b = t1->b;
		if (a.h==t1) a.h=t2->f;
	}

	if (t1 && t2) {
		if (h==0) {
			h = t1;
			t1->b=t2;
			t2->f=t1;
		}
		else {
			t1->b = h->b;
			h->b->f=t1;
			t2->f = h;
			h->b=t2;
		}
	}
	return Lix(h->b);
}

#ifdef ORI
Lix _PList::_append_mv(_PList& a)
{
	_PListNode* t = a.h;
	a.h = 0;
	if (h == 0)
		h = t;
	else if (t != 0) {
		_PListNode* l = t->b;
		h->b->f = t;
		t->b = h->b;
		h->b = l;
		l->f = h;
	}
	return Lix(h->b);
}
#endif

/**********************************************************/

Lix _PList::_insbefore_mv(Lix p, _PList& a, Lix ni, Lix nf )
{
	if (p==0) return _append_mv(a,ni,nf);
	LISTVERIFY(p,"_insbefore_mv");
	if ((p==h)||(!h)) return _prepend_mv(a);

	_PListNode* t1 = (_PListNode*)(ni?ni:a.h);
	_PListNode* t2 = (_PListNode*)(nf?nf: a.h?a.h->b:0);
#ifdef LIST_PTRVERIFY
	XLISTVERIFY(a,(Lix)t1,"_insbefore_mv");
	XLISTVERIFY(a,(Lix)t2,"_insbefore_mv");
	if (t1 && t2) assert(a.index(t1)<=a.index(t2));
#endif
	if (t1->b == t2) a.h = 0;
	else {
		t1->b->f = t2->f;
		t2->f->b = t1->b;
		if (a.h==t1) a.h=t2->f;
	}

	if (t1 && t2) {
		_PListNode* l = (_PListNode*)p;
		t1->b = l->b;
		t2->f = l;
		l->b->f=t1;
		l->b=t2;
	}

	return Lix(t1);
}

#ifdef ORI
Lix _PList::_insbefore_mv(Lix p, _PList& a)
{
	if (p == 0) return _append_mv(a);
	LISTVERIFY(p,"_insbefore_p");
	if (p==h) return _prepend_mv(a);
	_PListNode* t = a.h;
	a.h = 0;
	if (h == 0) {
		h = t;
		return Lix(h);
	}
	else if (t != 0) {
		_PListNode* l = ((_PListNode*)p)->b;
		t->b->f = (_PListNode*)p;
		((_PListNode*)p)->b = t->b;
		t->b = l;
		l->f = t;
	}
	return Lix(t);
}
#endif

/**********************************************************/

Lix _PList::_insafter_mv(Lix p, _PList& a, Lix ni, Lix nf )
{
	if (p==0) return _prepend_mv(a,ni,nf);
	LISTVERIFY(p,"_insafter_mv");
	if (!h) return _append_mv(a);

	_PListNode* t1 = (_PListNode*)(ni?ni:a.h);
	_PListNode* t2 = (_PListNode*)(nf?nf: a.h?a.h->b:0);
#ifdef LIST_PTRVERIFY
	XLISTVERIFY(a,(Lix)t1,"_insafter_mv");
	XLISTVERIFY(a,(Lix)t2,"_insafter_mv");
	if (t1 && t2) assert(a.index(t1)<=a.index(t2));
#endif
	if (t1->b == t2) a.h = 0;
	else {
		t1->b->f = t2->f;
		t2->f->b = t1->b;
		if (a.h==t1) a.h=t2->f;
	}

	if (t1 && t2) {
		_PListNode* l = (_PListNode*)p;
		t1->b = l;
		t2->f = l->f;
		l->f->b=t2;
		l->f=t1;
	}

	return Lix(t2);
}

#ifdef ORI
Lix _PList::_insafter_mv(Lix p, _PList& a)
{
	if (p==0) return _prepend_mv(a);
	LISTVERIFY(p,"_insafter_p");
	_PListNode* t = a.h;
	a.h = 0;
	if (h == 0) {
		h = t;
		return Lix(h->b);
	}
	else if (t != 0) {
		Lix tb = t->b;
		((_PListNode*)p)->f->b = t->b;
		t->b->f = ((_PListNode*)p)->f;
		t->b = (_PListNode*)p;
		((_PListNode*)p)->f = t;
		return tb;
	}
	return 0;  // $$$ deberia devolver el ultimo insertado
}
#endif

/**********************************************************/

Lix _PList::_del(Lix p, BOOL cdata, INT dir)
{
	Lix q= __del(p,dir);
	if (cdata) __deleteNode_d((_PListNode *)p);
	else __deleteNode_p((_PListNode *)p);
	return q;
}

/**********************************************************/

VOID _PList::_delnext(Lix p, BOOL cdata)
{
	if (!p) { _delfirst(cdata); return; }
	p=next(p);
	if (!p) { LISTERROR("_delnext","empty tail"); }
	else _del(p,cdata);
}

/**********************************************************/

VOID _PList::_delprev(Lix p, BOOL cdata)
{
	if (!p) { _dellast(cdata); return; }
	p=prev(p);
	if (!p) { LISTERROR("_delprev","empty front"); }
	else _del(p,cdata);
}

/**********************************************************/

Lix _PList::_delfirst(BOOL cdata)
{
	if (!h) {
		LISTERROR("_delfirst","empty list");
		return 0;
	}
	_del(first(),cdata);
	return h->f;
}

/**********************************************************/

Lix _PList::_dellast(BOOL cdata)
{
	if (!h) {
		LISTERROR("_dellast","empty list");
		return 0;
	}
	_del(last(),cdata);
	return h->b;
}

/**********************************************************/

#ifdef LIST_PTRVERIFY
Lix _PList::next(Lix p) const { if (p == 0 || p == h->b) return 0; LISTVERIFY(p,"next"); return Lix(((_PListNode*)p)->f); }
Lix _PList::prev(Lix p) const { if (p == 0 || p == h) return 0; LISTVERIFY(p,"prev"); return Lix(((_PListNode*)p)->b); }
const VOID *_PList::_item(Lix p) const { if (p == 0) { LISTNONULL(p,"_item"); return 0; }; LISTVERIFY(p,"_item"); return ((_PListNode*)p)->dp; }
const VOID *_PList::_item_first(VOID) const { if (h == 0) { LISTERROR("_item_first","empty list"); return 0; }; return h->dp; }
const VOID *_PList::_item_last(VOID) const { if (h == 0) { LISTERROR("_item_last","empty list"); return 0; }; return h->b->dp; }
#endif

/**********************************************************/
