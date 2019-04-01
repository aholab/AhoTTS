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
#ifndef __STRING_HPP__
#define __STRING_HPP__

/**********************************************************/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/*
Copyright: Free Software Foundation

Nombre fuente................ STRING.HPP
Nombre paquete............... -
Lenguaje fuente.............. C++
Estado....................... Completado
Dependencia Hard/OS.......... __CC_UNIX__
Codigo condicional........... -

Codificacion................. -

Version  dd/mm/aa  Autor     Comentario
-------  --------  --------  ----------
1.0.0    22/04/97  -         Codificacion inicial.

======================== Contenido ========================
envoltorio para la clase String. Si estamos usando el GCC,
se usa la de libg++, si no, una version portada de la misma.
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include "arch.h"

/**********************************************************/

// $$$$ he tenido que quitar la deteccion de CC_GNUC!!!!
#ifdef __CC_GNUC__NOT_DEFINED
//#include <String.h>
#include <string>
#else   // #include "gstring.hpp"

////////////////////////////////////////////////////////////
// This may look like C code, but it is really -*- C++ -*-
/*
  Cambiado por Deltatel Ingenieria Telematica SL el 10/9/96:
    - Se ha quitado el soporte de regex
*/

/*
Copyright (C) 1988 Free Software Foundation
    written by Doug Lea (dl@rocky.oswego.edu)

This file is part of the GNU C++ Library.  This library is free
software; you can redistribute it and/or modify it under the terms of
the GNU Library General Public License as published by the Free
Software Foundation; either version 2 of the License, or (at your
option) any later version.  This library is distributed in the hope
that it will be useful, but WITHOUT ANY WARRANTY; without even the
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the GNU Library General Public License for more details.
You should have received a copy of the GNU Library General Public
License along with this library; if not, write to the Free Software
Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
*/


#ifndef __GSTRING_HPP__
#ifdef __GNUG__
//#pragma interface
#endif
#define __GSTRING_HPP__ 1

#include <iostream.h>

struct StrRep                     // internal String representations
{
  unsigned short    len;         // string length
  unsigned short    sz;          // allocated space
  char              s[1];        // the string starts here
                                 // (at least 1 char for trailing null)
                                 // allocated & expanded via non-public fcts
};

// primitive ops on StrReps -- nearly all String fns go through these.

StrRep*     Salloc(StrRep*, const char*, int, int);
StrRep*     Scopy(StrRep*, const StrRep*);
StrRep*     Scat(StrRep*, const char*, int, const char*, int);
StrRep*     Scat(StrRep*, const char*, int,const char*,int, const char*,int);
StrRep*     Sprepend(StrRep*, const char*, int);
StrRep*     Sreverse(const StrRep*, StrRep*);
StrRep*     Supcase(const StrRep*, StrRep*);
StrRep*     Sdowncase(const StrRep*, StrRep*);
StrRep*     Scapitalize(const StrRep*, StrRep*);

// These classes need to be defined in the order given

class String;
class SubString;

class SubString
{
  friend class      String;
protected:

  String&           S;        // The String I'm a substring of
  unsigned short    pos;      // starting position in S's rep
  unsigned short    len;      // length of substring

  void              assign(const StrRep*, const char*, int = -1);
                    SubString(String& x, int p, int l);
                    SubString(const SubString& x);

public:

// Note there are no public constructors. SubStrings are always
// created via String operations

                   ~SubString();

  SubString&        operator =  (const String&     y);
  SubString&        operator =  (const SubString&  y);
  SubString&        operator =  (const char* t);
  SubString&        operator =  (char        c);

// return 1 if target appears anywhere in SubString; else 0

  int               contains(char        c) const;
  int               contains(const String&     y) const;
  int               contains(const SubString&  y) const;
  int               contains(const char* t) const;

// return 1 if target matches entire SubString

// IO

  friend ostream&   operator<<(ostream& s, const SubString& x);

// status

  unsigned int      length() const;
  int               empty() const;
  const char*       chars() const;

  int               OK() const;

};


