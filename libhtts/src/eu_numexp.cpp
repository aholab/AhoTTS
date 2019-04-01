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

Nombre fuente................ numexp.cpp
Nombre paquete............... normalizador
Lenguaje fuente.............. C++
Estado....................... desarrollo
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Juan Luis Murugarren
                 ................. Nora Barroso

Version  dd/mm/aa  Autor     Proposito de la edicion
-------------------------------------------------------------
0.1.1	 09/03/07  inaxio    zenbakiaren ezkerraldeko zeroak irakurtzeko zuzenketak (007: zero zero zazpi)
0.1.0	 16/04/05  Nora	     "zero" zenbakiaren kasuen prozesaketa osoa; 0, 00, 000, 00000, etabar.
0.0.7    25/04/01  Yon2.     Multiling support.
0.0.6    25/11/00  Yon2.     Si "uno" sin "millar" en "miloi" o
                             "biloi" invertir. Y est  es la £ltima.
0.0.5    25/11/00  Yon2.     Si "uno" con "mila" -> no "uno"
0.0.5    25/11/00  Yon2.     Corregir si terna no limite izq. es uno.
0.0.3    25/11/00  Yon2.     Quitar ETA secuela en millones + miles.
0.0.2    24/11/00  Yon2.     Corregir los nombres de los n£meros.
0.0.1    24/11/00  Yon2.     (milakoak) MILA ETA (ehunekoak/batekoak)
                             Genera duda "eta" tras milioi bilioi.
0.0.0    19/11/00  Yon2.     Codificacion inicial.

======================== Contenido ========================

  M¢dulo que expande n£meros. La celda que acepta como entrada
debe coincidir con un patr¢n "n", num‚rico. Inserta la
expansi¢n del n£mero a continuaci¢n de la celda de entrada.
Es un m¢dulo a nivel bajo. No rellena ning£n campo de las
celdas que trata salvo el campo de cadena. Las dem s labores
las deber  realizar alguna capa superior.
  Se consideran tareas de capas superiores la eliminaci¢n o
el deletreo de ceros por la izquierda, interpretaci¢n del signo
m s o del signo menos, numeros con decimales, o n£meros con
separador decimal y/o comas.

Reglas seguidas:
       - Se analizan los n£meros como grupos de ternas.
       - Hasta el n£mero veinte se escribe el nombre del
       n£mero sin c lculo alguno (n£meros de base).
       - Lo mismo para los m£ltiplos de 20 y las centenas.
       - Se tiene en cuenta el cambio de orden con el n£mero 1
       cuando hace falta. Ej: 1000000 -> Miloi bat.
       - De uno a cien se construyen con c lculos en base a
       los n£meros de base. En caso de necesitar la conjunci¢n
       y "eta" esta se a¤ade a la parte de las veintenas como "ta".
       Ej: berrogeiTA hamazortzi.
       - de cien a mil: (ehunekoak) ETA (batekoak). Batekoak -> 1:99
       Ej: zapiehun ETA hogeita hamaika.
       - de mil a un mill¢n se forman de una de estas formas:
         1.-(milakoak) MILA (ehunekoak) ETA (batekoak)
         2.-(milakoak) MILA ETA (ehunekoak/batekoak)
       Nota: En la forma 2 s¢lo aparecen "ehunekoak" o "batekoak".
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

#include "eu_t2l.hpp"
#include "symbolexp.h"
#include "choputi.h"



/**********************************************************/

CHAR noneStr[]="none";
CHAR taStr[]="ta";
CHAR andStr[]="eta";

pCHAR from10To19Table[]={
      (char*)"hamar", (char*)"hamaika", (char*)"hamabi", (char*)"hamahiru",
      (char*)"hamalau", (char*)"hamabost", (char*)"hamasei",
      (char*)"hamazazpi", (char*)"hamazortzi", (char*)"hemeretzi"
};

pCHAR tens2468Table[]={
      (char*)"hogei", (char*)"berrogei", (char*)"hirurogei", (char*)"laurogei"
};

pCHAR hundredsTable[]={
      (char*)"ehun", (char*)"berrehun", (char*)"hirurehun",(char*)"laurehun",
      (char*)"bostehun", (char*)"seiehun", (char*)"zazpiehun",
      (char*)"zortziehun", (char*)"bederatziehun"
};


CHAR thousandStr[]="mila";
CHAR millionStr[]="miloi";
CHAR billionStr[]="biloi";

/**********************************************************/


/**********************************************************/

