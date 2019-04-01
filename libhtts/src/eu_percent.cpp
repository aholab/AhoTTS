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

Nombre fuente................ eu_percent.cpp
Nombre paquete............... normalizador
Lenguaje fuente.............. C++
Estado....................... desarrollo
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Juan Luis Murugarren

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
0.0.4    07/07/05  Larraitz  %100, %10,01 eta %10.01 irakurtzeko aldaketak.
0.0.3    04/05/01  Yon2.     Split in language dependent files.
0.0.2    25/04/01  Yon2.     Multiling support.
0.0.1    07/04/01  Yon2.     If 2nd pattern null => need more data.
0.0.0    22/03/01  Yon2.     Codificacion inicial.

======================== Contenido ========================

	Este módulo se encarga de resolver el signo de tanto por
ciento. Si aparece solo se expandirá y se acabo.
	Si aparece con un número por delante (forma castellana)
se pasará a la forma en euskara. Si aparece detrás el número
se expandirá como corresponda, y si aparece un número detrás
y otro delante será preferida la forma en euskara, es decir,
se tomarña el número que va por detrás.

		% 100 -> ehuneko hamarra

Se declina el numero.  XX-a

Nota: Puede que estemos buscando el número siguiente y este todavía
no haber entrado => falla detección. => WorkArround
Solo números enteros.

Yon2.
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include "percent.hpp"
#include "eu_t2l.hpp"

#include "httsmsg.h"

/**********************************************************/

