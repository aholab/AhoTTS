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

Nombre fuente................ UTI_FILE.C
Nombre paquete............... UTI.H
Lenguaje fuente.............. C (BC31,GCC)
Estado....................... Utilizable
Dependencia Hard/OS.......... SI
Codigo condicional........... XFILE_FCLOSEALL ...

Codificacion................. Borja Etxebarria

Version  dd/mm/aa  Autor     Comentario
-------  --------  --------  ----------
2.0.4    2012  	   Inaki     eliminar dependencia de xalloc
2.0.3    21/04/00  Borja     fgetln_filt() -> comillas dobles
2.0.2    20/01/98  Borja     soporte MS Visual C++ para ftrunc()
2.0.1    06/08/97  Borja     incluir o no unistd _despues_ de uti.h
2.0.0    01/05/97  Borja     mix de xfiles, syst, xsyst, fname, vstr...
1.0.2    30/07/96  Borja     usar xdie() para errores
1.0.1    15/03/95  Borja     comp. condicional automatica (__CC_BORLANDC__)
1.0.0    11/01/93  Borja     Codificacion inicial.

======================== Contenido ========================
Funciones xfile, para manejo de streams similares
a fopen(), fclose() etc, pero que en caso de error imprimen
un mensaje y terminan el programa. Las funciones se llaman
igual, pero con una 'x' delante.

Hay varios segmentos condicionales, controlados por los defines
indicados en XFILES.H, para crear los equivalentes
'x' de funciones no estandar (fcloseall(), etc.).

Funciones para mover bloques, borrar bloques, truncar
ficheros, lectura de ficheros de texto (con comentarios, etc).
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "uti.h"

#ifdef __CC_GNUC__
#include <unistd.h>
#endif

/**********************************************************/
/* termina el programa despues de imprimir un mensaje de error.
Imprime la cadena de error que corresponda a {errno}, precedida
de la cadena {fn} */

void xfile_error ( const char * fn )
{
	die_beep("%s error: %s",fn,strerror(errno));
}

/**********************************************************/
/* Dependiendo del valor de esta variable, la lectura de datos
de un fichero con xfgetc, xfgets, xfgetln..., produce o no error si se
intenta leer EOF o despues de EOF.

Si {_eof_error} = 0 (XFILE_EOFE_NEVER)
   leer EOF o despues no produce error nunca.

Si {_eof_error} = 1 (XFILE_EOFE_RETRY)
  leer EOF no produce error la primera vez, pero despues si
Si {_eof_error} = 2 (XFILE_EOFE_ALWAYS)
  leer EOF o despues de EOF produce error siempre.

Por defecto esta variable vale XFILE_EOFE_NEVER. */

int _eof_error = XFILE_EOFE_NEVER;

/* {devuelve} distinto de cero si el fichero {f} (FILE *)
esta en estado de error por EOF */
#define eof_status(f)  \
  ((_eof_error==XFILE_EOFE_ALWAYS)||  \
   ((_eof_error==XFILE_EOFE_RETRY)&&(feof(f))));

/**********************************************************/
/* Modifica el valor de la variable {_eof_error}.
Dependiendo del valor de esta variable, la lectura de datos
de un fichero con xfgetc, xfgets, xfgetln..., produce o no error si se
intenta leer EOF o despues de EOF. Ver comentarios junto a la
definicion de {_eof_error} en este mismo fichero.

Si no se produce error, las funciones devolveran el codigo correspondiente
para lectura de EOF.
La funcion devuelve {_eof_error} */

int set_eof_error( int eof_error )
{
	_eof_error = (((eof_error!=XFILE_EOFE_NEVER)&&
			(eof_error!=XFILE_EOFE_ALWAYS)) ?
			XFILE_EOFE_RETRY : eof_error);

	return _eof_error;
}

/**********************************************************/
/* {devuelve} el valor de la variable {_eof_error} */

int get_eof_error( void )
{
	return _eof_error;
}

/**********************************************************/

/**********************************************************/
/* funciones estandard del C para control de ficheros pero con
control basico de errores: llamada a handler de error interno
en  caso de error */
/**********************************************************/

/**********************************************************/

