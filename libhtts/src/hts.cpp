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
//    FUNCIONES PARA LA SINTESIS CON HMM
/**********************************************************/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/*
(C) 1997 TSR/Aholab - ETSII/IT Bilbao (UPV/EHU)

Nombre fuente................ hts.cpp
Nombre paquete............... aHoTTS
Lenguaje fuente.............. C++
Estado....................... -
Dependencia Hard/OS.......... -a
Codigo condicional........... -



Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
0.0.2    08/11/11	Inaki     Funcion xinput_labels para sintetizar a partir de labels
0.0.1    12/02/11	Inaki     Añadir stream para Frequency Voicing del Ahocoder
0.0.0    15/12/10	Inaki     Codificacion inicial.

======================== Contenido ========================
<DOC>
</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

/************************************************************************************************************************/
#include "hts.hpp"
#ifdef HTTS_METHOD_HTS
#ifdef WIN32
//no esta definida la función round
inline double round( double d ){
		return floor( d + 0.5 );
}
#endif
HTS_U2W::HTS_U2W(VOID )
{
    //inicializar variables

   /* global parameter */
   sampling_rate = 16000;
   fperiod = 80;
   alpha = 0.42;
   stage = 0.0;          /* gamma = -1.0/stage */
   beta = 0.0;
   audio_buff_size = 1600;
   uv_threshold = 0.5;
   gv_weight_lf0 = 0.7;
   gv_weight_exc = 0.7;
   gv_weight_mcp = 1.0;
   num_ws_lf0=0, num_ws_mcp=0, num_ws_exc=0;
   half_tone = 0.0;
   phoneme_alignment = FALSE;
   speech_speed = 1.0;
   use_log_gain = FALSE;
   fn_ms_gvl = NULL;
   fn_ms_gve = NULL;
   fn_ms_gvm = NULL;
   fn_ts_gvl = NULL;
   fn_ts_gve = NULL;
   fn_ts_gvm = NULL;
   fn_gv_switch = NULL;
   fn_ws_lf0 = NULL;
   fn_ws_mcp = NULL;
   fn_ws_exc = NULL;
   #ifndef HTS_EMBEDDED
   lf0fp = NULL, mcpfp = NULL;
#endif                          /* !HTS_EMBEDDED */
   durfp = NULL, wavfp = NULL, rawfp = NULL, tracefp = NULL, excfp = NULL;
    /* number of speakers for interpolation */
   num_interp=1;
    /* number of each models for interpolation */
   num_ms_lf0=1, num_ms_mcp=1, num_ms_dur=1;
   /* number of each trees for interpolation */
   num_ts_lf0=1, num_ts_mcp=1, num_ts_dur=1;
   num_ws_lf0=3, num_ws_mcp=3, num_ws_exc =3;
   num_ms_gvl=1, num_ms_gvm=1;
   num_ts_gvl=1, num_ts_gvm=1;
   fn_ms_lf0 = (char **) calloc(num_interp, sizeof(char *));
   fn_ms_mcp = (char **) calloc(num_interp, sizeof(char *));
   fn_ms_dur = (char **) calloc(num_interp, sizeof(char *));
   fn_ms_exc = (char **) calloc(num_interp, sizeof(char *));
   fn_ts_lf0 = (char **) calloc(num_interp, sizeof(char *));
   fn_ts_mcp = (char **) calloc(num_interp, sizeof(char *));
   fn_ts_dur = (char **) calloc(num_interp, sizeof(char *));
   fn_ts_exc = (char **) calloc(num_interp, sizeof(char *));
   fn_ms_gvl = (char **) calloc(num_interp, sizeof(char *));
   fn_ms_gvm = (char **) calloc(num_interp, sizeof(char *));
   fn_ms_gve = (char **) calloc(num_interp, sizeof(char *));
   fn_ts_gvl = (char **) calloc(num_interp, sizeof(char *));
   fn_ts_gvm = (char **) calloc(num_interp, sizeof(char *));
   fn_ts_gve = (char **) calloc(num_interp, sizeof(char *));
   /* delta window handler for log f0 */
   fn_ws_lf0 = (char **) calloc(num_ws_lf0, sizeof(char *)); //fuerzo a 3
   /* delta window handler for mel-cepstrum */
	fn_ws_mcp = (char **) calloc(num_ws_mcp , sizeof(char *));
   /* delta window handler for mel-cepstrum */
	fn_ws_exc = (char **) calloc(num_ws_exc , sizeof(char *));
	HTS_ENGINE_INITIALIZED = FALSE;

#ifdef HTTS_INTERFACE_WAVEMARKS
    markMode="";
    mrkOnDOW = FALSE;
    mrkOnDph = FALSE;
    mrkOnPh = FALSE;
    mrkOnOLA = FALSE;
    mrkUsePrefix = FALSE;
#endif

}
/************************************************************************************************************************/

HTS_U2W::~HTS_U2W( VOID )
{
   //liberar memoria
	free(Language);
	 // free memory
	HTS_Engine_clear(&engine);
	//free(rate_interp);
	if(fn_ws_mcp)
		free(*fn_ws_mcp);
	if(fn_ws_lf0)
		free(*fn_ws_lf0);
	if(fn_ws_exc)
		free(*fn_ws_exc);
	free(*fn_ms_mcp);
	free(*fn_ms_lf0);
	free(*fn_ms_exc);
	free(*fn_ms_dur);
	free(*fn_ts_mcp);
	free(*fn_ts_lf0);
	free(*fn_ts_dur);
	free(*fn_ts_exc);
	free(*fn_ms_gvm);
	free(*fn_ms_gvl);
	free(*fn_ms_gve);
	free(*fn_ts_gvm);
	free(*fn_ts_gvl);
	free(*fn_ts_gve);
	// close files
#ifndef HTS_EMBEDDED
   if (mcpfp != NULL)
      fclose(mcpfp);
   if (lf0fp != NULL)
      fclose(lf0fp);
#endif                          // !HTS_EMBEDDED
   if (wavfp != NULL)
      fclose(wavfp);
   if (rawfp != NULL)
      fclose(rawfp);
   if (durfp != NULL)
      fclose(durfp);
   if (excfp != NULL)
      fclose(excfp);
   if (tracefp != NULL)
      fclose(tracefp);

}
/************************************************************************************************************************/
BOOL HTS_U2W::create( const char * Lang)
{

    if (!xcreate()) return FALSE;

#ifdef HTTS_INTERFACE_WAVEMARKS
    mrkl.setSRate(SRate);  // configurar frec. muestreo en lista de marcas //corregir lo de 16000
#endif

	Language=strdup(Lang);
    reset();


    return TRUE;
}
/************************************************************************************************************************/

VOID HTS_U2W::reset (VOID)
{
    ut=NULL;
    //borrar memoria intermedias

    //resetar variables

}
/************************************************************************************************************************/

