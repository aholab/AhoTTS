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
Buscar  "TypeCheck" para encontrar todos los
sitios en los que se debe anyadir codigo para manejar un
nuevo tipo de fichero.
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include "caudiox.hpp"
#include "uti.h"

/**********************************************************/

AFFormat * CAudioFile::getAFFormatObject( const CHAR *fname,
		const CHAR *mode )
{
#define RETURNIF(type) if (format.contains(type::formatStr())) return new type(this)
#define TESTFILE(type)  if (type::testFile(f)) { fclose(f); return new type(this); }

	FILE* f;
	const String& format = myOpts.val(CAUDIO_FFORMAT);
	if (!format.contains("Auto")) { // si no aparece "Auto"
		// Primero probamos todos los tipos de ficheros conocidos
		RETURNIF(AFWav);
		RETURNIF(AFRaw);
		RETURNIF(AFAho2);
		RETURNIF(AFAho1);
		RETURNIF(AFTei);
    RETURNIF(AFNist);
    RETURNIF(AFHak);    
		// TypeCheck - poner aqui encima nuevos tipos

		die_beep("CAudioFile: invalid file format string (%s)",
				(const char *)format);
	}

	// auto deteccion del tipo de fichero
	f = fopen(fname,"rb");
	const CHAR *ext= fname + path_extpos(fname);

	if ((*mode=='r')||(*mode=='m')||(*mode=='M')||((*mode=='a')&&(f))) {
		if (f) {
			TESTFILE(AFWav);
			TESTFILE(AFTei);
			TESTFILE(AFAho2);
			TESTFILE(AFAho1);
      TESTFILE(AFNist);
      TESTFILE(AFHak);
			// TypeCheck - poner aqui encima nuevos tipos
			TESTFILE(AFRaw);
			fclose(f);
		}
		// seleccion por extension. Solo para ficheros 'irreconocibles'
		if (!stricmp(ext,".wav")) return new AFWav(this);
		if (!stricmp(ext,".aho")) return new AFAho1(this);
		if (!stricmp(ext,".eus")) return new AFAho1(this);
		if (!stricmp(ext,".raw")) return new AFRaw(this);
		if (!stricmp(ext,".pcm")) return new AFRaw(this);
    if (!stricmp(ext,".nist")) return new AFNist(this);
    if (!stricmp(ext,".nis")) return new AFNist(this);
    if (!stricmp(ext,".hak")) return new AFHak(this);
		// TypeCheck - poner aqui encima nuevas extensiones

		// buscamos tipo por defecto
		RETURNIF(AFRaw);
		RETURNIF(AFWav);
		RETURNIF(AFAho2);
		RETURNIF(AFAho1);
		RETURNIF(AFTei);
		RETURNIF(AFNist);
    RETURNIF(AFHak);
		// TypeCheck - poner aqui encima nuevos tipos

		/* si se llega aqui, creamos tipo por defecto. Solo sucede
		si se indica AFFormat="Auto" sin especificar tipo
		por defecto valido, la extension es desconocida, y no
		se puede determinar el tipo del fichero en disco */
		return new AFRaw(this);
	}

	if (f) fclose(f);

	// si llegamos aqui, es porque hay que crear un fichero nuevo
	if (!stricmp(ext,".wav")) return new AFWav(this);
	if (!stricmp(ext,".aho")) return new AFAho2(this);
	if (!stricmp(ext,".eus")) return new AFAho2(this);
	if (!stricmp(ext,".raw")) return new AFRaw(this);
	if (!stricmp(ext,".pcm")) return new AFRaw(this);
	if (!stricmp(ext,".tei")) return new AFTei(this);
	if (!stricmp(ext,".nist")) return new AFNist(this);  
	if (!stricmp(ext,".nis")) return new AFNist(this);  
  if (!stricmp(ext,".hak")) return new AFHak(this);  
	// TypeCheck - poner aqui encima nuevas extensiones

	RETURNIF(AFRaw);
	RETURNIF(AFWav);
	RETURNIF(AFAho2);
	RETURNIF(AFAho1);
	RETURNIF(AFTei);
  RETURNIF(AFNist);
  RETURNIF(AFHak);
	// TypeCheck - poner aqui encima nuevos tipos

	/* si se llega aqui, creamos tipo por defecto. Solo sucede
	si se indica AFFormat="Auto" sin especificar tipo
	por defecto valido, y la extension es desconocida */
	return new AFRaw(this);
}

/**********************************************************/

VOID CAudioFile::txhGetName( const CHAR *fName )
{
#define IFEXT_2EXT(ext1,ext2) if (!stricmp(ext,ext1)) dest2 = path_src2destp(dest,"",ext2)
	CHAR *dest = path_src2destp(fName,myOpts.val(CAUDIO_TXHNAME,""),
			myOpts.val(CAUDIO_TXHRULE,""));

	// verificamos que la extension no sea igual
	const CHAR *ext = fName+path_extpos(fName);
	if (!stricmp(ext, dest+path_extpos(dest))) {
		CHAR *dest2=0;
		BOOL inc=FALSE;

		IFEXT_2EXT(".pcm",".inf");
		else IFEXT_2EXT(".raw",".hdr");
		else IFEXT_2EXT(".wav",".wai");
		else IFEXT_2EXT(".tei",".teh");
		else IFEXT_2EXT(".aho",".ahi");
		else IFEXT_2EXT(".eus",".eui");
		else IFEXT_2EXT(".inf",".nfo");
		else IFEXT_2EXT(".nist",".nish");    
		else IFEXT_2EXT(".nis",".nih");    
    else IFEXT_2EXT(".hak",".hai");    
		// TypeCheck - poner aqui encima nuevas extensiones para fichero txh asociado
		else IFEXT_2EXT(".",".dat");
		else IFEXT_2EXT("",".dot");
		else {  // por defecto, incrementa ultima letra de la extension
			assert(strlen(dest));
			(*(dest+strlen(dest)-1))++;
			inc=TRUE;
		}
		if (!inc) { free(dest); dest=dest2; }
	}

	myOpts.add(CAUDIO_TXHNAME,dest),
	free(dest);
}

/**********************************************************/
