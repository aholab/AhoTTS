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
#ifndef __CTLIST_HPP__
#define __CTLIST_HPP__

/**********************************************************/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/*
Copyright: 1998 - TSR/Aholab (DET) ETSII/IT-Bilbao

Nombre fuente................ ctlist.hpp
Nombre paquete............... normalizador
Lenguaje fuente.............. C++
Estado....................... desarrollo
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Juan Luis Murugarren

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
0.1.0	 12/07/10  Inaki	 Añadir celdas para modificar la prosodia desde el texto (proyecto Aritz) HTTS_PROSO_VAL
0.0.10   18/06/01  Yon2.     Scape sequencies support.
0.0.9    09/01/01  Yon2.     Flush propagation.
0.0.8    09/01/01  Yon2.     Empty.
0.0.7    06/01/01  Yon2.     LastGrp(), last().
0.0.6    13/12/00  Yon2.     Last&First in Group.
0.0.5    11/12/00  Yon2.     Soporte tipo de normalizaci¢n.
0.0.4    07/11/00  Yon2.     Soporte declinaciones.
0.0.3    19/11/00  Yon2.     Aniadir funcion extendStr.
0.0.2    09/11/00  Yon2.     Renombrar 'start' a 'status'
0.0.1    09/11/00  Yon2.     Añadir más significado al campo start.
0.0.0    30/10/00  Yon2.     Codificacion inicial.

======================== Contenido ========================

  Esta clase se almacenar palabras cortadas del stream por
el que viene el texto crudo en forma de lista. Mantiene
agrupadas palabras que forman unidades semánticas. Por
ejemplo: direcciones de correo, fechas, horas, ... en los
que se expresan los contenidos con combinación de
caracteres de distinto tipo (patron). Por ejemplo un patron
de fecha 2000/VII/15 ser¡a "nslsn", o sea, n£mero, s¡mbolo,
letra, s¡mbolo, n£mero.

Tiene m‚todos para recorrer la lista por celdas, por grupos;
acceder al contenido de las celdas, insertar celdas, borrar
celdas, borrar grupos ...
Cada grupo tiene unas propiedades como si el grupo est 
declinado, si es una declinaci¢n, si esta normalizado,
que tipo de normalizaci¢n soporta, si es un fragmento de
un grupo m s grande anterior, ...

Yon2.
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/


#include "tdef.h"
#include "tnor.h"
#include "strl.hpp"
#include "htts_cfg.h"
#include "httsmsg.h"

/**********************************************************/
//El campo status, además de si una celda es principio
//de grupo, recoge si la celda proviene del corte de otra,
//y de que posición (delante o detrás).

enum	{
	START   = 0x0001,	/* Principio de grupo. */
	FROMCUT = 0x0002,	/* Viene de un corte. */
	LEFTFRG = 0x0004,	/* Corte por la izquierda o por la derecha. */
	DEKD	= 0x0008,	/* Es una palabra declinada. */
	DEK	= 0x0010,	/* Es una declinaci¢n. */
	NOR	= 0x0020,	/* Est  normalizado. */
	FLUSH	= 0x0040,	/* Ultimo grupo, tras un flush de Input */
	ESCSEQ	= 0x0080	/* Secuencia de escape */
};

/**********************************************************/


/**********************************************************/

struct Celda	{
	pCHAR str;
	INT status;
	pCHAR pattern;
	INT emo;
	INT emoInt;
	TNOR tnor;
	#ifdef HTTS_PROSO_VAL
	INT val_break;//Aritz
	INT val_emphasis;//Aritz
	INT val_pitch;//Aritz
	INT val_range;//Aritz
	INT val_rate;//Aritz
	INT val_volume;//Aritz*/
	#endif

};


typedef Lix CtI; //El índice de texto crudo.

/**********************************************************/



/**********************************************************/

class CruTxt {
protected:
	typedef ListT<Celda> CeldaItemList;
 	CeldaItemList cl;
public:
        CruTxt( VOID ) {};
        ~CruTxt( VOID ) ;