int xfclose( FILE * fp )
{
	int retval;

	if ((retval=fclose(fp))==EOF)
		xfile_error ("fclose");

	return(retval);
}

/**********************************************************/
/* solo si esta definido */

#ifdef XFILE_FCLOSEALL

int xfcloseall( void )
{
	int retval;

	if ((retval=fcloseall())==EOF)
		xfile_error ("fcloseall");

	return(retval);
}

#endif
/**********************************************************/

int xfflush( FILE * fp )
{
	int retval;

	if ((retval=fflush(fp))==EOF)
		xfile_error ("fflush");

	return(retval);
}

/**********************************************************/

int xfgetc( FILE * fp )
{
	int retval;
	int eof_e = eof_status(fp);

	if ((retval=fgetc(fp))==EOF)
		if ( (!feof(fp)) || (eof_e) )
			xfile_error ("fgetc");

	return(retval);
}

/**********************************************************/

int xfgetpos( FILE * fp, fpos_t * pos )
{
	int retval;

	if ((retval=fgetpos(fp,pos))!=0)
		xfile_error ("fgetpos");

	return(retval);
}

/**********************************************************/

char * xfgets( char * s, int n,  FILE * fp )
{
	char * retval;
	int eof_e = eof_status(fp);

	if ((retval=fgets(s,n,fp))==NULL)
		if ( (!feof(fp)) || (eof_e) )
			xfile_error ("fgets");

	return(retval);
}

/**********************************************************/
/* solo si esta definido */

#ifdef XFILE_FLUSHALL

int xflushall( void )
{
	return(flushall());
}

#endif
/**********************************************************/

FILE * xfopen( const char * filename, const char * mode )
{
	FILE * retval;

	if ((retval=fopen(filename,mode))==NULL) {
		char *tmp=(char*)malloc(strlen(filename)+10);
		strcpy(tmp,"fopen("); strcat(tmp,filename); strcat(tmp,")");
		xfile_error (tmp);
		free(tmp);
	}

	return(retval);
}

/**********************************************************/

int xfputc( int c, FILE * fp )
{
	int retval;

	if ((retval=fputc(c,fp))==EOF)
		xfile_error ("fputc");

	return(retval);
}

/**********************************************************/

int xfputs( const char * s, FILE * fp )
{
	int retval;

	if ((retval=fputs(s,fp))==EOF)
		xfile_error ("fputs");

	return(retval);
}

/**********************************************************/

size_t xfread( void * ptr, size_t size, size_t n, FILE * fp )
{
	size_t retval;

	if ((retval=fread(ptr,size,n,fp))<n)
		xfile_error ("fread");

	return(retval);
}

/**********************************************************/

int xfseek( FILE * fp, long int offset, int whence )
{
	int retval;

	if ((retval=fseek(fp,offset,whence))!=0)
		xfile_error ("fseek");

	return(retval);
}

/**********************************************************/

int xfsetpos( FILE * fp, const fpos_t * pos )
{
	int retval;

	if ((retval=fsetpos(fp,pos))!=0)
		xfile_error ("fsetpos");

	return(retval);
}

/**********************************************************/

long xftell( FILE * fp )
{
	long retval;

	if ((retval=ftell(fp))==-1L)
		xfile_error ("ftell");

	return(retval);
}

/**********************************************************/

size_t xfwrite( const void * ptr, size_t size, size_t n, FILE * fp )
{
	size_t retval;

	if ((retval=fwrite(ptr,size,n,fp))<n)
		xfile_error ("fwrite");

	return(retval);
}

/**********************************************************/

int xrename( const char * oldname, const char * newname )
{
	int retval;

	if ((retval=rename(oldname,newname))==-1)
		xfile_error ("rename");

	return(retval);
}

/**********************************************************/

int xremove( const char * filename )
{
	int retval;

	if ((retval=remove(filename))==-1)
		xfile_error ("remove");

	return(retval);
}

/**********************************************************/
/* solo si esta definido */

#ifdef XFILE_UNLINK

int xunlink( const char * filename )
{
	int retval;

	if ((retval=unlink(filename))==-1)
		xfile_error ("unlink");

	return(retval);
}

#endif

