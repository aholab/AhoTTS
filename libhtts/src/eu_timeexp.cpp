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

Nombre fuente................ eu_timeexp.cpp
Nombre paquete............... normalizador
Lenguaje fuente.............. C++
Estado....................... desarrollo
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Juan Luis Murugarren

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
0.0.7	 30/09/06  inaxio.	 Decir "eta laurden","eta erdiak" y "...gutxiago".
0.0.6    04/05/01  Yon2.     Rename filename to eu_timeexp.cpp.
0.0.5    24/04/01  Yon2.     Multiling support.
0.0.4    23/12/00  Yon2.     0.0.3 & pasar trabajo al declinador.
0.0.3    13/12/00  Yon2.     Hora punta & declinada.
0.0.2    11/12/00  Yon2.     Soporte tipo de normalizacion.
0.0.1    30/11/00  Yon2.     Corregir hamaikaAK.
0.0.0    24/11/00  Yon2.     Codificacion inicial.

======================== Contenido ========================

  Este m¢dulo se encarga de expandir horas y lapsos de
tiempo. Todas las horas se normalizan a formato de 12 horas.
Las que superan las 12 horas se acompa¤an de la franja
del d¡a. Ej:
14:12 -> Arratsaldeko ordu biak eta hamabi.

Se normaliza "ordu bata" y "ordu biak" con palabras separadas
para poder hacer los dos acentos.

No se dice las cinco menos veinte, sino las cuatro y cuarenta.
Tampoco se dicen "y cuarto" e "y media", son "y quince"
e "y treinta".

Si la hora esta declinada y es una hora punta no se declina aqu¡,
se deja al declinador que lo haga.
Ej: 6:00-etan -> Si declinamos ambas seiaketan
              -> Si s¢lo declina el deklinador seietan.

Yon2.
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include "stdlib.h"
#include "stdio.h"

#include "eu_t2l.hpp"
#include "symbolexp.h"
#include "choputi.h"

/**********************************************************/

CHAR eu_hourStr[]="ordu";
CHAR eu_minStr[]="minutu";
CHAR eu_secStr[]="segundu";

CHAR eu_laurdenStr[]="laurden";
CHAR eu_erdiStr[]="erdi";

CHAR eu_kStr[]="k";
CHAR eu_akStr[]="ak";
CHAR eu_aStr[]="a";
CHAR eu_rStr[]="r";
CHAR eu_rakStr[]="rak";

CHAR eu_etaStr[]="eta";
CHAR eu_gutxiStr[]="gutxi";

CHAR eu_mornStr[]="goizeko";
CHAR eu_midStr[]="eguerdiko";
CHAR eu_evenStr[]="arratsaldeko";
CHAR eu_nightStr[]="gaueko";

/**********************************************************/


