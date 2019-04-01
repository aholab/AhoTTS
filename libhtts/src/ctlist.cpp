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

Nombre fuente................ ctlist.cpp
Nombre paquete............... normalizador
Lenguaje fuente.............. C++
Estado....................... desarrollo
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Juan Luis Murugarren

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
0.1.0	 12/07/10  Inaki	 Añadir celdas para modificar la prosodia desde el texto (proyecto Aritz) HTTS_PROSO_VAL
0.0.13   24/05/05  Larraitz  Problemas con bool.
0.0.12   18/06/01  Yon2.     Scape sequencies support.
0.0.11   10/01/01  Yon2.     Flush propagation.
0.0.10   06/01/01  Yon2.     lastGrp();
0.0.9    13/12/00  Yon2.     Last&First in Group.
0.0.8    12/12/00  Yon2.     Htts error handling.
0.0.7    11/12/00  Yon2.     Soporte tipo de normalizaci¢n.
0.0.6    11/12/00  Yon2.     Bug fix en delGrp.
0.0.5    07/12/00  Yon2.     Soporte declinaciones.
0.0.4    19/11/00  Yon2.     Aniadir funcion extendStr.
0.0.3    10/11/00  Yon2.     Cambiar 'start' por 'status'.
0.0.2    10/11/00  Yon2.     Ampliación del tipo de consultas.
0.0.1    09/11/00  Yon2.     Cambios para soportar start ampliado.
0.0.0    30/10/00  Yon2.     Codificacion inicial.

======================== Contenido ========================

  Esta clase se almacenar palabras cortadas del stream por
el que viene el texto crudo en forma de lista. Mantiene
agrupadas palabras que forman unidades semánticas. Por
ejemplo: direcciones de correo, fechas, horas, ... en los
que se expresan los contenidos con combinación de
caracteres de distinto tipo (patron).

Yon2.
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include "ctlist.hpp"

/**********************************************************/
#ifdef HTTS_PROSO_VAL
#define X_LOW 20
#define LOW  40
#define MEDIUM 60//Aritz
#define HIGH 80
#define X_HIGH 100
/***********************************************************************/
#define X_WEAK 20
#define WEAK 40
#define STRONG 80//Aritz
#define X_STRONG 100
/*************************************************************************/
#define REDUCED 25//Aritz
#define MODERATE 50
/************************************************************************/
#define X_SLOW 20//Aritz
#define SLOW 40
#define FAST 80
#define X_FAST 90
/*************************************************************************/
#define X_SOFT 20//Aritz
#define SOFT 40
#define LOUD 80
#define X_LOUD 90
#define DEFAULT 0
#endif
/**********************************************************/
CruTxt::~CruTxt( VOID )
{
	CtI p;
	for (p=first(); p!=NULL; p=del(p));
}
/**********************************************************/


//Esta la copio de la de Borja.
/**********************************************************/
Celda & CruTxt::cell(CtI p) const
{
#ifdef HTTS_DEBUG
        if (!p) htts_error("CruTxt::cell() - Trying to read null CruTxt");
#endif
        return (Celda &)cl.item(p);
}
/**********************************************************/


/**********************************************************/
#ifdef HTTS_PROSO_VAL
/*Aritz*/CtI CruTxt::append(pCHAR str, BOOL start, INT emotion, INT emo_intensity,INT val_Pros_b,INT val_Pros_e,INT val_Pros_p,INT val_Pros_ran,INT val_Pros_rat,INT val_Pros_v,pCHAR pattern)
{
	Celda tmp;

	if (str)
	{
		tmp.str = strdup(str);
	}
	else{
		tmp.str = NULL;
	}
	tmp.tnor = UTYPENOR_UNKNOWN;
	tmp.status = 0;
	tmp.status |= (start * START);
	if (pattern)
	{

		tmp.pattern = strdup(pattern);
	}
	else
	{

		tmp.pattern = NULL;
	}
	tmp.emo = emotion;
	tmp.emoInt = emo_intensity;
	//tmp.Pros=Prosody;//Aritz
	//tmp.val=val_Pros;//Aritz
	tmp.val_break=val_Pros_b;//Aritz
	tmp.val_emphasis=val_Pros_e;//Aritz
	tmp.val_pitch=val_Pros_p;//Aritz
	tmp.val_range=val_Pros_ran;//Aritz
	tmp.val_rate=val_Pros_rat;//Aritz
	tmp.val_volume=val_Pros_v;//Aritz
	return cl.append(tmp);


}
#else
CtI CruTxt::append(pCHAR str, BOOL start, INT emotion, INT emo_intensity, pCHAR pattern)
{
	Celda tmp;

	if (str) tmp.str = strdup(str);
	else tmp.str = NULL;
  tmp.tnor = UTYPENOR_UNKNOWN;
  tmp.status = 0;
	tmp.status |= (start * START);
	if (pattern) tmp.pattern = strdup(pattern);
	else tmp.pattern = NULL;
	tmp.emo = emotion;
	tmp.emoInt = emo_intensity;
	return cl.append(tmp);

}
#endif
/**********************************************************/


