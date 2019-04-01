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
Copyright: 1996 - Grupo de Voz (DET) ETSII/IT-Bilbao

Nombre fuente................ STRL.CPP
Nombre paquete............... -
Lenguaje fuente.............. C++
Estado....................... Completado
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Borja Etxebarria

Version  dd/mm/aa  Autor     Comentario
-------  --------  --------  ----------
1.2.5    30/08/98  Borja     split en varios modulos strl_?.cpp

======================== Contenido ========================
Ver strl.cpp para doc.
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include "strl.hpp"
#include "uti.h"
#include "wrkbuff.h"

/**********************************************************/

INT _strl_cmp(const String *s1, const String *s2);
INT _strl_rcmp(const String *s1, const String *s2);

/**********************************************************/

int KVStrList::toi( const String &k, const String &v )
{
	int n,i;
	int val;
	i = sscanf(v,"%d%n",&val,&n);
	if ((i!=1)||(size_t)n!=v.length())
		die_beep("KVStrList: value for key (%s) is not int (%s)",
				(const char *)k,(const char *)v);
	return val;
}

/**********************************************************/

long KVStrList::tol( const String &k, const String &v )
{
	int n,i;
	long val;
	i = sscanf(v,"%ld%n",&val,&n);
	if ((i!=1)||(size_t)n!=v.length())
		die_beep("KVStrList: value for key (%s) is not long (%s)",
				(const char *)k,(const char *)v);
	return val;
}

/**********************************************************/

float KVStrList::tof( const String &k, const String &v)
{
	int n,i;
	float val;
	i = sscanf(v,"%f%n",&val,&n);
	if ((i!=1)||(size_t)n!=v.length())
		die_beep("KVStrList: value for key (%s) is not float (%s)",
				(const char *)k,(const char *)v);
	return val;
}

/**********************************************************/

double KVStrList::tod( const String &k, const String &v )
{
	int n,i;
	double val;
	i = sscanf(v,"%lf%n",&val,&n);
	if ((i!=1)||(size_t)n!=v.length())
		die_beep("KVStrList: value for key (%s) is not double (%s)",
				(const char *)k,(const char *)v);
	return val;
}

/**********************************************************/

#define INVALID_BOOL_IS_TRUE
BOOL KVStrList::tob( const String &k, const String &v)
{
	int x=str2bool((const char*)v,-1);
	if (x>0) return TRUE;
	if (!x) return FALSE;
#ifdef INVALID_BOOL_IS_TRUE
	(void)k;  // quitar warning "arg not used
	return TRUE;  // si no tiene valor, lo consideramos TRUE !!!!
#else
	die_beep("KVStrList: value for key (%s) is not boolean (%s)",
			(const char *)k,(const char *)v);
	return FALSE;
#endif
}

/**********************************************************/

const String& KVStrList::val( const String& key ) const
{
	Lix p=seek(key);
	cdie_beep(!p, "List val(%s): non existant key",(const char*)key);
	return itemval(p);
}

/**********************************************************/

int KVStrList::ival( const String &k, int def ) const
{
	Lix p = seek(k);
	return (p ? toi(k,itemval(p)) : def);
}

/**********************************************************/

long KVStrList::lval( const String &k, long def ) const
{
	Lix p = seek(k);
	return (p ? tol(k,itemval(p)) : def);
}

/**********************************************************/

float KVStrList::fval( const String &k, float def ) const
{
	Lix p = seek(k);
	return (p ? tof(k,itemval(p)) : def);
}

/**********************************************************/

double KVStrList::dval( const String &k, double def ) const
{
	Lix p = seek(k);
	return (p ? tod(k,itemval(p)) : def);
}

/**********************************************************/

BOOL KVStrList::bval( const String &k, BOOL def ) const
{
	Lix p = seek(k);
	return (p ? tob(k,itemval(p)) : def);
}

/**********************************************************/

const char *KVStrList::val( const String &k, const char *vdef ) const
{
	Lix p=seek(k);
	if (!p) return vdef;
	else return itemval(p);
}

/**********************************************************/

Lix KVStrList::addi( const String &k, int val )
{
	WRKBUFF(128);
	sprintf(_wrkbuff,"%d",val);
	Lix p=add(k,_wrkbuff);
	WRKBUFF_FREE();
	return p;
}

/**********************************************************/

Lix KVStrList::addui( const String &k, unsigned int val )
{
	WRKBUFF(128);
	sprintf(_wrkbuff,"%u",val);
	Lix p=add(k,_wrkbuff);
	WRKBUFF_FREE();
	return p;
}