CtI LangEU_TextToList::expPercentPunt ( CtI r , BOOL puntu )
{
	CtI q;
	INT stat;
	pCHAR input;
	INT emotion, emo_intensity;

	emotion = ct.getEmotion(r);
	emo_intensity = ct.getEmo_int(r);
	stat = ct.getStatus(r);

	input = strdup(ct.cell(ct.next(r)).str);


	q = ct.next(ct.next(ct.next(r)));
	//q=expnumafterpoint(q);
	q = expnum(q);//inaxio
	q=ct.del(q);//inaxio

	if(puntu) // ponemos el punto o la coma segun corresponda
	{
		q = ct.insbefore(q, (char *)"puntu",FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
	}
	else
	{
		q = ct.insbefore(q, (char *)"koma",FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
	}

	ct.setPattern(q, (char *)"l");

	q = ct.insbefore(q,input,FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu

	q = expnum(q);

	q = ct.del(q);
	ct.setPattern(q, (char *)"l");


	q = ct.insbefore(q, (char *)"ehuneko",FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
	ct.setPattern(q, (char *)"l");
	ct.setStatus(q,stat);
	ct.setTnor(q, UTYPENOR_NUMBER);

	q = ct.delGrp(ct.prevGrp(q));
	free(input); //IÑAKI

	return q;
}


/**********************************************************/




CtI LangEU_TextToList::expPercent( CtI p , BOOL basque )
{
	CtI q,r;
	pCHAR temp,pattern;
	TNOR tnor;
	INT stat, emotion, emo_intensity,patlen;

	emotion = ct.getEmotion(p);
	emo_intensity = ct.getEmo_int(p);

	if (basque)
	{
		if (ct(p).pattern)
		{
			pattern = strdup(ct(p).pattern);
		}
		patlen = strlen(pattern);
		free(pattern); //IÑAKI
		if (patlen>1)
		{
			if(patlen==2)
			{
				stat = ct.getStatus(p);
				if(strcmp(ct.getStr(ct.next(p)), "%") != 0){
					q=ct.next(p);
					r = expnum(q);
				}
				else{
					q=p;
					r = expnum(q);
					r = ct.lastInGrp(r);
					r=ct.del(r);
				}
				//Trasladamos la frontera y eliminamos sobrantes.
				r = ct.next(q);
				ct.setPatternForce(r, (char *)"l");

				r = ct.insbefore(r, (char *)"ehuneko", FALSE, emotion, emo_intensity);//inaxio-> emotion eta emo_intensity gehitu
				ct.setPattern(r, (char *)"l");
				ct.setStatus(r, stat);
				ct.setTnor(r, UTYPENOR_NUMBER);

				r=ct.prevGrp(r);
				r=ct.delGrp(r);

				p=r;
			}
			else if(patlen==4)
			{
				if (strcmp(ct.getStr(ct.next(ct.next(p))), ".") == 0)
					p = expPercentPunt (p ,TRUE);
				else if (strcmp(ct.getStr(ct.next(ct.next(p))), ",") == 0)
					p= expPercentPunt (p , FALSE);
				else{
					r=ct.lastInGrp(p);
					r=ct.del(r);
					r=ct.extendStr(p,(char *)"");
					if(strcmp(ct.getStr(ct.next(p)), ".") == 0){
						r=expnum(ct.next(ct.next(r)));
						r=ct.del(r);
						r=ct.insafter(ct.prev(r),(char *)"puntu",FALSE, emotion, emo_intensity);
						r=ct.del(ct.prev(r));
						r=expnum(ct.prev(r));
						r=ct.del(r);
					}
					else if(strcmp(ct.getStr(ct.next(p)), ",") == 0)
						r=expPunt(r,FALSE);
					p=ct.insbefore(r,(char *)"ehuneko", FALSE, emotion, emo_intensity);


					ct.setPattern(p, (char *)"l");
					ct.setStatus(p, stat);
					ct.setTnor(p, UTYPENOR_NUMBER);

					return p;

				}
			}
			else return p;


		}
	}
	else
	{

		p = expandGrp(p);
		stat = ct.getStatus(p);
		temp = strdup(ct.getStr(p));
		tnor = ct.getTnor(p);

		q = ct.insbefore(p, (char *)"a", TRUE, emotion, emo_intensity);
		ct.setPattern(q, (char *)"l");
		ct.setDek(q);
		q = ct.prevGrp(q);
		q = dekline(q);

		q = ct.insbefore(q, temp, TRUE, emotion, emo_intensity);
		ct.setStatus(q, stat);
		ct.setStr(q, temp);
		ct.setTnor(q, tnor);

		free(temp);

		//eliminamos sobrantes.
		q = ct.nextGrp(q);
		p=q;
		q = ct.nextGrp(q);
		ct.delGrp(q);
	}

	return p;
}

/**********************************************************/

PercentType LangEU_TextToList::isPercent( CtI p)
{
	PercentType retval = PERCENT_UNKNOWN;
	pCHAR pattern;
	INT patlen;

	CtI q;

#ifdef HTTS_DEBUG

        if (!ct(p).pattern) htts_error("TextToList::isPercent(CtI) - Trying to read null Pattern.");
#endif



	if (ct(p).pattern) pattern = strdup(ct(p).pattern);
	else return retval;

	patlen = strlen(pattern);



	//hasierako garbiketa ondorengo baldintzak ez baditu betetzen zuzenean atera

	if (!(pattern[0] == 's'))
	{
		if ((!strcmp(pattern, "ns"))&&(strcmp(ct.getStr(ct.next(p)), "%") == 0))
		{
			free(pattern);

			retval = PERCENT_BASQUE;

			return retval;
		}
		else if((!strcmp(pattern, "npns"))&&((strcmp(ct.getStr(ct.next(p)), ",")== 0)||(strcmp(ct.getStr(ct.next(p)), ".")== 0))&&(strcmp(ct.getStr(ct.next(ct.next(ct.next(p)))), "%") == 0))
		{
			free(pattern);

			retval = PERCENT_BASQUE;

			return retval;
		}
		else
		{
			free(pattern);
			return retval;
		}
	}

	//%zenbakia erakoak
	if (patlen == 2)
	{
		if((!strcmp(pattern, "sn"))&&(ct.getStr(p)[0] == '%'))
		{
			free(pattern);
			retval =	PERCENT_BASQUE;
			return retval;
		}
		else
		{
			free(pattern);
			return retval;
		}
	}

	//%zenbakia(puntu edo koma)zenbaki erakoak


	if (patlen == 4)
	{
		if((!strcmp(pattern, "snpn"))&&(ct.getStr(p)[0] == '%')&&((!(strcmp(ct.getStr(ct.next(ct.next(p))), ".")))||(!(strcmp(ct.getStr(ct.next(ct.next(p))), ",")))))
		{
			free(pattern);
			retval =	PERCENT_BASQUE;
			return retval;
		}
		else
		{
			free(pattern);
			return retval;
		}
	}


	//% zenbakia erakoak
	//Baieztatzen du hurrengoa zenbakia den.

	if (patlen == 1)
	{
		if((!strcmp(pattern, "s"))&&(ct.getStr(p)[0] == '%'))
		{
			if (ct.nextGrp(p))
			{
				q = ct.nextGrp(p);

				#ifdef HTTS_DEBUG
					if (!ct(q).pattern) htts_warn("TextToList::isPercent(CtI) - Trying to read null 2nd Pattern.");
				#endif

				if (!ct(q).pattern)
				{
					if (moreDataNeedAllowed) retval =	PERCENT_NEEDMOREDATA;
				}
				else
				{
					pattern = strdup(ct(q).pattern);
					patlen = strlen(pattern);

					//Si es patron de numero tenemos un porcentaje en euskara.
					if (((pattern[0] == 'n') && (patlen == 1))) retval = PERCENT_BASQUE;
					free(pattern);
				}
			}

			else if (moreDataNeedAllowed) retval =	PERCENT_NEEDMOREDATA;

			if (!retval) if (ct.prevGrp(p))
			{
				q = ct.prevGrp(p);

				if ((ct.getTnor(q) == UTYPENOR_NUMBER) && !(ct.isDekd(q)) )	retval = PERCENT_SPANISH;
			}

			return retval;
		}
	}
	else
	{
		free(pattern);
		return retval;
	}

	free(pattern);
	return retval;
}

/**********************************************************/

