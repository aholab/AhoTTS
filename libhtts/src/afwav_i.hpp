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
#ifndef __AFWAV_I_HPP__
#define __AFWAV_I_HPP__

/**********************************************************/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/*
Copyright: 1996 - Grupo de Voz (DAET) ETSII/IT-Bilbao
                  & CSTR - Univ. of Edinburgh

Nombre fuente................ -
Nombre paquete............... -
Lenguaje fuente.............. C++ (Borland C/C++ 3.1)
Estado....................... desarrollo
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Borja Etxebarria

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
0.0.3    2012	   Iñaki     eliminar MINALIGN innecesarios
0.0.2    30/08/98  Borja     split en varios modulos afwav_?.cpp
0.0.0    15/07/96  Borja     codificacion inicial.

======================== Contenido ========================
ver afwav.cpp
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include "tdef.h"
#include "caudiox.hpp"
#include "uti.h"

/**********************************************************/

#ifdef __CC_MSVC__
#pragma pack(push,1)
#elif __CC_BORLANDC__
typedef struct {CHAR8 x; INT2 y; } ____tmp_tdef_align_test___;
#if sizeof(____tmp_tdef_align_test___)>3
#error You must disable Word Alignment
#endif
#endif

/**********************************************************/
/* Definir IGNORE_CUE_LABEL para ignorar los labels de los
cue points. Se usan solo los cue-notes como etiquetas para
las marcas */

#define IGNORE_CUE_LABEL

/**********************************************************/

typedef struct {
	CHAR8 chunkName [4];
	UINT32 len MINALIGN;
} WAVChunk;

typedef struct {
	CHAR8 Literal_RIFF[4];
	UINT32 len MINALIGN;
	CHAR8 Literal_WAVE[4];
} WAVHdr ;

#define WFORMAT_PCM 0x0001

typedef struct {
	UINT16 wFormatTag MINALIGN;       // Format category
	UINT16 wChannels MINALIGN;        // Number of channels
	UINT32 dwSamplesPerSec MINALIGN;  // Sampling rate
	UINT32 dwAvgBytesPerSec MINALIGN; // For buffer estimation
	UINT16 wBlockAlign MINALIGN;      // Data block size
} FormatInfo;

typedef struct {
	UINT32 dwName MINALIGN;
	UINT32 dwPosition MINALIGN;
	char fccChunk[4];
	UINT32 dwChunkStart MINALIGN;
	UINT32 dwBlockStart MINALIGN;
	UINT32 dwSampleOffset MINALIGN;
} CuePoint;


class UInt32 {
	public: UINT32 i;
	public:
		UInt32( ) {};
		UInt32( UINT32 n ): i(n) {};
		UInt32( const UInt32 &n ): i(n.i) {};
		UInt32& operator = (const UInt32 & a) { i=a.i; return *this; };
};

int operator < (const UInt32& a, const UInt32& b);
int operator == (const UInt32& a, const UInt32& b);
int operator > (const UInt32& a, const UInt32& b);

typedef SetListT<UInt32> CueNames;

/**********************************************************/

char *wstr( FILE *f );
char *wstr( FILE *f, UINT32 l );
VOID mrk_labl(Mark1DList &m, CueNames &cn, CHAR8 *buf);
VOID mrk_note(Mark1DList &m, CueNames &cn, CHAR8 *buf);
VOID mrk_ltxt(Mark1DList &m, CueNames &cn, CHAR8 *buf);

size_t strlen_zeven( const char *s );
void puts_zeven( const char * s, FILE *f );

/**********************************************************/

#ifdef __CC_MSVC__
#pragma pack(pop)
#endif

/**********************************************************/

#endif
