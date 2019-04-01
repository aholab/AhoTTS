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

Nombre fuente................ es_pronun.cpp
Nombre paquete............... normalizador
Lenguaje fuente.............. C++
Estado....................... desarrollo
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Juan Luis Murugarren

Version   dd/mm/aa  Autor     Proposito de la edicion
-------   --------  --------  -----------------------
0.1.6	  31/05/11  inaki     Usar un array temporal con strcpy en vez de utilizar la misma memoria en origen y destino --> funcion filterStr
0.1.5	  20/10/08  inaki     Añadir grupos pronunciables: HF,OPL,NOGP
0.1.5	  29/11/06  inaxio    'ph'->'f'
0.1.4	  11/10/06  inaxio    Added more words -> new groups.
0.1.4	  09/10/06  inaxio    "ns" final pronunciable (fans)
0.1.3	  09/10/06  inaxio    "nll" en medio pronunciable (conllevar)
0.1.2	  19/09/06  inaxio    "j" final pronunciable (reloj)
0.1.1	  06/10/04  inigos    "ms" final pronunciable (albums)
0.1.0	  15/03/03  inigos    Adaptacion al castellano ü
0.0.26    19/10/01  Yon2.     Added more words -> new groups.
0.0.25    18/10/01  Yon2.     Added more words -> new groups.
0.0.24    10/10/01  Yon2.     ONG pronounceable!!!!!!!!!!!!! -> New groups.
0.0.22    16/07/01  Yon2.     Added more words -> new groups.
0.0.21    11/07/01  Yon2.     EHZ -> no pronun -> solo quitar la h si lo siguiente es una vocal.
0.0.20    11/07/01  Yon2.     Added more words -> new groups.
0.0.19    11/07/01  Yon2.     'y' -> if cons before is vowel else if vowel after is cons
0.0.18    10/07/01  Yon2.     'j' -> new group.
0.0.17    30/05/01  Yon2.     'y' maybe vowel. 'ss' -> 's'
0.0.16    30/05/01  Yon2.     Added more words -> new groups.
0.0.15    29/05/01  Yon2.     Genesys support through defines.
0.0.14    04/05/01  Yon2.     Right Warning/Error info.
0.0.13    04/05/01  Yon2.     Split in language dependent files.
0.0.12    25/04/01  Yon2.     Multiling support.
0.0.11    09/04/01  Yon2.     Added more words -> new groups.
0.0.10    09/04/01  Yon2.     Added more words -> new groups.
0.0.09    09/04/01  Yon2.     Added more words -> new groups.
0.0.08    09/04/01  Yon2.     Added more words -> new groups.
0.0.07    08/04/01  Yon2.     Missing 's' letter group.
0.0.06    07/04/01  Yon2.     Splitting de los grupos en otros más interesantes.
0.0.05    30/03/01  Yon2.     Si solo hay un grupo y no es vocal=> no pronun.
0.0.04    30/03/01  Yon2.     Prono/Not prono completed.
0.0.03    30/03/01  Yon2.     Deduction of pronounceable groups.
0.0.02    30/03/01  Yon2.     Does The same for capitals & no capitals.
0.0.01    30/03/01  Yon2.     Funciones str2GrpList & freeGrpList
0.0.00    13/12/00  Yon2.     Codificacion inicial.

======================== Contenido ========================

	Dada una palabra compuesta por caracteres deducir
si es pronunciable o no lo es.
	El método seguido es el siguiente:

  - Se duplica la palabra.
  - Se convierte a min£sculas.
  - Se filtra :
  	  - Se eliminan las 'h'-s.
      - Se cambia ca, co, cu por ka, ko, ku.
      - Se cambia ce, ci por ze, zi.
      - Se cambia ch por tx.
      - Se considera si la 'y' actúa como vocal o consonante.
      - Se convierte 'ss' a 's' unica -> Muy usual en nombres propios en
        otros idiomas.
  - Se traduce la palabra a grupos nasales, dentales, oclusivas,
  	liquidas y liquidas dobles y vocales. Y subgrupos de éstas.
  - Se comprueba que los grupos consonanticos al inicio, en el
  	interior de la palabra y al final de esta pertenezcan a
    los grupos determinados como pronunciables en cada
    localización.

	Para la localización de los grupos consonanticos pronunciables
