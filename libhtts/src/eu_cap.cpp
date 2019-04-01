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
Copyright:2004-TSR/Aholab(DET)ETSII/IT-Bilbao

Nombrefuente................eu_cap.cpp
Nombrepaquete...............normalizador
Lenguajefuente..............C++
Estado.......................desarrollando
DependenciaHard/OS..........-
Codigocondicional...........-

Codificacion.................Larraitz

Version	dd/mm/aa	Autor	Propositodelaedicion
----------------------------------------------
0.0.5   30/11/06	inaxio		Quitar que gn->ñ (que no diga ñomo o ñostiko).
0.0.4	24/11/06	inaxio		qu->k bihurtu, ph+bokala->f eta chl->kl.
0.0.3	21/06/05	Larraitz	mayusDekline funtzioa sortu.
0.0.2	21/06/05	Larraitz	Pronuce funtzioan baldintza aldaketak.
0.0.1	15/06/05	Larraitz	maiuskula mota berriak.
0.0.0	30/05/05	Larraitz	es_cap	kopia.

========================Contenido========================
isCap funtzioan maiuskula mota berriak ezarri:
1-letra guztiak minuskulaz
2-lehen letra soilik maiuskulaz
3-lehen letrak maiuskulaz eta azkenak minuskulaz
4-letra guztiak maiuskulaz
5-letrak maiuskulaz eta minuskulaz nahasturik

pronounce funtzioan: ahoskaezinak diren hitzak
ahal den eran ahoskatuak izan daitezen baldintzak


mayusDekline funtzioan :  lehen letrak maiuskulaz eta
amaierakoak minuskulaz dauden hitzak tratatzeko baldintzak

===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/


#include "eu_t2l.hpp"
#include "chset.h"
#include "choputi.h"
#include "symbolexp.h"


INT LangEU_TextToList::isCap(CtI p)
{
	INT i;
	INT value=0;
	INT emotion, emo_intensity;
	pCHAR changeStr, origStr;
	BOOL mayus_min,	min_mayus; //Larraitz

	pCHAR pattern;

	emotion = ct.getEmotion(p);
	emo_intensity = ct.getEmo_int(p);

	//Comprobamos que sólo son letras
	if(ct(p).pattern)
	{
		pattern = strdup(ct(p).pattern);
	}
	if(strlen(pattern)!=1||strcmp(pattern,"l")!=0)
	{
		free(pattern); //IÑAKI
		return value;
	}

	free(pattern); //IÑAKI
	origStr = strdup(ct.getStr(p));
	changeStr = strdup(ct.getStr(p));
	ISOLatin1_StrLower(changeStr);


	// value = 5 denean letra guztiak minuskulaz
	value=5;

	//Comparamos la palabra con la pasada a minúsculas
	if(strcmp(changeStr,origStr))
	{
		//Si son diferentes comparamos a partir del segundo carácter
		//"Miguel"!="miguel"

		//value 2 denean hitzaren lehen letra soilik maiuskulaz
		//daudela idatziak
		if(!strcmp(&changeStr[1],&origStr[1]))
		{
			//Son iguales
			//"iguel"=="iguel"
			value=2;
		}

		//Baieztatzen da hitzeko letra guztiak ez daudela
		//minuskulaz idatzirik.
		//value = 3 denean hitzeko letraren bat minuskulaz
		//value = 1 denean hitzeko letra guztiak maiuskulaz

		else //Larraitz
		{
			for(i=0;i<strlen(changeStr);i++)
			{
				if(changeStr[i]==origStr[i])
				{
					value=3;
					goto exit;
				}
			}
			value=1;
		}
	}
exit:

	mayus_min = FALSE;
	min_mayus = FALSE;

	if(value == 3) //Larraitz
	{
		for(i=0;i<strlen(changeStr);i++)
		{
			if (changeStr[i]==origStr[i])
			{
				mayus_min = TRUE;
			}
			else if(mayus_min)
			{
				min_mayus = TRUE;
			}

		}

		if ((mayus_min)&&(!(min_mayus)))
		{
			value = 4;
		}
	}

	free(origStr);
	free(changeStr);

	return value;
}

/**********************************************************/

