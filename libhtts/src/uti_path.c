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

Nombre fuente................ UTI_PATH.C
Nombre paquete............... UTI.H
Lenguaje fuente.............. C (BC31,GCC)
Estado....................... Utilizable
Dependencia Hard/OS.......... -
Codigo condicional........... NO

Codificacion................. Borja Etxebarria

Version  dd/mm/aa  Autor     Comentario
-------  --------  --------  ----------
2.0.1    2012	   Inaki     eliminar dependencia de xalloc
2.0.0    01/05/97  Borja     mix de xfiles, syst, xsyst, fname, vstr...
1.1.0    01/08/96  Borja     make_dest2()
1.0.0    11/01/93  Borja     Codificacion inicial.

======================== Contenido ========================
Gestion de nombres de ficheros y ficheros temporales.
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "uti.h"

/**********************************************************/
/* {devuelve} un puntero a una estructura con informacion para
gestionar un nombre de fichero.
En {dest} se envia el nombre final que tendra el fichero, y en
{tmpfile} un nombre temporal. Si se envia NULL en {tmpfile} no
se utiliza el nombre temporal, y si {dest} ya existe, se
renombra a .BAK (extension definida en PATH_EXT_BAK) */

fntmp * fntmp_construct( const char * dest, const char * tmpfile )
{
	fntmp * f;
	f = (fntmp *)malloc(sizeof(fntmp));
	f->name = strdup(dest);
	if (tmpfile!=NULL)
		f->tmp = path_src2destp(dest,tmpfile,NULL);
	else {
		f->tmp = f->name;
		frename_bak(f->tmp,f->tmp); /* renombrar a .bak */
	}
	return f;
}

/**********************************************************/
/* [devuelve] el nombre (temporal o definitivo) que debe tener
el fichero */

const char * fntmp_getname( fntmp * f )
{
	return f->tmp;
}

/**********************************************************/
/* destruye la estructura fntmp {f}, pero antes renombra el
fichero temporal al nombre definitivo, teniendo en cuenta que
si ya existe el fichero destino, lo convierte en un .BAK */

void fntmp_destruct( fntmp * f )
{
	if (f->tmp!=f->name) {
		xfrename_bak(f->tmp,f->name);
		free(f->tmp);
	}
	free(f->name);
	free(f);
}

/**********************************************************/
/* {devuelve} la posicion del primer caracter del nombre de un
fichero, saltandose el directorio. */

size_t path_namepos( const char * path )
{
	size_t l;
	l = strlen(path);
	while ((l)&&(path[l]!=PATH_DIRSEP)&&(path[l]!=PATH_DRIVESEP))
		l--;
	if ((path[l]==PATH_DIRSEP)||(path[l]==PATH_DRIVESEP))
		return(l+1);
	else
		return(l);
}

/**********************************************************/
/* {devuelve} la posicion de la extension del nombre de un fichero */

size_t path_extpos( const char * path )
{
	size_t l;
	l = strlen(path);
	while ((l)&&(path[l]!='.')&&(path[l]!=PATH_DIRSEP))
		l--;
	if (path[l]=='.')
		return(l);
	else
		return(strlen(path));
}

/**********************************************************/
/* cambia el directorio del nombre de fichero {path}, por el
nuevo directorio especificado en {new_dir}. {devuelve} un puntero
a {path}. {path} debe tener sitio suficiente */

char * path_dirchange( char * path, const char * new_dir )
{
	char * tmp;
	tmp=strdup(&(path[path_namepos(path)]));
	strcpy(path,new_dir);
	strcat(path,tmp);
	free(tmp);
	return(path);
}

/**********************************************************/
/* cambia el nombre del fichero {path}, por el nuevo nombre
especificado en {new_name} (preserva el directorio y la extension).
{devuelve} un puntero a {path}. {path} debe tener sitio suficiente */

char * path_namechange( char * path, const char * new_name)
{
	char * ex;
	ex=strdup(&(path[path_extpos(path)]));
	path[path_namepos(path)]=0;
	strcat(path,new_name);
	strcat(path,ex);
	free(ex);
	return(path);
}

/**********************************************************/
/* cambia la extension del nombre de fichero path, por la
nueva extension especificada en new_ext. Devuelve un puntero a path.
path debe tener sitio suficiente */

char * path_extchange( char * path, const char * new_ext )
{
	path[path_extpos(path)]=0;
	strcat(path,new_ext);
	return(path);
}

/**********************************************************/
/* renombra el fichero en disco identificado por el nombre name,
con el nuevo nombre new_name, y si el fichero ese ya existia
lo borra. Devuelve el codigo de la funcion rename() */

int frename_del( const char * name, const char * new_name )
{
	remove(new_name);
	return rename(name,new_name);
}

/**********************************************************/
/* como frename_del, pero sale al SO en caso de error */

