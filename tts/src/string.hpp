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


#include <iostream>
#include <string.h>

class String {
private:
	char *buf;
protected:
public:
	String();
	String(const String& x);
	String(const char* t);
	String(const char* t, int len);
	String(char c);

	~String();

	String& operator = (const String& y);
	String& operator = (const char* y);
	String& operator = (char c);

	String& operator += (const String& y);
	String& operator += (const char* t);
	String& operator += (char c);

	char& operator [] (int i);
	const char& operator [] (int i) const;

	operator const char*() const;
	const char* chars() const;

	unsigned int length() const;
	int empty() const;

	int index(char c, int startpos = 0) const;
	int index(const String& y, int startpos = 0) const;
	int index(const char* t, int startpos = 0) const;

	int contains(char c) const;
	int contains(const String& y) const;
	int contains(const char* t) const;

	int contains(char c, int pos) const;
	int contains(const String& y, int pos) const;
	int contains(const char* t, int pos) const;

	void upcase();
	friend String upcase(const String& x);

	int gsub(const char* pat, const char* repl);

	String before(int pos);
	String before(const String& x, int startpos = 0);
	String before(const char* t, int startpos = 0);
	String before(char c, int startpos = 0);
	String after(int pos);
	String after(const String& x, int startpos = 0);
	String after(const char* t, int startpos = 0);
	String after(char c, int startpos = 0);

	int OK() const;
};


String operator + (const String& x, const String& y);
String operator + (const String& x, const char* y);
String operator + (const String& x, char y);
String operator + (const char* x, const String& y);

int compare(const String& x, const String& y);
int compare(const String& x, const char* y);

int operator==(const String& x, const String& y);
int operator!=(const String& x, const String& y);
int operator>(const String& x, const String& y);
int operator>=(const String& x, const String& y);
int operator<(const String& x, const String& y);
int operator<=(const String& x, const String& y);
int operator==(const String& x, const char* t);
int operator!=(const String& x, const char* t);
int operator>(const String& x, const char* t);
int operator>=(const String& x, const char* t);
int operator<(const String& x, const char* t);
int operator<=(const String& x, const char* t);

String upcase(const String& x);

#endif