se ha analizado una grupo de 6613 palabras correspondientes a
textos en euskara.

	Conclusiones del an lisis:
  - No hay grupos de 5 ó más consonantes pronunciables.
  - Los grupos de cuatro y tres consonates no forman parte ni
  	del comienzo ni del final de las palabras.
  - No hay ninguna palabra que empiece por liquida doble. !!!!
  Aunque deber¡a ser cierto pues en euskara no hay 'll', es 'il'
  existen nombres propios como "Llodio", que modifican este
  comportamiento, as¡ como las palabras castellanas.
  - Los dem s resultados se reflejan en las tablas de grupos
  v lidos.

  6613 palabras son muchas, pero no son todas. :)
	Ahora son 6626.
	Ahora son 6634.
	Ahora son 6641.
  Ahora son 66xx.


Yon2.
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

/**********************************************************/


/**********************************************************/

#include <stdio.h>
#include <stdlib.h>

#include <tdef.h>
#include <string.h>

#include <chset.h>
#include <httsmsg.h>

#include "es_t2l.hpp"
#include "choputi.h"

/**********************************************************/

#define DEBUG_PRONUN
#define USE_GENESYSx

/**********************************************************/

CHAR es_nasaStr[] = "n";
CHAR es_nasa1Str[] = "m";
CHAR es_ocluStr[] = "b g v w";
CHAR es_oclu1Str[] = "t k d p";
CHAR es_oclu2Str[] = "tt";
CHAR es_oclu3Str[]= "ñ";//inaxio->se ha separado la 'ñ' de es_ocluStr.
CHAR es_liqsStr[] = "l r";
CHAR es_liqdStr[] = "ll";
CHAR es_liqd1Str[] = "rr";
CHAR es_fricStr[] = "z f y";
CHAR es_fric1Str[] = "s";
CHAR es_fric2Str[] = "tz ts tx";
CHAR es_fric3Str[] = "j";
CHAR es_fric4Str[] = "x";//inaxio->se ha separado la 'x' de es_fricStr.

enum es_PronGrp {
	es_GRP_UNKNOWN='X',
	es_GRP_NASA='N',
	es_GRP_NASA1='M',
	es_GRP_OCLU='O',
	es_GRP_OCLU1='P',
	es_GRP_OCLU2='Q',
	es_GRP_OCLU3='B',//inaxio->añadido para la 'ñ'
	es_GRP_LIQS='L',
	es_GRP_LIQD='D',
	es_GRP_LIQD1='E',
	es_GRP_FRIC='F',
	es_GRP_FRIC1='G',
	es_GRP_FRIC2='H',
	es_GRP_FRIC3='I',
	es_GRP_FRIC4='J',//inaxio->añadido para la 'x'
	es_GRP_VOWEL='V'
};

CHAR es_validOneStaStr[] = "D F J G H I L M N O B P Q";
CHAR es_validOneMidStr[] = "D E F J G H I L M N O B P Q";
CHAR es_validOneEndStr[] = "D F J G H L M N O B P"
			   "I";//inaxio

CHAR es_validTwoStaStr[] = "FL JL OL BL PG PL PN";
CHAR es_validTwoMidStr[] = "FF FJ JF JJ FH JH FI JI FL JL FM JM FN JN FO JO FB JB FP JP GF GJ GH GI GL GM GN GO GB GP HO HB LF LJ LG LH LI LL LM LN LO LB LP ML MM MN MO MB MP NF NJ"
			   "NG NH NI NL NM NN NO NB NP NQ OG BG OI BI OL BL ON BN OM BM OP BP PF PJ PG PH PL PM PN PO PB PP "
			   "PI OF ND HP HG HF";//inaxio

