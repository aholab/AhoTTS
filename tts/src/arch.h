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
#ifndef __ARCH_H
#define __ARCH_H

/**********************************************************/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/*
Copyright: 1996 - Grupo de Voz (DAET) ETSII/IT-Bilbao

Nombre fuente................ ARCH.H
Nombre paquete............... -
Lenguaje fuente.............. C
Estado....................... en desarrollo
Dependencia Hard/OS.......... Si!! (define el SO / compilador)
Codigo condicional........... -

Codificacion................. Borja Etxebarria
............................. Yon2 - Juan Luis Murugarren

Version  dd/mm/aa  Autor     Comentario
-------  --------  --------  ----------
0.0.3    20/01/11  ikerl     detección win64 y linux64
0.0.2    06/19/99  Yon2      deteccion DJGPP
0.0.2    20/01/97  Borja     deteccion win32 y VisualC++ (German)
0.0.1    18/11/96  Borja     modificada deteccion linux/gcc
0.0.0    22/07/96  Borja     codificacion inicial

======================== Contenido ========================
<DOC>
Definicion de la plataforma utilizada (OS)
------------------------------------------
MSDOS: Se define __OS_MSDOS__ con valor 1. Si es posible se
define con valor igual a la version del DOS (ej. 62 para la 6.2).

Windows: Se define __OS_WINDOWS__ con valor 1. Si es posible se
define con valor igual a la version del Windows (31 para el win 3.1,
311 para el win 3.11TG, 32 para el Win32S, 95 para el Win95).
Tambien se definen __OS_WIN31__ para win3.1, __OS_WIN32__ para
Win32 y Win95, __OS_WIN64__ para versiones de Win a 64 bits
En Windows *no* se define __OS_MSDOS__.

Unix: Se define __OS_UNIX__

Linux: Se define __OS_LINUX__ (y tambien __OS_UNIX__)


Definicion del compilador utilizado  (CC)
-----------------------------------------
Se define  __CC_BORLANDC__ con valor 1 para el Borland. Si es
posible con el valor igual a la version del Borland.

Se define __CC_GNUC__ con valor 1 para el compilador gcc de GNU.

Se define __CC_MSVC__ con el valor de la version para el compilador
Microsoft Visual C++.

Si se compila en C++ tambien se define  __CC_CPLUSPLUS__

Definicion de tipo de arquitectura BIG/LITTLE endian
----------------------------------------------------
Se define __LITTLE_ENDIAN__ o __BIG_ENDIAN__ segun sea
el caso (intel=little, sun=big).

Definicion de arquitectura a 32 bits y 64 bits
----------------------------------------------
Se define __BITS_16__, __BITS_32__ o __BITS_64__ segun sea el caso

</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

/**********************************************************/
/* OS resolution */

#if _WIN32
#define __OS_WINDOWS__  95
#define __OS_WIN32__
#define __LITTLE_ENDIAN__ 1
#define __BITS_32__
// detect win32 console apps.
  #ifdef _CONSOLE
  #define __OS_WIN32_CONSOLE__
  #endif

#elif _WIN64
#define __OS_WINDOWS__
#define __OS_WIN64__
#define __LITTLE_ENDIAN__ 1
#define __BITS_64__
// detect win64 console apps.
  #ifdef _CONSOLE
  #define __OS_WIN64_CONSOLE__
  #endif

#elif _Windows
#define __OS_WINDOWS__  31
#define __OS_WIN31__
#define __LITTLE_ENDIAN__ 1
#define __BITS_16__

#elif __MSDOS__
#define __OS_MSDOS__  62
#define __LITTLE_ENDIAN__ 1
#define __BITS_16__

#elif __linux__
#define __OS_LINUX__  20
#define __OS_UNIX__   1
#define __LITTLE_ENDIAN__ 1
  #ifdef __x86_64
  #define __BITS_64__
  #else
  #define __BITS_32__
  #endif

#elif Linux
#define __OS_LINUX__  20
#define __OS_UNIX__   1
#define __LITTLE_ENDIAN__ 1
  #ifdef __x86_64
  #define __BITS_64__
  #else
  #define __BITS_32__
  #endif

#elif sun
#define __OS_SUNOS__  20
#define __OS_UNIX__   1
#define __BIG_ENDIAN__ 1
#define __BITS_32__

#elif SunOS
#define __OS_SUNOS__  20
#define __OS_UNIX__   1
#define __BIG_ENDIAN__ 1
#define __BITS_32__

#else
#error No OS resolved!
#endif

/**********************************************************/
/* Compiler detection */

#if __BORLANDC__
#define __CC_BORLANDC__  31

#elif __GNUC__
#define __CC_GNUC__  28

#elif _MSC_VER
#define __CC_MSVC__ _MSC_VER

#else
#error No CC resolved!
#endif


/* Modificaciones para el DJGPP (bajo MSDOS) */
#if  (defined(__OS_MSDOS__)&&defined(__CC_GNUC__))
#define __CC_DJGPP__ 28
#undef __OS_MSDOS__
#define __OS_MSDOS32__
#define  __OS_UNIX__  1
#endif


/**********************************************************/
/* C++ */

#ifdef __cplusplus
#define __CC_CPLUSPLUS__  1
#endif

/**********************************************************/
/* endian verification */

#if defined __LITTLE_ENDIAN__
#ifdef __BIG_ENDIAN__
#error Only one of __LITTLE_ENDIAN__ or __BIG_ENDIAN__ must be defined
#endif

#elif defined __BIG_ENDIAN__
#ifdef __LITTLE_ENDIAN__
#error Only one of __LITTLE_ENDIAN__ or __BIG_ENDIAN__ must be defined
#endif

#else
#error One of __LITTLE_ENDIAN__ or __BIG_ENDIAN__ must be defined
#endif


/**********************************************************/
/* bit verification */

#if defined __BITS_16__
#if defined (__BITS_32__) || defined (__BITS_64__)
#error Only one of __BITS_16__, __BITS_32__ or __BITS_64__ must be defined
#endif

#elif defined __BITS_32__
#if defined (__BITS_16__) || defined (__BITS_64__)
#error Only one of __BITS_16__, __BITS_32__ or __BITS_64__ must be defined
#endif

#elif defined __BITS_64__
#if defined (__BITS_32__) || defined (__BITS_16__)
#error Only one of __BITS_16__, __BITS_32__ or __BITS_64__ must be defined
#endif

#else
#error One of __BITS_16__, __BITS_32__ or __BITS_64__ must be defined
#endif

/**********************************************************/

#endif

