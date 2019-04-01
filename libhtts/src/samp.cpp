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
1.1.5    2012  	   Inaki     Eliminar dependencia de xalloc
1.1.4    09/04/99  Borja     uso tipos UINT
0.1.3    30/08/98  Borja     split en varios modulos samp_?.cpp
0.1.2    22/08/98  Borja     bug en lectores de bloque/cache
0.1.1    22/08/97  Borja     manejo FLUSH_WHEN_WRITE
0.1.0    05/05/97  Borja     get_i16(), get_d()
0.0.1    05/03/97  Borja     quitar warnings inocentes en GCC
0.0.0    15/07/96  Borja     codificacion inicial.

======================== Contenido ========================
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include "assert.h"
#include "uti.h"
#include "spl.h"
#include "samp.hpp"
#include <stdlib.h>
/**********************************************************/
// para tptr

enum { TPTR_NONE, TPTR_i16, TPTR_d, TPTR_i16ChAll, TPTR_dChAll };

/**********************************************************/
/* Si esta definido, el cache de get_d y get_i16 se descarta
cada vez que se escriba al fichero. asi no hay riesgos de
inconsistencias entre lo escrito y lo leido */

#define FLUSH_WHEN_WRITE

/**********************************************************/

Samp::Samp( VOID )
{
	ptr=0;
	// inicializamos a valor valido
	sSize = 1;
	nchan = 1;
	sSizeCh = 1;
}

/**********************************************************/

UINT Samp::getSampTypeSize( INT sampType )
{
	switch (sampType) {
	case SAMPTYPE_PCM8U: return sizeof(UINT8);
	case SAMPTYPE_PCM16: return sizeof(INT16);
	case SAMPTYPE_PCM32: return sizeof(INT32);
	case SAMPTYPE_FLOAT32: return sizeof(FLOAT32);
	case SAMPTYPE_FLOAT64: return sizeof(FLOAT64);
	}

	die_beep("Samp: invalid SampType (%li)",(long)sampType);
	return 0;
}

/**********************************************************/

VOID Samp::attach( FILE* fBin, INT sampType, UINT nChan, LONG N )
{
	if (!fBin) {  // unattach
		FBlock::attach(NULL,0);
		if (ptr) free(ptr);
		ptr=0;
		return;
	}

	tSamp = sampType;  // tipo muestra
	sSize = getSampTypeSize(sampType);  // tamanyo (bytes) de la muestra
	nchan = nChan;  // numero de canales
	cdie_beep(nchan<1,"Samp: invalid number of channels (%li)",(long)nchan);
	sSizeCh = sSize*nchan;

	if (N>=0) FBlock::attach(fBin,N*sSizeCh);
	else {
		FBlock::attach(fBin,N);
		cdie_beep((UINT)(FBlock::getN()%sSizeCh),
				"Samp: not enough bytes in file to complete last sample");
	}
}

/**********************************************************/

INT16 *Samp::get_i16( UINT len, BOOL reloadall )
{
	LONG nn=(LONG)len*sizeof(INT16);
	if ((!ptr)||(nptr!=nn)) {
		nptr=nn; if (ptr) free(ptr); ptr=malloc(nptr); reloadall=TRUE;
	}
	if ((tptr!=TPTR_i16)||(reloadall)) { pptr=getPos(); getBlk((INT16*)ptr,len); }
	else {
		LONG p=getPos();
		if (p<pptr) {  // anyadir por delante
			LONG nn=pptr-p;
			UINT n=(UINT)nn;
			if (nn>=(LONG)len) n=len;
			else memmove((void *)((INT16*)ptr+n),ptr,(len-n)*sizeof(INT16));
			getBlk((INT16*)ptr,n);
			pptr=p;
		}
		else if (p>pptr) {  // anyadir por detras
			LONG nn=p-pptr;
			UINT n=(UINT)nn;
			if (nn>=(LONG)len) getBlk((INT16*)ptr,len);
			else {
				memmove(ptr,(void*)((INT16*)ptr+n),(len-n)*sizeof(INT16));
				setPos(p+(len-n));
				getBlk((INT16*)ptr+(len-n),n);
			}
			pptr=p;
		}
	}
	tptr=TPTR_i16;
	return (INT16*)ptr;
}

/**********************************************************/