CtI LangEU_TextToList::pronounce(CtI p)
{
	INT stat;
	INT i, j, k;
	INT l=0, ll=0, r=0, rr=0, s;//Contadores de casos
	INT prevvowel,endpoint,startpoint;
	INT vowel=0;
	CHAR temp=' ';
	pCHAR word=NULL;
	CHAR lastWordOrig;
	CHAR pattern[MAX_WORD_BUFFER];
	CHAR tempWord[MAX_WORD_BUFFER];
	CHAR tempWord2[MAX_WORD_BUFFER];
	CHAR newWord[MAX_WORD_BUFFER];

	stat = ct.getStatus(p);


	//Trabajamos en minúsculas
	word=strdup(ct(p).str);
	ISOLatin1_StrLower(word);

	strcpy(pattern,ct(p).str);
	lastWordOrig=word[strlen(word)-1];

	//creamos un patrón de vocales/consonantes
	for(i=0;i<strlen(word);i++)
	{
		if(isVowel(word[i]))
		{
			pattern[i]='v';
			vowel++;
		}
		else
		{
			pattern[i]='c';
		}
	}

	//Y bokal bezela //Larraitz
	for(i=0;i<strlen(word);i++)
	{
		if(word[i]=='y')
		{
			if((i+1)<(strlen(word)))
			{
				if(pattern[i+1]=='v')
				{
					if((i-2)>=0)
					{
						if ((pattern[i-2]=='c')&&(pattern[i-1]=='c'))
						{
							pattern[i]='v';
							word[i]='i';
							vowel++;
							goto fin;
						}
					}
					else if(pattern[i-1]=='c')
					{
						pattern[i]='v';
						word[i]='i';
						vowel++;
						goto fin;
					}
				}//amaitu esk bokala denean
				else
				{
					if(pattern[i+1]=='c')
					{
						pattern[i]='v';
						word[i]='i';
						vowel++;
						goto fin;
					}//amaitu esk konsonantea denean
				}
			}//amaitu esk grafia denean
			else
			{
				pattern[i]='v';
				word[i]='i';
				vowel++;
				goto fin;
			}//amaitu esk ez denean grafiarikf
fin:
			;
		}
	}
	/*if(pattern[i+1]!='v')
			{
				word[i]='i';
				pattern[i]='v';
				vowel++;
			}
		}
	}*/

	//ez badago kontsonanterik deletreatu egin
	//behar da
	if(!vowel)
	{
		p=expandCell(p);
		free(word);
		return p;
	}

	ct.setStr(p,word);


	//////////////////////////////////////////////////////7
	///		Salbikegina							/////7
	/////////////////////////////////////////////////////

	//Lehenengoletrakontsonanteabadaetabigarrena
	//ezbokal,ez'h'
	/*if(pattern[0]=='c'&&pattern[1]!='v'&&word[1]!='h')
	{
		switch(word[0])
		{

		//Sinicial->"es"
		case's':
			fprintf(stderr,"Larraitz1%c",word[0]);//ezabatu
			strcpy(tempWord,"e");
			strcpy(&tempWord[1],word);
			ct.setStr(p,tempWord);
			break;

		//Xinicial->"es"
		case'x':
			fprintf(stderr,"Larraitz2%c",word[0]);//ezabatu

			strcpy(tempWord,"e");
			strcpy(&tempWord[1],word);
			ct.setStr(p,tempWord);
			break;

		//Zinicial->"Ze"
		case'z':
			strcpy(tempWord,"ce");
			strcpy(&tempWord[2],&word[1]);
			ct.setStr(p,tempWord);
			break;
		default:
			break;
		}
	}*/

	////////////////////////////////////////////////////////////////7


	if(word[0]=='s'&&pattern[1]!='v'&&word[1]!='h')
	{
		strcpy(tempWord,"e");
		strcpy(&tempWord[1],word);
		ct.setStr(p,tempWord);//Xinicial->"es"
	}

	tempWord[0]='\0';
	free(word); //IÑAKI
	word=strdup(ct(p).str);
	ISOLatin1_StrLower(word);

	//Letras dobles (salvo "rr" y "ll"; si "ee" --> i; si "oo" --> u) se simplifican//Larraitz
	for(i=0,j=0;i<strlen(word);i++)
	{
		if(word[i]!=temp)
		{
			tempWord[j]=word[i];
			j++;
			switch(word[i])
			{
			case 'r':
				r++;
				temp=' ';
				if(r==2)
					rr++;
				break;
			case 'l':
				l++;
				temp=' ';
				if(l==2)
					ll++;
				break;
			default:
				r=0;
				temp=word[i];
				break;
			}
		}
		else if(word[i] == 'e') tempWord[j-1] = 'i'; //Larraitz
		else if(word[i] == 'o') tempWord[j-1] = 'u'; //Larraitz
	}
	tempWord[j]='\0';
	ct.setStr(p,tempWord);
	strcpy(word,tempWord);

	/* //inaxio->quitarlo para que no diga "ñomo", "ñoseologia" o ñostiko

	//Ñ (epargne-->epañe)//Larraitz
	for(i=0,j=0,temp=' ';i<strlen(word);i++,j++)
	{
		if((word[i]=='g')&&((i+2)<strlen(word)))
		{
			if((word[i+1]=='n')&&(pattern[i+2]=='v'))
			{
				tempWord[j]='ñ';
				i++;
			}
			else
				tempWord[j]=word[i];
		}
		else
		{
			tempWord[j]=word[i];
		}
	}*/
	tempWord[j]='\0';



	ct.setStr(p,tempWord);
	strcpy(word,tempWord);

	//Haches mudas(Baghdag)
	for(i=0,j=0,temp=' ';i<strlen(word);i++)
	{
		if(word[i]=='h')
		{
			switch(temp)
			{
			case'c':
				if(word[i+1]=='r')//Larraitzez denean chr-->cr //iñaki, cambiar !=`por ==
				{
					tempWord[j-1]='k';
					tempWord[j]='r';
					//j++;	//inaxio
				}
				else if(word[i+1]=='l')//inaxio: chl-->cl bihurtu
				{
					tempWord[j-1]='k';
					tempWord[j]='l';
				}
				else  //iñaki para que no borre la h
				//	tempWord[j-1]='k';
				j++; //iñaki
				break;
			case's':
				if(pattern[i+1]=='v')//Larraitz sh+bokal-->x+bokal
				{
					tempWord[j-1]='x';
					//i++;//inaxio
				}
				break;
			case'p':
				if(pattern[i+1]=='v')//inaxio ph+bokal-->f+bokal
				{
					tempWord[j-1]='f';
				}
				break;
			case' '://Primera letra
				tempWord[j]='j';

				j++;
				break;
			default://no escribimos la"h"
				break;
			}
		}
		else if(word[i]=='c')//Larraitz
		{
			if(i==strlen(word))
			{
				tempWord[j]='k';
			}
			else
			{
				switch(word[i+1])
				{
				case'e':
					tempWord[j]='z';//inaxio
					break;
				case'i':
					tempWord[j]='z';//inaxio
					break;
				case'h':   //iñaki para que no borre la h de ch
					tempWord[j]='t';//inaxio
					tempWord[j+1]='x';//inaxio
					break;
				case'k':
					i++;
					break;
				default:
					tempWord[j]='k';
					break;
				}
				j++;
			}
		}
		else
		{
			tempWord[j]=word[i];
			j++;
		}
		temp=word[i];
	}
	tempWord[j]='\0';
	ct.setStr(p,tempWord);
	strcpy(word,tempWord);


	//actualizamos el patrón de vocales/consonantes
	for(i=0;i<strlen(word);i++)
	{
		if(isVowel(word[i]))
		{
			pattern[i]='v';
		}
		else
		{
			pattern[i]='c';
		}
	}

	pattern[i]='\0';
	tempWord[0] = '\0';
	newWord[0] = '\0';

	//M-s como N-s ante consonante
	//W-s como U-s
	//V-atratatu//Larraitz
	//Q-a tratatu//inaxio
	for(i=0;i<strlen(word);i++)
	{
		if(word[i]=='m')
		{
			if(pattern[i+1]!='v')
			{
				word[i]='n';
			}
		}

		/*if(word[i]=='w')
		{
			word[i]='u';
			pattern[i]='v';

		}*/

		if(word[i]=='v')
		{
			if((i+1)==strlen(word))
			{
				word[i]='f';
			}
			else if((pattern[i+1]!='v')&&(i==0))
			{
				if ((word[i+1]!='l')&&(word[i+1]!='r'))
				{
					word[i]='u';
					pattern[i]='v';
				}
				else
				{
					word[i]='b';
				}
			}
			else if ((pattern[i+1]!='v')&&(pattern[i-1]!='v'))
			{
				word[i]='u';
				pattern[i]='v';

			}
			else if(pattern[i+1]!='v')
			{
				if ((word[i+1]!='l')&&(word[i+1]!='r'))
				{
					word[i]='f';

				}
				else
				{
					word[i]='b';
				}
			}
			else
			{
				word[i]='b';

			}

		}
	}


	ct.setStr(p,word);
	tempWord[0]='\0';
	newWord[0]='\0';

	lastWordOrig=word[strlen(word)-1];


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

	for (i=0, j=0, k=0, prevvowel=-1, endpoint=0, startpoint=0; i<strlen(word);)
	{
		if(pattern[i]=='v')
		{
			for(j=0; (j+prevvowel)<i; j++)
			{
				tempWord[j]=word[prevvowel+j+1];
			}

		// Miramos desde dónde es pronunciable

// <pronun>
			tempWord[j]='\0';
			ct.setStr(p, tempWord);
			k=1;

			strcpy(tempWord2, tempWord);

			while(!isPronun(p))
			{
				strcpy(tempWord2, "kal");
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
		i++;
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

	ct.setStatus(p,stat);
	ct.setTnor(p, UTYPENOR_NAME);


// </sn>
	//free(word); //Larraitz
	free(word); //IÑAKI
	return p;
}


/**********************************************************/
//				Larraitz
/*********************************************************/
CtI LangEU_TextToList::mayusDekline( CtI p)
{
	CtI q;
	INT stat,emotion, emo_intensity;
	BOOL isDec = FALSE;
	pCHAR origStr = NULL, changeStr = NULL;
	CHAR wordMin[MAX_WORD_BUFFER];
	CHAR wordMay[MAX_WORD_BUFFER];
	UINT len;
	HDicRef myhdic_min,myhdic_may;
	INT min = 0, max = 0;

	origStr = strdup(ct.getStr(p));
	changeStr = strdup(ct.getStr(p));
	ISOLatin1_StrLower(changeStr);

	stat = ct.getStatus(p);
	emotion = ct.getEmotion(p);
	emo_intensity = ct.getEmo_int(p);

	min=0;
	max=0;

	for(int i=0;i<strlen(changeStr);i++)
	{

		if(changeStr[i]==origStr[i]) //minuskulaz idatzirik dauden letrak
		{
			wordMin [min] = changeStr[i];
			min++;
		}
		else //maiuskulaz idatzirik dauden letrak
		{
			wordMay [max] = origStr[i];
			max++;
		}
	}

	wordMin [min] = '\0';
	wordMay [max] = '\0';

	//Es declinación?
	myhdic_min = dic->search(wordMin);
	myhdic_may = dic->search(wordMay);

	isDec = dic->query(myhdic_min, HDIC_QUERY_EU_DEC);

	if (!isDec)
	{
		p = expandCell(p);
		return p;
	}

	len = dic->query(myhdic_min, HDIC_QUERY_MATCHLEN); //ez bada ezer aurkitu

	if (len)
	{
		p = expandCell(p);
		return p;
	}

	ct.setStr(p,wordMay);

	switch (isAbbAcrUni(p))
	{
		case AAU_ABB:
		case AAU_ACR:
			p = expAbbAcrUni(p, FALSE, FALSE );
			break;
		case AAU_UNIT:
			p = expAbbAcrUni(q, FALSE, TRUE );
			break;
		case AAU_ABBP:
		case AAU_ACRP:
			p = expAbbAcrUni(q, TRUE, FALSE );
			break;
		case AAU_UNITP:
			p = expAbbAcrUni(q, TRUE, TRUE );
			break;
		default:
			p = expandCell(p);
			break;
	}

	ct.setStatus(p,stat);
	ct.setTnor(p,UTYPENOR_CAPITAL);


	p=ct.lastInGrp(p);


	p = ct.insafter(p, wordMin, TRUE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
	ct.setPatternForce(p, (char *)"l");
	ct.setStatus(p, stat);
	//p = ct.prev(p);
	p = ct.prevGrp(p);

	p = dekline(p);
	return p;
}

