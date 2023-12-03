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

Nombre fuente................ UTI_STR.C
Nombre paquete............... UTI.H
Lenguaje fuente.............. C (BC31,GCC)
Estado....................... Utilizable
Dependencia Hard/OS.......... -
Codigo condicional........... STRUPR, etc.

Codificacion................. Borja Etxebarria

Version  dd/mm/aa  Autor     Comentario
-------  --------  --------  ----------
2.1.2    2012  	   Iñaki     eliminar dependencia de xalloc y deprecated warnings (char *)
2.1.1    26/04/11  Iñaki     cambios para que compile con nuevo gcc
2.1.0    04/08/98  Borja     strsearch()
2.0.1    23/09/97  Borja     deteccion switch mejorada
2.0.0    01/05/97  Borja     mix de xfiles, syst, xsyst, fname, vstr...
1.1.2    25/03/97  Borja     error en unos #ifdef __CC_GNUC__
1.1.1    05/03/97  Borja     quitar warnings inocentes en GCC
1.1.0    28/10/96  Borja     incorporar stristr()
1.0.1    22/07/96  Borja     uso de ARCH.H, bugs en stricmp()/strnicmp()
1.0.0    11/01/93  Borja     Codificacion inicial.

======================== Contenido ========================
Funciones para cadenas, que traen algunos compiladores
pero no otros.
Funciones utiles, cadenas con valor por defecto (vstr)
Conversion cadena->numero.
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "uti.h"

/**********************************************************/
/* convierte a mayusculas todos los caracteres de la cadena [str].
[devuelve] un puntero a la cadena */

#ifdef STRUPR

char * strupr( char * str )
{
	char* tmp=str;
	do
		(*tmp) = toupper(*tmp);
	while (*(tmp++));
	return(str);
}

#endif

/**********************************************************/
/* convierte a minusculas todos los caracteres de la cadena [str].
[devuelve] un puntero a la cadena */

#ifdef STRLWR

char * strlwr( char * str )
{
	char* tmp=str;
	do
		(*tmp) = tolower(*tmp);
	while (*(tmp++));
	return(str);
}

#endif

/**********************************************************/
/* reserva memoria para una nueva cadena y copia [str] en ella.
[devuelve] NULL si no se pudo copiar la cadena. El usuario debe
ejecutar free() para liberar la memoria */

#ifdef STRDUP

char * strdup( const char * str )
{
	char * retval;

	if ((retval=(char *)malloc(strlen(str)+1))!=NULL)
		strcpy(retval,str);

	return(retval);
}

#endif
/**********************************************************/
/* compara un maximo de [maxlen] caracteres de [s2] con [s1] ignorando
diferencias mayusculas-minusculas.
[devuelve]
  <0 si s1<s2
  =0 si s1=s2
  >0 si s1>s2    */

#ifdef STRNICMP
#ifndef __CC_GNUC__
#error mierda
int strnicmp( const char * s1, const char * s2, size_t maxlen )
{
	size_t i;
	int k;

	for (i=0; i<maxlen; i++) {
		k = toupper(*s1)-toupper(*s2);
		if ((k)||(!(*s1))||(!(*s2)))
			return k;
		s1++;
		s2++;
	}
	return 0;
}
#endif
#endif
/**********************************************************/
/* compara [s2] con [s1] ignorando mayusculas-minusculas.
  [devuelve]  <0 si [s1]<[s2]
			  =0 si [s1]=[s2]
			  >0 si [s1]>[s2] */

#ifdef STRICMP
#ifndef __CC_GNUC__
int stricmp( const char * s1, const char * s2 )
{
	int k;

	do
	k = toupper(*s1)-toupper(*s2);
	while ((!k)&&(*s1)&&(*s2));

	return k;
}
#endif
#endif

/**********************************************************/

#ifdef STRISTR
char * stristr( const char * s1, const char * s2 )
{
	const char *ss1, *ss2;

	while (*s1) {

		ss1 = s1;
		ss2 = s2;
		while (*ss1) {
			if (toupper(*ss1)!=toupper(*ss2))
				break;
			ss2++;
			ss1++;
		}
		if (!(*ss2))
			return (char*)s1;

		s1++;
	}

	return NULL;
}

