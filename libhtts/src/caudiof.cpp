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
1.0.1    09/04/99  Borja     uso tipos UINT
0.0.0    30/07/96  Borja     codificacion inicial.

======================== Contenido ========================
<DOC>
Acceso a ficheros de sonido con clase CAudioFile.
Soporta formatos: Wav, aho (antiguo y nuevo), nist (Sphere) y HAK.
Tambien soporta cabeceras separadas en fichero de texto.

Los ficheros .HAK tienen una cabecera donde se puede meter
cualquier cosa. Hay una serie de campos definidos que
permiten usar el formato .HAK para almacenar otros parametros
en vez de audio (aunque tienen que ser valores DOUBLE). Por
ejemplo, ficheros de espectrograma, curvas de pitch, tramas LPC, etc.

----
Campo "SRate" (CAUDIO_SRATE).
Realmente se accede a el a traves del metodo getSRate(). No se debe
acceder directamente al campo, porque puede no coincidir con lo
que indique getSRate(),
Indica la frecuencia de muestreo en Hz.
----
Campo "NChan" (CAUDIO_NCHAN)
Realmente se accede a el a traves del metodo getNChan(). No se debe
acceder directamente al campo, porque puede no coincidir con lo
que indique getNChan(),
En el caso de ficheros de audio, indica el numero de canales de
audio presentes.
En el caso de ficheros de parametros, indica la dimension de cada
trama de parametros (Ver comentarios importantes en "FrameRateMs").
----
Campo "OriginMs" (CAUDIO_ORIGINMS).
Fija el punto de origen del fichero, en milisegundos. Su valor por defecto
es 0. Para el caso de audio, sera la posicion de la primera muestra.
Para el caso de ficheros de parametros, la posicion central de la
primera trama de analisis.

----
Campo "ParamSet" (CAUDIO_PARAMSET).
Permite detectar si se trata de un fichero de audio o
de un fichero de parametros. En los ficheros de
audio, este campo no esta definido.
Por ejemplo, los ficheros de pitch pueden tomar el
valor "Pitch" en este campo para identificarlos como
ficheros de parametros, y ademas de curvas de pitch.

-----
Campo "FrameRateMs" (CAUDIO_FRAMERATEMS)
Da el desplazamiento de la ventana de analisis (longitud de trama).
En el caso de ficheros de audio no se utiliza, pero de aparecer
deberia tener el valor 1/SRate.
En el caso de ficheros de parametros por el contrario si tiene
importancia: Si FrameRateMs es distinto de cero, indica el
periodo en ms entre una trama y la siguiente. Si FrameRateMs
es cero o no esta definido (es decir, valor por defecto 0), entonces
cada trama de datos va precedida por un valor que indica el
instante de tiempo (en ms) en que se ha efectuado dicho analisis.
Notese que getNChan() devuelve la dimension de la trama de parametros
completa, incluyento este valor de tiempo. Este valor siempre es el
primero de cada trama.

-----
Otros campos:

Eje y (elementos del array, frecuencia, etc.):
DimMin  
DimMax
DimUnits

Eje Z (valores, alturas...):
ValMin
ValMax
ValUnits

</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>

#include "uti.h"

#include "caudiox.hpp"

#ifdef __OS_MSDOS__
#include <io.h>
#include <fcntl.h>
#endif

/**********************************************************/

CAudioFile::CAudioFile( VOID )
{
	myFile=NULL;
	afFormat=NULL;

	txhUpdatable=FALSE;
	updatable=FALSE;
	trackMarks=TRUE;
};

/**********************************************************/

BOOL CAudioFile::OK( VOID ) const
{
	if (!myOpts.OK()) return FALSE;
	if (!myInfo.OK()) return FALSE;
	if (!myMarks.OK()) return FALSE;
	if (!mySamp.OK()) return FALSE;
	if (afFormat) if (!afFormat->OK()) return FALSE;
	return TRUE;
}

/**********************************************************/

VOID CAudioFile::setSRate( DOUBLE srate, BOOL adjustMarks )
{
	mySRate=srate;
	if (adjustMarks) myMarks.adjustSRate(srate);
	else myMarks.setSRate(srate);
}

/**********************************************************/

VOID CAudioFile::open( const CHAR* fName, const CAudioFile &fa,
		const CHAR* options, ... )
{
	va_list argptr;
	va_start(argptr, options);
	open(fName,fa,options,argptr);
	va_end(argptr);
}

/**********************************************************/

VOID CAudioFile::open( const CHAR* fName, const CAudioFile &fa,
		const CHAR* options, va_list argptr )
{
	KVStrList tmp(options,"=", argptr);

	open(fName,fa,tmp);
}

/**********************************************************/

VOID CAudioFile::open( const CHAR* fName, const CAudioFile &fa,
		const KVStrList &pr )
{
	KVStrList p(pr);

	p.add(CAUDIO_FFORMAT,fa.getFFormat());
	p.add(CAUDIO_SRATE,fa.getSRate());
	p.add(CAUDIO_SAMPTYPE,fa.getSampType_a());
	p.add(CAUDIO_NCHAN,(LONG)fa.getNChan());
	p.add(CAUDIO_BIGENDIAN,fa.getBigEndian());

	open(fName,"w",p);
}

