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
Copyright: 2004 - TSR/Aholab (DET) ETSII/IT-Bilbao

Nombre fuente................ es_cap.cpp
Nombre paquete............... normalizador
Lenguaje fuente.............. C++
Estado....................... desarrollo
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. inigos

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
0.0.3	 15/06/11  inaki     Pronounce: declarar i,j y k como signed int, ya que en algún caso se comparan con signed
0.0.2	 28/11/06  inaxio     Pronounce-n: chr->cr, chl->cl, ph+bokala->f eta sh+bokala->x.
0.0.1    07/07/05  Larraitz   Aldaketa txiki bat memblock akatsa ez emateko.
0.0.0    01/10/04  inigos     Codificacion inicial.

======================== Contenido ========================



===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include "es_t2l.hpp"
#include "chset.h"
#include "choputi.h"
#include "symbolexp.h"


INT LangES_TextToList::isCap(CtI p)
{
	INT value = 0;
	pCHAR changeStr, origStr;

	pCHAR pattern;
	
	// Comprobamos que sólo son letras
 	if (ct(p).pattern)
	{
		pattern = strdup(ct(p).pattern);
	}
	if (strlen(pattern) != 1 || strcmp(pattern, "l") != 0)
	{
		free(pattern); //IÑAKI 
		return value;
	}
	free(pattern); //IÑAKI 

	origStr = strdup(ct.getStr(p));
	changeStr = strdup(ct.getStr(p));
	ISOLatin1_StrLower(changeStr);

	// Comparamos la palabra con la pasada a minúsculas
	if(strcmp(changeStr, origStr))
	{
		// Si son diferentes comparamos a partir del segundo carácter
		// "Miguel" != "miguel"

		if(!strcmp(&changeStr[1], &origStr[1]))
		{
			// Son iguales
			// "iguel" == "iguel"
			value = 2;
		}
		else
		{
			value = 1;
		}

	}
	free(origStr);
	free(changeStr);

	return value;
}

/**********************************************************/

