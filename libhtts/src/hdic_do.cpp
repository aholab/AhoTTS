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

Nombre fuente................ hdic_do.cpp
Nombre paquete............... aHoTTS
Lenguaje fuente.............. C++
Estado....................... -
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion: ............... Oscar Losada Abella
............................. Borja Etxebarria
............................. Yon2. Juan Luis Murugarren

Version  dd/mm/aa  Autor     Comentario
-------  --------  --------  ----------
1.1.2    14/06/00  Yon2.     Bug en searchBin.
1.1.1    03/11/00  Yon2.     Bug en searchTxt.
1.1.0    01/07/00  richie    Separated mutexes (bin!=txt)
1.0.2    01/07/00  richie    Win32 Multithread
1.0.1    22/06/00  richie    POSIX multithread
1.0.0    31/01/00  borja     codefreeze aHoTTS v1.0
0.0.1    16/11/99  Borja     rellenar skeleton
0.0.0    16/11/99  Oscar     Version Inicial.

======================== Contenido ========================
<DOC>

$$$$
Busqueda case sensitive o no, con expansion o no.

1. Buscar en listas con expansiones
2. Buscar en listas sin expansiones

Dentro de cada lista.

1. buscar en lista insensitive
2. buscar en lista sensitive, long >= match_insensitive

Estructura diccionario binario:

1. firma aholab
2. version etc
3. cs exp  (start, nentry, lenmax explenmax)
4. cs -    (start, nentry, lenmax )
5. -  exp  (start, nentry, lenmax explenmax)
6. -  -    (start, nentry, lenmax )

cada entry tiene:
1. longitud (UINT16)
2. cadena (minusculas si es case insens)  null term, padded a lenmax
3. hdicref
4. en listas de exp, cadena null term, padded a explenmax, con longitud
delante (UINT16)


</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include "strl.hpp"
#include "httsmsg.h"
#include "uti.h"
#include "hdic.hpp"
#include "chset.h"


#define DEBUGDICx

#ifdef __AHOTTS_MT__
#ifdef __OS_UNIX__
  extern pthread_mutex_t hdicmutex_txt;
  extern pthread_mutex_t hdicmutex_bin;
#endif
#ifdef __OS_WINDOWS__
  extern HANDLE hdicmutex_txt;
  extern HANDLE hdicmutex_bin;
#endif
#endif
/**********************************************************/

#define HDIC_CASE_FIRST_FLAG

/**********************************************************/
/* $$$ */

VOID HDicDB::txt2HDicRef( const CHAR *str, HDicRef &r, char **exp)
{
	StrList l(str);
	Lix p=l.first();

	if (exp&&(*exp)) { free(*exp); *exp=NULL; }
	if (!p) return;

	for (p=l.next(p);p!=0;p=l.next(p))
	{
		const CHAR *s=l.item(p);
		if (!strcmp(HDIC_STR_CASE,s))
			continue; // ignorar flag case-sens
		if (s[0]=='%')
		{  // si es un texto de sustitucion, lo copiamos
			if (s[1]!='\0' && exp)
			{  // pero solo si el puntero no es NULL y la cadena mide mas de 0
				if (*exp) free(*exp);  // borramos el anterior si lo hay
				*exp=strdup(s+1);
			}
		}
		else if (!flagParse(s,r))
			htts_warn("HDic flag not known: %s",s);
	}
}

/**********************************************************/

VOID HDicDB::ftxtCreate( const CHAR *fname )
{
#ifdef __AHOTTS_MT__
#ifdef __OS_UNIX__
  pthread_mutex_lock (&hdicmutex_txt);
#endif
#ifdef __OS_WINDOWS__
  WaitForSingleObject (hdicmutex_txt,INFINITE);
#endif
#endif
	fileTxt=fopen(fname,"r");
#ifdef __AHOTTS_MT__
#ifdef __OS_UNIX__
  pthread_mutex_unlock (&hdicmutex_txt);
#endif
#ifdef __OS_WINDOWS__
  ReleaseMutex(hdicmutex_bin);
#endif
#endif
}

/**********************************************************/

