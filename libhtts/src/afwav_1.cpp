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
                  & CSTR - Univ. of Edinburgh

Nombre fuente................ -
Nombre paquete............... -
Lenguaje fuente.............. C++ (Borland C/C++ 3.1)
Estado....................... desarrollo
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Borja Etxebarria

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
1.0.2	 2012	   Iñaki	 Eliminar tipos MINALIGN innecesarios
1.0.1    09/04/99  Borja     uso tipos UINT
0.0.2    30/08/98  Borja     split en varios modulos afwav_?.cpp
0.0.1    27/07/97  Borja     adapted to compile on SunOS.
0.0.0    15/07/96  Borja     codificacion inicial.

======================== Contenido ========================
ver afwav.cpp
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include "afwav_i.hpp"

/**********************************************************/
/* escribe a disco la cabecera */

VOID AFWav::HdrW( FILE *f, LONG nSamp )
{
#define WRITEIFIS(sk, key) \
{ \
	if (fa->info().contains(key)) { \
		const String &s=fa->info().val(key); \
		UINT32 n=strlen_zeven(s); \
		xfwrite(sk,4,1,f); \
		endian_tolittle32(&n); xfwrite(&n,sizeof(n),1,f); \
		puts_zeven((const char *)s,f); \
	}  \
}

	long b0len=sizeof(WAVHdr)                // global hdr
			+sizeof(WAVChunk)+sizeof(FormatInfo)+sizeof(UINT16)   // format info
			+sizeof(WAVChunk);   // audio data chunk hdr

	// nueva cabecera mayor que la original => inserta hueco
	if (b0len>datapos) xfinsn(f,0,b0len-datapos);
	// nueva menor que original => quita hueco
	else if (b0len<datapos) xfdeln(f,0,datapos-b0len);
	datapos=b0len;  // salva nueva posicion

	WAVHdr wh = { {'R','I','F','F'}, 0, {'W','A','V','E'} };
	WAVChunk fiwc = { {'f','m','t',' '}, sizeof(FormatInfo)+sizeof(UINT16) };
	FormatInfo fi = { WFORMAT_PCM, 0, 0, 0, 0 };
	UINT16 ssize;
	switch (fa->getSampType()) {
	case SAMPTYPE_PCM16: ssize=16; break;
	case SAMPTYPE_PCM8U: ssize=8; break;
	default: die_beep("%s: invalid sampType (%s)",fFormat(),fa->getSampType_a());
	}
	fi.wChannels = fa->getNChan();
	fi.dwSamplesPerSec = (UINT32)fa->getSRate();
	fi.dwAvgBytesPerSec = (ssize>>3)*fi.dwSamplesPerSec*fi.wChannels;
	fi.wBlockAlign = (ssize>>3)*fi.wChannels;
	WAVChunk wc = { {'d','a','t','a'}, 0 };
	wc.len = nSamp*(ssize>>3)*fi.wChannels;

	fseek(f,0,SEEK_SET);
	endian_tolittle32(&(wh.len)); xfwrite(&wh,sizeof(wh),1,f);
	endian_tolittle32(&(fiwc.len)); xfwrite(&fiwc,sizeof(fiwc),1,f);
	endian_tolittle16(&(fi.wFormatTag)); endian_tolittle16(&(fi.wChannels));
	endian_tolittle32(&(fi.dwSamplesPerSec)); endian_tolittle32(&(fi.dwAvgBytesPerSec));
	endian_tolittle16(&(fi.wBlockAlign)); 	xfwrite(&fi,sizeof(fi),1,f);
	endian_tolittle16(&ssize); xfwrite(&ssize,sizeof(ssize),1,f);
	endian_tolittle32(&(wc.len)); xfwrite(&wc,sizeof(wc),1,f); endian_fromlittle32(&(wc.len));
	fseek(f,wc.len,SEEK_CUR);
	// escribir info, marcas y demas
	long final, pos;

	// escribir campos LIST INFO
	pos = xftell(f);
	WAVChunk inf = { {'L','I','S','T'}, 0 };
	CHAR8 info[4] = {'I','N','F','O'};
	endian_tolittle32(&(inf.len)); xfwrite(&inf,sizeof(inf),1,f);
	xfwrite(&info,sizeof(info),1,f);
	WRITEIFIS("IART","OriginalArtist");
	WRITEIFIS("ICMT","Comments");
	WRITEIFIS("ICOP","Copyright");
	WRITEIFIS("ICRD","CreationDate");
	WRITEIFIS("IENG","Engineers");
	WRITEIFIS("IGNR","Genre");
	WRITEIFIS("IKEY","KeyWords");
	WRITEIFIS("IMED","OriginalMedium");
	WRITEIFIS("INAM","Name");
	WRITEIFIS("ISFT","SoftwarePackage");
	WRITEIFIS("ISRC","SourceSupplier");
	WRITEIFIS("ITCH","Digitizer");
	WRITEIFIS("ISBJ","Subject");
	WRITEIFIS("ISRF","DigitizationSource");

	final=xftell(f);
	// escribir tamanyo chunk LIST INFO, o quitar si no hay ningun campo
	UINT32 ll = final-pos-8;
	if (ll>4) {
		xfseek(f,pos+4,SEEK_SET);
		endian_tolittle32(&ll); xfwrite(&ll,sizeof(ll),1,f);
	}
	else {
		xfdeln(f,pos,12);  // borra
		final -= 12;
	}
	xfseek(f,final,SEEK_SET);

	if (fa->marks().length()) {
		WAVChunk cue = { {'c','u','e',' '}, 0 };
		UINT32 n32=fa->marks().length();
		Lix p;
		BOOL use_span=FALSE;

		// direct {cp} initialization does not work with SunOS,
		// ...so we use field by field init.
		CuePoint cp; // = { 0, 0, {'d','a','t','a'}, 0, 0, 0};
		cp.dwName = cp.dwPosition = cp.dwChunkStart=cp.dwBlockStart=cp.dwSampleOffset=0;
		cp.fccChunk[0]='d'; cp.fccChunk[1]='a';cp.fccChunk[2]='t';cp.fccChunk[3]='a';

		cue.len = sizeof(n32)+n32*sizeof(cp);
		LONG i;
		endian_tolittle32(&(cue.len)); xfwrite(&cue,sizeof(cue),1,f);
		endian_tolittle32(&n32); xfwrite(&n32,sizeof(n32),1,f);
		i=0;
		for (p=fa->marks().first(); p!=0; p=fa->marks().next(p)) {
			i++;
			cp.dwName=i;
			cp.dwPosition = cp.dwSampleOffset = fa->marks().pos(p);
			if (fa->marks().span(p)) use_span=TRUE;
			endian_tolittle32(&(cp.dwName)); endian_tolittle32(&(cp.dwPosition));
			endian_tolittle32(&(cp.dwSampleOffset)); xfwrite(&cp,sizeof(cp),1,f);
		}
		final=xftell(f);
		pos=final;

		// hdr para span, labl, note
		WAVChunk lst = { {'L','I','S','T'}, 0 };
		CHAR8 adtl [4] = {'a','d','t','l'};
		endian_tolittle32(&(lst.len)); xfwrite(&lst,sizeof(lst),1,f);
		xfwrite(&adtl,sizeof(adtl),1,f);

		use_span=TRUE;
		if (use_span) {
			WAVChunk ltxt = { {'l','t','x','t'}, 0 };
			i=0;
			struct { UINT32 n MINALIGN; UINT32 len MINALIGN; CHAR8 pur[4]; UINT16 country MINALIGN;
				UINT16 lang MINALIGN; UINT16 dialec MINALIGN; UINT16 cpage MINALIGN; } ltxts; // = { 0, 0, {'r','g','n',' '},0,0,0,0};
			// SunOS da problemas inicializando en la declaracion. inicializamos campo a campo:
			ltxts.n=0; 	ltxts.len=0; ltxts.pur[0]='r'; ltxts.pur[1]='g'; ltxts.pur[2]='n'; ltxts.pur[3]=' ';
			ltxts.country=0; ltxts.lang=0; ltxts.dialec=0;  ltxts.cpage=0;
			ltxt.len=sizeof(ltxts); endian_tolittle32(&(ltxt.len));
			for (p=fa->marks().first(); p!=0; p=fa->marks().next(p)) {
				i++;
				ltxts.n=i;
				ltxts.len=fa->marks().span(p);
				xfwrite(&ltxt,sizeof(ltxt),1,f);
				endian_tolittle32(&(ltxts.n)); endian_tolittle32(&(ltxts.len)); xfwrite(&ltxts,sizeof(ltxts),1,f);
			}
		}

#ifndef IGNORE_CUE_LABEL   // no salvar NADA sobre labl
		// labl
		WAVChunk label = { {'l','a','b','l'}, 0 };
		i=0;
		for (p=fa->marks().first(); p!=0; p=fa->marks().next(p)) {
#ifdef IGNORE_CUE_LABEL
			String tmp="";
#else
			String tmp=fa->marks().label(p).before(" // ");
#endif
			n32= ++i;
			label.len=strlen_zeven(tmp)+sizeof(n32);
			endian_tolittle32(&(label.len)); xfwrite(&label,sizeof(label),1,f);
			endian_tolittle32(&n32); xfwrite(&n32,sizeof(n32),1,f);
			puts_zeven(tmp,f);
		}
#endif

		//note
		WAVChunk note = { {'n','o','t','e'}, 0 };
		i=0;
		for (p=fa->marks().first(); p!=0; p=fa->marks().next(p)) {
			String tmp;
#ifdef IGNORE_CUE_LABEL
			tmp=fa->marks().label(p);
#else
			if (fa->marks().label(p).contains(" // ")) tmp=fa->marks().label(p).after(" // ");
			else tmp=fa->marks().label(p);
#endif
			n32= ++i;
			note.len=strlen_zeven(tmp)+sizeof(n32);
			endian_tolittle32(&(note.len)); xfwrite(&note,sizeof(note),1,f);
			endian_tolittle32(&n32); xfwrite(&n32,sizeof(n32),1,f);
			puts_zeven(tmp,f);
		}

		final=xftell(f);
		// escribir tamanyo chunk LIST
		UINT32 l = final-pos-8;
		xfseek(f,pos+4,SEEK_SET);
		endian_tolittle32(&l); xfwrite(&l,sizeof(l),1,f);
		xfseek(f,final,SEEK_SET);
	}

	// escribir DISP si existe
	if (fa->info().contains("DisplayTitle")) {
		WAVChunk disp = { {'D','I','S','P'}, 0 };
		const String &s=fa->info().val("DisplayTitle");
		UINT32 x=1;
		disp.len=sizeof(UINT32)+strlen_zeven(s);
		endian_tolittle32(&(disp.len)); xfwrite(&disp,sizeof(disp),1,f);
		endian_tolittle32(&x); xfwrite(&x,sizeof(x),1,f);  // no se que es esto
		puts_zeven(s,f);
	}

	final=xftell(f);
	//borrar la cola
	xfseek(f,0,SEEK_END);
	long fin=xftell(f);
	if (fin!=final) xfdeln(f,final,fin-final);

	// escribir longitud total en cabecera riff
	wh.len=xftell(f)-8;
	xfseek(f,4,SEEK_SET);
	endian_tolittle32(&(wh.len)); xfwrite(&(wh.len),sizeof(wh.len),1,f);

	xfseek(f,datapos,SEEK_SET);

  if (fa->getBigEndian())
    fprintf(stderr,"%s warning: saving big endian file!\n",fFormat());
}

/**********************************************************/
/* genera una cabecera nueva */

VOID AFWav::HdrG( KVStrList &def, BOOL override )
{
	// inicializar posicion de las muestras
	datapos=0;

	ADD(CAUDIO_NSAMPLES,0);
	ADDIFNOV(CAUDIO_SRATE,def.dval(CAUDIO_SRATE,8000));
  ADDIFNOV(CAUDIO_BIGENDIAN,"no");  // wav es little endian
	ADDIFNOV(CAUDIO_SAMPTYPE,def.val(CAUDIO_SAMPTYPE,SAMPTYPE_STR_PCM16));
	ADDIFNOV(CAUDIO_NCHAN,def.lval(CAUDIO_NCHAN,1));

	switch (SampType_a2i(fa->info().val(CAUDIO_SAMPTYPE))) {
	case SAMPTYPE_PCM16:
	case SAMPTYPE_PCM8U: break;
	default:
		die_beep("%s: sampType %s not supported",fFormat(),
		(const char *)fa->info().val(CAUDIO_SAMPTYPE));
	}
}

/**********************************************************/

/*#ifdef __CC_MSVC__
#pragma pack(pop)
#endif*/

/**********************************************************/
