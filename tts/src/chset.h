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
#ifndef __CHSET_H__
#define __CHSET_H__

/**********************************************************/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/*
(C) 1997 TSR/Aholab - ETSII/IT Bilbao (UPV/EHU)

Nombre fuente................ chset.h
Nombre paquete............... aHoTTS
Lenguaje fuente.............. C
Estado....................... -
Dependencia Hard/OS.......... pagina de codigos ISOLatin1
Codigo condicional........... -

Codificacion................. Borja Etxebarria
.............................

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
1.0.0    31/01/00  borja     codefreeze aHoTTS v1.0
0.0.1    23/12/99  borja     solo iso latin 1
0.0.0    06/05/95  borja     Codificacion inicial.

======================== Contenido ========================
<DOC>
Manejo paginas de codigos ISOLatin1
</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include <stdlib.h>
#include "tdef.h"

/******************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************/
/* Codigos SGML ISO 8859/1 Latin 1 /  Windows 3.1 Latin 1  */

#define CS_ISOLatin1_atilde '\341'
#define CS_ISOLatin1_etilde '\351'
#define CS_ISOLatin1_itilde '\355'
#define CS_ISOLatin1_otilde '\363'
#define CS_ISOLatin1_utilde '\372'
#define CS_ISOLatin1_uuml   '\374'
#define CS_ISOLatin1_ntilde '\361'
#define CS_ISOLatin1_Atilde '\301'
#define CS_ISOLatin1_Etilde '\311'
#define CS_ISOLatin1_Itilde '\315'
#define CS_ISOLatin1_Otilde '\323'
#define CS_ISOLatin1_Utilde '\332'
#define CS_ISOLatin1_Uuml   '\334'
#define CS_ISOLatin1_Ntilde '\321'
#define CS_ISOLatin1_iexcl  '\241'
#define CS_ISOLatin1_iquest '\277'
#define CS_ISOLatin1_ordf   '\252'
#define CS_ISOLatin1_ordm   '\272'
#define CS_ISOLatin1_deg    '\260'

#define CS_S_ISOLatin1_atilde "\341"
#define CS_S_ISOLatin1_etilde "\351"
#define CS_S_ISOLatin1_itilde "\355"
#define CS_S_ISOLatin1_otilde "\363"
#define CS_S_ISOLatin1_utilde "\372"
#define CS_S_ISOLatin1_uuml   "\374"
#define CS_S_ISOLatin1_ntilde "\361"
#define CS_S_ISOLatin1_Atilde "\301"
#define CS_S_ISOLatin1_Etilde "\311"
#define CS_S_ISOLatin1_Itilde "\315"
#define CS_S_ISOLatin1_Otilde "\323"
#define CS_S_ISOLatin1_Utilde "\332"
#define CS_S_ISOLatin1_Uuml   "\334"
#define CS_S_ISOLatin1_Ntilde "\321"
#define CS_S_ISOLatin1_iexcl  "\241"
#define CS_S_ISOLatin1_iquest "\277"
#define CS_S_ISOLatin1_ordf   "\252"
#define CS_S_ISOLatin1_ordm   "\272"
#define CS_S_ISOLatin1_deg    "\260"


#define CS_atilde CS_ISOLatin1_atilde
#define CS_etilde CS_ISOLatin1_etilde
#define CS_itilde CS_ISOLatin1_itilde
#define CS_otilde CS_ISOLatin1_otilde
#define CS_utilde CS_ISOLatin1_utilde
#define CS_uuml   CS_ISOLatin1_uuml
#define CS_ntilde CS_ISOLatin1_ntilde
#define CS_Atilde CS_ISOLatin1_Atilde
#define CS_Etilde CS_ISOLatin1_Etilde
#define CS_Itilde CS_ISOLatin1_Itilde
#define CS_Otilde CS_ISOLatin1_Otilde
#define CS_Utilde CS_ISOLatin1_Utilde
#define CS_Uuml   CS_ISOLatin1_Uuml
#define CS_Ntilde CS_ISOLatin1_Ntilde
#define CS_iexcl  CS_ISOLatin1_iexcl
#define CS_iquest CS_ISOLatin1_iquest
#define CS_ordf   CS_ISOLatin1_ordf
#define CS_ordm   CS_ISOLatin1_ordm
#define CS_deg    CS_ISOLatin1_deg

#define CS_S_atilde CS_S_ISOLatin1_atilde
#define CS_S_etilde CS_S_ISOLatin1_etilde
#define CS_S_itilde CS_S_ISOLatin1_itilde
#define CS_S_otilde CS_S_ISOLatin1_otilde
#define CS_S_utilde CS_S_ISOLatin1_utilde
#define CS_S_uuml   CS_S_ISOLatin1_uuml
#define CS_S_ntilde CS_S_ISOLatin1_ntilde
#define CS_S_Atilde CS_S_ISOLatin1_Atilde
#define CS_S_Etilde CS_S_ISOLatin1_Etilde
#define CS_S_Itilde CS_S_ISOLatin1_Itilde
#define CS_S_Otilde CS_S_ISOLatin1_Otilde
#define CS_S_Utilde CS_S_ISOLatin1_Utilde
#define CS_S_Uuml   CS_S_ISOLatin1_Uuml
#define CS_S_Ntilde CS_S_ISOLatin1_Ntilde
#define CS_S_iexcl  CS_S_ISOLatin1_iexcl
#define CS_S_iquest CS_S_ISOLatin1_iquest
#define CS_S_ordf   CS_S_ISOLatin1_ordf
#define CS_S_ordm   CS_S_ISOLatin1_ordm
#define CS_S_deg    CS_S_ISOLatin1_deg

/******************************************************************/

CHAR ISOLatin1_ToLower(CHAR ch);
size_t ISOLatin1_StrLower(CHAR *str);
BOOL ISOLatin1_IsLower(CHAR ch);

CHAR ISOLatin1_ToUpper(CHAR ch);
size_t ISOLatin1_StrUpper(CHAR *str);
BOOL ISOLatin1_IsUpper(CHAR ch);

BOOL ISOLatin1_IsDigit(CHAR ch);

/******************************************************************/

#define chset_ToLower(ch) ISOLatin1_ToLower(ch)
#define chset_StrLower(str) ISOLatin1_StrLower(str)
#define chset_IsLower(ch) ISOLatin1_IsLower(ch)

#define chset_ToUpper(ch) ISOLatin1_ToUpper(ch)
#define chset_StrUpper(ch) ISOLatin1_StrUpper(ch)
#define chset_IsUpper(ch) ISOLatin1_IsUpper(ch)

#define chset_IsDigit(ch) ISOLatin1_IsDigit(ch)

/******************************************************************/

#ifdef __cplusplus
}
#endif

/******************************************************************/

#endif