BOOL HTS_U2W::set( const CHAR* param, const CHAR* val )
{
#ifdef HTTS_INTERFACE_WAVEMARKS
    if (!strcmp(param,"MarkMode")) {
        markMode = val;
        mrkOnDOW = (strstr(val,"DOW")!=NULL);
        mrkOnDph = (strstr(val,"Dph")!=NULL);
        mrkOnOLA = (strstr(val,"OLA")!=NULL);
        mrkOnPh = (strstr(val,"Ph")!=NULL);
        mrkUsePrefix = (strstr(val,"Prefix")!=NULL);
        return TRUE;
    }
#endif
	//asignar valores
    if (!strcmp(param,"td")) {      //decision tree files for state duration
		fn_ts_dur[0]=strdup(val);
		return TRUE;
    }
    else  if (!strcmp(param,"tf")) {      //decision tree files for Log f0
		fn_ts_lf0[0]=strdup(val);
		return TRUE;
    }
	else  if (!strcmp(param,"tm")) {      //decision tree files for spectrum
		fn_ts_mcp[0]=strdup(val);
		return TRUE;
    }
	else  if (!strcmp(param,"te")) {      //decision tree files for excitation
		fn_ts_exc[0]=strdup(val);
		return TRUE;
    }

    else if (!strcmp(param,"md")) {      //model files for state duration
		fn_ms_dur[0]=strdup(val);
		return TRUE;
    }
    else  if (!strcmp(param,"mf")) {      //model files for Log f0
		fn_ms_lf0[0]=strdup(val);
		return TRUE;
    }
	else  if (!strcmp(param,"mm")) {      //model files for spectrum
		fn_ms_mcp[0]=strdup(val);
		return TRUE;
    }
    else  if (!strcmp(param,"me")) {      //model files for excitation
		fn_ms_exc[0]=strdup(val);
		return TRUE;
    }

	else  if (!strcmp(param,"df")) {      //window files for calculation delta of Log F0
		fn_ws_lf0[0]=strdup(val);
		return TRUE;
    }
    else  if (!strcmp(param,"df2")) {      //window files for calculation delta of Log F0
		fn_ws_lf0[1]=strdup(val);
		return TRUE;
    }
    else  if (!strcmp(param,"df3")) {      //window files for calculation delta of Log F0
		fn_ws_lf0[2]=strdup(val);
		return TRUE;
    }
	else  if (!strcmp(param,"dm")) {      //window files for calculation delta of spectrum
		fn_ws_mcp[0]=strdup(val);
		return TRUE;
    }
	else  if (!strcmp(param,"dm2")) {      //window files for calculation delta of spectrum
		fn_ws_mcp[1]=strdup(val);
		return TRUE;
    }
	else  if (!strcmp(param,"dm3")) {      //window files for calculation delta of spectrum
		fn_ws_mcp[2]=strdup(val);
		return TRUE;
    }
    else  if (!strcmp(param,"de")) {      //window files for calculation delta of excitation
		fn_ws_exc[0]=strdup(val);
		return TRUE;
    }
	else  if (!strcmp(param,"de2")) {      //window files for calculation delta of excitation
		fn_ws_exc[1]=strdup(val);
		return TRUE;
    }
	else  if (!strcmp(param,"de3")) {      //window files for calculation delta of excitation
		fn_ws_exc[2]=strdup(val);
		return TRUE;
    }

	else  if (!strcmp(param,"od")) {      // filename of output label with duration
		durfp=fopen(val, "wt");
		return TRUE;
	}
	else  if (!strcmp(param,"of")) {      // filename of output Log F0
		lf0fp=fopen(val,"wb");
		return TRUE;
	}
	else  if (!strcmp(param,"om")) {      // filename of output spectrum
		mcpfp=fopen(val,"wb");
		return TRUE;
	}
	else  if (!strcmp(param,"ow")) {      // filename of output wav audio (generated speech)
		wavfp=fopen(val, "wb");
		return TRUE;
	}
	else  if (!strcmp(param,"ot")) {      // filename of output trace information
		tracefp=fopen(val, "wt");
		return TRUE;
	}
	else  if (!strcmp(param,"oe")) {      // filename of output excitation
		excfp=fopen(val, "wt");
		return TRUE;
	}

	else if(!strcmp(param,"s")){		//sampling frequency
		str2i(val, &sampling_rate);
		return TRUE;
	}
	else if(!strcmp(param,"p")){		//frame period (point)
		str2i(val, &fperiod);
		return TRUE;
	}
	else if(!strcmp(param,"a")){		//all-pass constant
		str2d(val, &alpha);
		return TRUE;
	}
	else if(!strcmp(param,"g")){		//         gamma = -1 / i (if i=0 then gamma=0)
		str2d(val, &stage);
		return TRUE;
	}
	else if(!strcmp(param,"b")){		//         postfiltering coefficient
		str2d(val, &beta);
		return TRUE;
	}
	else if(!strcmp(param,"r")){		//         speech speed rate
		str2d(val, &speech_speed);
		return TRUE;
	}
	else if(!strcmp(param,"fm")){		//         add half tone
		str2d(val, &half_tone);
		if(half_tone < -24.0)
			half_tone=-24.0;
		if(half_tone > 24.0)
			half_tone=24.0;
		return TRUE;
	}
	else if(!strcmp(param,"u")){		//         voiced/unvoiced threshold
		str2d(val, &uv_threshold);
		return TRUE;
	}

    else  if (!strcmp(param,"ef")) {      //decision tree files for GV of Log F0
		fn_ts_gvl[0]=strdup(val);
		return TRUE;
    }
	else  if (!strcmp(param,"em")) {      //decision tree files for GV of spectrum
		fn_ts_gvm[0]=strdup(val);
		return TRUE;
    }
	else  if (!strcmp(param,"ee")) {      //decision tree files for GV of excitation
		fn_ts_gve[0]=strdup(val);
		return TRUE;
    }
    else  if (!strcmp(param,"cf")) {      //filenames of GV of Log F0
		fn_ms_gvl[0]=strdup(val);
		return TRUE;
    }
	else  if (!strcmp(param,"cm")) {      //filename of GV for spectrum
		fn_ms_gvm[0]=strdup(val);
		return TRUE;
    }
	else  if (!strcmp(param,"ce")) {      //filename of GV for excitation
		fn_ms_gve[0]=strdup(val);
		return TRUE;
    }
    else if(!strcmp(param,"jl")){		//weight of GV for Log F0
		str2d(val, &gv_weight_lf0);
		return TRUE;
	}
	else if(!strcmp(param,"jm")){		//weight of GV for spectrum
		str2d(val, &gv_weight_mcp);
		return TRUE;
	}
	else if(!strcmp(param,"je")){		//weight of GV for excitation
		str2d(val, &gv_weight_exc);
		return TRUE;
	}

	else if (!strcmp(param,"k")){		//  GV switch
		fn_gv_switch=strdup(val);
		return TRUE;
	}
	else if (!strcmp(param, "z")){		//Audio buffer size
		str2i(val, &audio_buff_size);
		return TRUE;
	}
	else if (!strcmp(param, "vp")){ 	//phoneme alignment
		if(str2bool(val, TRUE))
			phoneme_alignment=TRUE;
		else
			phoneme_alignment=FALSE;
		return TRUE;
	}
	else if (!strcmp(param, "voice_path")){ //a partir del dicho path, impone los nombres por defecto del resto d
						//ficheros necesarios para HTS
		char tmp [5000];
		strcpy(tmp, val);
		int len = strlen(tmp);
		strcpy(tmp+len, "tree-dur.inf");
		fn_ts_dur[0]=strdup(tmp);

		strcpy(tmp+len, "tree-lf0.inf");
		fn_ts_lf0[0]=strdup(tmp);

		strcpy(tmp+len, "tree-mgc.inf");
		fn_ts_mcp[0]=strdup(tmp);

		strcpy(tmp+len, "tree-bap.inf");
		fn_ts_exc[0]=strdup(tmp);

		strcpy(tmp+len, "dur.pdf");
		fn_ms_dur[0]=strdup(tmp);

		strcpy(tmp+len, "lf0.pdf");
		fn_ms_lf0[0]=strdup(tmp);

		strcpy(tmp+len, "mgc.pdf");
		fn_ms_mcp[0]=strdup(tmp);

		strcpy(tmp+len, "bap.pdf");
		fn_ms_exc[0]=strdup(tmp);

		strcpy(tmp+len, "tree-gv-lf0.inf");
		fn_ts_gvl[0]=strdup(tmp);

		strcpy(tmp+len, "tree-gv-mgc.inf");
		fn_ts_gvm[0]=strdup(tmp);

		strcpy(tmp+len, "tree-gv-bap.inf");
		fn_ts_gve[0]=strdup(tmp);

		strcpy(tmp+len, "gv-lf0.pdf");
		fn_ms_gvl[0]=strdup(tmp);

		strcpy(tmp+len, "gv-mgc.pdf");
		fn_ms_gvm[0]=strdup(tmp);

		strcpy(tmp+len, "gv-bap.pdf");
		fn_ms_gve[0]=strdup(tmp);

		strcpy(tmp+len, "gv-switch.inf");
		fn_gv_switch=strdup(tmp);
		strcpy(tmp+len, "lf0.win1");
		fn_ws_lf0[0]=strdup(tmp);

		strcpy(tmp+len, "lf0.win2");
		fn_ws_lf0[1]=strdup(tmp);

		strcpy(tmp+len, "lf0.win3");
		fn_ws_lf0[2]=strdup(tmp);

		strcpy(tmp+len, "mgc.win1");
		fn_ws_mcp[0]=strdup(tmp);

		strcpy(tmp+len, "mgc.win2");
		fn_ws_mcp[1]=strdup(tmp);

		strcpy(tmp+len, "mgc.win3");
		fn_ws_mcp[2]=strdup(tmp);

		strcpy(tmp+len, "bap.win1");
		fn_ws_exc[0]=strdup(tmp);

		strcpy(tmp+len, "bap.win2");
		fn_ws_exc[1]=strdup(tmp);

		strcpy(tmp+len, "bap.win3");
		fn_ws_exc[2]=strdup(tmp);
	return TRUE;
	}
    return FALSE;
}

/**********************************************************/

const CHAR* HTS_U2W::get( const CHAR* param )
{
#define VALRET(x) {  const char *buf_get; char buf[20]; sprintf(buf,"%g",(double)x); buf_get=buf; return buf_get; }

#ifdef HTTS_INTERFACE_WAVEMARKS
    if (!strcmp(param,"MarkMode")) return (const CHAR*)markMode;
#endif

    if (!strcmp(param,"td")) return (const char*)fn_ts_dur[0];
	else if (!strcmp(param,"tf")) return (const char*)fn_ts_lf0[0];
	else if (!strcmp(param,"tm")) return (const char*)fn_ts_mcp[0];
	else if (!strcmp(param,"te")) return (const char*)fn_ts_exc[0];

	else if (!strcmp(param,"md")) return (const char*)fn_ms_dur[0];
	else if (!strcmp(param,"mf")) return (const char*)fn_ms_lf0[0];
	else if (!strcmp(param,"mm")) return (const char*)fn_ms_mcp[0];
	else if (!strcmp(param,"me")) return (const char*)fn_ms_exc[0];

	else if (!strcmp(param,"df")) return (const char*)fn_ws_lf0[0];
	else if (!strcmp(param,"dm")) return (const char*)fn_ws_mcp[0];
	else if (!strcmp(param,"de")) return (const char*)fn_ws_exc[0];

	else if(!strcmp(param,"s")){ VALRET(sampling_rate);}
	else if(!strcmp(param,"p")){ VALRET(fperiod);}
	else if(!strcmp(param,"a")){ VALRET(alpha);}
	else if(!strcmp(param,"g")){ VALRET(stage);}
	else if(!strcmp(param,"b")){ VALRET(beta);}
	else if(!strcmp(param,"r")){ VALRET(speech_speed);}
	else if(!strcmp(param,"fm")){ VALRET(half_tone);}
	else if(!strcmp(param,"u")){ VALRET(uv_threshold);}

	else if (!strcmp(param,"ef")) return (const char*)fn_ts_gvl[0];
	else if (!strcmp(param,"em")) return (const char*)fn_ts_gvm[0];
	else if (!strcmp(param,"ee")) return (const char*)fn_ts_gve[0];
	else if (!strcmp(param,"cf")) return (const char*)fn_ms_gvl[0];
	else if (!strcmp(param,"cm")) return (const char*)fn_ms_gvm[0];
	else if (!strcmp(param,"ce")) return (const char*)fn_ms_gve[0];
	else if(!strcmp(param,"jl")){ VALRET(gv_weight_lf0);}
	else if(!strcmp(param,"jm")){ VALRET(gv_weight_mcp);}
	else if(!strcmp(param,"je")){ VALRET(gv_weight_exc);}

	else if (!strcmp(param,"k")) return (const char*)fn_gv_switch;
	else if (!strcmp(param,"z")) { VALRET(audio_buff_size); }
	else if (!strcmp(param,"vp")) return bool2str(phoneme_alignment);

    //if (!strcmp(param,"ModifDur")) return bool2str(MODIF_DUR);

    return NULL;
}
/**********************************************************/

