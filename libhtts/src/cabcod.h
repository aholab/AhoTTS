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
#ifndef __CABCOD_H
#define __CABCOD_H


/************************************/
/*   1992 (c) Grupo de Voz - DAET   */
/*         ETSII/IT Bilbao          */
/************************************/


/**********************************************************/
/**********************************************************/
/* C¢digos de las constantes utilizados en las cabeceras  */
/**********************************************************/
/* Tipos de fichero */

#define TF_SORIG 0        /* <100 ==> vector unidimensional (se¤ales) */
#define TF_SENF 5
#define TF_SDENF 7
#define TF_SERR 10
#define TF_SLPC 20
#define TF_SFOR 25
#define TF_FILT 30
#define TF_SVUS 40

#define TF_LPC 100         /* >=100 ==> vector multidimensional (parametros) */
#define TF_PARCOR 110
#define TF_ESPEC 120
#define TF_PITCH 130
#define TF_ENERG 140
#define TF_CPCERO 150

#define es_fsenal(tf) (tf<100)

/**********************************************************/
/* Tipos de datos */

#define TD_INT2 0         /* short int (2bytes) */
#define TD_FLOAT4 10      /* short float (2bytes) */
#define TD_INT4 20        /* long int (4bytes) */
#define TD_FLOAT8 30      /* long float (8bytes) */

/**********************************************************/
/* Mascaras de cabeceras */

#define MC_SENAL  0x0001
#define MC_PARAM  0x0002
/* otras
#define MC_?????  0x0004
*/

#define MC_NINGUNA  0x0000
#define MC_TODAS  0xFFFF

/**********************************************************/
/* Tipos de ventanas */

#define TV_RECT 0
#define TV_HAMM 10
#define TV_HANN 20
#define TV_KAIS 30

/**********************************************************/
/* Metodo de calculo de LPC */

#define MLPC_CORR 0             /* Autocorrelaciones */
#define MLPC_COV 10             /* Covarianzas */

/**********************************************************/
/* G si o no incluida en vectores de LPCs */

#define G_NO 0
#define G_SI 1
#define G_SI_AB 2

/**********************************************************/
/* Metodo de calculo de Pitch */

#define MP_ACOR
#define MP_CC
#define MP_CC3N

/**********************************************************/

#endif

