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
Copyright: 1994 - Grupo de Voz (DAET) ETSII/IT-Bilbao

Nombre fuente................ SPL10.C
Nombre paquete............... SPL
Lenguaje fuente.............. C (Borland C/C++ 3.1)
Estado....................... Completado
Dependencia Hard/OS.......... -
Codigo condicional........... NDEBUG

Codificacion................. Borja Etxebarria

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
2.0.1    23/03/99  Borja     hz2l2() l22hz()
2.0.0    01/07/97  Borja     units.c --> spl10.c
1.3.1    01/07/97  Borja     froundf, float2int salen de SPL (uti_math)
1.3.0    01/05/97  Borja     xnormalize->snormalize, norm_i16, froundf...
1.2.2    22/04/97  Borja     quitar warnings en g++
1.2.1    30/07/95  Borja     scope funciones explicito
1.2.0    30/07/95  Borja     sqa2dB() ---> aa2dB(), etc. (sqa-->aa)
1.1.0    08/12/94  Borja     revision general (tipos,idx,nel,tnel...)
1.0.0    06/07/93  Borja     Codificacion inicial.

======================== Contenido ========================
Conversion entre diversas unidades.

Definir NDEBUG para desconectar la validacion de parametros
con assert(). No definir este simbolo mientras se depuren
aplicaciones, ya que aunque las funciones son algo mas lentas,
el chequeo de validacion de parametros resuelve muchos problemas.
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include <math.h>
#include <assert.h>

#include "spli.h"

/**********************************************************/

#define N10DLN10 4.342944819032518280
#define N20DLN10 (N10DLN10*2.0)

/*<DOC>*/
/**********************************************************/
/* convierte y {devuelve} {samples} de muestras a milisegundos,
si la frecuencia de muestreo es {freq}.
Si {frec}==0 error, o si NDEBUG activado, {devuelve} 0. */

PUBLIC SPL_FLOAT XAPI samples2ms( SPL_FLOAT samples, SPL_FLOAT freq )
/*</DOC>*/
{
	assert(freq!=0.0);
	if (!freq) return 0.0;
	return (samples*1000.0)/freq;
}

/*<DOC>*/
/**********************************************************/
/* convierte y {devuelve} {ms} de milisegundos a muestras,
si la frecuencia de muestreo es {freq}.
Si {frec}==0 error, o si NDEBUG activado, {devuelve} 0. */

PUBLIC SPL_FLOAT XAPI ms2samples( SPL_FLOAT ms, SPL_FLOAT freq )
/*</DOC>*/
{
	assert(freq!=0.0);
	return (ms*freq)/1000.0;
}

/*<DOC>*/
/**********************************************************/
/* convierte y {devuelve} un periodo de {samples} muestras a
Hz, si la frecuencia de muestreo es {freq}.
Si {samples}==0 {devuelve} 0.
Si {frec}==0 error, o si NDEBUG activado, {devuelve} 0. */

PUBLIC SPL_FLOAT XAPI samples2hz( SPL_FLOAT samples, SPL_FLOAT freq )
/*</DOC>*/
{
	assert(freq!=0.0);
	return ((!samples) ? 0.0 : freq/samples);
}

/*<DOC>*/
/**********************************************************/
/* convierte y {devuelve} una frecuencia de {hz} Herzios a periodo
en muestras, si la frecuencia de muestreo es {freq}.
Si {hz}==0 {devuelve} 0.
Si {frec}==0 error, o si NDEBUG activado, {devuelve} 0. */

PUBLIC SPL_FLOAT XAPI hz2samples( SPL_FLOAT hz, SPL_FLOAT freq )
/*</DOC>*/
{
	assert(freq!=0.0);
	return ((!hz) ? 0.0 : freq/hz);
}

/*<DOC>*/
/**********************************************************/
/* convierte y {devuelve} una frecuencia de {hz} Herzios a periodo
en milisegundos. Si {hz}==0 {devuelve} 0 */

PUBLIC SPL_FLOAT XAPI hz2ms( SPL_FLOAT hz )
/*</DOC>*/
{
	return ((!hz) ? 0.0 : 1000.0/hz);
}