class String
{
  friend class      SubString;

protected:
  StrRep*           rep;   // Strings are pointers to their representations

// some helper functions

  int               search(int, int, const char*, int = -1) const;
  int               search(int, int, char) const;
  int               match(int, int, int, const char*, int = -1) const;
  int               _gsub(const char*, int, const char* ,int);
  SubString         _substr(int, int);

public:

// constructors & assignment

                    String();
                    String(const String& x);
                    String(const SubString&  x);
                    String(const char* t);
                    String(const char* t, int len);
                    String(char c);

                    ~String();

  String&           operator =  (const String&     y);
  String&           operator =  (const char* y);
  String&           operator =  (char        c);
  String&           operator =  (const SubString&  y);

// concatenation

  String&           operator += (const String&     y);
  String&           operator += (const SubString&  y);
  String&           operator += (const char* t);
  String&           operator += (char        c);

  void              prepend(const String&     y);
  void              prepend(const SubString&  y);
  void              prepend(const char* t);
  void              prepend(char        c);


// procedural versions:
// concatenate first 2 args, store result in last arg

	friend void     cat(const String&, const String&, String&);
	friend void     cat(const String&, const SubString&, String&);
	friend void     cat(const String&, const char*, String&);
	friend void     cat(const String&, char, String&);

	friend void     cat(const SubString&, const String&, String&);
	friend void     cat(const SubString&, const SubString&, String&);
	friend void     cat(const SubString&, const char*, String&);
	friend void     cat(const SubString&, char, String&);

	friend void     cat(const char*, const String&, String&);
	friend void     cat(const char*, const SubString&, String&);
	friend void     cat(const char*, const char*, String&);
  friend void     cat(const char*, char, String&);

// double concatenation, by request. (yes, there are too many versions,
// but if one is supported, then the others should be too...)
// Concatenate first 3 args, store in last arg

	friend void     cat(const String&,const String&, const String&,String&);
	friend void     cat(const String&,const String&,const SubString&,String&);
	friend void     cat(const String&,const String&, const char*, String&);
	friend void     cat(const String&,const String&, char, String&);
	friend void     cat(const String&,const SubString&,const String&,String&);
	friend void     cat(const String&,const SubString&,const SubString&,String&);
	friend void     cat(const String&,const SubString&, const char*, String&);
	friend void     cat(const String&,const SubString&, char, String&);
	friend void     cat(const String&,const char*, const String&,    String&);
	friend void     cat(const String&,const char*, const SubString&, String&);
	friend void     cat(const String&,const char*, const char*, String&);
	friend void     cat(const String&,const char*, char, String&);

	friend void     cat(const char*, const String&, const String&,String&);
	friend void     cat(const char*,const String&,const SubString&,String&);
	friend void     cat(const char*,const String&, const char*, String&);
	friend void     cat(const char*,const String&, char, String&);
	friend void     cat(const char*,const SubString&,const String&,String&);
	friend void     cat(const char*,const SubString&,const SubString&,String&);
	friend void     cat(const char*,const SubString&, const char*, String&);
	friend void     cat(const char*,const SubString&, char, String&);
	friend void     cat(const char*,const char*, const String&,    String&);
	friend void     cat(const char*,const char*, const SubString&, String&);
	friend void     cat(const char*,const char*, const char*, String&);
	friend void     cat(const char*,const char*, char, String&);


// searching & matching

// return position of target in string or -1 for failure

  int               index(char        c, int startpos = 0) const;
  int               index(const String&     y, int startpos = 0) const;
  int               index(const SubString&  y, int startpos = 0) const;
  int               index(const char* t, int startpos = 0) const;

// return 1 if target appears anyhere in String; else 0

  int               contains(char        c) const;
  int               contains(const String&     y) const;
  int               contains(const SubString&  y) const;
  int               contains(const char* t) const;

// return 1 if target appears anywhere after position pos
// (or before, if pos is negative) in String; else 0

  int               contains(char        c, int pos) const;
  int               contains(const String&     y, int pos) const;
  int               contains(const SubString&  y, int pos) const;
  int               contains(const char* t, int pos) const;

// return 1 if target appears at position pos in String; else 0