#endif

/*<DOC>*/
/**********************************************************/
/* busca la cadena {sub} dentro de {txt} y {devuelve}!=NULL si
la encuentra ({devuelve} puntero a la posicion en que se ha
encontrado la cadena).
En {sub} se puede usar el caracter $ como ancla de principio
o final de cadena.
Si {sub}==NULL {devuelve} NULL.
Si {txt}==NULL {devuelve} NULL. */

const char *strsearch( const char* txt, const char* sub, int casesens )
/*</DOC>*/
{
	int len;

	if (!sub) return NULL;
	if (!txt) return NULL;

	len = strlen(sub);

	if ((*sub=='$') && (sub[len-1]=='$')) { /* ancla pcpio. y final */
		if (casesens) { if (!strncmp(sub+1,txt,len-2) && ((int)strlen(txt)==len-2)) return txt; }
		else if (!strnicmp(sub+1,txt,len-2) && ((int)strlen(txt)==len-2)) return txt;
		return NULL;
	}

	if (*sub=='$') { /* ancla al principio */
		if (casesens) { if (!strncmp(txt,sub+1,len-1)) return txt; }
		else if (!strnicmp(txt,sub+1,len-1)) return txt;
		return NULL;
	}

	if (len && (sub[len-1]=='$')) { /* ancla al final */
		txt += strlen(txt)-(len-1);
		if (casesens) { if (!strncmp(txt,sub,len-1)) return txt; }
		else if (!strnicmp(txt,sub,len-1)) return txt;
		return NULL;
	}

	return strstr(txt,sub);
}

/**********************************************************/
/* Quita todas las apariciones del caracter {sch} en la cadena {str}.
{devuelve} un puntero a la cadena */

char * str_stripch( char * str, char sch )
{
	char * ch1, * ch2;

	ch1=ch2=str;
	do
		if ((*ch1)!=sch) *(ch2++) = (*ch1);
	while (*(ch1++));

	return(str);
}

/**********************************************************/
/* Quita todas las apariciones de los caracteres {schs} en la
cadena {str}. {devuelve} un puntero a la cadena */

char * str_stripchset( char * str, const char *schs )
{
	char * ch1, * ch2;

	ch1=ch2=str;
	do
		if (strchr(schs,*ch1)==NULL) *(ch2++) = (*ch1);
	while (*(ch1++));

	return(str);
}

/**********************************************************/
/* borra todos los caracteres de la cadena {s} a partir de la primera
aparicion de {cutch} (incluido).
{devuelve} un puntero a la cadena. */

char * str_cutfromch( char * s, char cutch )
{
	char * cut;

	if ((cut=strchr(s,cutch))!=NULL)
		*cut = '\0';

	return(s);
}

/**********************************************************/
/* borra todos los caracteres de la cadena {s} a partir de la primera
aparicion de alguno de los de {cutch} (incluido).
{devuelve} un puntero a la cadena. */

char * str_cutfromchset( char * s, const char* cutchset )
{
	s[strcspn(s,cutchset)]='\0';
	return(s);
}

/**********************************************************/
/* Busca {ch} en la cadena {s}, y {devuelve} un puntero a ese caracter.
Si no lo encuentra, {devuelve} un puntero al final de la cadena */

char * str_xchr( const char * s, char ch )
{
	char * ass;

	ass = (char *)strchr(s,ch);//inaki, añadir (char *)
	if (ass==NULL)
		return (char*)(s+strlen(s));
	else
		return (ass);
}

/**********************************************************/
/* Busca {ch} en la cadena {s}, y {devuelve} un puntero a lo
que viene a continuacion.
Si no lo encuentra, {devuelve} un puntero al final de la cadena */

char * str_xchrnext( const char * s, char ch )
{
	char * ass;

	ass = (char *) strchr(s,ch);//inaki, añadir (char *)
	if (ass==NULL)
		return (char*)(s+strlen(s));
	else
		return(ass+1); 
}

/**********************************************************/
/* Busca {ch} en la cadena {s} y {devuelve} un puntero a lo
que viene a continuacion. Modifica {s} para que devuelva solo
lo que viene hasta antes del caracter {ch}. */