DOUBLE *Samp::get_d( UINT len, BOOL reloadall )
{
	LONG nn=(LONG)len*sizeof(DOUBLE);
	if ((!ptr)||(nptr!=nn)) {
		nptr=nn; if (ptr) free(ptr); ptr=malloc(nptr); reloadall=TRUE;
	}
	if ((tptr!=TPTR_d)||(reloadall)) { pptr=getPos(); getBlk((DOUBLE*)ptr,len); }
	else {
		LONG p=getPos();
		if (p<pptr) {  // anyadir por delante
			LONG nn=pptr-p;
			UINT n=(UINT)nn;
			if (nn>=(LONG)len) n=len;
			else memmove((void *)((DOUBLE*)ptr+n),ptr,(len-n)*sizeof(DOUBLE));
			getBlk((DOUBLE*)ptr,n);
			pptr=p;
		}
		else if (p>pptr) {  // anyadir por detras
			LONG nn=p-pptr;
			UINT n=(UINT)nn;
			if (nn>=(LONG)len) getBlk((DOUBLE*)ptr,len);
			else {
				memmove(ptr,(void*)((DOUBLE*)ptr+n),(len-n)*sizeof(DOUBLE));
				setPos(p+(len-n));
				getBlk((DOUBLE*)ptr+(len-n),n);
			}
			pptr=p;
		}
	}
	tptr=TPTR_d;
	return (DOUBLE*)ptr;
}

/**********************************************************/

INT16 *Samp::getChAll_i16( UINT len, BOOL reloadall )
{
	LONG nn=(LONG)len*nchan*sizeof(INT16);
	if ((!ptr)||(nptr!=nn)) {
		nptr=nn; if (ptr) free(ptr); ptr=malloc(nptr); reloadall=TRUE;
	}
	if ((tptr!=TPTR_i16ChAll)||(reloadall)) { pptr=getPos(); getBlkChAll((INT16*)ptr,len); }
	else {
		LONG p=getPos();
		if (p<pptr) {  // anyadir por delante
			LONG nn=pptr-p;
			UINT n=(UINT)nn;
			if (nn>=(LONG)len) n=len;
			else memmove((void *)((INT16*)ptr+n*nchan),ptr,(len-n)*sizeof(INT16)*nchan);
			getBlkChAll((INT16*)ptr,n);
			pptr=p;
		}
		else if (p>pptr) {  // anyadir por detras
			LONG nn=p-pptr;
			UINT n=(UINT)nn;
			if (nn>=(LONG)len) getBlkChAll((INT16*)ptr,len);
			else {
				memmove(ptr,(void*)((INT16*)ptr+n*nchan),(len-n)*sizeof(INT16)*nchan);
				setPos(p+(len-n));
				getBlkChAll((INT16*)ptr+(len-n)*nchan,n);
			}
			pptr=p;
		}
	}
	tptr=TPTR_i16ChAll;
	return (INT16*)ptr;
}

/**********************************************************/

DOUBLE *Samp::getChAll_d( UINT len, BOOL reloadall )
{
	LONG nn=(LONG)len*nchan*sizeof(DOUBLE);
	if ((!ptr)||(nptr!=nn)) {
		nptr=nn; if (ptr) free(ptr); ptr=malloc(nptr); reloadall=TRUE;
	}
	if ((tptr!=TPTR_dChAll)||(reloadall)) { pptr=getPos(); getBlkChAll((DOUBLE*)ptr,len); }
	else {
		LONG p=getPos();
		if (p<pptr) {  // anyadir por delante
			LONG nn=pptr-p;
			UINT n=(UINT)nn;
			if (nn>=(LONG)len) n=len;
			else memmove((void *)((DOUBLE*)ptr+n*nchan),ptr,(len-n)*sizeof(DOUBLE)*nchan);
			getBlkChAll((DOUBLE*)ptr,n);
			pptr=p;
		}
		else if (p>pptr) {  // anyadir por detras
			LONG nn=p-pptr;
			UINT n=(UINT)nn;
			if (nn>=(LONG)len) getBlkChAll((DOUBLE*)ptr,len);
			else {
				memmove(ptr,(void*)((DOUBLE*)ptr+n*nchan),(len-n)*sizeof(DOUBLE)*nchan);
				setPos(p+(len-n));
				getBlkChAll((DOUBLE*)ptr+(len-n)*nchan,n);
			}
			pptr=p;
		}
	}
	tptr=TPTR_dChAll;
	return (DOUBLE*)ptr;
}

