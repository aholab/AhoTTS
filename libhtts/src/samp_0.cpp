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

Nombre fuente................ SAMP.CPP
Nombre paquete............... -
Lenguaje fuente.............. C++ (BC31, GNUC)
Estado....................... desarrollo
Dependencia Hard/OS.......... -
Codigo condicional........... FLUSH_WHEN_WRITE

Codificacion................. Borja Etxebarria

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
1.1.4    09/04/99  Borja     uso tipos UINT
0.1.3    30/08/98  Borja     split en varios modulos samp_?.cpp
0.1.2    22/08/98  Borja     bug en lectores de bloque/cache
0.1.1    22/08/97  Borja     manejo FLUSH_WHEN_WRITE
0.1.0    05/05/97  Borja     get_i16(), get_d()
0.0.1    05/03/97  Borja     quitar warnings inocentes en GCC
0.0.0    15/07/96  Borja     codificacion inicial.

======================== Contenido ========================
ver samp.cpp
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include "assert.h"
#include "uti.h"
#include "spl.h"
#include "samp.hpp"

/**********************************************************/

UINT Samp::getCh( INT16& sample, UINT ch )
{
	UINT ret=0;
	if (ch>=nchan) {
		LONG l=0;
		for (UINT i=0; i<nchan; i++) { ret=getNoCh(sample); l+=sample; }
		sample = (l+(nchan/2)/nchan);
		return ret;
	}
	else {
		if (ch>0) setPosNoCh(getPosNoCh()+ch);
		ret=getNoCh(sample);
		if (ch+1<nchan) setPosNoCh(getPosNoCh()+(nchan-ch-1));
		return ret;
	}
}

/**********************************************************/

UINT Samp::setCh( INT16 sample, UINT ch )
{
	UINT ret=0;
	if (ch>=nchan) {
		for (UINT i=0; i<nchan; i++) ret=setNoCh(sample);
		return ret;
	}
	else {
		if (getNNoCh()<=getPosNoCh()) return insCh(sample,ch);

		if (ch>0) setPosNoCh(getPosNoCh()+ch);
		ret=setNoCh(sample);
		if (ch+1<nchan) setPosNoCh(getPosNoCh()+(nchan-ch-1));
		return ret;
	}
}

/**********************************************************/

UINT Samp::insCh( INT16 sample, UINT ch )
{
	UINT ret=0;
	UINT i;
	if (ch>=nchan) {
		for (i=0; i<nchan; i++) ret=insNoCh(sample);
		return ret;
	}
	else {
		for (i=0; i<ch; i++) insNoCh((INT16)0);
		ret=insNoCh(sample);
		for (i=ch+1; i<nchan; i++) insNoCh((INT16)0);
		return ret;
	}
}

/**********************************************************/

UINT Samp::getCh( DOUBLE& sample, UINT ch )
{
	UINT ret=0;
	if (ch>=nchan) {
		DOUBLE l=0;
		for (UINT i=0; i<nchan; i++) { ret=getNoCh(sample); l+=sample; }
		sample = l/nchan;
		return ret;
	}
	else {
		if (ch>0) setPosNoCh(getPosNoCh()+ch);
		ret=getNoCh(sample);
		if (ch+1<nchan) setPosNoCh(getPosNoCh()+(nchan-ch-1));
		return ret;
	}
}

/**********************************************************/

UINT Samp::setCh( DOUBLE sample, UINT ch )
{
	UINT ret=0;
	if (ch>=nchan) {
		for (UINT i=0; i<nchan; i++) ret=setNoCh(sample);
		return ret;
	}
	else {
		if (ch>0) setPosNoCh(getPosNoCh()+ch);
		ret=setNoCh(sample);
		if (ch+1<nchan) setPosNoCh(getPosNoCh()+(nchan-ch-1));
		return ret;
	}
}

/**********************************************************/