CtI LangES_TextToList::pronounce( CtI p)
{
//	unsigned int i, j, k;
	int i, j, k;//inaki
	INT l=0, ll=0, r=0, rr=0, s; // Contadores de casos
	INT prevvowel, endpoint, startpoint;
	INT vowel=0;
	CHAR temp = ' ';
	pCHAR word = NULL;
	CHAR lastWordOrig;
	CHAR pattern[MAX_WORD_BUFFER];
	CHAR tempWord[MAX_WORD_BUFFER];
	CHAR tempWord2[MAX_WORD_BUFFER];
	CHAR newWord[MAX_WORD_BUFFER];

	// Trabajamos en minúsculas
	word = strdup(ct(p).str);
	ISOLatin1_StrLower(word);
	strcpy(pattern, ct(p).str);

	lastWordOrig = word[strlen(word)-1];
	// creamos un patrón de vocales/ consonantes
	for (i = 0; i < strlen(word); i++)
	{
		if(isVowel(word[i]))
		{
			pattern[i] = 'v';
			vowel++;
		}
		else
		{
			pattern[i] = 'c';
		}
	}
	
	// Y griegas como vocales
	for( i=0; i < strlen(word); i++)
	{
		if(word[i] == 'y')
		{
			if(pattern[i+1] !='v')
			{
				word[i] = 'i';
				pattern[i] = 'v';
				vowel++;
			}
		}
	}
	ct.setStr(p, word);

	if(!vowel)
	{
		p = expandCell(p);
		free(word);
		return p;
	}

	// Miramos la letra inicial
	if(pattern[0] == 'c' && pattern[1] != 'v' && word[1] != 'h')
	{
		switch(word[0])
		{

		// S inicial -> "es"
		case 's':
			strcpy(tempWord, "e");
			strcpy(&tempWord[1], word);
			ct.setStr(p, tempWord);
			break;

		// X inicial -> "es"
		case 'x':
			strcpy(tempWord, "e");
			strcpy(&tempWord[1], word);
			ct.setStr(p, tempWord);
			break;

		// Z inicial -> "Ze"
		case 'z':
			strcpy(tempWord, "ce");
			strcpy(&tempWord[2], &word[1]);
			ct.setStr(p, tempWord);
			break;
		default:
			break;
		}
	}
	tempWord[0] = '\0';
	free(word); //IÑAKI
	
	word = strdup(ct(p).str);
	ISOLatin1_StrLower(word);

	// Letras dobles  (salvo "rr" y "ll") se simplifican
	for (i=0, j=0; i < strlen(word); i++)
	{
		if(word[i] != temp)
		{
			tempWord[j] = word[i];
			j++;
			switch(word[i])
			{
			case 'r':
				r++;
				temp = ' ';
				if(r==2) rr++;
				break;
			case 'l':
				l++;
				temp = ' ';
				if(l==2) ll++;
				break;
			default:
				r=0;
				temp = word[i];
				break;
			}
		}
	}
	tempWord[j] = '\0';
	ct.setStr(p, tempWord);
	strcpy(word, tempWord);

	// Haches mudas (Baghdag)
	for (i=0, j=0, temp=' '; i < strlen(word); i++)
	{
		if(word[i] == 'h')
		{
			switch(temp)
			{
			case 'c': 
				if(word[i+1]=='r')//chr-->cr bihurtu
				{
					tempWord[j-1]='c';
					tempWord[j]='r';
				}
				else if(word[i+1]=='l')//inaxio: chl-->cl bihurtu
				{
					tempWord[j-1]='c';
					tempWord[j]='l';
				}
				else
					j++;
				break;
				//tempWord[j] = word[i];
				//j++;
				break;
			case's':
				if(pattern[i+1]=='v')// sh+bokal-->x+bokal
				{
					tempWord[j-1]='x';
				}
				break;
			case'p':
				if(pattern[i+1]=='v')//inaxio ph+bokal-->f+bokal
				{
					tempWord[j-1]='f';
				}
				break;
			case ' ': // Primera letra
				tempWord[j] = 'j';
				j++;
				break;
			default: // no escribimos la "h"
				break;
			}
		}
		else if(word[i]=='c')//inaxio
		{
			if(i==strlen(word))
			{
				tempWord[j]='c';
			}
			else
			{
				switch(word[i+1])
				{
				case'e':
					tempWord[j]='c';//inaxio
					break;
				case'i':
					tempWord[j]='c';//inaxio
					break;
				case'h':   //iñaki para que no borre la h de ch
					tempWord[j]='c';//inaxio
					tempWord[j+1]='h';//inaxio
					break;
				case'k':
					i++;
					break;
				default:
					tempWord[j]='c';
					break;
				}
				j++;
			}
		}
		else
		{
			tempWord[j] = word[i];
			j++;
		}
		temp = word[i];
	}
	tempWord[j] = '\0';
	ct.setStr(p, tempWord);
	strcpy(word, tempWord);


	// actualizamos el patrón de vocales/ consonantes
	for (i = 0; i < strlen(word); i++)
	{
		if(isVowel(word[i]))
		{
			pattern[i] = 'v';
		}
		else
		{
			pattern[i] = 'c';
		}
	}

	pattern[i]='\0';
	tempWord[0] = '\0';
	newWord[0] = '\0';


	// M-s como N-s ante consonante
	for( i=0; i < strlen(word); i++)
	{
		if(word[i] == 'm')
		{
			if(pattern[i+1] !='v')
			{
				word[i] = 'n';
			}
		}
	}

	// W-s como U-s ante consonante
	for( i=0; i < strlen(word); i++)
	{
		if(word[i] == 'w')
		{
			if(pattern[i+1] !='v')
			{
				word[i] = 'u';
				pattern[i] = 'v';
			}
		}
	}

	ct.setStr(p, word);

	tempWord[0] = '\0';
	newWord[0] = '\0';

	lastWordOrig = word[strlen(word)-1];


	// Si ya es pronunciable lo dejamos

		if(isPronun(p))
	{
		free(word);
		return p;
	}

    free(word); //IÑAKI
	word = strdup(ct(p).str);
	ISOLatin1_StrLower(word);



	// Quitar consonantes

	for (i=0, j=0, k=0, prevvowel=-1, endpoint=0, startpoint=0; i<strlen(word);i++)
	{
		
		if(pattern[i]=='v')
		{
			for(j=0; j+prevvowel<i; j++)
			{
				tempWord[j]=word[prevvowel+j+1];
			}

		// Miramos desde dónde es pronunciable

// <pronun>
			tempWord[j+1]='\0';
			ct.setStr(p, tempWord);
//			}
			k=1;

			strcpy(tempWord2, tempWord);

			while(!isPronun(p))
			{
				strcpy(tempWord2, "cal");
				strcpy(&tempWord2[3], &tempWord[k]);
				ct.setStr(p, tempWord2);
				strcpy(tempWord2, &tempWord[k]);
				k++;
			}
			startpoint=prevvowel+k;
// </pronun>

		// Miramos hasta dónde es pronunciable
// <pronun2>			
			while(isPronun(p)) // Mientras sea pronunciable añadimos letras
			{
				// Si la siguiente es vocal o si estamos en la última salimos del bucle
				if(pattern[prevvowel+j+1]=='v' || word[prevvowel+j+1]=='\0') 
				{
					j++;
					break;
				}

				if(j==1) // porque prevvowel es -1
				{
					tempWord2[j-k]=word[prevvowel+j];
				}

				tempWord2[j-k+1]=word[prevvowel+j+1];
				
				// Si no es final de palabra le añadimos una sílaba
				// para la prueba de pronunciacion
				if(word[prevvowel+j+2]!='\0') 
				{
					strcpy(&tempWord2[j-k+2], "za");
				}
				else
				{
					tempWord2[j-k+2]='\0';
				}

				ct.setStr(p, tempWord2);
				j++;
			}

// </pronun2>
			endpoint=prevvowel+j;

			// Si viene una s "impronunciable" la metemos a la fuerza
// <s>
			if(prevvowel<0) prevvowel =0;

			for(k=prevvowel+1, s=0; pattern[k]=='c'; k++)
			{
				if(tempWord[k] == 's' && k>endpoint)
				{
					s=1;
				}
			}
			tempWord[0]='\0';

			for(j=0; (startpoint+j)<endpoint; j++)
			{
				tempWord[j]=word[startpoint+j];
			}
			if(s)
			{
				strcpy(&tempWord[j], "s");
			}
			else
			{
				tempWord[j]='\0';
			}
			strcpy(&newWord[strlen(newWord)], tempWord);
// </s>
			prevvowel=i;
		}

		//i++;
	}


	tempWord[0] = '\0';
	temp = ' ';


	// Volvemos a las letras dobles  (salvo "rr" y "ll")
// <llrr2>
	for (i=0, j=0; i < strlen(newWord); i++)
	{
		if(newWord[i] != temp)
		{
			tempWord[j] = newWord[i];
			j++;
			switch(word[i])
			{
			case 'r':
				if(rr>0)
				{
					rr--;
					temp = ' ';
				}
				else
				{
					temp = newWord[i];
				}
				break;
			case 'l':
				if(ll>0)
				{
					ll--;
					temp = ' ';
				}
				else
				{
					temp = newWord[i];
				}
				break;
			default:
				temp = newWord[i];
			}
		}
	}

	tempWord[j] = '\0';
	ct.setStr(p, tempWord);
	//strcpy(word, tempWord); //IÑAKI
	free(word); //IÑAKI
	word = strdup(tempWord); //IÑAKI

// </llrr2>

	// Miramos si hemos dejado cortada por impronunciable una "s" o "n"
// <sn>
	switch(lastWordOrig)
	{
	case 's':
		if(tempWord[strlen(tempWord) -1] != 's')
		{
			strcpy(&tempWord[strlen(tempWord)], "s");
			ct.setStr(p, tempWord);
		}
		break;
	case 'n':
		if(tempWord[strlen(tempWord) -1] != 'n')
		{
			strcpy(&tempWord[strlen(tempWord)], "n");
			ct.setStr(p, tempWord);
		}
		break;
	default:
		break;
	}

// </sn>
	//free(word); //Larraitz
	free(word); //IÑAKI
	return p;
}

/**********************************************************/