void xfrename_del( const char * name, const char * new_name )
{
	remove(new_name);
	xrename(name,new_name);
}

/**********************************************************/
/* renombra el fichero en disco identificado por el nombre name,
cambiando su extension por new_ext, y si el fichero ese ya existia
lo borra. Devuelve el codigo de la funcion rename() */

int frename_ext_del( const char * name, const char * new_ext )
{
	int retval;
	char * tmp;
	tmp=(char *)malloc(strlen(name)+strlen(new_ext)+1);
	strcpy(tmp,name);
	path_extchange(tmp,new_ext);
	retval = frename_del(name,tmp);
	free(tmp);
	return retval;
}

/**********************************************************/
/* como frename_ext_del pero en caso de error, mensaje y al S.O */

void xfrename_ext_del( const char * name, const char * new_ext )
{
	char * tmp;
	tmp=(char *)malloc(strlen(name)+strlen(new_ext)+1);
	strcpy(tmp,name);
	path_extchange(tmp,new_ext);
	xfrename_del(name,tmp);
	free(tmp);
}

/**********************************************************/
/* renombra el fichero en disco identificado por el nombre name,
con el nuevo nombre new_name, y si el fichero ese ya existia
lo renombra a .BAK (valor del define EXT_BAK).
Devuelve el codigo de la funcion rename() */

int frename_bak( const char * name, const char * new_name )
{
	frename_ext_del(new_name,PATH_EXT_BAK);
	return rename(name,new_name);
}

/**********************************************************/
/* como frename_bak, pero sale al SO en caso de error */

void xfrename_bak( const char * name, const char * new_name )
{
	frename_ext_del(new_name,PATH_EXT_BAK);
	xrename(name,new_name);
}

/**********************************************************/
/* como frename_ext_del, pero si el fichero destino ya existe lo
renombra a .BAK */

int frename_ext_bak( const char * name, const char * new_ext )
{
	int retval;
	char * tmp;
	tmp=(char *)malloc(strlen(name)+strlen(new_ext)+1);
	strcpy(tmp,name);
	path_extchange(tmp,new_ext);
	retval = frename_bak(name,tmp);
	free(tmp);
	return retval;
}

/**********************************************************/
/* como frename_ext_bak, pero en caso de error, mensaje y al S.O */

void xfrename_ext_bak( const char * name, const char * new_ext )
{
	char * tmp;
	tmp=(char *)malloc(strlen(name)+strlen(new_ext)+1);
	strcpy(tmp,name);
	path_extchange(tmp,new_ext);
	xfrename_bak(name,tmp);
	free(tmp);
}

/*<DOC> */
/**********************************************************/
/* {source} es el nombre de un fichero, {dest} el nombre del fichero
destino, {ddir} y {dext} son el directorio y la extension por defecto.
La funcion {devuelve} un puntero a una cadena con el nombre definitivo del
fichero destino, (se hace malloc(), por lo que el usuario debe hacer
free() de esta cadena).
Si {ddir} esta vacio, se usa el directorio de {source} como {ddir}.
Si {dext} esta vacia, se usa la extension de {source} como {dext}.

Suponiendo que dest esta formado por DIR+NAME+EXT, se devuelve el nombre
definitivo a partir de {dest}, siguiendo las reglas:

	Si DIR esta vacio, se usa {ddir}.
	Si NAME esta vacio, se usa el nombre de fichero de {source}
	Si EXT esta vacia, se usa {dext}

Se considera que un parametro esta vacio si vale NULL o "".
Si no queda mas remedio que utilizar algun campo vacio, se usa "" */

char * path_src2destde( const char * source, const char * dest,
		const char * ddir, const char * dext )
/*</DOC> */
{
#define LEN(str)  ((str)?strlen(str):0)
#define LLENO(str) (LEN(str))
#define STRNCPY(dest,source,n) { \
	strncpy(dest,source,n); \
	dest[n] = 0; \
	}
#define STRNCAT(dest,source,n) { \
	dest[strlen(dest)+1]=0; \
	strncat(dest,source,n); \
	}

	size_t sl, sn,se, dl, dn,de;
	char * tmp = (char *)malloc(LEN(source)+LEN(dest)+LEN(ddir)+LEN(dext)+4);

	sn=se=dn=de=0;
	if ((dl=LEN(dest))!=0) {
		dn=path_namepos(dest);
		de=path_extpos(dest);
	}
	if ((sl=LEN(source))!=0) {
		sn=path_namepos(source);
		se=path_extpos(source);
	}

	if (dn) STRNCPY(tmp,dest,dn)
	else if LLENO(ddir) strcpy(tmp,ddir);
	else if (sn) STRNCPY(tmp,source,sn)
	else *tmp=0;

	if (de-dn) STRNCAT(tmp,dest+dn,de-dn)
	else if (se-sn) STRNCAT(tmp,source+sn,se-sn);

	if (dl-de) strcat(tmp,dest+de);
	else if LLENO(dext) strcat(tmp,dext);
	else if (sl-se) strcat(tmp,source+se);

	return tmp;