UINT Samp::insCh( DOUBLE sample, UINT ch )
{
	UINT ret=0;
	UINT i;
	if (ch>=nchan) {
		for (i=0; i<nchan; i++) ret=insNoCh(sample);
		return ret;
	}
	else {
		for (i=0; i<ch; i++) insNoCh((DOUBLE)0);
		ret=insNoCh(sample);
		for (i=ch+1; i<nchan; i++) insNoCh((DOUBLE)0);
		return ret;
	}
}

/**********************************************************/

UINT Samp::getBlkCh( INT16* block, UINT len, UINT ch )
{
	if (nchan==1) return getBlkNoCh(block,len);

	UINT n = 0;
	while (len--) n+=getCh(*(block++),ch);
	return n;
}

/**********************************************************/

UINT Samp::setBlkCh( const INT16* block, UINT len, UINT ch )
{
	if (nchan==1) return setBlkNoCh(block,len);

	UINT n = 0;
	if (!block) while (len--) n+=setCh((INT16)0,ch);
	else while (len--) n+=setCh(*(block++),ch);
	return n;
}

/**********************************************************/

UINT Samp::insBlkCh( const INT16* block, UINT len, UINT ch )
{
	if (nchan==1) return insBlkNoCh(block,len);

	UINT n = 0;
	if (!block) while (len--) n+=insCh((INT16)0,ch);
	else while (len--) n+=insCh(*(block++),ch);
	return n;
}

/**********************************************************/

UINT Samp::getBlkCh( DOUBLE* block, UINT len, UINT ch )
{
	if (nchan==1) return getBlkNoCh(block,len);

	UINT n = 0;
	while (len--) n+=getCh(*(block++),ch);
	return n;
}

/**********************************************************/

UINT Samp::setBlkCh( const DOUBLE* block, UINT len, UINT ch )
{
	if (nchan==1) return setBlkNoCh(block,len);

	UINT n = 0;
	if (!block) while (len--) n+=setCh((DOUBLE)0,ch);
	else while (len--) n+=setCh(*(block++),ch);
	return n;
}

/**********************************************************/

UINT Samp::insBlkCh( const DOUBLE* block, UINT len, UINT ch )
{
	if (nchan==1) return insBlkNoCh(block,len);

	UINT n = 0;
	if (!block) while (len--) n+=insCh((DOUBLE)0,ch);
	else while (len--) n+=insCh(*(block++),ch);
	return n;
}

/**********************************************************/

UINT Samp::getChRange( INT16* sampr, UINT ch1, UINT ch2 )
{
	UINT ret=0;
	assert(ch1<=ch2);
	while ((ch2>=nchan)&&(ch2>=ch1)) { sampr[ch2-ch1]=0; ch2--; }
	if (ch1>=nchan) return ret;

	if (ch1>0) setPosNoCh(getPosNoCh()+ch1);
	ret=getBlkNoCh(sampr,ch2-ch1+1);
	if (ch2+1<nchan) setPosNoCh(getPosNoCh()+(nchan-ch2-1));
	return ret;
}

/**********************************************************/

UINT Samp::setChRange( const INT16* sampr, UINT ch1, UINT ch2 )
{
	UINT ret=0;
	if (getNNoCh()<=getPosNoCh()) return insChRange(sampr,ch1,ch2);

	assert(ch1<=ch2);
	if (ch2>=nchan) ch2=nchan-1;
	if (ch1>=nchan) return ret;

	if (ch1>0) setPosNoCh(getPosNoCh()+ch1);
	ret=setBlkNoCh(sampr,ch2-ch1+1);
	if (ch2+1<nchan) setPosNoCh(getPosNoCh()+(nchan-ch2-1));
	return ret;
}

/**********************************************************/

UINT Samp::insChRange( const INT16* sampr, UINT ch1, UINT ch2 )
{
	UINT ret, i;
	assert(ch1<=ch2);
	if (ch2>=nchan) ch2=nchan-1;
	if (ch1>=nchan) ch1=nchan;

	for (i=0; i<ch1; i++) insNoCh((INT16)0);
	if (ch1>=nchan) return 0;
	ret=setBlkNoCh(sampr,ch2-ch1+1);
	for (i=ch2+1; i<nchan; i++) insNoCh((INT16)0);
	return ret;
}