BOOL HDicDB::checkDBType( const CHAR *dbname, CHAR t[2], LONG v )
{
	const CHAR *lang=getLangStr();
	assert(strlen(lang)==2);

	// si lang mide != 2, usamos 'xx' que seguro produce mensaje de warning :)
	if (strlen(lang)!=2) lang="xx";

	if ((t[0]!=lang[0])||(t[1]!=lang[1])) { htts_warn("HDic Database '%s' is not designed for language '%s'",dbname,lang); return FALSE; }
	if (v!=0) { htts_warn("HDic Database '%s' version is not valid",dbname); return FALSE; }
	return TRUE;
}

/**********************************************************/

VOID HDicDB::fbinCreate( const CHAR *fname )
{

#ifdef __AHOTTS_MT__
#ifdef __OS_UNIX__
  pthread_mutex_lock (&hdicmutex_bin);
#endif
#ifdef __OS_WINDOWS__
  WaitForSingleObject (hdicmutex_bin,INFINITE);
#endif
#endif
	fileBin=fopen(fname,"rb");
	if (!fileBin) {
#ifdef __AHOTTS_MT__
#ifdef __OS_UNIX__
	  pthread_mutex_unlock (&hdicmutex_bin);
#endif
#ifdef __OS_WINDOWS__
	  ReleaseMutex(hdicmutex_bin);
#endif
#endif
	  return;
	}
// 1. firma aholab
	const char *signature="Aholab aHoTTS HDIC Database\x1A";
	char buf[50];
	size_t l=strlen(signature)+1;
	size_t l2;
	l2=fread(buf,1,l,fileBin);
	if ((l2<l)||strncmp(signature,buf,l)) {
		htts_warn("Not an HDIC database '%s'",dbname);
		goto cerror;
	}

	UINT16 ui16;
	UINT32 ui32;

// 2. version etc
	CHAR type[2];
	LONG version;
	// dbase type
	l2=fread(&type,sizeof(CHAR),2,fileBin); if (l2!=2) goto ioerror;
	// dbase version
	l2=fread(&ui32,sizeof(ui32),1,fileBin); if (!l2) goto ioerror;
	endian_fromlittle32(&ui32); version=ui32;
	// check db type&version
	if (!checkDBType(dbname,type,version)) goto cerror;

// 3. cs exp  (start, nentry, lenmax explenmax)
	l2=fread(&ui32,sizeof(ui32),1,fileBin); if (!l2) goto ioerror;
	endian_fromlittle32(&ui32); b_base[0]=(long)ui32;
	l2=fread(&ui32,sizeof(ui32),1,fileBin); if (!l2) goto ioerror;
	endian_fromlittle32(&ui32); b_n[0]=(long)ui32;
	l2=fread(&ui16,sizeof(ui16),1,fileBin); if (!l2) goto ioerror;
	endian_fromlittle16(&ui16); b_slen[0]=(size_t)ui16;
	l2=fread(&ui16,sizeof(ui16),1,fileBin); if (!l2) goto ioerror;
	endian_fromlittle16(&ui16); b_exlen[0]=(size_t)ui16;
	b_blen[0]=sizeof(UINT16)+b_slen[0]+sizeof(UINT16)+b_exlen[0]
			+sizeof(UINT32);

// 4. cs -    (start, nentry, lenmax )
	l2=fread(&ui32,sizeof(ui32),1,fileBin); if (!l2) goto ioerror;
	endian_fromlittle32(&ui32); b_base[1]=(long)ui32;
	l2=fread(&ui32,sizeof(ui32),1,fileBin); if (!l2) goto ioerror;
	endian_fromlittle32(&ui32); b_n[1]=(long)ui32;
	l2=fread(&ui16,sizeof(ui16),1,fileBin); if (!l2) goto ioerror;
	endian_fromlittle16(&ui16); b_slen[1]=(size_t)ui16;
	b_exlen[1]=0;
	b_blen[1]=sizeof(UINT16)+b_slen[1]+sizeof(UINT32);

// 5. -  exp  (start, nentry, lenmax explenmax)
	l2=fread(&ui32,sizeof(ui32),1,fileBin); if (!l2) goto ioerror;
	endian_fromlittle32(&ui32); b_base[2]=(long)ui32;
	l2=fread(&ui32,sizeof(ui32),1,fileBin); if (!l2) goto ioerror;
	endian_fromlittle32(&ui32); b_n[2]=(long)ui32;
	l2=fread(&ui16,sizeof(ui16),1,fileBin); if (!l2) goto ioerror;
	endian_fromlittle16(&ui16); b_slen[2]=(size_t)ui16;
	l2=fread(&ui16,sizeof(ui16),1,fileBin); if (!l2) goto ioerror;
	endian_fromlittle16(&ui16); b_exlen[2]=(size_t)ui16;
	b_blen[2]=sizeof(UINT16)+b_slen[2]+sizeof(UINT16)+b_exlen[2]
			+sizeof(UINT32);

// 6. -  -    (start, nentry, lenmax )
	l2=fread(&ui32,sizeof(ui32),1,fileBin); if (!l2) goto ioerror;
	endian_fromlittle32(&ui32); b_base[3]=(long)ui32;
	l2=fread(&ui32,sizeof(ui32),1,fileBin); if (!l2) goto ioerror;
	endian_fromlittle32(&ui32); b_n[3]=(long)ui32;
	l2=fread(&ui16,sizeof(ui16),1,fileBin); if (!l2) goto ioerror;
	endian_fromlittle16(&ui16); b_slen[3]=(size_t)ui16;
	b_exlen[3]=3;
	b_blen[3]=sizeof(UINT16)+b_slen[3]+sizeof(UINT32);

	if (ftell(fileBin)!=b_base[0]) {
		htts_warn("Inconsistent HDIC database '%s'",dbname);
		goto cerror;
	}
	goto noerror;
	// si algo va mal, cerramos el fichero y nos olvidamos en el futuro.
ioerror:
	htts_warn("Failure reading HDIC database '%s'",dbname);
cerror:
	fclose(fileBin);
	fileBin=NULL;
noerror:
#ifdef __AHOTTS_MT__
#ifdef __OS_UNIX__
	  pthread_mutex_unlock (&hdicmutex_bin);
#endif
#ifdef __OS_WINDOWS__
	  ReleaseMutex(hdicmutex_bin);
#endif
#endif
	return;
}


