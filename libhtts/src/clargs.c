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

Nombre fuente................ CLARGS.C
Nombre paquete............... -
Lenguaje fuente.............. C (Borland C/C++ 3.1)
Estado....................... Utilizable
Dependencia Hard/OS.......... NO
Codigo condicional........... NO

Codificacion................. Borja Etxebarria

Vers.   DD/MM/AA   Autor     Proposito de la edicion
-----   --------   -------   -----------------------
1.1.1    2012	   Inaki     eliminar dependencia de xalloc
1.1.0    20/05/97  Borja     arg {allow_unknown} en clasw_getsw()
1.0.2    22/04/97  Borja     quitar warnings g++
1.0.1    30/07/96  Borja     usar xdie() para errores
1.0.0    11/01/93  Borja     Codificacion inicial.

======================== Contenido ========================
Gestion de argumentos de linea.
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "clargs.h"
#include "uti.h"

/**********************************************************/

PRIVATE const char * _file_switches[3] = {
  CLARGS_CFGFILE,  /* fichero de configuracion */
  CLARGS_ARGFILE,  /* fichero de argumentos */
  NULL
  };

int _clargs_case_sensitive = 0;

/**********************************************************/
/* cierra fichero de configuracion si se puede o si ya esta
cerrado. Mensaje de error si no se puede */

void clargs_close_cfgf( clargs * c )
{
if (c->cfgf!=NULL)
	cdie_beep(fclose(c->cfgf) == EOF,"clargs: error closing config file");

c->cfgf = NULL;
}

/**********************************************************/
/* cierra fichero de argumentos si se puede o si ya esta
cerrado. Mensaje de error si no se puede */

void clargs_close_argf( clargs * c )
{
if (c->argf!=NULL) {
	cdie_beep(fclose(c->argf) == EOF,"clargs: error closing args file");
	}
c->argf = NULL;
}

/**********************************************************/
/* Termina la lectura actual de argumentos (cierra
ficheros), cuando clargs_read() llega a devolver NULL, o si se
llama de nuevo a clargs_restart(), o al destruir clargs. */

void clargs_close_f( clargs * c )
{
clargs_close_argf(c);
clargs_close_cfgf(c);
}

/**********************************************************/
/* abre el fichero de configuracion cfgfn si no es NULL.
Si no existe y si exefn==NULL, mensaje de error y al SO
Si exefn!=NULL no pasa nada, e intenta buscarlo ahi, en el directorio
en el que esta el programa ejecutable, y tampoco pasa nada si no lo
encuentra ahi. */

void clargs_open_cfgf( clargs * c )
{
	if (c->cfgfn!=NULL) {
		if ((c->cfgf=fopen(c->cfgfn,"r")) == NULL) {
			cdie_beep(c->exefn==NULL,"clargs: error opening config file %s",c->cfgfn);
			c->cfgf=fopen(c->exefn,"r");
		}
	}
}

/**********************************************************/
/* abre el fichero de argumentos argfn. Si no existe, mensaje
de error y al SO.
Si ya habia un fichero abierto, mensaje y al SO */

void clargs_open_argf( clargs * c, char * argfn )
{
	cdie_beep(c->argf!=NULL,"clargs: error, can't manage recursive args file");

	cdie_beep((c->argf=fopen(argfn,"r")) == NULL,"clargs: error opening args file %s",argfn);
}

/**********************************************************/
/* busca entre los argumentos de linea si se encuentra
un comando para un fichero de configuracion.
Si no hay ninguno, [devuelve] NULL. Si encuentra uno o m s, se queda con
el ultimo y [devuelve] un puntero al nombre del fichero de configuracion.
No enviar argv[0], enviar a partir de argv[1]. */

char * clargs_find_cfgfn( clargs * c, int argc, char * argv[] )
{
int i;
char * retval = NULL;

(void)c;  /* eliminar "warning arg not used" */
for (i=0; i<argc; i++) {
	if (str_isswitch(argv[i])) {
		if (label_match(_file_switches,1,argv[i]+1,_clargs_case_sensitive,0)==0)
			retval = argv[i];
		}
	}
if (retval!=NULL)
	return str_assignptrnext(retval);
else
	return NULL;
}