/**********************************************************/

UINT Samp::getChRange( DOUBLE* sampr, UINT ch1, UINT ch2 )
{
	UINT ret;
	assert(ch1<=ch2);
	while ((ch2>=nchan)&&(ch2>=ch1)) { sampr[ch2-ch1]=0; ch2--; }
	if (ch1>=nchan) return 0;

	if (ch1>0) setPosNoCh(getPosNoCh()+ch1);
	ret=getBlkNoCh(sampr,ch2-ch1+1);
	if (ch2+1<nchan) setPosNoCh(getPosNoCh()+(nchan-ch2-1));
	return ret;
}

/**********************************************************/

UINT Samp::setChRange( const DOUBLE* sampr, UINT ch1, UINT ch2 )
{
	UINT ret;
	if (getNNoCh()<=getPosNoCh()) return insChRange(sampr,ch1,ch2);

	assert(ch1<=ch2);
	if (ch2>=nchan) ch2=nchan-1;
	if (ch1>=nchan) return 0;

	if (ch1>0) setPosNoCh(getPosNoCh()+ch1);
	ret=setBlkNoCh(sampr,ch2-ch1+1);
	if (ch2+1<nchan) setPosNoCh(getPosNoCh()+(nchan-ch2-1));
	return ret;
}

/**********************************************************/

UINT Samp::insChRange( const DOUBLE* sampr, UINT ch1, UINT ch2 )
{
	UINT ret, i;
	assert(ch1<=ch2);
	if (ch2>=nchan) ch2=nchan-1;
	if (ch1>=nchan) ch1=nchan;

	for (i=0; i<ch1; i++) insNoCh((DOUBLE)0);
	if (ch1>=nchan) return 0;
	ret=setBlkNoCh(sampr,ch2-ch1+1);
	for (i=ch2+1; i<nchan; i++) insNoCh((DOUBLE)0);
	return ret;
}

/**********************************************************/

UINT Samp::getBlkChRange( INT16* block, UINT len, UINT ch1, UINT ch2 )
{
	UINT n = 0;
	while (len--) {
		n+=getChRange(block,ch1,ch2);
		block+= (ch2-ch1+1);
	}
	return n;
}

/**********************************************************/

UINT Samp::setBlkChRange( const INT16* block, UINT len, UINT ch1, UINT ch2 )
{
	UINT n = 0;
	while (len--) {
		n+=setChRange(block,ch1,ch2);
		if (block) block+=(ch2-ch1+1);
	}
	return n;
}

/**********************************************************/

UINT Samp::insBlkChRange( const INT16* block, UINT len, UINT ch1, UINT ch2 )
{
	UINT n = 0;
	while (len--) {
		n+=insChRange(block,ch1,ch2);
		if (block) block+=(ch2-ch1+1);
	}
	return n;
}

/**********************************************************/

UINT Samp::getBlkChRange( DOUBLE* block, UINT len, UINT ch1, UINT ch2 )
{
	UINT n = 0;
	while (len--) {
		n+=getChRange(block,ch1,ch2);
		block+=(ch2-ch1+1);
	}
	return n;
}

/**********************************************************/

UINT Samp::setBlkChRange( const DOUBLE* block, UINT len, UINT ch1, UINT ch2 )
{
	UINT n = 0;
	while (len--) {
		n+=setChRange(block,ch1,ch2);
		if (block) block+=(ch2-ch1+1);
	}
	return n;
}

/**********************************************************/

UINT Samp::insBlkChRange( const DOUBLE* block, UINT len, UINT ch1, UINT ch2 )
{
	UINT n = 0;
	while (len--) {
		n+=insChRange(block,ch1,ch2);
		if (block) block+=(ch2-ch1+1);
	}
	return n;
}

/**********************************************************/