/************************************************************************************************************************/
BOOL HTS_U2W::xinput (Utt *u) {
	//fprintf(stderr,"HTS_U2W::xinput()\n");
    if (ut) {
		return FALSE; //si ya tenemos una frase, no se aceptan mas
    }
	if (!HTS_ENGINE_INITIALIZED ){
		 /* initialize (stream[0] = spectrum , stream[1] = lf0) */
		int with_excitation = 0;
		FILE *tmp;
		tmp=fopen(fn_ts_exc[0], "rb");
		if ( tmp != NULL){
			fclose(tmp);
			with_excitation = 1;
			HTS_Engine_initialize(&engine, 3);
		}
		else{
			HTS_Engine_initialize(&engine, 2);
		}
		/* load duration model */
		HTS_Engine_load_duration_from_fn(&engine, fn_ms_dur, fn_ts_dur, num_interp);
		/* load stream[0] (spectrum model) */
		HTS_Engine_load_parameter_from_fn(&engine, fn_ms_mcp, fn_ts_mcp, fn_ws_mcp,
										 0, FALSE, num_ws_mcp, num_interp);
		/* load stream[1] (lf0 model) */
		HTS_Engine_load_parameter_from_fn(&engine, fn_ms_lf0, fn_ts_lf0, fn_ws_lf0,
										 1, TRUE, num_ws_lf0, num_interp);
		/* load gv[0] (GV for spectrum) */
		HTS_Engine_load_gv_from_fn(&engine, fn_ms_gvm, fn_ts_gvm, 0,
										num_interp);
		/* load gv[1] (GV for lf0) */
		HTS_Engine_load_gv_from_fn(&engine, fn_ms_gvl, fn_ts_gvl, 1,
										num_interp);
		if ( with_excitation == 1 ){
			/* load stream[2] (excitation model) */
			HTS_Engine_load_parameter_from_fn(&engine, fn_ms_exc, fn_ts_exc, fn_ws_exc, 2, FALSE, num_ws_exc, num_interp);
			HTS_Engine_load_gv_from_fn(&engine, fn_ms_gve, fn_ts_gve, 2, num_interp);
		}
		/* load GV switch */
		if (fn_gv_switch != NULL)
			HTS_Engine_load_gv_switch_from_fn(&engine, fn_gv_switch);
		/* set parameter */
		HTS_Engine_set_sampling_rate(&engine, sampling_rate);
		HTS_Engine_set_fperiod(&engine, fperiod);
		HTS_Engine_set_alpha(&engine, alpha);
		HTS_Engine_set_gamma(&engine, stage);
		HTS_Engine_set_log_gain(&engine, use_log_gain);
		HTS_Engine_set_beta(&engine, beta);
		HTS_Engine_set_audio_buff_size(&engine, audio_buff_size);
		HTS_Engine_set_msd_threshold(&engine, 1, uv_threshold);      /* set voiced/unvoiced threshold for stream[1] */
		HTS_Engine_set_gv_weight(&engine, 0, gv_weight_mcp);
		HTS_Engine_set_gv_weight(&engine, 1, gv_weight_lf0);
		if ( with_excitation == 1 )
			HTS_Engine_set_gv_weight(&engine, 2, gv_weight_exc);

		//int i;
		//for (i = 0; i < num_interp; i++) {
			HTS_Engine_set_duration_interpolation_weight(&engine, 0, 1.0);
			HTS_Engine_set_parameter_interpolation_weight(&engine, 0, 0,
														1.0);
			HTS_Engine_set_parameter_interpolation_weight(&engine, 1, 0,
														1.0);
			if ( with_excitation == 1 )
				HTS_Engine_set_parameter_interpolation_weight(&engine, 2, 0, 1.0);
		//}
		if (num_interp == num_ms_gvm)
			//for (i = 0; i < num_interp; i++)
				HTS_Engine_set_gv_interpolation_weight(&engine, 0, 0, 1.0);
		if (num_interp == num_ms_gvl)
			//for (i = 0; i < num_interp; i++)
				HTS_Engine_set_gv_interpolation_weight(&engine, 1, 0, 1.0);
		if (with_excitation == 1)
			HTS_Engine_set_gv_interpolation_weight(&engine, 2, 0, 1.0);

		HTS_ENGINE_INITIALIZED = TRUE;
	}

    assert (u->isKindOf("UttPh"));
    ut=(UttPh*)u;

#ifdef DEBUG_SYNTHESIS
    ut->foutput(stderr);
#endif
//    ut->foutput(stdout);
	BOOL setdur=TRUE;//TRUE;
    //convertir de pho a formato de label adecuado para HTS-engine
	pho2hts(ut, labels_string, setdur); //convertir pho a labels
//	fprintf(stderr,"%s\n", (const char *)labels_string);

	/* synthesis */
	/* load label */
	//HTS_Engine_load_label_from_fn(&engine, "prueba.dur");       /* load label file */
	HTS_Engine_load_label_from_string(&engine, (char *)labels_string.chars());
	if (phoneme_alignment)       /* modify label */
		HTS_Label_set_frame_specified_flag(&engine.label, TRUE);
	if (speech_speed != 1.0)     /* modify label */
		HTS_Label_set_speech_speed(&engine.label, speech_speed);
	HTS_Engine_create_sstream(&engine);  /* parse label and determine state duration */
	double f;
	int i;
	if (half_tone != 0.0) {      /* modify f0 */
		for (i = 0; i < HTS_SStreamSet_get_total_state(&engine.sss); i++) {
			f = HTS_SStreamSet_get_mean(&engine.sss, 1, i, 0);
			f += half_tone * log(2.0) / 12;
			if (f < log(10.0))
				f = log(10.0);
			HTS_SStreamSet_set_mean(&engine.sss, 1, i, 0, f);
		}
	}
	HTS_Engine_create_pstream(&engine);  /* generate speech parameter vector sequence */
	HTS_Engine_create_gstream(&engine);  /* synthesize speech */
		//HTS_GStreamSet_create
			//HTS_Vocoder_synthesize --> gss->gspeech[] --> aquí se almacenan las muestras y con HTS_Engine_save_generated_speech se guardan en wav
				//para ello usan HTS_GStreamSet_get_speech que va devolviendo muestra a muestra hasta HTS_GStreamSet_get_total_nsample(gss). Las guardan en short, habrá que convertir a double
					// double(muestra_short / 32767.0);

	//escribir en nuestro wav
	short temp;
	//reservar memoria
	setBufSize(HTS_GStreamSet_get_total_nsample(&(engine.gss)) , 0);
	for(i = 0; i < HTS_GStreamSet_get_total_nsample(&(engine.gss)); ++i){
		temp = HTS_GStreamSet_get_speech(&(engine.gss), i);
		wav_buf[ i ] = double ( temp / 32768.0 );
	}
	wav_nready=HTS_GStreamSet_get_total_nsample(&(engine.gss)) ;
    wav_nused=HTS_GStreamSet_get_total_nsample(&(engine.gss)) ;
    wav_nlen=0;
	  /* output */
	if (tracefp != NULL)
		HTS_Engine_save_information(&engine, tracefp);
	if (durfp != NULL)
		HTS_Engine_save_label(&engine, durfp);
	if (rawfp)
		HTS_Engine_save_generated_speech(&engine, rawfp);
	if (wavfp)
		HTS_Engine_save_riff(&engine, wavfp);
	if (excfp)
		HTS_Engine_save_generated_parameter(&engine, excfp, 2);
#ifndef HTS_EMBEDDED
	if (mcpfp)
		HTS_Engine_save_generated_parameter(&engine, mcpfp, 0);
	if (lf0fp)
		HTS_Engine_save_generated_parameter(&engine, lf0fp, 1);
#endif
	/* free */
	HTS_Engine_refresh(&engine); //borra los labels y los streams generados
    return TRUE;
}
/************************************************************************************************************************/

/************************************************************************************************************************/
short int * HTS_U2W::xinput_labels(String labels, int  *num_muestras){
	//fprintf(stderr,"HTS_U2W::xinput()\n");
	if (!HTS_ENGINE_INITIALIZED ){
		 /* initialize (stream[0] = spectrum , stream[1] = lf0) */
		int with_excitation = 0;
		FILE *tmp;
		tmp=fopen(fn_ts_exc[0], "rb");
		if ( tmp != NULL){
			fclose(tmp);
			with_excitation = 1;
			HTS_Engine_initialize(&engine, 3);
		}
		else{
			HTS_Engine_initialize(&engine, 2);
		}
		/* load duration model */
		HTS_Engine_load_duration_from_fn(&engine, fn_ms_dur, fn_ts_dur, num_interp);
		/* load stream[0] (spectrum model) */
		HTS_Engine_load_parameter_from_fn(&engine, fn_ms_mcp, fn_ts_mcp, fn_ws_mcp,
										 0, FALSE, num_ws_mcp, num_interp);
		/* load stream[1] (lf0 model) */
		HTS_Engine_load_parameter_from_fn(&engine, fn_ms_lf0, fn_ts_lf0, fn_ws_lf0,
										 1, TRUE, num_ws_lf0, num_interp);
		/* load gv[0] (GV for spectrum) */
		HTS_Engine_load_gv_from_fn(&engine, fn_ms_gvm, fn_ts_gvm, 0,
										num_interp);
		/* load gv[1] (GV for lf0) */
		HTS_Engine_load_gv_from_fn(&engine, fn_ms_gvl, fn_ts_gvl, 1,
										num_interp);
		if ( with_excitation == 1 ){
			/* load stream[2] (excitation model) */
			HTS_Engine_load_parameter_from_fn(&engine, fn_ms_exc, fn_ts_exc, fn_ws_exc, 2, FALSE, num_ws_exc, num_interp);
			HTS_Engine_load_gv_from_fn(&engine, fn_ms_gve, fn_ts_gve, 2, num_interp);
		}
		/* load GV switch */
		if (fn_gv_switch != NULL)
			HTS_Engine_load_gv_switch_from_fn(&engine, fn_gv_switch);
		/* set parameter */
		HTS_Engine_set_sampling_rate(&engine, sampling_rate);
		HTS_Engine_set_fperiod(&engine, fperiod);
		HTS_Engine_set_alpha(&engine, alpha);
		HTS_Engine_set_gamma(&engine, stage);
		HTS_Engine_set_log_gain(&engine, use_log_gain);
		HTS_Engine_set_beta(&engine, beta);
		HTS_Engine_set_audio_buff_size(&engine, audio_buff_size);
		HTS_Engine_set_msd_threshold(&engine, 1, uv_threshold);      /* set voiced/unvoiced threshold for stream[1] */
		HTS_Engine_set_gv_weight(&engine, 0, gv_weight_mcp);
		HTS_Engine_set_gv_weight(&engine, 1, gv_weight_lf0);
		if ( with_excitation == 1 )
			HTS_Engine_set_gv_weight(&engine, 2, gv_weight_exc);

		//int i;
		//for (i = 0; i < num_interp; i++) {
			HTS_Engine_set_duration_interpolation_weight(&engine, 0, 1.0);
			HTS_Engine_set_parameter_interpolation_weight(&engine, 0, 0,
														1.0);
			HTS_Engine_set_parameter_interpolation_weight(&engine, 1, 0,
														1.0);
			if ( with_excitation == 1 )
				HTS_Engine_set_parameter_interpolation_weight(&engine, 2, 0, 1.0);
		//}
		if (num_interp == num_ms_gvm)
			//for (i = 0; i < num_interp; i++)
				HTS_Engine_set_gv_interpolation_weight(&engine, 0, 0, 1.0);
		if (num_interp == num_ms_gvl)
			//for (i = 0; i < num_interp; i++)
				HTS_Engine_set_gv_interpolation_weight(&engine, 1, 0, 1.0);
		if (with_excitation == 1)
			HTS_Engine_set_gv_interpolation_weight(&engine, 2, 0, 1.0);

		HTS_ENGINE_INITIALIZED = TRUE;
	}

	BOOL setdur=TRUE;//TRUE;
    //convertir de pho a formato de label adecuado para HTS-engine
//	fprintf(stderr,"%s\n", (const char *)labels_string);

	/* synthesis */
	/* load label */
	//HTS_Engine_load_label_from_fn(&engine, "prueba.dur");       /* load label file */
	HTS_Engine_load_label_from_string(&engine, (char *)labels.chars());
	if (phoneme_alignment)       /* modify label */
		HTS_Label_set_frame_specified_flag(&engine.label, TRUE);
	if (speech_speed != 1.0)     /* modify label */
		HTS_Label_set_speech_speed(&engine.label, speech_speed);
	HTS_Engine_create_sstream(&engine);  /* parse label and determine state duration */
	double f;
	int i;
	if (half_tone != 0.0) {      /* modify f0 */
		for (i = 0; i < HTS_SStreamSet_get_total_state(&engine.sss); i++) {
			f = HTS_SStreamSet_get_mean(&engine.sss, 1, i, 0);
			f += half_tone * log(2.0) / 12;
			if (f < log(10.0))
				f = log(10.0);
			HTS_SStreamSet_set_mean(&engine.sss, 1, i, 0, f);
		}
	}
	HTS_Engine_create_pstream(&engine);  /* generate speech parameter vector sequence */
	HTS_Engine_create_gstream(&engine);  /* synthesize speech */


		//HTS_GStreamSet_create
			//HTS_Vocoder_synthesize --> gss->gspeech[] --> aquí se almacenan las muestras y con HTS_Engine_save_generated_speech se guardan en wav
				//para ello usan HTS_GStreamSet_get_speech que va devolviendo muestra a muestra hasta HTS_GStreamSet_get_total_nsample(gss). Las guardan en short, habrá que convertir a double
					// double(muestra_short / 32767.0);
	//escribir en nuestro wav
	short temp;
	//reservar memoria
	//wav_nready=0;
    //wav_nused=0;
    *num_muestras= HTS_GStreamSet_get_total_nsample(&(engine.gss));
    short *wav_buffer = (short *)malloc(sizeof(short) * (*num_muestras) );

	//setBufSize(HTS_GStreamSet_get_total_nsample(&(engine.gss)) , 0);
	for(i = 0; i < HTS_GStreamSet_get_total_nsample(&(engine.gss)); ++i){
		temp = HTS_GStreamSet_get_speech(&(engine.gss), i);
	//	wav_buf[ i ] = double ( temp / 32768.0 );
		wav_buffer[ i ] = temp ;
		//fprintf(stderr,"%f ", wav_buf[i]);
	}
	//fprintf(stderr,"\n");
	//wav_nready=HTS_GStreamSet_get_total_nsample(&(engine.gss)) ;
    //wav_nused=HTS_GStreamSet_get_total_nsample(&(engine.gss)) ;
    //wav_nlen=0;
	  /* output */
	if (tracefp != NULL)
		HTS_Engine_save_information(&engine, tracefp);
	if (durfp != NULL)
		HTS_Engine_save_label(&engine, durfp);
	if (rawfp)
		HTS_Engine_save_generated_speech(&engine, rawfp);
	//if (wavfp)
		//HTS_Engine_save_riff(&engine, wavfp);
	if (excfp)
		HTS_Engine_save_generated_parameter(&engine, excfp, 2);
#ifndef HTS_EMBEDDED
	if (mcpfp)
		HTS_Engine_save_generated_parameter(&engine, mcpfp, 0);
	if (lf0fp)
		HTS_Engine_save_generated_parameter(&engine, lf0fp, 1);
#endif
	/* free */
	HTS_Engine_refresh(&engine); //borra los labels y los streams generados
	//return wav_buf;
	return wav_buffer;
}

