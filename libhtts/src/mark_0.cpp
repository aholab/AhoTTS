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

======================== Contenido ========================
ver mark.cpp
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include "mark.hpp"
#include "uti.h"
#include "spl.h"

/**********************************************************/

VOID Mark1DList::adjustSRate(DOUBLE sr)
{
	if (sr && srate) adjust_scale(sr/srate);
}

/**********************************************************/

BOOL Mark1DList::OK(VOID) const
{
	if (!l.OK()) return FALSE;
	for (Lix p=l.first(); p!=0; p=l.next(p))
		if (!l.item(p).label.OK()) return FALSE;
	return TRUE;
}

/**********************************************************/

VOID Mark1DList::adjust_range( LONG t0, LONG t1 )
{
	Lix p;
	LONG t, tt, sp;

	p=first();
	while (p) {
		sp=span(p);
		if (sp==0) {
			t=pos(p);
			if ((t<t0)||(t>t1)) p=del(p,1);
			else p=next(p);
		}
		else if (sp>0) {
			t=pos(p); tt=t+sp-1;
			if ((tt<t0)||(t>t1)) p=del(p,1);
			else if (t<t0) {
				if (tt>t1) tt=t1;
				pos(p)=t0; span(p)=tt-t0+1; p=next(p);
			}
			else if (tt>t1) { span(p)=t1-t+1; p=next(p); }
			else p=next(p);
		}
		else {
			t=pos(p); tt=t+sp+1;
			if ((t<t0)||(tt>t1)) p=del(p,1);
			else if (t>t1) {
				if (tt<t0) tt=t0;
				pos(p)=t1; span(p)=tt-t1-1; p=next(p);
			}
			else if (tt<t0) { span(p)=t0-t-1; p=next(p); }
			else p=next(p);
		}
	}
}

/**********************************************************/

VOID Mark1DList::adjust_scale( DOUBLE k, LONG t0 )
{
	Lix p;

	for (p=first(); p!=0; p=next(p)) {
		pos(p) = (LONG)fround((pos(p)-t0)*k);
		span(p) = (LONG)fround(span(p)*k);
	}
}

/**********************************************************/

VOID Mark1DList::adjust_del( LONG posi, LONG n )
{
	Lix p;
	LONG posif=posi+n-1;
	LONG sp, asp, iposi, iposif;

	if (!n) return;
	if (n<0) { n=-n; posi-=(n-1); }

	p=first();
	while (p!=0) {
		sp=span(p);
		if (sp==0) {
			iposi=pos(p);
			if (iposi>=posi) {
				if (iposi<=posif) p=del(p,1);
				else { pos(p)=iposi-n; p=next(p); }
			}
			else p=next(p);
		}
		else {
			if (sp>0) { iposi=pos(p); iposif=iposi+sp-1; asp=sp;}
			else { iposif=pos(p); iposi=iposif+sp+1; asp=-sp;}

			if (posi<iposi) {
				if (posif>=iposif) p=del(p,1);
				else if (posif<iposi) { pos(p)-=n; p=next(p); }
				else {
					asp -= posif-iposi+1;
					if (sp>0) { span(p)=asp; pos(p)=posi; }
					else { span(p)=-asp; pos(p)-=n; }
					p=next(p);
				}
			}
			else if (posi<=iposif) {
				asp -= ((posif>iposif)?iposif:posif)-posi+1;
				if (!asp) p=del(p,1);
				else {
					if (sp>0) span(p)=asp;
					else { span(p)=-asp; pos(p)+=(asp+sp); }
					p=next(p);
				}
			}
			else p=next(p);
		}
	}
}

/**********************************************************/

VOID Mark1DList::adjust_ins( LONG posi, LONG n )
{
	Lix p;
	LONG sp, iposi;

	if (!n) return;
	if (n<0) n=-n;

	p=first();
	while (p!=0) {
		sp=span(p);
		iposi=pos(p);
		if (sp==0)
			if (iposi>=posi) pos(p)=iposi+n;
		else if (sp>0) {
			if (posi<=iposi) pos(p)=iposi+n;
			else if (posi<iposi+sp-1) span(p)=sp+n;
		}
		else {
			if (posi<=iposi+sp+1) pos(p)=iposi+n;
			else if (posi<iposi) span(p)=sp-n;
		}
		p=next(p);
	}
}

/**********************************************************/

VOID Mark1DList::adjust_scale_ms( DOUBLE k, DOUBLE t0 )
{
	adjust_scale(k,(LONG)fround(srate?ms2samples(t0,srate):t0));
}

/**********************************************************/

VOID Mark1DList::adjust_del_ms( DOUBLE pos, DOUBLE n )
{
	adjust_del((LONG)fround(srate?ms2samples(pos,srate):pos),
			(LONG)fround(srate?ms2samples(n,srate):n));
}

/**********************************************************/

VOID Mark1DList::adjust_ins_ms( DOUBLE pos, DOUBLE n )
{
	adjust_ins((LONG)fround(srate?ms2samples(pos,srate):pos),
			(LONG)fround(srate?ms2samples(n,srate):n));
}

/**********************************************************/

VOID Mark1DList::adjust_range_ms( DOUBLE t0, DOUBLE t1 )
{
	adjust_range((LONG)fround(srate?ms2samples(t0,srate):t0),
			(LONG)fround(srate?ms2samples(t1,srate):t1));
}

/**********************************************************/