/**********************************************************/
/* genera el nombre del fichero de configuracion por defecto
con el directorio del programa ejecutable. [devuelve] una
cadena creada con malloc().
exen=argv[0].
cfgfn=nombre de fichero de conf. por defecto. Solo el fichero, no
puede tener path ni drive.
[devuelve] NULL si por cualquier causa no se puede hacer */

char * clargs_exe_cfgfn( clargs * c, const char * exen, const char * cfgfn )
{
	(void)c;  /* eliminar "warning arg not used" */
	return path_src2destp(exen,cfgfn,"");
}

/**********************************************************/
/* [devuelve] un puntero a una cadena leida del fichero de configuracion,
o NULL si no hay mas */

char * clargs_readcfgf( clargs * c )
{
char * retval;

clearerr(c->cfgf);
retval = fgetlnf(c->args,CLARGS_MAX_LEN,c->cfgf);
	cdie_beep(ferror(c->cfgf),"clargs: error reading config file");
return retval;
}

/**********************************************************/
/* [devuelve] un puntero a una cadena leida de un fichero de
argumentos, o NULL si no hay mas */

char * clargs_readargf( clargs * c )
{
char * retval;

clearerr(c->argf);
retval = fgetlnf(c->args,CLARGS_MAX_LEN,c->argf);
	cdie_beep(ferror(c->argf),"clargs: error reading args file");
return retval;
}

/**********************************************************/
/* [devuelve] un puntero a una cadena leida de la linea de
comandos, o NULL si no hay mas */

char * clargs_readcl( clargs * c )
{
if ((++(c->lastarg)) >= (c->argc))
	return NULL;
else
	return (strcpy(c->args,c->argv[c->lastarg]));
}

/**********************************************************/
/* [devuelve] un puntero a la siguiente cadena, leida de donde
corresponda, o NULL si no hay mas */

char * clargs_read_a( clargs * c )
{
char * retval;

if (c->argf!=NULL) {
	retval = clargs_readargf(c);
	if (retval==NULL)
		clargs_close_argf(c);
	else
		return retval;
	}
if (c->cfgf!=NULL) {
	retval = clargs_readcfgf(c);
	if (retval==NULL)
		clargs_close_cfgf(c);
	else
		return retval;
	}

retval = clargs_readcl(c);
if (retval==NULL)
	clargs_close_f(c);

return retval;
}

/**********************************************************/
/* Construye el lector de argumentos de linea. [argc] y [argv] son
los argumentos de main(), (incluido el argv[0] !). [def_cfgfn] es
el nombre de un fichero de configuracion por defecto. El switch a main
"/CfgFile=<fichero>" se trata de manera especial: se procesa <fichero>
como lista de argumentos de configuracion, con preferencia sobre
cualquier otro argumento a main. Si hay varios /CfgFile solo se
procesa el ultimo. Si no hay ningun /CfgFile se utiliza def_cfgfn
(si es distinto de NULL). Ademas [def_cfgfn] se busca primero en el
directorio actual y si no esta ahi, en el del programa ejecutable.
Tambies se procesan los switches "/ArgFile=<fichero>". Se procesa
<fichero> como lista de argumentos cuando le toque.
Puede haber /ArgFile dentro de un fichero /CfgFile, pero no al reves
(se ignorar n).
Ver fgetlncss() en SYST.C para informacion acerca del procesado de los
ficheros */

clargs * clargs_construct( int argc, char * argv[], const char * def_cfgfn )
{
clargs * c = NULL;

c = (clargs *)malloc(sizeof(clargs));
cdie_beep(c==NULL,"clargs: error constructing command line parser");

c->argc = argc-1;
c->argv = argv+1;
c->lastarg = -1;

c->exefn = NULL;
c->cfgfn = clargs_find_cfgfn(c,c->argc,c->argv);
if (c->cfgfn!=NULL)
  c->cfgfn = strdup(c->cfgfn);
else if (def_cfgfn!=NULL) {
  c->cfgfn = strdup(def_cfgfn);
  c->exefn = clargs_exe_cfgfn(c,argv[0],def_cfgfn);
  }

c->cfgf = NULL;
c->argf = NULL;

clargs_restart(c);
return c;
}