/**********************************************************/
CtI LangEU_TextToList::exptimeHM(CtI p)
{
	INT hours;
	INT minutes;
	BOOL dekd;
	CtI q;
	INT stat, emotion, emo_intensity;

	stat = ct.getStatus(p);
	emotion = ct.getEmotion(p);
	emo_intensity = ct.getEmo_int(p);

	dekd = ct.isDekd(p);

	hours = chu_str2num(ct.cell(p).str);
	p = ct.next(p);
	p = ct.next(p);
	minutes = chu_str2num(ct.cell(p).str);
	q=p;

	if(minutes > 30 && hours != 23) hours+=1;
	else if(minutes > 30 && hours == 23) hours=0;
	if (hours == 0)	p = ct.insafter(p, eu_nightStr, FALSE, emotion, emo_intensity);
	else if (hours > 12)	{
  		hours-=12;
		if (hours < 2) p = ct.insafter(p, eu_midStr, FALSE, emotion, emo_intensity);
		else {
    		if (hours < 9) p = ct.insafter(p, eu_evenStr, FALSE, emotion, emo_intensity);
			else p = ct.insafter(p, eu_nightStr, FALSE, emotion, emo_intensity);
		}
	}
	else	{
		if (hours < 12)	p = ct.insafter(p, eu_mornStr, FALSE, emotion, emo_intensity);
		else p = ct.insafter(p, eu_midStr, FALSE, emotion, emo_intensity);
	}
	switch(hours)	{
		case 0:
			hours+=12;
			p = upTo99(hours, p);
			if (minutes!=30) p = ct.extendStr(p, eu_akStr);
			break;
		case 1:
			p = ct.insafter(p, eu_hourStr, FALSE, emotion, emo_intensity);
            p = upTo99(hours, p);
            p = ct.extendStr(p, eu_aStr);
			break;
		case 2:
            p = ct.insafter(p, eu_hourStr, FALSE, emotion, emo_intensity);
            p = upTo99(hours, p);
            if ((!dekd || minutes)&&minutes!=30) p = ct.extendStr(p, eu_akStr);
 			break;
		case 11:
            p = upTo99(hours, p);
            if ((!dekd || minutes)&&minutes!=30) p = ct.extendStr(p, eu_kStr);
 			break;
		case 12:
            p = upTo99(hours, p);
            if ((!dekd || minutes)&&minutes!=30) p = ct.extendStr(p, eu_akStr);
 			break;
		case 3:
			p = upTo99(hours, p);
            if ((!dekd || minutes)&&minutes!=30) p = ct.extendStr(p, eu_rakStr);
 			break;
		case 4:
			p = upTo99(hours, p);
            if ((!dekd || minutes)&&minutes!=30) p = ct.extendStr(p, eu_rakStr);
 			break;
			/* Este caso nunca se da. 23 */
		case 23:
            p = upTo99(hours, p);
            if ((!dekd || minutes)&&minutes!=30) p = ct.extendStr(p, eu_rakStr);
			else {
				p = ct.extendStr(p, eu_rStr);
                p = ct.extendStr(p, eu_aStr);
            }
			break;
		case 10:
            p = upTo99(hours, p);
            if ((!dekd || minutes)&&minutes!=30) p = ct.extendStr(p, eu_rakStr);
//			else p = ct.extendStr(p, rStr);
 			break;
		default:
            p = upTo99(hours, p);
            if ((!dekd || minutes)&&minutes!=30) p = ct.extendStr(p, eu_akStr);
//          else p = ct.extendStr(p, eu_aStr);
 			break;
	}

	if (minutes){
		if (minutes<31){
			p = ct.insafter(p, eu_etaStr, FALSE, emotion, emo_intensity);
			if (minutes==15) p = ct.insafter(p, eu_laurdenStr, FALSE, emotion, emo_intensity);
			else if (minutes==30){
				p = ct.insafter(p, eu_erdiStr, FALSE, emotion, emo_intensity);
				if (!dekd) p = ct.extendStr(p, eu_akStr);
			}
			else p = upTo99(minutes, p);
		}
		else{
			if (minutes==45){
				p = ct.insafter(p, eu_laurdenStr, FALSE, emotion, emo_intensity);
				p = ct.insafter(p, eu_gutxiStr, FALSE, emotion, emo_intensity);
			}
			else{
				minutes=60-minutes;
				p = upTo99(minutes, p);
				p = ct.insafter(p, eu_gutxiStr, FALSE, emotion, emo_intensity);
			}
		}
	}

	q=ct.next(q);
	ct.setPatternForce(q, (char*)"l");
	ct.setStatus(q, stat);
	ct.setTnor(q, UTYPENOR_TIME);

	q=ct.prevGrp(q);
	q=ct.delGrp(q);

	return q;

}

/**********************************************************/


/**********************************************************/
CtI LangEU_TextToList::exptimeHMS(CtI p)
{
	INT hours;
	INT minutes;
	INT seconds;
  CtI q;
	INT stat, emotion, emo_intensity;

	stat = ct.getStatus(p);
	emotion = ct.getEmotion(p);
	emo_intensity = ct.getEmo_int(p);

	hours = chu_str2num(ct.cell(p).str);
	p = ct.next(p);
	p = ct.next(p);
	minutes = chu_str2num(ct.cell(p).str);
	p = ct.next(p);
	p = ct.next(p);
	seconds = chu_str2num(ct.cell(p).str);
  q = p;

	switch(hours)	{
		case 1:
                        p = ct.insafter(p, eu_hourStr, FALSE, emotion, emo_intensity);
                        p = ct.insafter(p, eu_getchexp((CHAR)('0' + hours)), FALSE, emotion, emo_intensity);
			break;

		default:
                        p = upTo99(hours, p);
                        p = ct.insafter(p, eu_hourStr, FALSE, emotion, emo_intensity);
 			break;
		}


	switch(minutes)	{
		case 1:
                        p = ct.insafter(p, eu_minStr, FALSE, emotion, emo_intensity);
                        p = ct.insafter(p, eu_getchexp((CHAR)('0' + minutes)), FALSE, emotion, emo_intensity);
			break;

		default:
                        p = upTo99(minutes, p);
                        p = ct.insafter(p, eu_minStr, FALSE, emotion, emo_intensity);
 			break;
		}

	switch(seconds)	{
		case 1:
                        p = ct.insafter(p, eu_secStr, FALSE,emotion, emo_intensity);
                        p = ct.insafter(p, eu_getchexp((CHAR)('0' + seconds)), FALSE, emotion, emo_intensity);
			break;

		default:
                        p = upTo99(seconds, p);
                        p = ct.insafter(p, eu_secStr, FALSE, emotion, emo_intensity);
 			break;
		}

  q=ct.next(q);
  ct.setPatternForce(q, (char*)"l");
  ct.setStatus(q, stat);
  ct.setTnor(q, UTYPENOR_TIME);

  q=ct.prevGrp(q);
  q=ct.delGrp(q);

  return q;
}

/**********************************************************/

