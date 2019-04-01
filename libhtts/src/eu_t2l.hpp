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
#ifndef __EU_TEXT2LIST_HPP__
#define __EU_TEXT2LIST_HPP__

/**********************************************************/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/*
Copyright: 1998 - TSR/Aholab (DET) ETSII/IT-Bilbao

Nombre fuente................ eu_t2l.hpp
Nombre paquete............... normalizador
Lenguaje fuente.............. C++
Estado....................... desarrollo
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Juan Luis Murugarren

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
0.0.31	 09/03/07  inaxio    CtI expUnit( CtI p, BOOL junto );->BOOL junto gehitu.
0.0.31	 02/01/07  inaxio    añadido expSuma desde eu_numhilvl.
0.0.30   29/12/06  inaxio    añadido expNegative y expNegativePunt desde eu_numhilvl.
0.0.29   11/10/04  inigos    añadido getchtype desde wordchop
0.0.28   07/05/01  Yon2.     Non dic Units.
0.0.27   07/05/01  Yon2.     Palabras con ap¢strofe.
0.0.27   27/04/01  Yon2.     Split -> Eusk Mormalizer.
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


#include "t2l.hpp"

/**********************************************************/

#define HEAVY_DEBUGx
#define HEAVY_DEBUG2x


/***********************************************************************/



/***********************************************************************/

class LangEU_TextToList : public TextToList {

public:

	//Devuelve el tipo de caracter: letra, numero...
	INT getchtype(CHAR c);

	// Corta el buffer por el punto
	CtI puntChop(HDicDB *dic);

	//Expansión num‚rica a nivel bajo.
	CtI expnum(CtI p);  //el numero en p lo expande en p
	CtI expnum(CtI q, CtI p); // el numero en q lo expande en p
	CtI expnumafterpoint(CtI p);  //el numero en p lo expande en p

	CtI upTo99(UINT num, CtI p);
	CtI upTo999(UINT num, CtI p );
	CtI expPunt(CtI p, BOOL punto);  //el numero en p lo expande en p

	//Expansión num‚rica a nivel bajo.
	NumType isNum(CtI p);

	//Expansi¢n num‚rica a nivel alto.
	CtI expCard(CtI p);
	CtI expOrd(CtI p, BOOL withPoint);
	CtI expNegative(CtI p); //inaxio
	CtI expNegativePunt(CtI r, BOOL puntu); //inaxio
	CtI expSuma(CtI p, BOOL suma); //inaxio

	//Expansi¢n de horas.
	CtI exptimeHM(CtI p);
	CtI exptimeHMS(CtI p);

	//Expansi¢n de fechas.
	DateType isDate(CtI p);
	CtI expdateYMD(CtI p);
	CtI dateFromSpanish(CtI p);
	CtI dateFromRoman(CtI p);
	CtI dateFromRomanSpanish(CtI p);

	//Numeros romanos fuera de fechas.
	CtI expRoman( CtI p, BOOL nearPoint, BOOL farPoint );
	RomanType isRomanN(CtI p);

	//Expansión de acronimos, abreviaturas y unidades.
	AbbAcrUniType isAbbAcrUni( CtI p );
	CtI expAbbAcrUni( CtI p, BOOL withPoint, BOOL unit );

	//Expanción de por cientos.
	PercentType isPercent( CtI p);

	//Expanción de por cientos.
	CtI expPercent( CtI p , BOOL basque );
	CtI expPercentPunt ( CtI p , BOOL puntu );

	//Expansion de palabras compuestas.
	CtI expComp( CtI p );

	//Expansion de unidades de no diccionario.
	CtI expUnit( CtI p, BOOL junto );//inaxio->BOOL junto gehitu

	//Expansi¢n de palabras con ap¢strofe.
	BOOL isApost( CtI p );
	CtI expApost( CtI p );

	//Declinador.
	CtI dekline( CtI p);
	BOOL isGroupDecd(CtI p);

	//Es pronunciable la cadena contenida en la celda p.
	BOOL isPronun(CtI p);

	//Tiene mayúsculas la cadena
	INT isCap(CtI p);

	// Trata cadenan en mayuscula declinadas
	CtI mayusDekline( CtI p);

	// Hace pronunciable el nombre
	CtI pronounce( CtI p);

	//Kernel Normalización
	INT ct2nt(const CHAR *str, BOOL flush = FALSE);
	INT ct2nt(BOOL flush);
	CtI normal(VOID);

	//Expansor de grupo.
	BOOL mustExpand(CtI p);

	//Expansor de grupo.
	CtI expandGrp(CtI p); //normaliza un grupo desconocido.
	CtI expandCell(CtI p); //normaliza una celda no pronunciable.
	CtI spellCell(CtI p, CtI q); //la celda p la expande en q

};

/***********************************************************************/


#endif

