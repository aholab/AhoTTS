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
Copyright: 1996 - Grupo de Voz (DAET) ETSII/IT-Bilbao

Nombre fuente................ -
Nombre paquete............... -
Lenguaje fuente.............. C++ (Borland C/C++ 3.1)
Estado....................... desarrollo
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Borja Etxebarria

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
0.0.2    30/08/98  Borja     split en varios modules mark_?.cpp

======================== Contenido ========================
ver mark.cpp
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include "mark.hpp"
#include "uti.h"
#include "wrkbuff.h"

/**********************************************************/
/* uso interno: salta espacios/tabs/comillas iniciales */

PRIVATE const CHAR * strjumpstart( const CHAR * s )
{
	while ((*s==' ')||(*s=='\t')) s++;
	if (*s=='"') s++;
	return s;
}

/**********************************************************/
/* uso interno: salta espacios/tabs/comillas finales.
detecta punto final */

PRIVATE const CHAR * strjumpend( const CHAR * s )
{
	const CHAR *ss=s+strlen(s);
	while (ss>s) {
		CHAR ch=*(--ss);
		if ((ch==' ')||(ch=='\t')) continue;
		if (ch!='\"') ss++;
		break;
  }
	return ss;
}

/*<DOC>*/
/**********************************************************/
/* Imprime como texto la lista de marcas en el fichero {f}.
Una marca por linea.
Si {indent} es TRUE, se utiliza una sangria de dos espacios
en cada linea.
Si {rangespan} es TRUE, el rango no se imprime como rango
sino como punto final.
Si {ms} es TRUE, se utilizan milisegundos en vez de muestras.

Formato de la linea:
pos/span : label
o si {rangespan} es TRUE:
pos,posfinal : label
(notese que se utilza "," en vez de "/" para separar {pos}
de {posfinal}. Asi se distingue del formato pos/span.

La funcion {devuelve} TRUE si todo va bien, o FALSE si hay
algun error al escribir al fichero.

foutput(f,indent,rangespan,ms) es una funcion de bajo nivel.
Normalmente se utilizaran las funciones foutput(f,rangespan) y
foutput_ms(f,rangespan), que llaman directamente a esta, volcando las
marcas sin sangrar (indent=FALSE), la primera en muestras y la segunda
en milisegundos. El parametro {rangespan} es opcional, con valor FALSE
por defecto */

BOOL Mark1DList::foutput( FILE *f, BOOL indent, BOOL rangespan, BOOL ms )
/*</DOC>*/
{
	BOOL ret=TRUE;

	for (Lix p=first(); p!=0; p=next(p)) {
		if (indent) ret=ret&&EOF!=fprintf(f,"  ");
		if (ms) ret=ret&&EOF!=fprintf(f,"%.12g",(double)getpos_ms(p));
		else ret=ret&&EOF!=fprintf(f,"%li",(long)pos(p));
		if (span(p)!=0) {
			if (rangespan) {
				if (ms) ret=ret&&EOF!=fprintf(f,",%.12g",(double)(getpos_ms(p)+getspan_ms(p)));  // como rango (,)
				else ret=ret&&EOF!=fprintf(f,",%li",(long)(pos(p)+span(p)));  // como rango (,)
			}
			else {
				if (ms) ret=ret&&EOF!=fprintf(f,"/%.12g",(double)getspan_ms(p));  // como span (/)
				else ret=ret&&EOF!=fprintf(f,"/%li",(long)span(p));  // como span (/)
			}
		}
		if (label(p)!="") {
			const CHAR *s=label(p);
			BOOL quote=(s!=strjumpstart(s))||(s+strlen(s)!=strjumpend(s));
			ret=ret&&EOF!=fprintf(f," : %s%s%s",
				quote ? "\"" : "",s, quote ? "\"" : "");
		}
		ret=ret&&EOF!=fprintf(f,"\n");
	}
	return ret;
}

