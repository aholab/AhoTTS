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
#ifndef __WORDCHOP_HPP__
#define __WORDCHOP_HPP__

/**********************************************************/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/*
Copyright: 1998 - TSR/Aholab (DET) ETSII/IT-Bilbao

Nombre fuente................ wordchop.hpp
Nombre paquete............... normalizador
Lenguaje fuente.............. C++
Estado....................... desarrollo
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Juan Luis Murugarren

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
0.1.0	 12/07/10  Inaki	 Añadir celdas para modificar la prosodia desde el texto (proyecto Aritz) HTTS_PROSO_VAL
0.0.27   28/11/01  Yon2.     Multi chtype support.
0.0.26   24/04/01  Yon2.     Include Split.
0.0.25   30/03/01  Yon2.     expandCell.
0.0.24   29/03/01  Yon2.     Percent.
0.0.23   28/03/01  Yon2.     More Data Need.
0.0.22   25/03/01  Yon2.     Comp words.
0.0.21   22/03/01  Yon2.     Abb, acr & units.
0.0.20   18/03/01  Yon2.     Roman -> more funcional.
0.0.19   12/02/01  Yon2.     Roman not inside date.
0.0.18   11/02/01  Yon2.     Use create method param WS.
0.0.17   05/02/01  Yon2.     Create method.
0.0.16   15/01/01  Yon2.     Text output support.
0.0.15   08/01/01  Yon2.     Flushing support.
0.0.14   08/01/01  Yon2.     WS output.
0.0.13   07/01/01  Yon2.     CALLBACK support.
0.0.12   07/01/01  Yon2.     Parametros correctos a output.
0.0.11   06/01/01  Yon2.     Ultimo grupo aniadido completo?
0.0.10   04/01/00  Yon2.     Std module i/o.
0.0.9    26/12/00  Yon2.     UttWs support.
0.0.8    12/11/00  Yon2.     Group speller.
0.0.7    09/11/00  Yon2.     Aniadir isNum.
0.0.6    29/11/00  Yon2.     Aniadir isDate.
0.0.5    29/11/00  Yon2.     Conversión de fechas.
0.0.4    xx/11/00  Yon2.     Expansion de fechas.
0.0.3    xx/11/00  Yon2.     Expansion de horas.
0.0.2    20/11/00  Yon2.     Normalizar n£meros b sico.
0.0.1    19/11/00  Yon2.     Aniadir exp de numeros a nivel bajo.
0.0.0    30/10/00  Yon2.     Codificacion inicial.

======================== Contenido ========================

  Esta el la clase fundamental del normalizador. En ella
se encuentran los m¢dulos de entrada/salida, de procesado/
expansi¢n, detecci¢n de grupos expandibles, declinador,
expansor de grupos desconocidos, deletrador.

  La entrada se hace cortando palabras del stream por
el que viene el texto crudo. Separa el stream en palabras
conteniendo caracteres del mismo tipo y conservando la
cercanía que hay entre caracteres. Esto último mantiene
agrupadas palabras que forman unidades semánticas. Por
ejemplo: direcciones de correo, fechas, horas, ... en los
que se expresan los contenidos con combinación de
caracteres de distinto tipo (patrones).

Preprocesados (previos a la normalizaci¢n):
   Separaci¢n del stream en grupos sem nticos.
   Aislamiento de los s¡mbolos de puntuaci¢n.
   Localizaci¢n y aislamiento de grupos deklinados.

Se detectan/expanden:
   Fechas: En euskara, castellano, con y sin n£meros romanos.
   Horas y lapsos de tiempos.
   N£meros.
   ...
   Grupos desconocidos.
   Palabras no pronunciables. (todav¡a no).

Se define aqu¡ HEAVY_DEBUG que indica si se informara del
proceso de E/S a trav‚s de htts_warn.
Se define aqu¡ HEAVY_DEBUG2 que indica si se informara del
proceso del ciclo de normalizacion a trav‚s de htts_warn.


Yon2.

===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

/**********************************************************/

#include "tdef.h"
#include "ctlist.hpp"
#include "hdic.hpp"


/**********************************************************/

#define MAX_EMO_NESTED 30 /*Numero máximo de emociones anidadas*/
#define MAX_WORD_BUFFER 256 /* Una palabra de 256 caracteres !!!!!! */
#define MAX_PATTERN_BUFFER 128 /* Un patrón de 128 caracteres !!!!!! */
#define MAX_LABEL_BUFFER 128 /* Una etiqueta de 128 caracteres !!!!!! */
#define MAX_LABEL_WORDS 32
#define MAX_WORD_LEN 64
#ifdef HTTS_PROSO_VAL
#define MAX_PROS_NESTED 100
#endif
//Verificar el desborde del patrón
/***********************************************************************/

#define EMO_DEFAULT_STR "DEFAULT"
#define EMO_NEUTRAL_STR "NEUTRAL"
#define EMO_AFRAID_STR "AFRAID"
#define EMO_ANGRY_STR "ANGRY"
#define EMO_CONFUSED_STR "CONFUSED"
#define EMO_DAZED_STR "DAZED"
#define EMO_DISGUSTED_STR "DISGUSTED"
#define EMO_HAPPY_STR "HAPPY"
#define EMO_SAD_STR "SAD"
#define EMO_SURPRISED_STR "SURPRISED"

