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
(C) 1997 TSR/Aholab - ETSII/IT Bilbao (UPV/EHU)

Nombre fuente................ eu_gf.cpp
Nombre paquete............... aHoTTS
Lenguaje fuente.............. C++
Estado....................... -
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Inma Sardon
.............................


======================== Contenido ========================
<DOC>
eu_gf.cpp:programa para separar una utterance en grupos funcionales
</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#ifndef __EU_GF_CPP__
#define __EU_GF_CPP__
#include "eu_pos.hpp"

/**********************************************************/

LangEU_GF::LangEU_GF( VOID )
{
	created=FALSE;
	encontrado=FALSE;
}

/**********************************************************/

LangEU_GF::~LangEU_GF( VOID )
{
}

/**********************************************************/

BOOL LangEU_GF::create( VOID )
{
	created=TRUE;
	return TRUE;
}

/**********************************************************/

BOOL LangEU_GF::set( const CHAR *param, const CHAR * value )
{
	if (!strcmp(param,"HDicDBName")&& !created) { dbName=value; return TRUE; }
	else return FALSE;
//	if (code) htts_error("LangEU_GF: invalid value for %s (%s)",param,value);
//	return TRUE;
}

/**********************************************************/

const CHAR *LangEU_GF::get( const CHAR *param )
{
#define VALRET(x) { sprintf(buf,"%g",(double)x); buf_get=buf; return buf_get; }
	if (!strcmp(param,"HDicDBName")) return dbName;

	return FALSE;
}

/**********************************************************/

VOID LangEU_GF::utt_gf(UttWS &u)
{
	int gf=0;
	int i=0;
	UttI p=NULL;		//incide para recorrer las celdas
		//recorrido palabra por palabra
		for(p=u.wordFirst();p!=0;p=u.wordNext(p)){
			u.cell(p).setFGrp(1);
			indice2=1;
			encontrado=FALSE;
			/*llamadas a funciones*/
			set_inf(u,p);
			set_up_flag(u,p);
			enk(u,p,&gf);

//			fprintf(stderr, "\n enk %s encontrado= %d",u.cell(p).getWord(),encontrado);
			if (encontrado==FALSE){
				detize(u,p,&gf);
			}
//			fprintf(stderr, "\n detize %s encontrado= %d",u.cell(p).getWord(),encontrado);
			if (encontrado==FALSE) {
				izedet(u,p,&gf);
			}
//			fprintf(stderr, "\n izedet %s encontrado= %d",u.cell(p).getWord(),encontrado);
			if (encontrado==FALSE) {
				izeadb(u,p,&gf);
			}
			if (encontrado==FALSE) {
				adbadj(u,p,&gf);
			}

//			fprintf(stderr, "\n izeadb %s encontrado= %d",u.cell(p).getWord(),encontrado);
			if (encontrado==FALSE) {
				izeize(u,p,&gf);
			}
//			fprintf(stderr, "\n izeize %s encontrado= %d",u.cell(p).getWord(),encontrado);
			if (encontrado==FALSE) {
				baitadi(u,p,&gf);
			}
//			fprintf(stderr, "\n baitadi encontrado= %d",encontrado);
			if (encontrado==FALSE) {
				joklgn(u,p,&gf);
			}
//			fprintf(stderr, "\n joklgn %s encontrado= %d",u.cell(p).getWord(),encontrado);
			if (encontrado==FALSE) {
				jokjok(u,p,&gf);
			}
//			fprintf(stderr, "\n trn %s encontrado= %d",u.cell(p).getWord(),encontrado);
			if (encontrado==FALSE) {
				trn(u,p,&gf);
			}
//			fprintf(stderr, "\n invpau %s encontrado= %d",u.cell(p).getWord(),encontrado);
			invpau(u,p,&gf);
//			fprintf(stderr, "\n jnt %s encontrado= %d",u.cell(p).getWord(),encontrado);
			if (encontrado==FALSE) {
				jnt(u,p,&gf);
			}
//		fprintf(stderr,"gf %s encontrado %d gf %d\n",u.cell(p).getWord(),encontrado,indice2);
			for (i=0;i<(indice2-1);i++) {
				p=u.wordNext(p);
			}
		}
		galdeg(u);
#ifdef HTTS_IOTXT
//	u.foutput(stderr);
#endif
}
#endif