/**********************************************************/
/* $$$ */

HDicRef HDicDB::searchTxt(char **exp)
{
	int tooLong;
	const CHAR *fstr;  // cadena leida del fichero;
	LONG line, hit_line;  // linea actual y linea del ultimo hit
	size_t len_fstr;

	rewind(fileTxt);
	line=0;
	hit_line=-1;
	len_fstr=0;
	hitBuf[0]='\0';
	while (fgetln_filt(fBuf,HDIC_FTXT_STRLEN,fileTxt,TRUE,FALSE,TRUE,&tooLong)) {
		line++;
		if (tooLong) {  // si la linea es muy larga, mensaje de warning, leemos los restos y saltamos a la siguiente linea
			htts_warn("%s" HDIC_FNAMEEXT_TXT " - line too long (%ld) ",dbname,(long)line);
			while (fgetln_filt(fBuf,HDIC_FTXT_STRLEN,fileTxt,TRUE,FALSE,TRUE,&tooLong))
				if (!tooLong) break;
			continue;
		}
		// si por mucho que busquemos no vamos a encontrar nada mejor porque ya es de longitud maxima, fuera!
		if (hitlen+(hitlonger?1:0)>toklen) break;

		StrList strList(fBuf);
		Lix lix=strList.first();
		fstr=strList.item(lix);
		len_fstr=strlen(fstr);

		// solo interesa la linea leida si la cadena es mas corta que la cadena buscada, y mas larga que lo fijado por el ultimo hitBuf
		if ((len_fstr<=toklen)&&(len_fstr>=hitlen+(hitlonger?1:0))) {
			assert(len_fstr<=HDIC_MAX_STRLEN);
			// comprobamos si es una cadena case sensitive
			BOOL caseSen=FALSE;
			for (lix=strList.next(lix); lix!=0; lix=strList.next(lix)) {
				if (!strcmp(strList.item(lix), HDIC_STR_CASE)) { caseSen=TRUE; break; }
#ifdef HDIC_CASE_FIRST_FLAG
				else break;  // si el flag de casesens debe ser el primero, no comprobar mas.
#endif
			}

			if (caseSen) {
				if (!strncmp(fstr, tok, len_fstr)) {  // encaje case-sensitive
					strcpy(hitBuf,fBuf);  // salvar linea
					hit_line=line;
					hitlonger=TRUE; // ha sido case-sensitive, asi que la proxima debera ser mas larga
					hitlen=len_fstr;  // apuntar longitud hitBuf

					if (len_fstr==toklen) break;  // si ya tenemos la longitud maxima, salir de busqueda
				}
				continue;  // siguiente linea
			}
			// probamos encaje case-insensitive
			strcpy(fstr_nc,fstr);
			chset_StrLower(fstr_nc); // pasar a minusculas
			if (!strncmp(fstr_nc, tokl, len_fstr)) {  // encaje case-insensitive
				strcpy(hitBuf,fBuf);  // salvar linea
				hit_line=line;
				hitlonger=FALSE; // NO ha sido case-sensitive, asi que la proxima puede medir lo mismo
				hitlen=len_fstr;  // apuntar longitud hitBuf

// $$$$ Yon2.
				if (len_fstr==toklen) break;

			}
		}
	}
	if (ferror(fileTxt)) {  // si el fichero no va bien, lo cerramos.
		htts_warn("Invalid file %s" HDIC_FNAMEEXT_TXT " (%ld)", dbname, (long)line);
		fclose(fileTxt);
		fileTxt=NULL;
	}

	HDicRef ref=HDIC_REF_NULL;
	if (hit_line>=0) {
		txt2HDicRef(hitBuf,ref,exp);  // analizar campos de bits
		ref.__setbits(HDIC_QUERY_CASE,hitlonger?1:0);  // salvar case-sensitive
		// salvar longitud del match. 0 si es full




//		if (len_fstr==toklen){
		if (hitlen==toklen){
			ref.__setbits(HDIC_QUERY_MATCHLEN,0);
		}
		else
		{	//luzatzeko garaian hemen sartu
			ref.__setbits(HDIC_QUERY_MATCHLEN,hitlen);
			// si no entra en el campo de bits, lo ponemos a full (mejor que dejarlo indeterminado...)
			if (hitlen!=ref.__getbits(HDIC_QUERY_MATCHLEN)) {
					ref.__setbits(HDIC_QUERY_MATCHLEN,0);
			}
		}
	}
	return ref;
}

