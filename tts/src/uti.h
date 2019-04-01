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
#ifndef __UTI_H
#define __UTI_H

/**********************************************************/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/*
Copyright: 1993 - Grupo de Voz (DAET) ETSII/IT-Bilbao

Nombre fuente................ uti.h
Nombre paquete............... AhoLib
Lenguaje fuente.............. C (BC31/GCC)
Estado....................... Utilizable
Dependencia Hard/OS.......... SI
Codigo condicional........... XFILE_???, STR???, etc

Codificacion................. Borja Etxebarria

Version  dd/mm/aa  Autor     Comentario
-------  --------  --------  ----------
2.2.4    21/04/00  Borja     fgetln_filt() -> comillas dobles (macro fgetlnf)
2.2.3    21/04/00  Borja     soporte DJGPP
2.2.2    06/08/97  Borja     bug en macros endian_???
2.2.1    06/08/97  Borja     bug en define XFILES_?? -> XFILE_??
2.2.0    22/07/97  Borja     show_srcpos()
2.1.0    22/07/97  Borja     little/big endian management
2.0.1    01/07/97  Borja     uti_math
2.0.0    01/05/97  Borja     mix de xfiles, syst, xsyst, fname, vstr...
1.2.0    31/07/96  Borja     die
1.1.0    30/07/96  Borja     ftrunc, fmoven, finsn, fdeln.
1.0.0    11/01/93  Borja     Codificacion inicial.

======================== Contenido ========================
<DOC>
Funciones para ficheros, cadenas, etc. ver los
ficheros fuentes UTI_????.C
</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "arch.h"
#include "tdef.h"

/**********************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************/
/* caracteres que indican comienzo de switch */
#if (defined(__OS_UNIX__)&&!defined(__OS_MSDOS32__))
#define CL_SWITCH    "-"  
#else 
#define CL_SWITCH    "-/"  
#endif


#define FILE_COMMENT "#;"  /* caracteres que indican comentario */
#define FILE_STRCODE  '"'   /* acotador de cadenas de texto */
#define FILE_KVASSIGN '='   /* signo de asignacion n1=n2 */

/**********************************************************/

int nptrs( const void * const *ptrarray );
int str_match( const char * const * strarr, int nstrs, const char * str,
		int lcode, int case_sensitive );
int label_match( const char * const * labelarr, int nlabels, const char * label,
		int case_sensitive, int allow_partial );
int xstr_match( const char * const *strarr, int nstrs, const char * str,
		int lcode, int case_sensitive );
int xlabel_match( const char * const *labelarr, int nlabels, const char * label,
		int case_sensitive, int allow_partial );
int str2k( const char * str, const char * const *labels,
		int defval, int case_sensitive, int allow_partial );

/**********************************************************/

void die( const char *fmt, ... );
//void xdie( const char *fmt, ... );
void die_beep( const char *fmt, ... );
void cdie( int condition, const char *fmt, ... );
//void xcdie( int condition, const char *fmt, ... );
void cdie_beep( int condition, const char *fmt, ... );

void vdie( const char *fmt, va_list argptr );
void vdie_beep( const char *fmt, va_list argptr );
void vcdie( int condition, const char *fmt, va_list argptr );
void vcdie_beep( int condition, const char *fmt, va_list argptr );

#define show_srcpos() __show_srcpos(__FILE__,__LINE__)
#define cshow_srcpos(c) if (c) __show_srcpos(__FILE__,__LINE__)
void __show_srcpos( const char *file, int line );

/**********************************************************/
/* Funciones sobre ficheros, funciones xfile */


/* codigos para configurar comportamiento en caso de EOF de las
funciones xfile */
#define XFILE_EOFE_NEVER  0
#define XFILE_EOFE_RETRY  1
#define XFILE_EOFE_ALWAYS  2


/* algunas funciones que dependen del compilador */
#if __CC_BORLANDC__
#define XFILE_FCLOSEALL   /* fcloseall() */
#define XFILE_FLUSHALL    /* flushall() */
#define XFILE_UNLINK      /* unlink() */

#elif __CC_GNUC__
#define XFILE_UNLINK

#else
#endif

