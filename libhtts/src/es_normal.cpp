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

Nombre fuente................ es_normal.cpp
Nombre paquete............... normalizador
Lenguaje fuente.............. C++
Estado....................... desarrollo
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Juan Luis Murugarren

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
0.1.05   02/01/07  inaxio    NUM_RESTA eta NUM_SUMA kasuak gehitu dira.
0.1.04   29/12/06  inaxio    NUM_NEGATIVE kasua gehitu da.
0.1.03	 20/11/06  inaxio    case NUM_UNIT: gehitu da.
0.1.02	 05/10/06  inaxio    Zuzenketak zenbaki ordinal pluralentzat.
0.1.01	 08/05/03  inigos    Adaptación para porcentajes
0.1.00   15/11/02  inigos    Recopia y adaptación para castellano
0.0.25   10/07/01  Yon2.     Dek bug!
0.0.24   18/06/01  Yon2.     Scape sequencies support.
0.0.23   07/05/01  Yon2.     Non dic Units.
0.0.22   07/05/01  Yon2.     Words with apost.
0.0.21   07/05/01  Yon2.     Languaje dependent filename.
0.0.20   25/04/01  Yon2.     Better Multiling support.
0.0.19   25/04/01  Yon2.     Multiling support.
0.0.18   08/04/01  Yon2.     Avoid Warnings.
0.0.17   07/04/01  Yon2.     Too Large Number.
0.0.16   30/03/01  Yon2.     spell non pron cell.
0.0.15   30/03/01  Yon2.     isPronun?
0.0.14   29/03/01  Yon2.     Percent.
0.0.13   28/03/01  Yon2.     Need More Data.
0.0.12   22/03/01  Yon2.     Abb, acr, units.
0.0.11   18/03/01  Yon2.     Roman -> more funcional.
0.0.10   03/03/01  Yon2.     Comp word expansion.
0.0.09   03/03/01  Yon2.     Not normalizable (already normalized) word declination.
0.0.08   12/02/01  Yon2.     Roman not inside date.
0.0.07   04/02/01  Yon2.     Added Heavy2 Debug Warnings to normalization cicle.
0.0.06   03/02/01  Yon2.     If group normalized -> don't check for another norm
0.0.05   15/01/01  Yon2.     Crude text list empty => don't proccess.
0.0.04   08/01/01  Yon2.     Flushing support.
0.0.03   06/01/01  Yon2.     Ultimo grupo aniadido completo?
0.0.02   04/01/01  Yon2.     Real Time normalizer!!!!!!
0.0.01   xx/12/00  Yon2.     Horas, fechas, numeros, deks, ...
0.0.00   19/11/00  Yon2.     Codificacion inicial.

======================== Contenido ========================

	Normalizador propiamente dicho.
	Versi¢n en euskara.

Yon2.

===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/
  

#include "es_t2l.hpp"
#include "isofilt.h"

#include "string.h"

#include "httsmsg.h"

/**********************************************************/

