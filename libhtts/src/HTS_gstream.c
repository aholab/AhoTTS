/* ----------------------------------------------------------------- */
/*           The HMM-Based Speech Synthesis Engine "hts_engine API"  */
/*           developed by HTS Working Group                          */
/*           http://hts-engine.sourceforge.net/                      */
/* ----------------------------------------------------------------- */
/*                                                                   */
/*  Copyright (c) 2001-2011  Nagoya Institute of Technology          */
/*                           Department of Computer Science          */
/*                                                                   */
/*                2001-2008  Tokyo Institute of Technology           */
/*                           Interdisciplinary Graduate School of    */
/*                           Science and Engineering                 */
/*                                                                   */
/* All rights reserved.                                              */
/*                                                                   */
/* Redistribution and use in source and binary forms, with or        */
/* without modification, are permitted provided that the following   */
/* conditions are met:                                               */
/*                                                                   */
/* - Redistributions of source code must retain the above copyright  */
/*   notice, this list of conditions and the following disclaimer.   */
/* - Redistributions in binary form must reproduce the above         */
/*   copyright notice, this list of conditions and the following     */
/*   disclaimer in the documentation and/or other materials provided */
/*   with the distribution.                                          */
/* - Neither the name of the HTS working group nor the names of its  */
/*   contributors may be used to endorse or promote products derived */
/*   from this software without specific prior written permission.   */
/*                                                                   */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND            */
/* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,       */
/* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF          */
/* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE          */
/* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS */
/* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,          */
/* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,     */
/* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON */
/* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,   */
/* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY    */
/* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE           */
/* POSSIBILITY OF SUCH DAMAGE.                                       */
/* ----------------------------------------------------------------- */

#ifndef HTS_GSTREAM_C
#define HTS_GSTREAM_C

#ifdef __cplusplus
#define HTS_GSTREAM_C_START extern "C" {
#define HTS_GSTREAM_C_END   }
#else
#define HTS_GSTREAM_C_START
#define HTS_GSTREAM_C_END
#endif                          /* __CPLUSPLUS */

HTS_GSTREAM_C_START;

/* hts_engine libraries */
#include "HTS_hidden.h"

/* HTS_GStreamSet_initialize: initialize generated parameter stream set */
void HTS_GStreamSet_initialize(HTS_GStreamSet * gss)
{
   gss->nstream = 0;
   gss->total_frame = 0;
   gss->total_nsample = 0;
   gss->gstream = NULL;
   gss->gspeech = NULL;
}

