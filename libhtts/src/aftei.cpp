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
1.0.2    2012	   Iñaki     eliminar MINALIGN (__attribute(packed) donde no es necesario
1.0.1    09/04/99  Borja     uso tipos UINT
0.0.0    15/07/96  Borja     codificacion inicial.

======================== Contenido ========================
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include <string.h>

#include "uti.h"
#include "caudiox.hpp"

/**********************************************************/

#define BD_LENFILEID    4
#define BD_LENLOCUTOR   30
#define BD_LENORIGEN    20
#define BD_LENMICROFONO 20
#define BD_LENUNIDADES  150

/**********************************************************/

#ifdef __CC_MSVC__
#pragma pack(push,1)
#elif __CC_BORLANDC__
typedef struct {CHAR8 x; INT2 y; } ____tmp_tdef_align_test___;
#if sizeof(____tmp_tdef_align_test___)>3
#error You must disable Word Alignment
#endif
#endif

/**********************************************************/

/* Estructura de la cabecera de un fichero de voz del C31 */
typedef union _tagHeaderFileC30 {
	BYTE buffer[512]; /* Buffer de 512 bytes */
	struct _tagFile {
		CHAR8 FileID[BD_LENFILEID]; /* Identificador de fichero de voz "LTI" */
		INT32 NumeroPuntos MINALIGN;        /* N£mero de puntos de la se¤al */
		INT32 Frecuencia MINALIGN;          /* Frecuencia de muestreo */
		INT32 NumberOfChannels MINALIGN;    /* N£mero de canales */
		INT32 TimeId MINALIGN;              /* Identificador randomiano de fichero */
		INT16 FiltroAntialiasing MINALIGN;   /* Filtro */
		CHAR Locutor[BD_LENLOCUTOR]; /* Identificaci¢n del locutor */
		CHAR Sexo;                 /* Sexo del locutor */
		INT16 Edad MINALIGN;                 /* Edad del locutor */
		CHAR Origen[BD_LENORIGEN]; /* Origen del locutor */
		CHAR TipoMicrofono[BD_LENMICROFONO]; /* Tipo de micr¢fono usado */
		INT16 NumeroParesSegmentos MINALIGN; /* N£mero de pares de marcas almacenadas */
		CHAR Unidades[BD_LENUNIDADES]; /* Unidad de corpus */
		INT16 NumeroParesSubUnidades MINALIGN;
		} T;
	} HEADERFILEC30;

#ifdef __CC_MSVC__
#pragma pack(pop)
#endif

/**********************************************************/

LONG AFTei::HdrR( FILE *f, KVStrList &, BOOL override )
{
	HEADERFILEC30 Header;
	LONG nSamp;

	xfseek(f,0,SEEK_SET);
	xfread(&Header,sizeof(Header),1,f);
	if (strcmp(Header.T.FileID, "LTI")) die_beep("Tei: not a .tei file");
	endian_fromlittle32(&(Header.T.NumeroPuntos));
	endian_fromlittle32(&(Header.T.Frecuencia));
	endian_fromlittle32(&(Header.T.NumberOfChannels));
	endian_fromlittle32(&(Header.T.TimeId));
	endian_fromlittle16(&(Header.T.FiltroAntialiasing));
	endian_fromlittle16(&(Header.T.Edad));
	endian_fromlittle16(&(Header.T.NumeroParesSegmentos));
	endian_fromlittle16(&(Header.T.NumeroParesSubUnidades));

	if (Header.T.NumeroPuntos) nSamp = Header.T.NumeroPuntos;
	else {
		long pos0 = xftell(f);
		xfseek(f,0,SEEK_END);
		LONG fsize = xftell(f)-pos0;
		fseek(f,pos0,SEEK_SET);
		nSamp = (fsize-(Header.T.NumeroParesSegmentos*sizeof(INT32)*2))/sizeof(INT16);
	}
	ADD(CAUDIO_SAMPTYPE,SAMPTYPE_STR_PCM16);
  ADDIFNOV(CAUDIO_BIGENDIAN,"no");  // tei es little endian

	ADDIFNOV(CAUDIO_SRATE,(LONG)(Header.T.Frecuencia));
	ADDIFNOV(CAUDIO_NCHAN,(LONG)(Header.T.NumberOfChannels));
	ADDIFNOV("TimeID",(LONG)(Header.T.TimeId));
	ADDIFNOV("AntiAliasFilterFrec",(LONG)(Header.T.FiltroAntialiasing));
	if (*(Header.T.Locutor)) ADDIFNOV(INFO_SPEAKER,Header.T.Locutor);
	ADDIFNOV("SpeakerSex",String(Header.T.Sexo));
	ADDIFNOV("SpeakerAge",(LONG)(Header.T.Edad));
	if (*(Header.T.Origen)) ADDIFNOV("SpeakerOrigin",Header.T.Origen);
	if (*(Header.T.TipoMicrofono)) ADDIFNOV("MicType",Header.T.TipoMicrofono);
	if (*(Header.T.Unidades)) ADDIFNOV("Units",Header.T.Unidades);

	if ( (Header.T.NumeroParesSegmentos>0)
		&& (fa->marks().length()==0) || override) { // leemos las marcas?
		fa->marks().clear(); //borrar marcas viejas
		xfseek(f,sizeof(Header)+sizeof(INT16)*nSamp,SEEK_SET);
		for (LONG i=0; i<Header.T.NumeroParesSegmentos; i++) {
			INT32 m, m2;
			xfread(&m,sizeof(m),1,f); xfread(&m2,sizeof(m2),1,f);
			endian_fromlittle32(&m);
			endian_fromlittle32(&m2);
			if (m2!=m) fa->marks().append(m,m2-m+1);
			else fa->marks().append(m);
		}
	}

// $#$#$# Ignoro {Header.T.NumeroParesSubUnidades}

	xfseek(f,sizeof(Header),SEEK_SET);

  if (fa->info().bval(CAUDIO_BIGENDIAN))
    fprintf(stderr,"%s warning: reading big endian file!.\n",fFormat());

	return nSamp;
}

/**********************************************************/

VOID AFTei::HdrG( KVStrList &def, BOOL override )
{
	ADD(CAUDIO_NSAMPLES,0);
	ADDIFNOV(CAUDIO_SRATE,def.dval(CAUDIO_SRATE,8000));
	ADDIFNOV(CAUDIO_SAMPTYPE,def.val(CAUDIO_SAMPTYPE,SAMPTYPE_STR_PCM16));
	ADDIFNOV(CAUDIO_NCHAN,def.val(CAUDIO_NCHAN,1));

	cdie_beep(SampType_a2i(fa->info().val(CAUDIO_SAMPTYPE))!=SAMPTYPE_PCM16,
			"%s: sampType %s not supported",fFormat(),
			(const char *)fa->info().val(CAUDIO_SAMPTYPE));
  ADDIFNOV(CAUDIO_BIGENDIAN,"no");  // tei es little endian
}

/**********************************************************/

VOID AFTei::HdrW( FILE *f, LONG nSamp )
{
	HEADERFILEC30 Header;

	cdie_beep(fa->getSampType()!=SAMPTYPE_PCM16,
		"%s: sampType %s not supported",fFormat(),fa->getSampType_a());

	memset(&Header,0,sizeof(Header));
	strcpy(Header.T.FileID, "LTI");
	Header.T.NumeroPuntos = nSamp;
	Header.T.Frecuencia = (INT32)fa->getSRate();
	Header.T.NumberOfChannels = fa->getNChan();
	Header.T.TimeId = fa->info().lval("TimeID",0);
	Header.T.FiltroAntialiasing = (INT16)fa->info().lval("AntiAliasFilterFrec",3600);
	strncpy(Header.T.Locutor,fa->info().val(INFO_SPEAKER,""),BD_LENLOCUTOR);
	Header.T.Sexo = (fa->info().val("SpeakerSex","V"))[0];
	Header.T.Edad = (INT16)fa->info().lval("SpeakerAge",30);
	strncpy(Header.T.Origen,fa->info().val("SpeakerOrigin",""),BD_LENORIGEN);
	strncpy(Header.T.TipoMicrofono,fa->info().val("MicType",""),BD_LENMICROFONO);
	Header.T.NumeroParesSegmentos = (INT16)(fa->marks().length());
	strncpy(Header.T.Unidades,fa->info().val("Units",""),BD_LENUNIDADES);

	endian_tolittle32(&(Header.T.NumeroPuntos));
	endian_tolittle32(&(Header.T.Frecuencia));
	endian_tolittle32(&(Header.T.NumberOfChannels));
	endian_tolittle32(&(Header.T.TimeId));
	endian_tolittle16(&(Header.T.FiltroAntialiasing));
	endian_tolittle16(&(Header.T.Edad));
	endian_tolittle16(&(Header.T.NumeroParesSegmentos));
	endian_tolittle16(&(Header.T.NumeroParesSubUnidades));
	xfseek(f,0,SEEK_SET);
	xfwrite(&Header,sizeof(Header),1,f);

	// escribimos las marcas
	xfseek(f,sizeof(Header)+sizeof(INT16)*nSamp,SEEK_SET);
	for (Lix p=fa->marks().first(); p!=0; p=fa->marks().next(p)) {
		INT32 m1, m2;
		m1 = fa->marks().pos(p);
		m2 = fa->marks().span(p);
		if (m2) m2+=(m1-1); else m2=m1;
		endian_tolittle32(&m1);
		endian_tolittle32(&m2);
		xfwrite(&m1,sizeof(m1),1,f);
		xfwrite(&m2,sizeof(m2),1,f);
	}

// $#$#$# Ignoro {Header.T.NumeroParesSubUnidades}
	xfseek(f,sizeof(Header),SEEK_SET);

	if (fa->getBigEndian())
		fprintf(stderr,"%s warning: saving big endian file!\n",fFormat());

}

/**********************************************************/

BOOL AFTei::testFile( FILE *f )
{
	CHAR id[BD_LENFILEID];

	memset(id,0,sizeof(id));
	fseek(f,0,SEEK_SET);
	fread(&id,sizeof(id),1,f);
	return (!strcmp(id,"LTI"));
}

/**********************************************************/
