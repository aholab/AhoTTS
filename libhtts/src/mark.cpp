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

Nombre fuente................ -
Nombre paquete............... -
Lenguaje fuente.............. C++ (Borland C/C++ 3.1)
Estado....................... desarrollo
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Borja Etxebarria

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
0.0.2    30/08/98  Borja     split en varios modules mark_?.cpp
0.0.1    05/03/97  Borja     quitar warnings inocentes en GCC
0.0.0    15/07/96  Borja     codificacion inicial.

======================== Contenido ========================
<DOC>
Listas de marcas unidimensionales {{Mark1DList}}. Ver mark.hpp
para lista completa de metodos y sus prototipos.

Una marca {{Mark1D}} consta de
- pos:  posicion de la marca en muestras
- span: span (rango) de la marca en muestras. 0 para ninguno.
- label: texto asociado.
Las listas de marcas tienen los metodos para acceder a
las listas (ver listt.cpp). Ademas la lista tiene asociada
una frecuencia de muestreo que se puede manejar con los
metodos:
VOID setSRate(DOUBLE sr) ->  para cambiar la frec. muestreo.
DOUBLE getSRate(VOID) -> para leer la frec. muestreo
VOID adjustSRate(DOUBLE sr) -> para cambiar la frec. muestreo y
ademas reajustar todas las marcas a la nueva frec. muestreo.

Una vez configurada la frecuencia de muestreo, los metodos que
reciben valores {pos} y {span} para la posicion y span de la
marca tambien tiene versiones metodo_ms() que reciben los mismos
valores pero en milisegundos, haciendose la conversion a muestras
automaticamente (la marca siempre se almacena en muestras).

Los metodos seek_??() permiten buscar marcas. {Devuelven} una
referencia Lix {p} a la marca encontrada dentro de la lista,
o NULL si no encuentran una marca. Los metodos seek_?? buscan
hacia delante a partir de un Lix inicial {from} (first() si no
se especifica nada), mientras que los metodos rseek_?? buscan
hacia atras a partir de un Lix inicial {from} (last() si no
se especifica nada). Se pueden hacer busquedas de posicion (seek_p),
span (seek_s), conjunta de posicion y span (seek_ps), etc, etc.

Los metodos uniq_??() sirven para eliminar marcas repetidas,
de forma similar al comando uniq del unix. Las marcas
repetidas se buscan fijandose solo en la posicion (seek_p()) o
en la posicion y el span (seek_ps()) etc etc.

Los metodos pos(p) span(p) label(p) permiten leer/escribir la posicion
el span o la etiqueta de la marca identificada por el indice de
lista Lix {p}.  Lo mismo se puede hacer en milisegundos con los
metodos getpos_ms(), setpos_ms(), getspan_ms(), setspan_ms().

adjust_range(t0,t1) quita las marcas que no esten en el rango
en muestras [t0,t1].

adjust_scale(k, t0) desplaza el origen de las marcas a t0 (resta t0)
y escala las posiciones y el span por k(multiplica por k).

adjust_del(posi, n) ajusta la marcas asumiendo que se ha borrado
un rango de {n} muestras comenzando en {posi}: borra las marcas
que estuvieran en dicho rango, y corrige las demas.

adjust_ins(posi, n) ajusta la marcas asumiendo que se ha insertado
un rango de {n} muestras comenzando en {posi}: desplaza las marcas
que esten despues de ese rango.

</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include "mark.hpp"
#include "uti.h"
#include "spl.h"

/**********************************************************/

Lix Mark1DList::insbefore(Lix p, LONG pos, const String &lbl)
{
	Mark1D m(pos);
	p=insbefore(p,m);
	label(p)=lbl;
	return p;
}

/**********************************************************/

Lix Mark1DList::insafter(Lix p, LONG pos, const String &lbl)
{
	Mark1D m(pos);
	p=insafter(p,m);
	label(p)=lbl;
	return p;
}

/**********************************************************/