/*<DOC>*/
/**********************************************************/
/* convierte y {devuelve} un periodo de {ms} milisegundos a
frecuencia en herzios. Si {ms}==0 devuelve 0 */

PUBLIC SPL_FLOAT XAPI ms2hz( SPL_FLOAT ms )
/*</DOC>*/
{
	return ((!ms) ? 0.0 : 1000.0/ms);
}

/*<DOC>*/
/**********************************************************/
/* convierte y {devuelve} la frecuencia {hz} Herzios a radianes,
si la frecuencia de muestreo es {freq}.
Si {frec}==0 error, o si NDEBUG activado, {devuelve} 0. */

PUBLIC SPL_FLOAT XAPI hz2rad( SPL_FLOAT hz, SPL_FLOAT freq )
/*</DOC>*/
{
	assert(freq!=0.0);
	if (!freq) return 0.0;
	return (hz*(2.0*M_PI))/freq;
}

/*<DOC>*/
/**********************************************************/
/* convierte y {devuelve} la frecuencia {rad} radianes a Herzios,
si la frecuencia de muestreo es {freq}.
Si {frec}==0 error, o si NDEBUG activado, {devuelve} 0. */

PUBLIC SPL_FLOAT XAPI rad2hz( SPL_FLOAT rad, SPL_FLOAT freq )
/*</DOC>*/
{
	assert(freq!=0.0);
	return (rad*freq)/(2.0*M_PI);
}

/*<DOC>*/
/**********************************************************/
/* convierte y {devuelve} la frecuencia {hz} herzios
a LogHz (log2(Hz)). Si {Hz}<=1 {devuelve} 0. */

PUBLIC SPL_FLOAT XAPI hz2lhz( SPL_FLOAT hz )
/*</DOC>*/
{
	return hz>1?lin2log2(hz):0;
}

/*<DOC>*/
/**********************************************************/
/* convierte y {devuelve} la frecuencia {l2} LogHz=log2(Hz))
a Hz. Si {l2}<=0 {devuelve} 0. */

PUBLIC SPL_FLOAT XAPI lhz2hz( SPL_FLOAT l2 )
/*</DOC>*/
{
	return l2>0?log22lin(l2):0;
}

/*<DOC>*/
/**********************************************************/
/* convierte y {devuelve} la frecuencia {hz} herzios
a LnHz (ln(Hz)). Si {Hz}<=e {devuelve} 0. */

PUBLIC SPL_FLOAT XAPI hz2lnhz( SPL_FLOAT hz )
/*</DOC>*/
{
	return hz>M_E?lin2ln(hz):0;
}

/*<DOC>*/
/**********************************************************/
/* convierte y {devuelve} la frecuencia {lnhz} LnHz=ln(Hz))
a Hz. Si {lnhz}<=0 {devuelve} 0. */

PUBLIC SPL_FLOAT XAPI lnhz2hz( SPL_FLOAT lnhz )
/*</DOC>*/
{
	return lnhz>0?ln2lin(lnhz):0;
}

/*<DOC>*/
/**********************************************************/
/* Funciones similares a hz2lhz() y lhz2hz() pero para
otras unidades (muestras y milisegundos):

ms2lms()
lms2ms()
samples2lsamples()
lsamples2samples()
*/
/*</DOC>*/
/* definido en spl.h */


/*<DOC>*/
/**********************************************************/
/* Esta funcion sirve para calcular el ancho de banda a partir de
la posicion de un polo dentro del circulo de radio unidad.
Convierte y {devuelve} el radio {r} ({r}<1) en ancho de banda.
{freq} es la frecuencia de muestreo.
Si {frec}==0 error, o si NDEBUG activado, {devuelve} 0. */

PUBLIC SPL_FLOAT XAPI r2bw( SPL_FLOAT r, SPL_FLOAT freq )
/*</DOC>*/
{
	assert(freq!=0.0);
	return (rad2hz(-2.0*log((r<DB_MIN_A)?DB_MIN_A:r),freq));
}

/*<DOC>*/
/**********************************************************/
/* Esta funcion sirve para calcular la posicion de un polo (dentro
del circulo de radio unidad) a partir del ancho de banda.
convierte y {devuelve} el ancho de banda {bw} en radio r (r<1).
{freq} es la frecuencia de muestreo.
Si {frec}==0 error, o si NDEBUG activado, {devuelve} 0. */

