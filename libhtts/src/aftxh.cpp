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
1.0.2	 20/09/07  Ibon		 Eliminar el máximo de 20 columnas en el paso de un caudio a un fichero de texto
1.0.1    09/04/99  Borja     uso tipos UINT
0.0.1    08/08/98  Borja     bug en txhLoads
0.0.1    08/08/97  Borja     bug en parser de marcas
0.0.0    15/07/96  Borja     codificacion inicial.

======================== Contenido ========================
$$$ todo:
* revisar lo que se escribe para saber si meterlo en varias lineas o
en un campo raw, o entre comillas.
*leer marcas
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include <stdlib.h>
#include <string.h>

#include "uti.h"
#include "assert.h"
#include "caudiox.hpp"
#include "wrkbuff.h"

/**********************************************************/

#define INFO_MARKS     "Marks"

/**********************************************************/
// linea mas larga a leer
#define TXHLINE_MAXLEN 4096
// cada cuanto poner comentario indicador de muestra en la que estamos
#define DEF_TXTCOMMENT 100
// cantidad maxima de columnas (muestras de muestra-multi-canal) por linea
#define DEF_TXTCOLUMNS 20


#define TXH_FIRM  "# TXH"
#define TXH_VMAYOR 1
#define TXH_VMINOR 0

/**********************************************************/

LONG txtReadAllInfo( FILE *f, KVStrList &info, Mark1DList &marks,
		const CHAR *txhName, BOOL verify_fformat )
{
	int tl;
	WRKBUFF(TXHLINE_MAXLEN);

	if (verify_fformat) {
		xfgetln(_wrkbuff,TXHLINE_MAXLEN,f,&tl);
		cdie_beep(tl,"CAudioFile: line to long in %s (1)", txhName);
		size_t len = strlen(TXH_FIRM);
		if ((strlen(_wrkbuff)<=len)||(strncmp(_wrkbuff,TXH_FIRM,len)))
			die_beep("CAudioFile: not a valid audio-info file (%s)",txhName);
		int major, minor;
		sscanf(_wrkbuff+len,"%i.%i",&major, &minor);
		if (TXH_VMAYOR*100+TXH_VMINOR<major*100+minor)
			die_beep("CAudioFile: invalid version in %s (%i.%i>%i.%i)",
					txhName, major, minor, TXH_VMAYOR, TXH_VMINOR);
	}

	LONG line = 0;
	enum { GET_ENTRY, GET_MULTI} mode = GET_ENTRY;
	String name, val;

	while (xfgetln_filt(_wrkbuff,TXHLINE_MAXLEN,f,TRUE,2,FALSE,&tl)) {
		line++;
		cdie_beep(tl,"CAudioFile: line to long in %s (%ld)", txhName, line);
		if (*_wrkbuff=='\0') continue;

		/*{*/
		if (*_wrkbuff=='}') {  // si es fin de modo raw
			cdie_beep((mode!=GET_MULTI),"CAudioFile: invalid line in %s (%ld)", txhName, line);
			info.add(name,val);  // salva entrada
			mode = GET_ENTRY;
			continue;
		}

		if (mode==GET_MULTI) { val+="\n"; val+=_wrkbuff; continue; }

		char *s=strchr(_wrkbuff,'=');
		if (!s) { name=_wrkbuff;  val=""; }
		else { name=String(_wrkbuff,(size_t)(s-_wrkbuff)); val=(s+1); }
		cdie_beep(!name.length(),"CAudioFile: invalid line in %s (%ld)", txhName, line);

		if (!strcmp(name,INFO_MARKS)) { // son las marcas?
			if (!strcmp(val,"{}")) continue;
			cdie_beep(strcmp(val,"{"), "CAudioFile: invalid line in %s (%ld)", txhName, line); /* } */
			marks.finput(f,FALSE,txhName,line,_wrkbuff,TXHLINE_MAXLEN,'}');
			continue;
		}

		if (!strcmp(name,INFO_SAMPLES)) { // es el vector de muestras
			cdie_beep(val!="{", "CAudioFile: invalid line in %s (%ld)", txhName, line); /* } */
			WRKBUFF_FREE();
			return line;
		}


		if (!strcmp(val,"{}")) val="";
		if (!strcmp(val,"{")) {	mode=GET_MULTI; val=""; /*}*/ }
		else info.add(name,val);  // salva entrada
	}
	cdie_beep(mode==GET_MULTI, "CAudioFile: unspected EOF in %s (%ld)", txhName, line);

	WRKBUFF_FREE();

	if (!line) return -1;
	else return 0;
}

/**********************************************************/

VOID hdrRead( CAudioFile *fa, const CHAR *txhName )
{
	FILE* f;
	LONG b;

	f = xfopen(txhName,"r");

	b=txtReadAllInfo(f,fa->info(), fa->marks(), txhName, TRUE );
	cdie_beep(b!=0,"CAudioFile: sample data (%s) not allowed in TXH files",
			INFO_SAMPLES);

	xfclose(f);
}