CHAR es_validTwoEndStr[] = "FP JP GP LF LJ LH LP NF NJ NH NP MG"
			   "OG NG LG LM";//inaxio
CHAR es_validThreeStaStr[] = "";
CHAR es_validThreeMidStr[] = "FPL JPL GFL GJL GOL GBL GPL GPM GPO GPB LFL LJL LFO LJO LFB LJB LFP LJP LGP LHM LOL LBL LPL MOL MBL MPL NFL NJL NFP NJP NGF NGJ NGH NGI NGL NGM NGN NGO NGB NGP NOL NBL NPL NPP OGP BGP PPL"
			     "OGF HPL OPL";//inaxio
CHAR es_validThreeEndStr[] = "";

CHAR es_validFourStaStr[] = "";
CHAR es_validFourMidStr[] = "NGOL NGBL NGPL OGPL BGPL"
			    "PGPL NOGP";//inaxio  //INAKI NOGP (GANGSTER)
CHAR es_validFourEndStr[] = "";


/**********************************************************/


/**********************************************************/
//Crea un array de cadenas con vocales y grupos de consonantes
//que provienen de una cadena con multiples grupos. Hay que
//liberar toda la reserva hecha. Se hace con la funcion freeGrpLst().

INT es_str2GrpLst(pCHAR srcStr, pCHAR *Grp[])
{
	pCHAR wrkStr;
	pCHAR tempStr;
	INT strl;
	INT i,j,k;
	INT count = 0;
	BOOL vowelPrev = FALSE, vowel;

	wrkStr = strdup(srcStr);
	strl = strlen(wrkStr);
	tempStr = (pCHAR)malloc((strl + 1) * sizeof(CHAR));

	//Calculamos el numero de grupos Vocal/Consonante total. Dos flancos.

	if (wrkStr[0] == 'V') vowelPrev = TRUE;
  else vowelPrev = FALSE;

  for (i=1; i < strl; i++)	{
		vowel = (wrkStr[i] == 'V')?TRUE:FALSE;
		if ((!vowelPrev && vowel) || (vowelPrev && !vowel)) count++;
		vowelPrev = vowel;
	}


	count++;

	*Grp = (pCHAR *)malloc(count * sizeof(pCHAR));

	//Buscamos el numero de grupos tanto de vocales como de consonantes.
	if (wrkStr[0] == 'V') vowelPrev = TRUE;
  else vowelPrev = FALSE;

  j=k=0;

  tempStr[j++] = wrkStr[0];

	for (i=1; i < strl; i++)	{
		vowel = (wrkStr[i] == 'V')?TRUE:FALSE;
		if ( (!vowelPrev && vowel) || (vowelPrev && !vowel) ) {
			tempStr[j] = '\x0';
			j=0;
			(*Grp)[k++] = strdup(tempStr);
		}
		vowelPrev = vowel;
		tempStr[j++] = wrkStr[i];
	}
	//El último grupo.
	tempStr[j] = '\x0';
	(*Grp)[k] = strdup(tempStr);



	free(tempStr);
	free(wrkStr);
	return count;
}
/**********************************************************/


/**********************************************************/
//Libera la memoria reservada por str2GrpList().
//Una llamada a str2GrpList -> una llamada a freewrdLst
//no vale dos o tres llamadas a str2wrdList y una a freewrdLst

VOID es_freeGrpLst(INT count, pCHAR Grp[])
{
	INT i;
	for (i=0; i<count; i++) free(Grp[i]);
	free(Grp);
}

/**********************************************************/


/**********************************************************/

