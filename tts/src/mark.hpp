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
#ifndef __MARK_HPP__
#define __MARK_HPP__

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
0.0.1    05/10/98  Borja     srate por defecto=128000
0.0.0    15/07/96  Borja     codificacion inicial.

======================== Contenido ========================
$$$ todo:
prepend, etc etc _ms indicando srate cuando no sea una lista.

===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

/**********************************************************/

#ifndef __cplusplus
#error Must use C++ compiler
#endif

/**********************************************************/

#include <stdio.h>
#include "string.hpp"
#include "listt.hpp"

/**********************************************************/

class Mark1D {
private:
	static int cmp(LONG a,LONG b) {if (a>b) return 1; if (a<b) return -1; return 0;}
public:
	LONG pos;
	LONG span;
	String label;

	Mark1D (VOID) {};
	Mark1D (const Mark1D& d) : pos(d.pos), span(d.span), label(d.label) {}
	Mark1D (LONG posx, String labelx="") : pos(posx), span(0), label(labelx) {}
	Mark1D (LONG posx, LONG spanx, String labelx="") : pos(posx), span(spanx), label(labelx) {}

	Mark1D& operator = (const Mark1D& m) { pos=m.pos; span=m.span; label=m.label; return *this; }

	static int compare_p( const Mark1D&a, const Mark1D& b) { return cmp(a.pos,b.pos); }
	static int compare_s( const Mark1D&a, const Mark1D& b) { return cmp(a.span,b.span); }
	static int compare_l( const Mark1D&a, const Mark1D& b) { return compare(a.label,b.label); }
	static int compare_ps( const Mark1D&a, const Mark1D& b) { int c=compare_p(a,b); return c?c:compare_s(a,b); }
	static int compare_pl( const Mark1D&a, const Mark1D& b) { int c=compare_p(a,b); return c?c:compare_l(a,b); }
	static int compare_sl( const Mark1D&a, const Mark1D& b) { int c=compare_s(a,b); return c?c:compare_l(a,b); }
	static int compare_psl( const Mark1D&a, const Mark1D& b) { int c=compare_p(a,b); return c?c:compare_sl(a,b); }
	int operator == (const Mark1D& m) { return compare_psl(*this,m); }
};

/**********************************************************/

class Mark1DList {
	ListT<Mark1D> l;
	DOUBLE srate;
public:
	Mark1DList(VOID) {srate=128000.;}
	Mark1DList(const Mark1DList& a) { l=a.l; srate=a.srate; }
	Mark1DList& operator = (const Mark1DList& a) { l=a.l; srate=a.srate; return *this; }
	virtual ~Mark1DList() { }

	VOID setSRate(DOUBLE sr) { srate=sr; }
	DOUBLE getSRate(VOID) { return srate; }
	VOID adjustSRate(DOUBLE sr);