  int               matches(char        c, int pos = 0) const;
  int               matches(const String&     y, int pos = 0) const;
  int               matches(const SubString&  y, int pos = 0) const;
  int               matches(const char* t, int pos = 0) const;

//  return number of occurences of target in String

  int               freq(char        c) const;
  int               freq(const String&     y) const;
  int               freq(const SubString&  y) const;
  int               freq(const char* t) const;

// SubString extraction

// Note that you can't take a substring of a const String, since
// this leaves open the possiblility of indirectly modifying the
// String through the SubString

  SubString         at(int         pos, int len);
  SubString         operator () (int         pos, int len); // synonym for at

  SubString         at(const String&     x, int startpos = 0);
  SubString         at(const SubString&  x, int startpos = 0);
  SubString         at(const char* t, int startpos = 0);
  SubString         at(char        c, int startpos = 0);

  SubString         before(int          pos);
  SubString         before(const String&      x, int startpos = 0);
  SubString         before(const SubString&   x, int startpos = 0);
  SubString         before(const char*  t, int startpos = 0);
  SubString         before(char         c, int startpos = 0);

  SubString         through(int          pos);
  SubString         through(const String&      x, int startpos = 0);
  SubString         through(const SubString&   x, int startpos = 0);
  SubString         through(const char*  t, int startpos = 0);
  SubString         through(char         c, int startpos = 0);

  SubString         from(int          pos);
  SubString         from(const String&      x, int startpos = 0);
  SubString         from(const SubString&   x, int startpos = 0);
  SubString         from(const char*  t, int startpos = 0);
  SubString         from(char         c, int startpos = 0);

  SubString         after(int         pos);
  SubString         after(const String&     x, int startpos = 0);
  SubString         after(const SubString&  x, int startpos = 0);
  SubString         after(const char* t, int startpos = 0);
  SubString         after(char        c, int startpos = 0);


// deletion

// delete len chars starting at pos
  void              del(int         pos, int len);

// delete the first occurrence of target after startpos

  void              del(const String&     y, int startpos = 0);
  void              del(const SubString&  y, int startpos = 0);
  void              del(const char* t, int startpos = 0);
  void              del(char        c, int startpos = 0);

// global substitution: substitute all occurrences of pat with repl

  int               gsub(const String&     pat, const String&     repl);
  int               gsub(const SubString&  pat, const String&     repl);
  int               gsub(const char* pat, const String&     repl);
  int               gsub(const char* pat, const char* repl);

// friends & utilities

// split string into array res at separators; return number of elements

  friend int        split(const String& x, String res[], int maxn,
                          const String& sep);

  friend String     common_prefix(const String& x, const String& y,
                                  int startpos = 0);
  friend String     common_suffix(const String& x, const String& y,
                                  int startpos = -1);
  friend String     replicate(char        c, int n);
  friend String     replicate(const String&     y, int n);
  friend String     join(String src[], int n, const String& sep);

// simple builtin transformations

	friend String     reverse(const String& x);
	friend String     upcase(const String& x);
	friend String     downcase(const String& x);
	friend String     capitalize(const String& x);

// in-place versions of above

	void              reverse();
	void              upcase();
	void              downcase();
	void              capitalize();

// element extraction

	char&             operator [] (int i);
	const char&       operator [] (int i) const;
  char              elem(int i) const;
  char              firstchar() const;
  char              lastchar() const;

// conversion

                    operator const char*() const;
  const char*       chars() const;


// IO

  friend ostream&   operator<<(ostream& s, const String& x);
  friend ostream&   operator<<(ostream& s, const SubString& x);
  friend istream&   operator>>(istream& s, String& x);

  friend int        readline(istream& s, String& x,
                             char terminator = '\n',
                             int discard_terminator = 1);

// status

  unsigned int      length() const;
  int               empty() const;

// preallocate some space for String
  void              alloc(int newsize);

// report current allocation (not length!)

  int               allocation() const;


  void     error(const char* msg) const;

  int               OK() const;
};

typedef String StrTmp; // for backward compatibility

// other externs

