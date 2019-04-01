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
1.1.1    2012  	   Inaki     corregir warnings deprecated
1.1.0    07/05/99  Borja     modif ??_mv()
1.0.1    30/08/98  Borja     split varios modulos listt_?.cpp

======================== Contenido ========================
ver doc. en listt.cpp
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include "listt_i.hpp"

/**********************************************************/

VOID _PList::__exchange_p( _PListNode *n1, _PListNode *n2 )
{
	VOID *tmp;
	__SWAP__( n1->dp, n2->dp, tmp);
}

/**********************************************************/

VOID _PList::_exchange_lp( Lix p1, Lix p2 )
{
	if ((p1==0) || (p2==0)) {
		LISTERROR("_exchange_lp","null Lix");
		return;
	}
	else if (p1!=p2) {
		LISTVERIFY(p1,"_exchange_lp");
		LISTVERIFY(p2,"_exchange_lp");
		_PListNode* b1 = (_PListNode*) p1;
		_PListNode* b2 = (_PListNode*) p2;
		_PListNode* t;
		if (b1->f == b2) {     // b1 -> b2 ??
			if (b2->f == b1) {   // b2 <-> b1
				__SWAP__(b1->f,b1->b,t);
				__SWAP__(b2->f,b2->b,t);
			}
			else {     // b1 -> b2
				b2->f->b=b1;
				b1->b->f=b2;
				b1->f = b2->f;
				b2->b = b1->b;
				b1->b = b2;
				b2->f = b1;
			}
		}
		else if (b2->f == b1) {   // b2 -> b1
			b1->f->b=b2;
			b2->b->f=b1;
			b2->f = b1->f;
			b1->b = b2->b;
			b2->b = b1;
			b1->f = b2;
		}
		else {   // unrelated
			b1->b->f=b2;
			b1->f->b=b2;
			b2->b->f=b1;
			b2->f->b=b1;
			__SWAP__(b1->f,b2->f,t);
			__SWAP__(b1->b,b2->b,t);
		}
		if (h == b1) h = b2;
		else if (h == b2) h = b1;
	}
}

/**********************************************************/

VOID _PList::_exchange_p( Lix p1, Lix p2 )
{
	if ((p1==0) || (p2==0)) {
		LISTERROR("_exchange_p","null Lix");
		return;
	}
	else if (p1!=p2) {
		LISTVERIFY(p1,"_exchange_p");
		LISTVERIFY(p2,"_exchange_p");
		__exchange_p((_PListNode*)p1,(_PListNode*)p2);
	}
}

/**********************************************************/

Lix _PList::_movebefore_lp(Lix who, Lix where)
{
	if ((who==0) || (where==0)) {
		LISTERROR("_movebefore_lp","null Lix");
		return 0;
	}
	if (who==where) return who;

	__del(who,1);
	return __insbefore(where,(_PListNode *)who);
}

/**********************************************************/

Lix _PList::_moveafter_lp(Lix who, Lix where)
{
	if ((who==0) || (where==0)) {
		LISTERROR("_moveafter_lp","null Lix");
		return 0;
	}
	if (who==where) return who;

	__del(who,1);
	return __insafter(where,(_PListNode *)who);
}

/**********************************************************/

VOID _PList::_sort( INT(*comparefunc)(const VOID*item1, const VOID*item2), BOOL lpmove )
{
	_PListNode *i, *j, *m;
	if (!h) return;

	i=h;
	do {
		m=i;
		j=i->f;
		while (j!=h) { if (comparefunc(j->dp,m->dp)<0) m=j; j=j->f; }
		j = i->f;
		if (m!=i) {
			if (lpmove) _exchange_lp(i,m);
			else _exchange_p(i,m);
		}
		i = j;
	} while (i!=h);

}

/**********************************************************/

Lix _PList::_insbefore_d(LIINT i, const VOID* item)
{
	return _insbefore_d(LIX_SAFE(i,(char *)"_insbefore_d",TRUE),item);
}