/**********************************************************/
/* Lee una cadena de un Stream. Devuelve en {s} la cadena, como mucho {n}
caracteres. {fp} es el Stream.
Se lee una linea completa, y NO se envia el caracter EOL.
La funcion {devuelve} un puntero a la cadena, o NULL si se produce algun
error o se llega a EOF.
Si se envia toolong!=NULL, se devuelve en esa variable !=0 si
la linea es muy larga y no ha entrado en los n caracteres.
En Unix, se tiene en cuenta que el fichero puede provenir de MSDOS, y si
aparece un ^M al final de la linea tambien se elimina.
Puede haber lineas vacias */

char * fgetln( char * s, int n, FILE *fp, int* toolong )
{
	char * code;
	size_t l;

	code = fgets(s,n,fp);
	if (code!=NULL) {
		l = strlen(s);
		if ((l)&&(s[l-1]=='\n')) {
			s[--l]=0;
			if (toolong) *toolong=0;
		}
		else if (toolong && (l>=(size_t)(n-1))) *toolong=1;
#ifdef __OS_UNIX__
		if ((l)&&(s[l-1]=='\r')) s[--l]=0;
#endif
	}

	return code;
}

/**********************************************************/
/* Uso interno. procesado de la linea de texto {s} para la funcion
fgetln_filt().

{remove_white}==1 quita todo
{remove_white}==2 preserva cadenas
{remove_white}==3 preserva comentarios
{remove_white}==4 preserva cadenas y comentarios
Los anteriores +4 (=5,6,7,8) eliminan las comillas iniciales y
finales de las cadenas, y las comillas intermedias dobles se
pasan a simples ("" --> "))
*/

PRIVATE char *lncss( char * s, int remove_comment, int remove_white )
{
	char *s1, *s2;
	int instr = 0;
	int incom = 0;
	int quote = 0;

	if (remove_white>4) {
		remove_white -= 4;
		quote = 1;
	}

	if (s!=NULL) {
		s1 = s2 = s;
		do {
			if (incom) {
				if (((remove_white==1)||(remove_white==2)) &&
						(strchr(" \t",*s1)!=NULL)) continue;
			}
			else if (instr) {
				if (((remove_white==1)||(remove_white==3)) &&
						(strchr(" \t",*s1)!=NULL)) continue;
				else if (*s1==FILE_STRCODE) {
					if (quote) {
						if (s1[1]==FILE_STRCODE) s1++;
						else { instr=0; continue; }
					}
					else instr=0;
				}
			}
			else {
				if (remove_white && (strchr(" \t",*s1)!=NULL)) continue;
				else if (*s1==FILE_STRCODE) { instr=1; if (quote) continue; }
				else if (strchr(FILE_COMMENT,*s1)!=NULL) {
					if (remove_comment) *s1='\0'; else incom=1;
				}
			}
			*(s2++)= *s1;
		} while (*(s1++));
		*s2 = '\0';
	}
	return s;
}

/**********************************************************/
/* Lee una cadena de un Stream. Devuelve en {s} la cadena, como
mucho {n} caracteres ('\0' incluido). {fp} es el Stream.
Se lee una linea completa, y NO se envia el caracter EOL, ademas
en Unix se tiene en cuenta que el fichero puede venir del MSDOS y
si aparece el ^M final se elimina tambien.

Si {remove_comment}==1 de la linea leida, se eliminan los
comentarios (comienzan por FILE_COMMENT ('#' o ';') definido en SYST.H)

Si {remove_white}==1 de la linea leida, se eliminan los espacios
en blanco y los tabs.

Si {remove_white}==2, como cuando vale 1, solo que se consideran las
cadenas (entre comillas dobles, definido en FILE_STRCODE), de las que no
se eliminan los espacios (las comillas inicial/final tampoco se eliminan.
para insertar comillas dentro de la cadena, usar "" por ejemplo
"esto "" es una comilla". ).

Si {remove_white}==3, como cuando vale 1, solo que se consideran los
comentarios, y en caso de preservarse (remove_comment=0) no se modifican
sus espacios.

Si {remove_white}==4, como cuando vale 2 y 3 a la vez, es decir, se
consideran las cadenas y los comentarios a la hora de eliminar los
espacios/tabs.

Si a los valores anteriores de {remove_white} se les suma 4, se obtiene
el mismo comportamiento (el que corresponda) y ademas se eliminan las
comillas inicial/final, y las dobles internas se simplifican ("" --> ").

Las lineas vacias (despues de los procesos anteriores) se eliminan
si remove_wlines==TRUE, o se dejan pasar si es FALSE.

Si se envia toolong!=NULL, se devuelve en esa variable !=0 si
la linea es muy larga y no ha entrado en los n caracteres.

La funcion {devuelve} un puntero a la cadena, o NULL si se produce algun
error o se llega a EOF. */

