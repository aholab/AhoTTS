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
Copyright: 1998 - TSR/Aholab (DET) ETSII/IT-Bilbao

Nombre fuente................ roman.c
Nombre paquete............... normalizador
Lenguaje fuente.............. C++
Estado....................... desarrollo
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Juan Luis Murugarren

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
0.0.2    02/05/01  Yon2.     Lost <string.h>
0.0.1    08/04/01  Yon2.     Avoid Warnings.
0.0.0    28/11/00  Yon2.     Codificacion inicial.

======================== Contenido ========================

  Se da en este m¢dulo soporte para n£meros Romanos. Se da en
base a dos funciones:
  - isRoman() -> que nos dice si un n£mero es romano o no.
  - romanStr2num -> que convierte una cadena de caracteres
romanos en un n£mero decimal.
  La mayor complejidad se encuentra en saber si un n£mero es
romano. Sabido esto convertir el n£mero a decimal es
simplic¡simo.

NOTA: Actualmente, operan ambas funciones est‚ la cadena
formada por letras en mayusculas, minusculas o combinaci¢n
de ambas.

!!!!!!!!!!!!!!!!!
NOTA: Verificar que no haya caracteres en may£culas y
minusculas a la vez ya que en este caso no es un n£mero
romano. O incluso que no aparezcan en min£sculas.
!!!!!!!!!!!!!!!!!

Yon2.
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/

/**********************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "chset.h"

#include "roman.h"

/**********************************************************/

UINT romanValue[]={0, 1, 5, 10, 50, 100, 500, 1000};


/**********************************************************/


/**********************************************************/

UINT rom2ind(CHAR ch)
{
 UINT value;

 switch (ch)  {
   case 'I':
        value = ROMAN_I;
        break;
   case 'V':
        value = ROMAN_V;
        break;
   case 'X':
        value = ROMAN_X;
        break;
   case 'L':
        value = ROMAN_L;
        break;
   case 'C':
        value = ROMAN_C;
        break;
   case 'D':
        value = ROMAN_D;
        break;
   case 'M':
        value = ROMAN_M;
        break;
   default:
        // !!!!!!!! Error informar HTTS
        value = ROMAN_NONUM;
        break;
   }


 return value;
}

/**********************************************************/


/**********************************************************/

UINT rom2num(CHAR ch)
{
 return romanValue[rom2ind(ch)];
}

/**********************************************************/


/**********************************************************/

BOOL isRoman(pCHAR str)
{
  INT len, i;
  pCHAR roman;
  UINT current, prev;
  BOOL validRoman = TRUE;
  BOOL subsRoman[NUM_ROMAN];
  INT howmanytimes[NUM_ROMAN];
  INT howmanytimesSubs[NUM_ROMAN];
  INT count;

  roman = strdup(str);
  ISOLatin1_StrUpper(roman);

  len = strlen(roman);

  for(i=0; i < NUM_ROMAN; i++) {
     howmanytimes[i] = 0;
     howmanytimesSubs[i] = 0;
     subsRoman[i] = FALSE;
  }


  //I, X, C, M Seguidos como m ximo hasta tres veces.
  count = 0;
  prev = rom2ind(roman[0]);
  for (i = 1; i < len; i++) {
     current = rom2ind(roman[i]);
     if (current == prev) count++;
     else count = 0;
     if (count == 3) {
       free(roman);
       return FALSE;
     }
  }

  prev = rom2ind(roman[0]);
  howmanytimes[prev]++;
  for (i = 1; i < len; i++) {
     current = rom2ind(roman[i]);
     howmanytimes[current]++;
     if (prev < current) {
       validRoman = FALSE;
       switch(prev) {
         case ROMAN_I:
              if ((current == ROMAN_V) || ( current == ROMAN_X))
                  validRoman = TRUE;
                  subsRoman[ROMAN_I] = TRUE;
                  howmanytimesSubs[ROMAN_I]++;
              break;
         case ROMAN_X:
              if ((current == ROMAN_L) || ( current == ROMAN_C))
                  validRoman = TRUE;
                  subsRoman[ROMAN_X] = TRUE;
                  howmanytimesSubs[ROMAN_X]++;
            break;
         case ROMAN_C:
              if ((current == ROMAN_D) || ( current == ROMAN_M))
                  validRoman = TRUE;
                  subsRoman[ROMAN_C] = TRUE;
                  howmanytimesSubs[ROMAN_C]++;
            break;
         default:
            break;
       }
       if (!validRoman) break;
     }
     prev = current;

  }

  if (howmanytimes[ROMAN_NONUM]) validRoman = FALSE;

  if (subsRoman[ROMAN_I])  {
    if (howmanytimes[ROMAN_I] > 1) validRoman = FALSE;
  }
  else {
    if (howmanytimes[ROMAN_I] > 4) validRoman = FALSE;
  }

  if (howmanytimes[ROMAN_V] > 1) validRoman = FALSE;

  if (subsRoman[ROMAN_X])  {
    if (howmanytimesSubs[ROMAN_X] > 1) validRoman = FALSE;
    else if (howmanytimes[ROMAN_X] > 2) validRoman = FALSE;
    else if ((howmanytimes[ROMAN_X] == 2) && (!subsRoman[ROMAN_I])) validRoman = FALSE;
  }
  else {
    if (howmanytimes[ROMAN_X] > 4) validRoman = FALSE;
  }

  if (howmanytimes[ROMAN_L] > 1) validRoman = FALSE;

  if (subsRoman[ROMAN_C])  {
    if (howmanytimesSubs[ROMAN_C] > 1) validRoman = FALSE;
    else if (howmanytimes[ROMAN_C] > 2) validRoman = FALSE;
    else if ((howmanytimes[ROMAN_C] == 2) && (!subsRoman[ROMAN_X])) validRoman = FALSE;
  }
  else {
    if (howmanytimes[ROMAN_C] > 4) validRoman = FALSE;
  }

  if (howmanytimes[ROMAN_D] > 1) validRoman = FALSE;
  if (howmanytimes[ROMAN_M] > 3) validRoman = FALSE;


  free(roman);

  return validRoman;

}

/**********************************************************/


/**********************************************************/

UINT romanStr2num(pCHAR str)
{
  INT num, len;
  pCHAR roman;
  INT i;
  UINT current, next=0;

  roman = strdup(str);
  ISOLatin1_StrUpper(roman);

//  fprintf(stdout, "Las cadenas: orig: [%s] toupp: [%s]\n", str, roman);

  num = 0;
  len = strlen(roman);

  if (len)  {
    if (len > 1)  {
        for (i = 0; i < (len - 1); i++) {
            current = rom2num(roman[i]);
            next = rom2num(roman[i + 1]);
            if (current < next) num-=current;
            else num+=current;
        }
        num+=next;
    }
    else num = rom2num(roman[0]);
  }

  free(roman);

  return num;
}

/**********************************************************/

