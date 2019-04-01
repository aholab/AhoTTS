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

#include <string.h>
#include <stdlib.h>

#include "uti.h"
#include "caudiox.hpp"

/**********************************************************/

PRIVATE CHAR *_nist_id16 = (char*) "NIST_1A\n   1024\n";

/**********************************************************/

LONG AFNist::HdrR( FILE *f, KVStrList &, BOOL override )
{
#define N 1024
  CHAR buf[N];
  CHAR *s;
  LONG nSamp;

	if (!testFile(f))
		fprintf(stderr,"%s warning: probably not a NIST 1024 file!\n",fFormat());
	xfseek(f,16,SEEK_SET);

  nSamp=-1;  // auto nsamp

  while (1) {
    if (!xfgets(buf,N,f)) break;
    if (strlen(buf) && (buf[strlen(buf)-1]!='\n'))
      die_beep("%s error: NIST header line too long",fFormat());
    s = strtok(buf," \n");
    if (!s) continue;
    if (!strcmp(s,"end_head")) break;
		if (!strcmp(s,"channel_count")) { s=strtok(NULL," \n"); s=strtok(NULL," \n;"); if (s) ADDIFNOV(CAUDIO_NCHAN,atol(s)); };
    if (!strcmp(s,"sample_count")) { s=strtok(NULL," \n"); s=strtok(NULL," \n;"); if (s) nSamp=atol(s); };
    if (!strcmp(s,"sample_rate")) { s=strtok(NULL," \n"); s=strtok(NULL," \n;"); if (s) ADDIFNOV(CAUDIO_SRATE,atol(s)); };
    if (!strcmp(s,"sample_coding")) { s=strtok(NULL," \n"); s=strtok(NULL," \n;");
      if (s && (strcmp("pcm",s))) die_beep("%s error: sample coding not supported (%s)",fFormat(),s); }
    if (!strcmp(s,"sample_n_bytes")) { s=strtok(NULL," \n"); s=strtok(NULL," \n;");
      INT slen=atoi(s);
			if (slen==2) ADD(CAUDIO_SAMPTYPE,SAMPTYPE_STR_PCM16);
      else if (slen==1) ADD(CAUDIO_SAMPTYPE,SAMPTYPE_STR_PCM8U);
      else die_beep("%s error: sample size not supported (%s)",fFormat(),s);
    }
    if (!strcmp(s,"sample_byte_format")) { s=strtok(NULL," \n"); s=strtok(NULL," \n;");
      if (s && (!strcmp("10",s))) ADDIFNOV(CAUDIO_BIGENDIAN,"yes");
      else if (s && (!strcmp(s,"01"))) ADDIFNOV(CAUDIO_BIGENDIAN,"no");
      else if (s) die_beep("%s error: sample byte format not supported (%s)",fFormat(),s);
    }
  }  // while

	xfseek(f,1024,SEEK_SET);

	return nSamp;
}

/**********************************************************/

VOID AFNist::HdrG( KVStrList &def, BOOL override )
{
	ADD(CAUDIO_NSAMPLES,0);
	ADDIFNOV(CAUDIO_SRATE,def.dval(CAUDIO_SRATE,8000));
	ADDIFNOV(CAUDIO_SAMPTYPE,def.val(CAUDIO_SAMPTYPE,SAMPTYPE_STR_PCM16));
	ADDIFNOV(CAUDIO_NCHAN,def.lval(CAUDIO_NCHAN,1));

	// endian por defecto
	ADDIFNOV(CAUDIO_BIGENDIAN,def.val(CAUDIO_BIGENDIAN,CAUDIO_BIGENDIAN_DEFAULT?"yes":"no"));

	switch (SampType_a2i(fa->info().val(CAUDIO_SAMPTYPE))) {
	case SAMPTYPE_PCM16:
	case SAMPTYPE_PCM8U: break;
	default:
		die_beep("%s: sampType %s not supported",fFormat(),
		(const char *)fa->info().val(CAUDIO_SAMPTYPE));
	}
}

/**********************************************************/

VOID AFNist::HdrW( FILE *f, LONG nSamp )
{
	xfseek(f,0,SEEK_SET);
	xfwrite(_nist_id16,16,1,f);

	fprintf(f,"channel_count -i %ld\n",(long)(fa->getNChan()));
  fprintf(f,"sample_count -i %ld\n",(long)nSamp);
  fprintf(f,"sample_rate -i %ld\n",(long)(fa->getSRate()+0.5));

	long ssize=0;
	switch (fa->getSampType()) {
	case SAMPTYPE_PCM16: ssize=2; break;
	case SAMPTYPE_PCM8U: ssize=1; break;
	default: die_beep("%s: invalid sampType (%s)",fFormat(),fa->getSampType_a());
	}
  fprintf(f,"sample_coding -s3 pcm\n");
  fprintf(f,"sample_n_bytes -i %ld\n",(long)ssize);
  fprintf(f,"sample_byte_format -s2 %s\n",fa->getBigEndian()?"10":"01");

  fprintf(f,"end_head\n");

  // completar a 1024
  long pos0 = xftell(f);
  cdie_beep(pos0>1024,"%s error: NIST header too long (%ld)",fFormat(),(long)pos0);
  int i, nn, n=(int)(1024-pos0);
  nn = (n>10) ? 10 : n;
  for (i=0; i<nn; i++) { char ch='\n'; fwrite(&ch,1,1,f); }
  for (i=nn; i<n; i++) { char ch=0; fwrite(&ch,1,1,f); }
}

/**********************************************************/

BOOL AFNist::testFile( FILE *f )
{
	CHAR id[17];

	memset(id,0,sizeof(id));
	fseek(f,0,SEEK_SET);
	fread(id,16,1,f);
	return (!strcmp(id,_nist_id16));
}

/**********************************************************/