//#define fgetlns(s,n,fp) fgetln_filt(s,n,fp,0,1,1,NULL)
//#define fgetlnc(s,n,fp) fgetln_filt(s,n,fp,1,0,1,NULL)
//#define fgetlncs(s,n,fp) fgetln_filt(s,n,fp,1,1,1,NULL)
//#define fgetlncss(s,n,fp) fgetln_filt(s,n,fp,1,4,1,NULL)
char * fgetln( char * s, int n, FILE *fp, int *tl );
char * fgetln_filt( char * s, int n, FILE *fp, int rm_comnt, int rm_white, int rm_wline, int *tl );
#define fgetlnf(s,n,fp) fgetln_filt(s,n,fp,1,8,1,NULL)
long flen( FILE * fp );
int ftrunc( FILE * fp );
int fmoven( FILE* fp, long from, long to, long n );
int finsn( FILE* fp, long pos, long n );
int fdeln( FILE* fp, long pos, long n );
//#define xfgetlns(s,n,fp) xfgetln_filt(s,n,fp,0,1,1,NULL)
//#define xfgetlnc(s,n,fp) xfgetln_filt(s,n,fp,1,0,1,NULL)
//#define xfgetlncs(s,n,fp) xfgetln_filt(s,n,fp,1,1,1,NULL)
//#define xfgetlncss(s,n,fp) xfgetln_filt(s,n,fp,1,4,1,NULL)
char * xfgetln( char * s, int n, FILE *fp, int *tl );
char * xfgetln_filt( char * s, int n, FILE *fp, int rm_comnt, int rm_white, int rm_wline, int *tl);
#define xfgetlnf(s,n,fp) xfgetln_filt(s,n,fp,1,8,1,NULL)
long xflen( FILE * fp );
int xftrunc( FILE * fp );
int xfmoven( FILE* fp, long from, long to, long n );
int xfinsn( FILE* fp, long pos, long n );
int xfdeln( FILE* fp, long pos, long n );
void xfile_error( const char * fn );
int set_eof_error( int eof_error );
#define eof_error_off()  set_eof_error(XFILE_EOFE_NEVER)
#define eof_error_on()  set_eof_error(XFILE_EOFE_ALWAYS)
int get_eof_error( void );
#define xfeof(fp) feof(fp)
int xfclose( FILE * fp );
int xfflush( FILE * fp );
int xfgetc( FILE * fp );
int xfgetpos( FILE * fp, fpos_t * pos );
char * xfgets( char * s, int n,  FILE * fp );
FILE * xfopen( const char * filename, const char * mode );
int xfputc( int c, FILE * fp );
int xfputs( const char * s, FILE * fp );
size_t xfread( void * ptr, size_t size, size_t n, FILE * fp );
int xfseek( FILE * fp, long int offset, int whence );
int xfsetpos( FILE * fp, const fpos_t * pos );
long int xftell( FILE * fp );
size_t xfwrite( const void * ptr, size_t size, size_t n, FILE * fp );
int xrename( const char * oldname, const char * newname );
int xremove( const char * filename );
#ifdef XFILE_FCLOSEALL
int xfcloseall( void );
#endif
#ifdef XFILE_FLUSHALL
int xflushall( void );
#endif
#ifdef XFILE_UNLINK
int xunlink( const char * filename );
#endif

/**********************************************************/
/* funciones para manejo de cadenas, busquedas etc.
Funciones para cadenas, que traen algunos compiladores
pero no otros. */

/* Defines que activan individualmente funciones para cadenas */
#ifdef __CC_BORLANDC__
#define STRISTR

#elif __OS_LINUX__
#define STRUPR
#define STRLWR
#define STRNICMP
#define STRICMP
#define STRISTR

#elif __OS_UNIX__
#define STRUPR
#define STRLWR
#define STRDUP
#define STRNICMP
#define STRICMP
#define STRISTR

#else
#endif

