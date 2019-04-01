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

Nombre fuente................ es_numexp.cpp
Nombre paquete............... normalizador
Lenguaje fuente.............. C++
Estado....................... desarrollo
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Juan Luis Murugarren

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
0.1.2	 09/03/07  inaxio    	 zenbakiaren ezkerraldeko zeroak irakurtzeko zuzenketak (007: zero zero zazpi)
0.1.1	 14/04/03  inigos	 Soporte para decimales
0.1.0	 11/11/02  inigos	 Adaptación para castellano
0.0.X			   Yon2.	 Mejoras para euskera
0.0.0    19/11/00  Yon2.     Codificacion inicial.

======================== Contenido ========================

  M¢dulo que expande numeros. La celda que acepta como entrada
debe coincidir con un patron "n", numerico. Inserta la
expansion del numero a continuacion de la celda de entrada.
Es un modulo a nivel bajo. No rellena ningun campo de las
celdas que trata salvo el campo de cadena. Las demas labores
las debera realizar alguna capa superior.
  Se consideran tareas de capas superiores la eliminacian o
el deletreo de ceros por la izquierda, interpretaci¢n del signo
mas o del signo menos, numeros con decimales, o numeros con
separador decimal y/o comas.

Reglas seguidas:
       - Se analizan los n£meros como grupos de ternas.
       - Hasta el numero veinte se escribe el nombre del
       n£mero sin calculo alguno (numeros de base).
       - Lo mismo para los multiplos de 20 y las centenas.
       - Se tiene en cuenta el cambio de orden con el numero 1
       cuando hace falta. Ej: 1000000 -> Miloi bat.
       - De uno a cien se construyen con c lculos en base a
       los n£meros de base. En caso de necesitar la conjuncion
       y "eta" esta se añade a la parte de las veintenas como "ta".
       Ej: berrogeiTA hamazortzi.
       - de cien a mil: (ehunekoak) ETA (batekoak). Batekoak -> 1:99
       Ej: zapiehun ETA hogeita hamaika.
       - de mil a un mill¢n se forman de una de estas formas:
         1.-(milakoak) MILA (ehunekoak) ETA (batekoak)
         2.-(milakoak) MILA ETA (ehunekoak/batekoak)
       Nota: En la forma 2 solo aparecen "ehunekoak" o "batekoak".
       Ej: (Forma 1) 1.123 MILA ehun eta hogeita hiru.
       Ej: (Forma 2) 1.200 MILA ETA berrehun.
       La conjunci¢n "ETA" de la forma 2 no aparece para c¢pulas
       de millones y miles ni de billones y miles de millones.
       Ej: 110.100.600 ehun eta hamar MILOI ehun MILA eta seiehun.



Nota:
     Se sigue la nomenclatura de "Euskal Gramatika Osoa" salvo para
los n£meros:
    - 18 -> Se toma "hamazortzi" en vez de "hemezortzi".
    - Se toma "miloi" en vez de "milioi".
    - Se toma "biloi" en vez de "bilioi".
Yon2.

===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/
  
/**********************************************************/

#include <stdio.h>

#include "es_t2l.hpp"
#include "symbolexp.h"
#include "choputi.h"



/**********************************************************/

CHAR es_yStr[]="y";
CHAR es_100Str[]="cien";

pCHAR es_from10To20Table[]={
      (char *)"diez", (char *)"once", (char *)"doce", (char *)"trece",
      (char *)"catorce", (char *)"quince", (char *)"dieciséis",
      (char *)"diecisiete", (char *)"dieciocho", (char *)"diecinueve",
	  (char *)"veinte", (char *)"veintiuno", (char *)"veintidos",
	  (char *)"veintitrés", (char *)"veinticuatro", (char *)"veinticinco",
	  (char *)"veintiséis", (char *)"veintisiete", (char *)"veintiocho",
	  (char *)"veintinueve"
};

pCHAR es_tensTable[]={
      (char *)"veinti", (char *)"treinta", (char *)"cuarenta", (char *)"cincuenta",
		(char *)"sesenta", (char *)"setenta", (char *)"ochenta", (char *)"noventa"
};

pCHAR es_hundredsTable[]={
      (char *)"ciento", (char *)"doscientos", (char *)"trescientos",(char *)"cuatrocientos",
      (char *)"quinientos", (char *)"seiscientos", (char *)"setecientos",
      (char *)"ochocientos", (char *)"novecientos"
};


CHAR es_thousandStr[]="mil";
CHAR es_millionStr[]="millones";
CHAR es_billionStr[]="billones";
CHAR es_millionSingStr[]="millón";
CHAR es_billionSingStr[]="billón";