/**********************************************************/
#ifdef HTTS_PROSO_VAL
/*Aritz*/CtI CruTxt::prepend(pCHAR str, BOOL start, INT emotion, INT emo_intensity,INT val_Pros_b,INT val_Pros_e,INT val_Pros_p,INT val_Pros_ran,INT val_Pros_rat,INT val_Pros_v, pCHAR pattern)
{
	Celda tmp;

	if (str) tmp.str = strdup(str);
	else tmp.str = NULL;
  tmp.tnor = UTYPENOR_UNKNOWN;
	tmp.status = 0;
	tmp.status |= (start * START);
	if (pattern) tmp.pattern = strdup(pattern);
	else tmp.pattern = NULL;
	tmp.emo = emotion;
	tmp.emoInt = emo_intensity;
	//tmp.Pros=Prosody;//Aritz
	//tmp.val=val_Pros;//Aritz
	tmp.val_break=val_Pros_b;//Aritz

	tmp.val_emphasis=val_Pros_e;//Aritz
	tmp.val_pitch=val_Pros_p;//Aritz
	tmp.val_range=val_Pros_ran;//Aritz
	tmp.val_rate=val_Pros_rat;//Aritz
	tmp.val_volume=val_Pros_v;//Aritz
	return cl.prepend(tmp);

}
#else
CtI CruTxt::prepend(pCHAR str, BOOL start, INT emotion, INT emo_intensity, pCHAR pattern)
{
	Celda tmp;

	if (str) tmp.str = strdup(str);
	else tmp.str = NULL;
  tmp.tnor = UTYPENOR_UNKNOWN;
	tmp.status = 0;
	tmp.status |= (start * START);
	if (pattern) tmp.pattern = strdup(pattern);
	else tmp.pattern = NULL;
	tmp.emo = emotion;
	tmp.emoInt = emo_intensity;
	return cl.prepend(tmp);

}
#endif
/**********************************************************/


/**********************************************************/
#ifdef HTTS_PROSO_VAL
/*Aritz*/CtI CruTxt::insbefore(CtI p, pCHAR str, BOOL start, INT emotion, INT emo_intensity, INT val_Pros_b,INT val_Pros_e,INT val_Pros_p,INT val_Pros_ran,INT val_Pros_rat,INT val_Pros_v, pCHAR pattern)
{
	Celda tmp;

#ifdef HTTS_DEBUG
        if (!p) htts_error("CruTxt::insbefore() - Trying to read null CtI");
#endif

	if (str) tmp.str = strdup(str);
	else tmp.str = NULL;
  tmp.tnor = UTYPENOR_UNKNOWN;
	tmp.status = 0;
	tmp.status |= (start * START);
	if (pattern) tmp.pattern = strdup(pattern);
	else tmp.pattern = NULL;
	tmp.emo = emotion;
	tmp.emoInt = emo_intensity;
	//tmp.Pros=Prosody;//Aritz
	//tmp.val=val_Pros;//Aritz
	tmp.val_break=val_Pros_b;//Aritz
	tmp.val_emphasis=val_Pros_e;//Aritz
	tmp.val_pitch=val_Pros_p;//Aritz
	tmp.val_range=val_Pros_ran;//Aritz
	tmp.val_rate=val_Pros_rat;//Aritz
	tmp.val_volume=val_Pros_v;//Aritz
	return cl.insbefore(p, tmp);

}
#else
CtI CruTxt::insbefore(CtI p, pCHAR str, BOOL start, INT emotion, INT emo_intensity, pCHAR pattern)
{
	Celda tmp;

#ifdef HTTS_DEBUG
        if (!p) htts_error("CruTxt::insbefore() - Trying to read null CtI");
#endif

	if (str) tmp.str = strdup(str);
	else tmp.str = NULL;
  tmp.tnor = UTYPENOR_UNKNOWN;
	tmp.status = 0;
	tmp.status |= (start * START);
	if (pattern) tmp.pattern = strdup(pattern);
	else tmp.pattern = NULL;
	tmp.emo = emotion;
	tmp.emoInt = emo_intensity;
	return cl.insbefore(p, tmp);

}
#endif
/**********************************************************/