/**********************************************************/
// SINTETIZA UN FONEMA
BOOL HTS_U2W::doNext(BOOL flush) {
    assert(!wav_nready);// solo llegamos aqui cuando no quedan mas muestras listas.
    if (!ut) return FALSE;

  /*  if (semif<num_semif) {  //si quedan semifonemas en la frase
        if (semif==-1) {
            reset();
            return FALSE;
        }

        sintetizar(semif,num_semif);
        ++semif;
        return TRUE;
    }
    else {
        //	ut->foutput(stdout);
#ifdef ANHITZ
        ut->foutput(stdout);
#endif
        for (i=0;i<n_semifonemas;++i)
            free(*(array_costes+i)); //KORPUS (podrian irse eliminado uno a uno??
        free(array_costes);
        array_costes=NULL;
        for (i=0;i<num_semif;++i) {
            if (sphs[i].num_olas > 0)
                free(sphs[i].mrks);
        }
        free(sphs);
        free(camino);
        semif=-1;*/
        reset();


        return FALSE; //*/
   // }


}

VOID HTS_U2W::shiftedWav( INT n ) {
    if (!wav_buf) return;
    assert(wav_nused+n<=wav_len);
    if (!n) memset(wav_buf-wav_nlen,0,(wav_len+wav_nlen)*sizeof(DOUBLE));
    else
        memset(wav_buf+wav_nused,0,n*sizeof(DOUBLE));

}