/**********************************************************/

BOOL Samp::OK( VOID ) const
{
	if (!FBlock::OK()) return FALSE;
//	if (testblk(ptr)) return FALSE;

	return TRUE;
}

/**********************************************************/

Samp::~Samp( VOID )
{
	if (ptr) free(ptr);
	ptr=0; nptr=-1;
};

/**********************************************************/

PRIVATE class { public: INT i; const CHAR *a; } allSampTypes[]= {
	{ SAMPTYPE_PCM8U,    SAMPTYPE_STR_PCM8U },
	{ SAMPTYPE_PCM16,   SAMPTYPE_STR_PCM16 },
	{ SAMPTYPE_PCM32,   SAMPTYPE_STR_PCM32 },
	{ SAMPTYPE_FLOAT32, SAMPTYPE_STR_FLOAT32 },
	{ SAMPTYPE_FLOAT64, SAMPTYPE_STR_FLOAT64 },
	{ -1, NULL }
};

/**********************************************************/
/* {devuelve} el tipo de muestra numerico partiendo del
nombre alfanumerico {sampType} del tipo de muestra.
{devuelve} -1 si {sampType} es una cadena invalida (
desconocida o NULL). */

INT SampType_a2i( const CHAR* sampType, BOOL errexit )
{
	if (sampType) {
		INT i=0;
		while (allSampTypes[i].i>=0) {
			if (!strcmp(allSampTypes[i].a,sampType))
				return allSampTypes[i].i;
			i++;
		}
	}
	if (errexit) die_beep("SampType: invalid sample type (%s)",sampType);
	return -1;
}

/**********************************************************/
/* {devuelve} el tipo de muestra alfanumerico (cadena de texto)
partiendo del codigo numerico {sampType} del tipo de muestra.
{devuelve} NULL si {sampType} es un codigo invalido */

const CHAR* SampType_i2a( INT sampType, BOOL errexit )
{
	if (sampType) {
		INT i=0;
		while (allSampTypes[i].i>=0) {
			if (allSampTypes[i].i==sampType)
				return allSampTypes[i].a;
			i++;
		}
	}
	if (errexit) die_beep("SampType: invalid sample type (%d)",sampType);
	return NULL;
}

/**********************************************************/

UINT Samp::getNoCh( INT16& sample )
{
	UINT8 u8;
	FLOAT32 f32;
	FLOAT64 f64;
	UINT ret;

	switch (tSamp) {
	case SAMPTYPE_PCM8U:
		ret=FBlock::get((BYTE*)&u8,sSize)/sSize;
		sample = (((INT16)u8)-128)<<8;
		return ret;
	case SAMPTYPE_PCM16:
		ret=FBlock::get((BYTE*)&sample,sSize)/sSize;
		if (endian>0) endian_fromlittle16(&sample); else if (endian<0) endian_frombig16(&sample);
		return ret;
	case SAMPTYPE_FLOAT32:
		ret=FBlock::get((BYTE*)&f32,sSize)/sSize;
		if (endian>0) endian_fromlittle32(&f32); else if (endian<0) endian_frombig32(&f32);
		sample = sunnorm_i16(f32);
		return ret;
	case SAMPTYPE_FLOAT64:
		ret=FBlock::get((BYTE*)&f64,sSize)/sSize;
		if (endian>0) endian_fromlittle64(&f64); else if (endian<0) endian_frombig64(&f64);
		sample = sunnorm_i16(f64);
		return ret;
	}

	die_beep("Samp: invalid SampType (%ld)",(long)tSamp);
	return 0;
}

/**********************************************************/