/**********************************************************/
#ifdef HTTS_PROSO_VAL
/*Aritz*/CtI CruTxt::insafter(CtI p, pCHAR str, BOOL start, INT emotion, INT emo_intensity,INT val_Pros_b,INT val_Pros_e,INT val_Pros_p,INT val_Pros_ran,INT val_Pros_rat, INT val_Pros_v, pCHAR pattern)
{
	Celda tmp;

#ifdef HTTS_DEBUG
        if (!p) htts_error("CruTxt::insafter() - Trying to read null CtI");
#endif

	if (str) tmp.str = strdup(str);
	else tmp.str = NULL;
  tmp.tnor = UTYPENOR_UNKNOWN;
	tmp.status = 0;
	tmp.status |= (start * START);
	if (pattern) tmp.pattern = strdup(pattern);
	else tmp.pattern = NULL;
	tmp.emo = emotion;
	tmp.emoInt = emo_intensity;
	//tmp.Pros=Prosody;//Aritz
	//tmp.val=val_Pros;//Aritz
	tmp.val_break=val_Pros_b;//Aritz
	tmp.val_emphasis=val_Pros_e;//Aritz
	tmp.val_pitch=val_Pros_p;//Aritz
	tmp.val_range=val_Pros_ran;//Aritz
	tmp.val_rate=val_Pros_rat;//Aritz
	tmp.val_volume=val_Pros_v;//Aritz
	return cl.insafter(p, tmp);

}
#else
CtI CruTxt::insafter(CtI p, pCHAR str, BOOL start, INT emotion, INT emo_intensity, pCHAR pattern)
{
	Celda tmp;

#ifdef HTTS_DEBUG
        if (!p) htts_error("CruTxt::insafter() - Trying to read null CtI");
#endif

	if (str) tmp.str = strdup(str);
	else tmp.str = NULL;
  tmp.tnor = UTYPENOR_UNKNOWN;
	tmp.status = 0;
	tmp.status |= (start * START);
	if (pattern) tmp.pattern = strdup(pattern);
	else tmp.pattern = NULL;
	tmp.emo = emotion;
	tmp.emoInt = emo_intensity;
	return cl.insafter(p, tmp);

}
#endif

/**********************************************************/


/**********************************************************/

CtI CruTxt::setStr(CtI p, pCHAR str)
{

#ifdef HTTS_DEBUG
        if (!p) htts_error("CruTxt::setSrt() - Trying to read null CtI");
#endif

	if (!p) return p;

	if (cell(p).str) free(cell(p).str);
	if (str) cell(p).str = strdup(str);
	else cell(p).str = NULL;

	return p;
}

/**********************************************************/


/**********************************************************/

pCHAR CruTxt::getStr(CtI p)
{

#ifdef HTTS_DEBUG
        if (!p) htts_error("CruTxt::getSrt() - Trying to read null CtI");
#endif

	if (!p) return NULL;


	return cell(p).str;
}

/**********************************************************/


/**********************************************************/
//Sólo el elemento cabeza de grupo posee patrón. Si el indice
//entregado no es el cabeza de grupo se busca hacia atrás.
//Devuelve el indice que se entrego salvo que no se encuentre
//el padre caso en el que entrega un NULL.
/**********************************************************/

/**********************************************************/

CtI CruTxt::setPattern(CtI p, pCHAR pattern)
{
	CtI q;

#ifdef HTTS_DEBUG
        if (!p) htts_error("CruTxt::setPattern() - Trying to read null CtI");
#endif

	if (!p)	return p;
	q=p;

	if (!isStart(p)) {
		do	{
			p=prev(p);
			if (p==NULL) return p;
		}
		while (!(isStart(p)));
	}

	if (cell(p).pattern) free(cell(p).pattern);
	if (pattern) cell(p).pattern = strdup(pattern);
	else cell(p).pattern = NULL;

	return q;

}