PUBLIC SPL_FLOAT XAPI bw2r( SPL_FLOAT bw, SPL_FLOAT freq )
/*</DOC>*/
{
	assert(freq!=0.0);
	return (exp(-hz2rad(bw,freq)/2.0));
}

/*<DOC>*/
/**********************************************************/
/* convierte y {devuelve} la amplitud lineal {a} a dB, teniendo en
cuenta que el m¡nimo permitido en amplitud lineal es DB_MIN_A para
evitar que se produzca una singularidad (log(0)=-INF) */

PUBLIC SPL_FLOAT XAPI a2db( SPL_FLOAT a )
/*</DOC>*/
{
	assert(a>=0);
	return 20.0*log10((a<DB_MIN_A)?DB_MIN_A:a);
}

/*<DOC>*/
/**********************************************************/
/* convierte y {devuelve} {db} de dB a amplitud lineal */

PUBLIC SPL_FLOAT XAPI db2a( SPL_FLOAT db )
/*</DOC>*/
{
	return exp(db/(N20DLN10));
}

/*<DOC>*/
/**********************************************************/
/* convierte y {devuelve} de la amplitud al cuadrado {aa} a dB
teniendo en cuenta que la amplitud cuadrada minima es DB_MIN_AA */

PUBLIC SPL_FLOAT XAPI aa2db( SPL_FLOAT aa )
/*</DOC>*/
{
	assert(aa>=0);
	return 10.0*log10((aa<DB_MIN_AA)?DB_MIN_AA:aa);
}

/*<DOC>*/
/**********************************************************/
/* convierte y {devuelve} {db} de dB a amplitud lineal al cuadrado */

PUBLIC SPL_FLOAT XAPI db2aa( SPL_FLOAT db )
/*</DOC>*/
{
	return exp(db/(N10DLN10));
}

/*<DOC>*/
/**********************************************************/
/* convierte y {devuelve} de amplitud lineal {a} a bedelios (Bd) siendo
la definicion del Bd:  Bd = 10 * log10 ( {a}*{a} + 1 ) */

PUBLIC SPL_FLOAT XAPI a2bd( SPL_FLOAT a )
/*</DOC>*/
{
	assert(a>=0);
	return 10.0*log10(a*a+1.0);
}

/*<DOC>*/
/**********************************************************/
/* convierte y {devuelve} {bd} de bedelios a amplitud lineal */

PUBLIC SPL_FLOAT XAPI bd2a( SPL_FLOAT bd )
/*</DOC>*/
{
	return sqrt(exp(bd/N10DLN10)-1.0);
}

/*<DOC>*/
/**********************************************************/
/* convierte y {devuelve} {aa} de amplitud lineal al cuadrado
a bedelios */

PUBLIC SPL_FLOAT XAPI aa2bd( SPL_FLOAT aa )
/*</DOC>*/
{
	assert(aa>=0);
	return 10.0*log10(aa+1.0);
}

/*<DOC>*/
/**********************************************************/
/* convierte y {devuelve} {bd} de bedelios a amplitud lineal al cuadrado */

PUBLIC SPL_FLOAT XAPI bd2aa( SPL_FLOAT bd )
/*</DOC>*/
{
	return exp(bd/N10DLN10)-1.0;
}

/*<DOC>*/
/**********************************************************/
/* normaliza (escala) el valor {n} al rango +-1,
de forma que {neg1} pasa ser -1 y {pos1} pasa a ser +1.
Si {n} se sale del rango {neg1,pos1} el valor normalizado
tambien se sale del rango {-1,+1}.
{neg1}!={pos1}, si no da 0.  */

PUBLIC SPL_FLOAT XAPI normalize( SPL_FLOAT n, SPL_FLOAT neg1,
		SPL_FLOAT pos1 )
/*</DOC>*/
{
	assert(neg1!=pos1);
	if (neg1==pos1) return 0;
	return ((n-neg1)*2.0)/(pos1-neg1)-1.0;
}

/*<DOC>*/
/**********************************************************/
/* desnormaliza el valor {n} que va entre -+1 y lo pasa al valor
que toque para el escalado -1--->{neg1}  y +1--->{pos1}.
Si {n} se sale de {-1,+1} el valor desnormalizado tambien se sale
del rango {neg1,pos1} */