#ifdef HTTS_PROSO_VAL
/**************************************************************************/
#define PROS_EMPHASIS       "EMPHASIS"	//Aritz
#define PROS_PROSODY_PITCH  "PROSODY PITCH"	//
#define PROS_PROSODY_RANGE	"PROSODY RANGE"	//
#define PROS_PROSODY_RATE	"PROSODY RATE"//
#define PROS_PROSODY_VOLUME	"PROSODY VOLUME" //
#define PROS_BREAK			"BREAK"//
/************************************************************************/
#define X_LOW -12
#define LOW  -6
#define MEDIUM_P 6//Aritz Valores de Pitch
#define HIGH 12
#define X_HIGH 18
/***********************************************************************/
#define X_WEAK 100
#define WEAK 150
#define MEDIUM_B 200
#define STRONG 250//Aritz Valores de pausas
#define X_STRONG 300
/*************************************************************************/
#define REDUCED -10//Aritz Valores de emphasis
#define MODERATE 5
#define STRONG_E 10
/************************************************************************/
#define X_SLOW -8//Aritz Valores de rate
#define SLOW -6
#define MEDIUM_RAT 3

#define FAST 6
#define X_FAST 8
/*************************************************************************/
#define X_SOFT -50//Aritz Valores de volumen
#define SOFT -30
#define MEDIUM_V 5
#define LOUD 30
#define X_LOUD 40
#define DEFAULT 0
/********************************************************************/

#define X_LOW_RAN -200
#define LOW_RAN -100
#define MEDIUM_RAN 100//Aritz Valores de rango
#define HIGH_RAN 175
#define X_HIGH_RAN 225
/***********************************************************************/
#define DEFAULT 0
#endif

class WdChop {
enum _Emotion {

	UEMOTION_DEFAULT=0,
	UEMOTION_NEUTRAL=1,
	UEMOTION_AFRAID=2,
	UEMOTION_ANGRY=3,
	UEMOTION_CONFUSED=4,		//Sin implementar
	UEMOTION_DAZED=5,			//Sin implementar
	UEMOTION_DISGUSTED=6,	 	//Sin implementar
	UEMOTION_HAPPY=7,
	UEMOTION_SAD=8,
	UEMOTION_SURPRISED=9

};

protected:
#ifdef HTTS_PROSO_VAL
enum _Prosody//
{
	UDEFAULT=0,//
	UEMPHASIS=10,//
	UPROSODY_PITCH=11,//
	UPROSODY_RANGE=12,//
	UPROSODY_RATE=13,//
	UPROSODY_VOLUME=14,//
	UBREAK=15//
};
protected:
#endif

	CruTxt ct;

	//Variables para el recorte de patrones.
	CHAR wordbuff[MAX_WORD_BUFFER];
	CHAR pattern[MAX_PATTERN_BUFFER];
	INT j;
	INT k;
	INT chtypemem;
	BOOL wordStarted;
	BOOL groupStarted;

	//Variables para recordar el estado entre entradas.
	CtI lastGrpProc;
	BOOL procLastGrp;

	INT emo[MAX_EMO_NESTED];
	INT emoInt [MAX_EMO_NESTED];
	int indiceEmo;

	char labelBuff [MAX_LABEL_BUFFER];
	bool labelStarted;

public:
	#ifdef HTTS_PROSO_VAL
	INT Pros[MAX_PROS_NESTED];//Aritz
	int indicePros;//Aritz
	int val_Pros_pitch;//Aritz
	int val_Pros_break;//Aritz
	int val_Pros_emphasis;//Aritz
	//int val;//Aritz
	int val_Pros_range;
	int val_Pros_rate;
	int val_Pros_volume;
	int cont;
	#endif
	WdChop( VOID ) {indiceEmo = 0;
					emo[0] = 1;
					emoInt[0] = 100;
		#ifdef HTTS_PROSO_VAL
		indicePros=0;//Aritz
		Pros[0]=0;//Aritz
		//val=0;//Aritz
		val_Pros_pitch=0;//Aritz
		val_Pros_break=0;//Aritz
		val_Pros_emphasis=0;//Aritz
		val_Pros_range=0;//Aritz
		val_Pros_rate=0;//Aritz
		val_Pros_volume=0;//Aritz
		cont=0;
		#endif
	};
	virtual ~WdChop( VOID ) {};

	INT indexEmo (String & Emo);
	BOOL filterLabel(pCHAR label);
	CtI parserEmo(pCHAR fltbuff, BOOL flush = false);
	#ifdef HTTS_PROSO_VAL
	BOOL EtiqPros(pCHAR label);//Aritz
	INT indexProsod(String Pros);//Aritz
	BOOL ValorValPitch(String valor,int*val_Pros_pitch);//Aritz
	BOOL ValorValRange(String valor,int *val_Pros_range);//Aritz

	BOOL ValorValRate(String valor,int *val_Pros_rate);//Aritz
	BOOL ValorValProsodyVolum(String valor, int *val_Pros_volume);//Aritz
	BOOL ValorValEmphasis(String valor,int *val_Pros_emphasis);//Aritz
	BOOL ValorValBreakStrength(String valor,int *val_Pros_break);//Aritz
	BOOL ValorValBreakTime(String valor,int *val_Pros_break);//Aritz
	#endif
	CtI patCutLeft(CtI p);
	CtI patCutRight(CtI p);
	BOOL hasLeftPuntt(pCHAR pattern);
	BOOL hasRightPuntt(pCHAR pattern);

	//Operaciones de corte de stream, grupos, patrones.
	CtI preChop(pCHAR fltbuff);
	virtual CtI puntChop(HDicDB *dic)=0;

	virtual CHAR getpatsym(INT chtype); //Simbolo utilizado para rellenar el patr¢n
	virtual pCHAR strChType(INT chtype); //Forma legible del tipo de patr¢n.
	virtual INT getchtype(CHAR c); //Devuelve el tipo de caracter: letra, numero, .

	pCHAR concatStr(pCHAR to, pCHAR from); // Recopia de otra clase pero necesaria
};

/***********************************************************************/


#endif
