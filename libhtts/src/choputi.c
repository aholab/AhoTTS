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

Nombre fuente................ choputi.c
Nombre paquete............... normalizador
Lenguaje fuente.............. C++
Estado....................... desarrollo
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Juan Luis Murugarren

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
0.0.7	 13/06/11  Inaki     casting en uniqVowel por comportamientos anómalos
0.0.6	 02/10/07  Inaki     Añadir u con dieresis
0.0.5    28/03/01  Yon2.     chu_str2num un pel¡n m s dura.
0.0.4    28/03/01  Yon2.     Quitar los tabuladores también.
0.0.3	 28/03/01  Yon2.     Quitar CR, LF y espacios del final.
0.0.2    12/03/01  Yon2.     Se mueve del speller wrdList.
0.0.1    13/12/00  Yon2.     Fix to iso charset
0.0.0    11/12/00  Yon2.     Codificacion inicial.

======================== Contenido ========================

  Utilidades varias comunes a los fuentes del normalizador.

  $$$ Str2num est  bastante floja, hay que hacerla mas dura.
  De momento el suficiente para fechas, horas y lapsos de tiempo.
  Pero hay que hacer que soporte numeros mas largos.

Yon2.
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/
 
/**********************************************************/

#include <stdio.h>
#include <string.h>

#include "tdef.h"
#include "uti.h"
#include "chset.h"
#include "chartype.h"
#include "httsmsg.h"

#include "choputi.h"

/**********************************************************/

CHAR vowelStr[] = "aeiouAEIOUáéíóúÁÉÍÓÚüÜ"; //INAKI: añadir üÜ
CHAR consoStr[] = "bcdfghjklmnñpqrstvwxyzBCDFGHJKLMNÑPQRSTVWXYZ";

/**********************************************************/

/**********************************************************/
UINT chu_str2num(pCHAR str)
{
	LONG tmpnum;
	
	if (strlen(str) > 4) {
#ifdef HTTS_DEBUG
       htts_warn("ChopUti::chu_str2num Try to convert a number with more than 4 digits.");
#endif
	  return 0;
	}
  tmpnum = xstr2li( str, 0 );

	return (tmpnum > 30000)?0:(UINT)tmpnum;
}

/**********************************************************/

/**********************************************************/

BOOL isVowel(CHAR ch)
{

  BOOL retval = FALSE;

  pCHAR where;

  where = strchr (vowelStr, (INT)ch);

  if (where != NULL) retval = TRUE;

  return retval;

}

/**********************************************************/


/**********************************************************/

BOOL isConso(CHAR ch)
{

  BOOL retval = FALSE;

  pCHAR where;

  where = strchr (consoStr, (INT)ch);

  if (where != NULL) retval = TRUE;

  return retval;

}

/**********************************************************/

/**********************************************************/

CHAR uniqVowel(CHAR ch)
{
  CHAR retval = 'X';
  switch  ((unsigned int)ch)  { //inaki (casting)
          case 'a':
          case 'A':
          case 'á':
          case 'Á':
               retval = 'a';
               break;
          case 'e':
          case 'E':
          case 'é':
          case 'É':
               retval = 'e';
               break;
          case 'i':
          case 'I':
          case 'í':
          case 'Í':
               retval = 'i';
               break;
          case 'o':
          case 'O':
          case 'ó':
          case 'Ó':
               retval = 'o';
               break;
          case 'u':
          case 'U':
          case 'ú':
          case 'Ú':
	  case 'ü':
	  case 'Ü':
               retval = 'u';
               break;
          default:
          	break;    
  }
  return retval;
}

/**********************************************************/



/**********************************************************/

CHAR uniqConso(CHAR ch)
{

  return ISOLatin1_ToLower(ch);
}

/**********************************************************/


/**********************************************************/
//Crea un array de cadenas con palabras que provienen de una
//cadena con multiples palabras. Hay que liberar toda la
//reserva hecha. Se hace con la funcion freewrsLst().
INT str2wrdLst(pCHAR srcStr, pCHAR *words[])
{
	pCHAR wrkStr;
	pCHAR tempStr;
	INT strl;
	INT i,j,k;
	INT count = 0;
	BOOL spacePrev = FALSE, space;

	wrkStr = strdup(srcStr);
	strl = strlen(wrkStr);
	tempStr = (pCHAR)malloc((strl + 1) * sizeof(CHAR));

	//filtramos CR, LF, seguramente con LF sería suficiente.
	//y tabuladores, Ahi!!!!!!!!!!!!!!!
	for (i=0; i < strl; i++)	{
		if (wrkStr[i] == '\x0a') wrkStr[i] = ' ';
		if (wrkStr[i] == '\x0d') wrkStr[i] = ' ';
		if (wrkStr[i] == '\x09') wrkStr[i] = ' ';
	}


	//quitamos los espacios al final.

	i=(strl - 1);
	while (wrkStr[i] == ' ') i--;
	i++;
	wrkStr[i] = '\x0'; //Final de la cadena.

	strl = strlen(wrkStr);

	//Calculamos los huecos entre palabras. Solo un flanco.
	
	for (i=0; i < strl; i++)	{
		space = (wrkStr[i] == ' ')?TRUE:FALSE;
		if (!spacePrev && space) count++;
		spacePrev = space;
	}
	
	//Siempre hay una palabra más que huecos.
	count++;

	*words = (pCHAR *)malloc(count * sizeof(pCHAR));

	//Buscamos las palabras.
	spacePrev = FALSE;
	j=k=0;
	for (i=0; i < strl; i++)	{
		space = (wrkStr[i] == ' ')?TRUE:FALSE;
		if (!space) tempStr[j++] = wrkStr[i];
		if (!spacePrev && space) {
			tempStr[j] = '\x0';
			j=0;
			(*words)[k++] = strdup(tempStr);
		}
		spacePrev = space;
	}
	//La última palabra.
	tempStr[j] = '\x0';
	(*words)[k] = strdup(tempStr);



	free(tempStr);
	free(wrkStr);
	return count;
} 
/**********************************************************/


/**********************************************************/
//Libera la memoria reservada por str2wrdList().
//Una llamada a str2wrdList -> una llamada a freewrdLst
//no vale dos o tres llamadas a str2wrdList y una a freewrdLst
VOID freewrdLst(INT count, pCHAR words[])
{
	INT i;
	for (i=0; i<count; i++) free(words[i]);
	free(words);
}

/**********************************************************/