PUBLIC SPL_FLOAT XAPI unnormalize( SPL_FLOAT n, SPL_FLOAT neg1,
		SPL_FLOAT pos1 )
/*</DOC>*/
{
	return neg1+((n+1.0)*(pos1-neg1))/2.0;
}

/*<DOC>*/
/**********************************************************/
/* como normalize pero no permite que el valor escalado se salga
de rango, truncandolo (saturando) a {-1,+1}.
{neg1}!={pos1}, si no da 0.  */

PUBLIC SPL_FLOAT XAPI snormalize( SPL_FLOAT n, SPL_FLOAT neg1,
		SPL_FLOAT pos1 )
/*</DOC>*/
{
	SPL_FLOAT d = normalize(n,neg1,pos1);
	if (d>1.0)
		return 1.0;
	else if (d<-1.0)
		return -1.0;
	else
		return d;
}

/*<DOC>*/
/**********************************************************/
/* como unnormalize pero no permite que el valor deescalado se
salga de rango, truncandolo (saturando a {neg1, pos1} */

PUBLIC SPL_FLOAT XAPI sunnormalize( SPL_FLOAT n, SPL_FLOAT neg1,
		SPL_FLOAT pos1 )
/*</DOC>*/
{
	if (n<-1.0)
		return unnormalize(-1.0,neg1,pos1);
	else if (n>1.0)
		return unnormalize(1.0,neg1,pos1);
	else
		return unnormalize(n,neg1,pos1);
}

/*<DOC>*/
/**********************************************************/
/* Normaliza a +-1 un INT16 */

PUBLIC SPL_FLOAT XAPI norm_i16( INT16 i16 )
/*</DOC>*/
{
	return ((SPL_FLOAT)i16)/((SPL_FLOAT)INT16_RANGE);
}

/*<DOC>*/
/**********************************************************/
/* Desnormaliza de +-1 a rango INT16 */

PUBLIC INT16 XAPI unnorm_i16( SPL_FLOAT f )
/*</DOC>*/
{
	f *= INT16_RANGE;
	if (f>=0) return (INT16)(f+0.5);
	return (INT16)(f-0.5);
}

/*<DOC>*/
/**********************************************************/
/* Desnormaliza de +-1 a rango INT16
SPL_FLOAT snorm_i16( INT16 i16) */
/*</DOC>*/
/* definido en spl.h */

/* #define snorm_i16(i16)  norm_i16(i16) */


/*<DOC>*/
/**********************************************************/
/* Desnormaliza de +-1 a rango INT16, saturando si hace falta. */

PUBLIC INT16 XAPI sunnorm_i16( SPL_FLOAT f )
/*</DOC>*/
{
	f *= INT16_RANGE;
	if (f>=INT16_MAX) return INT16_MAX;
	else if (f<=INT16_MIN) return INT16_MIN;
	else if (f>=0) return (INT16)(f+0.5);
	return (INT16)(f-0.5);
}

/*<DOC>*/
/**********************************************************/
/* Calcula el log2(x), con tope inferior para x<LOG2_MIN. */

PUBLIC SPL_FLOAT XAPI lin2log2( SPL_FLOAT x )
/*</DOC>*/
{
	return log((x<LOG2_MIN )?LOG2_MIN :x)/M_LN2;
}

/*<DOC>*/
/**********************************************************/
/* Calcula el antilog2(x) */

PUBLIC SPL_FLOAT XAPI log22lin( SPL_FLOAT x )
/*</DOC>*/
{
	return pow(2,x);
}

/**********************************************************/
/* Calcula el ln(x), con tope inferior para x<LN_MIN. */

PUBLIC SPL_FLOAT XAPI lin2ln( SPL_FLOAT x )
/*</DOC>*/
{
	return log((x<LN_MIN )?LN_MIN :x);
}

/*<DOC>*/
/**********************************************************/
/* Calcula el antilog2(x)

SPL_FLOAT XAPI ln2lin( SPL_FLOAT x );
</DOC> */

/* definido en spl.h */

/*#define ln2lin(x) exp(x)*/

/**********************************************************/
