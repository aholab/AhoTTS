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

Nombre fuente................ eu_units.cpp
Nombre paquete............... normalizador
Lenguaje fuente.............. C++
Estado....................... desarrollo
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Juan Luis Murugarren

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
0.0.1	 09/03/07  inaxio    Zuzenketa zkia. eta moneta batera doazenerako:45$
0.0.0    22/03/01  Yon2.     Codificacion inicial.

======================== Contenido ========================
	Modulo para normalizar unidades no contenidas en el
diccionario. Las unidades normalizadas son:
	- dolar.
	- libra.
	- yen.

Yon2.
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include "eu_t2l.hpp"
#include "symbolexp.h"
#include "choputi.h"

#include "httsmsg.h"

/**********************************************************/

/**********************************************************/

CtI LangEU_TextToList::expUnit( CtI p, BOOL junto )//inaxio-> BOOL junto gehitu
{
	CtI q;
	BOOL unitSwap = FALSE;
	pCHAR exp;

	INT count, j;
	pCHAR pattern, unidad, *words;
	INT stat, emotion, emo_intensity, patlen;

	stat = ct.getStatus(p);
	emotion = ct.getEmotion(p);
	emo_intensity = ct.getEmo_int(p);
	CHAR unit;

	//si es del tipo ns (45$)
	if(junto){

		if (ct(p).pattern)
			pattern = strdup(ct(p).pattern);
		patlen = strlen(pattern);


		if(patlen==2) q=ct.next(p);
		else if(patlen==4) q=ct.next(ct.next(ct.next(p)));

		unit =  (ct.getStr(q))[0];
		if(unit=='$') unidad=(char *)"dolar";
		else if(unit=='£') unidad=(char *)"libra esterlina";
		else if(unit == '¥') unidad=(char *)"yen";
		else if(unit == '¤') unidad=(char *)"euro";
		ct.del(q);
		q=p;
		//ns (45$)
		if(patlen==2)
		{
			if(chu_str2num(ct.getStr(p))==1)
				unitSwap = TRUE;
			if(unitSwap){
				q=expnum(p);

				ct.setPatternForce(q, (char *)"l");
				ct.setStatus(q, stat);
				ct.setTnor(q, UTYPENOR_NUMBER);

				q = ct.insafter(q, unidad, FALSE, emotion, emo_intensity);
				ct.setPatternForce(q, (char *)"l");
				ct.setStatus(q, stat);
				ct.setTnor(q, UTYPENOR_UNIT);


			}
			else{
				q = ct.insafter(q, unidad, FALSE, emotion, emo_intensity);
				ct.setPatternForce(q, (char *)"l");
				ct.setStatus(q, stat);
				ct.setTnor(q, UTYPENOR_UNIT);
				q=expnum(p);

				ct.setPatternForce(q, (char *)"l");
				ct.setStatus(q, stat);
				ct.setTnor(q, UTYPENOR_NUMBER);

			}

			q=ct.next(p);
			ct.setPatternForce(q, (char *)"l");
			ct.setStatus(q, stat);
			ct.setTnor(q, UTYPENOR_UNIT);

			q=ct.prevGrp(q);
			q=ct.delGrp(q);
		}
		//npns(45,76$)
		else if(patlen==4)
		{

			q=ct.lastInGrp(q);
			q=ct.insafter(q, unidad, FALSE, emotion, emo_intensity);
			q=expnum(ct.prev(q));
			q=ct.del(q);
			if (strcmp(ct.getStr(ct.prev(q)), ".") == 0)
				q=ct.insafter(ct.prev(q),(char *)"puntu",FALSE, emotion, emo_intensity);
			else if (strcmp(ct.getStr(ct.prev(q)), ",") == 0)
				q=ct.insafter(ct.prev(q),(char *)"koma",FALSE, emotion, emo_intensity);
			q=ct.del(ct.prev(q));
			q=expnum(ct.prev(q));
			q=ct.del(q);
			ct.setPattern(q, (char *)"l");
			ct.setStatus(q,stat);
			ct.setTnor(q, UTYPENOR_UNIT);
		}

		else return p;
	}

	//si es de tipo n s (45 $)

	else{

		exp = eu_getchexp( ct.getStr(p)[0] );

 		//Si es el n£mero uno hay que hacer swap.
		q = p;
		if (ct.prevGrp(q))	{
			q = ct.prevGrp(q);
			if ((ct.getTnor(q) == UTYPENOR_NUMBER) &&
				!strcmp(ct(q).str, "bat") &&
			(ct.next(q) == ct.nextGrp(q)) )
			unitSwap = TRUE;
		}


  		if (!unitSwap)	{
  			q=p;
	  		q=ct.lastInGrp(q);
	  		p=q;


			count = str2wrdLst(exp, &words);
			for (j=0; j<count; j++) q = ct.insafter(q, words[j], FALSE, emotion, emo_intensity);
				freewrdLst(count, words);


			//Trasladamos la frontera y eliminamos sobrantes.


	 		q=ct.next(p);
	  		ct.setPatternForce(q, (char *)"l");
	  		ct.setStatus(q, stat);
	  		ct.setTnor(q, UTYPENOR_UNIT);

	 		q=ct.prevGrp(q);
			q=ct.delGrp(q);

		}
		else {
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
	  		ct.setTnor(p, UTYPENOR_UNIT);

	  		q=ct.nextGrp(p);
	  		q=ct.nextGrp(q);
	  		q=ct.delGrp(q);
	  		q=p;
		}
	}
	return q;

}

/**********************************************************/
