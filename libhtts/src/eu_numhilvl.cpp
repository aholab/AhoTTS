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

Nombre fuente................ eu_numhilvl.cpp
Nombre paquete............... normalizador
Lenguaje fuente.............. C++
Estado....................... desarrollo
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Juan Luis Murugarren

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
0.0.11	 09/03/07  inaxio    isNum funtzioan NUM_UNIT kasua gehitu (2$, 5,67¤...)
0.0.10   02/01/07  inaxio    expSuma(CtI q,BOOL suma) funtzioa gehitu batuketa eta kenketak irakurtezako (a+b, a-b=c, a+b=c).
0.0.9	 29/12/06  inaxio    expNegativePunt(CtI r, BOOL puntu) funtzioa gehitu zenbaki hamartar negatiboentzat.
0.0.8    29/12/06  inaxio    expNegative(CtI q) funtzioa gehitu zenbaki oso negatiboentzat.
0.0.7    07/22/05  Larraitz  100000 100000000 100000000 erako zenbakiak irakurtzeko konponketak.
0.0.6    04/05/05  Nora.     00 000 0000 erako zenbakiak irakurtzeko konponketak.
0.0.5    04/05/01  Yon2.     Split in language dependent files.
0.0.4    25/04/01  Yon2.     Multiling support.
0.0.3    22/03/00  Yon2.     Point & 2. resolv
0.0.2    22/03/00  Yon2.     Need More Data.
0.0.1    11/12/00  Yon2.     Soporte tipo de normalizacion.
0.0.0    19/11/00  Yon2.     Codificacion inicial.

======================== Contenido ========================

M¢dulo expansor de n£meros a nivel alto. Expande cardinales
y ordinales. Se apoya en el m¢dulo de expansi¢n num‚rica de
nivel bajo.

Nota: ~~~~Faltan los casos de cardinales con el punto como 
separador de miles,
 cardinales con separador decimal. O 
cardinales con signo.
Se deja por no ser prioritario en este momento.


Yon2.

===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/
  
/**********************************************************/
#include <stdio.h>

#include "eu_t2l.hpp"
#include "choputi.h"

#include "httsmsg.h"
#include "chset.h"

/**********************************************************/

CHAR eu_lehenStr[] = "lehenengo";
CHAR eu_garrenStr[] = "garren";

/**********************************************************/