char * str_xchrnextdiv( char * s, char ch )
{
	char * ass;

	ass = strchr(s,ch);
	if (ass==NULL)
		return(s+strlen(s));
	else {
		*ass = '\0';
		return(ass+1);
	}
}

/**********************************************************/
/* Busca FILE_KVASSIGN ('=', definido en SYST.H) en la cadena {s},
y {devuelve} un puntero a ese caracter, o al final de la cadena
si no lo encuentra */

char * str_assignptr( const char * s )
{
	return str_xchr(s,FILE_KVASSIGN);
}

/**********************************************************/
/* Busca FILE_KVASSIGN ('=', definido en SYST.H) en la cadena {s},
y {devuelve} un puntero al siguiente caracter, o al final de la
cadena si no lo encuentra */

char * str_assignptrnext( const char * s )
{
	return str_xchrnext(s,FILE_KVASSIGN);
}

/**********************************************************/
/* {devuelve} distinto de cero si {str} es un switch (comienza
por alguno de los caracteres que definen a un switch. Estos
caracteres se fijan en UTI.H en la cadena CL_SWITCH */

int str_isswitch( const char * str )
{
	return (str[0] && str[1] && (strchr(CL_SWITCH,str[0])!=NULL));
}


/**********************************************************/
/* convierte {str} a float, y mete el resultado en {f}.
{devuelve} 0 si todo va bien, !=0 si error (-1 si no se ha podido
convertir nada, o {devuelve} n>0 si la cadena contiene un valor
hasta cierta posicion n incluida */

int str2f( const char * str, float * f )
{
	int n, i;
	i=sscanf(str,"%f%n",f,&n);
	if (i!=1) return -1;
	if (str[n]!='\0') return n;
	return 0;
}

/**********************************************************/
/* convierte {str} a double, y mete el resultado en {d}.
{devuelve} 0 si todo va bien, !=0 si error (-1 si no se ha podido
convertir nada, o {devuelve} n>0 si la cadena contiene un valor
hasta cierta posicion n incluida */

int str2d( const char * str, double * d )
{
	int n, i;
	i=sscanf(str,"%lf%n",d,&n);
	if (i!=1) return -1;
	if (str[n]!='\0') return n;
	return 0;
}

/**********************************************************/
/* convierte {str} a short int, y mete el resultado en {si}.
{devuelve} 0 si todo va bien, !=0 si error (-1 si no se ha podido
convertir nada, o {devuelve} n>0 si la cadena contiene un valor
hasta cierta posicion n incluida */

int str2si( const char * str, short int * si )
{
	int n, i;
	i=sscanf(str,"%hd%n",si,&n);
	if (i!=1) return -1;
	if (str[n]!='\0') return n;
	return 0;
}


/**********************************************************/
/* convierte {str} a int, y mete el resultado en {ii}.
{devuelve} 0 si todo va bien, !=0 si error (-1 si no se ha podido
convertir nada, o {devuelve} n>0 si la cadena contiene un valor
hasta cierta posicion n incluida */

int str2i( const char * str, int * ii )
{
	int n, i;
	i=sscanf(str,"%d%n",ii,&n);
	if (i!=1) return -1;
	if (str[n]!='\0') return n;
	return 0;
}

/**********************************************************/
/* convierte {str} a long double, y mete el resultado en {ld}.
{devuelve} 0 si todo va bien, !=0 si error (-1 si no se ha podido
convertir nada, o {devuelve} n>0 si la cadena contiene un valor
hasta cierta posicion n incluida */

int str2ld( const char * str, long double * ld )
{
	int n, i;
	i=sscanf(str,"%Lf%n",ld,&n);
	if (i!=1) return -1;
	if (str[n]!='\0') return n;
	return 0;
}

/**********************************************************/
/* convierte {str} a long int, y mete el resultado en {li}.
{devuelve} 0 si todo va bien, !=0 si "error" (-1 si no se ha podido
convertir nada, o {devuelve} n>0 si la cadena contiene un valor
hasta cierta posicion n incluida (li es valido en tal caso) */

