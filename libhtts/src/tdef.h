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
#ifndef __TDEF_H
#define __TDEF_H

/**********************************************************/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/*
Copyright: 1993 - Grupo de Voz (DAET) ETSII/IT-Bilbao

Nombre fuente................ TDEF.H
Nombre paquete............... -
Lenguaje fuente.............. C (Borland C/C++ 3.1)
Estado....................... Completado
Dependencia Hard/OS.......... Si!! (definicion de tipos fundamentales)
Codigo condicional........... -

Codificacion................. Borja Etxebarria

Version  dd/mm/aa  Autor     Comentario
-------  --------  --------  ----------
1.5.2	 18/01/00  inaki     que el tamaño de los typedef para 64bits y 32 coincidan
1.5.1    29/04/08  ikerl     typedef void VOID no compatible con gcc 4.2.3
1.5.0    20/01/98  Borja     defines SIZEOF_???
1.4.1    20/01/98  Borja     Soporte MSVC++ (template complex)
1.4.0    04/05/97  Borja     definicion de MINALIGN
1.3.3    04/03/97  Borja     usar double_complex con el GCC
1.3.2    22/07/96  Borja     utiliza ARCH.H para resolver OS/CC
1.3.1    11/07/96  Borja     tipos nuevos (LDOUBLE, SHORT)
1.3.0    27/07/95  Borja     tdef.hpp incluido en tdef.h
1.2.2    27/07/95  Borja     soporte tipos Windows 3.1
1.2.1    15/03/95  Borja     reconocim. S.O y compilador (__MSDOS__, etc)
1.2.0    12/06/94  Borja     Tipos de longitud fija en bits (INT2-->INT16...)
1.1.0    26/06/92  Borja     Soporte near/far con PTRN y PTRF.
1.0.0    06/05/92  Borja     Codificacion inicial.

======================== Contenido ========================
<DOC>
Fichero de cabecera, macro para definicion de tipos.
Definicion de tipos fundamentales.

Todos los modulos basados en TDEF.H utilizan ARCH.H para
su auto-configuracion en funcion del OS/CC.

Se definen estos tipos basicos:
INT  -> entero (int)
pINT -> puntero a entero (int *)

De forma similar se definen LONG, FLOAT, DOUBLE, CHAR,
SHORT, VOID, UINT (unsigned int), UCHAR (unsigned char).

Tambien se definen tipos de longitud conocida (utiles para
salvar/cargar ficheros en diferentes plataformas):
INT8, INT16, INT32, UINT8, UINT16, UINT32, CHAR8, UCHAR8,
FLOAT32, FLOAT64, etc.

Se definen constantes SIZEOF_INT, SIZEOF_DOUBLE etc etc para
saber la dimension de los datos en tiempo de preprocesador (el
sizeof(DOUBLE) solo existe en tiempo de compilacion).

Tambien hay macros para controlar el alineamiento de estructuras
y otras cosillas. Repasar el contenido del fichero.
</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include "arch.h"

/**********************************************************/
/* Definicion de un puntero normal, near y far  */

#define PTR  *        /* puntero normal */


#if defined(__OS_MSDOS__)      /* Solo para MS-DOS */

#define PTRN  near *  /* puntero near */
#define PTRF  far *   /* puntero far */
#define PTRH  huge *  /* puntero huge */

#elif defined(__OS_WIN31__)     /* Windows */

#define PTRN  _near *  /* puntero near */
#define PTRF  _far *   /* puntero far */
#define PTRH  _huge *  /* puntero huge */

#elif defined(__OS_UNIX__)        /* Unix */

#define PTRN  PTR
#define PTRF  PTR
#define PTRH  PTR

#elif defined(__OS_WIN32__)   /* WIN32*/

#define PTRN  PTR
#define PTRF  PTR
#define PTRH  PTR

#elif defined(__OS_WIN64__)   /* WIN64*/

#define PTRN  PTR
#define PTRF  PTR
#define PTRH  PTR

#else
#error plataforma no configurada
#endif

/**********************************************************/
/* Plantilla de definicion de un nuevo tipo NAME y puntero
a NAME (pNAME), siendo NAME de tipo TYPE.
Tambien define punteros near, far, huge al tipo (pnNAME, pfNAME, phNAME) */

#define __TDEF(TYPE,NAME)  \
	typedef TYPE NAME;  \
	typedef NAME PTR p##NAME;  \
	typedef NAME PTRN pn##NAME;  \
	typedef NAME PTRF pf##NAME;  \
	typedef NAME PTRH ph##NAME

/**********************************************************/
/* importante! ver defines SIZEOF_??? mas abajo, que dependen de estos tipos! */