/**********************************************************/
/* escribe s al fichero, entre comillas si tiene espacios,
y mete \n final*/

VOID writeField( FILE* f, const CHAR* s )
{
	size_t len;

	if (!s) return;
	len = strlen(s);
	if (!len) return;

	if (strchr(s,'\n')) {
		fprintf(f,"{\n");
		fwrite(s,1,len,f);
		fprintf(f,"\n}\n\n");
		return;
	}

	if (strcspn(s," \t")==len) fprintf(f,"%s\n",s);
	else fprintf(f,"\"%s\"\n",s);
}

/**********************************************************/

VOID CAudioFile::txhLoad( const CHAR *mode )
{
	FILE* f;
	const char * txhName = myOpts.val(CAUDIO_TXHNAME);

	const char *tmode = (mode[0] && mode[1]) ? mode+1 : mode;
	f = fopen(txhName,"r");
	if (f) fclose(f);

	txhUpdatable = TRUE;
	switch (*tmode) {
	case 'r':
		txhUpdatable = FALSE;
		if (f || (tmode!=mode)) hdrRead(this,txhName); // existe o modo es "?r"
		break;
	case 'w':
		myInfo.add(CAUDIO_NSAMPLES,(LONG)0); // crea entrada NSamples
		break;
	case 'm':
		if (f || (tmode!=mode)) hdrRead(this,txhName); // existe o modo es "?m"
		else myInfo.add(CAUDIO_NSAMPLES,(LONG)0); // crea entrada NSamples
		break;
	case 'M':
		if (f || (tmode!=mode)) hdrRead(this,txhName); // existe o modo es "?m"
		else myInfo.add(CAUDIO_NSAMPLES,(LONG)0); // crea entrada NSamples
		f = fopen(txhName,"r+b");
		if (f) fclose(f); else txhUpdatable=FALSE;
		break;
	case 'a':
		if (f) hdrRead(this,txhName);
		else myInfo.add(CAUDIO_NSAMPLES,(LONG)0); // crea entrada NSamples
		break;
	default:
		die_beep("CAudioFile: invalid open() mode (%c)",*tmode);
	}
}

/**********************************************************/

VOID CAudioFile::txtWriteAllInfo( FILE *f, BOOL force_nsamp )
{
	Lix p;
	String s;
	WRKBUFF(50);

	if (!myInfo.contains(CAUDIO_NSAMPLES) && force_nsamp) {
		sprintf(_wrkbuff,CAUDIO_NSAMPLES " = %ld",(long)getNSamples()); fprintf(f,"%s\n",_wrkbuff);
	}
	if (!myInfo.contains(CAUDIO_SRATE)) {
		sprintf(_wrkbuff,CAUDIO_SRATE " = %g",(double)getSRate()); fprintf(f,"%s\n",_wrkbuff);
	}
	if (!myInfo.contains(CAUDIO_SAMPTYPE)) {
		sprintf(_wrkbuff,CAUDIO_SAMPTYPE" = %s",getSampType_a()); fprintf(f,"%s\n",_wrkbuff);
	}
	if (!myInfo.contains(CAUDIO_NCHAN)) {
		sprintf(_wrkbuff,CAUDIO_NCHAN " = %g",(double)getNChan()); fprintf(f,"%s\n",_wrkbuff);
	}
	if (!myInfo.contains(CAUDIO_BIGENDIAN)) {
		sprintf(_wrkbuff,CAUDIO_BIGENDIAN " = %s",getBigEndian()?"yes":"no"); fprintf(f,"%s\n",_wrkbuff);
	}

	for (p=myInfo.first(); p!=0; p=myInfo.next(p)) {
			fprintf(f,"%s = ",(const char *)myInfo.itemkey(p));
			writeField(f,myInfo.itemval(p));
	}

	if (myMarks.length()>0) {
		fprintf(f,"%s = {\n",INFO_MARKS);
		myMarks.foutput(f,TRUE,FALSE,FALSE);  /* indent, pos/span:label, samples */
		fprintf(f,"}\n");
	}
	WRKBUFF_FREE();
}

/**********************************************************/

VOID CAudioFile::txhWrite( VOID )
{
	const char * txhName = myOpts.val(CAUDIO_TXHNAME);
	String s;

	/* abrimos el fichero en binario. Nada de modificar lo que
	escribimos como hace el msdos/ventanuko al abrir en modo texto */
	FILE *f = xfopen(txhName,"wb");

	fprintf(f,"%s %i.%i - AhoLab\n\n",TXH_FIRM,TXH_VMAYOR,TXH_VMINOR);

	txtWriteAllInfo(f,FALSE);

	xfclose(f);
}

/**********************************************************/