/**********************************************************/


/**********************************************************/
//Misma versi¢n que el de arriba pero fuerza la celda.
//No tiene porque ser cabeza de grupo.
CtI CruTxt::setPatternForce(CtI p, pCHAR pattern)
{

#ifdef HTTS_DEBUG
        if (!p) htts_error("CruTxt::setPatternForce() - Trying to read null CtI");
#endif

	if (!p)	return p;

	if (cell(p).pattern) free(cell(p).pattern);
	if (pattern) cell(p).pattern = strdup(pattern);
	else cell(p).pattern = NULL;

	return p;

}

/**********************************************************/


/**********************************************************/

CtI CruTxt::setStart(CtI p, BOOL start)
{
#ifdef HTTS_DEBUG
        if (!p) htts_error("CruTxt::setStart() - Trying to read null CtI");
#endif
	if (p) cell(p).status |= (start * START);
	return p;
}

/**********************************************************/


/**********************************************************/

CtI CruTxt::setFlush(CtI p)
{
#ifdef HTTS_DEBUG
        if (!p) htts_error("CruTxt::setFlush() - Trying to read null CtI");
#endif
	if (p) cell(p).status |= (FLUSH);
	return p;
}

/**********************************************************/


/**********************************************************/
//Añade una cadena al final de otra ya creada en una celda.
CtI CruTxt::extendStr(CtI p, pCHAR str)
{
  CHAR tmp[1024]="";

#ifdef HTTS_DEBUG
        if (!p) htts_error("CruTxt::extendStr() - Trying to read null CtI");
	if (!cell(p).str) htts_error("CruTxt::extendStr() - Trying to extend a null string");
#endif

  strcpy(tmp, cell(p).str);
  strcat(tmp, str);
  p = setStr(p, tmp);

  return p;
}
/**********************************************************/


/**********************************************************/

CtI CruTxt::first(VOID)
{
	return cl.first();
}

/**********************************************************/


/**********************************************************/

CtI CruTxt::last(VOID)
{
	return cl.last();
}

/**********************************************************/


/**********************************************************/

CtI CruTxt::next(CtI p)
{
	return cl.next(p);
}

/**********************************************************/

/**********************************************************/

CtI CruTxt::prev(CtI p)
{
	return cl.prev(p);
}

/**********************************************************/

/**********************************************************/
//Ojo que no mira si efectivamente es inicio de grupo.
CtI CruTxt::firstGrp(VOID)
{
	CtI p;

	p = cl.first();

#ifdef HTTS_DEBUG
        if (!isStart(p)) htts_error("CruTxt::firstGrp() - First cell is not part of group");
#endif
	return p;
}

/**********************************************************/


/**********************************************************/
//Ojo que no mira si efectivamente es inicio de grupo.
CtI CruTxt::lastGrp(VOID)
{

	CtI p;

	p = cl.last();

  	if (!p) return p;


	while (!(isStart(p))) {
		p=prev(p);
		if (p==NULL) return p;
	}

	return p;
}

/**********************************************************/


/**********************************************************/

CtI CruTxt::nextGrp(CtI p)
{

#ifdef HTTS_DEBUG
        if (!p) htts_error("CruTxt::nextGrp() - Trying to read null CtI");
#endif
	if (isStart(p))	{
		do	{
			p=next(p);
			if (p==NULL) return p;
		}
		while (!(isStart(p)));
		return p;
	}
	return NULL;
}

/**********************************************************/


/**********************************************************/

CtI CruTxt::prevGrp(CtI p)
{

#ifdef HTTS_DEBUG
        if (!p) htts_error("CruTxt::prevGrp() - Trying to read null CtI");
#endif

	if (isStart(p))	{
		do	{
			p=prev(p);
			if (p==NULL) return p;
		}
		while (!(isStart(p)));
		return p;
	}
	return NULL;
}

/**********************************************************/


/**********************************************************/

CtI CruTxt::lastInGrp(CtI p)
{

#ifdef HTTS_DEBUG
        if (!p) htts_error("CruTxt::lastInGrp() - Trying to read null CtI");
#endif

	if (next(p) == NULL) return p;
	while ( !isStart(next(p)) ) {
		p=next(p);
		if (next(p)==NULL) return p;
	}
	return p;
}

/**********************************************************/


/**********************************************************/