CtI LangES_TextToList::normal(VOID)
{
	CtI p, firstNonP, lastToNoP;
	CtI IneedData = NULL;
	BOOL needMoreData = FALSE;
	pCHAR input;//,pattern; //ezabatu

	dataNeed = FALSE;
 

	if (lastGrpProc) firstNonP = ct.nextGrp(lastGrpProc);
	else firstNonP = ct.firstGrp();

	if (procLastGrp) lastToNoP = NULL;
	else lastToNoP = ct.lastGrp();

	for (p=firstNonP; (p!=lastToNoP) && (!needMoreData); p=ct.nextGrp(p))
	{
		
		//ABB
		//
		input = ct.getStr(p);

		if(ct.getTnor(p) == UTYPENOR_ACROABBUNI) 
		{
			p = expAbbAcrUni(p, FALSE );
		}
		else if(ct.getTnor(p) == UTYPENOR_UNKGROUP) 
		{
			p = expAbbAcrUni(p, FALSE );
		}

	
		if (ct.isNorm(p)) continue;

		if (isScapeSeq(p)) markScapeSeq(p);
		if (ct.isNorm(p)) continue;

		 

		switch (isDate(p)) {
			case DATE_SPANISH:
				dateFromSpanish(p);
				p = expdateYMD(p);
				break;
			case DATE_ROMAN_SPANISH:
				dateFromRoman(p);
				dateFromSpanish(p);
				p = expdateYMD(p);
				break;
			case DATE_UNKNOWN:
			default:
				break;
		}
		 
		if (ct.isNorm(p)) continue;

		switch(isTime(p)) {
			case TIME_SHORT:
				p = exptimeHM(p);
				break;
			case TIME_LONG:
				p = exptimeHMS(p);
				break;
			case TIME_UNKNOWN:
			default:
				break;
		}

		 
		if (ct.isNorm(p)) continue;

		switch(isNum(p))
		{
			case NUM_CARD:
				p = expCard(p);
				break;
			case NUM_ORD_ES_O:
				p = expOrd(p, FALSE, FALSE);
				break;
			case NUM_ORD_ES_OS:
				p = expOrd(p, FALSE, TRUE);// inaxio
				break;
			case NUM_ORD_ES_A:
				p = expOrd(p, TRUE, FALSE);
				break;
			case NUM_ORD_ES_AS:
				p = expOrd(p, TRUE, TRUE);// inaxio
				break;
			case NUM_UNIT:
				p = expUnit(p, TRUE);// inaxio
				break;
			case NUM_PUNT:
				p = expPunt(p, TRUE);
				break;
			case NUM_COMA:
				p = expPunt(p, FALSE);
				break;
			case NUM_NEGATIVE: //inaxio
				p = expNegative(p);
				break;
			case NUM_RESTA: //inaxio
				p = expSuma(p,false);
				break;
			case NUM_SUMA: //inaxio
				p = expSuma(p,true);
				break;
			case NUM_TOO_LARGE:
				p = expandGrp(p);
				break;
			case NUM_NEEDMOREDATA:
				IneedData = p;
				needMoreData = TRUE;
				dataNeed = TRUE;
				break;
			case NUM_CARD_PER:
				p = expCard(p);
				p = ct.nextGrp(p);
				p = expPercent( p , FALSE );
				break;
			case NUM_PUNT_PER:
				p = expPunt(p, TRUE);
				break;
			case NUM_COMA_PER:
				p = expPunt(p, FALSE);
				break;
			case NUM_UNKNOWN:
			default:
				break;
		}

			  
		if (needMoreData) continue;
		if (ct.isNorm(p)) continue;

		switch(isRomanN(p)) {
			case ROMAN_ROMAN:
				p = expRoman(p, FALSE, FALSE);
				break;
			case ROMAN_ROMAN_P:
				p = expRoman(p, TRUE, FALSE);
				break;
			case ROMAN_ROMAN_PUNT:
				p = expRoman(p, FALSE, TRUE);
				break;
			case ROMAN_NEEDMOREDATA:
				IneedData = p;
				needMoreData = TRUE;
				dataNeed = TRUE;
				break;
			case ROMAN_UNKNOWN:
			default:
				break;
		}

		 
		if (needMoreData) continue;
		if (ct.isNorm(p)) continue;

		if (isComp(p)) p = expComp(p);
		if (ct.isNorm(p)) continue;

		if (isUnit(p)) p = expUnit(p, FALSE);
		if (ct.isNorm(p)) continue;

		if (isApost(p)) p = expApost(p);
		if (ct.isNorm(p)) continue;

		 

		switch (isAbbAcrUni(p)) {
			case AAU_ABB:
//			p = expAbbAcrUni(p, FALSE, FALSE );
			//break;
			case AAU_ACR:
				p = expAbbAcrUni(p, FALSE );
				break;
			case AAU_UNIT:
				p = expAbbAcrUni(p, TRUE );
				break;
//			case AAU_ABBP:
//			case AAU_ACRP:
//				p = expAbbAcrUni(p, FALSE );
//				break;
//			case AAU_UNITP:
//				p = expAbbAcrUni(p, TRUE );
//				break;
//			case AAU_NEEDMOREDATA:
//				IneedData = p;
//				needMoreData = TRUE;
//				dataNeed = TRUE;
//				break;
			case AAU_UNKNOWN:
			default:
				break;
		}

		 
		if (needMoreData) continue;
		if (ct.isNorm(p)) continue;

		switch(isPercent(p))
		{
			case PERCENT_SPANISH:
				p = expPercent( p , TRUE );
				break;
			case PERCENT_NEEDMOREDATA:
				IneedData = p;
				needMoreData = TRUE;
				dataNeed = TRUE;
				break;
			case PERCENT_UNKNOWN:
			default:
				break;
		}

		 
		if (needMoreData) continue;
		if (ct.isNorm(p)) continue;

		if (mustExpand(p)) 
		{
			p = expandGrp(p);
		}
		
		if (ct.isNorm(p)) 
			continue;

		 

		// Mirar mayúsculas (Nombres propios...)
		switch (isCap(p)) 
		{
		case 1: // TODAS
			ct.setTnor(p, UTYPENOR_CAPITAL);
			if (!isPronun(p))
			{
				ct.setTnor(p, UTYPENOR_NAME);
				p = expandCell(p);
			}
			break;
		case 2: // Primera
			ct.setTnor(p, UTYPENOR_NAME);
			if (!isPronun(p))
			{
				p=pronounce(p);
			}
			break;
		default:
			break;
		}
		if (ct.isNorm(p)) continue;

		 

		//Palabra aislada. Es pronunciable?
		if (!isPuntt(p) && !isPronun(p))
		{
			ct.setTnor(p, UTYPENOR_NORMAL);
			p=pronounce(p);
			//p = expandCell(p);
		}
		if (ct.isNorm(p)) continue;

  	//Palabras que no necesitan normalización.
		if (!isPuntt(p) && !ct.isNorm(p))
		{
			ct.setTnor(p, UTYPENOR_NORMAL);
		}

	}

  return needMoreData?ct.prevGrp(IneedData):(procLastGrp?ct.lastGrp():ct.prevGrp(ct.lastGrp()));
}