	BOOL OK(VOID) const;
	BOOL empty(VOID) const {return l.empty();}
	LONG length(VOID) const {return l.length();}
	VOID clear(VOID) {l.clear();}
	Lix first() const {return l.first();}
	Lix last()  const {return l.last();}
	Lix prev(Lix p)  const {return l.prev(p);}
	Lix next(Lix p)  const {return l.next(p);}
	BOOL owns(Lix p)  const {return l.owns(p);}
	LONG index(Lix p)  const {return l.index(p);}
	Lix lix(LONG i)  const {return l.lix(i);}
	VOID reverse(VOID) {l.reverse();}

public:
	Lix insbefore(Lix p, const Mark1D &m) {return l.insbefore(p,m);}
	Lix insafter(Lix p, const Mark1D &m) {return l.insafter(p,m);}
	Lix prepend(const Mark1D &m) {return l.prepend(m);}
	Lix append(const Mark1D &m) {return l.append(m);}
	Lix insbefore(Lix p, const Mark1DList &a) {return l.insbefore(p,a.l);}
	Lix insafter(Lix p, const Mark1DList &a) {return l.insafter(p,a.l);}
	Lix prepend(const Mark1DList &a) {return l.prepend(a.l);}
	Lix append(const Mark1DList &a) {return l.append(a.l);}
	Lix insbefore_mv(Lix p, Mark1DList &a) {return l.insbefore_mv(p,a.l);}
	Lix insafter_mv(Lix p, Mark1DList &a) {return l.insafter_mv(p,a.l);}
	Lix prepend_mv(Mark1DList &a) {return l.prepend_mv(a.l);}
	Lix append_mv(Mark1DList &a) {return l.append_mv(a.l);}
	Lix insbefore(Lix p, LONG pos, const String &label="");
	Lix insafter(Lix p, LONG pos, const String &label="");
	Lix prepend(LONG pos, const String &label="");
	Lix append(LONG pos, const String &label="");
	Lix insbefore(Lix p, LONG pos, LONG span, const String &label="");
	Lix insafter(Lix p, LONG pos, LONG span, const String &label="");
	Lix prepend(LONG pos, LONG span, const String &label="");
	Lix append(LONG pos, LONG span, const String &label="");

	Lix seek_p(LONG pos, Lix from=0) const;
	Lix seek_s(LONG span, Lix from=0) const;
	Lix seek_l(String label, Lix from=0) const;
	Lix seek_ps(LONG pos, LONG span, Lix from=0) const;
	Lix seek_pl(LONG pos, String label, Lix from=0) const;
	Lix seek_sl(LONG span, String label, Lix from=0) const;
	Lix seek_psl(LONG pos, LONG span, String label, Lix from=0) const;
	Lix seek_p(const Mark1D& m, Lix from=0) const { return seek_p(m.pos,from); }
	Lix seek_s(const Mark1D& m, Lix from=0) const { return seek_s(m.span,from); }
	Lix seek_l(const Mark1D& m, Lix from=0) const { return seek_l(m.label,from); }
	Lix seek_ps(const Mark1D& m, Lix from=0) const { return seek_ps(m.pos,m.span,from); }
	Lix seek_pl(const Mark1D& m, Lix from=0) const { return seek_pl(m.pos,m.label,from); }
	Lix seek_sl(const Mark1D& m, Lix from=0) const { return seek_sl(m.span,m.label,from); }
	Lix seek_psl(const Mark1D& m, Lix from=0) const { return seek_psl(m.pos,m.span,m.label,from); }

	Lix rseek_p(LONG pos, Lix from=0) const;
	Lix rseek_s(LONG span, Lix from=0) const;
	Lix rseek_l(String label, Lix from=0) const;
	Lix rseek_ps(LONG pos, LONG span, Lix from=0) const;
	Lix rseek_pl(LONG pos, String label, Lix from=0) const;
	Lix rseek_sl(LONG span, String label, Lix from=0) const;
	Lix rseek_psl(LONG pos, LONG span, String label, Lix from=0) const;
	Lix rseek_p(const Mark1D& m, Lix from=0) const { return rseek_p(m.pos,from); }
	Lix rseek_s(const Mark1D& m, Lix from=0) const { return rseek_s(m.span,from); }
	Lix rseek_l(const Mark1D& m, Lix from=0) const { return rseek_l(m.label,from); }
	Lix rseek_ps(const Mark1D& m, Lix from=0) const { return rseek_ps(m.pos,m.span,from); }
	Lix rseek_pl(const Mark1D& m, Lix from=0) const { return rseek_pl(m.pos,m.label,from); }
	Lix rseek_sl(const Mark1D& m, Lix from=0) const { return rseek_sl(m.span,m.label,from); }
	Lix rseek_psl(const Mark1D& m, Lix from=0) const { return rseek_psl(m.pos,m.span,m.label,from); }