CtI CruTxt::firstInGrp(CtI p)
{

#ifdef HTTS_DEBUG
        if (!p) htts_error("CruTxt::firstInGrp() - Trying to read null CtI");
#endif

	if (prev(p) == NULL) return p;
	while ( !isStart(prev(p)) ) {
		p=prev(p);
		if (prev(p)==NULL) return p;
	}
	return p;
}

/**********************************************************/


/**********************************************************/

CtI CruTxt::del(CtI p)
{
#ifdef HTTS_DEBUG
        if (!p) htts_error("CruTxt::del() - Trying to read null CtI");
#endif
	if (cell(p).str) free(cell(p).str);
	if (cell(p).pattern) free(cell(p).pattern);
	return cl.del(p);
}

/**********************************************************/

/**********************************************************/

CtI CruTxt::delGrp(CtI p)
{
#ifdef HTTS_DEBUG
        if (!p) htts_error("CruTxt::delGrp() - Trying to read null CtI");
#endif
	if (isStart(p))	{
		do	{
			p=del(p);
			if (p==NULL) return p;
		}
		while (!(isStart(p)));
		return p;
	}
	return NULL;
}

/**********************************************************/


/**********************************************************/

BOOL CruTxt::isStart(CtI p)
{
#ifdef HTTS_DEBUG
        if (!p) htts_error("CruTxt::isStart() - Trying to read null CtI");
#endif
	return (cell(p).status & START);
}

/**********************************************************/


/**********************************************************/

BOOL CruTxt::isFlush(CtI p)
{
#ifdef HTTS_DEBUG
        if (!p) htts_error("CruTxt::isFlush() - Trying to read null CtI");
#endif
	return (cell(p).status & FLUSH);
}

/**********************************************************/


/**********************************************************/

BOOL CruTxt::isFrgmnt(CtI p)
{
#ifdef HTTS_DEBUG
        if (!p) htts_error("CruTxt::isFrgmnt() - Trying to read null CtI");
#endif
	return (cell(p).status & FROMCUT);
}

/**********************************************************/

/**********************************************************/

BOOL CruTxt::isLeftFrg(CtI p)
{
#ifdef HTTS_DEBUG
        if (!p) htts_error("CruTxt::isLeftFrg() - Trying to read null CtI");
#endif
	return (cell(p).status & LEFTFRG) && isFrgmnt(p);
}

/**********************************************************/

/**********************************************************/

BOOL CruTxt::isRightFrg(CtI p)
{
#ifdef HTTS_DEBUG
        if (!p) htts_error("CruTxt::isRightFrg() - Trying to read null CtI");
#endif
	return !(cell(p).status & LEFTFRG) && isFrgmnt(p);
}

/**********************************************************/


/**********************************************************/

BOOL CruTxt::isDekd(CtI p)
{
#ifdef HTTS_DEBUG
        if (!p) htts_error("CruTxt::isDekd() - Trying to read null CtI");
#endif
	return (cell(p).status & DEKD);
}

/**********************************************************/


/**********************************************************/

BOOL CruTxt::isDek(CtI p)
{
#ifdef HTTS_DEBUG
        if (!p) htts_error("CruTxt::isDek() - Trying to read null CtI");
#endif
	return (cell(p).status & DEK);
}

/**********************************************************/

/**********************************************************/

INT CruTxt::getEmotion( CtI p)
{
#ifdef HTTS_DEBUG
        if (!p) htts_error("CruTxt::getEmotion() - Trying to read null CtI");
#endif
  return cell(p).emo;
}

/**********************************************************/


/**********************************************************/

CtI CruTxt::setEmotion( CtI p, INT emotion)
{
#ifdef HTTS_DEBUG
        if (!p) htts_error("CruTxt::setEmotion() - Trying to read null CtI");
#endif
  cell(p).emo = emotion;
  return p;
}