BOOL es_isDoubleWide(pCHAR str)
{
	BOOL retval = FALSE;

	CHAR tmp[]="XX";

	tmp[0] = str[0];
	tmp[1] = str[1];

	if (strstr(es_oclu2Str, tmp)) retval = TRUE;
	if (strstr(es_liqdStr, tmp)) retval = TRUE;
	if (strstr(es_liqd1Str, tmp)) retval = TRUE;
	if (strstr(es_fric2Str, tmp)) retval = TRUE;

	return retval;
}

/**********************************************************/


/**********************************************************/

CHAR es_getGroup(pCHAR str, BOOL dblGrp)
{
	CHAR retval = es_GRP_UNKNOWN;
	CHAR group[]="XX";

	if (!dblGrp)	{
		group[0] = str[0];
		group[1] = '\x0';
	}
	else	{
		group[0] = str[0];
		group[1] = str[1];

	}

	chset_StrLower(group);

	if (strstr(es_nasaStr, group)) return es_GRP_NASA;
	if (strstr(es_nasa1Str, group)) return es_GRP_NASA1;
	if (strstr(es_ocluStr, group)) return es_GRP_OCLU;
	if (strstr(es_oclu1Str, group)) return es_GRP_OCLU1;
	if (strstr(es_oclu2Str, group)) return es_GRP_OCLU2;
	if (strstr(es_oclu3Str, group)) return es_GRP_OCLU3;//inaxio
	if (strstr(es_liqsStr, group)) return es_GRP_LIQS;
	if (strstr(es_liqdStr, group)) return es_GRP_LIQD;
	if (strstr(es_liqd1Str, group)) return es_GRP_LIQD1;
	if (strstr(es_fricStr, group)) return es_GRP_FRIC;
	if (strstr(es_fric1Str, group)) return es_GRP_FRIC1;
	if (strstr(es_fric2Str, group)) return es_GRP_FRIC2;
	if (strstr(es_fric3Str, group)) return es_GRP_FRIC3;
	if (strstr(es_fric4Str, group)) return es_GRP_FRIC4;//inaxio
	if (!dblGrp) if (isVowel(group[0])) return es_GRP_VOWEL;

	return retval;
}

/**********************************************************/


/**********************************************************/
//Prepara una cadena para ver si es pronunciable.
//Quita las haches ca -> Ka, ce -> ze, ci -> zi, ...
//Si qu -> K, si q sola no se puede.
//Mirar la h con la ch.	-> 'ch' pasa a ser 'tx'
pCHAR es_filterStr(pCHAR str)
{
	INT i;
	INT strl;

	strl = strlen(str);
	pCHAR tmp; //inaki
	for (i=0; i < strl; i++)	{
		switch (uniqConso(str[i]))	{

			case 'h':
				switch(uniqVowel(str[i+1]))	{
					case 'a':
					case 'e':
					case 'i':
					case 'o':
					case 'u':
						//strcpy(&(str[i]), &(str[i+1]));
						tmp = strdup(str);//inaki, usamos array temporal para evitar problemas
			                        strcpy(&(str[i]), &(tmp[i+1]));//inaki
                        			free(tmp);//inaki
						strl--;
						i--;
						break;
					default:
						break;
				}
				break;

			case 's':
				if (uniqConso(str[i+1]) == 's')	{
					//strcpy(&(str[i]), &(str[i+1]));
					tmp = strdup(str);//inaki, usamos array temporal para evitar problemas
			                strcpy(&(str[i]), &(tmp[i+1]));//inaki
                        		free(tmp);//inaki
					strl--;
				}
				break;

			case 'y':
				if (i)	{
					switch(uniqVowel(str[i-1]))	{
						case 'a':
						case 'e':
						case 'i':
						case 'o':
						case 'u':
							break;
						default:
							str[i]='i';
						break;
					}
				}
				else	{
					switch(uniqVowel(str[i+1]))	{
						case 'a':
						case 'e':
						case 'i':
						case 'o':
						case 'u':
							break;
						default:
							str[i]='i';
							break;
					}
				}
				break;

			case 'c':
				switch (uniqConso(str[i+1]))	{
					case 'e':
					case 'i':
						str[i]='z';
						break;
					case 'h':
						str[i]='t';
						str[i+1]='x';
						break;
					default:
						str[i]='k';
						break;
				}
				break;

			case 'q':
				switch (uniqConso(str[i+1]))	{
					case 'u':
						//strcpy(&(str[i]), &(str[i+1]));
						tmp = strdup(str);//inaki, usamos array temporal para evitar problemas
			                        strcpy(&(str[i]), &(tmp[i+1]));//inaki
                        			free(tmp);//inaki
						str[i] = 'k';
						strl--;
						break;
					default:
						break;
				}
				break;

			case 'p':	//inaxio para que ph->f
				switch (uniqConso(str[i+1]))	{
					case 'h':
						//strcpy(&(str[i]), &(str[i+1]));
						tmp = strdup(str);//inaki, usamos array temporal para evitar problemas
			                        strcpy(&(str[i]), &(tmp[i+1]));//inaki
                        			free(tmp);//inaki
						str[i] = 'f';
						strl--;
						break;
					default:
						break;
				}
				break;

			default:
				break;
		}


	}

	return str;
}