BOOL CAudioFile::openFromTxt( const CHAR *fname, FILE *f, const KVStrList &Opts )
{
	Mark1DList mrk;
	KVStrList inf;
	KVStrList opt(Opts);
	LONG line=txtReadAllInfo(f,inf,mrk,"file",FALSE);

	if (inf.contains(CAUDIO_SRATE)) opt.add(CAUDIO_SRATE,inf.val(CAUDIO_SRATE));
	if (inf.contains(CAUDIO_SAMPTYPE)) opt.add(CAUDIO_SAMPTYPE,inf.val(CAUDIO_SAMPTYPE));
	if (inf.contains(CAUDIO_NCHAN)) opt.add(CAUDIO_NCHAN,inf.val(CAUDIO_NCHAN));
	if (inf.contains(CAUDIO_BIGENDIAN)) opt.add(CAUDIO_BIGENDIAN,inf.val(CAUDIO_BIGENDIAN));

	open(fname,"w",opt);

	myInfo = inf;
	myMarks = mrk;

	if (line<=0) { WRKBUFF_FREE(); return FALSE; }

	int tl;

	WRKBUFF(TXHLINE_MAXLEN);
	short int si;
	double d;
	INT stype;

	stype=getSampType();

	while (xfgetln_filt(_wrkbuff,TXHLINE_MAXLEN,f,TRUE,0,FALSE,&tl)) {
		line++;

		cdie_beep(tl,"CAudioFile: line to long in file (%ld)", line);
		CHAR *s=strtok(_wrkbuff," \t");

		if (!s) continue;
		if (*s=='\0') continue;

		/*{*/
		if (*s=='}') { WRKBUFF_FREE(); return TRUE; } // si es fin de Samples

		while (s) {  // varias "muestras" por linea
			if (stype==SAMPTYPE_PCM16) {
				cdie_beep(str2si(s,&si),"CAudioFile: invalid PCM16 sample value (%s)",s);
				setNoCh((INT16)si);
			}
			else if (stype==SAMPTYPE_PCM8U) {
				cdie_beep(str2si(s,&si),"CAudioFile: invalid PCM8U sample value (%s)",s);
				setNoCh((INT16)((si-128)<<8));
			}
			else if ((stype==SAMPTYPE_FLOAT32)||(stype==SAMPTYPE_FLOAT64)) {
				cdie_beep(str2d(s,&d),"CAudioFile: invalid FLOAT32 sample value (%s)",s);
				setNoCh(d);
			}
			else die_beep("CAudioFile: sample type %s not supported from txt file",
					getSampType_a());
			s=strtok(NULL," \t");
		}
	}

	WRKBUFF_FREE();

	return FALSE;
}

/**********************************************************/

BOOL CAudioFile::openFromTxt( const CHAR *fname, FILE *f,
		const CHAR* options, ... )
{
	va_list argptr;
	va_start(argptr, options);
	BOOL b=openFromTxt(fname,f,options,argptr);
	va_end(argptr);

	return b;
}

/**********************************************************/

BOOL CAudioFile::openFromTxt( const CHAR *fname, FILE *f,
		const CHAR* options, va_list argptr )
{
	KVStrList tmp(options,"=", argptr);

	return openFromTxt(fname,f,tmp);
}

/**********************************************************/

VOID CAudioFile::writeToTxt( FILE *f )
{
	LONG whence, i, j, n;
	INT nch;
	LONG cols;

	txtWriteAllInfo(f,TRUE);

	whence = myOpts.lval(CAUDIO_TXTCOMMENT,DEF_TXTCOMMENT);
	cols = myOpts.lval(CAUDIO_TXTCOLUMNS,DEF_TXTCOLUMNS);

	fprintf(f,INFO_SAMPLES " = {\n");
	n = getNSamples();
	nch = getNChan();
	j=whence;
	INT stype = getSampType();
	INT16 i16;
	DOUBLE d;
	LONG opos=getPos();
	setPos(0);
	LONG nn=n*nch;

	for (i=0; i<nn; i++) {
		switch (stype) {
		case SAMPTYPE_PCM8U: getNoCh(i16); fprintf(f,"%4d",(int)((i16>>8)+128)); break;
		case SAMPTYPE_PCM16: getNoCh(i16); fprintf(f,"%6d",(int)i16); break;
		case SAMPTYPE_FLOAT32:
		case SAMPTYPE_FLOAT64: getNoCh(d); fprintf(f,"%g",(double)d); break;
		default:
			die_beep("CAudioFile: sample type %s not supported for txt file",
				getSampType_a());
		}

		INT r=(INT)((i+1)%nch);
		if (!r) {
			if (whence && (j==whence)) { fprintf(f, "    ; %ld",(long)(i/nch)); j=0; }
			j++;
			fprintf(f,"\n");
		}
		else {
			fprintf(f," ");
//1.0.2	 20/09/07  Ibon
/*			if (r<cols) fprintf(f," ");
			else fprintf(f,"\n"); */
		}
	}

	fprintf(f,"}\n");
	setPos(opos);
}

/**********************************************************/