UINT Samp::getNoCh( DOUBLE& sample )
{
	UINT8 u8;
	INT16 i16;
	FLOAT32 f32;
	FLOAT64 f64;
	UINT ret;

	switch (tSamp) {
	case SAMPTYPE_PCM8U:
		ret = FBlock::get((BYTE*)&u8,sSize)/sSize;
		sample = norm_i16((((INT16)u8)-128)<<8);
		return ret;
	case SAMPTYPE_PCM16:
		ret = FBlock::get((BYTE*)&i16,sSize)/sSize;
		if (endian>0) endian_fromlittle16(&i16); else if (endian<0) endian_frombig16(&i16);
		sample = norm_i16(i16);
		return ret;
	case SAMPTYPE_FLOAT32:
		ret=FBlock::get((BYTE*)&f32,sSize)/sSize;
		if (endian>0) endian_fromlittle32(&f32); else if (endian<0) endian_frombig32(&f32);
		sample = f32;
		return ret;
	case SAMPTYPE_FLOAT64:
		ret=FBlock::get((BYTE*)&f64,sSize)/sSize;
		if (endian>0) endian_fromlittle64(&f64); else if (endian<0) endian_frombig64(&f64);
		sample=f64;
		return ret;
	}

	die_beep("Samp: invalid SampType (%li)",(long)tSamp);
	return 0;
}

/**********************************************************/

UINT Samp::insNoCh( INT16 sample )
{
	UINT8 u8;
	FLOAT32 f32;
	FLOAT64 f64;
	UINT ret;

#ifdef FLUSH_WHEN_WRITE
	nptr=-1;  // force block cache flush (in get_d get_i16)
#endif
	switch (tSamp) {
	case SAMPTYPE_PCM8U:
		u8 = (sample>>8)+128;
		ret=FBlock::ins((BYTE*)&u8,sSize)/sSize;
		return ret;
	case SAMPTYPE_PCM16:
		if (endian>0) endian_tolittle16(&sample); else if (endian<0) endian_tobig16(&sample);
		return FBlock::ins((BYTE*)&sample,sSize)/sSize;
	case SAMPTYPE_FLOAT32:
		f32 = norm_i16(sample);
		if (endian>0) endian_tolittle32(&f32); else if (endian<0) endian_tobig32(&f32);
		ret=FBlock::ins((BYTE*)&f32,sSize)/sSize;
		return ret;
	case SAMPTYPE_FLOAT64:
		f64 = norm_i16(sample);
		if (endian>0) endian_tolittle64(&f64); else if (endian<0) endian_tobig64(&f64);
		ret=FBlock::ins((BYTE*)&f64,sSize)/sSize;
		return ret;
	}

	die_beep("Samp: invalid SampType (%li)",(long)tSamp);
	return 0;
}

/**********************************************************/

UINT Samp::insNoCh( DOUBLE sample )
{
	UINT8 u8;
	INT16 i16;
	FLOAT32 f32;
	FLOAT64 f64;
	UINT ret;

#ifdef FLUSH_WHEN_WRITE
	nptr=-1;  // force block cache flush (in get_d get_i16)
#endif
	switch (tSamp) {
	case SAMPTYPE_PCM8U:
		u8 = (sunnorm_i16(sample)>>8)+128;
		ret=FBlock::ins((BYTE*)&u8,sSize)/sSize;
		return ret;
	case SAMPTYPE_PCM16:
		i16 = sunnorm_i16(sample);
		if (endian>0) endian_tolittle16(&i16); else if (endian<0) endian_tobig16(&i16);
		ret = FBlock::ins((BYTE*)&i16,sSize)/sSize;
		return ret;
	case SAMPTYPE_FLOAT32:
		f32 = sample;
		if (endian>0) endian_tolittle32(&f32); else if (endian<0) endian_tobig32(&f32);
		ret=FBlock::ins((BYTE*)&f32,sSize)/sSize;
		return ret;
	case SAMPTYPE_FLOAT64:
		f64 = sample;
		if (endian>0) endian_tolittle64(&f64); else if (endian<0) endian_tobig64(&f64);
		ret=FBlock::ins((BYTE*)&f64,sSize)/sSize;
		return ret;
	}

	die_beep("Samp: invalid SampType (%li)",(long)tSamp);
	return 0;
}

/**********************************************************/