/**********************************************************/

VOID CAudioFile::open( const CHAR* fName, const CHAR* mode,
		const CHAR* options, ... )
{
	va_list argptr;
	va_start(argptr, options);
	open(fName,mode,options,argptr);
	va_end(argptr);
}

/**********************************************************/

VOID CAudioFile::open( const CHAR* fName, const CHAR* mode,
		const CHAR* options, va_list argptr )
{
	KVStrList tmp(options,"=", argptr);

	open(fName,mode,tmp);
}

/**********************************************************/

VOID CAudioFile::open( const CHAR* fName, const CHAR* mode, const KVStrList &pr)
{
	INT txhPos;
	BOOL stdinout=FALSE;

	if (myFile!=NULL)
		die_beep("CAudioFile: close file %s before opening another one",
				(const char *)myOpts.val(CAUDIO_FNAME));
	cdie_beep(!mode,"CAudioFile: invalid open() mode (NULL)");

	myOpts.clear();
	// opciones por defecto
	{ KVStrList tmp(CAUDIO_OPTS_DEFAULT);	myOpts.add_mv(tmp,TRUE); }
	// entorno
	char *s=getenv(CAUDIO_ENVOPTS);
	if (s) { KVStrList tmp(s);	myOpts.add_mv(tmp,TRUE); }
	// parametro {pr} (options)
	myOpts.add(pr);
	// si el fichero es "-", es stdin (r) o stdout (w), y solo puede ser RAW
	if (!strcmp(fName,"-")) {
		myOpts.add(CAUDIO_FFORMAT,"Raw");
		stdinout=TRUE;
	}
	// verificamos algunos, que sean validos
	SampType_a2i(myOpts.val(CAUDIO_SAMPTYPE,SAMPTYPE_STR_PCM16));
	myOpts.dval(CAUDIO_SRATE,8000);
	myOpts.lval(CAUDIO_NCHAN,1);


	myOpts.add(CAUDIO_FNAME,fName);

	const String &format=myOpts.val(CAUDIO_FFORMAT);

	txhUpdatable=FALSE;
	txhPos=0;
	if (format.contains("Txh")) {  // usar fichero TxtHdr?
		txhGetName(fName);  // determinar nombre
		txhLoad(mode);  // leer si se puede, etc etc
		// tiene prioridad esta informacion (format empieza con Txh)?
		txhPos = format.contains("Txh",0) ? -1 : 1;  // si=-1
	}
	afFormat = getAFFormatObject(fName,mode);  // crear formato adecuado
	// sustituir formato por el definitivo
	if (txhPos==1) myOpts.add(CAUDIO_FFORMAT, String(afFormat->fFormat())+"-Txh");
	else if (txhPos==-1) myOpts.add(CAUDIO_FFORMAT, "Txh-"+String(afFormat->fFormat()));
	else myOpts.add(CAUDIO_FFORMAT, afFormat->fFormat());

	updatable = TRUE; // solo en mode "r" es FALSE
	BOOL override = (txhPos>0);  // Solo override si Txh esta, y no al principio
	BOOL write=FALSE;
	LONG nSamp=0;

	CHAR xmode= *mode;
	if (xmode=='M') {  // r/w existente, o "r" si write protected
		myFile = fopen(fName,"r+b");
		if (myFile) { fclose(myFile); xmode='m'; myFile=NULL; }
		else xmode='r';
	}

	// abrir el fichero en modo adecuado y procesar la cabecera
	switch (xmode) {
	case 'r':  // r existente
		updatable = FALSE;
		if (stdinout) {
			myFile=stdin;
#ifdef __OS_MSDOS__
			setmode(fileno(stdin), O_BINARY); // MSDOS: asegurar que utilizamos fichero binario
#endif
		}
		else myFile = xfopen(fName,"rb");
		nSamp=afFormat->HdrR(myFile,myOpts,override);
		break;
	case 'w':  // rw nuevo
		if (stdinout) {
			myFile=stdout;
			updatable=FALSE;
#ifdef __OS_MSDOS__
			setmode(fileno(stdout), O_BINARY); // MSDOS: asegurar que utilizamos fichero binario
#endif
		}
		else myFile = xfopen(fName,"w+b");
		afFormat->HdrG(myOpts,override);
		write=TRUE;
		break;
	case 'm':  // r/w existente
		cdie_beep(stdinout,"CAudioFile: can't open stdin/stdout in modify (m) mode");
		myFile = xfopen(fName,"r+b");
		nSamp=afFormat->HdrR(myFile,myOpts,override);
		break;
	case 'a':  // r/w existente o nuevo
		cdie_beep(stdinout,"CAudioFile: can't open stdin/stdout in append (a) mode");
		myFile = fopen(fName,"r+b");
		if (myFile) nSamp=afFormat->HdrR(myFile,myOpts,override);  // lee si existe
		else { /* si no existe... */
			myFile = xfopen(fName,"w+b");  // ...crearlo
			afFormat->HdrG(myOpts,override);
			write=TRUE;
		}
		break;
	default:
		die_beep("CAudioFile: invalid open() mode (%c)",xmode);
		break;
	}

	mySampType = SampType_a2i(myInfo.val(CAUDIO_SAMPTYPE,
			myOpts.val(CAUDIO_SAMPTYPE,SAMPTYPE_STR_PCM16)));
	myNChan = (UINT)myInfo.lval(CAUDIO_NCHAN,myOpts.lval(CAUDIO_NCHAN,1));
	mySRate = myInfo.dval(CAUDIO_SRATE,myOpts.dval(CAUDIO_SRATE,8000));
	myMarks.setSRate(mySRate);
  // big/little endian default value: OS dependant:
	myBigEndian = myInfo.bval(CAUDIO_BIGENDIAN,myOpts.bval(CAUDIO_BIGENDIAN,CAUDIO_BIGENDIAN_DEFAULT));

	myOpts.erase(CAUDIO_SAMPTYPE);
	myOpts.erase(CAUDIO_SRATE);
	myOpts.erase(CAUDIO_NCHAN);
	myOpts.erase(CAUDIO_NSAMPLES);
	myOpts.erase(CAUDIO_BIGENDIAN);  

//	myInfo.erase(CAUDIO_SAMPTYPE);
//	myInfo.erase(CAUDIO_SRATE);
//	myInfo.erase(CAUDIO_NCHAN);
//	myInfo.erase(CAUDIO_NSAMPLES);
//	myInfo.erase(CAUDIO_BIGENDIAN);

	if (write) afFormat->HdrW(myFile,nSamp);
	mySamp.attach(myFile, mySampType, myNChan, stdinout?-2:nSamp);
	mySamp.setendian(myBigEndian?-1:1);

	if (myInfo.contains(CAUDIO_NSAMPLES)) 
    myInfo.add(CAUDIO_NSAMPLES,getNSamples());
}

