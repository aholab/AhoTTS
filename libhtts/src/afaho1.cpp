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
1.0.3	 2012		Iñaki	 corregir warnings deprecated
1.0.2	 2012		Iñaki	 eliminar dependencias de xalloc (xstrdup, xmalloc, xfree)
1.0.1    09/04/99  Borja     uso tipos UINT
0.0.0    15/07/96  Borja     codificacion inicial.

======================== Contenido ========================
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "caudiox.hpp"

#include "uti.h"
#include "cabecer.h"
#include "cabctrl.h"

/**********************************************************/
/* Lee de disco una cabecera (.aho-1.0) y crea todos los campos
de informacion etc etc */

LONG AFAho1::HdrR( FILE *f, KVStrList &, BOOL override )
{
	LONG nSamp;
	struct cgsenal cgs;

	xfseek(f,0,SEEK_SET);
	reset_cgsenal(&cgs);
	vleecfs(f,&cgs);

	nSamp = (LONG)cgs.nm;  // numero muestras

	ADDIFNOV(CAUDIO_SRATE,(DOUBLE)cgs.fm);
  ADDIFNOV(CAUDIO_BIGENDIAN,"no");  // aho1 es little endian
	ADD(CAUDIO_SAMPTYPE, SAMPTYPE_STR_PCM16);  // fuerza samptype
	ADD(CAUDIO_NCHAN, 1);  // fuerza nchan

	if (cgs.locu.ltxt) ADDIFNOV(INFO_SPEAKER,cgs.locu.txt);
	if (cgs.texto.ltxt) ADDIFNOV(INFO_TEXT, cgs.texto.txt);
	if (cgs.version.ltxt) ADDIFNOV(INFO_VERSION,cgs.version.txt);
	if (cgs.obs.ltxt) ADDIFNOV(INFO_COMMENTS,cgs.obs.txt);

	if ( (cgs.marc.nmarc>0)
		&& (fa->marks().length()==0) || override) { // leemos las marcas?
		fa->marks().clear(); //borrar marcas viejas
		for (LONG i=0; i<(LONG)cgs.marc.nmarc; i++)
			fa->marks().append(cgs.marc.marcas[(size_t)i]*cgs.marc.unimarc);
	}

	free_cgsenal(&cgs);

  if (fa->info().bval(CAUDIO_BIGENDIAN))
    fprintf(stderr,"%s warning: reading big endian file!.\n",fFormat());

	return nSamp;
}

/**********************************************************/
/* genera una cabecera nueva */

VOID AFAho1::HdrG( KVStrList &def, BOOL override )
{
	ADD(CAUDIO_NSAMPLES,0);
	ADDIFNOV(CAUDIO_SRATE,def.dval(CAUDIO_SRATE,8000));
	ADDIFNOV(CAUDIO_SAMPTYPE,def.val(CAUDIO_SAMPTYPE,SAMPTYPE_STR_PCM16));
	ADDIFNOV(CAUDIO_NCHAN,def.dval(CAUDIO_NCHAN,1));

	cdie_beep(SampType_a2i(fa->info().val(CAUDIO_SAMPTYPE))!=SAMPTYPE_PCM16,
			"%s: sampType %s not supported",fFormat(),
			(const char *)fa->info().val(CAUDIO_SAMPTYPE));
	cdie_beep(fa->info().lval(CAUDIO_NCHAN)!=1,
			"%s: multichannel (%ld) not supported",fFormat(),
			(long)(fa->info().lval(CAUDIO_NCHAN)));

	ADDIFNOV(CAUDIO_BIGENDIAN,"no");  // aho1 es little endian
}

/**********************************************************/
/* escribe a disco la cabecera */

VOID AFAho1::HdrW( FILE *f, LONG nSamp )
{
#define TXT(prop,field)  { \
	cgs.##field##.txt = (char*)fa->info().val(prop,""); \
	cgs.##field##.ltxt = strlen(cgs.##field##.txt); \
}

	struct cgsenal cgs;
	Lix p;

	cdie_beep(fa->getSampType()!=SAMPTYPE_PCM16,
			"%s: sampType %s not supported",fFormat(),fa->getSampType_a());
	cdie_beep(fa->getNChan()!=1,
			"%s: multichannel (%ld) not supported",fFormat(),
			(long)(fa->getNChan()));

	cgs.nm = nSamp;
	cgs.fm = fa->getSRate();

	//TXT(INFO_SPEAKER,locu);
	cgs.locu.txt = (char*)fa->info().val(INFO_SPEAKER,"");
	cgs.locu.ltxt = strlen(cgs.locu.txt);
	//TXT(INFO_TEXT,texto);
	cgs.texto.txt = (char*)fa->info().val(INFO_TEXT,"");
	cgs.texto.ltxt = strlen(cgs.texto.txt);
	//TXT(INFO_VERSION,version);
	cgs.version.txt = (char*)fa->info().val(INFO_VERSION,"");
	cgs.version.ltxt = strlen(cgs.version.txt);
	//TXT(INFO_COMMENTS,obs);
	cgs.obs.txt = (char*)fa->info().val(INFO_COMMENTS,"");
	cgs.obs.ltxt = strlen(cgs.obs.txt);

	LONG max=0;
	for (p=fa->marks().first(); p!=0; p=fa->marks().next(p)) {
		LONG x= fa->marks().pos(p);
		if (max<x) max=x;
	}
	cgs.marc.unimarc = (UINT32)(max/32768L+1);
	cgs.marc.nmarc = fa->marks().length();
	cgs.marc.marcas = (pUINT32)malloc(sizeof(UINT32)*cgs.marc.nmarc);
	LONG i=0;
	for (p=fa->marks().first(); p!=0; p=fa->marks().next(p))
		cgs.marc.marcas[(size_t)i++] = (fa->marks().pos(p)+cgs.marc.unimarc/2)/cgs.marc.unimarc;
	vescribecfs(f,&cgs);

	if (cgs.marc.marcas) free(cgs.marc.marcas);

  if (fa->getBigEndian())
    fprintf(stderr,"%s warning: saving big endian file!\n",fFormat());

}

/**********************************************************/

BOOL AFAho1::testFile( FILE * )
{
	return FALSE;
}

/**********************************************************/