UINT Samp::setNoCh( INT16 sample )
{
	UINT8 u8;
	FLOAT32 f32;
	FLOAT64 f64;
	UINT ret;

#ifdef FLUSH_WHEN_WRITE
	nptr=-1;  // force block cache flush (in get_d get_i16)
#endif
	switch (tSamp) {
	case SAMPTYPE_PCM8U:
		u8 = (sample>>8)+128;
		ret=FBlock::set((BYTE*)&u8,sSize)/sSize;
		return ret;
	case SAMPTYPE_PCM16:
		if (endian>0) endian_tolittle16(&sample); else if (endian<0) endian_tobig16(&sample);
		return FBlock::set((BYTE*)&sample,sSize)/sSize;
	case SAMPTYPE_FLOAT32:
		f32 = norm_i16(sample);
		if (endian>0) endian_tolittle32(&f32); else if (endian<0) endian_tobig32(&f32);
		ret=FBlock::set((BYTE*)&f32,sSize)/sSize;
		return ret;
	case SAMPTYPE_FLOAT64:
		f64 = norm_i16(sample);
		if (endian>0) endian_tolittle64(&f64); else if (endian<0) endian_tobig64(&f64);
		ret=FBlock::set((BYTE*)&f64,sSize)/sSize;
		return ret;
	}

	die_beep("Samp: invalid SampType (%li)",(long)tSamp);
	return 0;
}

/**********************************************************/

UINT Samp::setNoCh( DOUBLE sample )
{
	UINT8 u8;
	INT16 i16;
	FLOAT32 f32;
	FLOAT64 f64;
	UINT ret;

#ifdef FLUSH_WHEN_WRITE
	nptr=-1;  // force block cache flush (in get_d get_i16)
#endif
	switch (tSamp) {
	case SAMPTYPE_PCM8U:
		u8= (sunnorm_i16(sample)>>8) + 128;
		ret = FBlock::set((BYTE*)&u8,sSize)/sSize;
		return ret;
	case SAMPTYPE_PCM16:
		i16 = sunnorm_i16(sample);
		if (endian>0) endian_tolittle16(&i16); else if (endian<0) endian_tobig16(&i16);
		ret = FBlock::set((BYTE*)&i16,sSize)/sSize;
		return ret;
	case SAMPTYPE_FLOAT32:
		f32 = sample;
		if (endian>0) endian_tolittle32(&f32); else if (endian<0) endian_tobig32(&f32);
		ret=FBlock::set((BYTE*)&f32,sSize)/sSize;
		return ret;
	case SAMPTYPE_FLOAT64:
		f64 = sample;
		if (endian>0) endian_tolittle64(&f64); else if (endian<0) endian_tobig64(&f64);
		ret=FBlock::set((BYTE*)&f64,sSize)/sSize;
		return ret;
	}

	die_beep("Samp: invalid SampType (%li)",(long)tSamp);
	return 0;
}

/**********************************************************/

UINT Samp::getBlkNoCh( INT16* block, UINT len )
{
#ifdef __LITTLE_ENDIAN__
	if ((tSamp==SAMPTYPE_PCM16)&&(endian>=0))
		return FBlock::get((BYTE*)block,len*sSize)/sSize;
#endif
#ifdef __BIG_ENDIAN__
	if ((tSamp==SAMPTYPE_PCM16)&&(endian<=0))
		return FBlock::get((BYTE*)block,len*sSize)/sSize;
#endif


	UINT n = 0;
	while (len--) n+=getNoCh(*(block++));
	return n;
}

/**********************************************************/

UINT Samp::getBlkNoCh( DOUBLE* block, UINT len )
{
#if (SIZEOF_DOUBLE==8)
#ifdef __LITTLE_ENDIAN__
	if ((tSamp==SAMPTYPE_FLOAT64)&&(endian>=0))
		return FBlock::get((BYTE*)block,len*sSize)/sSize;
#endif
#ifdef __BIG_ENDIAN__
	if ((tSamp==SAMPTYPE_FLOAT64)&&(endian<=0))
		return FBlock::get((BYTE*)block,len*sSize)/sSize;
#endif
#endif

#if (SIZEOF_DOUBLE==4)
#ifdef __LITTLE_ENDIAN__
	if ((tSamp==SAMPTYPE_FLOAT32)&&(endian>=0))
		return FBlock::get((BYTE*)block,len*sSize)/sSize;
#endif
#ifdef __BIG_ENDIAN__
	if ((tSamp==SAMPTYPE_FLOAT32)&&(endian<=0))
		return FBlock::get((BYTE*)block,len*sSize)/sSize;
#endif
#endif


	UINT n = 0;
	while (len--) n+=getNoCh(*(block++));
	return n;
}

/**********************************************************/