/**********************************************************/

long HDicDB::tokbsearch( const CHAR *tok, long base, long nmemb, long size )
{
	long l, u, idx;
	int comparison;
	CHAR buf[HDIC_MAX_STRLEN+1];
	size_t len;
	long hit=-1;

	l = 0;
	u = nmemb;
	while (l < u) {
		idx = (l + u) / 2;
		fseek(fileBin,base+size*idx,SEEK_SET);
		UINT16 u16;
		if (!fread(&u16,sizeof(u16),1,fileBin)) break;
		endian_fromlittle16(&u16); len=(size_t)u16;
		if (len>HDIC_MAX_STRLEN) { htts_warn("Entry too long in HDic dictionary %s" HDIC_FNAMEEXT_BIN, dbname); break; }
		if (!fread(buf,sizeof(CHAR),len+1,fileBin)) break;
		comparison = strncmp(tok,buf,(len<toklen)?(size_t)len:toklen);  // comparar solo lo que solapan
		if (!comparison) {   // si lo que solapa es igual
			if (len>toklen) comparison=-1;  // si es demasiado larga, ir hacia atras
			else {
				if (len>hitlen) { hitlen=len; hit=idx; }  // nos la quedamos si es mas larga que la que ya tenemos
				if (len<toklen) comparison=1; // si es corta, ir hacia delante
				else break; // si miden lo mismo salimos del bucle de busqueda
			}
		}
		if (comparison<0) u=idx; else l=idx+1;
		assert(comparison!=0);
	}
#ifdef DEBUGDIC
	htts_warn("idx: %d buf: %s hit: %d", idx, buf, hit);
#endif
	return hit;
}

/**********************************************************/
