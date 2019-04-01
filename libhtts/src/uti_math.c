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
Copyright: 1993 - Grupo de Voz (DAET) ETSII/IT-Bilbao

Nombre fuente................ UTI_MATH.C
Nombre paquete............... UTI.H
Lenguaje fuente.............. C (BC31,GCC)
Estado....................... Utilizable
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Borja Etxebarria

Version  dd/mm/aa  Autor     Comentario
-------  --------  --------  ----------
1.0.1    11/05/11  Inaki     Eliminar warning gcc-4.5
1.0.0    01/07/97  Borja     Codificacion inicial.

======================== Contenido ========================
Funciones matematicas diversas
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include <math.h>

#include "uti.h"

/**********************************************************/
/* redondea un valor double {n} al valor entero (double)
mas proximo */

double fround( double n )
{
	return ((n>=0.0) ? floor(n+0.5) : ceil(n-0.5));
}

/**********************************************************/
/* redondea un valor double {n} al valor entero (int)
mas proximo */

int float2int( double n )
{
	return ((n>=0.0) ? (int)(n+0.5) : (int)(n-0.5));
}

/**********************************************************/
/* redondea un valor double {n} al valor entero (long)
mas proximo */

long float2long( double n )
{
	return ((n>=0.0) ? (long)(n+0.5) : (long)(n-0.5));
}

/**********************************************************/
/* dados {y1} {y2} {y3} siendo {y2} mayor o igual que los
otros dos valores, efectua una interpolacion cuadratica
(parabola) entre los tres puntos y {devuelve} la
posicion X normalizada del maximo (en el rango [-1,1], y tal
que los valores -1,0,1 se corresponden respectivamente con
los valores y1,y2,y3). Si en {y} se envia !=NULL, se devuelve
el valor del maximo referido a {y2} (max-y2, probablemente algo
mayor que 0). */

double cuadfit( double y1, double y2, double y3, double *y )
{
  double b1 = (y3-y1)*0.5;
  double b2 = (y3+y1)*0.5-y2;
  //double x = ( b2 ? x = -0.5*b1/b2 : 0 );
  double x = ( b2 ? -0.5*b1/b2 : 0 );	//inaki
  if (y) *y = (b1+b2*x)*x;
  return x;
}

/**********************************************************/
/* {devuelve} el valor "{x} modulo {y}" pero en el rango -y/2
a y/2). */

#ifdef REMAINDER
double remainder( double x, double y )
{
	double r,y2;
	r = fmod(x,y);
	y = fabs(y);
	y2 = y/2;
	if (r>=y2) r -= y;
	else if (r<=-y2) r += y;
	return r;
}
#endif

/**********************************************************/
/*Interpola linealmente y {devuelve} y para el valor {x}:
.                        {x1}----->{y1}
.                        {x2}----->{y2}
Entonces:
.                        {x}----->y ?  ==>> interpolacion lineal

Si {x1}=={x2} {devuelve} la media de {y1} e {y2}
*/

double linterpol( double x, double x1, double x2,
		double y1, double y2 )
{
	if (x1==x2) return (y1+y2)*0.5;
	else return y1 + ((x-x1)/(x2-x1))*(y2-y1);
}

/**********************************************************/