/**********************************************************/

VOID CAudioFile::set_updatable( BOOL update )
{
	updatable=update;
}

/**********************************************************/

BOOL CAudioFile::update( VOID )
{
	if (!myFile) return FALSE;

	if (myInfo.contains(CAUDIO_SAMPTYPE))
		myInfo.add(CAUDIO_SAMPTYPE,SampType_i2a(getSampType()));
	if (myInfo.contains(CAUDIO_SRATE)) myInfo.add(CAUDIO_SRATE,getSRate());
	if (myInfo.contains(CAUDIO_BIGENDIAN)) myInfo.add(CAUDIO_BIGENDIAN,getBigEndian());
	if (myInfo.contains(CAUDIO_NCHAN)) myInfo.add(CAUDIO_NCHAN,(LONG)getNChan());
	if (myInfo.contains(CAUDIO_NSAMPLES)) myInfo.add(CAUDIO_NSAMPLES,getNSamples());

	if (updatable) {
		LONG pos = mySamp.getPos();  // salva posicion muestra
		LONG n = mySamp.getN();
		afFormat->HdrW(myFile,n);  // escribe cabecera
		mySamp.attach(myFile, mySamp.getSampType(), mySamp.getNChan(), n);  // resincroniza muestras
		mySamp.setPos(pos);  // recupera posicion de muestra actual
	}
	// si hay fichero TxtHdr asociado y es actualizable
	if (txhUpdatable) { txhWrite(); return TRUE; }
	if (updatable) return TRUE;
	return FALSE;
}

/**********************************************************/

VOID CAudioFile::close( VOID )
{
	if (!myFile) return;

	update();
	mySamp.attach(NULL,0);  // detach
	if (myFile) {
		if ((myFile!=stdin)&&(myFile!=stdout))
			xfclose(myFile);
		myFile = NULL;
	}
	if (afFormat) { delete(afFormat); afFormat = NULL; }
	// borramos listas
	myMarks.clear();
	myInfo.clear();
	myOpts.clear();
	updatable=txhUpdatable=FALSE;
}

/**********************************************************/

VOID CAudioFile::sndCopy( CAudioFile &fromf, LONG n )
{
#define BUFLEN 1024
	if ((fromf.getSampType()==getSampType())&&
		(fromf.getSampType()==SAMPTYPE_PCM16)&&
		(fromf.getNChan()==getNChan())) {
		while (n>=BUFLEN) {
			INT16 *v=fromf.getChAll_i16(BUFLEN);
			setBlkChAll(v,BUFLEN);
			n-=BUFLEN;
		}
		if (n) {
			INT16 *v=fromf.getChAll_i16((UINT)n);
			setBlkChAll(v,(UINT)n);
		}
	}
	else {
		UINT nch=getNChan();
		if (nch) {
			nch--;
			while (n>=BUFLEN) {
				DOUBLE *v=fromf.getChAll_d(BUFLEN);
				setBlkChRange(v,BUFLEN,0,nch);
				n-=BUFLEN;
			}
			if (n) {
				DOUBLE *v=fromf.getChAll_d((UINT)n);
				setBlkChRange(v,(UINT)n,0,nch);
			}
		}
	}
}

/**********************************************************/
