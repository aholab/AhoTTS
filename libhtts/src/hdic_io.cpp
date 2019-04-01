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
(C) 1999 TSR/Aholab - ETSII/IT Bilbao (UPV/EHU) // Euskaltel

Nombre fuente................ hdic_io.cpp
Nombre paquete............... aHoTTS
Lenguaje fuente.............. C++
Estado....................... -
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion: ............... Oscar Losada Abella
............................. Borja Etxebarria

Version  dd/mm/aa  Autor     Comentario
-------  --------  --------  ----------
1.1.1    08/10/01  Yon2.     Avoid dit warns
1.1.0    01/07/00  richie    Separated mutexes (bin!=txt)
1.0.2    01/07/00  richie    Win32 Multithread
1.0.1    22/06/00  richie    POSIX multithread
1.0.0    13/05/00  borja     uppercase hexa en hdicRefToTxt()
1.0.0    31/01/00  borja     codefreeze aHoTTS v1.0
0.1.0    15/12/99  borja     1a. version funcionando
0.0.0    16/11/99  Oscar     Version Inicial.

======================== Contenido ========================
<DOC>
</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include <string.h>
#include "hdic.hpp"
#include "httsmsg.h"
#include "chset.h"

/**********************************************************/

#ifdef __AHOTTS_MT__

#ifdef __OS_UNIX__
  pthread_mutex_t hdicmutex_txt=PTHREAD_MUTEX_INITIALIZER;
  pthread_mutex_t hdicmutex_bin=PTHREAD_MUTEX_INITIALIZER;
#endif
#ifdef __OS_WINDOWS__
  HANDLE hdicmutex_txt=NULL;
  HANDLE hdicmutex_bin=NULL;
#endif

#endif

KINDOF_BDEF(HDicDB);

/**********************************************************/

HDicDB::HDicDB()
{
	dbname=NULL;
	fileTxt=fileBin=NULL;
#ifdef __AHOTTS_MT__
#ifdef __OS_WINDOWS__
  hdicmutex_txt=CreateMutex(NULL,FALSE,NULL);
  hdicmutex_bin=CreateMutex(NULL,FALSE,NULL);
#endif
#endif

}

/**********************************************************/

HDicDB::~HDicDB()
{
	if (dbname) { free(dbname); dbname=NULL; }
	if (fileTxt) { fclose(fileTxt); fileTxt=NULL; }
	if (fileBin) { fclose(fileBin); fileBin=NULL; }
}

/**********************************************************/

BOOL HDicDB::create( const CHAR * dbn )
{
	assert(!dbname);

	dbname=strdup(dbn);
	CHAR *tmp=(CHAR*)malloc(sizeof(CHAR)*(strlen(dbname)+5));

	sprintf(tmp,"%s" HDIC_FNAMEEXT_TXT,dbname);
	ftxtCreate(tmp);
#ifdef HTTS_DEBUG
	if (!fileTxt) htts_warn("Can't use HDic database '%s'",tmp);
#endif
	sprintf(tmp,"%s" HDIC_FNAMEEXT_BIN,dbname);
	fbinCreate(tmp);
	if (!fileBin) htts_warn("Can't use HDic database '%s'",tmp);

	free(tmp);
	return TRUE;
}

/**********************************************************/

HDicRef HDicDB::search(const CHAR *str, char **exp)
{
	HDicRef refb=HDIC_REF_NULL;
	HDicRef reft=HDIC_REF_NULL;

	if (exp) *exp=NULL;  // inicializamos exp a NULL.

	if (!str) return refb;
	if (!(*str)) return refb;

	toklen=strlen(str);
	if (toklen>HDIC_MAX_STRLEN) return refb;  // si el token es muy largo, fuera

	tok=str;  // puntero a token
	strcpy(tokl, str);  // copiar token...
	chset_StrLower(tokl);  // ... y pasar a minusculas

	hitlen=0;  // inicializamos hitlen a 0.
	hitlonger=TRUE;  // primer_hitlen > hitlen actual, o sea >0

#ifdef __AHOTTS_MT__
#ifdef __OS_UNIX__
	pthread_mutex_lock (&hdicmutex_bin);
#endif
#ifdef __OS_WINDOWS__
	WaitForSingleObject (hdicmutex_bin,INFINITE);
#endif
#endif
	if (fileBin) refb=searchBin(exp);  // Primero hacemos la busqueda binaria

#ifdef __AHOTTS_MT__
#ifdef __OS_UNIX__
	pthread_mutex_unlock (&hdicmutex_bin);
#endif
#ifdef __OS_WINDOWS__
	ReleaseMutex(hdicmutex_bin);
#endif
#endif

#ifdef __AHOTTS_MT__
#ifdef __OS_UNIX__
	pthread_mutex_lock (&hdicmutex_txt);
#endif
#ifdef __OS_WINDOWS__
	WaitForSingleObject (hdicmutex_txt,INFINITE);
#endif
#endif

	if (fileTxt) reft=searchTxt(exp);  // y luego la de texto

#ifdef __AHOTTS_MT__
#ifdef __OS_UNIX__
	pthread_mutex_unlock (&hdicmutex_txt);
#endif
#ifdef __OS_WINDOWS__
	ReleaseMutex(hdicmutex_txt);
#endif
#endif

	// devolvemos la de texto si obtuvimos algo
	return (reft!=HDIC_REF_NULL) ? reft : refb;
}

/**********************************************************/

HDicRef HDicDB::searchFull(const CHAR *str, char **exp)
{
	HDicRef ref=search(str,exp);
	if (query(ref,HDIC_QUERY_MATCHLEN)) {  // si no es FULL
		ref=HDIC_REF_NULL;  // lo olvidamos
		if (exp && *exp) { free(*exp); *exp=NULL; }  // tambien olvidamos la expansion si la habia
	}
	return ref;
}

/**********************************************************/

#ifdef HTTS_IOTXT
const CHAR *HDicDB::hDicRefToTxt(HDicRef ref)
{
	CHAR buf[10];
	sprintf(buf,"%lX",(unsigned long)ref.bits);
	txtbuf=buf;
	return txtbuf;
}
#endif

/**********************************************************/

#ifdef HTTS_IOTXT
HDicRef HDicDB::txtToHDicRef(const CHAR *str)
{
	unsigned long x;
	if (!(str&&*str)) return HDIC_REF_NULL;
	if (!sscanf(str,"%lx",&x)) return HDIC_REF_NULL;
	HDicRef r;
	r.bits=x;
	return r;
}
#endif

/**********************************************************/