/**********************************************************/
#ifdef HTTS_PROSO_VAL
INT CruTxt::getProso_val_break( CtI p)//Aritz
{
#ifdef HTTS_DEBUG
        if (!p) htts_error("CruTxt::getProso_val_break() - Trying to read null CtI");
#endif
  return cell(p).val_break;
}
/**************************************************************/
INT CruTxt::getProso_val_emphasis(CtI p)//Aritz
{
#ifdef HTTS_DEBUG
        if (!p) htts_error("CruTxt::getProso_val_emphasis() - Trying to read null CtI");
#endif
  return cell(p).val_emphasis;
}
/**********************************************************/
INT CruTxt::getProso_val_pitch( CtI p)//Aritz
{
#ifdef HTTS_DEBUG
        if (!p) htts_error("CruTxt::getProso_val_pitch() - Trying to read null CtI");
#endif
  return cell(p).val_pitch;
}
/*************************************************************/
INT CruTxt::getProso_val_range( CtI p)//Aritz
{
#ifdef HTTS_DEBUG
        if (!p) htts_error("CruTxt::getProso_val_range() - Trying to read null CtI");
#endif
  return cell(p).val_range;
}
/*************************************************************/
INT CruTxt::getProso_val_rate( CtI p)//Aritz
{
#ifdef HTTS_DEBUG
        if (!p) htts_error("CruTxt::getProso_val_rate() - Trying to read null CtI");
#endif
  return cell(p).val_rate;
}
/**********************************************************/
INT CruTxt::getProso_val_volume( CtI p)//Aritz
{
#ifdef HTTS_DEBUG
        if (!p) htts_error("CruTxt::getProso_val_volume() - Trying to read null CtI");
#endif
  return cell(p).val_volume;
}
#endif
/**********************************************************/

INT CruTxt::getEmo_int( CtI p)
{
#ifdef HTTS_DEBUG
        if (!p) htts_error("CruTxt::getEmo_int() - Trying to read null CtI");
#endif
  return cell(p).emoInt;
}

/**********************************************************/
#ifdef HTTS_PROSO_VAL
CtI CruTxt::setProso_val_break( CtI p, INT val_Pros_b)//Aritz
{
#ifdef HTTS_DEBUG
        if (!p) htts_error("CruTxt::setProso_val_break() - Trying to read null CtI");
#endif

		cell(p).val_break=val_Pros_b;
		return p;


}
/**********************************************************/
CtI CruTxt::setProso_val_emphasis( CtI p, INT val_Pros_e)//Aritz
{
#ifdef HTTS_DEBUG
        if (!p) htts_error("CruTxt::setProso_val_emphasis() - Trying to read null CtI");
#endif

		cell(p).val_emphasis=val_Pros_e;
		return p;
}
/**********************************************************/
CtI CruTxt::setProso_val_pitch( CtI p, INT val_Pros_p)//Aritz
{
#ifdef HTTS_DEBUG
        if (!p) htts_error("CruTxt::setProso_val_pitch() - Trying to read null CtI");
#endif

		cell(p).val_pitch=val_Pros_p;
		return p;
}
/********************************************************/
CtI CruTxt::setProso_val_range( CtI p, INT val_Pros_ran)//Aritz
{
#ifdef HTTS_DEBUG
        if (!p) htts_error("CruTxt::setProso_val_range() - Trying to read null CtI");
#endif

		cell(p).val_range=val_Pros_ran;
		return p;
}
/*************************************************************/
CtI CruTxt::setProso_val_rate( CtI p, INT val_Pros_rat)//Aritz
{
#ifdef HTTS_DEBUG
        if (!p) htts_error("CruTxt::setProso_val_rate() - Trying to read null CtI");
#endif
		cell(p).val_rate=val_Pros_rat;
		return p;
}
/*************************************************************/
CtI CruTxt::setProso_val_volume( CtI p, INT val_Pros_v)//Aritz
{
#ifdef HTTS_DEBUG
        if (!p) htts_error("CruTxt::setProso_val_volume() - Trying to read null CtI");
#endif

			cell(p).val_volume=val_Pros_v;
			return p;
}
#endif
/**********************************************************/

CtI CruTxt::setEmo_int( CtI p, INT emo_intensity)
{
#ifdef HTTS_DEBUG
        if (!p) htts_error("CruTxt::setEmo_int() - Trying to read null CtI");
#endif
  cell(p).emoInt = emo_intensity;
  return p;
}