//char * str_strip_ch( char * str, char sch );
char * str_stripch( char * str, char sch );
//char * str_strip_chset( char * str, const char *schs );
char * str_stripchset( char * str, const char *schs );
//char * strcut( char * s, char cutch );
char * str_cutfromch( char * s, char cutch );
char * str_cutfromchset( char * s, const char *cutchset );
//char * getsch( char * s, char ch );
char * str_xchr( const char * s, char ch );
//char * getnsch( char * s, char ch );
char * str_xchrnext( const char * s, char ch );
//char * getsnsch( char * s, char ch );
char * str_xchrnextdiv( char * s, char ch );
char * str_assignptr( const char * s );
//char * nassign_ptr( const char * s );
char * str_assignptrnext( const char * s );
int str_isswitch( const char * str );

/* conversion cadena -> numero */
int str2si( const char * str, short int * si );
int str2i( const char * str, int * ii );
int str2li( const char * str, long int * li );
int str2f( const char * str, float * f );
int str2d( const char * str, double * d );
int str2ld( const char * str, long double * ld );

short int xstr2si( const char * str, short int defval);
int xstr2i( const char * str, int defval );
long int xstr2li( const char * str, long int defval );
float xstr2f( const char * str, float defval );
double xstr2d( const char * str, double defval );
long double xstr2ld( const char * str, long double defval);

int str2bool( const char *str, int def );
const char *bool2str( int boo );

/* algunas funciones dependientes del compilador */
#ifdef STRUPR
char * strupr( char * str );
#endif
#ifdef STRLWR
char * strlwr( char * str );
#endif
#ifdef STRDUP
char * strdup( const char * str );
#endif
#ifdef STRNICMP
#ifdef __CC_GNUC__
#define strnicmp(s1,s2,maxlen)  strncasecmp(s1,s2,maxlen)
#else
int strnicmp( const char * s1, const char * s2, size_t maxlen );
#endif
#endif
#ifdef STRICMP
#ifdef __CC_GNUC__
#define stricmp(s1,s2)  strcasecmp(s1,s2)
#else
int stricmp( const char * s1, const char * s2 );
#endif
#endif
#ifdef STRISTR
char * stristr( const char * s1, const char * s2 );
#endif

const char *strsearch( const char* txt, const char* sub, int casesens );

/*Funciones para cadenas variables con valor por defecto, vstr_??? */
char* vstr_dup( char* oldvs, const char* newstr );
void vstr_free( char* vs );
char* vstr_getstr( const char* vs, const char* defval );

/**********************************************************/
/* renombrar ficheros, fuente->destino, etc */

/* En unix, el separador entre directorios es '/', y el
de disco igua, que no hay.
En el resto de los casos (DOS,WINDOWS), es '\', y el de
disco ':' */

#if (defined(__OS_UNIX__)&&!defined(__OS_MSDOS32__))
#define PATH_DIRSEP '/'
#define PATH_DRIVESEP '/'

#else   /* MSDOS */
#define PATH_DIRSEP '\\'
#define PATH_DRIVESEP ':'
#endif

#define PATH_EXT_BAK ".bak"

typedef struct {
	char *name;
	char *tmp;
} fntmp;
fntmp *fntmp_construct (const char *dest, const char *tmpfile);
const char *fntmp_getname (fntmp * f);
void fntmp_destruct (fntmp * f);

size_t path_namepos (const char *path);
size_t path_extpos (const char *path);

char *path_dirchange (char *path, const char *new_dir);
char *path_namechange(char *path, const char *new_name);
char *path_extchange (char *path, const char *new_ext);
#define path_dirextchange(fname,new_dir,new_ext) path_dirchange(path_extchange(fname,new_ext),new_dir)
char * path_src2dest( const char * source, const char * dest );
char * path_src2deste( const char * source, const char * dest, const char *dext );
char *path_src2destp(const char *source, const char *dest,
		const char *dpath);
#define path_applyrule(source,dest) path_src2destp(source,dest,NULL)

char *path_src2destde(const char *source, const char *dest,
		const char *ddir, const char *dext);

int frename_del (const char *name, const char *new_name);
int frename_ext_del (const char *name, const char *new_ext);
int frename_bak (const char *name, const char *new_name);
int frename_ext_bak (const char *name, const char *new_ext);

void xfrename_del (const char *name, const char *new_name);
void xfrename_ext_del (const char *name, const char *new_ext);
void xfrename_bak (const char *name, const char *new_name);
void xfrename_ext_bak (const char *name, const char *new_ext);