UINT Samp::setBlkNoCh( const INT16* block, UINT len )
{
#ifdef FLUSH_WHEN_WRITE
	nptr=-1;  // force block cache flush (in get_d get_i16)
#endif

#ifdef __LITTLE_ENDIAN__
	if ((tSamp==SAMPTYPE_PCM16)&&(endian>=0))
		return FBlock::set((BYTE*)block,len*sSize)/sSize;
#endif
#ifdef __BIG_ENDIAN__
	if ((tSamp==SAMPTYPE_PCM16)&&(endian<=0))
		return FBlock::set((BYTE*)block,len*sSize)/sSize;
#endif

	if (!block) return FBlock::set(NULL,len*sSize)/sSize;

	UINT n = 0;
	while (len--) n+=setNoCh(*(block++));
	return n;
}

/**********************************************************/

UINT Samp::setBlkNoCh( const DOUBLE* block, UINT len )
{
#ifdef FLUSH_WHEN_WRITE
	nptr=-1;  // force block cache flush (in get_d get_i16)
#endif

#if (SIZEOF_DOUBLE==8)
#ifdef __LITTLE_ENDIAN__
	if ((tSamp==SAMPTYPE_FLOAT64)&&(endian>=0))
		return FBlock::set((BYTE*)block,len*sSize)/sSize;
#endif
#ifdef __BIG_ENDIAN__
	if ((tSamp==SAMPTYPE_FLOAT64)&&(endian<=0))
		return FBlock::set((BYTE*)block,len*sSize)/sSize;
#endif
#endif

#if (SIZEOF_DOUBLE==4)
#ifdef __LITTLE_ENDIAN__
	if ((tSamp==SAMPTYPE_FLOAT32)&&(endian>=0))
		return FBlock::set((BYTE*)block,len*sSize)/sSize;
#endif
#ifdef __BIG_ENDIAN__
	if ((tSamp==SAMPTYPE_FLOAT32)&&(endian<=0))
		return FBlock::set((BYTE*)block,len*sSize)/sSize;
#endif
#endif

	if (!block) return FBlock::set(NULL,len*sSize)/sSize;

	UINT n = 0;
	while (len--) n+=setNoCh(*(block++));
	return n;
}

/**********************************************************/

UINT Samp::insBlkNoCh( const INT16* block, UINT len )
{
#ifdef FLUSH_WHEN_WRITE
	nptr=-1;  // force block cache flush (in get_d get_i16)
#endif

#ifdef __LITTLE_ENDIAN__
	if ((tSamp==SAMPTYPE_PCM16)&&(endian>=0))
		return FBlock::ins((BYTE*)block,len*sSize)/sSize;
#endif
#ifdef __BIG_ENDIAN__
	if ((tSamp==SAMPTYPE_PCM16)&&(endian<=0))
		return FBlock::ins((BYTE*)block,len*sSize)/sSize;
#endif

	if (!block) return FBlock::ins(NULL,len*sSize)/sSize;

	UINT n = 0;
	while (len--)
		n+=insNoCh(*(block++));
	return n;
}

/**********************************************************/

UINT Samp::insBlkNoCh( const DOUBLE* block, UINT len )
{
#ifdef FLUSH_WHEN_WRITE
	nptr=-1;  // force block cache flush (in get_d get_i16)
#endif

#if (SIZEOF_DOUBLE==8)
#ifdef __LITTLE_ENDIAN__
	if ((tSamp==SAMPTYPE_FLOAT64)&&(endian>=0))
		return FBlock::ins((BYTE*)block,len*sSize)/sSize;
#endif
#ifdef __BIG_ENDIAN__
	if ((tSamp==SAMPTYPE_FLOAT64)&&(endian<=0))
		return FBlock::ins((BYTE*)block,len*sSize)/sSize;
#endif
#endif

#if (SIZEOF_DOUBLE==4)
#ifdef __LITTLE_ENDIAN__
	if ((tSamp==SAMPTYPE_FLOAT32)&&(endian>=0))
		return FBlock::ins((BYTE*)block,len*sSize)/sSize;
#endif
#ifdef __BIG_ENDIAN__
	if ((tSamp==SAMPTYPE_FLOAT32)&&(endian<=0))
		return FBlock::ins((BYTE*)block,len*sSize)/sSize;
#endif
#endif


	if (!block) return FBlock::ins(NULL,len*sSize)/sSize;

	UINT n = 0;
	while (len--) n+=insNoCh(*(block++));
	return n;
}

/**********************************************************/