CtI LangEU_TextToList::upTo99(UINT num, CtI p)
{

	INT emotion, emo_intensity;
	emotion = ct.getEmotion(p);
	emo_intensity = ct.getEmo_int(p);

	if (num == 0) p = ct.insafter(p, eu_getchexp((CHAR)('0')), FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
	while (num)	{
		if (num<=9) {
			p = ct.insafter(p, eu_getchexp((CHAR)(num+'0')), FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
			num = 0;
		}
		else 
			if (num<=19) {
				p = ct.insafter(p, from10To19Table[num-10], FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
				num = 0;
			}
		
			else {
				p = ct.insafter(p, tens2468Table[(UINT)(num/20-1)], FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
				num = num%20;
				if (num) p = ct.extendStr(p, taStr);
			}
	}

	return p;
}

/**********************************************************/


/**********************************************************/

CtI LangEU_TextToList::upTo999(UINT num, CtI p )
{

	INT emotion, emo_intensity;
	emotion = ct.getEmotion(p);
	emo_intensity = ct.getEmo_int(p);

	UINT cent;

	if (num < 100)	{
		p = upTo99(num, p);
		return p;
	}

	cent = (num / 100);
	p = ct.insafter(p, hundredsTable[cent - 1], FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu

	num = num - cent * 100;

	if (num) {
		p = ct.insafter(p, andStr, FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
		p = upTo99(num, p);
	}

	return p;

}

/**********************************************************/


/**********************************************************/

pCHAR get1E3n(INT numtern)
{
	if (numtern > 5)	{
		/* Error no se debería dar el caso */
		return NULL; //noneStr;
	}

	switch (numtern)	{
		case 1:
			return thousandStr;
		case 2:
			return millionStr;
		case 3:
			return thousandStr;
		case 4:
			return billionStr;
		case 5:
			return thousandStr;
		default:
			return NULL; //noneStr;
	}

	return NULL; //noneStr;

}

/**********************************************************/



/**********************************************************/

CtI LangEU_TextToList::expnum(CtI q, CtI p)
{
	BOOL numzone[6]={FALSE, FALSE, FALSE, FALSE, FALSE, FALSE};
	BOOL etaPend = FALSE;
	INT len;
	INT i, j, numceros=0;
	UINT terna, terna2;
	INT resto, numtern;
	CHAR temp[]="XXX";
	pCHAR input;
	INT emotion, emo_intensity;
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
		ct.setPatternForce(p, (char*)"l");
		ct.setStatus(p, ct.getStatus(q));
		ct.setTnor(p, UTYPENOR_NUMBER);
		p = ct.insafter(p,(char*)"zero",FALSE, emotion, emo_intensity);
		numceros--;
	}
	if (len > 18) 
	{
		/* !!!! deletrear el número */
		return p;
	}
	
	resto = len % 3; 
	numtern = (len - resto) / 3; 

	if (resto)	
	{
		etaPend = TRUE;
		for (i=0; i < resto; i++) 
			temp[i]=input[i]; 
		
		temp[i]=0;
		terna = chu_str2num(temp); 
		
		if (terna!= 1)	
		{				
			if (terna==0) 
				p = upTo99(terna, p); //NORA

			if (terna!=0)
			{
				p = upTo999(terna, p); //NORA
				if (get1E3n(numtern)) p = ct.insafter(p, get1E3n(numtern), FALSE, emotion, emo_intensity);//NORA
			}
		}

		else
		{
			switch (numtern)	
			{
			case 0:
				p = upTo99(terna, p);
				break;
			case 1:
			case 3:
			case 5:
				if (get1E3n(numtern))
					p = ct.insafter(p, get1E3n(numtern), FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
				break;
			case 2:
			case 4:
				if (get1E3n(numtern))
					p = ct.insafter(p, get1E3n(numtern), FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
				p = upTo99(terna, p);
				break;
			}
		}

		strcpy(input, &(input[resto]));
		numzone[numtern] = TRUE;
	} 
	
	for(j=0; j<numtern; j++)	
	{
		temp[0] = '\0';
		for (i=0; i < 3; i++)
			temp[i]=input[i]; 

		
		temp[i]=0;
		terna2 = chu_str2num(temp);

		if (terna2 != 0)	
		{			
		     if ((!(terna2 % 100)||!(terna2 / 100)) && etaPend
			   && (j != numtern - 2) && (j != numtern - 4))
					p = ct.insafter(p, andStr, FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
							
			if (terna2 != 1)       
			{
				p = upTo999(terna2, p);
				if (get1E3n(numtern - (j + 1))) p = ct.insafter(p, get1E3n(numtern - (j + 1)), FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
			}
			
			else switch (numtern - (j + 1))	
			{
			case 0:
				if (terna!=0) 
				{ 
					p = upTo99(terna2, p);
					break; 
				}//NORA
			case 1:
			case 3:
			case 5:
				if (get1E3n(numtern - (j + 1)))p = ct.insafter(p, get1E3n(numtern - (j + 1)), FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
				break;
			case 2:
				if (numzone[3] == TRUE) 
				{
        				p = upTo99(terna2, p);
        				if (get1E3n(numtern - (j + 1)))p = ct.insafter(p, get1E3n(numtern - (j + 1)), FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
				}
				else 
				{
        				if (get1E3n(numtern - (j + 1)))p = ct.insafter(p, get1E3n(numtern - (j + 1)), FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
					p = upTo99(terna2, p);
				}
			break;
			case 4:
				if (numzone[5] == TRUE) 
				{
        				p = upTo99(terna2, p);
        				if (get1E3n(numtern - (j + 1)))p = ct.insafter(p, get1E3n(numtern - (j + 1)), FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
				}
		        else 
				{
        				if (get1E3n(numtern - (j + 1)))p = ct.insafter(p, get1E3n(numtern - (j + 1)), FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
					p = upTo99(terna2, p);
				}
				break;
			} 
			numzone[numtern - (j + 1)] = TRUE;
			etaPend = TRUE;
			strcpy(input, &(input[3]));//Larraitz
		} 
		else
		{
			switch (numtern - (j + 1))	
			{
			case 2:
				if (numzone[3] == TRUE)
				{
					//if (terna2!=0) 
					p = ct.insafter(p, millionStr, FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
				}
				break;
			case 4:
				if (numzone[5] == TRUE) 
				{
					//if (terna2!=0) 
					p = ct.insafter(p, billionStr, FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
				}
				break;
			case 0:   
				if(terna2==0 && resto==0 &&numzone[j]==FALSE) 
				{
					p = upTo99(terna2, p); 
					break;
				}	// NORA 
			case 1:
			case 3:
			case 5:
				break;
			default:
				break;
			}
			strcpy(input, &(input[3]));//Larraitz

		}

	}

	free(input);

  return p;
}
/**********************************************************/

/**********************************************************/

CtI LangEU_TextToList::expnum(CtI p)
{
  expnum(p, p);
  return p;
}
/**********************************************************/