int        compare(const String&    x, const String&     y);
int        compare(const String&    x, const SubString&  y);
int        compare(const String&    x, const char* y);
int        compare(const SubString& x, const String&     y);
int        compare(const SubString& x, const SubString&  y);
int        compare(const SubString& x, const char* y);
int        fcompare(const String&   x, const String&     y); // ignore case

extern StrRep  _nilStrRep;
extern String _nilString;

// status reports, needed before defining other things

inline unsigned int String::length() const {  return rep->len; }
inline int         String::empty() const { return rep->len == 0; }
inline const char* String::chars() const { return &(rep->s[0]); }
inline int         String::allocation() const { return rep->sz; }

inline unsigned int SubString::length() const { return len; }
inline int         SubString::empty() const { return len == 0; }
inline const char* SubString::chars() const { return &(S.rep->s[pos]); }


// constructors

inline String::String()
  : rep(&_nilStrRep) {}
inline String::String(const String& x)
  : rep(Scopy(0, x.rep)) {}
inline String::String(const char* t)
  : rep(Salloc(0, t, -1, -1)) {}
inline String::String(const char* t, int tlen)
  : rep(Salloc(0, t, tlen, tlen)) {}
inline String::String(const SubString& y)
  : rep(Salloc(0, y.chars(), y.length(), y.length())) {}
inline String::String(char c)
  : rep(Salloc(0, &c, 1, 1)) {}

inline String::~String() { if (rep != &_nilStrRep) delete rep; }

inline SubString::SubString(const SubString& x)
  :S(x.S), pos(x.pos), len(x.len) {}
inline SubString::SubString(String& x, int first, int l)
  :S(x), pos(first), len(l) {}

inline SubString::~SubString() {}

// assignment

inline String& String::operator =  (const String& y)
{
  rep = Scopy(rep, y.rep);
  return *this;
}

inline String& String::operator=(const char* t)
{
  rep = Salloc(rep, t, -1, -1);
  return *this;
}

inline String& String::operator=(const SubString&  y)
{
  rep = Salloc(rep, y.chars(), y.length(), y.length());
  return *this;
}

inline String& String::operator=(char c)
{
  rep = Salloc(rep, &c, 1, 1);
  return *this;
}


inline SubString& SubString::operator = (const char* ys)
{
  assign(0, ys);
  return *this;
}

inline SubString& SubString::operator = (char ch)
{
  assign(0, &ch, 1);
  return *this;
}

inline SubString& SubString::operator = (const String& y)
{
  assign(y.rep, y.chars(), y.length());
  return *this;
}

inline SubString& SubString::operator = (const SubString& y)
{
  assign(y.S.rep, y.chars(), y.length());
  return *this;
}

// Zillions of cats...

inline void cat(const String& x, const String& y, String& r)
{
  r.rep = Scat(r.rep, x.chars(), x.length(), y.chars(), y.length());
}

inline void cat(const String& x, const SubString& y, String& r)
{
  r.rep = Scat(r.rep, x.chars(), x.length(), y.chars(), y.length());
}

inline void cat(const String& x, const char* y, String& r)
{
  r.rep = Scat(r.rep, x.chars(), x.length(), y, -1);
}

inline void cat(const String& x, char y, String& r)
{
  r.rep = Scat(r.rep, x.chars(), x.length(), &y, 1);
}

inline void cat(const SubString& x, const String& y, String& r)
{
  r.rep = Scat(r.rep, x.chars(), x.length(), y.chars(), y.length());
}

inline void cat(const SubString& x, const SubString& y, String& r)
{
  r.rep = Scat(r.rep, x.chars(), x.length(), y.chars(), y.length());
}

inline void cat(const SubString& x, const char* y, String& r)
{
  r.rep = Scat(r.rep, x.chars(), x.length(), y, -1);
}

inline void cat(const SubString& x, char y, String& r)
{
  r.rep = Scat(r.rep, x.chars(), x.length(), &y, 1);
}

inline void cat(const char* x, const String& y, String& r)
{
  r.rep = Scat(r.rep, x, -1, y.chars(), y.length());
}