char * fgetln_filt( char * s, int n, FILE *fp,
		int remove_comment, int remove_white, int remove_wlines, int *toolong )
{
	char *code;

	do {
		code = fgetln(s,n,fp,toolong);
		if (code==NULL) break;
		lncss(s,remove_comment,remove_white);
	} while (remove_wlines &&(*s=='\0'));

	return code;
}

/**********************************************************/
/* {devuelve} el tamano del fichero {fp}, -1 en caso de error */

long flen( FILE * fp )
{
	long pos, len;

	pos=ftell(fp);
	len=-1L;

	if (pos!=-1L) {
		if (fseek(fp,0,SEEK_END)==0)
			len = ftell(fp);

		if (fseek(fp,pos,SEEK_SET)!=0)
			len=-1L;
	}

	return len;
}

/**********************************************************/
/* ftrunc() trunca un fichero a partir de la posicion actual.
La funcion {devuelve} 0 si todo va bien, o un codigo !=0 en caso
de error. La posicion actual en el fichero no cambia */

#ifdef __CC_GNUC__
#include <unistd.h>
int ftrunc( FILE * fp )
{
	return ftruncate(fileno(fp),ftell(fp));
}

#elif __CC_BORLANDC__
#include <dos.h>
int ftrunc( FILE * fp )
{
	union REGS regs;
	int ax;

	regs.h.ah = 0x40;
	regs.x.bx = fileno(fp);
	regs.x.cx = 0;    /* trunc */
	ax = intdos(&regs, &regs);

	return(regs.x.cflag ? ax : 0);
}
#elif __CC_MSVC__
#include <stdio.h>
#include <io.h>
int ftrunc( FILE * fp )
{
	return _chsize(_fileno(fp),ftell(fp));
}

#else
#error Undefined Compiler/Platform
#endif

/**********************************************************/
/* uso interno. Reserva un buffer lo mas grande posible,
{size} se modifica para reflejar el tamanyo final */

PRIVATE void *_amalloc( size_t * size )
{
	void *p;

	while (*size) {
		p = malloc(*size);
		if (p)
			return p;
		(*size) =  (size_t)(((long)(*size)+1) >> 1);
	}
	return NULL;
}

/**********************************************************/
/* mueve un segmento de un fichero a otra posicion del fichero.
{devuelve} 0 si todo va bien, o -1 en caso de error.
Parametros:  {fp} es el fichero; {from} es la posicion inicial;
{to} es la posicion final; {n} es la cantidad de bytes a mover.
La funcion tiene en cuenta que fuente y destino pueden solaparse, y
mueve los datos de forma adecuada.
La posicion en el fichero queda indeterminada.
El fichero tiene que estar abierto para lectura y escritura.
Todas las posiciones modificadas deben ser validas y existir
previamente (no se anyaden bytes nuevo al fichero) */

int fmoven( FILE* fp, long from, long to, long n )
{
	long i;
	char *buf;
	size_t l, nn;

	if ((from==to)||(!n)) return 0;

	l = 32767;
	buf = (char*)_amalloc(&l);
	if (!buf) return -1;

	if (from>to) {
		i = 0;
		while (n) {
			nn = (n>(long)l)? l : (size_t)n;
			if (fseek(fp,from+i,SEEK_SET)) break;
			if (fread(buf,1,nn,fp)<nn) break;
			if (fseek(fp,to+i,SEEK_SET)) break;
			if (fwrite(buf,1,nn,fp)<nn) break;
			n -= nn;
			i += nn;
		}
	}
	else {
		i = n;
		while (n) {
			nn = (n>(long)l)? l : (size_t)n;
			n -= nn;
			i -= nn;
			if (fseek(fp,from+i,SEEK_SET)) break;
			if (fread(buf,1,nn,fp)<nn) break;
			if (fseek(fp,to+i,SEEK_SET)) break;
			if (fwrite(buf,1,nn,fp)<nn) break;
		}
	}
	free(buf);
	if (n) return -1;

	return 0;
}

