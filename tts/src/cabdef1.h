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
#ifndef __CABDEF1_H
#define __CABDEF1_H


/************************************/
/*   1992 (c) Grupo de Voz - DAET   */
/*         ETSII/IT Bilbao          */
/************************************/
//Inaki 2012 eliminar MINALIGN innecesarios

/**********************************************************/

#include "cabdef0.h"

/**********************************************************/

#ifdef __CC_MSVC__
#pragma pack(push,1)
#elif __CC_BORLANDC__
typedef struct {CHAR8 x; INT2 y; } ____tmp_cabdef1_align_test___;
#if sizeof(____tmp_cabdef1_align_test___)>3
#error You must disable Word Alignment
#endif
#endif

/**********************************************************/
/*cabecera general de parametros de ficheros de espectrogramas */

struct cgp_espect {
       struct cgparam  cgp;
       FLOAT4  preenf;
       };

/**********************************************************/
/*cabecera general de parametros de ficheros lpc */

struct cgp_lpc {
	   struct cgparam  cgp;
	   FLOAT4  preenf MINALIGN;
	   UINT2  orden MINALIGN;
	   UCHAR1  ginc;              /* g incluida */
	   UCHAR1  mlpc;              /* metodo calculo */
	   };

/**********************************************************/
/*cabecera general de parametros de ficheros parcor */

struct cgp_parcor {
	   struct cgparam  cgp;
	   FLOAT4  preenf MINALIGN;
	   UINT2  orden MINALIGN;
	   UCHAR1  ginc;              /* g incluida */
};

/**********************************************************/

#ifdef __CC_MSVC__
#pragma pack(pop)
#endif

/**********************************************************/

#endif
