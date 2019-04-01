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

Nombre fuente................ es_units.cpp
Nombre paquete............... normalizador
Lenguaje fuente.............. C++
Estado....................... desarrollo
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Juan Luis Murugarren

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
0.1.3	 11/12/06  inaxio    zuzenketak n,n$ edo n.n$ kasuak ebazteko
0.1.2    29/11/06  inaxio    Expandir también cuando la unidad va unida al nº (1$)
0.1.1	 14/11/06  inaxio    Decir "una libra"(no "un") y "diez libras
			     esterlinas" (no "esterlinaes")
0.1.0    19/11/02  inigos    Adaptación al castellano
0.0.0    22/03/01  Yon2.     Codificacion inicial.

======================== Contenido ========================
	Modulo para normalizar unidades no contenidas en el
diccionario. Las unidades normalizadas son:
	- dolar.
	- libra.
	- yen.

Yon2.

	Versión para castellano, diría 10 libraes. Intentar mejorar esto.
	El euro debería venir aquí, pero hay que redefinir el mapa de caracteres.

inigos
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include "es_t2l.hpp"
#include "symbolexp.h"
#include "choputi.h"

#include "httsmsg.h"
 
/**********************************************************/

CHAR es_esStr[]="es";
CHAR es_sStr[]="s";//inaxio

/**********************************************************/

CtI LangES_TextToList::expUnit( CtI p, BOOL junto )
{
	CtI q;
	BOOL unitSwap = FALSE;
	pCHAR exp;
	CHAR unit;//inaxio
	INT stat, patlen, emotion, emo_intensity;
	INT count, j;
	pCHAR *words;
	pCHAR pattern, unidad;

	stat = ct.getStatus(p);
	emotion = ct.getEmotion(p);
	emo_intensity = ct.getEmo_int(p); 

	// Si es del tipo "ns" ("1$", "1¤",...)
	if(junto){
		if (ct(p).pattern) pattern = strdup(ct(p).pattern);
		else return(p);

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

		if(patlen==2)
		{

			if(chu_str2num(ct.getStr(p))==1)
			unitSwap = TRUE;

			if(unitSwap){
				if(unit=='£'){
					q = ct.insafter(q, (char *)"una", FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
					ct.setPatternForce(q, (char *)"l"); 
					ct.setStatus(q, stat);
					ct.setTnor(q, UTYPENOR_UNIT);

					q = ct.insafter(q, unidad, FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
				}
				else{
					q = ct.insafter(q, (char *)"un", FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
					ct.setPatternForce(q, (char *)"l"); 
					ct.setStatus(q, stat);
					ct.setTnor(q, UTYPENOR_UNIT);
				
					q = ct.insafter(q, unidad, FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu

				}
			}

			else{
		
				if(unit=='£'){
					q = ct.insafter(q, (char *)"libras esterlinas", FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
				}
				else{
					q = ct.insafter(q, unidad, FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
					if(unit=='¤')
						q = ct.extendStr(q, es_sStr);
					else
						q = ct.extendStr(q, es_esStr);
				}

				q=expnum(p);

				ct.setPatternForce(q, (char *)"l"); 
				ct.setStatus(q, stat);
				ct.setTnor(q, UTYPENOR_UNIT);
			
			}
			//Trasladamos la frontera y eliminamos sobrantes.
			q=ct.next(p);
			ct.setPatternForce(q, (char *)"l");
			ct.setStatus(q, stat);
			ct.setTnor(q, UTYPENOR_UNIT);

			q=ct.prevGrp(q);
			q=ct.delGrp(q);
		}

		else if(patlen==4)
		{
			
			q=expnum(q);
			q=ct.del(q);
			if (strcmp(ct.getStr(ct.next(q)), ".") == 0)
				q = ct.insafter(q,(char *)"punto",FALSE, emotion, emo_intensity);
			else if (strcmp(ct.getStr(ct.next(q)), ",") == 0)
				q = ct.insafter(q,(char *)"coma",FALSE, emotion, emo_intensity);
			q = ct.del(ct.next(q));
								
			if(unit=='£'){
				q = ct.insafter(q, (char *)"libras esterlinas", FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
				
			}
			else{
				q = ct.insafter(q, unidad, FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
				if(unit=='¤')
					q = ct.extendStr(q, es_sStr);
				else
					q = ct.extendStr(q, es_esStr);
			}
			
			q = expnum(ct.prev(q));
			q = ct.del(q);

			ct.setPattern(q, (char *)"l");
			ct.setStatus(q,stat);
			ct.setTnor(q, UTYPENOR_UNIT);
			
		}
		else return p;
	}

	// Si es del tipo "n s" ("1 $", "1 ¤",...)
	else{

		unit =  (ct.getStr(p))[0];//inaxio
		exp = es_getchexp( ct.getStr(p)[0] );

  		//Si es el n£mero uno hay que hacer swap.
		q = p;
		if (ct.prevGrp(q))
		{
			q = ct.prevGrp(q);
			if ((ct.getTnor(q) == UTYPENOR_NUMBER) &&
				!strcmp(ct(q).str, "uno") &&
				(ct.next(q) == ct.nextGrp(q)) )
			{
				unitSwap = TRUE;
			}
		}

  		q=p;
		q=ct.lastInGrp(q);
		p=q;
  	

		count = str2wrdLst(exp, &words);
	
		if((unitSwap && unit == '£') || unit != '£'){ //inaxio--> para que no diga 										"esterlinaes"
			for (j=0; j<count; j++)
				q = ct.insafter(q, words[j], FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
		}

		if (!unitSwap)
		{
			if(unit=='$' || unit=='¥')//inaxio
				q = ct.extendStr(q, es_esStr);

			else if (unit=='¤')//inaxio
				q = ct.extendStr(q, es_sStr);
			else
				q = ct.insafter(q, (char *)"libras esterlinas", FALSE, emotion, emo_intensity);//inaxio
		
		}
		else
		{
			q=p;
			q = ct.prevGrp(q);
			q = ct.delGrp(q);
			if(unit=='£')//inaxio
				q = ct.insafter(q, (char *)"una", FALSE, emotion, emo_intensity);//inaxio
			else
				q = ct.insafter(q, (char *)"un", FALSE, emotion, emo_intensity);
		}

		freewrdLst(count, words);

		//Trasladamos la frontera y eliminamos sobrantes.
		q=ct.next(p);
		ct.setPatternForce(q, (char *)"l");
		ct.setStatus(q, stat);
		ct.setTnor(q, UTYPENOR_UNIT);

		q=ct.prevGrp(q);
		q=ct.delGrp(q);

	}
		
	return q;
}

/**********************************************************/