/**********************************************************/
/* inserta un hueco de {n} bytes en la posicion {pos} del fichero {fp}.
Si todo va bien, {devuelve} 0; en caso de error -1.  La posicion
en el fichero queda indeterminada.
El hueco insertado se rellena con cualquier cosa (indeterminado).
El fichero tiene que estar abierto para lectura y escritura */

int finsn( FILE* fp, long pos, long n )
{
	long len, xn;
	char *buf;
	size_t l, nn;

	if (fseek(fp,0,SEEK_END)) return -1;
	if ((len=ftell(fp))==-1L) return -1;
	if ((pos>len)||(pos<0)) return -1;

	xn = n;
	l = 32767;
	buf = (char*)_amalloc(&l);
	if (!buf) return -1;
	while (n) {
		nn = (n>(long)l)? l : (size_t)n;
		if (fwrite(buf,1,nn,fp)<nn) break;
		n -= nn;
	}
	free(buf);
	if (n) return -1;

	return fmoven(fp, pos, pos+xn, len-pos);
}

/**********************************************************/
/* borra un segmento de {n} bytes en la posicion {pos} del fichero {fp}.
Si todo va bien, {devuelve} 0; en caso de error -1.  La posicion
en el fichero queda indeterminada.
El fichero tiene que estar abierto para lectura y escritura */

int fdeln( FILE* fp, long pos, long n )
{
	long len;

	if (fseek(fp,0,SEEK_END)) return -1;
	if ((len=ftell(fp))==-1L) return -1;
	if ((pos>len)||(pos<0)) return -1;
	if (pos+n > len)
		n = len-pos;
	if (fmoven(fp,pos+n,pos,len-(pos+n))) return -1;
	if (fseek(fp,len-n,SEEK_SET)) return -1;

	return ftrunc(fp);
}

/**********************************************************/
/* ver fgetln() */

char * xfgetln( char * s, int n, FILE *fp, int *toolong )
{
	char * retval;
	int eof_e = eof_status(fp);

	if ((retval=fgetln(s,n,fp,toolong))==NULL)
		if ( (!feof(fp)) || (eof_e) )
			xfile_error ("fgetln");

	return(retval);
}

/**********************************************************/
/* ver fgetln_filt()  */

char * xfgetln_filt( char * s, int n, FILE *fp,
		int remove_comment, int remove_white, int remove_wlines, int *toolong )
{
	char * retval;
	int eof_e = eof_status(fp);

	if ((retval=fgetln_filt(s,n,fp,remove_comment,remove_white,remove_wlines,toolong))==NULL)
		if ( (!feof(fp)) || (eof_e) )
			xfile_error ("fgetln_filt");

	return(retval);
}

/**********************************************************/
/* ver flen() */

long xflen( FILE *fp )
{
long l;

l = flen(fp);

if (l==-1L)
  xfile_error ("flen");

return l;
}

/**********************************************************/
/* ver ftrunc */

int xftrunc( FILE * fp )
{
	int retval;

	if ((retval=ftrunc(fp))!=0)
		xfile_error ("ftrunc");

	return(retval);
}

/**********************************************************/
/* ver fmoven */

int xfmoven( FILE* fp, long from, long to, long n )
{
	int retval;

	if ((retval=fmoven(fp,from,to,n))!=0)
		xfile_error ("fmoven");

	return(retval);
}

/**********************************************************/
/* ver finsn */

int xfinsn( FILE* fp, long pos, long n )
{
	int retval;

	if ((retval=finsn(fp,pos,n))!=0)
		xfile_error ("finsn");

	return(retval);
}

/**********************************************************/
/* ver fdeln */

int xfdeln( FILE* fp, long pos, long n )
{
	int retval;

	if ((retval=fdeln(fp,pos,n))!=0)
		xfile_error ("fdeln");

	return(retval);
}

/**********************************************************/
