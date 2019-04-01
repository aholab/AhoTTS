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

Nombre fuente................ eu_speller.cpp
Nombre paquete............... normalizador
Lenguaje fuente.............. C++
Estado....................... desarrollo
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Juan Luis Murugarren

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
0.1.0	 05/05/03  inigos	 Renombrado a eu_speller al crear el es_speller
0.0.3    04/05/01  Yon2.     Right Warning/Error info.
0.0.2    24/04/01  Yon2.     Multiling support.
0.0.1    12/03/01  Yon2.     Se mueve del *wrdList a choputi.c.
0.0.0    11/12/00  Yon2.     Codificacion inicial.

======================== Contenido ========================

  Los grupos no normalizados se normalizan celda a celda:
  Los campos numéricos se dicen como números, los signos
se deletrean y los campos de tipo letra no se deletrean
si son pronunciables.


NOTA: El parrafo de antes no vale: los n£meros no se dicen
como n£meros, se dicen uno a uno. El resto igual.


Yon2.
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/
 
/**********************************************************/

#include <stdio.h>

#include "tdef.h"
#include "httsmsg.h"

#include "eu_t2l.hpp"
#include "symbolexp.h"
#include "choputi.h"

/**********************************************************/


/**********************************************************/

BOOL LangEU_TextToList::mustExpand(CtI p)
{
	BOOL retval = FALSE;

#ifdef HTTS_DEBUG
        if (!ct(p).pattern) htts_error("TextToList::mustExpand() - Trying to read null Pattern.");
#endif

  if (!ct(p).pattern) return retval;

	if (strlen(ct(p).pattern) > 1) retval = TRUE;
	else 
	if ( ((ct(p).pattern)[0] != 'l') && ((ct(p).pattern)[0] != 'p') )
		retval = TRUE;
	if (ct.isNorm(p)) retval = FALSE;

	return retval;
}

/**********************************************************/


/**********************************************************/

CtI LangEU_TextToList::spellCell(CtI p, CtI q)
{
	INT strl;
	INT i,j;
	INT count;
	pCHAR *words;
	INT emotion, emo_intensity;

	strl = strlen(ct(p).str);
//	fprintf(stderr,"spellCell=%s\n", ct(p).str);
	for (i=0; i < strl; i++)	{
		count = str2wrdLst(eu_getchexp((ct(p).str)[i]), &words);
		emotion=ct.getEmotion(p);
		emo_intensity=ct.getEmo_int(p);
		for (j=0; j<count; j++) q = ct.insafter(q, words[j], FALSE, emotion, emo_intensity);
		freewrdLst(count, words);
	}

	return q;
}

/**********************************************************/



/**********************************************************/

CtI LangEU_TextToList::expandGrp(CtI p)
{
	CtI q,r;
	INT patlen;
	INT i;
	INT status, emotion, emo_intensity;

	status=ct.getStatus(p);
	emotion=ct.getEmotion(p);
	emo_intensity=ct.getEmo_int(p);
	patlen = strlen(ct(p).pattern);

	q=p;
	r=ct.lastInGrp(p);

	for (i = 0; i < patlen ; i++) {
		switch ((ct(p).pattern)[i]) {
			case 'n':
				r=spellCell(q,r);
				break;
			case 's':
		 		r=spellCell(q, r);
				break;
			case 'p':
				r=spellCell(q, r);
				break;
			case 'l':
				if (isPronun(q)) r=ct.insafter(r, ct.getStr(q), FALSE, *(emo+indiceEmo), *(emoInt+indiceEmo));
				else r = spellCell(q, r);
				break;
			default:
				break;
		}
		q = ct.next(q);
	}
	
	//Eliminamos sobrantes y trasladamos la frontera.
	for(i=0; i<patlen; i++) p = ct.del(p);
	ct.setStatus(p, status);
	ct.setEmotion(p, emotion);
	ct.setEmo_int(p, emo_intensity);
	ct.setPattern(p, (char *)"l");
	ct.setTnor(p, UTYPENOR_UNKGROUP);
	
	return p;
}

/**********************************************************/

/**********************************************************/

CtI LangEU_TextToList::expandCell(CtI p)
{
	CtI q,r;
	INT patlen;
	INT i;
	INT status, emotion, emo_intensity;

	status=ct.getStatus(p);
	emotion=ct.getEmotion(p);
	emo_intensity=ct.getEmo_int(p);
	patlen = strlen(ct(p).pattern);

	q=p;
	r=ct.lastInGrp(p);

	for (i = 0; i < patlen ; i++) {
		switch ((ct(p).pattern)[i]) {
			case 'n':
				r=spellCell(q,r);
				break;
			case 's':
		 		r=spellCell(q, r);
				break;
			case 'p':
				r=spellCell(q, r);
				break;
			case 'l':
				if (isPronun(q)) r=ct.insafter(r, ct.getStr(q), FALSE, *(emo+indiceEmo), *(emoInt+indiceEmo));
        else r = spellCell(q, r);
				break;
			default:
				break;
		}
		q = ct.next(q);
	}
	
	//Eliminamos sobrantes y trasladamos la frontera.
	for(i=0; i<patlen; i++) 
		p = ct.del(p);

	ct.setStatus(p, status);
	ct.setEmotion(p, emotion);
	ct.setEmo_int(p, emo_intensity);
	ct.setPattern(p, (char *)"l");
	ct.setTnor(p, UTYPENOR_SPELL);
	
	return p;
}

/**********************************************************/