	VOID uniq_p( VOID );
	VOID uniq_s( VOID );
	VOID uniq_l( VOID );
	VOID uniq_ps( VOID );
	VOID uniq_pl( VOID );
	VOID uniq_sl( VOID );
	VOID uniq_psl( VOID );

	Lix del(Lix p, INT dir=1) {return l.del(p,dir);}
	VOID delprev(Lix p) {l.delprev(p);}
	VOID delnext(Lix p) {l.delnext(p);}
	Lix delfirst() {return l.delfirst();}
	Lix dellast() {return l.dellast();}

	const Mark1D& mark(Lix p) const {return l.item(p);}
	const Mark1D& operator () (Lix p) const {return l.item(p);}
	Mark1D& mark(Lix p) {return l.item(p);}

	const LONG& pos(Lix p) const {return l.item(p).pos;}
	LONG& pos(Lix p) {return l.item(p).pos;}
	const LONG& span(Lix p) const {return l.item(p).span;}
	LONG& span(Lix p) {return l.item(p).span;}
	const String& label(Lix p) const {return l.item(p).label;}
	String& label(Lix p) {return l.item(p).label;}

	VOID exchange( Lix p1, Lix p2 ) {l.exchange(p1,p2);}
	VOID sort( BOOL reverse=FALSE );

	VOID adjust_scale( DOUBLE k, LONG t0=0 );
	VOID adjust_del( LONG pos, LONG n=1 );
	VOID adjust_ins( LONG pos, LONG n=1 );
	VOID adjust_range( LONG t0, LONG t1 );

	Lix insbefore_ms(Lix p, const Mark1D &m, DOUBLE srate);
	Lix insafter_ms(Lix p, const Mark1D &m, DOUBLE srate);
	Lix prepend_ms(const Mark1D &m, DOUBLE srate);
	Lix append_ms(const Mark1D &m, DOUBLE srate);
	Lix insbefore_ms(Lix p, const Mark1DList &a);
	Lix insafter_ms(Lix p, const Mark1DList &a);
	Lix prepend_ms(const Mark1DList &a);
	Lix append_ms(const Mark1DList &a);
	Lix insbefore_mv_ms(Lix p, Mark1DList &a);
	Lix insafter_mv_ms(Lix p, Mark1DList &a);
	Lix prepend_mv_ms(Mark1DList &a);
	Lix append_mv_ms(Mark1DList &a);
	Lix insbefore_ms(Lix p, DOUBLE posms, const String &label="");
	Lix insafter_ms(Lix p, DOUBLE posms, const String &label="");
	Lix prepend_ms(DOUBLE posms, const String &label="");
	Lix append_ms(DOUBLE posms, const String &label="");
	Lix insbefore_ms(Lix p, DOUBLE posms, DOUBLE spanms, const String &label="");
	Lix insafter_ms(Lix p, DOUBLE posms, DOUBLE spanms, const String &label="");
	Lix prepend_ms(DOUBLE posms, DOUBLE spanms, const String &label="");
	Lix append_ms(DOUBLE posms, DOUBLE spanms, const String &label="");

	Lix seek_p_ms(DOUBLE posms, Lix from=0) const;
	Lix seek_s_ms(DOUBLE spanms, Lix from=0) const;
	Lix seek_ps_ms(DOUBLE posms, DOUBLE spanms, Lix from=0) const;
	Lix seek_pl_ms(DOUBLE posms, String label, Lix from=0) const;
	Lix seek_sl_ms(DOUBLE spanms, String label, Lix from=0) const;
	Lix seek_psl_ms(DOUBLE posms, DOUBLE spanms, String label, Lix from=0) const;
	Lix seek_p_ms(const Mark1D& m, DOUBLE sr, Lix from=0) const;
	Lix seek_s_ms(const Mark1D& m, DOUBLE sr, Lix from=0) const;
	Lix seek_ps_ms(const Mark1D& m, DOUBLE sr, Lix from=0) const;
	Lix seek_pl_ms(const Mark1D& m, DOUBLE sr, Lix from=0) const;
	Lix seek_sl_ms(const Mark1D& m, DOUBLE sr, Lix from=0) const;
	Lix seek_psl_ms(const Mark1D& m, DOUBLE sr, Lix from=0) const;