int str2li( const char * str, long int * li )
{
	int n, i;
	i=sscanf(str,"%ld%n",li,&n);
	if (i!=1) return -1;
	if (str[n]!='\0') return n;
	return 0;
}

/**********************************************************/
/* convierte str a float.
Si str=="" devuelve defval.
Si str no contiene un valor valido, mensaje y al SO */

float xstr2f( const char * str, float defval)
{
	float f;
	if (strlen(str)==0) return(defval);
	cdie_beep(str2f(str,&f)!=0,"valor numerico no valido (%s)",str);
	return f;
}

/**********************************************************/
/* convierte str a short int.
Si str=="" devuelve defval.
Si str no contiene un valor valido, mensaje y al SO */

short int xstr2si( const char * str, short int defval )
{
	short int si;
	if (strlen(str)==0) return(defval);
	cdie_beep(str2si(str,&si)!=0,"valor entero no valido (%s)",str);
	return si;
}

/**********************************************************/
/* convierte str a double.
Si str=="" devuelve defval.
Si str no contiene un valor valido, mensaje y al SO */

double xstr2d( const char * str, double defval)
{
	double d;
	if (strlen(str)==0) return(defval);
	cdie_beep(str2d(str,&d)!=0,"valor numerico no valido (%s)",str);
	return d;
}

/**********************************************************/
/* convierte str a int.
Si str=="" devuelve defval.
Si str no contiene un valor valido, mensaje y al SO */

int xstr2i( const char * str, int defval )
{
	int i;
	if (strlen(str)==0) return(defval);
	cdie_beep(str2i(str,&i)!=0,"valor entero no valido (%s)",str);
	return i;
}

/**********************************************************/
/* convierte str a long double.
Si str=="" devuelve defval.
Si str no contiene un valor valido, mensaje y al SO */

long double xstr2ld( const char * str, long double defval)
{
	long double ld;
	if (strlen(str)==0) return(defval);
	cdie_beep(str2ld(str,&ld)!=0,"valor numerico no valido (%s)",str);
	return ld;
}

/**********************************************************/
/* convierte str a long int.
Si str=="" devuelve defval.
Si str no contiene un valor valido, mensaje y al SO */

long int xstr2li( const char * str, long int defval )
{
	long int li;
	if (strlen(str)==0) return(defval);
	cdie_beep(str2li(str,&li)!=0,"valor entero no valido (%s)",str);
	return li;
}

/**********************************************************/
/* Convierte str a booleano (1/0) */

int str2bool( const char *str, int def )
{
	char *yes[]={"yes", "on", "true", "bai", "si",NULL};
	char *no[]= {"no", "off", "false", "ez", NULL};
	char *pos="+123456789sSyYtTbB";
	char *neg="-0nNfFeE";
	char **s;
	if (!str || (*str=='\0')) return def;
	s=yes; while (*s) { if (!stricmp(str,*s)) return 1; s++; }
	s=no; while (*s) { if (!stricmp(str,*s)) return 0; s++; }
	if (strchr(pos,*str)) return 1;
	if (strchr(neg,*str)) return 0;
	return def;
}

/**********************************************************/

const char *bool2str( int boo )
{
	return boo?"True":"False";
}

/**********************************************************/
/* hace una copia de {newstr} y la {devuelve} (usar free()
para borrarla). ademas si {oldvs}!=NULL, la borra con free() */

char *vstr_dup( char *oldvs, const char *newstr )
{
	char* vs;

	vstr_free(oldvs);

	if (newstr!=NULL) {
		vs=(char*)malloc(strlen(newstr)+1);
		strcpy(vs,newstr);
		return vs;
	}
	else
		return NULL;
}

/**********************************************************/
/* borra {vs} si es !=NULL */

void vstr_free( char* vs )
{
	if (vs!=NULL) free(vs);
}

/**********************************************************/
/* {devuelve} {vs} si !=NULL, o {defval} si {vs}==NULL */

char* vstr_getstr( const char* vs, const char* defval )
{
	if (vs==NULL) return (char*)defval;
	else return (char*)vs;
}

/**********************************************************/