#undef LEN
#undef LLENO
#undef STRNCPY
#undef STRNCAT
}

/*<DOC> */
/**********************************************************/
/* {source} es el nombre de un fichero, {dest} el nombre del fichero
destino, {dpath} contiene campos por defecto (directorio, extension,
incluso nombre).
La funcion {devuelve} un puntero a una cadena con el nombre definitivo del
fichero destino, (se hace malloc(), por lo que el usuario debe hacer
free() de esta cadena).
Si {dpath} tiene el campo directorio vacio, se usa el directorio de
{source} como directorio por defecto.
Si {dpath} tiene el campo nombre vacio, se usa el nombre de
{source} como nombre por defecto.
Si {dpath} tiene el campo extension vacio, se usa la extension de
{source} como extension por defecto.

Suponiendo que {dest} esta formado por DIR+NAME+EXT, se devuelve el path
definitivo a partir de {dest}, siguiendo las reglas:

	Si DIR esta vacio, se usa dir. por defecto.
	Si NAME esta vacio, se usa el nombre por defecto.
	Si EXT esta vacia, se usa ext. por defecto

Se considera que un parametro esta vacio si vale NULL o "".
Si no queda mas remedio que utilizar algun campo vacio, se usa ""

Resumen: Los campos de {dest} tienen prioridad, luego viene {dpath}
y por ultimo {source}, y si no, "".

La funcion path_applyrule(src,dest) es una macro que llama a
path_src2destp() con un path por defecto (dpath) nulo. */

char * path_src2destp( const char * source, const char * dest,
		const char *dpath )
/*</DOC> */
{
#define LEN(str)  ((str)?strlen(str):0)
#define LLENO(str) (LEN(str))
#define STRNCPY(dest,source,n) { \
	strncpy(dest,source,n); \
	dest[n] = 0; \
  }
#define STRNCAT(dest,source,n) { \
  dest[strlen(dest)+1]=0; \
	strncat(dest,source,n); \
  }
	size_t sl,sn,se, dl,dn,de, ddl,ddn,dde;
	//fprintf(stderr,"len_source=%d\tlen_dest=%d\tlen_dpath=%d\n", LEN(source),LEN(dest),LEN(dpath));
	char * tmp = (char *)malloc(LEN(source)+LEN(dest)+LEN(dpath)+4);


	sn=se=dn=de=ddn=dde=0;
	if ((dl=LEN(dest))!=0) {
		dn=path_namepos(dest);
		de=path_extpos(dest);
	}
	if ((sl=LEN(source))!=0) {
		sn=path_namepos(source);
		se=path_extpos(source);
	}
	if ((ddl=LEN(dpath))!=0) {
		ddn=path_namepos(dpath);
		dde=path_extpos(dpath);
	}

	if (dn) STRNCPY(tmp,dest,dn)
	else if (ddn) STRNCPY(tmp,dpath,ddn)
	else if (sn) STRNCPY(tmp,source,sn)
	else *tmp=0;

	if (de-dn) STRNCAT(tmp,dest+dn,de-dn)
	else if (dde-ddn) STRNCAT(tmp,dpath+ddn,dde-ddn)
	else if (se-sn) STRNCAT(tmp,source+sn,se-sn);

	if (dl-de) strcat(tmp,dest+de);
	else if (ddl-dde) strcat(tmp,dpath+dde);
	else if (sl-se) strcat(tmp,source+se);

	return tmp;
}

/*<DOC> */
/**********************************************************/
/* Similar a path_src2destp(), pero sin path por defecto
(internamente se pone a "./" (".\" en MSDOS)).

Ademas:

Si {dest} es "-", no lo modifica, y si {source}
es "-" realmente se considera que es "noname".

Si el resultado final de {dest} empieza por "./", dicha
subcadena se elimina.

Esta es la funcion mas adecuada para convertir fuente en
destino para argumentos enviados en la linea de comando. */

char * path_src2dest( const char * source, const char * dest )
/*</DOC> */
{
	return path_src2deste(source,dest,NULL);
}

/*<DOC> */
/**********************************************************/
/* Similar a path_src2dest(), pero permite enviar una
extension por defecto. */

char * path_src2deste( const char * source, const char * dest, const char *dext )
/*</DOC> */
{
	static char dd[]={ '.', PATH_DIRSEP, '\0'};
	char * s;

	if (!strcmp(dest,"-")) s=strdup(dest);
	else if (!strcmp(source,"-")) s=path_src2destde("noname",dest,dd,dext);
	else s=path_src2destde(source,dest,dd,dext);

	if (s==strstr(s,dd)) strcpy(s,s+2);
	return s;
}

/**********************************************************/
