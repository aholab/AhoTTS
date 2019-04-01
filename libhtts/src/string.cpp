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
Copyright: 1998 - Aholab

Nombre fuente................ STRING.HPP
Nombre paquete............... -
Lenguaje fuente.............. C++
Estado....................... -
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Borja Etxebarria

Version  dd/mm/aa  Autor     Comentario
-------  --------  --------  ----------
1.0.1    2012  	   Inaki     Eliminar dependencia de xalloc
1.0.0    17/09/98  Borja     Codificacion inicial.

======================== Contenido ========================
<DOC>
Clase String simplificada, codificada desde 0.

Contiene lo minimo utilizado por otros modulos Aholab.
</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include <string.h>
#include <ctype.h>
#include "string.hpp"
#include <stdlib.h>
/**********************************************************/

String::String()
{
	buf=strdup("");
}

/**********************************************************/

String::String(const String& x)
{
	buf=strdup(x);
}

/**********************************************************/

String::String(const char* t)
{
	buf=strdup(t);
}

/**********************************************************/

String::String(const char* t, int len)
{
	int l=strlen(t);
	if (l>len) l=len;
	buf=(char*)malloc(l+1);
	strncpy(buf,t,l);
	buf[l]='\0';
}

/**********************************************************/

String::String(char c)
{
	buf=(char*)malloc(2);
	buf[0]=c;
}

/**********************************************************/

String::~String()
{
	if (buf) free(buf);
}

/**********************************************************/

String& String::operator = (const String& y)
{
	char *tmp=strdup(y.buf);
	if (buf) free(buf);
	buf=tmp;
	return *this;
}

/**********************************************************/

String& String::operator = (const char* y)
{
	char *tmp=strdup(y);
	if (buf) free(buf);
	buf=tmp;
	return *this;
}

/**********************************************************/

String& String::operator = (char c)
{
	char *tmp=(char*)malloc(2);
	tmp[0]=c;
	tmp[1]='\0';
	if (buf) free(buf);
	buf=tmp;
	return *this;
}

/**********************************************************/

String& String::operator += (const String& y)
{
	return operator +=((const char*)y);
}

/**********************************************************/

String& String::operator += (const char* t)
{
	char *tmp;
	tmp=(char*)malloc((buf?strlen(buf):0)+(t?strlen(t):0)+1);
	tmp[0]='\0';
	if (buf) { strcat(tmp,buf); free(buf); }
	if (t) strcat(tmp,t);
	buf=tmp;
	return *this;
}

/**********************************************************/

String& String::operator += (char c)
{
	char *tmp;
	tmp=(char*)malloc((buf?strlen(buf):0)+2);
	tmp[0]='\0';
	if (buf) { strcat(tmp,buf); free(buf); }
	size_t l=strlen(tmp);
	tmp[l+1]='\0';
	tmp[l]=c;
	buf=tmp;
	return *this;
}

/**********************************************************/

char& String::operator [] (int i)
{
	return buf[i];
}

/**********************************************************/

const char& String::operator [] (int i) const
{
	return buf[i];
}

/**********************************************************/

String::operator const char*() const
{
	return buf;
}

/**********************************************************/

const char* String::chars() const
{
	return buf;
}

/**********************************************************/

unsigned int String::length() const
{
	return strlen(buf);
}

/**********************************************************/

int String::empty() const
{
	return (buf[0]=='\0');
}

/**********************************************************/

int String::OK() const
{
	return buf!=NULL;
}

/**********************************************************/

int String::index(char c, int pos) const
{
	const char *x=strchr(buf+pos,c);
	if (x) return (int)(buf-(char*)x);
	return -1;
}

/**********************************************************/

int String::index(const String& y, int pos) const
{
	return index(y.buf,pos);
}

/**********************************************************/

int String::index(const char* t, int pos) const
{
	const char *x=strstr(buf+pos,t);
	if (x) return (int)(buf-(char*)x);
	return -1;
}

/**********************************************************/

int String::contains(char c) const { return strchr(buf,c)!=NULL; }

/**********************************************************/

int String::contains(const String& y) const { return strstr(buf,y.buf)!=NULL; }

/**********************************************************/

int String::contains(const char* t) const { return strstr(buf,t)!=NULL; }

/**********************************************************/

int String::contains(char c, int pos) const
{
	if (pos>=0) return strchr(buf+pos,c)!=NULL;
	char *x=strchr(buf,c);
	if (!x) return 0;
	return (buf-x <= pos);
}

/**********************************************************/

int String::contains(const String& y, int pos) const
{
	return contains(y.buf,pos);
}

/**********************************************************/

int String::contains(const char* t, int pos) const
{
	if (pos>=0) return strstr(buf+pos,t)!=NULL;
	char *x=strstr(buf,t);
	if (!x) return 0;
	return (buf-x <= pos);
}

/**********************************************************/

String operator + (const String& x, const String& y) { String r(x); r+=y; return r; }
String operator + (const String& x, const char* y) { String r(x); r+=y; return r; }
String operator + (const String& x, char y) { String r(x); r+=y; return r; }
String operator + (const char* x, const String& y) { String r(x); r+=y; return r; }


int compare(const String& x, const String& y) { return strcmp(x,y); }
int compare(const String& x, const char* y) { return strcmp(x,y); }

int operator==(const String& x, const String& y) { return compare(x, y) == 0; }
int operator!=(const String& x, const String& y) { return compare(x, y) != 0; }
int operator>(const String& x, const String& y) { return compare(x, y) > 0; }
int operator>=(const String& x, const String& y) { return compare(x, y) >= 0; }
int operator<(const String& x, const String& y) { return compare(x, y) < 0; }
int operator<=(const String& x, const String& y) { return compare(x, y) <= 0; }
int operator==(const String& x, const char* t) { return compare(x, t) == 0; }
int operator!=(const String& x, const char* t) { return compare(x, t) != 0; }
int operator>(const String& x, const char* t) { return compare(x, t) > 0; }
int operator>=(const String& x, const char* t) { return compare(x, t) >= 0; }
int operator<(const String& x, const char* t) { return compare(x, t) < 0; }
int operator<=(const String& x, const char* t) { return compare(x, t) <= 0; }

/**********************************************************/

void String::upcase()
{
	char *s=buf;
	while (*s)
	{
		*s=toupper(*s);
		s++;
	}
}

/**********************************************************/

String upcase(const String& x) { String s(x); s.upcase(); return s; }

/**********************************************************/

int String::gsub(const char* pat, const char* repl)
{
	size_t l=strlen(pat);
	char *from=buf;
	int n=0;
	char *s;
	String dest;

	while ( (s=strstr(from,pat))!=NULL ) {
		n++;
		String tmp(from,(int)(s-from));
		dest += tmp+repl;
		from = s+l;
	}
	dest += from;
	free(buf);
	buf=dest.buf;
	dest.buf=NULL;
	return n;
}

/**********************************************************/
String String::before(int pos)
{
	if (pos<0) return String(buf);
	if (pos>0) return	String(buf,pos-1);
	else return String();
}

String String::before(const String& x, int startpos) { return before(index(x,startpos)); }
String String::before(const char* t, int startpos) { return before(index(t,startpos)); }
String String::before(char c, int startpos) { return before(index(c,startpos)); }
String String::after(int pos)
{
	if (pos<0) return String(buf);
	String tmp(buf+pos+1);
	return tmp;
}

String String::after(const String& x, int startpos)  { return after(index(x,startpos)); }
String String::after(const char* t, int startpos) { return after(index(t,startpos)); }
String String::after(char c, int startpos) { return after(index(c,startpos)); }

/**********************************************************/
