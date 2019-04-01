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

Nombre fuente................ es_numhilvl.cpp
Nombre paquete............... normalizador
Lenguaje fuente.............. C++
Estado....................... desarrollo
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Juan Luis Murugarren

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
0.1.7    02/01/07  inaxio    expSuma(CtI q,BOOL suma) funtzioa gehitu batuketa eta kenketak irakurtezako (a+b, a-b=c, a+b=c).
0.1.6	 29/12/06  inaxio    expNegative(CtI q) funtzioa gehitu zenbaki negatiboentzat
0.1.5	 20/11/06  inaxio    isNum, numTemp = NUM_UNIT gehitu
0.1.4	 05/10/06  inaxio    ExpCard, milakoen puntua onartu
0.1.4	 05/10/06  inaxio    ExpOrd, 10en multiplo ez diren ordinalak irakurtzeko zuzendu
0.1.4	 05/10/06  inaxio    ExpOrd, ordinal pluralak irakurtzeko zuzendu
0.1.3    22/05/05  Larraitz  ExpOrd, zuzendu next pattern null irakurtzean
0.1.2	 28/09/04  inigos    Correción de las comas
0.1.2	 07/05/03  inigos    0,07 cero coma CERO siete.
0.1.1    15/03/03  inigos    Version funcional
0.1.0    15/11/02  inigos    Recopia y adaptación para castellano
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

#include "es_t2l.hpp"
#include "choputi.h"

#include "httsmsg.h"
#include "chset.h"

/**********************************************************/

pCHAR es_ordinalTable[] = {
	(char*)"primer", (char*)"segund", (char*)"tercer", (char*)"cuart", (char*)"quint",
		(char*)"sext", (char*)"septim", (char*)"octav", (char*)"noven", (char*)"décim",
		(char*)"undecim", (char*)"duodécim"
	};

pCHAR es_tensOrdinalTable[] = {
	(char*)"décim", (char*)"vigésim", (char*)"trigésim", (char*)"cuadragésim", (char*)"quincuagésim",
		(char*)"sexagésim", (char*)"septuagésim", (char*)"octogésim", (char*)"nonagésim"
	};

pCHAR es_hundredsOrdinalTable[] = {
	(char*)"centésim", (char*)"ducentésim", (char*)"tricentésim", (char*)"cuadrigentésim", (char*)"quingentésim",
		(char*)"sexcentésim", (char*)"septingentésim", (char*)"octingentésim", (char*)"noningentésim"
	};


CHAR es_masculinoStr[]="o";
CHAR es_femeninoStr[]="a";
CHAR es_masc_pluStr[]="os";//inaxio
CHAR es_fem_pluStr[]="as";//inaxio

/**********************************************************/