/**********************************************************/
/* funciones matematicas varias */

double fround( double n );
int float2int( double n );
long float2long( double n );
double cuadfit( double y1, double y2, double y3, double *y );
double linterpol( double x, double x1, double x2, double y1, double y2 );

#ifdef __CC_BORLANDC__
#define REMAINDER
#elif __OS_LINUX__
#define REMAINDER
#else
/* $$$ Mirar que plataformas implementan remainder() */
#endif

#ifdef REMAINDER
double remainder( double x, double y );
#endif

/**********************************************************/
/* Manejo little/big endian */

void endian_swap16( VOID * word16 );
void endian_swap32( VOID * word32 );
void endian_swap64( VOID * word32 );
void endian_swap16_n( VOID * word16, int n );
void endian_swap32_n( VOID * word32, int n );
void endian_swap64_n( VOID * word32, int n );

#ifdef __LITTLE_ENDIAN__
#define endian_tolittle16(x) ((void)0)
#define endian_tolittle32(x) ((void)0)
#define endian_tolittle64(x) ((void)0)
#define endian_fromlittle16(x) ((void)0)
#define endian_fromlittle32(x) ((void)0)
#define endian_fromlittle64(x) ((void)0)
#define endian_tolittle16_n(x,n) ((void)0)
#define endian_tolittle32_n(x,n) ((void)0)
#define endian_tolittle64_n(x,n) ((void)0)
#define endian_fromlittle16_n(x,n) ((void)0)
#define endian_fromlittle32_n(x,n) ((void)0)
#define endian_fromlittle64_n(x,n) ((void)0)
#else
#define endian_tolittle16(w) endian_swap16(w)
#define endian_tolittle32(w) endian_swap32(w)
#define endian_tolittle64(w) endian_swap64(w)
#define endian_fromlittle16(w) endian_swap16(w)
#define endian_fromlittle32(w) endian_swap32(w)
#define endian_fromlittle64(w) endian_swap64(w)
#define endian_tolittle16_n(w,n) endian_swap16_n(w,n)
#define endian_tolittle32_n(w,n) endian_swap32_n(w,n)
#define endian_tolittle64_n(w,n) endian_swap64_n(w,n)
#define endian_fromlittle16_n(w,n) endian_swap16_n(w,n)
#define endian_fromlittle32_n(w,n) endian_swap32_n(w,n)
#define endian_fromlittle64_n(w,n) endian_swap64_n(w,n)
#endif

#ifdef __BIG_ENDIAN__
#define endian_tobig16(x) ((void)0)
#define endian_tobig32(x) ((void)0)
#define endian_tobig64(x) ((void)0)
#define endian_frombig16(x) ((void)0)
#define endian_frombig32(x) ((void)0)
#define endian_frombig64(x) ((void)0)
#define endian_tobig16_n(x,n) ((void)0)
#define endian_tobig32_n(x,n) ((void)0)
#define endian_tobig64_n(x,n) ((void)0)
#define endian_frombig16_n(x,n) ((void)0)
#define endian_frombig32_n(x,n) ((void)0)
#define endian_frombig64_n(x,n) ((void)0)
#else
#define endian_tobig16(w) endian_swap16(w)
#define endian_tobig32(w) endian_swap32(w)
#define endian_tobig64(w) endian_swap64(w)
#define endian_frombig16(w) endian_swap16(w)
#define endian_frombig32(w) endian_swap32(w)
#define endian_frombig64(w) endian_swap64(w)
#define endian_tobig16_n(w,n) endian_swap16_n(w,n)
#define endian_tobig32_n(w,n) endian_swap32_n(w,n)
#define endian_tobig64_n(w,n) endian_swap64_n(w,n)
#define endian_frombig16_n(w,n) endian_swap16_n(w,n)
#define endian_frombig32_n(w,n) endian_swap32_n(w,n)
#define endian_frombig64_n(w,n) endian_swap64_n(w,n)
#endif

/**********************************************************/

#ifdef __cplusplus
}  /* extern "C" */
#endif

/**********************************************************/

#endif
