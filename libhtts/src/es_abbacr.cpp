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
	1997-2012  Aholab Signal Processing Laboratory, University of the Basque
 	Country (UPV/EHU)
	*2011-2012 Aholab Signal Processing Laboratory, University of the Basque
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

Nombre fuente................ es_abbabr.cpp
Nombre paquete............... normalizador
Lenguaje fuente.............. C++
Estado....................... desarrollo
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Juan Luis Murugarren
            ................. inigos

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
0.0.1	 19/05/04  inigos	 "Un_ kilometro", "dos kilometro_S"
0.0.0	 11/11/02  inigos	 Recopia para castellano de abbabr.cpp

======================== Contenido ========================

  Este modulo detecta y expande abreviaturas, acronimos y
unidades reflejadas en el diccionario.

  Los patrones que presentas estas formas son heterogeneos,
pudiendo ser:
- Para abreviaturas:
   lp     Sr.
   lplp   S.L.
   lplplp S.A.R.
- Para acronimos:
   l --> Generalmente en mayusculas.
- Para unidades:
   l --> no incluyen punto al final.

Regla generica -> letras y puntos repetidos uno tras otro.

En el caso de las unidades se tendrá en cuenta que se puede
ser plural y singular:
   1 m -> un metro.
   2 m -> dos metro_S.

Para decidir si se puede eliminar el punto correspondiente a un
acrónimo o abreviatura se necesita tener en ese momento el
grupo siguiente, para ver si es una letra mayúscula o minúscula.
Si este grupo siguiente no existe, se para la normalización hasta
que entren más datos.


inigos.
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include "abbacr.hpp"
#include "es_t2l.hpp"
#include "choputi.h"

#include "httsmsg.h"
#include "chset.h"

#include "es_hdic.hpp"

/**********************************************************/

/**********************************************************/
AbbAcrUniType	LangES_TextToList::isAbbAcrUni( CtI p )
{
	AbbAcrUniType retval = AAU_UNKNOWN;

	pCHAR pattern;
	CtI q;

	UINT i;
	BOOL strPunt = FALSE; //FALSE -> looking for 'l' pattern. TRUE -> looking for 'p' pattern
	BOOL patternOk = TRUE;
	pCHAR temp = NULL;

	INT len, abbacr;
	HDicRef myref;

#ifdef HTTS_DEBUG
    if (!ct(p).pattern) htts_error("LangES_TextToList::isAbbAcrUni() - Trying to read null Pattern.");
#endif

	if (ct(p).pattern) pattern = strdup(ct(p).pattern);

	else return retval;

	q = p;

 
	//Buscamos patrones de tipo AbbAcrUnit validos.
	for (i=0; i < strlen(pattern); i++)
	{
  		if (!strPunt)
		{
	    	if ( pattern[i] != 'l' )
			{
				patternOk = FALSE;
			}
			q=ct.next(q);
		}
		else
		{
			if ( pattern[i] != 'p' )
			{
				patternOk = FALSE;
			}
			else if ( strlen(ct(q).str) != 1 )
			{
				patternOk = FALSE;
			}
			else if ( (ct(q).str)[0] != '.' )
			{
				patternOk = FALSE;
			}
			q = ct.next(q);
		}
	 	if (!patternOk) break;
		strPunt = !strPunt;
	}

	/*if (!patternOk) //Iñaki, para que siga mirando el resto de abreviaturas
	{
  		xfree(pattern);
		return retval;
	}*/

	//Construimos la posible abb, acr, uni.
	q = p;
	temp = strdup(ct(q).str);
	for ( i = 1; i < strlen(pattern); i++)
	{
		q=ct.next(q);
		temp = concatStr(temp, ct(q).str);
	}
	free(pattern); // Ya no nos hace falta.

  //La buscamos en el diccionario.
	myref = dic->search(temp);
	len = dic->query(myref, HDIC_QUERY_MATCHLEN);
	if (!len) {
		abbacr = dic->query(myref, HDIC_QUERY_ES_NOR);
		if (abbacr) {
			if (abbacr == HDIC_ANSWER_ES_NOR_ABB) retval = AAU_ABB;
			if (abbacr == HDIC_ANSWER_ES_NOR_UNIT) retval = AAU_UNIT;
			if (abbacr == HDIC_ANSWER_ES_NOR_ACR) retval = AAU_ACR;
		}
	}
	else
	{
		// Por problemas con el punto final (si pertenece a una breviatura)
		// Miramos si poniendole un punto al final coincide con alguna abb

		temp = concatStr(temp,(char *) ".");

		//La volvemos a buscar en el diccionario.

		myref = dic->search(temp);
		
		len = dic->query(myref, HDIC_QUERY_MATCHLEN);
		if (!len)
		{
			abbacr = dic->query(myref, HDIC_QUERY_ES_NOR);
			if (abbacr)
			{
				if (abbacr == HDIC_ANSWER_ES_NOR_ABB) retval = AAU_ABB;
			}
		}
	}

	//Si no, todavía puede que con algún punto cortado, ...
	if (retval == AAU_UNKNOWN)
	{
  		q=p;
  		if ( ct.nextGrp(p) )
			if (ct.isRightFrg(ct.nextGrp(p)))
			{
				q = ct.nextGrp(p);
				if ((strlen(ct(q).pattern) == 1) && ((ct(q).pattern)[0] == 'p'))
					if ((ct(q).str)[0] == '.')
					{
						temp = concatStr(temp,(char *) ".");
						myref = dic->search(temp);
						
						len = dic->query(myref, HDIC_QUERY_MATCHLEN);
						if (!len) 
						{
//EVA Se estaba preguntando por la etiqueta en euskera, por eso no encontraba nada!!
//							abbacr = dic->query(myref, HDIC_QUERY_EU_NOR);
							abbacr = dic->query(myref, HDIC_QUERY_ES_NOR);
							if (abbacr) 
							{
								if (abbacr == HDIC_ANSWER_ES_NOR_ABB) retval = AAU_ABBP;
								if (abbacr == HDIC_ANSWER_ES_NOR_UNIT) retval = AAU_UNITP;
								if (abbacr == HDIC_ANSWER_ES_NOR_ACR) retval = AAU_ACRP;
								//Verificamos que se pueda resolver el punto.
								if (moreDataNeedAllowed)
								{
									if (!ct.nextGrp(q)) retval = AAU_NEEDMOREDATA;
									else
									{
										q = ct.nextGrp(q);
										if (!ct(q).pattern) 
											retval = AAU_NEEDMOREDATA;
									}
								}
							}
						}
					}
			}
	}
	
  free(temp);


	return retval;
}

