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

Nombre fuente................ abbabr.cpp
Nombre paquete............... normalizador
Lenguaje fuente.............. C++
Estado....................... desarrollo
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Juan Luis Murugarren

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
0.0.7    04/05/05  Larraitz  ln eta lp erako patroaiak ere onartu
0.0.6    04/05/01  Yon2.     Right Warning/Error info.
0.0.5    25/04/01  Yon2.     Multiling support.
0.0.4    29/03/01  Yon2.     If not valid pattern in Group => Need More Data.
0.0.3    29/03/01  Yon2.     Paso al directorio principal.
0.0.2    28/03/01  Yon2.     Need More Data.
0.0.1    28/03/01  Yon2.     Quitar el punto de abb solo si lo que sigue es minuscula.
0.0.0    22/03/01  Yon2.     Codificacion inicial.

======================== Contenido ========================
	Este modulo detecta y expande abreviaturas, acronimos y
unidades reflejadas en el diccionario.

	Los patrones que presentas estas formas son heterogeneos,
pudiendo ser:
- Para abreviaturas:
	lp    zbk.
  lplp  G.b.
- Para acronimos:
	l --> Generalmente en mayusculas.
- Para unidades:
  l --> no incluyen punto al final.

Regla generica -> letras y puntos repetidos uno tras otro.

Se han encontrado tambien en el diccionario actual varias
ocurrencias extrañas, que habrá que corregir en el diccionario:
PSE-EE acr -> debería aparecer PSE por un lado y EE por otro.
zum. acr zumardi -> abreviatura o acrónimo? No importa.

Sólo en el caso de las unidades se tendrá en cuenta que se puede
alterar el orden:
   1 m -> metro bat.
   2 m -> bi metro.

Para decidir si se puede eliminar el punto correspondiente a un
acrónimo o abreviatura se necesita tener en ese momento el
grupo siguiente, para ver si es una letra mayúscula o minúscula.
Si este grupo siguiente no existe, se para la normalización hasta
que entren más datos.


Yon2.
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include "abbacr.hpp"
#include "eu_t2l.hpp"
#include "choputi.h"

#include "httsmsg.h"
#include "chset.h"

/**********************************************************/


/**********************************************************/


/**********************************************************/