CtI LangEU_TextToList::expCard(CtI p)
{
	CtI q,r;
	INT stat,patlen, i = 0,emotion, emo_intensity;
	pCHAR pattern, input, input_temp;

	stat = ct.getStatus(p);
	emotion = ct.getEmotion(p);
	emo_intensity = ct.getEmo_int(p);

	if (ct(p).pattern) 
		pattern = strdup(ct(p).pattern);
	patlen = strlen(pattern);
	free(pattern); //IÑAKI

	if (patlen > 1)
	{
				
		input = strdup(ct.cell(p).str);
						
		q = ct.next(p);
		i=1;
	
		while((!(strcmp(ct.getStr(q), ".")))&&(i<patlen))
		{
			input_temp = strdup(ct.getStr(ct.next(q)));
			input = concatStr(input,input_temp);	
			free(input_temp); //IÑAKI

			i=i+2;
			
			if (i<patlen)
			{
				q = ct.next(ct.next(q));
			
				if (ct.prev(q)!=p)
					ct.del(ct.prev(q));
				if (ct.prev(q)!=p)
					ct.del(ct.prev(q));
		   	}
		}
		
		
		q = ct.del(q);
		q = ct.insafter(q,input,FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
		free(input); //IÑAKI
		p = expnum(q);
	}
	else
	{
		p = expnum(p);
	}
	
	//Trasladamos la frontera y eliminamos sobrantes.
	r = ct.next(p);
	ct.setPatternForce(r, (char*)"l");
	ct.setStatus(r, stat);
	ct.setTnor(r, UTYPENOR_NUMBER);
	
	r=ct.prevGrp(r);
	r=ct.delGrp(r);
	
	

	return r;
}

/**********************************************************/

/**********************************************************/

CtI LangEU_TextToList::expOrd(CtI p, BOOL withPoint)
{
	CtI q,r;
	pCHAR temp;
	INT stat;
	
	stat = ct.getStatus(p);

	if (!withPoint)	
	{
		q = ct.next(p);
		r = q;
		q = expnum(p, q);
		
		if ( (q == ct.next(r)) && (!strcmp(ct.getStr(q), "bat")) ) 
			ct.setStr(q, eu_lehenStr);
		else	
		{
			if (((ct(q).str)[strlen(ct(q).str) - 1] == 't') &&((ct(q).str)[strlen(ct(q).str) - 2] == 's'))
				(ct(q).str)[strlen(ct(q).str) - 1] = '\x0';
			q = ct.extendStr(q, eu_garrenStr);
		}
		
		//Trasladamos la frontera y eliminamos sobrantes.
		r = ct.next(p);
		r = ct.next(r);
		ct.setPatternForce(r,(char*) "l");
		ct.setStatus(r, stat);
		ct.setTnor(r, UTYPENOR_NUMBER);
		
		r=ct.prevGrp(r);
		r=ct.delGrp(r);
	}
	else	
	{
		q = p;
		r = p;
		q = expnum(p, q);
		//Aniadimos el GARREN. Si acaba en 'st' se pierde la 't'.
		if ( (q == ct.next(r)) && (!strcmp(ct.getStr(q), "bat"))) ct.setStr(q, eu_lehenStr);

		else	
		{
	  
			if (((ct(q).str)[strlen(ct(q).str) - 1] == 't') &&((ct(q).str)[strlen(ct(q).str) - 2] == 's'))
				(ct(q).str)[strlen(ct(q).str) - 1] = '\x0';
			
			q = ct.extendStr(q, eu_garrenStr);
		}
		
		//Trasladamos la frontera y eliminamos sobrantes.
		r = ct.next(p);
		ct.setPatternForce(r, (char*)"l");
		ct.setStatus(r, stat);
		ct.setTnor(r, UTYPENOR_NUMBER);

		
		r=ct.delGrp(p);
		p=r;

		//Resolvemos el punto.
		q = ct.nextGrp(p);
		if (strlen(ct.getStr(q)) > 1)	
		{
			temp = strdup(ct.getStr(q));
			ct.setStr(q, &(temp[1]));
			free (temp);
		}
		else 
		{
			q = ct.nextGrp(q);
			if (ct.getStr(q)[0] == chset_ToLower(ct.getStr(q)[0]) ) 
			{
				q = ct.prevGrp(q);
				ct.delGrp(q);
			}
		}

	}
		return r;
}

/**********************************************************/
CtI LangEU_TextToList::expPunt(CtI p, BOOL puntu)
{

	CtI q,t;
	INT stat,i,len;
	pCHAR pattern, input, input_temp;
	INT patlen;
	INT emotion, emo_intensity;
			
	stat = ct.getStatus(p);
	emotion = ct.getEmotion(p);
	emo_intensity = ct.getEmo_int(p);

	if (ct(p).pattern) 
		pattern = strdup(ct(p).pattern);
	patlen = strlen(pattern);
	free(pattern); //IÑAKI ?

	input = strdup(ct.cell(p).str);

	if(patlen > 3)
	{
		t = ct.next(p);
		i=1;
		
		while(i < patlen)
		{
				if (!(strcmp(ct.getStr(t), ",")))
				{
					q = ct.next(t);
									
				}
				else
				{
					input_temp = strdup(ct.getStr(ct.next(t)));
					input = concatStr(input,input_temp);
					free(input_temp); //IÑAKI
					t = ct.next(ct.next(t));
				}
				i = 2+i;
		}
	}
	else
	{
		q = ct.next(ct.next(p));
	}
		
	len = strlen(input);
	if (len > 18) 
	{
		/* !!!! deletrear el número */
		return p;
	}
	
	if (strlen(strdup(ct.cell(q).str))<19){	// inaxio
		q = expnum(q);
		q = ct.del(q);
		ct.setPatternForce(q, (char*)"l");
	}
	else{	// inaxio
		p=expandCell(p);
		return p;
	}
		
	if(puntu) // ponemos el punto o la coma segun corresponda
	{
		q = ct.insbefore(q, (char*)"puntu",FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
	}
	else
	{
		q = ct.insbefore(q, (char*)"koma",FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
	}

	ct.setPatternForce(q, (char*)"l");
	//ct.setStatus(q,1);
	
	q = ct.insbefore(q,input,FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
	
	if (strlen(strdup(ct.cell(q).str))<19){	// inaxio
		q = expnum(q);
		q = ct.del(q);
	}
	else{	// inaxio
		p=expandCell(p);
		return p;
	}

	ct.setPatternForce(q, (char*)"l");
	ct.setStatus(q,stat);
	ct.setTnor(q, UTYPENOR_NUMBER);
	
		
	q = ct.delGrp(ct.prevGrp(q));
	//q=ct.lastInGrp(q);
	free(input); //IÑAKI
	return q;
		
}

/**********************************************************/

CtI LangEU_TextToList::expNegativePunt(CtI r, BOOL puntu)
{
	CtI q;
	INT stat;
	pCHAR input;
	INT emotion, emo_intensity;

	stat = ct.getStatus(r);	
	emotion = ct.getEmotion(r);
	emo_intensity = ct.getEmo_int(r);
	
	input = strdup(ct.cell(ct.next(r)).str);
	q = ct.next(ct.next(ct.next(r)));
	q = expnum(q);
	q = ct.del(q);
	
			
	if(puntu) // ponemos el punto o la coma segun corresponda
	{
		q = ct.insbefore(q, (char*)"puntu",FALSE, emotion, emo_intensity);
	}
	else
	{
		q = ct.insbefore(q, (char*)"koma",FALSE, emotion, emo_intensity);
	}
	
	ct.setPattern(q, (char*)"l");

	q = ct.insbefore(q,input,FALSE, emotion, emo_intensity);
	
	q = expnum(q);
	
	q = ct.del(q);
	ct.setPattern(q, (char*)"l");


	q = ct.insbefore(q, (char*)"minus",FALSE, emotion, emo_intensity);
	ct.setPattern(q, (char*)"l");
	ct.setStatus(q,stat);
	ct.setTnor(q, UTYPENOR_NUMBER);

	q = ct.delGrp(ct.prevGrp(q));
	free(input);
	
	return q;
}


/**********************************************************/

CtI LangEU_TextToList::expNegative(CtI p)//inaxio->zenbaki negatiboen tratamendua egiteko
{

	CtI q,r;
	INT stat,emotion, emo_intensity;
	pCHAR pattern;
	INT patlen,i;
		
	stat = ct.getStatus(p);
	emotion = ct.getEmotion(p);
	emo_intensity = ct.getEmo_int(p);

	if (ct(p).pattern) pattern = strdup(ct(p).pattern);
	patlen = strlen(pattern);
	free(pattern); 
	stat = ct.getStatus(p);

	if(patlen==2)
	{
		q=ct.next(p);
		r = expnum(q);
		r = ct.next(q);
		ct.setPatternForce(r, (char*)"l");
	
		r = ct.insbefore(r, (char*)"minus", FALSE, emotion, emo_intensity);
		ct.setPattern(r, (char*)"l");
		ct.setStatus(r, stat);
		ct.setTnor(r, UTYPENOR_NUMBER);

		r=ct.prevGrp(r);
		r=ct.delGrp(r);

		p=r;
	}
	else if(patlen==4)
	{
		//milako puntudun zenbaki negatiboa bada (puntu bakarrarekin, adib. -1.000):
		if ((strcmp(ct.getStr(ct.next(ct.next(p))), ".") == 0) && (3 == strlen(ct.getStr(ct.next(ct.next(ct.next(p))))))){
			q=p;
			q=ct.lastInGrp(q);
			q=ct.insafter(q,ct.getStr(ct.next(p)),FALSE);
			q=ct.extendStr(q,ct.getStr(ct.next(ct.next(ct.next(p)))));
			
			q=expnum(q);
			q=ct.del(q);
			
			q=ct.insbefore(q,(char*)"minus", FALSE, emotion, emo_intensity);
			ct.setPattern(q, (char*)"l");
			ct.setStatus(q,stat);
			ct.setTnor(q, UTYPENOR_NUMBER);	
			q=ct.prevGrp(q);
			q=ct.delGrp(q);
			p=q;
		}
		//zenbaki negatibo hamartarra bada (adib. -3,75):
		else{
			if (strcmp(ct.getStr(ct.next(ct.next(p))), ".") == 0)
				p = expNegativePunt (p ,TRUE);
			else if (strcmp(ct.getStr(ct.next(ct.next(p))), ",") == 0)
				p = expNegativePunt (p , FALSE);
		}
	}
	else if(patlen>5){
			
			q=p;
			p=ct.next(p);
			q=ct.lastInGrp(q);
			q=ct.insafter(q,ct.getStr(p),FALSE);
			p=ct.next(ct.next(p));
			q=ct.extendStr(q,ct.getStr(p));
			
			i=4;
			while(i<patlen){
				q=ct.extendStr(q,ct.getStr(ct.next(ct.next(p))));
				p=ct.next(ct.next(p));
				i=i+2;
			}
			
			q=expnum(q);
			q=ct.del(q);
			
			q=ct.insbefore(q,(char*)"minus", FALSE, emotion, emo_intensity);
			ct.setPattern(q, (char*)"l");
			ct.setStatus(q,stat);
			ct.setTnor(q, UTYPENOR_NUMBER);	
			q=ct.prevGrp(q);
			q=ct.delGrp(q);
			p=q;
	}
	return p;		
}

/**********************************************************/

CtI LangEU_TextToList::expSuma(CtI p, BOOL suma)
{

	CtI q;
	INT stat,emotion, emo_intensity;
	pCHAR pattern;
	INT patlen;
		
	stat = ct.getStatus(p);
	emotion = ct.getEmotion(p);
	emo_intensity = ct.getEmo_int(p);

	if (ct(p).pattern) pattern = strdup(ct(p).pattern);
	patlen = strlen(pattern);
	free(pattern); 
	stat = ct.getStatus(p);

	if(patlen==3){
		q = expnum(p);
		q = ct.del(q);
		ct.setPatternForce(q, (char*)"l");
		ct.setStatus(q, stat);
		ct.setTnor(q, UTYPENOR_NUMBER);

		q = ct.lastInGrp(q);
		q = ct.insbefore(q, (char*)"gei", FALSE, emotion, emo_intensity);
		ct.setPatternForce(q, (char*)"l");
		ct.setStatus(q, stat);
		ct.setTnor(q, UTYPENOR_NUMBER);
		q = ct.del(ct.prev(q));

		q = expnum(ct.next(q));
		q = ct.del(q);
		ct.setPatternForce(q, (char*)"l");
		ct.setStatus(q, stat);
		ct.setTnor(q, UTYPENOR_NUMBER);	
	}
	else if(patlen==5){
		q = ct.lastInGrp(p);
		q = expnum(q);
		q = ct.del(q);
		ct.setPatternForce(q, (char*)"l"); 
		ct.setStatus(q, stat);
		ct.setTnor(q, UTYPENOR_NUMBER);

		q = ct.insbefore(q, (char*)"berdin", FALSE, emotion, emo_intensity);
		ct.setPatternForce(q, (char*)"l");
		ct.setStatus(q, stat);
		ct.setTnor(q, UTYPENOR_NUMBER);
		q = ct.del(ct.prev(q));

		q = expnum(ct.prev(q));
		q = ct.del(q);
		ct.setPatternForce(q, (char*)"l"); 
		ct.setStatus(q, stat);
		ct.setTnor(q, UTYPENOR_NUMBER);

		if(suma)
			q = ct.insbefore(q, (char*)"gei", FALSE, emotion, emo_intensity);
		else
			q = ct.insbefore(q, (char*)"ken", FALSE, emotion, emo_intensity);
		ct.setPatternForce(q, (char*)"l");
		ct.setStatus(q, stat);
		ct.setTnor(q, UTYPENOR_NUMBER);
		q = ct.del(ct.prev(q));

		q = expnum(ct.prev(q));
		q = ct.del(q);
		ct.setPatternForce(q, (char*)"l"); 
		ct.setStatus(q, stat);
		ct.setTnor(q, UTYPENOR_NUMBER);
		
	}
		return q;
}

/**********************************************************/

NumType LangEU_TextToList::isNum(CtI p)
{
	NumType retval = NUM_UNKNOWN;
	NumType numTemp = NUM_UNKNOWN;
	pCHAR pattern;
	INT patlen;
	CtI q;
	CHAR puntt;

#ifdef HTTS_DEBUG
        if (!ct(p).pattern) htts_error("TextToList::isNum() - Trying to read null Pattern.");
#endif
	if (ct(p).pattern){
		pattern = strdup(ct(p).pattern);
	}
	else return retval;
	
	patlen = strlen(pattern);

	//Si no es uno de los patrones de numero.

	if (pattern[0] != 'n') 
	{	
		if (ct.getStr(p)[0] == '-')
		{
			//zenbaki negatiboak (-n)
			if (patlen == 2)
			{
				if((!strcmp(pattern, "pn"))&&(ct.getStr(p)[0] == '-'))
				{
					free(pattern);
					retval = NUM_NEGATIVE;
					return retval;
				}
				else 
				{
					free(pattern);
					return retval;
				}
			}

			//zenbaki negatibo hamartarrak (-n,m / -n.m)

			else if (patlen == 4)
			{
				if((!(strcmp(pattern, "pnpn")))&&(ct.getStr(p)[0] == '-')&&((!(strcmp(ct.getStr(ct.next(ct.next(p))), ".")))||(!(strcmp(ct.getStr(ct.next(ct.next(p))), ",")))))
				{
					free(pattern);
					retval = NUM_NEGATIVE;
					return retval;
				}
				else 
				{
					free(pattern);
					return retval;
				}
			}
			//milako puntudun zenbaki negatiboak (-1.000.000)
			else if ((patlen >5) && ((patlen%2)==0)){
				int i=3;
				numTemp = NUM_NEGATIVE;
				while(i<patlen){
					if((strcmp(ct.getStr(ct.next(ct.next(p))), ".") != 0) || (3 != strlen(ct.getStr(ct.next(ct.next(ct.next(p)))))))
						numTemp = NUM_UNKNOWN;
					p=ct.next(ct.next(p));
					i=i+2;
				}
				retval = numTemp;
				free(pattern);
				return retval;
			}
		}
		
		else
		{
			numTemp = NUM_UNKNOWN;
			retval = numTemp;
			free(pattern); //IÑAKI
			return retval;
		}
	}
	
	if ((pattern[0] == 'n') && (patlen == 1)) 
		numTemp = NUM_CARD;

			 
	
	if (patlen == 2)
		if ((pattern[0] == 'n') && (pattern[1] == 'p')) 
			 numTemp = NUM_ORD;
		//inaxio: zenbakia moneta unitatearekin batera badoa:
		else if ((pattern[0] == 'n') && (pattern[1] == 's'))
	 		if (strcmp(ct.getStr(ct.next(p)), "$") == 0 ||
				strcmp(ct.getStr(ct.next(p)), "£") == 0 ||
				strcmp(ct.getStr(ct.next(p)), "¥") == 0 ||
				strcmp(ct.getStr(ct.next(p)), "¤") == 0) numTemp = NUM_UNIT;
	if (patlen == 3)
	{
		if ((pattern[0] == 'n') && (pattern[1] == 'p')&& (pattern[2] == 'n')) 
		{
			if (strcmp(ct.getStr(ct.next(p)), ".") == 0)
			{
				if (3 == strlen(ct.getStr(ct.next(ct.next(p)))))
				{ 
					if (strcmp(ct.getStr(p), "0"))
					{
						numTemp = NUM_CARD;
						retval = numTemp;
						free(pattern); //IÑAKI
						return retval;								
					}
					else if (strcmp(ct.getStr(ct.next(ct.next(p))), "000") == 0)
					{
						numTemp = NUM_UNKNOWN;
						retval = numTemp;
						free(pattern); //IÑAKI
						return retval;
					}
					else 
					{
						numTemp = NUM_PUNT;
						retval = numTemp;
						free(pattern); //IÑAKI
						return retval;
									 
					}
				}
				else
				{
					numTemp = NUM_PUNT;
					retval = numTemp;
					free(pattern); //IÑAKI
					return retval;
				}
			}
			else if (strcmp(ct.getStr(ct.next(p)), ",") == 0)
			{
				numTemp = NUM_COMA;
				retval = numTemp;
				free(pattern); 
				return retval;
			}
		}
		else if ((pattern[0] == 'n') && (pattern[1] == 's')&& (pattern[2] == 'n')) 
		{
			if (strcmp(ct.getStr(ct.next(p)), "+") == 0)
			{
				numTemp = NUM_SUMA;
				retval = numTemp;
				free(pattern); 
				return retval;
			}
		}									  
	}
	if (patlen == 4 &&(!strcmp(pattern, "npns")))//inaxio: 5,50¤ / 9,99$
	{
		if((strcmp(ct.getStr(ct.next(ct.next(ct.next(p)))), "$") == 0 ||
		strcmp(ct.getStr(ct.next(ct.next(ct.next(p)))), "£") == 0 ||
		strcmp(ct.getStr(ct.next(ct.next(ct.next(p)))), "¥") == 0 ||
		strcmp(ct.getStr(ct.next(ct.next(ct.next(p)))), "¤") == 0) &&
		((!(strcmp(ct.getStr(ct.next(p)), ".")))||(!(strcmp(ct.getStr(ct.next(p)), ",")))))
		{
			numTemp = NUM_UNIT;//inaxio
		}	
	}
	if (patlen == 5)
	{
		if ((pattern[0] == 'n') && (pattern[2] == 'n') && (pattern[3] == 's')&& (pattern[4] == 'n')) 
		{
			if ((strcmp(ct.getStr(ct.next(p)), "-") == 0) && (strcmp(ct.getStr(ct.next(ct.next(ct.next(p)))), "=") == 0) )//si es una resta (l-m=n)
			{
				numTemp = NUM_RESTA;
				retval = numTemp;
				free(pattern); //IÑAKI
				return retval;
			}
			else if ((strcmp(ct.getStr(ct.next(p)), "+") == 0) && (strcmp(ct.getStr(ct.next(ct.next(ct.next(p)))), "=") == 0) )//si es una suma (l+m=n)
			{
				numTemp = NUM_SUMA;
				retval = numTemp;
				free(pattern); //IÑAKI
				return retval;
			}
		}
	}

	if ((patlen > 4) && (1 == (patlen % 2)))
	{
		int i = 0;
		numTemp = NUM_CARD;
		if ((!(strcmp(ct.getStr(p), "0")))|| (strcmp(ct.getStr(ct.next(p)), "."))||(pattern[i] != 'n')||(pattern[i+2] != 'n'))
		{
			numTemp = NUM_UNKNOWN;
			retval = numTemp;
			free(pattern); //IÑAKI
			return retval;
		}
		
		i = 1;
		q = ct.next(p);

		

		while (i < (patlen-2))
		{

			if ((strcmp(ct.getStr(q), "."))||(strlen(ct.getStr(ct.next(q)))!=3)||(pattern[i+1] != 'n'))
			{
				numTemp = NUM_UNKNOWN;
				retval = numTemp;
				free(pattern); //IÑAKI
				return retval;
			}

			q = ct.next(ct.next(q));

			i = i + 2;
		}

		if (i == (patlen-2))
		{

			if (!(strcmp(ct.getStr(q), ",")))
			{
				numTemp = NUM_COMA;
			}			
			else 
			{
				if ((strcmp(ct.getStr(q), "."))||(strlen(ct.getStr(ct.next(q)))!=3)||(pattern[i+1] != 'n'))
				{
					numTemp = NUM_UNKNOWN;
					retval = numTemp;
					free(pattern); //IÑAKI
					return retval;
				}
				else
				{
					numTemp = NUM_CARD;
				
				}
			}		
		}
		
	}
					 	
	if ((numTemp == NUM_CARD) && (pattern[0] == 'n') && (patlen == 1)) 
	{
  		if ( ct.nextGrp(p) )
		{
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
								{
									if (ct.getStr(q)[0] == chset_ToLower(ct.getStr(q)[0]) )
									{
										numTemp = NUM_ORD_PUNT;
									}
								}
							}
							else if (moreDataNeedAllowed) if (!ct.nextGrp(q)) numTemp = NUM_NEEDMOREDATA;
						}
						else if (moreDataNeedAllowed) if (!ct.nextGrp(q)) numTemp = NUM_NEEDMOREDATA;
	          			}
					if (((ct(q).str)[0] == '.') && (strlen(ct(q).str) > 1))
					{
						numTemp = NUM_ORD_PUNT;
					}
				}
				else if ((strlen(ct(q).pattern) > 1) && ((ct(q).pattern)[0] == 'p'))
				{
					if ((ct(q).str)[0] == '.')	numTemp = NUM_ORD_PUNT;
				}
      			}
		}
	}
	free(pattern);

	 
	//Chequeamos que los separadores sean válidos.
	if (numTemp == NUM_ORD)
	{
		q=ct.next(p);
		if (strlen(ct(q).str) != 1) return retval;
		puntt = (ct(q).str)[0];
		if (puntt != '.') return retval;
	}

	 
	//Verificamos que el n£mero no tiene m s de 18 cifras.
	if ( (numTemp != NUM_UNKNOWN) && (numTemp != NUM_NEEDMOREDATA) )
		if (strlen(ct.getStr(p)) >18) numTemp = NUM_TOO_LARGE;

	retval = numTemp;

			 
	return retval;
}
/**********************************************************/