/**********************************************************/


/**********************************************************/

CtI LangES_TextToList::expAbbAcrUni( CtI p, BOOL unit )
{
	CtI q;
	pCHAR temp = NULL;
	pCHAR temp2 = NULL;
	UINT unitMult = 0; // 0: No Unidad; 1: Unidad; 2: Multiple
	UINT i;
	HDicRef myref;
	pCHAR exp;

	INT stat, emotion, emo_intensity;
	INT count, j;
	pCHAR *words;

	stat = ct.getStatus(p);
	emotion = ct.getEmotion(p);
	emo_intensity = ct.getEmo_int(p);

	//Reconstruimos la posible abb, acr, uni.
	q = p;
	temp = strdup(ct(q).str);

	for ( i = 1; i < strlen(ct(p).pattern); i++)
	{
		q=ct.next(q);
		temp = concatStr(temp, ct(q).str);
	} 

	//Si el punto de la deteccion esta en el grupo contiguo
	//lo eliminamos. Cuidadín!!!!!!!!

	if(!unit && temp[strlen(temp)-1] != '.')
	{
		temp = concatStr(temp,(char *)".");
//EVA: Si lo unico que hay en el texto de entrada es una abreviatura y no hay punto (c/c por ejemplo) no existe un grupo siguiente, asi que no debemos apuntarlo
                if (ct.nextGrp(p)!=NULL)
                {
			q=ct.nextGrp(p);
			if(strlen(ct(q).str) != 1)
			{
				temp2 = strdup(ct(q).str);
//EVA: Solo eliminar la primera letra si es el punto
				if(temp2[1]=='.')
				ct.setStr(q, &(temp2[1]));
				free(temp2);
			}
			//Si no hay que verificar que el siguiente grupo está en mayúsculas.
			else if (ct.nextGrp(q))
			{
				q = ct.nextGrp(q);
				if ((ct(q).pattern)[0] == 'l')
				{
					if (ct.getStr(q)[0] == chset_ToLower(ct.getStr(q)[0]) )
					{
						q = ct.prevGrp(q);
						ct.delGrp(q);
					}	
				}
			}
		}
	}

  //La buscamos en el diccionario. No chequemos nada ya que
  //ya se hizo en la detección.
	myref = dic->search(temp, &exp);
	
  //Si es una unidad miramos si lo anterior es un numero normalizado,
  //y si es el número uno. (solo unidades).
	if (unit)
	{
  		q = p;
		if (ct.prevGrp(q))
		{
			q = ct.prevGrp(q);
			if ((ct.getTnor(q) == UTYPENOR_NUMBER) && (ct.next(q) == ct.nextGrp(q)))
			{
				if(strcmp(ct.getStr(q), "uno")==0)
				{
					unitMult = 1;
				}
				else
				{
					unitMult = 2;
				}
			}
		}
	}
  
	q=p;
	q=ct.lastInGrp(q);
	p=q;

	count = str2wrdLst(exp, &words);
	for (j=0; j<count; j++) q = ct.insafter(q, words[j], FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu

	
	if(unitMult == 2)  // Si no es la unidad hay que ponerlo en plural (2 kilometroS)
	{
		q = ct.extendStr(q,(char *) "s");
	}
	else  // Si es la unidad cambiar "uno" por "un"
	{
		if(unitMult == 1)
		{
			ct.setStr(ct.prev(p),(char *) "un");
		}
	}

	freewrdLst(count, words);

	//Trasladamos la frontera y eliminamos sobrantes.

	q=ct.next(p);
	ct.setPatternForce(q,(char *) "l");
	ct.setStatus(q, stat);
	ct.setTnor(q, UTYPENOR_ACROABBUNI);

	q=ct.prevGrp(q);
	q=ct.delGrp(q);


	free(exp);
	free(temp);

	return q;
}
/**********************************************************/