/* HTS_GStreamSet_create: generate speech */
/* (stream[0] == spectrum && stream[1] == lf0) */
HTS_Boolean HTS_GStreamSet_create(HTS_GStreamSet * gss, HTS_PStreamSet * pss, int stage, HTS_Boolean use_log_gain, int sampling_rate, int fperiod, double alpha, double beta, HTS_Boolean * stop, double volume, HTS_Audio * audio)
{
   int i, j, k;
   int msd_frame;
   // DERRO: quito el vocoder porque voy a usar el m�o propio
   //HTS_Vocoder v;
   // int nlpf = 0;
   // double *lpf = NULL;
   (void) stage;
   (void) beta;
   (void) use_log_gain;
   (void) stop;
   (void) volume;
   (void) audio;

   /* check */
   if (gss->gstream || gss->gspeech) {
      HTS_error(1, "HTS_GStreamSet_create: HTS_GStreamSet is not initialized.\n");
      return FALSE;
   }

   /* initialize */
   gss->nstream = HTS_PStreamSet_get_nstream(pss);
   gss->total_frame = HTS_PStreamSet_get_total_frame(pss);
   // DERRO: reemplazo el n�mero de muestras por el que calculo yo con mi funcion de ahocoder
   //gss->total_nsample = fperiod * gss->total_frame;
   gss->total_nsample = (int)get_ahocoder_waveform_length((unsigned int)fperiod, (unsigned int)gss->total_frame);
   gss->gstream = (HTS_GStream *) HTS_calloc(gss->nstream, sizeof(HTS_GStream));
   for (i = 0; i < gss->nstream; i++) {
      gss->gstream[i].static_length = HTS_PStreamSet_get_static_length(pss, i);
      gss->gstream[i].par = (double **) HTS_calloc(gss->total_frame, sizeof(double *));
      for (j = 0; j < gss->total_frame; j++)
         gss->gstream[i].par[j] = (double *) HTS_calloc(gss->gstream[i].static_length, sizeof(double));
   }
   // DERRO: debido a la implementacion de ahocoder, voy a reservar a tama�o double, que es
   //        el que usa ahocoder al generar, y luego lo sobreescribire con shorts dentro del
   //        propio ahocoder, ahorrando as� tener que reservar distintos bloques de memoria
   //gss->gspeech = (short *) HTS_calloc(gss->total_nsample, sizeof(short));
   gss->gspeech = (short *) HTS_calloc(gss->total_nsample, sizeof(double));

   /* copy generated parameter */
   for (i = 0; i < gss->nstream; i++) {
      if (HTS_PStreamSet_is_msd(pss, i)) {      /* for MSD */
         for (j = 0, msd_frame = 0; j < gss->total_frame; j++)
            if (HTS_PStreamSet_get_msd_flag(pss, i, j)) {
               for (k = 0; k < gss->gstream[i].static_length; k++)
                  gss->gstream[i].par[j][k] = HTS_PStreamSet_get_parameter(pss, i, msd_frame, k);
               msd_frame++;
            } else
               for (k = 0; k < gss->gstream[i].static_length; k++)
                  gss->gstream[i].par[j][k] = LZERO;
      } else {                  /* for non MSD */
         for (j = 0; j < gss->total_frame; j++)
            for (k = 0; k < gss->gstream[i].static_length; k++)
               gss->gstream[i].par[j][k] = HTS_PStreamSet_get_parameter(pss, i, j, k);
      }
   }

   /* check */
   if (gss->nstream != 2 && gss->nstream != 3) {
      HTS_error(1, "HTS_GStreamSet_create: The number of streams should be 2 or 3.\n");
      HTS_GStreamSet_clear(gss);
      return FALSE;
   }
   if (HTS_PStreamSet_get_static_length(pss, 1) != 1) {
      HTS_error(1, "HTS_GStreamSet_create: The size of lf0 static vector should be 1 and is = %d.\n", HTS_PStreamSet_get_static_length(pss, 1) );
      HTS_GStreamSet_clear(gss);
      return FALSE;
   }
	//INAKI
   /*if (gss->nstream >= 3 && gss->gstream[2].static_length % 2 == 0) {
      HTS_error(1, "HTS_GStreamSet_create: The number of low-pass filter coefficient should be odd numbers.");
      HTS_GStreamSet_clear(gss);
      return FALSE;
   }*/

   /* synthesize speech waveform */
   // DERRO: desactivo el vocoder tradicional y lo reemplazo por ahocoder, con o sin excitaci�n
   if (gss->nstream == 2)
	   gen_ahocoder_waveform(gss->gspeech, gss->total_nsample, (unsigned int)sampling_rate, (unsigned int)fperiod, (unsigned int)gss->total_frame, gss->gstream[1].par, NULL, (unsigned int)gss->gstream[0].static_length-1, alpha, gss->gstream[0].par);
   else
	   gen_ahocoder_waveform(gss->gspeech, gss->total_nsample, (unsigned int)sampling_rate, (unsigned int)fperiod, (unsigned int)gss->total_frame, gss->gstream[1].par, gss->gstream[2].par, (unsigned int)gss->gstream[0].static_length-1, alpha, gss->gstream[0].par);
	// if (audio)
      //HTS_Audio_flush(audio);
	return TRUE;
/*HTS_Vocoder_initialize(&v, gss->gstream[0].static_length - 1, stage, use_log_gain, sampling_rate, fperiod);
   if (gss->nstream >= 3)
      nlpf = (gss->gstream[2].static_length - 1) / 2;
   for (i = 0; i < gss->total_frame && (*stop) == FALSE; i++) {
      if (gss->nstream >= 3)
         lpf = &gss->gstream[2].par[i][0];
      HTS_Vocoder_synthesize(&v, gss->gstream[0].static_length - 1, gss->gstream[1].par[i][0], &gss->gstream[0].par[i][0], nlpf, lpf, alpha, beta, volume, &gss->gspeech[i * fperiod], audio);
   }
   HTS_Vocoder_clear(&v);
   if (audio)
      HTS_Audio_flush(audio);

   return TRUE;*/
}

/* HTS_GStreamSet_get_total_nsample: get total number of sample */
int HTS_GStreamSet_get_total_nsample(HTS_GStreamSet * gss)
{
   return gss->total_nsample;
}

/* HTS_GStreamSet_get_total_frame: get total number of frame */
int HTS_GStreamSet_get_total_frame(HTS_GStreamSet * gss)
{
   return gss->total_frame;
}

/* HTS_GStreamSet_get_static_length: get static features length */
int HTS_GStreamSet_get_static_length(HTS_GStreamSet * gss, int stream_index)
{
   return gss->gstream[stream_index].static_length;
}

/* HTS_GStreamSet_get_speech: get synthesized speech parameter */
short HTS_GStreamSet_get_speech(HTS_GStreamSet * gss, int sample_index)
{
   return gss->gspeech[sample_index];
}

/* HTS_GStreamSet_get_parameter: get generated parameter */
double HTS_GStreamSet_get_parameter(HTS_GStreamSet * gss, int stream_index, int frame_index, int vector_index)
{
   return gss->gstream[stream_index].par[frame_index][vector_index];
}

/* HTS_GStreamSet_clear: free generated parameter stream set */
void HTS_GStreamSet_clear(HTS_GStreamSet * gss)
{
   int i, j;

   if (gss->gstream) {
      for (i = 0; i < gss->nstream; i++) {
         for (j = 0; j < gss->total_frame; j++)
            HTS_free(gss->gstream[i].par[j]);
         HTS_free(gss->gstream[i].par);
      }
      HTS_free(gss->gstream);
   }
   if (gss->gspeech)
      HTS_free(gss->gspeech);
   HTS_GStreamSet_initialize(gss);
}

HTS_GSTREAM_C_END;

#endif                          /* !HTS_GSTREAM_C */
