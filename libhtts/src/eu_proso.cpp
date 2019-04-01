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

Nombre fuente................ eu_proso.cpp
Nombre paquete............... aHoTTS
Lenguaje fuente.............. C++
Estado....................... -
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Borja Etxebarria
.............................

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
1.0.5 	 21/06/11  Inaki	Modificar comportamiento etiqueta <prosody rate"1000ms"  HTTS_PROSO_VAL
1.0.4 	 12/07/10  Inaki	 Añadir celdas para modificar la prosodia desde el texto (proyecto Aritz) HTTS_PROSO_VAL
1.0.3    05/01/10  inaki     Añadir modelos de duracion y pitch para Amaia (dur_amaia, pth3_amaia)
1.0.3    20/04/09  inaki     Añadir modelos de duracion para emociones karolina (dur_emo)
1.0.2    20/10/08  inaki     Añadir modelos de duracion para emociones (dur_sad y dur_happy)(Eva)
1.0.1    08/10/01  Yon2.     default dur model Dur2.
1.0.1    08/10/01  Yon2.     default pitch model Pth2.
1.0.0    31/01/00  borja     codefreeze aHoTTS v1.0
0.0.0    24/11/97  borja     Codificacion inicial.

======================== Contenido ========================
<DOC>
Metodos principales de {{LangEU_Prosod}} que llaman a los modelos de
pitch, energia y duracion para obtener la prosodia de una frase.
</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include <assert.h>
#include "httsmsg.h"
#include "eu_lingp.hpp"
#include "xx_uti.hpp"
#include "uti.h"
//#define DEBUG() fprintf (stderr,"file..: %s -- line..: %d\n",__FILE__,__LINE__);
#define DEBUG()

/**********************************************************/

LangEU_Prosod::LangEU_Prosod( VOID )
{
	created=FALSE;

	pth_mean=1;
	pth_dev=0.1;

	dur_mean=1;
	dur_dev=0.1;

	pow_mean=1;
	pow_dev=0.1;

	prosdbname="base.lyp";

/* seleccion de modelos por defecto. El ultimo de cada tipo
es el que se selecciona. */
#ifdef HTTS_PROSOD_EU_DUR1
	dur_model="Dur1";
#endif
#ifdef HTTS_PROSOD_EU_DUR2
	dur_model="Dur2";
#endif
#ifdef HTTS_PROSOD_EU_POW1
	pow_model="Pow1";
#endif
#ifdef HTTS_PROSOD_EU_PTH1
	pth_model="Pth1";
#endif

#ifdef HTTS_PROSOD_EU_PTH1
//	pth1_dummy_ptr=NULL;
#endif
}

/**********************************************************/

LangEU_Prosod::~LangEU_Prosod( VOID )
{
#ifdef HTTS_PROSOD_EU_PTH1
//	if (pth1_dummy_ptr) { xfree(pth1_dummy_ptr); pth1_dummy_ptr=NULL; }
#endif
}

/**********************************************************/

BOOL LangEU_Prosod::create( VOID )
{
#ifdef HTTS_PROSOD_EU_PTH1
//	pth1_dummy_ptr=xmalloc(10);
#endif

	created=TRUE;
	return TRUE;
}

/**********************************************************/

VOID LangEU_Prosod::utt_prosody(UttPh & ut)
{
	assert(created);

	utt_emphasis(ut);  // calcular focos de enfasis

DEBUG()
	// calcular duraciones (ms)
#ifdef HTTS_PROSOD_EU_DUR1
	if (!strcmp(dur_model,"Dur1")) utt_dur1(ut); else
#endif
#ifdef HTTS_PROSOD_EU_DUR2
	if (!strcmp(dur_model,"Dur2")) utt_dur2(ut); else
#endif
	htts_error("Invalid LangEU_Prosod::dur_model (%s)",(const CHAR*)dur_model);

DEBUG()

	// calcular curva de pitch
#ifdef HTTS_PROSOD_EU_PTH1
	if (!strcmp(pth_model,"Pth1")) utt_pth1(ut); else
#endif

	htts_error("Invalid LangEU_Prosod::pth_model (%s)",(const CHAR*)pth_model);

DEBUG()
	// calcular curva de energia
#ifdef HTTS_PROSOD_EU_POW1
	DEBUG()
	if (!strcmp(pow_model,"Pow1")) utt_pow1(ut); else
#endif
	htts_error("Invalid LangEU_Prosod::pow_model (%s)",(const CHAR*)pow_model);
	DEBUG()
}

/**********************************************************/

BOOL LangEU_Prosod::set( const CHAR *param, const CHAR * value )
{
	double d;
	int code = str2d(value,&d);

	if (!strcmp(param,"PthMean"))pth_mean=d;
	else if (!strcmp(param,"PthDev")) pth_dev=d;
	else if (!strcmp(param,"DurMean")) dur_mean=d;
	else if (!strcmp(param,"DurDev")) dur_dev=d;
	else if (!strcmp(param,"PowMean")) pow_mean=d?d:pow_mean;
	else if (!strcmp(param,"PthModel")&& !created) { pth_model=value; return TRUE; }
	else if (!strcmp(param,"DurModel")&& !created) { dur_model=value; return TRUE; }
	else if (!strcmp(param,"PowModel")&& !created) { pow_model=value; return TRUE; }
	
#ifdef HTTS_PROSOD_EU_PTH1
//	else if (!strcmp(param,"Pth1_dummy")) {
//		if (!created) return FALSE;  // if needed
//		// do whatever
//		return TRUE;
//	}
#endif

	else return FALSE;

	if (code) htts_error("LangEU_Prosod: invalid value for %s (%s)",param,value);
	return TRUE;

	
}

/**********************************************************/

const CHAR *LangEU_Prosod::get( const CHAR *param )
{
#define VALRET(x) { sprintf(buf,"%g",(double)x); buf_get=buf; return buf_get; }
	char buf[20];



	if (!strcmp(param,"PthMean")) VALRET(pth_mean);
	if (!strcmp(param,"PthDev")) VALRET(pth_dev);
	if (!strcmp(param,"DurMean")) VALRET(dur_mean);
	if (!strcmp(param,"DurDev")) VALRET(dur_dev);
	if (!strcmp(param,"PowMean")) VALRET(pow_mean);
	if (!strcmp(param,"PowDev")) VALRET(pow_dev);
	if (!strcmp(param,"PthModel")) return pth_model;
	if (!strcmp(param,"DurModel")) return dur_model;
	if (!strcmp(param,"PowModel")) return pow_model;
	//if (!strcmp(param,"ProsDBName")) return prosdbname;

#ifdef HTTS_PROSOD_EU_PTH1
//	if (!strcmp(param,"Pth1_dummy")) return "dummy";
#endif

	return FALSE;
}

/**********************************************************/

				