CtI LangES_TextToList::expCard(CtI p)
{
  CtI q,r;
  INT stat,emotion, emo_intensity, patlen, i = 0;
  pCHAR pattern, input, input_temp;

  stat = ct.getStatus(p);
	emotion = ct.getEmotion(p);
	emo_intensity = ct.getEmo_int(p);

  if (ct(p).pattern) 
		pattern = strdup(ct(p).pattern);
	patlen = strlen(pattern);
	free(pattern); 

	if (patlen > 1)
	{
				
		input = strdup(ct.cell(p).str);
						
		q = ct.next(p);
		i=1;
	
		while((!(strcmp(ct.getStr(q), ".")))&&(i<patlen))
		{
			input_temp = strdup(ct.getStr(ct.next(q)));
			input = concatStr(input,input_temp);	
			free(input_temp);

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
		free(input); 
		p = expnum(q);
	}
	else
	{
		p = expnum(p);
	}

  //Trasladamos la frontera y eliminamos sobrantes.
  r = ct.next(p);

  ct.setPatternForce(r,(char*) "l");
  ct.setStatus(r, stat);
  ct.setTnor(r, UTYPENOR_NUMBER);

  r=ct.prevGrp(r);
  r=ct.delGrp(r);
  return r;
}

/**********************************************************/

/**********************************************************/

CtI LangES_TextToList::expOrd(CtI p, BOOL female, BOOL plural) 
{
	
	pCHAR temp,input; //ezabatu input
	INT stat,emotion, emo_intensity;
	INT i;
	UINT num;
	INT len;
	CtI q;

	stat = ct.getStatus(p);
	emotion = ct.getEmotion(p);
	emo_intensity = ct.getEmo_int(p);

	temp = strdup(ct.getStr(p));

	len = strlen(temp);

	for (i=0; i < len; i++) temp[i]=temp[i];
	temp[i]=0;

	num = chu_str2num(temp);

	q=p;

	while (num)	{ 
		if (num<=12)
		{
			q = ct.insafter(q, es_ordinalTable[num-1], FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
			num = 0;
		}
		else 
			if (num<100) {
				q = ct.insafter(q, es_tensOrdinalTable[(UINT)(num/10 - 1)], FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
				num = num%10;
				if (num) q = ct.extendStr(q, es_masculinoStr); 
			}
		
			else {
				q = ct.insafter(q, es_hundredsOrdinalTable[(UINT)(num/100 - 1)], FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
				num = num%100;
				if (num) q = ct.extendStr(q, es_masculinoStr);
			}
	}

	
	if (female)
	{
		if(plural) q = ct.extendStr(q, es_fem_pluStr);//inaxio
		else q = ct.extendStr(q, es_femeninoStr);//inaxio
	}
	else
	{
		if (plural) q = ct.extendStr(q, es_masc_pluStr);//inaxio
		else q = ct.extendStr(q, es_masculinoStr);//inaxio
	}
	
	q=ct.del(ct.next(q));
	
	q=ct.next(p);
	ct.setPatternForce(q, (char*)"l");
	ct.setStatus(q, stat);
	ct.setTnor(q, UTYPENOR_NUMBER);

	q=ct.prevGrp(q);
	q=ct.delGrp(q);

	free(temp);
	

	return q;
}

/**********************************************************/

CtI LangES_TextToList::expPunt(CtI p, BOOL punto)
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
	free(pattern);

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
					free(input_temp);
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
			
	if(punto) // ponemos el punto o la coma segun corresponda
	{
		q = ct.insbefore(q, (char*)"punto",FALSE, emotion, emo_intensity);
	}
	else
	{
		q = ct.insbefore(q, (char*)"coma",FALSE, emotion, emo_intensity);
	}

	ct.setPatternForce(q, (char*)"l");
	
	q = ct.insbefore(q,input,FALSE, emotion, emo_intensity);
	
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
	free(input);
	return q;
}

/**********************************************************/

CtI LangES_TextToList::expNegative(CtI p)//inaxio->Zenbaki negatiboen tratamendua egiteko.
{

	CtI q,r;
	INT stat,emotion, emo_intensity;
	pCHAR pattern;
	INT patlen,i;
	BOOL punto;
		
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
	
		//Trasladamos la frontera y eliminamos sobrantes.
		r = ct.next(q);
		ct.setPatternForce(r, (char*)"l");
	
		r = ct.insbefore(r, (char*)"menos", FALSE, emotion, emo_intensity);
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
			
			q=ct.insbefore(q,(char*)"menos", FALSE, emotion, emo_intensity);
			ct.setPattern(q, (char*)"l");
			ct.setStatus(q,stat);
			ct.setTnor(q, UTYPENOR_NUMBER);	
			q=ct.prevGrp(q);
			q=ct.delGrp(q);
			p=q;
		}

		else{
			if (strcmp(ct.getStr(ct.next(ct.next(p))), ".") == 0){
				punto=TRUE;
			}
			else if (strcmp(ct.getStr(ct.next(ct.next(p))), ",") == 0){
				punto=FALSE;
			}
		
			q=ct.lastInGrp(p);
			r = expnum(q);
			r = ct.del(r);

			if (punto){
				r = ct.insbefore(r, (char*)"punto",FALSE, emotion, emo_intensity);
			}
			else{
				r = ct.insbefore(r, (char*)"coma",FALSE, emotion, emo_intensity);
			}
		
			r = ct.del(ct.prev(r));
		
			r = expnum(ct.next(p));
			r = ct.del(r);
		
			r= ct.insafter(p,(char*)"menos", FALSE, emotion, emo_intensity);
			ct.setPatternForce(r, (char*)"l");

			p=r;

			ct.setPatternForce(p, (char*)"l");
			ct.setStatus(p,stat);
			ct.setTnor(p, UTYPENOR_NUMBER);
			
			p = ct.delGrp(ct.prevGrp(p));
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
			
			q=ct.insbefore(q,(char*)"menos", FALSE, emotion, emo_intensity);
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

CtI LangES_TextToList::expSuma(CtI p, BOOL suma)
{
	
	CtI q;
	INT stat,emotion, emo_intensity;;
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
		q=ct.lastInGrp(p);
		q=expnum(q);
		q = ct.del(q);
		ct.setPatternForce(q, (char*)"l");
		ct.setStatus(q, stat);
		ct.setTnor(q, UTYPENOR_NUMBER);

		q = ct.del(ct.prev(q));

		q=ct.prevGrp(q);
		q = ct.insafter(q, (char*)"mas", FALSE, emotion, emo_intensity);
		ct.setPatternForce(q, (char*)"l");
		ct.setStatus(q, stat);
		ct.setTnor(q, UTYPENOR_NUMBER);

		q = expnum(ct.prev(q));
		q = ct.del(q);
		ct.setPatternForce(q, (char*)"l");
		ct.setStatus(q, stat);
		ct.setTnor(q, UTYPENOR_NUMBER);
	}
	else if(patlen==5){
		q = ct.lastInGrp(p);
		q = expnum(q);
		q = ct.del(q);
		ct.setPatternForce(q,(char*) "l"); 
		ct.setStatus(q, stat);
		ct.setTnor(q, UTYPENOR_NUMBER);

		q = ct.insbefore(q, (char*)"igual a", FALSE, emotion, emo_intensity);
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
			q = ct.insbefore(q, (char*)"mas", FALSE, emotion, emo_intensity);
		else
			q = ct.insbefore(q, (char*)"menos", FALSE, emotion, emo_intensity);
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

NumType LangES_TextToList::isNum(CtI p)
{
	NumType retval = NUM_UNKNOWN;
	NumType numTemp = NUM_UNKNOWN;
	pCHAR pattern, pattern2;
	INT patlen, patlen2;
	CtI q;

#ifdef HTTS_DEBUG
        if (!ct(p).pattern) htts_error("TextToList::isNum() - Trying to read null Pattern.");
#endif
	if (ct(p).pattern) pattern = strdup(ct(p).pattern);
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

	// Si es un numero solo

 	if ((pattern[0] == 'n') && (patlen == 1))
	{
		numTemp = NUM_CARD;
	}
			
	// Si el siguiente es un signo
	if (patlen == 2)
	{
		if ((pattern[0] == 'n') && (pattern[1] == 's')) // si es ordinal singular y menor que 1000
		{
			if (strcmp(ct.getStr(ct.next(p)), "º") == 0)
			{
				if (strlen(ct.getStr(p)) < 4) numTemp = NUM_ORD_ES_O;
			}
			else if (strcmp(ct.getStr(ct.next(p)), "ª") == 0)
			{
				if (strlen(ct.getStr(p)) < 4) numTemp = NUM_ORD_ES_A;
			}
			else if (strcmp(ct.getStr(ct.next(p)), "$") == 0 ||
			strcmp(ct.getStr(ct.next(p)), "£") == 0 ||
			strcmp(ct.getStr(ct.next(p)), "¥") == 0 ||
			strcmp(ct.getStr(ct.next(p)), "¤") == 0) numTemp = NUM_UNIT;//inaxio
		}
		// Si el siguiente es una letra 
		else if (pattern[0] == 'n' && pattern[1] == 'l') // si es ordinal plural y menor que 1000
		{
			if (strcmp(ct.getStr(ct.next(p)), "os") == 0)
			{
				if (strlen(ct.getStr(p)) < 4) numTemp = NUM_ORD_ES_OS;
			}
			else if (strcmp(ct.getStr(ct.next(p)), "as") == 0)
			{
				if (strlen(ct.getStr(p)) < 4) numTemp = NUM_ORD_ES_AS;
			}
		}
	}

	// Si es un número con comas o puntos
	
	else if((patlen == 3) && strcmp(pattern, "npn")==0){
		if (strcmp(ct.getStr(ct.next(p)), ".") == 0){
			// Si es un número cardinal con punto de millar
			if (3 == strlen(ct.getStr(ct.next(ct.next(p))))){ 
				if (strcmp(ct.getStr(p), "0")){
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

			}else{
				numTemp = NUM_PUNT;
				retval = numTemp;
				free(pattern); //IÑAKI
				return retval;
			}
		}
		else if (strcmp(ct.getStr(ct.next(p)), ",") == 0){
			numTemp = NUM_COMA;
			retval = numTemp;
			free(pattern); 
			return retval;
		}
	}
	else if ((patlen==3) && strcmp(pattern, "nsn")==0)
	{
		if (strcmp(ct.getStr(ct.next(p)), "+") == 0)
		{
			numTemp = NUM_SUMA;
			retval = numTemp;
			free(pattern); 
			return retval;
		}
	}									  
	else if (patlen == 5)
	{
		if ((pattern[0] == 'n') && (pattern[2] == 'n') && (pattern[3] == 's')&& (pattern[4] == 'n')) 
		{
			if ((strcmp(ct.getStr(ct.next(p)), "-") == 0) && (strcmp(ct.getStr(ct.next(ct.next(ct.next(p)))), "=") == 0) )//si es una resta (l-m=n)
			{
				numTemp = NUM_RESTA;
				retval = numTemp;
				free(pattern); 
				return retval;
			}
			else if ((strcmp(ct.getStr(ct.next(p)), "+") == 0) && (strcmp(ct.getStr(ct.next(ct.next(ct.next(p)))), "=") == 0) )//si es una suma (l+m=n)
			{
				numTemp = NUM_SUMA;
				retval = numTemp;
				free(pattern);
				return retval;
			}
		}
	}
	//si es una unidad monetaria con decimales (con coma o punto)
	if (patlen == 4 &&(!strcmp(pattern, "npns")))
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

	if ((patlen > 4) && (1 == (patlen % 2))){
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


	if((q=ct.nextGrp(p)) != NULL)
	{
		if (ct(q).pattern)
		{
			pattern2 = strdup(ct(q).pattern);
			patlen2 = strlen(pattern2);
	
			if((pattern2[0] == 's') && (patlen2 == 1))
			{
				if (strcmp(ct.getStr(ct.nextGrp(p)), "%") == 0)
				{
					switch(numTemp)
					{
						case NUM_CARD:
							numTemp = NUM_CARD_PER;
							break;
						case NUM_PUNT:
							numTemp = NUM_PUNT_PER;
							break;
						case NUM_COMA:
							numTemp = NUM_COMA_PER;
							break;
						default:
							break;
					}
				}
			}
			free(pattern2);
		}
	}


	free(pattern);

	//Verificamos que el numero no tiene mas de 18 cifras.
	if ( (numTemp != NUM_UNKNOWN) && (numTemp != NUM_NEEDMOREDATA) )
		if (strlen(ct.getStr(p)) >18) numTemp = NUM_TOO_LARGE;

	retval = numTemp;

	return retval;
}
/**********************************************************/