inline void cat(const char* x, const SubString& y, String& r)
{
  r.rep = Scat(r.rep, x, -1, y.chars(), y.length());
}

inline void cat(const char* x, const char* y, String& r)
{
  r.rep = Scat(r.rep, x, -1, y, -1);
}

inline void cat(const char* x, char y, String& r)
{
  r.rep = Scat(r.rep, x, -1, &y, 1);
}

inline void cat(const String& a, const String& x, const String& y, String& r)
{
  r.rep = Scat(r.rep, a.chars(), a.length(), x.chars(), x.length(), y.chars(), y.length());
}

inline void cat(const String& a, const String& x, const SubString& y, String& r)
{
  r.rep = Scat(r.rep, a.chars(), a.length(), x.chars(), x.length(), y.chars(), y.length());
}

inline void cat(const String& a, const String& x, const char* y, String& r)
{
  r.rep = Scat(r.rep, a.chars(), a.length(), x.chars(), x.length(), y, -1);
}

inline void cat(const String& a, const String& x, char y, String& r)
{
  r.rep = Scat(r.rep, a.chars(), a.length(), x.chars(), x.length(), &y, 1);
}

inline void cat(const String& a, const SubString& x, const String& y, String& r)
{
  r.rep = Scat(r.rep, a.chars(), a.length(), x.chars(), x.length(), y.chars(), y.length());
}

inline void cat(const String& a, const SubString& x, const SubString& y, String& r)
{
  r.rep = Scat(r.rep, a.chars(), a.length(), x.chars(), x.length(), y.chars(), y.length());
}

inline void cat(const String& a, const SubString& x, const char* y, String& r)
{
  r.rep = Scat(r.rep, a.chars(), a.length(), x.chars(), x.length(), y, -1);
}

inline void cat(const String& a, const SubString& x, char y, String& r)
{
  r.rep = Scat(r.rep, a.chars(), a.length(), x.chars(), x.length(), &y, 1);
}

inline void cat(const String& a, const char* x, const String& y, String& r)
{
  r.rep = Scat(r.rep, a.chars(), a.length(), x, -1, y.chars(), y.length());
}

inline void cat(const String& a, const char* x, const SubString& y, String& r)
{
  r.rep = Scat(r.rep, a.chars(), a.length(), x, -1, y.chars(), y.length());
}

inline void cat(const String& a, const char* x, const char* y, String& r)
{
  r.rep = Scat(r.rep, a.chars(), a.length(), x, -1, y, -1);
}

inline void cat(const String& a, const char* x, char y, String& r)
{
  r.rep = Scat(r.rep, a.chars(), a.length(), x, -1, &y, 1);
}


inline void cat(const char* a, const String& x, const String& y, String& r)
{
  r.rep = Scat(r.rep, a, -1, x.chars(), x.length(), y.chars(), y.length());
}

inline void cat(const char* a, const String& x, const SubString& y, String& r)
{
  r.rep = Scat(r.rep, a, -1, x.chars(), x.length(), y.chars(), y.length());
}

inline void cat(const char* a, const String& x, const char* y, String& r)
{
  r.rep = Scat(r.rep, a, -1, x.chars(), x.length(), y, -1);
}

inline void cat(const char* a, const String& x, char y, String& r)
{
  r.rep = Scat(r.rep, a, -1, x.chars(), x.length(), &y, 1);
}

inline void cat(const char* a, const SubString& x, const String& y, String& r)
{
  r.rep = Scat(r.rep, a, -1, x.chars(), x.length(), y.chars(), y.length());
}

inline void cat(const char* a, const SubString& x, const SubString& y, String& r)
{
  r.rep = Scat(r.rep, a, -1, x.chars(), x.length(), y.chars(), y.length());
}

inline void cat(const char* a, const SubString& x, const char* y, String& r)
{
  r.rep = Scat(r.rep, a, -1, x.chars(), x.length(), y, -1);
}

inline void cat(const char* a, const SubString& x, char y, String& r)
{
  r.rep = Scat(r.rep, a, -1, x.chars(), x.length(), &y, 1);
}