	Lix rseek_p_ms(DOUBLE posms, Lix from=0) const;
	Lix rseek_s_ms(DOUBLE spanms, Lix from=0) const;
	Lix rseek_ps_ms(DOUBLE posms, DOUBLE spanms, Lix from=0) const;
	Lix rseek_pl_ms(DOUBLE posms, String label, Lix from=0) const;
	Lix rseek_sl_ms(DOUBLE spanms, String label, Lix from=0) const;
	Lix rseek_psl_ms(DOUBLE posms, DOUBLE spanms, String label, Lix from=0) const;
	Lix rseek_p_ms(const Mark1D& m, DOUBLE sr, Lix from=0) const;
	Lix rseek_s_ms(const Mark1D& m, DOUBLE sr, Lix from=0) const;
	Lix rseek_ps_ms(const Mark1D& m, DOUBLE sr, Lix from=0) const;
	Lix rseek_pl_ms(const Mark1D& m, DOUBLE sr, Lix from=0) const;
	Lix rseek_sl_ms(const Mark1D& m, DOUBLE sr, Lix from=0) const;
	Lix rseek_psl_ms(const Mark1D& m, DOUBLE sr, Lix from=0) const;

	DOUBLE getpos_ms( Lix p) const;
	VOID setpos_ms(Lix p, DOUBLE pos_ms);
	DOUBLE getspan_ms(Lix p) const;
	VOID setspan_ms(Lix p, DOUBLE span_ms);

	VOID adjust_scale_ms( DOUBLE k, DOUBLE t0_ms=0 );
	VOID adjust_del_ms( DOUBLE pos_ms, DOUBLE n_ms=1 );
	VOID adjust_ins_ms( DOUBLE pos_ms, DOUBLE n_ms=1 );
	VOID adjust_range_ms( DOUBLE t0_ms, DOUBLE t1_ms );

	// funcionamiento con indices
	const Mark1D& mark(LIINT i) const {return l.item(i);}
	const Mark1D& operator () (LIINT i) const {return l.item(i);}
	Mark1D& mark(LIINT i) {return l.item(i);}

	const LONG& pos(LIINT i) const {return l.item(i).pos;}
	LONG& pos(LIINT i) {return l.item(i).pos;}
	const LONG& span(LIINT i) const {return l.item(i).span;}
	LONG& span(LIINT i) {return l.item(i).span;}
	const String& label(LIINT i) const {return l.item(i).label;}
	String& label(LIINT i) {return l.item(i).label;}
	VOID exchange( LIINT i1, LIINT i2 ) {l.exchange(i1,i2);}
	DOUBLE getpos_ms(LIINT i) const;
	VOID setpos_ms(LIINT i, DOUBLE pos_ms);
	DOUBLE getspan_ms(LIINT i) const;
	VOID setspan_ms(LIINT i, DOUBLE span_ms);

	BOOL foutput( FILE *f, BOOL indent, BOOL rangespan, BOOL ms );
	VOID finput( FILE* f,	BOOL ms, const CHAR *fileName, LONG &line, CHAR*buf, INT buflen, INT eofch );

	BOOL foutput( FILE *f, BOOL rangespan=FALSE ) { return foutput(f,FALSE,rangespan,FALSE); }
	BOOL foutput_ms( FILE *f, BOOL rangespan=FALSE ) { return foutput(f,FALSE,rangespan,TRUE); }
	VOID finput( FILE* f, BOOL ms=FALSE );
	VOID finput_ms( FILE *f ) { finput(f,TRUE); }
};

/**********************************************************/

#endif

