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
#ifndef __HTTSDO_HPP__
#define __HTTSDO_HPP__

/**********************************************************/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/*
(C) 1997 TSR/Aholab - ETSII/IT Bilbao (UPV/EHU)

Nombre fuente................ httsdo.hpp
Nombre paquete............... aHoTTS
Lenguaje fuente.............. C++
Estado....................... -
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Borja Etxebarria
.............................

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
1.0.3	 02/10/11  Inaki     add synthesize API
1.0.1	 15/12/10  Inaki     Añadir Metodo HTS
1.0.1	 03/10/07  Inaki     Añadir Metodo Corpus
1.0.0    31/01/00  borja     codefreeze aHoTTS v1.0
0.0.0    06/02/98  borja     Codificacion inicial.

======================== Contenido ========================
<DOC>
</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include "tdef.h"
#include "htts_cfg.h"

#include "lingp.hpp"
#include "u2w.hpp"

#ifdef HTTS_INTERFACE_WAVEMARKS
#include "mark.hpp"
#endif

#ifdef HTTS_DIPHONE
#include "p2d.hpp"
#include "dphdb.hpp"
#endif

#ifdef HTTS_METHOD_FORMANT
#include "p2f.hpp"
#endif
#ifdef HTTS_METHOD_CORPUS
#include "corpus.hpp" //INAKI
#endif
#ifdef HTTS_METHOD_HTS
#include "hts.hpp"
#endif

#ifdef HTTS_LANG_GL
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

#include "modos.hpp"
#include "tip_var.hpp"
#include "errores.hpp"
#include "leer_frase.hpp"
#include "sep_pal.hpp"
#include "clas_pal.hpp"
#include "preproc.hpp"
#include "sil_acen.hpp"
#include "transcripcion.hpp"
#include "alofonos.hpp"
#include "crea_descriptores.hpp"
#include "estadistica.hpp"
#include "frecuencia.hpp"
#include "audio.hpp"
#include "procesado_senhal.hpp"
#include "modelo_duracion.hpp"
#include "morfolo.hpp"
#include "categorias_analisis_morfosintactico.hpp"
#include "perfhash.hpp"
#include "modelo_lenguaje.hpp"
#include "matriz.hpp"
#include "path_list.hpp"
#include "Viterbi_categorias.hpp"
#include "analisis_morfosintactico.hpp"
//#include "sintesis_difonemas.hpp"
#include "sintagma.hpp"
#include "pausas.hpp"
#include "trat_fon.hpp"
#include "prosodia.hpp"
#include "info_corpus_voz.hpp"
#include "matriz.hpp"
#include "cache.hpp"
#include "info_corpus_voz.hpp"
#include "energia.hpp"
#include "red_neuronal.hpp"
#include "util_neuronal.hpp"
#include "modelo_duracion.hpp"
#include "grupos_acentuales.hpp"
#include "distancia_espectral.hpp"
#include "locutor.hpp"
#include "Viterbi_acentual.hpp"
#include "Viterbi.hpp"
#include "info_estructuras.hpp"
#include "seleccion_unidades.hpp"
#include "gbm_locuciones.hpp"
//#include "difonemas.hpp"
#include "configuracion.hpp"
#include "rupturas_entonativas.hpp"
#include "cotovia.hpp"
#endif
/**********************************************************/


#ifdef USE_TOKENIZER
#include "t2l.hpp"
#include "hdic.hpp"
#else
#include "t2u.hpp"
#include "tfil.hpp"
#endif

//#if defined HTTS_LANG_CAT || defined HTTS_LANG_EN || defined HTTS_LANG_GAL
#ifdef HTTS_LANG_FEST
#include "festival.h"
#endif
/**********************************************************/
#ifdef HTTS_LANG_GL
#include "xx_uti.hpp"
class LingCotovia:public Cotovia
{

	int Inicio(char * entrada);
	void Fin(void);

	public:
	int lingCotovia(char *entrada, t_opciones opc, UttPh& u, HTS_U2W  *u2w, BOOL INICIALIZACION);
	String gl_labels;
};
#endif

class HTTSDo {
private:
	BOOL created;
	INT flushbuf;
#ifdef USE_TOKENIZER
	TextToList *t2u;
	HDicDB * hdic;
	String hdicdbname;
#else
	T2ULst * t2u;
	TxtFilt * tfil;
	String sustfn;
#endif
	LingP * lingp;
	Utt2Wav * u2w;
	Utt * utt;
	BOOL localdb;
#ifdef HTTS_DIPHONE
	Ph2Dph * p2d;
	DphDB * dphdb;
	String dbname;
	String dbmode;
#endif
#ifdef HTTS_METHOD_FORMANT
	Ph2FormP *p2f;
	BOOL formant;
#endif
#ifdef HTTS_METHOD_CORPUS
	BOOL corpus;
#endif
#ifdef HTTS_METHOD_HTS
	BOOL hts;
	char DataPath[1024];
#endif
#ifdef HTTS_LANG_GL
	LingCotovia *ling_cotovia;
#endif

	String lang;
	String emo;
	String emoint;
	String smethod;
	String modelpth;
	String dbpros;
	String modelpow;
	String modeldur;
	String modelpau;

	BOOL ackpending;

	BOOL advance( VOID );
	VOID destroy( VOID );

public:
	HTTSDo( VOID );
	virtual ~HTTSDo( );
	BOOL create( VOID *db=NULL );
	VOID *getDB(VOID);

	INT input( const CHAR * str );
	BOOL input( CHAR ch );
	BOOL flush( VOID );
	const DOUBLE* output( INT *len, BOOL *flush, INT mode, VOID *cb_n );
	BOOL isFlush( VOID );


	/**************/
	//inaki
	BOOL synthesize_do_input( const CHAR *str, const CHAR *lang , BOOL InputIsFile, const CHAR *data_path);
	int synthesize_do_next_sentence(  const CHAR *lang , short **samples);//procesa frase
#ifdef HTTS_LANG_FEST
	int str2num(const char * cadena);
	char *num2str(int num);
	//Variables necesarias
	EST_TokenStream ts;
	LISP eou_tree;
	LISP stream;
	//friend LISP tts_chunk_stream_aho(EST_TokenStream &ts,
	//	      LISP eou_tree,
	//	      LISP utt, String &label_string, Utt2Wav * u2w);
#endif

	/*************/
#ifdef HTTS_INTERFACE_WAVEMARKS
	const Mark1DList & mrkget( VOID );
	VOID mrkdel( VOID );
	BOOL outack( INT n, BOOL mrkdel );
#else
	BOOL outack( INT n );
#endif

#ifdef HTTS_TIMEEVS
	INT get_timeev( VOID );
#endif

	virtual VOID callback( VOID * /*cb_n*/ ) { };

	BOOL set( const CHAR* param, const CHAR* val );
	const CHAR* get( const CHAR* param );


};

/**********************************************************/

#endif
