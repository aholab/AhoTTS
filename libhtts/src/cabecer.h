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
#ifndef __CABECER_H
#define __CABECER_H


/************************************/
/*   1992 (c) Grupo de Voz - DAET   */
/*         ETSII/IT Bilbao          */
/************************************/


/**********************************************************/

#include <stdio.h>

#include "cabdef0.h"
#include "cabdef1.h"

/**********************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************/

void vleecfs( FILE * fp, struct cgsenal * cgs );

void vescribecfs( FILE * fp, struct cgsenal * cgs );

UCHAR1 leecfs( FILE * fp, struct cgsenal * cgs, int tdato );

void escribecfs_ftyp( FILE * fp,
                      struct cgsenal * cgs,
                      UINT2 ftyp,
                      UCHAR1 tdata );

void escribecfs( FILE * fp,
                 struct cgsenal * cgs,
                 UCHAR1 tdata );

void escribe_cab_preenf( FILE * fp,
                         struct cgsenal * cgs,
                         UCHAR1 tdata );

void escribe_cab_deenf( FILE * fp,
                        struct cgsenal * cgs,
                        UCHAR1 tdata );

void escribe_cab_filt( FILE * fp,
                       struct cgsenal * cgs,
                       UCHAR1 tdata );

void escribe_cab_parcor( FILE * fp,
                         struct cgsenal * cgs,
                         struct cgp_parcor * cgpp );

void escribecfslpc( FILE * fp,
                    struct cgsenal * cgs,
                    struct cgp_lpc * cpl,
                    UINT2 ftyp,
                    UINT2 tdat );

UINT2 leecfslpc( FILE * fp,
                struct cgsenal * cgs,
                struct cgp_lpc * cpl,
                int tdato,
                BOOL * hayclpc );

void escribe_cab_lpc( FILE * fp,
                      struct cgsenal * cgs,
                      struct cgp_lpc * cgpl );

void lee_cab_lpc( FILE * fp,
                  struct cgp_lpc * cgpl );

void escribe_cab_err( FILE * fp,
                      struct cgsenal * cgs,
                      struct cgp_lpc * cgpl,
                      UCHAR1 tdata );

void escribe_cab_lpcsint( FILE * fp,
                          struct cgsenal * cgs,
                          struct cgp_lpc * cgpl,
                          UCHAR1 tdata );

void escribecfs_form( FILE * fp,
                      struct cgsenal * cgs,
                      int espera_en_marcas );

/**********************************************************/

#ifdef __cplusplus
}  /* extern "C" */
#endif

/**********************************************************/

#endif

