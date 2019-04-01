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
#include "eu_t2l.hpp"
#include "httsmsg.h"

#include "romanhilvl.hpp"
#include "roman.h"
#include "chset.h"

/**********************************************************/

CHAR eu_romLehenStr[] = "lehenengo";
CHAR eu_romGarrenStr[] = "garren";

/**********************************************************/
RomanType LangEU_TextToList::isRomanN(CtI p)
{
	RomanType retval = ROMAN_UNKNOWN;
	pCHAR pattern;
	INT patlen;
	pCHAR temp;
	CtI q;

#ifdef HTTS_DEBUG
	if (!ct(p).pattern) htts_error("TextToList::isRoman(CtI) - Trying to read null Pattern.");
#endif

	if (ct(p).pattern) pattern = strdup(ct(p).pattern);
	else return retval;
	
	patlen = strlen(pattern);
	
	//Si es patron de numero romano.
 	if ((pattern[0] == 'l') && (patlen == 1)) 
	{
		if (strlen(ct(p).str) != 1)
		{
			retval = ROMAN_ROMAN;
		}
		else
		{
			free(pattern);
			return ROMAN_UNKNOWN;
		}
	}
	if (patlen == 2)
	{
		if ((pattern[0] == 'l') && (pattern[1] == 'p')) 
		{
			if (strlen(ct(p).str) != 1)
			{
				retval = ROMAN_ROMAN_P;
			}
			else
			{
				free(pattern);
				return ROMAN_UNKNOWN;
			}
		}
	}
		
			
	
	//Chequeamos que las letras correspondan a un n£mero romano.
	if (!isRoman(ct.getStr(p))) 
	{
		free(pattern);
		return ROMAN_UNKNOWN;
	}
	
	//Si no están en mayusculas no son numeros romanos.
	if (retval) 
	{
		temp = strdup(ct.getStr(p));
		chset_StrUpper(temp);
		
		if (strcmp(temp, ct.getStr(p))) 
			retval = ROMAN_UNKNOWN;
		
		free(temp);
	}
	
	if (retval == ROMAN_ROMAN)	
	{
		if ( ct.nextGrp(p) )
			if (ct.isRightFrg(ct.nextGrp(p))) 
			{
				q = ct.nextGrp(p);
				if ((strlen(ct(q).pattern) == 1) && ((ct(q).pattern)[0] == 'p'))
				{
					if (((ct(q).str)[0] == '.') && (strlen(ct(q).str) == 1)) 
					{
						if (ct.nextGrp(q))	
						{
							q=ct.nextGrp(q);
							if (ct(q).pattern)	
							{								
								if ((ct(q).pattern)[0] == 'l')									
									if (ct.getStr(q)[0] == chset_ToLower(ct.getStr(q)[0]) )
										retval = ROMAN_ROMAN_PUNT;
							}
							else if (moreDataNeedAllowed) 
								if (!ct.nextGrp(q)) 
									retval = ROMAN_NEEDMOREDATA;
						}
						else if (moreDataNeedAllowed) if (!ct.nextGrp(q)) 
							retval = ROMAN_NEEDMOREDATA;
					}
					if (((ct(q).str)[0] == '.') && (strlen(ct(q).str) > 1)) 
						retval = ROMAN_ROMAN_PUNT;
				}
				else if ((strlen(ct(q).pattern) > 1) && ((ct(q).pattern)[0] == 'p'))
					if ((ct(q).str)[0] == '.')	
						retval = ROMAN_ROMAN_PUNT;
			}
	}
	free(pattern);

#ifdef HTTS_DEBUG
#ifdef DEBUG_ROMAN

  switch(retval)  
  {
  case ROMAN_ROMAN:
	  htts_warn("Type ROMAN_ROMAN");
	  break;
  case ROMAN_ROMAN_P:
	  htts_warn("Type ROMAN_ROMAN_P");
	  break;
  case ROMAN_ROMAN_PUNT:
	  htts_warn("Type ROMAN_ROMAN_PUNT");
	  break;
  case ROMAN_NEEDMOREDATA:
	  htts_warn("Type ROMAN_NEEDMOREDATA");
	  break;
  default:
	  htts_warn("Type ROMAN_UNKNOWN");
	  break;
  }
#endif
#endif
	return retval;
}

/**********************************************************/

CtI LangEU_TextToList::expRoman(CtI p, BOOL nearPoint, BOOL farPoint)
{
  CtI q = NULL;
  CtI r = NULL;
  INT stat;
  INT num;
  CHAR str[]="XXXXX";  //Los numeros romanos solo van hasta 3999.

  //convertimos el n£mero romano a decimal
  stat = ct.getStatus(p);
  num = romanStr2num(ct.getStr(p));
  sprintf(str, "%d", num);
  ct.setStr(p, str);
  (ct(p).pattern)[0] = 'n';

  //expandimos el numero.
  if (!nearPoint && !farPoint) 
  {
	  r = expCard(p);
	  if (!strcmp(ct.getStr(r), "bat")) 
		  ct.setStr(r, eu_romLehenStr);
	  else	
	  {
		  q=r;
		  r = ct.lastInGrp(r);
		  
		  if (((ct(r).str)[strlen(ct(r).str) - 1] == 't') &&((ct(r).str)[strlen(ct(r).str) - 2] == 's'))
			  (ct(r).str)[strlen(ct(r).str) - 1] = '\x0';
		  
		  r = ct.extendStr(r, eu_romGarrenStr);
		  r = q;
	  }
  }

  if (nearPoint && !farPoint) 
	  r = expOrd(p, FALSE);
  
  if (!nearPoint && farPoint) 
	  r = expOrd(p, TRUE);

  return r;
}

/**********************************************************/