__TDEF(int, INT);          /* define INT, pINT, pnINT, pfINT, phINT */
#ifdef __BITS_64__
__TDEF(int, LONG);
#else
__TDEF(long, LONG);
#endif
#ifndef __OS_WIN32__
__TDEF(long long, LLONG);
#endif
__TDEF(short, SHORT);
__TDEF(char, CHAR);
__TDEF(float, FLOAT);
__TDEF(double, DOUBLE);
__TDEF(long double, LDOUBLE);
#ifndef VOID
__TDEF(void, VOID);
/* A partir de gcc 4.2.3 typedef void VOID no esta soportado correctamente
 * Lo corregimos con esta chapucilla
 */
#define VOID void

#else  /* chapucilla para cuando VOID ya este definido */
	typedef VOID PTR pVOID;
	typedef VOID PTRN pnVOID;
	typedef VOID PTRF pfVOID;
	typedef VOID PTRH phVOID;
#endif
__TDEF(int, BOOL);

__TDEF(unsigned int, UINT);
#ifdef __BITS_64__
__TDEF(unsigned int, ULONG);
#else
__TDEF(unsigned long, ULONG);
#endif
#ifndef __OS_WIN32__
__TDEF(unsigned long long, ULLONG);
#endif
__TDEF(unsigned char, UCHAR);
__TDEF(unsigned int, USHORT);

/* En C++, define tambien el tipo complejo */
#if defined(__CC_CPLUSPLUS__)

#ifdef __CC_GNUC__
#include <complex>
__TDEF(std::complex<double>, COMPLEX);  /* define COMPLEX, pCOMPLEX ... */
#elif __CC_MSVC__
#include <complex>
__TDEF(std::complex<double>, COMPLEX);  /* define COMPLEX, pCOMPLEX ... */
#else
#include <complex.h>
__TDEF(complex, COMPLEX);  /* define COMPLEX, pCOMPLEX ... */
#endif

#endif

/**********************************************************/
/* defines para saber el sizeof() de los tipos mas importantes.
Depende del SO!. */

/* MSDOS y Win31, SO de 16 bits */
#if defined(__BITS_16__)
#define SIZEOF_INT 2
#define SIZEOF_LONG 4
#define SIZEOF_LLONG 4
#define SIZEOF_SHORT 2
#define SIZEOF_CHAR 1
#define SIZEOF_FLOAT 4
#define SIZEOF_DOUBLE 8
#define SIZEOF_LDOUBLE 10
#define SIZEOF_UINT 2
#define SIZEOF_ULONG 4
#define SIZEOF_ULLONG 4
#define SIZEOF_USHORT 2
#define SIZEOF_UCHAR 1

/* Resto, 32 bits y 64 bits (se ha forzado a que LONG tenga 4 bytes incluso en 64) */
#elif defined(__BITS_32__) || defined(__BITS_64__)
#define SIZEOF_INT 4
#define SIZEOF_LONG 4
#define SIZEOF_LLONG 8
#define SIZEOF_SHORT 2
#define SIZEOF_CHAR 1
#define SIZEOF_FLOAT 4
#define SIZEOF_DOUBLE 8
#define SIZEOF_LDOUBLE 12
/* $$$ no estoy seguro de que el LDOUBLE sea de 12bytes, salvo en Linux */
#define SIZEOF_UINT 4
#define SIZEOF_ULONG 4
#define SIZEOF_ULLONG 8
#define SIZEOF_USHORT 2
#define SIZEOF_UCHAR 1

#else
#error plataforma no configurada
#endif

/**********************************************************/
/* valores booleanos */

#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE  0
#endif

/**********************************************************/
/* tipos de longitud determinada (en bits) */

#if defined(__OS_MSDOS__)

__TDEF(char, INT8);                /* signed ints */
__TDEF(short, INT16);
__TDEF(long, INT32);
__TDEF(unsigned char, UINT8);      /* unsigned ints */
__TDEF(unsigned short, UINT16);
__TDEF(unsigned long, UINT32);
__TDEF(char, CHAR8);               /* char */
__TDEF(unsigned char, UCHAR8);     /* unsigned char */
__TDEF(float, FLOAT32);            /* float types */
__TDEF(double, FLOAT64);
__TDEF(long double, FLOAT80);

#elif defined(__OS_WIN31__)

__TDEF(char, INT8);                /* signed ints */
__TDEF(short, INT16);
__TDEF(long, INT32);
__TDEF(unsigned char, UINT8);      /* unsigned ints */
__TDEF(unsigned short, UINT16);
__TDEF(unsigned long, UINT32);
__TDEF(char, CHAR8);               /* char */
__TDEF(unsigned char, UCHAR8);     /* unsigned char */
__TDEF(float, FLOAT32);            /* float types */
__TDEF(double, FLOAT64);
__TDEF(long double, FLOAT80);

#elif defined(__OS_WIN32__)

__TDEF(char, INT8);
__TDEF(short, INT16);
__TDEF(long, INT32);
__TDEF(unsigned char, UINT8);
__TDEF(unsigned short, UINT16);
__TDEF(unsigned long, UINT32);
__TDEF(char, CHAR8);
__TDEF(unsigned char, UCHAR8);
__TDEF(float, FLOAT32);
__TDEF(double, FLOAT64);
__TDEF(long double, FLOAT80);