Lix Mark1DList::prepend(LONG pos, const String &lbl)
{
	Mark1D m(pos);
	Lix p=prepend(m);
	label(p)=lbl;
	return p;
}

/**********************************************************/

Lix Mark1DList::append(LONG pos, const String &lbl)
{
	Mark1D m(pos);
	Lix p=append(m);
	label(p)=lbl;
	return p;
}

/**********************************************************/

Lix Mark1DList::insbefore(Lix p, LONG pos, LONG span, const String &lbl)
{
	Mark1D m(pos,span);
	p=insbefore(p,m);
	label(p)=lbl;
	return p;
}

/**********************************************************/

Lix Mark1DList::insafter(Lix p, LONG pos, LONG span, const String &lbl)
{
	Mark1D m(pos,span);
	p=insafter(p,m);
	label(p)=lbl;
	return p;
}

/**********************************************************/

Lix Mark1DList::prepend(LONG pos, LONG span, const String &lbl)
{
	Mark1D m(pos,span);
	Lix p=prepend(m);
	label(p)=lbl;
	return p;
}

/**********************************************************/

Lix Mark1DList::append(LONG pos, LONG span, const String &lbl)
{
	Mark1D m(pos,span);
	Lix p=append(m);
	label(p)=lbl;
	return p;
}

/**********************************************************/

Lix Mark1DList::insbefore_ms(Lix p, const Mark1D &m, DOUBLE sr)
{
	DOUBLE f = ((sr && srate) ? srate/sr : 1);
	return insbefore(p,(LONG)fround(m.pos*f),(LONG)fround(m.span*f),m.label);
}

/**********************************************************/

Lix Mark1DList::insafter_ms(Lix p, const Mark1D &m, DOUBLE sr)
{
	DOUBLE f = ((sr && srate) ? srate/sr : 1);
	return insafter(p,(LONG)fround(m.pos*f),(LONG)fround(m.span*f),m.label);
}

/**********************************************************/

Lix Mark1DList::prepend_ms(const Mark1D &m, DOUBLE sr)
{
	DOUBLE f = ((sr && srate) ? srate/sr : 1);
	return prepend((LONG)fround(m.pos*f),(LONG)fround(m.span*f),m.label);
}

/**********************************************************/

Lix Mark1DList::append_ms(const Mark1D &m, DOUBLE sr)
{
	DOUBLE f = ((sr && srate) ? srate/sr : 1);
	return append((LONG)fround(m.pos*f),(LONG)fround(m.span*f),m.label);
}

/**********************************************************/

Lix Mark1DList::insbefore_ms(Lix p, const Mark1DList &a)
{
	for (Lix q=a.last(); q!=0; q=a.prev(q)) p=insbefore_ms(p,a(q),a.srate);
	return p;
}

/**********************************************************/

Lix Mark1DList::insafter_ms(Lix p, const Mark1DList &a)
{
	for (Lix q=a.first(); q!=0; q=a.next(q)) p=insafter_ms(p,a(q),a.srate);
	return p;
}

/**********************************************************/

Lix Mark1DList::prepend_ms(const Mark1DList &a)
{
	Lix p=0;
	for (Lix q=a.last(); q!=0; q=a.prev(q)) p=prepend_ms(a(q),a.srate);
	return p;
}

/**********************************************************/

Lix Mark1DList::append_ms(const Mark1DList &a)
{
	Lix p=0;
	for (Lix q=a.first(); q!=0; q=a.next(q)) p=append_ms(a(q),a.srate);
	return p;
}

/**********************************************************/

Lix Mark1DList::insbefore_mv_ms(Lix p, Mark1DList &a)
{
	a.adjustSRate(srate);
	return insbefore_mv(p,a);
}

/**********************************************************/

Lix Mark1DList::insafter_mv_ms(Lix p, Mark1DList &a)
{
	a.adjustSRate(srate);
	return insafter_mv(p,a);
}

/**********************************************************/

Lix Mark1DList::prepend_mv_ms(Mark1DList &a)
{
	a.adjustSRate(srate);
	return prepend_mv(a);
}