inline void cat(const char* a, const char* x, const String& y, String& r)
{
  r.rep = Scat(r.rep, a, -1, x, -1, y.chars(), y.length());
}

inline void cat(const char* a, const char* x, const SubString& y, String& r)
{
  r.rep = Scat(r.rep, a, -1, x, -1, y.chars(), y.length());
}

inline void cat(const char* a, const char* x, const char* y, String& r)
{
  r.rep = Scat(r.rep, a, -1, x, -1, y, -1);
}

inline void cat(const char* a, const char* x, char y, String& r)
{
  r.rep = Scat(r.rep, a, -1, x, -1, &y, 1);
}


// operator versions

inline String& String::operator +=(const String& y)
{
  cat(*this, y, *this);
  return *this;
}

inline String& String::operator +=(const SubString& y)
{
  cat(*this, y, *this);
  return *this;
}

inline String& String::operator += (const char* y)
{
  cat(*this, y, *this);
  return *this;
}

inline String& String:: operator +=(char y)
{
  cat(*this, y, *this);
  return *this;
}

// constructive concatenation

#if defined(__GNUG__) && !defined(_G_NO_NRV)

inline String operator + (const String& x, const String& y) return r;
{
  cat(x, y, r);
}

inline String operator + (const String& x, const SubString& y) return r;
{
  cat(x, y, r);
}

inline String operator + (const String& x, const char* y) return r;
{
  cat(x, y, r);
}

inline String operator + (const String& x, char y) return r;
{
  cat(x, y, r);
}

inline String operator + (const SubString& x, const String& y) return r;
{
  cat(x, y, r);
}

inline String operator + (const SubString& x, const SubString& y) return r;
{
  cat(x, y, r);
}

inline String operator + (const SubString& x, const char* y) return r;
{
  cat(x, y, r);
}

inline String operator + (const SubString& x, char y) return r;
{
  cat(x, y, r);
}

inline String operator + (const char* x, const String& y) return r;
{
  cat(x, y, r);
}

inline String operator + (const char* x, const SubString& y) return r;
{
  cat(x, y, r);
}

inline String reverse(const String& x) return r;
{
  r.rep = Sreverse(x.rep, r.rep);
}

inline String upcase(const String& x) return r;
{
  r.rep = Supcase(x.rep, r.rep);
}

inline String downcase(const String& x) return r;
{
  r.rep = Sdowncase(x.rep, r.rep);
}

inline String capitalize(const String& x) return r;
{
  r.rep = Scapitalize(x.rep, r.rep);
}

#else /* NO_NRV */

inline String operator + (const String& x, const String& y)
{
  String r;  cat(x, y, r);  return r;
}

inline String operator + (const String& x, const SubString& y)
{
  String r; cat(x, y, r); return r;
}

inline String operator + (const String& x, const char* y)
{
  String r; cat(x, y, r); return r;
}

inline String operator + (const String& x, char y)
{
  String r; cat(x, y, r); return r;
}

inline String operator + (const SubString& x, const String& y)
{
  String r; cat(x, y, r); return r;
}

inline String operator + (const SubString& x, const SubString& y)
{
  String r; cat(x, y, r); return r;
}

inline String operator + (const SubString& x, const char* y)
{
  String r; cat(x, y, r); return r;
}

inline String operator + (const SubString& x, char y)
{
  String r; cat(x, y, r); return r;
}

inline String operator + (const char* x, const String& y)
{
  String r; cat(x, y, r); return r;
}

inline String operator + (const char* x, const SubString& y)
{
  String r; cat(x, y, r); return r;
}

inline String reverse(const String& x)
{
  String r; r.rep = Sreverse(x.rep, r.rep); return r;
}

inline String upcase(const String& x)
{
  String r; r.rep = Supcase(x.rep, r.rep); return r;
}

inline String downcase(const String& x)
{
  String r; r.rep = Sdowncase(x.rep, r.rep); return r;
}

inline String capitalize(const String& x)
{
  String r; r.rep = Scapitalize(x.rep, r.rep); return r;
}

#endif