/**********************************************************/

Lix KVStrList::addl( const String &k, long val )
{
	WRKBUFF(128);
	sprintf(_wrkbuff,"%ld",val);
	Lix p=add(k,_wrkbuff);
	WRKBUFF_FREE();
	return p;
}

/**********************************************************/

Lix KVStrList::addf( const String &k, float val )
{
	WRKBUFF(128);
	sprintf(_wrkbuff,"%g",(double)val);
	Lix p=add(k,_wrkbuff);
	WRKBUFF_FREE();
  return p;
}

/**********************************************************/

Lix KVStrList::addd( const String &k, double val )
{
	WRKBUFF(128);
	sprintf(_wrkbuff,"%g",val);
	Lix p=add(k,_wrkbuff);
	WRKBUFF_FREE();
	return p;
}

/**********************************************************/

Lix KVStrList::addb( const String &k, BOOL val )
{
	return add(k,bool2str(val));
}

/**********************************************************/

VOID KVStrList::add_prefix( const String &prefix )
{
	Lix p;

	for (p=first(); p!=0; p=next(p))
		_itemkey(p)=prefix+_itemkey(p);
}

/**********************************************************/

VOID KVStrList::remove_prefix( const String &prefix )
{
	Lix p;
	size_t l=strlen(prefix);

	for (p=first(); p!=0; p=next(p)) {
		if (!strncmp(_itemkey(p),prefix,l))
			_itemkey(p) = ((const char *)_itemkey(p))+l;
	}
}

/**********************************************************/

std::ostream& operator << (std::ostream &st, const KVStrList &l)
{
	for (Lix p=l.first(); p!=0; p=l.next(p))
		st<< "\"" << l.itemkey(p) << "\"" << "="
				<<"\"" << l.itemval(p) << "\"" <<std::endl;
	return st;
}

/**********************************************************/

VOID KVStrList::add_s( const char *words, const char* sep )
{
	static const char *fseparator=" \t\n";

	if (!words) return;

	size_t lk, lv, lk2;
	const char *k;
	const char *v;

	while (words) {
		words += strspn(words,fseparator);
		if (!(*words)) break;
		k=words; lk=strcspn(k,sep); lk2=strcspn(k,fseparator);
		if (lk2<lk) {
			lk=lk2;
			words+=lk;
			if (lk) add(String(k,lk),"");
		}
		else {
			words+=lk;
			if (lk) {
				BOOL cute=FALSE;
				words += strspn(words,sep);
				v=words;
				if (*v=='"') {
					v++;
					words++;
					cute=TRUE;
          lv=strcspn(v,"\"");
        }
        else
        	lv=strcspn(v,fseparator);
				add(String(k,lk),String(v,lv));
				words+=lv;
				if (cute && (*words)) words++;
			}
		}
	}
}

/**********************************************************/

VOID KVStrList::add_sf( const char *fwords, const char* sep, ... )
{
	va_list argptr;
	va_start(argptr, sep);
	add_s(fwords,sep,argptr);
	va_end(argptr);
}

/**********************************************************/

VOID KVStrList::add_s( const char *fwords, const char* sep, va_list va )
{
	WRKBUFF(2048);

	if (!fwords) return;
	vsprintf(_wrkbuff, fwords, va);
	add_s(_wrkbuff,sep);
	WRKBUFF_FREE();
}

/**********************************************************/

VOID KVStrList::add_s( const char *wordarray[], const char* sep )
{
	const char *k;
	const char *v;
	size_t lk;

	if (!wordarray) return;
	while (*wordarray) {
		if (sep && (*sep)) {
			k= *wordarray; lk=strcspn(k,sep);
			if (lk) {
				v = *wordarray+lk+ strspn(*wordarray+lk,sep);
				add(String(k,lk),v);
			}
			else add(k,"");
			wordarray++;
		}
		else {  // separator="" or null
			if (*(wordarray+1)) {
				add(*wordarray, *(wordarray+1));
				wordarray+=2;
			}
			else {
				add(*wordarray, "");
				wordarray++;
			}
		}
	}
}

/**********************************************************/

VOID KVStrList::sort( BOOL reverse )
{
	if (reverse) sortf(_strl_rcmp);
	else sortf(_strl_cmp);
}

/**********************************************************/

BOOL KVStrList::OK ( VOID ) const
{
	if (!(l.OK())) return FALSE;
	for (Lix p=first(); p!=0; p=next(p))
			if (!itemkey(p).OK() || !itemval(p).OK()) return FALSE;
	return TRUE;
}

/**********************************************************/