        Celda& cell( CtI p ) const;
        Celda& operator() ( CtI p ) const { return cell(p); }

//Para añadir celdas.
	#ifdef HTTS_PROSO_VAL
	/*Aritz*/CtI append(pCHAR str, BOOL start, INT emotion, INT emo_intensity,INT val_Pros_b,INT val_Pros_e,INT val_Pros_p,INT val_Pros_ran,INT val_Pros_rat,INT val_Pros_v, pCHAR pattern = NULL);
	/*Aritz*/CtI prepend(pCHAR str, BOOL start, INT emotion, INT emo_intensity,INT val_Pros_b, INT val_Pros_e,INT val_Pros_p,INT val_Pros_ran,INT val_Pros_rat,INT val_Pros_v,pCHAR pattern = NULL);
	/*Aritz*/CtI insbefore(CtI p, pCHAR str, BOOL start, INT emotion = 1, INT emo_intensity = 100,INT val_Pros_b=0,INT val_Pros_e=0,INT val_Pros_p=0,INT val_Pros_ran=0,INT val_Pros_rat=0,INT val_Pros_v=0, pCHAR pattern = NULL);
	/*Aritz*/CtI insafter(CtI p, pCHAR str, BOOL start, INT emotion = 1, INT emo_intensity = 100,INT val_Pros_b=0,INT val_Pros_e=0,INT val_Pros_p=0,INT val_Pros_ran=0,INT val_Pros_rat=0,INT val_Pros_v=0, pCHAR pattern = NULL);
	#else
	CtI append(pCHAR str, BOOL start, INT emotion, INT emo_intensity, pCHAR pattern = NULL);
	CtI prepend(pCHAR str, BOOL start, INT emotion, INT emo_intensity, pCHAR pattern = NULL);
	CtI insbefore(CtI p, pCHAR str, BOOL start, INT emotion = 1, INT emo_intensity = 100, pCHAR pattern = NULL);
	CtI insafter(CtI p, pCHAR str, BOOL start, INT emotion = 1, INT emo_intensity = 100, pCHAR pattern = NULL);
	#endif

	//Utiles.
	BOOL empty( VOID ) { return cl.empty(); }
	LIINT index( CtI p ) { return cl.index(p); }

	//Exportar las emociones
	INT getEmotion (CtI p);
	INT getEmo_int (CtI p);
	#ifdef HTTS_PROSO_VAL
	//Exportar las etiquetas de prosodia
	INT getProso_val_break(CtI p);//Aritz
	INT getProso_val_emphasis(CtI p);//Aritz
	INT getProso_val_pitch(CtI p);//Aritz
	INT getProso_val_range(CtI p);//Aritz
	INT getProso_val_rate(CtI p);//Aritz
	INT getProso_val_volume(CtI p);//Aritz
	#endif

	//Importar y exportar el estado.
	INT getStatus( CtI p );
	INT setStatus( CtI p, INT status );

	//Contenido de la cadena de una celda.
	pCHAR getStr( CtI p );

	//Para imponer valores a celdas ya creadas.
	CtI setEmotion (CtI p, INT emotion);
	CtI setEmo_int (CtI p, INT emo_intensity);
	CtI setStr( CtI p, pCHAR str );
	CtI setStart( CtI p, BOOL start );
	CtI setPattern( CtI p, pCHAR pattern );
	CtI setPatternForce( CtI p, pCHAR pattern );
	CtI setRightFrg( CtI p );
	CtI setLeftFrg( CtI p );
	CtI extendStr( CtI p, pCHAR str );
	CtI setDekd( CtI p );
	CtI setDek( CtI p );
	CtI setFlush( CtI p );
	#ifdef HTTS_PROSO_VAL
	CtI setProso_val_break( CtI p, INT val_Pros_b);//Aritz
	CtI setProso_val_emphasis(CtI p, INT val_Pros_e);//Aritz
	CtI setProso_val_pitch( CtI p,INT val_Pros_p);//Aritz
	CtI setProso_val_range( CtI p, INT val_Pros_ran);//Aritz
	CtI setProso_val_rate( CtI p,INT val_Pros_rat);//Aritz
	CtI setProso_val_volume(CtI p, INT val_Pros_v);//Aritz*/
	#endif
	//Tipo de normalizaci¢n
	TNOR getTnor( CtI p);
	TNOR setTnor( CtI p, TNOR tnor);

	//Secuencias de escape.
	BOOL setEscSeq(CtI p, BOOL val);
	BOOL isEscSeq(CtI p);

	//Para comsultar propiedades de un grupo.
	BOOL isStart( CtI p ); /* Es principio de grupo */
	BOOL isFrgmnt( CtI p ); /* Viene de un corte de un grupo mayor */
	BOOL isRightFrg( CtI p ); /* Estaba pegado por detrás */
	BOOL isLeftFrg( CtI p ); /* Estaba pegado por delante */
	BOOL isDekd( CtI p ); /* Est  declinada? */
	BOOL isDek( CtI p ); /* Es una declinaci¢n? */
	BOOL isNorm( CtI p ); /* Est  normalizado? */
	BOOL isFlush( CtI p ); /* Grupo viene de un flush? */

	//Estas son para moverse por la lista a nivel de celda.
	CtI first( VOID );
	CtI last( VOID );
	CtI next( CtI p );
	CtI prev( CtI p );

	//Estas son para moverse por la lista a niver de grupo.
	CtI firstGrp( VOID );
	CtI lastGrp( VOID );
	CtI nextGrp( CtI p );
	CtI prevGrp( CtI p );
	CtI firstInGrp( CtI p );
	CtI lastInGrp( CtI p );

	//Para borrar celdas y grupos. Propagar el grupo si se borra celda??
	CtI del( CtI p );
	CtI delGrp( CtI p );
};

/**********************************************************/


/**********************************************************/

#endif