/**********************************************************/
/**********************************************************/
#ifdef HTTS_PROSO_VAL
/*INT CruTxt::getProso_tipo(CtI p)
{
#ifdef HTTS_DEBUG
		if(!p) htts_error("CruTxt::getProso_tipo() - Trying to read null CtI");

#endif
 return cell(p).tipo;
}*/
/**************************************************************/
/***************************************************************/
/*CtI CruTxt::setProso_tipo(CtI p, INT pros_tipo)
{
#ifdef HTTS_DEBUG
	if(!p) htts_error("CruTxt::setProso_tipo() - Trying to read null CtI");
#endif
	cell(p).tipo=pros_tipo;
	return p;
}*/
/***************************************************************/
/****************************************************************/
/*INT CruTxt::getProso_val(CtI p)
{
#ifdef HTTS_DEBUG
		if(!p) htts_error("CruTxt::getProso_val() - Trying to read null CtI");

#endif
 return cell(p).valor;
}*/
/******************************************************************/
/*****************************************************************/
/*CtI CruTxt::setProso_val(CtI p, INT pros_val)
{
#ifdef HTTS_DEBUG
	if(!p) htts_error("CruTxt::setProso_val() - Trying to read null CtI");
#endif
	cell(p).valor=pros_val;
	return p;
}*/
/*******************************************************************/
/********************************************************************/
#endif
CtI CruTxt::setDekd(CtI p)
{
#ifdef HTTS_DEBUG
        if (!p) htts_error("CruTxt::setDekd() - Trying to read null CtI");
#endif
	if (!(isStart(p))) return NULL;
	cell(p).status |= DEKD;

	return p;

}

/**********************************************************/


/**********************************************************/

CtI CruTxt::setDek(CtI p)
{
#ifdef HTTS_DEBUG
        if (!p) htts_error("CruTxt::setDek() - Trying to read null CtI");
#endif
	if (!(isStart(p))) return NULL;
	cell(p).status |= DEK;

	return p;

}

/**********************************************************/


/**********************************************************/

CtI CruTxt::setLeftFrg(CtI p)
{
#ifdef HTTS_DEBUG
        if (!p) htts_error("CruTxt::setLeftFrg() - Trying to read null CtI");
#endif
	if (!(isStart(p))) return NULL;
	cell(p).status |= FROMCUT;
	cell(p).status |= LEFTFRG;

	return p;

}

/**********************************************************/


/**********************************************************/

CtI CruTxt::setRightFrg(CtI p)
{
#ifdef HTTS_DEBUG
        if (!p) htts_error("CruTxt::setRightFrg() - Trying to read null CtI");
#endif
	if (!(isStart(p))) return NULL;
	cell(p).status |= FROMCUT;
//	cell(p).status &= !LEFTFRG;

	return p;
}

/**********************************************************/

/**********************************************************/

INT CruTxt::getStatus( CtI p)
{
#ifdef HTTS_DEBUG
        if (!p) htts_error("CruTxt::getStatus() - Trying to read null CtI");
#endif
  return cell(p).status;
}

/**********************************************************/


/**********************************************************/

INT CruTxt::setStatus( CtI p, INT status)
{
#ifdef HTTS_DEBUG
        if (!p) htts_error("CruTxt::setStatus() - Trying to read null CtI");
#endif
		cell(p).status = status;
		return status;
}

/**********************************************************/

/**********************************************************/

TNOR CruTxt::getTnor( CtI p)
{
#ifdef HTTS_DEBUG
        if (!p) htts_error("CruTxt::getTnor() - Trying to read null CtI");
#endif
  return cell(p).tnor;
}

/**********************************************************/


/**********************************************************/

TNOR CruTxt::setTnor( CtI p, TNOR tnor)
{
#ifdef HTTS_DEBUG
        if (!p) htts_error("CruTxt::setTnor() - Trying to read null CtI");
#endif
	cell(p).status |= NOR;
  cell(p).tnor = tnor;
  return tnor;
}

/**********************************************************/


/**********************************************************/

BOOL CruTxt::isNorm(CtI p)
{
#ifdef HTTS_DEBUG
        if (!p) htts_error("CruTxt::isNorm() - Trying to read null CtI");
#endif
	return (cell(p).status & NOR);
}

/**********************************************************/


/**********************************************************/

BOOL CruTxt::setEscSeq( CtI p, BOOL val)
{
#ifdef HTTS_DEBUG
	if (!p) htts_error("CruTxt::setEscSeq() - Trying to read null CtI");
#endif

	if (val) cell(p).status |= ESCSEQ;

	return (cell(p).status & ESCSEQ);
}

/**********************************************************/


/**********************************************************/

BOOL CruTxt::isEscSeq(CtI p)
{
#ifdef HTTS_DEBUG
        if (!p) htts_error("CruTxt::isEscSeq() - Trying to read null CtI");
#endif
	return (cell(p).status & ESCSEQ);
}

/**********************************************************/