// prepend

inline void String::prepend(const String& y)
{
  rep = Sprepend(rep, y.chars(), y.length());
}

inline void String::prepend(const char* y)
{
  rep = Sprepend(rep, y, -1);
}

inline void String::prepend(char y)
{
  rep = Sprepend(rep, &y, 1);
}

inline void String::prepend(const SubString& y)
{
  rep = Sprepend(rep, y.chars(), y.length());
}

// misc transformations


inline void String::reverse()
{
  rep = Sreverse(rep, rep);
}


inline void String::upcase()
{
  rep = Supcase(rep, rep);
}


inline void String::downcase()
{
  rep = Sdowncase(rep, rep);
}


inline void String::capitalize()
{
  rep = Scapitalize(rep, rep);
}

// element extraction

inline char&  String::operator [] (int i)
{
  if (((unsigned)i) >= length()) error("invalid index");
  return rep->s[i];
}

inline const char&  String::operator [] (int i) const
{
  if (((unsigned)i) >= length()) error("invalid index");
  return rep->s[i];
}

inline char  String::elem (int i) const
{
  if (((unsigned)i) >= length()) error("invalid index");
  return rep->s[i];
}

inline char  String::firstchar() const
{
  return elem(0);
}

inline char  String::lastchar() const
{
  return elem(length() - 1);
}

// searching

inline int String::index(char c, int startpos) const
{
  return search(startpos, length(), c);
}

inline int String::index(const char* t, int startpos) const
{
  return search(startpos, length(), t);
}

inline int String::index(const String& y, int startpos) const
{
  return search(startpos, length(), y.chars(), y.length());
}

inline int String::index(const SubString& y, int startpos) const
{
  return search(startpos, length(), y.chars(), y.length());
}

inline int String::contains(char c) const
{
  return search(0, length(), c) >= 0;
}

inline int String::contains(const char* t) const
{
  return search(0, length(), t) >= 0;
}

inline int String::contains(const String& y) const
{
  return search(0, length(), y.chars(), y.length()) >= 0;
}

inline int String::contains(const SubString& y) const
{
  return search(0, length(), y.chars(), y.length()) >= 0;
}

inline int String::contains(char c, int p) const
{
  return match(p, length(), 0, &c, 1) >= 0;
}

inline int String::contains(const char* t, int p) const
{
  return match(p, length(), 0, t) >= 0;
}

inline int String::contains(const String& y, int p) const
{
  return match(p, length(), 0, y.chars(), y.length()) >= 0;
}

inline int String::contains(const SubString& y, int p) const
{
  return match(p, length(), 0, y.chars(), y.length()) >= 0;
}

inline int String::matches(const SubString& y, int p) const
{
  return match(p, length(), 1, y.chars(), y.length()) >= 0;
}

inline int String::matches(const String& y, int p) const
{
  return match(p, length(), 1, y.chars(), y.length()) >= 0;
}

inline int String::matches(const char* t, int p) const
{
  return match(p, length(), 1, t) >= 0;
}

inline int String::matches(char c, int p) const
{
  return match(p, length(), 1, &c, 1) >= 0;
}

inline int SubString::contains(const char* t) const
{
  return S.search(pos, pos+len, t) >= 0;
}

inline int SubString::contains(const String& y) const
{
  return S.search(pos, pos+len, y.chars(), y.length()) >= 0;
}

inline int SubString::contains(const SubString&  y) const
{
  return S.search(pos, pos+len, y.chars(), y.length()) >= 0;
}

inline int SubString::contains(char c) const
{
  return S.search(pos, pos+len, c) >= 0;
}

inline int String::gsub(const String& pat, const String& r)
{
  return _gsub(pat.chars(), pat.length(), r.chars(), r.length());
}

inline int String::gsub(const SubString&  pat, const String& r)
{
  return _gsub(pat.chars(), pat.length(), r.chars(), r.length());
}

inline int String::gsub(const char* pat, const String& r)
{
  return _gsub(pat, -1, r.chars(), r.length());
}

inline int String::gsub(const char* pat, const char* r)
{
  return _gsub(pat, -1, r, -1);
}