/*<DOC>*/
/**********************************************************/
/* Permite leer marcas de un fichero {f}. Las marcas leidas
se anyaden al final de la lista actual.
El fichero tiene el formato de texto que genera foutput().
Con {ms} se puede indicar que el fichero contiene valores
en milisegundos en lugar de en muestras.

{fileName} Es el nombre del fichero. Solo se usa para
los mensajes de error (error en el fichero 'tal').

{line} es el numero de la linea actual del fichero (0 si
se va a procesar la primera linea. Se utiliza en los
mensajes de error (error, marca invalida en linea tal).
Ademas al terminar de leer marcas se devuelve en este mismo
parametro el numero de linea de la ultima marca).

{buf} Es el buffer de trabajo donde se van leyendo las
lineas del fichero. Debe ser suficientemente grande como
para almacenar una linea.

{buflen} es la longitud en bytes del buffer {buf}.

{eofch} es un caracter que se utiliza para indicar fin
de marcas. Si es cero, se siguen leyendo marcas hasta la ultima
linea del fichero (EOF). Si es distinto de cero, si se encuentra
en el fichero una linea que empiece por este caracter, se termina
de leer marcas.

La funcion finput(f,ms,finename,line,buf,buflen,eofch) es de
bajo nivel. Normalmente se utilizara finput(f) y finput_ms(f)
para leer un fichero hasta llegar al final del mismo (EOF).
*/

VOID Mark1DList::finput( FILE* f,	BOOL ms, const CHAR *fileName, LONG &line, CHAR*buf, INT buflen, INT eofch )
/*</DOC>*/
{
	int tl;

	while (xfgetln_filt(buf,buflen,f,TRUE,0,FALSE,&tl)) {
		line++;
		cdie_beep(tl,"Mark1DList: line to long in %s (%ld)", fileName, (long)line);
		CHAR *s=buf;
		while ((*s==' ')||(*s=='\t')) s++;  //saltar espacios/tab iniciales
		if (*s=='\0') continue;

		if (eofch && (*s==eofch)) return;  // caracter que marca fin de marcas

		long int mpos, mspan=0;
		double mpos_ms, mspan_ms=0;
		int n;
		if (ms) n = str2d(s,&mpos_ms);
		else n = str2li(s,&mpos);
		cdie_beep(n<0,"Mark1DList: error reading marks from %s (%ld), invalid value (%s)",fileName,line,s);
    if (!n) n=strlen(s);
		s+=n;  while ((*s==' ')||(*s=='\t')) s++;
		if ((*s==',')||(*s=='/')) {
			BOOL range=(*s==',');
			s++; while ((*s==' ')||(*s=='\t')) s++;
			if (ms) n = str2d(s,&mspan_ms);
			else n = str2li(s,&mspan);
			cdie_beep(n<0,"Mark1DList: error reading marks from %s (%ld), invalid span value (%s)",fileName,line,s+1);
			if (!n) n=strlen(s);
			if (range) {
				if (ms) mspan_ms=mspan_ms-mpos_ms;
				else mspan=mspan-mpos;
			}
			s+=n; while ((*s==' ')||(*s=='\t')) s++;
			if (*s==':') {
				s=(char*)strjumpstart(s+1);
				*(char*)strjumpend(s)='\0';
				if (ms) append_ms(mpos_ms,mspan_ms,s); else append(mpos,mspan,s);
			}
			else {
				cdie_beep(*s,"Mark1DList: error reading marks from %s (%ld), invalid info (%s)",fileName,line,s);
				if (ms) append_ms(mpos_ms,mspan_ms); else append(mpos,mspan);
			}
		}
		else if (*s==':') { 
			s=(char*)strjumpstart(s+1);
			*(char*)strjumpend(s)='\0';
			if (ms) append_ms(mpos_ms,s); else append(mpos,s); 
		}
		else {
			cdie_beep(*s,"Mark1DList: error reading marks from %s (%ld), invalid info (%s)",fileName,line,s);
			if (ms) append_ms(mpos_ms); else append(mpos);
		}
	}

	cdie_beep(eofch,"Mark1DList: unspected EOF in %s (%ld)", fileName, line);
}

/**********************************************************/

VOID Mark1DList::finput( FILE* f, BOOL ms )
{
#define MAXLEN 4096
	WRKBUFF(MAXLEN);
	LONG line=0;
	finput(f,ms,"file",line,_wrkbuff,MAXLEN,0);
	WRKBUFF_FREE();
#undef MAXLEN
}

/**********************************************************/