/**********************************************************/

Lix _PList::_insbefore_p(LIINT i, VOID* item)
{
	return _insbefore_p(LIX_SAFE(i,(char *)"_insbefore_p",TRUE),item);
}

/**********************************************************/

Lix _PList::_insbefore_d(LIINT i, const _PList& a)
{
	return _insbefore_d(LIX_SAFE(i,(char *)"_insbefore_d",TRUE),a);
}

/**********************************************************/

Lix _PList::_insbefore_p(LIINT i, _PList& a)
{
	return _insbefore_p(LIX_SAFE(i,(char *)"_insbefore_p",TRUE),a);
}

/**********************************************************/

Lix _PList::_insbefore_mv(LIINT i, _PList& a)
{
	return _insbefore_mv(LIX_SAFE(i,(char *)"_insbefore_mv",TRUE),a);
}

/**********************************************************/

Lix _PList::_insafter_d(LIINT i, const VOID* item)
{
	return _insafter_d(LIX_SAFE(i,(char *)"_insafter_d",TRUE),item);
}

/**********************************************************/

Lix _PList::_insafter_p(LIINT i, VOID* item)
{
	return _insafter_p(LIX_SAFE(i,(char *)"_insafter_p",TRUE),item);
}

/**********************************************************/

Lix _PList::_insafter_d(LIINT i, const _PList& a)
{
	return _insafter_d(LIX_SAFE(i,(char *)"_insafter_d",TRUE),a);
}

/**********************************************************/

Lix _PList::_insafter_p(LIINT i, _PList& a)
{
	return _insafter_p(LIX_SAFE(i,(char *)"_insafter_p",TRUE),a);
}

/**********************************************************/

Lix _PList::_insafter_mv(LIINT i, _PList& a)
{
	return _insafter_mv(LIX_SAFE(i,(char *)"_insafter_mv",TRUE),a);
}

/**********************************************************/

VOID _PList::_del(LIINT i, BOOL cdata)
{
	_del(LIX_SAFE(i,(char *)"_del",FALSE),cdata);
}

/**********************************************************/

VOID _PList::_delprev(LIINT i, BOOL cdata)
{
	_delprev(LIX_SAFE(i,(char *)"_delprev",FALSE),cdata);
}

/**********************************************************/

VOID _PList::_delnext(LIINT i, BOOL cdata)
{
	_delnext(LIX_SAFE(i,(char *)"_delnext",FALSE),cdata);
}

/**********************************************************/

const VOID* _PList::_item(LIINT i) const
{
	return ((_PListNode*)LIX_SAFE(i,(char *)"_item",FALSE))->dp;
}

/**********************************************************/

VOID _PList::_exchange_lp( LIINT i1, LIINT i2 )
{
	_exchange_lp(LIX_SAFE(i1,(char *)"_exchange_lp",FALSE),LIX_SAFE(i2,(char *)"_exchange_lp",FALSE));
}

/**********************************************************/

VOID _PList::_exchange_p( LIINT i1, LIINT i2 )
{
	_exchange_p(LIX_SAFE(i1,(char *)"_exchange_p",FALSE),LIX_SAFE(i2,(char *)"_exchange_p",FALSE));
}

/**********************************************************/

Lix _PList::_moveafter_lp( LIINT i1, LIINT i2 )
{
	return _moveafter_lp(LIX_SAFE(i1,(char *)"_moveafter_lp",FALSE),LIX_SAFE(i2,(char *)"_moveafter_lp",FALSE));
}

/**********************************************************/

Lix _PList::_movebefore_lp( LIINT i1, LIINT i2 )
{
	return _movebefore_lp(LIX_SAFE(i1,(char *)"_movebefore_lp",FALSE),LIX_SAFE(i2,(char *)"_movebefore_lp",FALSE));
}

/**********************************************************/
