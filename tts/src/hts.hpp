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
#ifndef __HTSSYN_HPP__
#define __HTSSYN_HPP__
/**********************************************************/
//    FUNCIONES PARA LA síntesis por HMM utilizando HTS-ENGINE
/**********************************************************/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/*
(C) 1997 TSR/Aholab - ETSII/IT Bilbao (UPV/EHU)

Nombre fuente................ hts.hpp
Nombre paquete............... aHoTTS
Lenguaje fuente.............. C++
Estado....................... -
Dependencia Hard/OS.......... -
Codigo condicional........... -



Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
0.0.2    08/11/11	Inaki     Funcion xinput_labels para sintetizar a partir de labels
0.0.1    10/02/11	Inaki     Añadir stream de Frequency Voicing para AhoCoder
0.0.0    15/12/10	Inaki     Codificacion inicial.

======================== Contenido ========================
<DOC>
</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/
#include "htts_cfg.h"
#ifdef HTTS_METHOD_HTS
//#ifdef WIN32
#ifdef __cplusplus
extern "C"{
#include "HTS_engine.h"
}
#else
#include "HTS_engine.h"
#endif
#include "u2w.hpp"
#include "kindof.hpp"
#include "tdef.h"
#include "mark.hpp"

#include "strl.hpp"
#include "uttws.hpp"
#include "uttph.hpp"
//#include "uttdph.hpp"
//#include "uttps.hpp"
#include "eu_hdic.hpp"
#include "eu_lingp.hpp"
//#include "en_lingp.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "str2win.h"
//#include "aholib.hpp"
#include "uti.h"
class HTS_U2W : public Utt2Wav {
protected:
	char *Language;
	//parametros de HTS
	/* engine */
    /* number of speakers for interpolation */
   int num_interp;
    /* number of each models for interpolation */
   int num_ms_lf0, num_ms_mcp, num_ms_dur;
   /* number of each trees for interpolation */
   int num_ts_lf0, num_ts_mcp, num_ts_dur;
   int num_ms_gvl, num_ms_gvm;
   int num_ts_gvl, num_ts_gvm;

   /* global parameter */
   int sampling_rate;
   int fperiod;
   double alpha;
   double stage;          /* gamma = -1.0/stage */
   double beta;
   int audio_buff_size;
   double uv_threshold;
   double gv_weight_lf0;
   double gv_weight_mcp;
   double gv_weight_exc;
   double half_tone;
   HTS_Boolean phoneme_alignment;
   double speech_speed;
   HTS_Boolean use_log_gain;


   #ifndef HTS_EMBEDDED
   FILE *lf0fp, *mcpfp;
#endif                          /* !HTS_EMBEDDED */
   FILE *durfp, *wavfp, *rawfp, *tracefp, *excfp;

  	UttPh *ut;
	int SRate;

	/********sintesis*//////////
	//CAudioFile h;					//.wav de salida, .wav donde se encuentran los semifonemas
	INT16 muestra;					//para leer muestras de ficheros

	long num_muestras;			//numero de muestras del resultado = columnas de array

	/* file name of global variance switch */
   char *fn_gv_switch;
	/* file names of models */
   char **fn_ms_lf0;
   char **fn_ms_mcp;
   char **fn_ms_dur;
   char **fn_ms_exc;
   /* file names of trees */
   char **fn_ts_lf0;
   char **fn_ts_mcp;
   char **fn_ts_dur;
   char **fn_ts_exc;
   /* file names of windows */
   char **fn_ws_lf0;
   char **fn_ws_mcp;
   char **fn_ws_exc;
   int num_ws_lf0, num_ws_mcp, num_ws_exc;
    /* file names of global variance */
   char **fn_ms_gvl;
   char **fn_ms_gvm;
   char **fn_ms_gve;
   /* file names of global variance trees */
   char **fn_ts_gvl;
   char **fn_ts_gvm;
   char **fn_ts_gve;

	BOOL HTS_ENGINE_INITIALIZED; //cuando está deshabilitado cargamos toda la configuración