#elif defined(__OS_WIN64__)

__TDEF(char, INT8);
__TDEF(short, INT16);
__TDEF(int, INT32);
__TDEF(unsigned char, UINT8);
__TDEF(unsigned short, UINT16);
__TDEF(unsigned int, UINT32);
__TDEF(char, CHAR8);
__TDEF(unsigned char, UCHAR8);
__TDEF(float, FLOAT32);
__TDEF(double, FLOAT64);
__TDEF(long double, FLOAT80);


#elif defined(__OS_LINUX__)

__TDEF(char, INT8);
__TDEF(short, INT16);
#ifdef __BITS_64__
  __TDEF(int, INT32);
#else
  __TDEF(long, INT32);
#endif
__TDEF(unsigned char, UINT8);
__TDEF(unsigned short, UINT16);
#ifdef __BITS_64__
  __TDEF(unsigned int, UINT32);
#else
  __TDEF(unsigned long, UINT32);
#endif
__TDEF(char, CHAR8);
__TDEF(unsigned char, UCHAR8);
__TDEF(float, FLOAT32);
__TDEF(double, FLOAT64);
__TDEF(long double, FLOAT80);

#elif defined(__OS_UNIX__)

__TDEF(char, INT8);
__TDEF(short, INT16);
#ifdef __BITS_64__
  __TDEF(int, INT32);
#else
  __TDEF(long, INT32);
#endif
__TDEF(unsigned char, UINT8);
__TDEF(unsigned short, UINT16);
#ifdef __BITS_64__
  __TDEF(unsigned int, UINT32);
#else
  __TDEF(unsigned long, UINT32);
#endif
__TDEF(char, CHAR8);
__TDEF(unsigned char, UCHAR8);
__TDEF(float, FLOAT32);
__TDEF(double, FLOAT64);
__TDEF(long double, FLOAT80);

#else
#error plataforma no configurada
#endif


/* otros nombres para los valores de longitud fija */

__TDEF(UINT8, BYTE);
__TDEF(UINT8, WORD8);
__TDEF(UINT16, WORD16);
__TDEF(UINT32, WORD32);


/**********************************************************/
/* tipos de longitud determinada (en octetos) */

__TDEF(INT8, INT1);
__TDEF(INT16, INT2);
__TDEF(INT32, INT4);
__TDEF(WORD8, WORD1);
__TDEF(WORD16, WORD2);
__TDEF(WORD32, WORD4);
__TDEF(UINT8, UINT1);
__TDEF(UINT16, UINT2);
__TDEF(UINT32, UINT4);
__TDEF(CHAR8, CHAR1);
__TDEF(UCHAR8, UCHAR1);
__TDEF(FLOAT32, FLOAT4);
__TDEF(FLOAT64, FLOAT8);
__TDEF(FLOAT80, FLOAT10);

/**********************************************************/
/* definiciones para gestionar el scope y manejo de los simbolos */

#ifdef __OS_WIN31__   /* windows 3.1 */

#include <windows.h>

#define PUBLIC             /*exportable y utilizable por otros modulos*/
#define PRIVATE static     /* uso interno al modulo */
#define EXTERN extern      /* referencia a simbolo externo */
#define STATIC static      /* datos estaticos de una funcion */
#define VOLATILE volatile  /* datos compartidos (no asumir nada) */
#define API   FAR PASCAL    /* prototipo exportable */
#define XAPI  FAR PASCAL _export  /* definicion exportable */

#else   /* resto de plataformas */

#define PUBLIC             /* exportable y utilizable por otros modulos */
#define PRIVATE static     /* uso interno al modulo */
#define EXTERN extern      /* referencia a simbolo externo */
#define STATIC static      /* datos estaticos de una funcion */
#define VOLATILE volatile  /* datos compartidos (no asumir nada) */
#define API                /* prototipo exportable */
#define XAPI               /* definicion exportable */

#endif

/**********************************************************/
/* Definicion de MINALIGN para alineamientos minimo de campos
en estructuras o empaquetado (se pone al final de cada campo) */

#ifdef __CC_GNUC__
#define MINALIGN  __attribute__ ((packed))
#else
#define MINALIGN
#endif

/* Ademas (y por desgracia) hay que meter este churro
antes de comenzar con las estructuras a empaquetar
(sustituir ???? en ____tmp_????_align_test___
por el nombre del fichero en el que se haga la
comprobacion):

#ifdef __CC_MSVC__
#pragma pack(push,1)
#elif __CC_BORLANDC__
typedef struct {CHAR8 x; INT2 y; } ____tmp_????_align_test___;
#if sizeof(____tmp_????_align_test___)>3
#error You must disable Word Alignment
#endif
#endif

Y este otro despues de las estructuras a empaquetar:

#ifdef __CC_MSVC__
#pragma pack(pop)
#endif

*/

/**********************************************************/

#endif