inline  ostream& operator<<(ostream& s, const String& x)
{
   s << x.chars(); return s;
}

// a zillion comparison operators

inline int operator==(const String& x, const String& y)
{
  return compare(x, y) == 0;
}

inline int operator!=(const String& x, const String& y)
{
  return compare(x, y) != 0;
}

inline int operator>(const String& x, const String& y)
{
  return compare(x, y) > 0;
}

inline int operator>=(const String& x, const String& y)
{
  return compare(x, y) >= 0;
}

inline int operator<(const String& x, const String& y)
{
  return compare(x, y) < 0;
}

inline int operator<=(const String& x, const String& y)
{
  return compare(x, y) <= 0;
}

inline int operator==(const String& x, const SubString&  y)
{
  return compare(x, y) == 0;
}

inline int operator!=(const String& x, const SubString&  y)
{
  return compare(x, y) != 0;
}

inline int operator>(const String& x, const SubString&  y)
{
  return compare(x, y) > 0;
}

inline int operator>=(const String& x, const SubString&  y)
{
  return compare(x, y) >= 0;
}

inline int operator<(const String& x, const SubString&  y)
{
  return compare(x, y) < 0;
}

inline int operator<=(const String& x, const SubString&  y)
{
  return compare(x, y) <= 0;
}

inline int operator==(const String& x, const char* t)
{
  return compare(x, t) == 0;
}

inline int operator!=(const String& x, const char* t)
{
  return compare(x, t) != 0;
}

inline int operator>(const String& x, const char* t)
{
  return compare(x, t) > 0;
}

inline int operator>=(const String& x, const char* t)
{
  return compare(x, t) >= 0;
}

inline int operator<(const String& x, const char* t)
{
  return compare(x, t) < 0;
}

inline int operator<=(const String& x, const char* t)
{
  return compare(x, t) <= 0;
}

inline int operator==(const SubString& x, const String& y)
{
  return compare(y, x) == 0;
}

inline int operator!=(const SubString& x, const String& y)
{
  return compare(y, x) != 0;
}

inline int operator>(const SubString& x, const String& y)
{
  return compare(y, x) < 0;
}

inline int operator>=(const SubString& x, const String& y)
{
  return compare(y, x) <= 0;
}

inline int operator<(const SubString& x, const String& y)
{
  return compare(y, x) > 0;
}

inline int operator<=(const SubString& x, const String& y)
{
  return compare(y, x) >= 0;
}

inline int operator==(const SubString& x, const SubString&  y)
{
  return compare(x, y) == 0;
}

inline int operator!=(const SubString& x, const SubString&  y)
{
  return compare(x, y) != 0;
}

inline int operator>(const SubString& x, const SubString&  y)
{
  return compare(x, y) > 0;
}

inline int operator>=(const SubString& x, const SubString&  y)
{
  return compare(x, y) >= 0;
}

inline int operator<(const SubString& x, const SubString&  y)
{
  return compare(x, y) < 0;
}

inline int operator<=(const SubString& x, const SubString&  y)
{
  return compare(x, y) <= 0;
}

inline int operator==(const SubString& x, const char* t)
{
  return compare(x, t) == 0;
}

inline int operator!=(const SubString& x, const char* t)
{
  return compare(x, t) != 0;
}

inline int operator>(const SubString& x, const char* t)
{
  return compare(x, t) > 0;
}

inline int operator>=(const SubString& x, const char* t)
{
  return compare(x, t) >= 0;
}

inline int operator<(const SubString& x, const char* t)
{
  return compare(x, t) < 0;
}

inline int operator<=(const SubString& x, const char* t)
{
  return compare(x, t) <= 0;
}


// a helper needed by at, before, etc.

inline SubString String::_substr(int first, int l)
{
  if (first < 0 || (unsigned)(first + l) > length() )
    return SubString(_nilString, 0, 0) ;
  else
    return SubString(*this, first, l);
}

#endif
////////////////////////////////////////////////////////////

#endif  // #include "gstring.hpp"

/**********************************************************/

#endif