/**********************************************************/


/**********************************************************/

CtI LangES_TextToList::upTo99(UINT num, CtI p, BOOL primero)
{
	INT numInicial = num;
	INT emotion, emo_intensity;
	emotion = ct.getEmotion(p);
	emo_intensity = ct.getEmo_int(p);

	if (num == 0)
	{
		p = ct.insafter(p, es_getchexp((CHAR)('0')), FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
		ct.setPatternForce(p,(char *) "l");
		ct.setStatus(p, START);
		ct.setTnor(p, UTYPENOR_NUMBER);	
		primero = FALSE;
	}

	while (num)	{
		if (num<=9)
		{
			p = ct.insafter(p, es_getchexp((CHAR)(num+'0')), FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
			num = 0;
				
			if(primero)
			{
				ct.setPatternForce(p,(char *) "l");
				ct.setStatus(p, START);
				ct.setTnor(p, UTYPENOR_NUMBER);	
				primero = FALSE;
			}
		}
		else 
			if (num<30) {
				p = ct.insafter(p, es_from10To20Table[num-10], FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
				num = 0;

				if(primero)
				{
					ct.setPatternForce(p,(char *) "l");
					ct.setStatus(p, START);
					ct.setTnor(p, UTYPENOR_NUMBER);	
					primero = FALSE; //Larraitz
				}
			}
		
			else {
				p = ct.insafter(p, es_tensTable[(UINT)(num/10-2)], FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
				if(primero)
				{
					ct.setPatternForce(p,(char *) "l");
					ct.setStatus(p, START);
					ct.setTnor(p, UTYPENOR_NUMBER);	
					primero = FALSE; //larraitz
				}
				num = num%10;
				if (num) p = ct.insafter(p, es_yStr, FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu

			}
	}

	return p;
}

/**********************************************************/


/**********************************************************/

CtI LangES_TextToList::upTo999(UINT num, CtI p )
{
	UINT cent;
	INT emotion, emo_intensity;
	emotion = ct.getEmotion(p);
	emo_intensity = ct.getEmo_int(p);

	if (num < 100)	{
		p = upTo99(num, p, TRUE);
		return p;
	}

	if (num==100)
	{
		p = ct.insafter(p, es_100Str, TRUE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
		ct.setPatternForce(p,(char *) "l");
		ct.setStatus(p, START);
		ct.setTnor(p, UTYPENOR_NUMBER);
		return p;
	}
	
	cent = (num / 100);
	p = ct.insafter(p, es_hundredsTable[cent - 1], FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
	
	ct.setPatternForce(p, (char *)"l");
	ct.setStatus(p, START);
	ct.setTnor(p, UTYPENOR_NUMBER);

	num = num - cent * 100;

	if (num) {
		p = upTo99(num, p, FALSE); //FALSE ordez TRUE jarri
	}
	
	return p;

}

/**********************************************************/


/**********************************************************/

pCHAR es_get1E3n(INT numtern, BOOL singular)
{
	if (numtern > 5)	{
		/* Error no se debería dar el caso */
		return NULL; //noneStr;
	}

	switch (numtern)
	{
		case 1:
			return es_thousandStr;
		case 2:
			if(singular)
			{
				return es_millionSingStr;
			}
			return es_millionStr;
		case 3:
			return es_thousandStr;
		case 4:
			if(singular)
			{
				return es_billionSingStr;
			}
			return es_billionStr;
		case 5:
			return es_thousandStr;
		default:
			return NULL; //noneStr;
	}

	return NULL; //noneStr;

}

/**********************************************************/


/**********************************************************/

CtI LangES_TextToList::expnum(CtI q, CtI p)
{
	BOOL numzone[6]={FALSE, FALSE, FALSE, FALSE, FALSE, FALSE};
	INT len;
	INT i, j, numceros=0;
	INT emotion, emo_intensity;
	UINT terna;
	INT resto, numtern;
	CHAR temp[]="XXX";
	pCHAR input;
	BOOL limit = FALSE; // Ciclo anterior limitador (mil, millones...) para la pausación.

	emotion = ct.getEmotion(p);
	emo_intensity = ct.getEmo_int(p);
	input = strdup(ct.cell(q).str);
	len = strlen(input);

	//inaxio-> se cuentan los ceros a la izquierda
	for(i=0;input[0]=='0'&&len>1;)
	{
		numceros++;
		strcpy(&(input[i]), &(input[i+1]));
		len--;
	}
	while(numceros>0){
		ct.setPatternForce(p, (char *)"l");
		ct.setStatus(p, ct.getStatus(q));
		ct.setTnor(p, UTYPENOR_NUMBER);
		p = ct.insafter(p,(char *)"cero",FALSE, emotion, emo_intensity);
		numceros--;
	}

	if (len > 18) {
		/* !!!! deletrear el número */
		free(input);
		return p;
	}
	resto = len % 3;
	numtern = (len - resto) / 3;

	if (resto)
	{
		for (i=0; i < resto; i++) temp[i]=input[i];
		temp[i]=0;
		terna = chu_str2num(temp);


		if (terna != 1 )
		{
			if (terna==0 && len!=1) 
				p = upTo99(terna, p, TRUE); //LARRAITZ

			if(terna != 0 || (terna ==0 && len==1))
			{
				p = upTo999(terna, p);
				if (es_get1E3n(numtern, FALSE))
				{
					p = ct.insafter(p, es_get1E3n(numtern, FALSE), FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
					limit = TRUE;
				}
			}
		}
		else switch (numtern)
		{
			case 0:
				p = upTo99(terna, p, TRUE);
				break;
			case 1:
			case 3:
			case 5:
				if (es_get1E3n(numtern, TRUE))
				{
					p = ct.insafter(p, es_get1E3n(numtern, TRUE), FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
					limit = TRUE;
				}
				break;
			case 2:
			case 4:
				p = ct.insafter(p, (char *)"un", FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
				if (es_get1E3n(numtern, TRUE))
				{
					p = ct.insafter(p, es_get1E3n(numtern, TRUE), FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
					limit = TRUE;
				}
				break;
		}

		strcpy(input, &(input[resto]));
		numzone[numtern] = TRUE;
	}
	
	for(j=0; j<numtern; j++)
	{
		for (i=0; i < 3; i++) temp[i]=input[i];
		temp[i]=0;
		terna = chu_str2num(temp);

		if (terna != 0)
		{
			if (terna != 1)
			{
		        p = upTo999(terna, p);

				if (es_get1E3n(numtern - (j + 1), FALSE))
				{
					p = ct.insafter(p, es_get1E3n(numtern - (j + 1), FALSE), FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
				}
			}
			else
			{
				switch (numtern - (j + 1))
				{
				case 0:
					p = upTo99(terna, p, TRUE);
					break;
				case 1:
				case 3:
				case 5:
					if (es_get1E3n(numtern - (j + 1), FALSE))p = ct.insafter(p, es_get1E3n(numtern - (j + 1), FALSE), FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
					break;
				case 2:
					if (numzone[3] == TRUE)
					{
        				p = upTo99(terna, p, TRUE);
       					if (es_get1E3n(numtern - (j + 1), FALSE))p = ct.insafter(p, es_get1E3n(numtern - (j + 1), FALSE), FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
					}
					else
					{
       					if (es_get1E3n(numtern - (j + 1), FALSE))p = ct.insafter(p, es_get1E3n(numtern - (j + 1), FALSE), FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
						p = upTo99(terna, p, TRUE);
					}
					break;
				case 4:
					if (numzone[5] == TRUE)
					{
       					p = upTo99(terna, p, TRUE);
       					if (es_get1E3n(numtern - (j + 1), FALSE))p = ct.insafter(p, es_get1E3n(numtern - (j + 1), FALSE), FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
					}
					else
					{
       					if (es_get1E3n(numtern - (j + 1), FALSE))p = ct.insafter(p, es_get1E3n(numtern - (j + 1), FALSE), FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
						p = upTo99(terna, p, TRUE);
					}
					break;
				}
				numzone[numtern - (j + 1)] = TRUE;
			}
		}
		else
		{
			switch (numtern - (j + 1))
			{
			case 2:
				if (numzone[3] == TRUE)	p = ct.insafter(p, es_millionStr, FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
				break;
			case 4:
				if (numzone[5] == TRUE) p = ct.insafter(p, es_billionStr, FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
				break;
			case 0:
				if(terna==0 && resto==0 &&numzone[j]==FALSE) 
				{
					p = upTo99(terna, p, TRUE);
					break;
				}	// NORA 
			case 1:
			case 3:
			case 5:
				break;
			default:
				break;
			}
		}
			strcpy(input, &(input[3]));
	}


	free(input);

  return p;
}
/**********************************************************/

/**********************************************************/

CtI LangES_TextToList::expnum(CtI p)
{
  expnum(p, p);
  return p;
}
/**********************************************************/