/**********************************************************/


/**********************************************************/
//Pasa de una cadena a otra solo con contenido por grupos:
// fricativas, oclusivas, liquidas, liquidas dobles y vocales.
// también devuelve consonantes no recogidas en los grupos
// -> Verificar cuales.
//Hay que liberar después la cadena que devuelve.

pCHAR es_str2grpStr(pCHAR str)
{
	pCHAR grpStr;
	pCHAR fltStr;
	INT strl;
	INT i,j;
	BOOL doublGrp;

	fltStr = strdup(str);
  chset_StrLower(fltStr);
	es_filterStr(fltStr); //Hacemos desaparecer las "h" acomodamos la c, la q;

	strl = strlen(fltStr);

  if (!strl) {
    free(fltStr);
    return NULL;
  }

	grpStr = (pCHAR)malloc( (strl + 1) * sizeof(CHAR) );

	j=0;
	for (i=0; i < strl; i++)	{
		doublGrp = es_isDoubleWide(&(fltStr[i]));
		grpStr[j++] = es_getGroup(&(fltStr[i]), doublGrp);
		if (doublGrp) i++;
	}
	grpStr[j] = '\x0';

	free(fltStr);
	return grpStr;
}

/**********************************************************/


/**********************************************************/
BOOL LangES_TextToList::isPronun(CtI p)
{
  BOOL wordPronun = FALSE;
  pCHAR grpStr;

	pCHAR *Grp;
	INT cuenta;
	INT i;

	grpStr = es_str2grpStr(ct.getStr(p));
#ifdef HTTS_DEBUG
	if (!grpStr) htts_warn("LangES_TextToList::isPronun() - No group string returned for %s", ct.getStr(p));
#endif
  if (!grpStr) return wordPronun;


	cuenta = es_str2GrpLst(grpStr, &Grp);

  //Verificamos que tratamos con grupos pronunciables. validOneSta
  wordPronun = TRUE; //Damos un voto de confianza a la palabra.


#ifndef USE_GENESYS
  //Si solo hay un grupo y es de consonantes, no se puede pronunciar.
  if ((cuenta == 1) && ((Grp[0])[0] != 'V')) wordPronun = FALSE;

	for ( i = 0; i < cuenta; i++ ) {
		if ( (Grp[i])[0]  != 'V')	{
    	switch (strlen(Grp[i])){
      	case 1:
					if (!i) {
            if (!strstr(es_validOneStaStr, Grp[i])) wordPronun = FALSE;
          }
          else {
            if (i == (cuenta -1)) {
              if (!strstr(es_validOneEndStr, Grp[i])) wordPronun = FALSE;
            }
            else if (!strstr(es_validOneMidStr, Grp[i])) wordPronun = FALSE;
          }
          break;
        case 2:
					if (!i) {
            if (!strstr(es_validTwoStaStr, Grp[i])) wordPronun = FALSE;
          }
          else {
            if (i == (cuenta -1)) {
               if (!strstr(es_validTwoEndStr, Grp[i])) wordPronun = FALSE;
            }
            else if (!strstr(es_validTwoMidStr, Grp[i])) wordPronun = FALSE;
          }
          break;
        case 3:
					if (!i) {
            if (!strstr(es_validThreeStaStr, Grp[i])) wordPronun = FALSE;
          }
          else {
            if (i == (cuenta -1)) {
              if (!strstr(es_validThreeEndStr, Grp[i])) wordPronun = FALSE;
            }
            else if (!strstr(es_validThreeMidStr, Grp[i])) wordPronun = FALSE;
          }
          break;
        case 4:
					if (!i) {
            if (!strstr(es_validFourStaStr, Grp[i])) wordPronun = FALSE;
          }
          else {
            if (i == (cuenta -1)) {
              if (!strstr(es_validFourEndStr, Grp[i])) wordPronun = FALSE;
            }
            else if (!strstr(es_validFourMidStr, Grp[i])) wordPronun = FALSE;
          }
          break;
        case 5:
					wordPronun = FALSE;
          break;
        default:
					wordPronun = FALSE;
        	break;
      }
    }
	}


#else

	for ( i = 0; i < cuenta; i++ ) {
		if ( (Grp[i])[0]  != 'V')	{
    	switch (strlen(Grp[i])){
      	case 1:
					if (!i) htts_warn("_one Pro:: _Sta %s -> %s", Grp[i], ct.getStr(p));
						else if (i == (cuenta -1)) htts_warn("_one Pro:: _End %s -> %s", Grp[i], ct.getStr(p));
						else htts_warn("_one Pro:: _Mid %s -> %s", Grp[i], ct.getStr(p));
          break;
        case 2:
					if (!i) htts_warn("_two Pro:: _Sta %s -> %s", Grp[i], ct.getStr(p));
						else if (i == (cuenta -1)) htts_warn("_two Pro:: _End %s -> %s", Grp[i], ct.getStr(p));
						else htts_warn("_two Pro:: _Mid %s -> %s", Grp[i], ct.getStr(p));
          break;
        case 3:
					if (!i) htts_warn("_thr Pro:: _Sta %s -> %s", Grp[i], ct.getStr(p));
						else if (i == (cuenta -1)) htts_warn("_thr Pro:: _End %s -> %s", Grp[i], ct.getStr(p));
						else htts_warn("_thr Pro:: _Mid %s -> %s", Grp[i], ct.getStr(p));
          break;
        case 4:
					if (!i) htts_warn("_fou Pro:: _Sta %s -> %s", Grp[i], ct.getStr(p));
						else if (i == (cuenta -1)) htts_warn("_fou Pro:: _End %s -> %s", Grp[i], ct.getStr(p));
						else htts_warn("_fou Pro:: _Mid %s -> %s", Grp[i], ct.getStr(p));
          break;
        case 5:
					if (!i) htts_warn("_fiv Pro:: _Sta %s -> %s", Grp[i], ct.getStr(p));
						else if (i == (cuenta -1)) htts_warn("_fiv Pro:: _End %s -> %s", Grp[i], ct.getStr(p));
						else htts_warn("_fiv Pro:: _Mid %s -> %s", Grp[i], ct.getStr(p));
          break;
        default:
        	break;
      }
    }
	}

#endif


#ifdef HTTS_DEBUG
  if (!wordPronun) htts_warn("LangES_TextToList::isPronun() word [%s] is not pronounceable. %d:%s", ct.getStr(p), cuenta, Grp[0]);
#endif


	es_freeGrpLst(cuenta, Grp);

	free(grpStr);

	return wordPronun;
}
/**********************************************************/


/**********************************************************/

