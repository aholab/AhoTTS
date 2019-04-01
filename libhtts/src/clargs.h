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
#ifndef __CLARGS_H
#define __CLARGS_H

/**********************************************************/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/*
Copyright: 1993 - Grupo de Voz (DAET) ETSII/IT-Bilbao

Nombre fuente................ CLARGS.H
Nombre paquete............... -
Lenguaje fuente.............. C (Borland C/C++ 3.1)
Estado....................... Utilizable
Dependencia Hard/OS.......... NO
Codigo condicional........... NO

Codificacion................. Borja Etxebarria

Version  dd/mm/aa  Autor     Comentario
-------  --------  --------  ----------
1.1.0    20/05/97  Borja     arg {allow_unknown} en clasw_getsw()
1.0.0    11/01/93  Borja     Codificacion inicial.

======================== Contenido ========================
Fichero de cabecera de CLARGS.C

Gestion de argumentos de linea.
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include <stdio.h>

/**********************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************/

#define CLARGS_MAX_LEN 256    /* Maxima longitud de un argumento
                                  (incluido el 0 final) */

#define CLARGS_CFGFILE  "CfgFile"  /* switch de fichero de configuracion */
#define CLARGS_ARGFILE  "ArgFile"  /* switch de fichero de argumentos */

/**********************************************************/
/* 1 para mayusculas!=minusculas.
0 no distingue  (0 es el valor por defecto) */

extern int _clargs_case_sensitive;

/**********************************************************/

typedef struct {
    char args[CLARGS_MAX_LEN];  /* buffer para meter los argumentos */
    int argc;  /* argc-1 de main() */
    char * * argv;  /* argv+1[] de main() */

    int lastarg;  /* ultimo argumento de linea procesado */

    char * cfgfn;  /* nombre del fichero de configuracion a utilizar */
    char * exefn;  /* _cfgfn, pero en el directorio del .exe, o NULL
                       si _cfgfn no es el indicado por defecto */

    FILE * cfgf;  /* fichero de configuracion */
    FILE * argf;  /* fichero de argumentos */
    } clargs;

typedef struct {
  clargs * c;
	const char * * switches;  /* array de switches validos para getsw() */
  int nswitches;  /* numero de switches */
  } clasw;

/**********************************************************/

clargs * clargs_construct( int argc, char * argv[], const char * def_cfgfn );
void clargs_destruct( clargs * c );
void clargs_restart( clargs * c );
char * clargs_getarg( clargs * c );
char * clargs_getsarg( clargs * c );
char * clargs_getparg( clargs * c );

/**********************************************************/

clasw * clasw_construct( int argc, char * argv[], const char * sw[],
													const char * def_cfgfn );
void clasw_destruct( clasw * c );
int clasw_getsw( clasw * c, char  ** value, int allow_unknown );
void clasw_restart( clasw * c );
char * clasw_getarg( clasw * c );
char * clasw_getsarg( clasw * c );
char * clasw_getparg( clasw * c );

/**********************************************************/

#ifdef __cplusplus
}  /* extern "C" */
#endif

/**********************************************************/

#endif
