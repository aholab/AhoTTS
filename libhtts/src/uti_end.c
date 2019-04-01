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

Nombre fuente................ UTI_END.C
Nombre paquete............... UTI.H
Lenguaje fuente.............. C (BC31,GCC)
Estado....................... Utilizable
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Borja Etxebarria

Version  dd/mm/aa  Autor     Comentario
-------  --------  --------  ----------
1.0.0    22/07/97  Borja     initial

======================== Contenido ========================
Big endian / little endian conversion
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include "uti.h"

VOID reverseendian( VOID *buff, int elemsize, int count )

{

      int k;

      BYTE *aux1, *aux2, *aux3, temp;

      for (aux3=(BYTE *)buff, k=0; k<count; k++) {

            aux1 = aux3;

            aux3 += elemsize;

            aux2 = aux3-1;

            while (aux2 > aux1) {

                  temp = *aux1;

                  *aux1 = *aux2;

                  *aux2 = temp;

                  aux1++;

                  aux2--;

            }

      }

}

 

 

/**********************************************************/

 

VOID endian_swap16( VOID * word16 )

{

      reverseendian(word16,2,1);

}

 

/**********************************************************/

 

VOID endian_swap32( VOID * word32 )

{

      reverseendian(word32,4,1);

}

 

/**********************************************************/

 

VOID endian_swap64( VOID * word64 )

{

      reverseendian(word64,8,1);

}

 

/**********************************************************/

 

VOID endian_swap16_n( VOID * word16, int n )

{

      reverseendian(word16,2,n);

}

 

/**********************************************************/

 

VOID endian_swap32_n( VOID * word32, int n )

{

      reverseendian(word32,4,n);

}

 

/**********************************************************/

 

VOID endian_swap64_n( VOID * word64, int n )

{

      reverseendian(word64,8,n);

}

 

/**********************************************************/