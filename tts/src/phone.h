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
#ifndef __PHONE_H__
#define __PHONE_H__

/**********************************************************/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/*
(C) 1997 TSR/Aholab - ETSII/IT Bilbao (UPV/EHU)

Nombre fuente................ phone.h
Nombre paquete............... aHoTTS
Lenguaje fuente.............. C
Estado....................... -
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Borja Etxebarria
.............................

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
1.1.0    20/10/08  iñaki     Añadir fonemas Inglés
1.0.0    31/01/00  borja     codefreeze aHoTTS v1.0
0.0.0    24/11/97  borja     Codificacion inicial.

======================== Contenido ========================
<DOC>
SAMPA: son los codigos que usamos para los fonemas, aunque
se codifican como un unico caracter siguiendo esta tabla (para
representarlos como char).
Si hace falta, la conversion a un tipo mayor que el char deberia
ser inmediata. Para ello hay que tener cuidado de no considerarlos
nunca CHAR (no usar funciones de strings, etc).
</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/
#include "tdef.h"
#include "htts_cfg.h"

/******************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************/

typedef INT Phone;

/**********************************************************/

#define PH_none  ' '

#define PH_sil  '_'
#define PH_sic  '+'

#define PH_breath  '~'
#define PH_p  'p'
#define PH_b  'b'
#define PH_t  't'
#define PH_d  'd'
#define PH_k  'k'
#define PH_g  'g'
#define PH_m  'm'
#define PH_n  'n'
#define PH_J  'J'
#define PH_tS 'C'
#define PH_B  'B'
#define PH_f  'f'
#define PH_T  'T'
#define PH_D  'D'
#define PH_s  's'
#define PH_jj 'y'
#define PH_x  'x'
#define PH_G  'G'
#define PH_l  'l'
#define PH_L  'L'
#define PH_rr 'R'
#define PH_r  'r'
#define PH_i  'i'
#define PH_j  'j'
#define PH_e  'e'
#define PH_a  'a'
#define PH_o  'o'
#define PH_u  'u'
#define PH_w  'w'

#define PH_S  'S'
#define PH_ts 'V'
#define PH_dj 'K'

/* MARIA */

#define PH_v  'v'
#define PH_z  'z'
#define PH_Z  'Z'
#define PH_h  'h'
#define PH_N  'N'
#define PH_I  'I'
#define PH_IA 'Y'
#define PH_eA 'E'
#define PH_a_ '{'
#define PH_A_ 'A'
#define PH_V  'c'
#define PH_U  'U'
#define PH_UA '5'
#define PH_A  '@'
#define PH_i_ ':'
#define PH_eI 'q'
#define PH_aI 'H'
#define PH_OI 'O'
#define PH_AU 'M'
#define PH_aU 'W'
#define PH_3_ '3'
#define PH_O_ '0'
#define PH_Q_ '4'   
#define PH_dZ_ '2' 
#define PH_u_ '1'

/*unilex BLIZZARD2008*/
#define PH_nx '6'
#define PH_lw '7'
#define PH_uw '8'
#define PH_lx '9'
#define PH_mx '}'

/* * * * * * * */

/**********************************************************/
/* estos realmente no son sampa, pero han hecho falta para 
el euskera */

#define PH_X  'X'    /* para la z  */
#define PH_tZ 'P'    /* para la tz */
#define PH_Q  'Q'    /* para la tt */
//#define PH_dZ 'F'    /* para la dZ de Bizkaiera */ //BLIZZARD

/* La u afrancesada del dialecto iparralde que se marca con ü
 * Solo quedaba libre la letra 'F' (sampa y)*/
#define PH_y 'F' //iparralde

/**********************************************************/

const CHAR * phone_tosampa( Phone ph );
Phone phone_fromsampa( const CHAR *sampa );

/******************************************************************/

#ifdef __cplusplus
}  /* extern "C" */
#endif

/**********************************************************/

#endif