/**********************************************************/

Lix Mark1DList::append_mv_ms(Mark1DList &a)
{
	a.adjustSRate(srate);
	return append_mv(a);
}

/**********************************************************/

Lix Mark1DList::insbefore_ms(Lix p, DOUBLE posms, const String &lbl)
{
	return insbefore(p,(LONG)fround( srate ? ms2samples(posms,srate): posms),lbl);
}

/**********************************************************/

Lix Mark1DList::insafter_ms(Lix p, DOUBLE posms, const String &lbl)
{
	return insafter(p,(LONG)fround( srate ? ms2samples(posms,srate): posms),lbl);
	}

/**********************************************************/

Lix Mark1DList::prepend_ms(DOUBLE posms, const String &lbl)
{
	return prepend((LONG)fround( srate ? ms2samples(posms,srate): posms),lbl);
}

/**********************************************************/

Lix Mark1DList::append_ms(DOUBLE posms, const String &lbl)
{
	return append((LONG)fround( srate ? ms2samples(posms,srate): posms),lbl);
}

/**********************************************************/

Lix Mark1DList::insbefore_ms(Lix p, DOUBLE posms, DOUBLE spanms, const String &lbl)
{
	return insbefore(p,(LONG)fround(srate?ms2samples(posms,srate):posms),
			(LONG)fround(srate?ms2samples(spanms,srate):posms),lbl);
}

/**********************************************************/

Lix Mark1DList::insafter_ms(Lix p, DOUBLE posms, DOUBLE spanms, const String &lbl)
{
	return insafter(p,(LONG)fround(srate?ms2samples(posms,srate):posms),
			(LONG)fround(srate?ms2samples(spanms,srate):posms),lbl);
}

/**********************************************************/

Lix Mark1DList::prepend_ms(DOUBLE posms, DOUBLE spanms, const String &lbl)
{
	return prepend((LONG)fround(srate?ms2samples(posms,srate):posms),
			(LONG)fround(srate?ms2samples(spanms,srate):posms),lbl);
}

/**********************************************************/

Lix Mark1DList::append_ms(DOUBLE posms, DOUBLE spanms, const String &lbl)
{
	return append((LONG)fround(srate?ms2samples(posms,srate):posms),
			(LONG)fround(srate?ms2samples(spanms,srate):posms),lbl);
}

/**********************************************************/

DOUBLE Mark1DList::getpos_ms( Lix p) const
{
	return srate ? samples2ms(pos(p),srate) : pos(p);
}

/**********************************************************/

VOID Mark1DList::setpos_ms(Lix p, DOUBLE pos_ms)
{
	pos(p) = (LONG)fround(srate ? ms2samples(pos_ms,srate) : pos_ms);
}

/**********************************************************/

DOUBLE Mark1DList::getspan_ms(Lix p) const
{
	return srate ? samples2ms(span(p),srate) : span(p);
}

/**********************************************************/

VOID Mark1DList::setspan_ms(Lix p, DOUBLE span_ms)
{
	span(p) = (LONG)fround(srate ? ms2samples(span_ms,srate) : span_ms);
}

/**********************************************************/

DOUBLE Mark1DList::getpos_ms( LIINT i) const
{
	return srate ? samples2ms(pos(i),srate) : pos(i);
}

/**********************************************************/

VOID Mark1DList::setpos_ms(LIINT i, DOUBLE pos_ms)
{
	pos(i) = (LONG)fround(srate ? ms2samples(pos_ms,srate) : pos_ms);
}

/**********************************************************/

DOUBLE Mark1DList::getspan_ms(LIINT i) const
{
	return srate ? samples2ms(span(i),srate) : span(i);
}

/**********************************************************/

VOID Mark1DList::setspan_ms(LIINT i, DOUBLE span_ms)
{
	span(i) = (LONG)fround(srate ? ms2samples(span_ms,srate) : span_ms);
}

/**********************************************************/