/**********************************************************/


/**********************************************************/

INT LangES_TextToList::ct2nt(const CHAR *str, BOOL flush)
{
	INT retval = 0;
	pCHAR ibuffer=NULL;//,pattern;
	CtI lastGrp;

	if (!flush)
	{
		ibuffer = strdup(str);

		if (ibuffer) retval = strlen(ibuffer);
		else return 0;

		buff_fltr(ibuffer);
#ifdef HEAVY_DEBUG
		htts_warn("ES Input Buffer: [%s]", ibuffer);
#endif
		
		// Parser EML
		// Dentro se llama a preChop
		
		lastGrp = parserEmo (ibuffer, FALSE);
		
		if (ct.empty())
		{
			free(ibuffer);
			return retval;
		}
		
		if (!ct(ct.lastGrp()).pattern) 
			procLastGrp = FALSE;
		else procLastGrp = TRUE;
		
		free(ibuffer);
	}
	else
	{
		lastGrp = parserEmo (ibuffer, TRUE);
	}

#ifdef HEAVY_DEBUG2
	htts_warn("ES Ct2nt: Pre norm cicle.");
#endif
	
	if ((procLastGrp?ct.lastGrp():ct.prevGrp(ct.lastGrp())) != lastGrpProc)
	{

#ifdef HEAVY_DEBUG2
       htts_warn("ES Ct2nt: inside norm cicle.");
#endif
		puntChop(dic);

#ifdef HEAVY_DEBUG2
       htts_warn("ES Ct2nt: postPuntChop.");
#endif

	   lastGrpProc = normal();

#ifdef HEAVY_DEBUG2
       htts_warn("ES Ct2nt: PostNormal.");
#endif
	}

#ifdef HEAVY_DEBUG2
  htts_warn("ES Ct2nt: Post norm cicle.");
#endif

   return retval;
}

/**********************************************************/


/**********************************************************/

INT LangES_TextToList::ct2nt(BOOL flush)
{
  return flush?ct2nt("", flush):0;
}

/**********************************************************/