#ifdef HTTS_INTERFACE_WAVEMARKS
  String markMode;
  BOOL mrkUsePrefix; // prefijos de tipo a cada marca
  BOOL mrkOnDph; //marcas en cada difonema
  BOOL mrkOnPh; //marcas en cada fonema
  BOOL mrkOnDOW;
  BOOL mrkOnOLA;

#endif
	String labels_string;
	HTS_Engine engine;
protected:



  virtual VOID reset (VOID);
  BOOL xinput (Utt *u);
  virtual BOOL doNext (BOOL flush);
  //virtual VOID shiftedWav (INT n);

  //funciones HTS_engine


public:
   HTS_U2W ( VOID );
  ~HTS_U2W ( );
   virtual BOOL create (const char * lang);
	//FUNCIONES
  short * xinput_labels (String labels, int * num_samples);
  void pho2hts(UttPh *u, String &labels, BOOL setdur); //devuelve la salida en labels
   virtual BOOL set (const CHAR * param, const CHAR* val);
  const CHAR* get (const CHAR * param);
  virtual VOID shiftedWav( INT n );

private:
	// funciones auxiliares de pho2hts
	void phone2sampa(char *phoneme, UttPh *u, Lix p);
	void pos_syl(unsigned char &pos_left, unsigned char &pos_right, UttPh *u, Lix p, char *phoneme_prev, char *phoneme_next);
	void pos_pau(unsigned short &pos_left, unsigned short &pos_right, UttPh *u, Lix p);
	void syllable_stress(char &syl_left_stress, char &syl_stress, char &syl_right_stress, UttPh *u, Lix p);
	void syllable_emphasis(char &syl_left_emphasis, char &syl_emphasis, char &syl_right_emphasis, UttPh *u, Lix p);
	void syllable_num_phone(unsigned char &syl_left_num_phone, unsigned char &syl_num_phone, unsigned char &syl_right_num_phone, UttPh *u, Lix p);
	void syllable_pos_word(unsigned char &syl_pos_word_left, unsigned char &syl_pos_word_right, UttPh *u, Lix p);
	void syllable_pos_ag(unsigned char &syl_pos_ag_left, unsigned char &syl_pos_ag_right, UttPh *u, Lix p);
	void syllable_pos_sentence(unsigned short &syl_pos_sentence_left, unsigned short &syl_pos_sentence_right, UttPh *u, Lix p);
	void syllable_pos_pause(unsigned short &syl_pos_pause_left, unsigned short &syl_pos_pause_right, UttPh *u, Lix p);
	char get_POS(UttPh *u, Lix p);
	void word_POS(char &word_POS_left, char &word_POS, char &word_POS_right, UttPh *u, Lix p);
	void word_num_syl(char &word_num_syl_left, char &word_num_syl, char &word_num_syl_right, UttPh *u, Lix p);
	void word_pos_sentence(unsigned char &word_pos_sentence_left, unsigned char &word_pos_sentence_right, UttPh *u, Lix p);
	void word_pos_pause(unsigned char &word_pos_pause_left, unsigned char &word_pos_pause_right, UttPh *u, Lix p);
	char get_ga(UttPh *u, Lix p);
	void ag_type(char &ga_type_left, char &ga_type, char &ga_type_right, UttPh *u, Lix p);
	void ag_num_syl(unsigned char &ag_num_syl_left, unsigned char &ag_num_syl, unsigned char &ag_num_syl_right, UttPh *u, Lix p);
	void ag_pos_sentence(unsigned char &ag_pos_sentence_left, unsigned char &ag_pos_sentence_right, UttPh *u, Lix p);
	void ag_pos_pause(unsigned char &ag_pos_pause_left, unsigned char &ag_pos_pause_right, UttPh *u, Lix p);
	char get_pau(UttPh *u, Lix p);
	void pau_type(char &pau_type_left, char &pau_type_right, UttPh *u, Lix p);
	void pau_pos_sentence(unsigned char &pau_pos_sentence_left, unsigned char &pau_pos_sentence_right, UttPh *u, Lix p);
	char get_sentence(UttPh *u, Lix p);
	char sentence_type(UttPh *u, Lix p);

};
#endif
#endif