///////////////////////////////////////////////////////////////////
//  			FUNCIONES para convertir de pho a label
////////////////////////////////////////////////////////////////////
///////////////
//PHONEME LEVEL
///////////////
void HTS_U2W::phone2sampa(char *phoneme, UttPh *u, Lix p){
	if(p == NULL)
		strcpy(phoneme,"0");
	else{
		char phone=u->cell(p).getPhone();
		if( phone == 'X') //s`
			strcpy(phoneme,"z");
		else if( phone == 'P') //ts`
			strcpy(phoneme, "tz");
		//English mapeo interno a formato derro
		else if( phone == 'A') //A:
			strcpy(phoneme, "AP");
		else if( phone == '0') //O:
			strcpy(phoneme, "OP");
		else if( phone == 'E') //e@
			strcpy(phoneme, "eX");
		else if( phone == '6') //n!
			strcpy(phoneme, "nE");
		else if( phone == '3') //3:
			strcpy(phoneme, "3P");
		else if( phone == ':') //i:
			strcpy(phoneme, "iP");
		else if( phone == 'Y') //I@
			strcpy(phoneme, "IX");
		else if( phone == '9') //l!
			strcpy(phoneme, "lE");
		else if( phone == '}') //m!
			strcpy(phoneme, "mE");
		else if( phone == '5') //U@
			strcpy(phoneme, "UX");
		else if( phone == 'M') //@U
			strcpy(phoneme, "XU");
		else if( phone == '1') //u:
			strcpy(phoneme, "uP");
		else if( phone == '@') //@
			strcpy(phoneme, "X");
		else if( phone == '{') //{
			strcpy(phoneme, "K");
		else
			strcpy(phoneme, phone_tosampa(u->cell(p).getPhone()));


	}
}
/////
void HTS_U2W::pos_syl(unsigned char &pos_left, unsigned char &pos_right, UttPh *u, Lix p, char *phoneme_prev, char *phoneme_next){
	pos_left=0, pos_right=0;
	if(p==NULL)
		return;
	if(u->cell(p).getPhone() == '_'){
		if(u->phonePrev(p) != NULL)
			pos_left = u->phonePos(u->phonePrev(p), URANGE_SYLLABLE) + 1;
		if(u->phoneNext(p) != NULL){
			pos_right = u->phonePos(u->phoneLast(u->phoneNext(p), URANGE_SYLLABLE), URANGE_SYLLABLE) - u->phonePos(u->phoneNext(p), URANGE_SYLLABLE) + 1;
			if(u->syllableThis(u->phoneNext(p)) == u->syllableLast(u->phoneNext(p), URANGE_PAUSE))
				pos_right = pos_right -1;
		}
	}
	else{
		pos_left = u->phonePos(p, URANGE_SYLLABLE) + 1;
		pos_right = u->phonePos(u->phoneLast(p, URANGE_SYLLABLE), URANGE_SYLLABLE) - u->phonePos(p, URANGE_SYLLABLE) + 1;
		if(u->syllableThis(p) == u->syllableLast(p, URANGE_PAUSE))
			pos_right = pos_right - 1;
		if(!strcmp(phoneme_prev, "0"))
			pos_left=pos_left-1;
		if(!strcmp(phoneme_next, "0"))
			pos_right=pos_right-1;
	}
}
///////////////
//SYLLABLE LEVEL
///////////////
void HTS_U2W::pos_pau(unsigned short &pos_left, unsigned short &pos_right, UttPh *u, Lix p){
	pos_left=0, pos_right=0;
	if(p==NULL)
		return;
	if(u->cell(p).getPhone() == '_'){
		if(u->phonePrev(p) != NULL)
			pos_left = u->phonePos(u->phonePrev(p), URANGE_PAUSE);
		if(u->phoneNext(p) != NULL)
			pos_right = u->phonePos(u->phoneLast(u->phoneNext(p) , URANGE_PAUSE), URANGE_PAUSE) - u->phonePos(u->phoneNext(p) , URANGE_PAUSE) + 1;
	}
	else{
		pos_left = u->phonePos(p, URANGE_PAUSE);
		pos_right = u->phonePos(u->phoneLast(p, URANGE_PAUSE), URANGE_PAUSE) - u->phonePos(p, URANGE_PAUSE) + 1;
	}
}
/////
void HTS_U2W::syllable_stress(char &syl_left_stress, char &syl_stress, char &syl_right_stress, UttPh *u, Lix p){
	syl_left_stress=0, syl_stress=0, syl_right_stress=0;
	if(u->cell(p).getPhone() == '_'){
		Lix q;
		for(q=u->syllableThis(u->phonePrev(p)); q!=NULL && u->cell(q).getPhone() != '_'; q=u->phoneNext(q,URANGE_SYLLABLE)){
			if( u->cell(q).getStress()==USTRESS_TEXT || u->cell(q).getStress()==USTRESS_AUTO ){
				syl_left_stress=1;
				break;
			}
		}
		for(q=u->syllableThis(u->phoneNext(p)); q!=NULL && u->cell(q).getPhone() != '_'; q=u->phoneNext(q,URANGE_SYLLABLE)){
			if( u->cell(q).getStress()==USTRESS_TEXT || u->cell(q).getStress()==USTRESS_AUTO ){
				syl_right_stress=1;
				break;
			}
		}
	}
	else{
		p=u->syllableThis(p);
		Lix q;
		if(p==NULL)
			return;
		for(q=p; q!=NULL && u->cell(q).getPhone() != '_'; q=u->phoneNext(q,URANGE_SYLLABLE)){
			if( u->cell(q).getStress()==USTRESS_TEXT || u->cell(q).getStress()==USTRESS_AUTO ){
				syl_stress=1;
				break;
			}
		}
		for(q=u->syllablePrev(p,URANGE_PAUSE); q!=NULL && u->cell(q).getPhone() != '_'; q=u->phoneNext(q,URANGE_SYLLABLE)){
			if( u->cell(q).getStress()==USTRESS_TEXT || u->cell(q).getStress()==USTRESS_AUTO ){
				syl_left_stress=1;
				break;
			}
		}
		for(q=u->syllableNext(p,URANGE_PAUSE); q!=NULL && u->cell(q).getPhone() != '_'; q=u->phoneNext(q,URANGE_SYLLABLE)){
			if( u->cell(q).getStress()==USTRESS_TEXT || u->cell(q).getStress()==USTRESS_AUTO ){
				syl_right_stress=1;
				break;
			}
		}
	}
}
/////
void HTS_U2W::syllable_emphasis(char &syl_left_emphasis, char &syl_emphasis, char &syl_right_emphasis, UttPh *u, Lix p){
	syl_left_emphasis=0, syl_emphasis=0, syl_right_emphasis=0;
	if(u->cell(p).getPhone() == '_'){
		Lix q;
		for(q=u->syllableThis(u->phonePrev(p)); q!=NULL && u->cell(q).getPhone() != '_'; q=u->phoneNext(q,URANGE_SYLLABLE)){
			if( u->cell(q).getEmphasis()==UEMPHASIS_STRESS || u->cell(q).getEmphasis()==UEMPHASIS_STRONG ){
				syl_left_emphasis=1;
				break;
			}
		}
		for(q=u->syllableThis(u->phoneNext(p)); q!=NULL && u->cell(q).getPhone() != '_'; q=u->phoneNext(q,URANGE_SYLLABLE)){
			if( u->cell(q).getEmphasis()==UEMPHASIS_STRESS || u->cell(q).getEmphasis()==UEMPHASIS_STRONG ){
				syl_right_emphasis=1;
				break;
			}
		}
	}
	else{
		p=u->syllableThis(p);
		Lix q;
		if(p==NULL)
			return;
		for(q=p; q!=NULL && u->cell(q).getPhone() != '_'; q=u->phoneNext(q,URANGE_SYLLABLE)){
			if( u->cell(q).getEmphasis()==UEMPHASIS_STRESS || u->cell(q).getEmphasis()==UEMPHASIS_STRONG ){
				syl_emphasis=1;
				break;
			}
		}
		for(q=u->syllablePrev(p,URANGE_PAUSE); q!=NULL && u->cell(q).getPhone() != '_'; q=u->phoneNext(q,URANGE_SYLLABLE)){
			if( u->cell(q).getEmphasis()==UEMPHASIS_STRESS || u->cell(q).getEmphasis()==UEMPHASIS_STRONG ){
				syl_left_emphasis=1;
				break;
			}
		}
		for(q=u->syllableNext(p,URANGE_PAUSE); q!=NULL && u->cell(q).getPhone() != '_'; q=u->phoneNext(q,URANGE_SYLLABLE)){
			if( u->cell(q).getEmphasis()==UEMPHASIS_STRESS || u->cell(q).getEmphasis()==UEMPHASIS_STRONG ){
				syl_right_emphasis=1;
				break;
			}
		}
	}
}
/////
void HTS_U2W::syllable_num_phone(unsigned char &syl_left_num_phone, unsigned char &syl_num_phone, unsigned char &syl_right_num_phone, UttPh *u, Lix p){
	syl_left_num_phone=0, syl_num_phone=0, syl_right_num_phone=0;
	if(u->cell(p).getPhone() == '_'){
		Lix q;
		for(q=u->syllableThis(u->phonePrev(p)); q!=NULL && u->cell(q).getPhone() != '_'; q=u->phoneNext(q,URANGE_SYLLABLE))
			syl_left_num_phone += 1;
		for(q=u->syllableThis(u->phoneNext(p)); q!=NULL && u->cell(q).getPhone() != '_'; q=u->phoneNext(q,URANGE_SYLLABLE))
			syl_right_num_phone += 1;
	}
	else{
		p=u->syllableThis(p);
		if(p==NULL)
			return;
		Lix q;
		for(q=p; q!=NULL && u->cell(q).getPhone() != '_'; q=u->phoneNext(q,URANGE_SYLLABLE))
			syl_num_phone += 1;
		for(q=u->syllablePrev(p,URANGE_PAUSE); q!=NULL && u->cell(q).getPhone() != '_'; q=u->phoneNext(q,URANGE_SYLLABLE))
			syl_left_num_phone += 1;
		for(q=u->syllableNext(p,URANGE_PAUSE); q!=NULL && u->cell(q).getPhone() != '_'; q=u->phoneNext(q,URANGE_SYLLABLE))
			syl_right_num_phone += 1;
	}
}
/////
void HTS_U2W::syllable_pos_word(unsigned char &syl_pos_word_left, unsigned char &syl_pos_word_right, UttPh *u, Lix p){
	syl_pos_word_left=0, syl_pos_word_right=0;
	if(u->cell(p).getPhone() == '_'){
		if(u->phonePrev(p) != NULL)
			syl_pos_word_left = u->syllablePos(u->phonePrev(p), URANGE_WORD) + 1;
		if(u->phoneNext(p) != NULL)
			syl_pos_word_right = u->syllablePos(u->syllableLast(u->phoneNext(p), URANGE_WORD), URANGE_WORD) - u->syllablePos(u->phoneNext(p), URANGE_WORD) + 1;
	}
	else{
		syl_pos_word_left = u->syllablePos(p, URANGE_WORD) + 1;
		syl_pos_word_right = u->syllablePos(u->syllableLast(p, URANGE_WORD), URANGE_WORD) - u->syllablePos(p, URANGE_WORD) + 1;
	}
}
//////
void HTS_U2W::syllable_pos_ag(unsigned char &syl_pos_ag_left, unsigned char &syl_pos_ag_right, UttPh *u, Lix p){
	syl_pos_ag_left=0, syl_pos_ag_right=0;
	if(u->cell(p).getPhone() == '_'){
		if(u->phonePrev(p) != NULL)
			syl_pos_ag_left = u->syllablePos(u->phonePrev(p), URANGE_AGRP) + 1;
		if(u->phoneNext(p) != NULL)
			syl_pos_ag_right = u->syllablePos(u->syllableLast(u->phoneNext(p), URANGE_AGRP), URANGE_AGRP) - u->syllablePos(u->phoneNext(p), URANGE_AGRP) + 1;
	}
	else{
		syl_pos_ag_left = u->syllablePos(p, URANGE_AGRP) + 1;
		syl_pos_ag_right = u->syllablePos(u->syllableLast(p, URANGE_AGRP), URANGE_AGRP) - u->syllablePos(p, URANGE_AGRP) + 1;
	}
}
//////
void HTS_U2W::syllable_pos_sentence(unsigned short &syl_pos_sentence_left, unsigned short &syl_pos_sentence_right, UttPh *u, Lix p){
	syl_pos_sentence_left=0, syl_pos_sentence_right=0;
	if(u->cell(p).getPhone() == '_'){
		if(u->phonePrev(p) != NULL)
			syl_pos_sentence_left = u->syllablePos(u->phonePrev(p), URANGE_UTT) + 1;
		if(u->phoneNext(p) != NULL)
			syl_pos_sentence_right = u->syllablePos(u->syllableLast(u->phoneNext(p), URANGE_UTT), URANGE_UTT) - u->syllablePos(u->phoneNext(p), URANGE_UTT) + 1;
	}
	else{
		syl_pos_sentence_left = u->syllablePos(p, URANGE_UTT) + 1;
		syl_pos_sentence_right = u->syllablePos(u->syllableLast(p, URANGE_UTT), URANGE_UTT) - u->syllablePos(p, URANGE_UTT) + 1;
	}
}
//////
void HTS_U2W::syllable_pos_pause(unsigned short &syl_pos_pause_left, unsigned short &syl_pos_pause_right, UttPh *u, Lix p){
	syl_pos_pause_left=0, syl_pos_pause_right=0;
	if(u->cell(p).getPhone() == '_'){
		if(u->phonePrev(p) != NULL)
			syl_pos_pause_left = u->syllablePos(u->phonePrev(p), URANGE_PAUSE) + 1;
		if(u->phoneNext(p) != NULL)
			syl_pos_pause_right = u->syllablePos(u->syllableLast(u->phoneNext(p), URANGE_PAUSE), URANGE_PAUSE) - u->syllablePos(u->phoneNext(p), URANGE_PAUSE) + 1;
	}
	else{
		syl_pos_pause_left = u->syllablePos(p, URANGE_PAUSE) + 1;
		syl_pos_pause_right = u->syllablePos(u->syllableLast(p, URANGE_PAUSE), URANGE_PAUSE) - u->syllablePos(p, URANGE_PAUSE) + 1;
	}
}
///////////////
//WORD LEVEL
///////////////
char HTS_U2W::get_POS(UttPh *u, Lix p){
	//char part_of_speech=0;
	//POS = 1 --> Content
	//POS = 2 --> Function
/*	if(u->cell(u->wordThis(p)).queryPOS(POS_EU_NONE)){
		part_of_speech=1;
	}
	else if(u->cell(u->wordThis(p)).queryPOS(POS_EU_IZE)){
		part_of_speech=1;
	}
	else if(u->cell(u->wordThis(p)).queryPOS(POS_EU_ADJ)){
		part_of_speech=1;
	}
	else if(u->cell(u->wordThis(p)).queryPOS(POS_EU_ADB)){
		part_of_speech=1;
	}
	else if(u->cell(u->wordThis(p)).queryPOS(POS_EU_ATZ_IZE)){
		part_of_speech=1;
	}
	else{
		part_of_speech=2;
	}*/
	if(!u->cell(u->wordThis(p)).getPOS())
		return 1;
	if(!strcmp(Language, "es")){
		if(u->cell(u->wordThis(p)).getPOS() & 466944)
			return 1;
	}
	/*if(!strcmp(Language, "en")){
		//POS simplificado para labels en formato derro
		//NOUN (CONTENT WORD)
		if(u->cell(u->wordThis(p)).getPOS() == POS_EN_NONE )
			return 1;
		if(u->cell(u->wordThis(p)).getPOS() == POS_EN_NOUN_SING )
			return 1;
		if(u->cell(u->wordThis(p)).getPOS() == POS_EN_NOUN_PL )
			return 1;
		if(u->cell(u->wordThis(p)).getPOS() == POS_EN_FOREING_WORD )
			return 1;
		if(u->cell(u->wordThis(p)).getPOS() == POS_EN_PROPER_NOUN_SING )
			return 1;
		if(u->cell(u->wordThis(p)).getPOS() == POS_EN_PROPER_NOUN_PL )
			return 1;
		//VERBS (CONTENT WORD)
		if(u->cell(u->wordThis(p)).getPOS() == POS_EN_VERB)
			return 2;
		if(u->cell(u->wordThis(p)).getPOS() == POS_EN_VERB_PAST)
			return 2;
		if(u->cell(u->wordThis(p)).getPOS() == POS_EN_GERUN)
			return 2;
		if(u->cell(u->wordThis(p)).getPOS() == POS_EN_PAST_PARTICIP)
			return 2;
		if(u->cell(u->wordThis(p)).getPOS() == POS_EN_VERB_NO_3PERS)
			return 2;
		if(u->cell(u->wordThis(p)).getPOS() == POS_EN_VERB_3PERS)
			return 2;
		if(u->cell(u->wordThis(p)).getPOS() == POS_EN_MODAL)
			return 2;
		//ADJ (CONTENT WORD)
		if(u->cell(u->wordThis(p)).getPOS() == POS_EN_ADJ)
			return 3;
		if(u->cell(u->wordThis(p)).getPOS() == POS_EN_ADJ_COMP)
			return 3;
		if(u->cell(u->wordThis(p)).getPOS() == POS_EN_ADJ_SUPERL)
			return 3;
		//ADV (CONTENT WORD)
		if(u->cell(u->wordThis(p)).getPOS() == POS_EN_ADV)
			return 4;
		if(u->cell(u->wordThis(p)).getPOS() == POS_EN_ADV_COMP)
			return 4;
		if(u->cell(u->wordThis(p)).getPOS() == POS_EN_ADV_SUPER)
			return 4;
		//PRONOUN AND SIMILAR (FUNCTION WORD)
		if(u->cell(u->wordThis(p)).getPOS() == POS_EN_CARD_NUM)
			return 5;
		if(u->cell(u->wordThis(p)).getPOS() == POS_EN_LIST)
			return 5;
		if(u->cell(u->wordThis(p)).getPOS() == POS_EN_PERS_PRONOUN)
			return 5;
		if(u->cell(u->wordThis(p)).getPOS() == POS_EN_POSS_PRONOUN)
			return 5;
		//WH.... (FUNCTION WORD)
		if(u->cell(u->wordThis(p)).getPOS() == POS_EN_WH_DET)
			return 6;
		if(u->cell(u->wordThis(p)).getPOS() == POS_EN_WH_PRONOUN)
			return 6;
		if(u->cell(u->wordThis(p)).getPOS() == POS_EN_WH_PRONOUN_POSS)
			return 6;
		if(u->cell(u->wordThis(p)).getPOS() == POS_EN_WH_ADV)
			return 6;
		//INTERJECTION (FUNCTION WORD)
		if(u->cell(u->wordThis(p)).getPOS() == POS_EN_INTERJ)
			return 7;
		//OTHER (FUNCTION WORD)
		if(u->cell(u->wordThis(p)).getPOS() == POS_EN_COORD_CONJ)
			return 8;
			if(u->cell(u->wordThis(p)).getPOS() == POS_EN_DET)
			return 8;
			if(u->cell(u->wordThis(p)).getPOS() == POS_EN_EX_THERE)
			return 8;
			if(u->cell(u->wordThis(p)).getPOS() == POS_EN_SUBORD_CONJ)
			return 8;
			if(u->cell(u->wordThis(p)).getPOS() == POS_EN_SYMBOL)
			return 8;
			if(u->cell(u->wordThis(p)).getPOS() == POS_EN_PRE_DET)
			return 8;
			if(u->cell(u->wordThis(p)).getPOS() == POS_EN_TO)
			return 8;
		//
		else
			return 8;
	}*/
	else{
		if(u->cell(u->wordThis(p)).getPOS() & 95)
			return 1;
	}
	return 2;
//	return part_of_speech;
}
//////
void HTS_U2W::word_POS(char &word_POS_left, char &word_POS, char &word_POS_right, UttPh *u, Lix p){
	word_POS_left=0, word_POS=0, word_POS_right=0;
	if(u->cell(p).getPhone() == '_'){
		if(u->wordThis(u->phoneNext(p),URANGE_PAUSE))
			word_POS_right = get_POS(u, u->wordThis(u->phoneNext(p), URANGE_PAUSE));
		if(u->wordThis(u->phonePrev(p), URANGE_PAUSE))
			word_POS_left = get_POS(u, u->wordThis(u->phonePrev(p), URANGE_PAUSE));
	}
	else{
		p=u->wordThis(p);
		if(p==NULL)
			return;
		word_POS = get_POS(u, p);
		if(u->wordNext(p,URANGE_PAUSE))
			word_POS_right = get_POS(u, u->wordNext(p, URANGE_PAUSE));
		if(u->wordPrev(p, URANGE_PAUSE))
			word_POS_left = get_POS(u, u->wordPrev(p, URANGE_PAUSE));
	}
}
/////
void HTS_U2W::word_num_syl(char &word_num_syl_left, char &word_num_syl, char &word_num_syl_right, UttPh *u, Lix p){
	word_num_syl_left=0, word_num_syl=0, word_num_syl_right=0;
	if(u->cell(p).getPhone() == '_'){
		Lix q=p;
		p=u->wordThis(u->phonePrev(p), URANGE_PAUSE);
        if(u->wordPrev(u->wordThis(p)))
        	word_num_syl_left=u->syllableN(p,URANGE_WORD)-u->syllableN(u->wordPrev(u->wordThis(p)),URANGE_WORD);
        else
            word_num_syl_left=u->syllableN(p,URANGE_WORD);
        p=u->wordNext(q, URANGE_PAUSE);
        if(p!=NULL){
        	if(u->wordPrev(u->wordThis(p)))
        		word_num_syl_right=u->syllableN(p,URANGE_WORD)-u->syllableN(u->wordPrev(u->wordThis(p)),URANGE_WORD);
        	else
            	word_num_syl_right=u->syllableN(p,URANGE_WORD);
        }
	}
	else{
		if(u->wordPrev(u->wordThis(p)))
        	word_num_syl=u->syllableN(p,URANGE_WORD)-u->syllableN(u->wordPrev(u->wordThis(p)),URANGE_WORD);
        else
            word_num_syl=u->syllableN(p,URANGE_WORD);

        Lix q=p;
        p=u->wordPrev(p, URANGE_PAUSE);
        if(p!=NULL){
        	if(u->wordPrev(u->wordThis(p)))
        		word_num_syl_left=u->syllableN(p,URANGE_WORD)-u->syllableN(u->wordPrev(u->wordThis(p)),URANGE_WORD);
        	else
            	word_num_syl_left=u->syllableN(p,URANGE_WORD);
        }
        p=u->wordNext(q, URANGE_PAUSE);
        if(p!=NULL){
        	if(u->wordPrev(u->wordThis(p)))
        		word_num_syl_right=u->syllableN(p,URANGE_WORD)-u->syllableN(u->wordPrev(u->wordThis(p)),URANGE_WORD);
        	else
            	word_num_syl_right=u->syllableN(p,URANGE_WORD);
        }
	}
}
/////
void HTS_U2W::word_pos_sentence(unsigned char &word_pos_sentence_left, unsigned char &word_pos_sentence_right, UttPh *u, Lix p){
	word_pos_sentence_left=0, word_pos_sentence_right=0;
	if(u->cell(p).getPhone() == '_'){
		if(u->phonePrev(p) != NULL)
			word_pos_sentence_left = u->wordPos(u->phonePrev(p), URANGE_UTT) + 1;
		if(u->phoneNext(p) != NULL)
			word_pos_sentence_right = u->wordPos(u->wordLast(u->phoneNext(p), URANGE_UTT), URANGE_UTT) - u->wordPos(u->phoneNext(p), URANGE_UTT) + 1;
	}
	else{
		word_pos_sentence_left = u->wordPos(p, URANGE_UTT) + 1;
		word_pos_sentence_right = u->wordPos(u->wordLast(p, URANGE_UTT), URANGE_UTT) - u->wordPos(p, URANGE_UTT) + 1;
	}
}
/////
void HTS_U2W::word_pos_pause(unsigned char &word_pos_pause_left, unsigned char &word_pos_pause_right, UttPh *u, Lix p){
	word_pos_pause_left=0, word_pos_pause_right=0;
	if(u->cell(p).getPhone() == '_'){
		if(u->phonePrev(p) != NULL)
			word_pos_pause_left = u->wordPos(u->phonePrev(p), URANGE_PAUSE) + 1;
		if(u->phoneNext(p) != NULL)
			word_pos_pause_right = u->wordPos(u->wordLast(u->phoneNext(p), URANGE_PAUSE), URANGE_PAUSE) - u->wordPos(u->phoneNext(p), URANGE_PAUSE) + 1;
	}
	else{
		word_pos_pause_left = u->wordPos(p, URANGE_PAUSE) + 1;
		word_pos_pause_right = u->wordPos(u->wordLast(p, URANGE_PAUSE), URANGE_PAUSE) - u->wordPos(p, URANGE_PAUSE) + 1;
	}
}
///////////////
//ACCENT GROUP LEVEL
///////////////
char HTS_U2W::get_ga(UttPh *u, Lix p){
	char type=0;
	p=u->agrpThis(p);
	if(p==NULL)
		return type;
	return u->cell(p).getAGrp();
	/*switch (u->cell(p).getAGrp()) {
    case AGRP_EU_NONE:
    	type=0;
        break;
    case AGRP_EU_MRK:
    	type=1;
		break; //en la primera
	case AGRP_EU_OROK:
		type=2;
		break; //acento en la segunda silaba
	case AGRP_EU_GABE:
		type=3;
		break; //sin acento
	case AGRP_EU_TXT:
		type=4;
		break;
	case '5':
		type=5;
		break;
	}
	return type;*/
}
/////
void HTS_U2W::ag_type(char &ga_type_left, char &ga_type, char &ga_type_right, UttPh *u, Lix p){
	ga_type_left=0, ga_type=0, ga_type_right=0;
	if(u->cell(p).getPhone() == '_'){
		ga_type_left=get_ga(u,u->phonePrev(p));
		ga_type_right=get_ga(u, u->phoneNext(p));
	}
	else{
		p=u->agrpThis(p);
		if(p==NULL)
			return;
		ga_type=get_ga(u,p);
		ga_type_left=get_ga(u,u->agrpPrev(p, URANGE_PAUSE));
		ga_type_right=get_ga(u, u->agrpNext(p, URANGE_PAUSE));
	}
}
/////
void HTS_U2W::ag_num_syl(unsigned char &ag_num_syl_left, unsigned char &ag_num_syl, unsigned char &ag_num_syl_right, UttPh *u, Lix p){
	ag_num_syl_left=0, ag_num_syl=0, ag_num_syl_right=0;
	if(u->cell(p).getPhone() == '_'){
		Lix q=p;
		p=u->agrpThis(u->phonePrev(p), URANGE_PAUSE);
        if(u->agrpPrev(u->agrpThis(p)))
        	ag_num_syl_left=u->syllableN(p,URANGE_AGRP)-u->syllableN(u->agrpPrev(u->agrpThis(p)),URANGE_AGRP);
        else
            ag_num_syl_left=u->syllableN(p,URANGE_AGRP);
        p=u->agrpNext(q, URANGE_PAUSE);
        if(p!=NULL){
        	if(u->agrpPrev(u->agrpThis(p)))
        		ag_num_syl_right=u->syllableN(p,URANGE_AGRP)-u->syllableN(u->agrpPrev(u->agrpThis(p)),URANGE_AGRP);
        	else
            	ag_num_syl_right=u->syllableN(p,URANGE_AGRP);
        }
	}
	else{
		if(u->agrpPrev(u->agrpThis(p)))
        	ag_num_syl=u->syllableN(p,URANGE_AGRP)-u->syllableN(u->agrpPrev(u->agrpThis(p)),URANGE_AGRP);
        else
            ag_num_syl=u->syllableN(p,URANGE_AGRP);

        Lix q=p;
        p=u->agrpPrev(p, URANGE_PAUSE);
        if(p!=NULL){
        	if(u->agrpPrev(u->agrpThis(p)))
        		ag_num_syl_left=u->syllableN(p,URANGE_AGRP)-u->syllableN(u->agrpPrev(u->agrpThis(p)),URANGE_AGRP);
        	else
            	ag_num_syl_left=u->syllableN(p,URANGE_AGRP);
        }
        p=u->agrpNext(q, URANGE_PAUSE);
        if(p!=NULL){
        	if(u->agrpPrev(u->agrpThis(p)))
        		ag_num_syl_right=u->syllableN(p,URANGE_AGRP)-u->syllableN(u->agrpPrev(u->agrpThis(p)),URANGE_AGRP);
        	else
            	ag_num_syl_right=u->syllableN(p,URANGE_AGRP);
        }
	}
}
/////
void HTS_U2W::ag_pos_sentence(unsigned char &ag_pos_sentence_left, unsigned char &ag_pos_sentence_right, UttPh *u, Lix p){
	ag_pos_sentence_left=0, ag_pos_sentence_right=0;
	if(u->cell(p).getPhone() == '_'){
		if(u->phonePrev(p) != NULL)
			ag_pos_sentence_left = u->agrpPos(u->phonePrev(p), URANGE_UTT) + 1;
		if(u->phoneNext(p) != NULL)
			ag_pos_sentence_right = u->agrpPos(u->agrpLast(u->phoneNext(p), URANGE_UTT), URANGE_UTT) - u->agrpPos(u->phoneNext(p), URANGE_UTT) + 1;
	}
	else{
		ag_pos_sentence_left = u->agrpPos(p, URANGE_UTT) + 1;
		ag_pos_sentence_right = u->agrpPos(u->agrpLast(p, URANGE_UTT), URANGE_UTT) - u->agrpPos(p, URANGE_UTT) + 1;
	}
}
/////
void HTS_U2W::ag_pos_pause(unsigned char &ag_pos_pause_left, unsigned char &ag_pos_pause_right, UttPh *u, Lix p){
	ag_pos_pause_left=0, ag_pos_pause_right=0;
	if(u->cell(p).getPhone() == '_'){
		if(u->phonePrev(p) != NULL)
			ag_pos_pause_left = u->agrpPos(u->phonePrev(p), URANGE_PAUSE) + 1;
		if(u->phoneNext(p) != NULL)
			ag_pos_pause_right = u->agrpPos(u->agrpLast(u->phoneNext(p), URANGE_PAUSE), URANGE_PAUSE) - u->agrpPos(u->phoneNext(p), URANGE_PAUSE) + 1;
	}
	else{
		ag_pos_pause_left = u->agrpPos(p, URANGE_PAUSE) + 1;
		ag_pos_pause_right = u->agrpPos(u->agrpLast(p, URANGE_PAUSE), URANGE_PAUSE) - u->agrpPos(p, URANGE_PAUSE) + 1;
	}
}
///////////////
//PAUSE LEVEL
///////////////
char HTS_U2W::get_pau(UttPh *u, Lix p){
	char type=0;
	//p=u->pauThis(p);
	if(p==NULL)
		return type;
	switch (u->cell(p).getPause()) {
    case UPAUSE_NONE:
    	type=0;
        break;
    case UPAUSE_UBEGIN:
    	type=1;
		break;
	case UPAUSE_UEND:
		type=2;
		break;
	case UPAUSE_SHORT:
		type=3;
		break;
	case UPAUSE_LONG:
		type=4;
		break;
	case UPAUSE_SDECL:
    	type=5;
		break;
	case UPAUSE_SQUEST:
		type=6;
		break;
	case UPAUSE_SEXCL:
		type=7;
		break;
	case UPAUSE_SPAUSE:
		type=8;
		break;
	case UPAUSE_SNEUTR:
    	type=9;
		break;
	case UPAUSE_SUSPEN:
		type=10;
		break;
	case UPAUSE_COLON:
		type=11;
		break;
	case UPAUSE_SCOLON:
		type=12;
		break;
	case UPAUSE_PAR:
		type=14;
		break;
	case UPAUSE_PAROPEN:
		type=13; //correg?
		break;
	default :
		type=0;
		break;
	}
	return type;
}
/////
void HTS_U2W::pau_type(char &pau_type_left, char &pau_type_right, UttPh *u, Lix p){
	pau_type_left=0, pau_type_right=0;
	if(u->cell(p).getPhone() == '_'){
		pau_type_left=get_pau(u, p);
		pau_type_right=pau_type_left;
	}
	else{
		if(p==NULL)
			return;
		Lix q;
		for(q=p; q!=NULL; q=u->phonePrev(q)){
			if(u->cell(q).getPhone() == '_'){
					pau_type_left=get_pau(u, q);
					break;
			}
		}
		for(q=p; q!=NULL; q=u->phoneNext(q)){
			if(u->cell(q).getPhone() == '_'){
					pau_type_right=get_pau(u, q);
					break;
			}
		}
	}
}
/////
void HTS_U2W::pau_pos_sentence(unsigned char &pau_pos_sentence_left, unsigned char &pau_pos_sentence_right, UttPh *u, Lix p){
	pau_pos_sentence_left=0, pau_pos_sentence_right=0;
	Lix q;
	for(q=u->phonePrev(p, URANGE_UTT); q!=NULL; q=u->phonePrev(q, URANGE_UTT)){
		if(u->cell(q).getPhone() == '_')
			pau_pos_sentence_left += 1;
	}
	for(q=u->phoneNext(p, URANGE_UTT); q!=NULL; q=u->phoneNext(q, URANGE_UTT)){
		if(u->cell(q).getPhone() == '_')
			pau_pos_sentence_right += 1;
	}
}
///////////////
//SENTENCE LEVEL
///////////////
char HTS_U2W::get_sentence(UttPh *u, Lix p){
	char type=0;
	switch(u->cell(u->sentenceLast(u->sentenceThis(p),URANGE_UTT)).getSentence()){
		case USENTENCE_NONE :
			type=0;
			break; // tipo nada
		case USENTENCE_DECL :
			type=1;
			break; // tipo .
		case USENTENCE_QUEST :
			type=2;
			break; // tipo ?
		case USENTENCE_EXCL :
			type=3;
			break; // tipo !
		case USENTENCE_PAUSE :
			type=4;
			break; // tipo ,
		case USENTENCE_NEUTR :
			type=5;
			break; // tipo _
		case USENTENCE_SUSPEN :
			type=6;
			break; // tipo ...
		case USENTENCE_COLON :
			type=7;
			break; // tipo ;
		case USENTENCE_SCOLON :
			type=8;
			break; // tipo :
		case USENTENCE_PAR :
			type=9;
			break; // tipo (
		case USENTENCE_PAROPEN :
			type=9;
			break;
		default:
			type=0;
			break;
	}
	return type;
}
//////
char HTS_U2W::sentence_type(UttPh *u, Lix p){
	char type=0;
	if(p==NULL)
		return type;
	if(u->cell(p).getPhone() == '_'){
		if(u->phoneNext(p, URANGE_UTT))
			type = get_sentence(u, u->phoneNext(p, URANGE_UTT));
		else if(u->phonePrev(p, URANGE_UTT))
			type = get_sentence(u, u->phonePrev(p, URANGE_UTT));

		return type;
	}
	type = get_sentence(u,p);
	return type;
}
///////////////////////
void HTS_U2W::pho2hts(UttPh *u, String &labels_string, BOOL setdur){
	labels_string="";
	struct hts_label{
		//phoneme
		char phoneme [5];
		char phoneme_prev[5];
		char phoneme_prev_prev[5];
		char phoneme_next[5];
		char phoneme_next_next[5];
		unsigned char phoneme_pos_syl_left, phoneme_pos_syl_right;
		unsigned short phoneme_pos_pau_left, phoneme_pos_pau_right;
		//syllable
		char syl_left_stress, syl_stress, syl_right_stress;
		char syl_left_emphasis, syl_emphasis, syl_right_emphasis;
		unsigned char syl_left_num_phone, syl_num_phone, syl_right_num_phone;
		unsigned char syl_pos_word_left, syl_pos_word_right;
		unsigned char syl_pos_ag_left, syl_pos_ag_right;
		unsigned short syl_pos_sentence_left, syl_pos_sentence_right;
		unsigned short syl_pos_pause_left, syl_pos_pause_right;
		//word
		char word_POS_left, word_POS, word_POS_right;
		char word_num_syl_left, word_num_syl, word_num_syl_right;
		unsigned char word_pos_sentence_left, word_pos_sentence_right;
		unsigned char word_pos_pause_left, word_pos_pause_right;
		//accent group
		char ag_type_left, ag_type, ag_type_right;
		unsigned char ag_num_syl_left, ag_num_syl, ag_num_syl_right;
		unsigned char ag_pos_sentence_left, ag_pos_sentence_right;
		unsigned char ag_pos_pause_left, ag_pos_pause_right;
		//pause
		char pau_type_left, pau_type_right;
		unsigned char pau_pos_sentence_left, pau_pos_sentence_right;
		//sentence
		char sentence_type;
		unsigned short sentence_num_phone;
		unsigned short sentence_num_syl;
		unsigned char sentence_num_word;
		unsigned char sentence_num_ag;
		unsigned char sentence_num_pau;
		char sentence_emotion;
	};
	//struct hts_label *labels;
	Lix p;//index
	p=u->phoneFirst();
//	int num_phoneme=u->phoneN(u->phoneLast(p, URANGE_UTT));
	//int ind=0; //index for labels
	//labels = (struct hts_label *)malloc(sizeof(struct hts_label)*num_phoneme);
	struct hts_label labels;

	//count pauses in sentences
	unsigned char sentence_num_pau=0;
	char sentence_emotion= u->cell(u->wordFirst()).getEmotion();
	for(p=u->phoneFirst(); p!=0; p=u->phoneNext(p)){
		if(u->cell(p).getPhone() == '_')
			sentence_num_pau += 1;
	}

	if(!strcmp(Language,"es")){
		//corregir los AG para castellano
		for(p=u->agrpFirst();p!=0; p=u->agrpNext(p)){
			int AG=u->cell(p).getAGrp();
			if(AG > 5){
				u->cell(p).setAGrp( AG - 5 );
				//avanzamos hasta un AG menor que 5 borrando todos
				Lix k=u->agrpNext(p);
				while(k!=0){
					if(u->cell(k).getAGrp() > 5)
						u->cell(k).setAGrp(0);
					else{
						//ponemos a cero y salimos del while
						u->cell(k).setAGrp(0);
						break;
					}
					k=u->agrpNext(k);
				}
			}
		}
	}

	long start_dur=0, end_dur=0; //en cientos de nanosegundos
	//fprintf(stderr,"%s\n", (const char *)a);
	//for each phoneme
	for(p=u->phoneFirst();p!=0;p=u->phoneNext(p)){
		char labels_tmp [128];
		//dur
		if(setdur)
			//end_dur=start_dur + u->cell(p).getDur()*10000;
			end_dur=start_dur + round(int((u->cell(p).getDur())/ 5.0 + 0.5)*5.0)*10000;
			//end_dur=start_dur +round(u->cell(p).getDur())*10000;
		//PHONEME LEVEL
		phone2sampa(labels.phoneme_prev_prev, u, u->phonePrev(u->phonePrev(p)));
		phone2sampa(labels.phoneme_prev, u, u->phonePrev(p));
		phone2sampa(labels.phoneme, u, p);
		phone2sampa(labels.phoneme_next, u , u->phoneNext(p));
		phone2sampa(labels.phoneme_next_next, u , u->phoneNext(u->phoneNext(p)));
		pos_syl(labels.phoneme_pos_syl_left, labels.phoneme_pos_syl_right, u, p, labels.phoneme_prev, labels.phoneme_next);
		pos_pau(labels.phoneme_pos_pau_left, labels.phoneme_pos_pau_right, u, p);

		//fprintf(stdout,"%s^%s-%s+%s=%s|%d-%d|%d+%d#", labels.phoneme_prev_prev, labels.phoneme_prev, labels.phoneme, labels.phoneme_next, labels.phoneme_next_next, labels.phoneme_pos_syl_left, labels.phoneme_pos_syl_right, labels.phoneme_pos_pau_left, labels.phoneme_pos_pau_right);
		if(setdur)
			sprintf(labels_tmp,"%d %d %s^%s-%s+%s=%s|%d-%d|%d+%d#", start_dur, end_dur,labels.phoneme_prev_prev, labels.phoneme_prev, labels.phoneme, labels.phoneme_next, labels.phoneme_next_next, labels.phoneme_pos_syl_left, labels.phoneme_pos_syl_right, labels.phoneme_pos_pau_left, labels.phoneme_pos_pau_right);

		else
			sprintf(labels_tmp,"%s^%s-%s+%s=%s|%d-%d|%d+%d#", labels.phoneme_prev_prev, labels.phoneme_prev, labels.phoneme, labels.phoneme_next, labels.phoneme_next_next, labels.phoneme_pos_syl_left, labels.phoneme_pos_syl_right, labels.phoneme_pos_pau_left, labels.phoneme_pos_pau_right);
		labels_string = labels_string + labels_tmp;

		//SYLLABLE LEVEL
		syllable_stress(labels.syl_left_stress, labels.syl_stress, labels.syl_right_stress, u, p);
		syllable_emphasis(labels.syl_left_emphasis, labels.syl_emphasis, labels.syl_right_emphasis, u, p);
		syllable_num_phone(labels.syl_left_num_phone, labels.syl_num_phone, labels.syl_right_num_phone, u, p);
		syllable_pos_word(labels.syl_pos_word_left, labels.syl_pos_word_right, u, p);
		syllable_pos_ag(labels.syl_pos_ag_left, labels.syl_pos_ag_right, u, p);
		syllable_pos_sentence(labels.syl_pos_sentence_left, labels.syl_pos_sentence_right, u, p);
		syllable_pos_pause(labels.syl_pos_pause_left, labels.syl_pos_pause_right, u, p);
		//fprintf(stdout,"%d-%d-%d!%d+%d+%d!%d=%d=%d!%d^%d!%d$%d!%d:%d!%d;%d#", labels.syl_left_stress, labels.syl_stress, labels.syl_right_stress, labels.syl_left_emphasis, labels.syl_emphasis, labels.syl_right_emphasis, labels.syl_left_num_phone, labels.syl_num_phone, labels.syl_right_num_phone, labels.syl_pos_word_left, labels.syl_pos_word_right, labels.syl_pos_ag_left, labels.syl_pos_ag_right, labels.syl_pos_sentence_left, labels.syl_pos_sentence_right, labels.syl_pos_pause_left, labels.syl_pos_pause_right);
		sprintf(labels_tmp,"%d-%d-%d!%d+%d+%d!%d=%d=%d!%d^%d!%d$%d!%d:%d!%d;%d#", labels.syl_left_stress, labels.syl_stress, labels.syl_right_stress, labels.syl_left_emphasis, labels.syl_emphasis, labels.syl_right_emphasis, labels.syl_left_num_phone, labels.syl_num_phone, labels.syl_right_num_phone, labels.syl_pos_word_left, labels.syl_pos_word_right, labels.syl_pos_ag_left, labels.syl_pos_ag_right, labels.syl_pos_sentence_left, labels.syl_pos_sentence_right, labels.syl_pos_pause_left, labels.syl_pos_pause_right);
		labels_string = labels_string + labels_tmp;

		//WORD LEVEL
		word_POS(labels.word_POS_left, labels.word_POS, labels.word_POS_right, u, p);
		word_num_syl(labels.word_num_syl_left, labels.word_num_syl, labels.word_num_syl_right, u, p);
		word_pos_sentence(labels.word_pos_sentence_left, labels.word_pos_sentence_right, u, p);
		word_pos_pause(labels.word_pos_pause_left, labels.word_pos_pause_right, u, p);
		//fprintf(stdout,"%d^%d^%d/%d$%d$%d/%d-%d/%d^%d#", labels.word_POS_left, labels.word_POS, labels.word_POS_right, labels.word_num_syl_left, labels.word_num_syl, labels.word_num_syl_right,labels.word_pos_sentence_left, labels.word_pos_sentence_right, labels.word_pos_pause_left, labels.word_pos_pause_right);
		sprintf(labels_tmp,"%d^%d^%d/%d$%d$%d/%d-%d/%d^%d#", labels.word_POS_left, labels.word_POS, labels.word_POS_right, labels.word_num_syl_left, labels.word_num_syl, labels.word_num_syl_right,labels.word_pos_sentence_left, labels.word_pos_sentence_right, labels.word_pos_pause_left, labels.word_pos_pause_right);
		labels_string = labels_string + labels_tmp;

		//ACCENT GROUP LEVE
		ag_type(labels.ag_type_left, labels.ag_type, labels.ag_type_right, u, p);
		ag_num_syl(labels.ag_num_syl_left, labels.ag_num_syl, labels.ag_num_syl_right, u, p);
		ag_pos_sentence(labels.ag_pos_sentence_left, labels.ag_pos_sentence_right, u, p);
		ag_pos_pause(labels.ag_pos_pause_left, labels.ag_pos_pause_right, u, p);
		//fprintf(stdout,"%d:%d:%d|%d;%d;%d|%d^%d|%d=%d#",labels.ag_type_left, labels.ag_type, labels.ag_type_right, labels.ag_num_syl_left, labels.ag_num_syl, labels.ag_num_syl_right, labels.ag_pos_sentence_left, labels.ag_pos_sentence_right, labels.ag_pos_pause_left, labels.ag_pos_pause_right);
		sprintf(labels_tmp,"%d:%d:%d|%d;%d;%d|%d^%d|%d=%d#",labels.ag_type_left, labels.ag_type, labels.ag_type_right, labels.ag_num_syl_left, labels.ag_num_syl, labels.ag_num_syl_right, labels.ag_pos_sentence_left, labels.ag_pos_sentence_right, labels.ag_pos_pause_left, labels.ag_pos_pause_right);
		labels_string = labels_string + labels_tmp;

		//PAUSE LEVEL
		pau_type(labels.pau_type_left, labels.pau_type_right, u, p);
		pau_pos_sentence(labels.pau_pos_sentence_left, labels.pau_pos_sentence_right, u, p);
	//	fprintf(stdout,"%d+%d;%d-%d#",labels.pau_type_left, labels.pau_type_right, labels.pau_pos_sentence_left, labels.pau_pos_sentence_right);
		sprintf(labels_tmp,"%d+%d;%d-%d#",labels.pau_type_left, labels.pau_type_right, labels.pau_pos_sentence_left, labels.pau_pos_sentence_right);
		labels_string = labels_string + labels_tmp;

		//SENTENCE LEVEL
		labels.sentence_type = sentence_type(u, p);
		labels.sentence_num_phone = u->phoneN(u->phoneLast(p, URANGE_UTT)) - sentence_num_pau;
		labels.sentence_num_syl = u->syllableN(u->syllableLast(p, URANGE_UTT));
		labels.sentence_num_word = u->wordN(u->wordLast(p, URANGE_UTT));
		labels.sentence_num_ag = u->agrpN(u->agrpLast(p, URANGE_UTT));
		labels.sentence_num_pau = sentence_num_pau;
		labels.sentence_emotion = sentence_emotion;

	//	fprintf(stdout,"%d=%d+%d-%d$%d^%d:%d\n", labels.sentence_type, labels.sentence_num_phone, labels.sentence_num_syl, labels.sentence_num_word, labels.sentence_num_ag, labels.sentence_num_pau, labels.sentence_emotion);
		sprintf(labels_tmp,"%d=%d+%d-%d$%d^%d:%d\n", labels.sentence_type, labels.sentence_num_phone, labels.sentence_num_syl, labels.sentence_num_word, labels.sentence_num_ag, labels.sentence_num_pau, labels.sentence_emotion);
		labels_string = labels_string + labels_tmp;

		//ind += 1;
		if(setdur)
			start_dur=end_dur;
	}
	//free(labels);
	//fprintf(stderr,"%s", (const char *)labels_string);
}
///////////////
#endif