/**********************************************************/
/* Termina la lectura de argumentos,borra todas las variables
dinamicas y cierra ficheros */

void clargs_destruct( clargs * c )
{
clargs_close_f(c);

if (c->cfgfn!=NULL)
  free(c->cfgfn);
if (c->exefn!=NULL)
  free(c->exefn);

if (c)
  free(c);
}

/**********************************************************/
/* Reinicializa la lectura de argumentos, comenzando de nuevo
desde el primero */

void clargs_restart( clargs * c )
{
clargs_close_f(c);
clargs_open_cfgf(c);
c->lastarg = (-1);
}

/**********************************************************/
/* [devuelve] un puntero al siguiente argumento (switch o no),
o NULL si no hay mas */

char * clargs_getarg( clargs * c )
{
char * retval;

while (1) {
  retval=clargs_read_a(c);
  if (retval==NULL)
    break;

  if (str_isswitch(retval)) {
    int i = label_match(_file_switches,2,retval+1,_clargs_case_sensitive,0);
    if (i==1)
	  clargs_open_argf(c,str_assignptrnext(retval));
    else if (i<0)
      break;
    }
  else
    break;
  }
return retval;
}

/**********************************************************/
/* [devuelve] un puntero al siguiente argumento que sea switch,
o NULL si no hay mas */

char * clargs_getsarg( clargs * c )
{
char * retval;

while (1) {
  retval=clargs_getarg(c);
  if ((retval==NULL)||(str_isswitch(retval)))
    break;
  }
return retval;
}

/**********************************************************/
/* [devuelve] un puntero al siguiente argumento que sea parametro
no-switch, o NULL si no hay mas */

char * clargs_getparg( clargs * c )
{
char * retval;

while (1) {
  retval=clargs_getarg(c);
  if ((retval==NULL)||(!(str_isswitch(retval))))
    break;
  }
return retval;
}

/**********************************************************/
/* Como clargs, pero recibe ademas array en [sw] de switches validos
para getsw(). Este array ha de terminar con puntero NULL.
Los switches del array NO han de llevar el caracter '-' o '/' */

clasw * clasw_construct( int argc, char * argv[], const char * sw[],
		const char * def_cfgfn )
{
clasw * c;

c = (clasw *)malloc(sizeof(clasw));
cdie_beep(c==NULL,"clasw: error constructing command line parser");

c->c=clargs_construct(argc,argv,def_cfgfn);
c->switches=sw;
c->nswitches=nptrs((const void * *)sw);

return c;
}

/**********************************************************/
/* Termina la lectura de argumentos,borra todas las variables
dinamicas y cierra ficheros */

void clasw_destruct( clasw * c )
{
clargs_destruct(c->c);
free(c);
}

/**********************************************************/
/* [devuelve] el indice del siguiente switch leido. El indice
indica el elemento del array sw[] pasado al constructor.
[devuelve] -1 y value=NULL si no hay mas switches.
Si se lee un switch que no este en sw[], mensaje de error
y al SO (si allow_unknown==FALSE) o {devuelve} -2 si
allow_unknown==TRUE.
En [value] devuelve un puntero a la cadena de VALOR del switch */

int clasw_getsw( clasw * c, char * * value, int allow_unknown )
{
int retval;

*value = clargs_getsarg(c->c);
if (*value!=NULL) {
	(*value)++;
	retval = label_match( c->switches,c->nswitches,*value,
			_clargs_case_sensitive,1);
	cdie_beep(retval==-2,"clasw: ambiguous label (%s)",*value);
	if (retval==-1) {
		cdie_beep(!allow_unknown,"error: unknown label (%s)",*value);
		retval=-2;
	}
	else
		*value = str_assignptrnext(*value);
	}
else
	retval = -1;

return retval;
}

/**********************************************************/

void clasw_restart( clasw * c )
{
clargs_restart(c->c);
}

/**********************************************************/

char * clasw_getarg( clasw * c )
{
return clargs_getarg(c->c);
}

/**********************************************************/

char * clasw_getsarg( clasw * c )
{
return clargs_getsarg(c->c);
}

/**********************************************************/

char * clasw_getparg( clasw * c )
{
return clargs_getparg(c->c);
}

/**********************************************************/