AbbAcrUniType	LangEU_TextToList::isAbbAcrUni( CtI p )
{
	AbbAcrUniType retval = AAU_UNKNOWN;

	pCHAR pattern;
	CtI q;

	UINT i;
	BOOL strPunt = FALSE; //FALSE -> looking for 'l' pattern. TRUE -> looking for 'p' pattern
	BOOL patternOk = TRUE;
	pCHAR temp = NULL,input;

	INT len, abbacr;
	HDicRef myref;

#ifdef HTTS_DEBUG

	if (!ct(p).pattern)
		htts_error("LangEU_TextToList::isAbbAcrUni() - Trying to read null Pattern.");
#endif

	if (ct(p).pattern)
		pattern = strdup(ct(p).pattern);
	else
		return retval;

	q = p;

	input = ct.getStr(p);

	//Buscamos patrones de tipo AbbAcrUnit validos.
	if(strlen(pattern)==1)
	{
		if ( pattern[0] != 'l' )
		{
			patternOk = FALSE;
		}
	}
	else if(strlen(pattern)==2)
	{
		if ((pattern[0] != 'l') || ((pattern[1] != 's')&&(pattern[1] != 'n')))
		{
			patternOk = FALSE;
		}
	}
	else if((strlen(pattern)>3)&&((strlen(pattern)%2)==0))
	{

		for (i=0; i < strlen(pattern); i++)
		{
			if (!strPunt)
			{
				if ( pattern[i] != 'l' )
					patternOk = FALSE;
				q=ct.next(q);
			}
			else
			{
				if ( pattern[i] != 'p' )
						patternOk = FALSE;
					else
						if ( strlen(ct(q).str) != 1 )
							patternOk = FALSE;
						else
							if ((ct(q).str)[0] != '.' )
								patternOk = FALSE;
				q = ct.next(q);
			}

			if (!patternOk)
				break;

			strPunt = !strPunt;
		}
	}
	/*if (!patternOk)	//Iñaki, para que siga mirando el resto de abreviaturas
	{
		free(pattern);
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


	if (!len)
	{
		abbacr = dic->query(myref, HDIC_QUERY_EU_NOR);
		if (abbacr)
		{
			if (abbacr == HDIC_ANSWER_EU_NOR_ABB) retval = AAU_ABB;
			if (abbacr == HDIC_ANSWER_EU_NOR_UNIT) retval = AAU_UNIT;
			if (abbacr == HDIC_ANSWER_EU_NOR_ACR) retval = AAU_ACR;
		}
	}

	//Si no, todavía puede que con algún punto cortado, ...

	if (retval == AAU_UNKNOWN)
	{
		q=p;
		if ( ct.nextGrp(p) )
		{
			if (ct.isRightFrg(ct.nextGrp(p)))
			{
				q = ct.nextGrp(p);
				if ((strlen(ct(q).pattern) == 1) && ((ct(q).pattern)[0] == 'p'))
					if ((ct(q).str)[0] == '.')
					{
						temp = concatStr(temp, (char *)".");
						myref = dic->search(temp);

						len = dic->query(myref, HDIC_QUERY_MATCHLEN);
						if (!len)
						{
							abbacr = dic->query(myref, HDIC_QUERY_EU_NOR);
							if (abbacr)
							{
								if (abbacr == HDIC_ANSWER_EU_NOR_ABB) retval = AAU_ABBP;
								if (abbacr == HDIC_ANSWER_EU_NOR_UNIT) retval = AAU_UNITP;
								if (abbacr == HDIC_ANSWER_EU_NOR_ACR) retval = AAU_ACRP;

								//Verificamos que se pueda resolver el punto.
								if (moreDataNeedAllowed)
								{
									if (!ct.nextGrp(q))
										retval = AAU_NEEDMOREDATA;
									else
									{
										q = ct.nextGrp(q);
										if (!ct(q).pattern) retval = AAU_NEEDMOREDATA;
									}
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

CtI LangEU_TextToList::expAbbAcrUni( CtI p, BOOL withPoint, BOOL unit )
{
	CtI q;
	pCHAR temp = NULL;
	pCHAR temp2 = NULL;
	BOOL unitSwap = FALSE;
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
	////lo eliminamos. Cuidadín!!!!!!!!
	if (withPoint)
	{
		temp = concatStr(temp, (char *)".");
		q=ct.nextGrp(p);
		if (strlen(ct(q).str) != 1)
		{
			temp2 = strdup(ct(q).str);
			ct.setStr(q, &(temp2[1]));
			free(temp2);
		}
		//Si no hay que verificar que el siguiente grupo está en mayúsculas.
		else if (ct.nextGrp(q))
		{
			q = ct.nextGrp(q);
			if ((ct(q).pattern)[0] == 'l')
				if (ct.getStr(q)[0] == chset_ToLower(ct.getStr(q)[0]) )
				{
					q = ct.prevGrp(q);
					ct.delGrp(q);
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
			if ((ct.getTnor(q) == UTYPENOR_NUMBER) && !strcmp(ct(q).str, "bat") && (ct.next(q) == ct.nextGrp(q)))
				unitSwap = TRUE;
		}
	}

	if (!unitSwap)
	{
		q=p;
		q=ct.lastInGrp(q);
		p=q;


		count = str2wrdLst(exp, &words);

		for (j=0; j<count; j++)
			q = ct.insafter(q, words[j], FALSE, emotion, emo_intensity);
		freewrdLst(count, words);

		//Trasladamos la frontera y eliminamos sobrantes.



		q=ct.next(p);
		ct.setPatternForce(q, (char *)"l");
		ct.setStatus(q, stat);
		ct.setTnor(q, UTYPENOR_ACROABBUNI);


		q=ct.prevGrp(q);
		q=ct.delGrp(q);
	}
	else
	{
		q=p;
		q=ct.prevGrp(q);


		count = str2wrdLst(exp, &words);
		q = ct.insbefore(q, words[0], FALSE, emotion, emo_intensity);
		p=q;
		for (j=1; j<count; j++) q = ct.insafter(q, words[j], FALSE, emotion, emo_intensity);
		freewrdLst(count, words);

		//Trasladamos la frontera y eliminamos sobrantes.
		ct.setPatternForce(p, (char *)"l");
		ct.setStatus(p, stat);
		ct.setTnor(p, UTYPENOR_ACROABBUNI);
		q=ct.nextGrp(p);
		q=ct.nextGrp(q);
		q=ct.delGrp(q);
		q=p;
	}


	free(exp);
	free(temp);
	return q;

}
/**********************************************************/

