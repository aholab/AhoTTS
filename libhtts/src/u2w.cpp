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

Nombre fuente................ u2w.cpp
Nombre paquete............... aHoTTS
Lenguaje fuente.............. C++
Estado....................... -
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Borja Etxebarria
.............................

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
1.0.0    31/01/00  borja     codefreeze aHoTTS v1.0
0.0.0    24/11/97  borja     Codificacion inicial.

======================== Contenido ========================
<DOC>
Conversion Utt -> wave
</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include "u2w.hpp"

/**********************************************************/
/* Si se define NUSED_READY, al llegar a un flush las muestras
utilizadas pero no marcadas como completadas (wav_nused) tambien
se envian a la salida. Si no se define, solo las muestras marcadas
como completas (wav_nready) llegan a la salida */

#define NUSED_READYx

/**********************************************************/

KINDOF_BDEF(Utt2Wav)

/**********************************************************/

Utt2Wav::Utt2Wav( VOID )
{
	wav_buf=NULL;
	created=FALSE;
}

/**********************************************************/

Utt2Wav::~Utt2Wav( )
{
	if (wav_buf) { free(wav_buf-wav_nlen); wav_buf=NULL; }
}

/**********************************************************/

BOOL Utt2Wav::xcreate( VOID )
{
	assert(!created);
	created=TRUE;
	wav_nused=0; // buffer vacio
	wav_nready=0; // no hay muestras listas
	wav_nlen=0;
	flushing=0;
	woutack=FALSE;

#ifdef HTTS_TIMEEVS
	timeev_current=0;
#endif

#ifdef HTTS_INTERFACE_WAVEMARKS
	mrkl.clear();
	wav_origin=0;
#endif

	return TRUE;
}

/**********************************************************/

VOID Utt2Wav::setBufSize( INT n, INT nlen )
{
	assert(!wav_nused);

	if (!wav_buf || (n!=wav_len) || (nlen!=wav_nlen)) {
		if (wav_buf) free(wav_buf-wav_nlen);
		wav_len=n;
		wav_nlen=nlen;
		wav_buf=(DOUBLE*)malloc(sizeof(DOUBLE)*(wav_len+wav_nlen));
		wav_buf += wav_nlen;
		shiftedWav(0);
	}
}

/**********************************************************/

BOOL Utt2Wav::input( Utt* utt )
{
	assert(created);
	if (woutack) return FALSE;  // error: a la espera de ACK de salida
	if (!utt) return flush();  // si recibe NULL -> flush
	if (flushing) return FALSE; // error: flushing pendiente

	return xinput(utt);
}

/**********************************************************/

BOOL Utt2Wav::flush( VOID )
{
	assert(created);

	if (woutack) return FALSE;  // error: a la espera de ACK de salida
	flushing++;  // incrementar acumulador de flushes
	return TRUE;
}

/**********************************************************/

const DOUBLE* Utt2Wav::output( INT *len, BOOL *flush, INT mode, VOID *cb_n )
{
	assert(created);

	while (1) {
		while (1) {
			if (wav_nready>0) {  // envia salida si la hay
				if (len) *len = wav_nready;
				if (flush) *flush = FALSE;
				woutack = TRUE;
				return wav_buf;
			}
			if (!doNext(flushing>0)) break; // si no generamos nada nuevo, salir
		} // salimos del segundo while(1) solo si no hay nada que generar

		if (flushing) {  // si hay algun flush.

#ifdef NUSED_READY
			if (wav_nused>0) {  // envia _toda_ la salida si la hay
				if (len) *len = wav_nused;
				if (flush) *flush = FALSE;
				woutack = TRUE;
				return wav_buf;
			}
#endif
			// si no, devuelve un flush
			if (len) *len = 0;
			if (flush) *flush = TRUE;
			woutack = TRUE;
			return NULL;
		}

		assert(!woutack);  //nunca deberia dispararse

		if (mode != U2W_CB_NOCALL) {  //si hay que llamar al call-back
			callback(cb_n);
			if (mode == U2W_CB_CALL1) mode = U2W_CB_NOCALL;
		}
		else break;
	}

	if (len) *len = 0;        // no devolvemos nada
	if (flush) *flush = FALSE;
	return NULL;
}

/**********************************************************/

BOOL Utt2Wav::isFlush( VOID )
{
	if (!woutack) return FALSE;  // si no hay ACK pendiente, fuera
	if (!flushing) return FALSE;  // si no hay flushes pendientes, fuera.
	if (wav_nready>0) return FALSE;  // si quedan muestras, fuera
#ifdef NUSED_READY
	if (wav_nused>0) return FALSE;  // si quedan muestras residuales, fuera.
#endif
	return TRUE;  // si no hay muestras y hay un ACK y hay flushings pendientes, estamos en un flush!
}

/**********************************************************/

#ifdef HTTS_INTERFACE_WAVEMARKS

const Mark1DList & Utt2Wav::mrkget( VOID )
{
	return mrkl;
}

#endif

/**********************************************************/

#ifdef HTTS_INTERFACE_WAVEMARKS

VOID Utt2Wav::mrkdel( VOID )
{
	mrkl.clear();
}

#endif

/**********************************************************/

#ifdef HTTS_INTERFACE_WAVEMARKS
BOOL Utt2Wav::outack( INT n, BOOL mrkdel )
#else
BOOL Utt2Wav::outack( INT n )
#endif
{
	assert(created);

	if (!woutack) return FALSE;
	woutack = FALSE;

#ifdef HTTS_INTERFACE_WAVEMARKS
	if (mrkdel)	mrkl.clear();  // borrado de marcas
#endif

	if (wav_nready>0)  // si habia muestras listas, actualiza
		return shiftReady(n,FALSE);

	assert(flushing);  // nunca deberia dispararse

	// si llegamos aqui, tiene que ser por estar en estado flush
#ifdef NUSED_READY
	if (wav_nused) // si habia muestras, actualiza
		return shiftReady(n,TRUE);
	else
#endif
	{
#ifdef HTTS_TIMEEVS
		timeev_current=0;  // reinicializar eventos de tiempo
#endif
		flushing--;
		if (!flushing) reset();  // resetea motor de sintesis (una vez basta, al llegar la cuenta de resets a cero)
#ifdef HTTS_INTERFACE_WAVEMARKS
		mrkl.clear();  // borrado forzoso de marcas
		wav_origin=0;  // reinicializa conteo de muestras
#endif
	}
	return TRUE;
}

/**********************************************************/

BOOL Utt2Wav::shiftReady( INT n, BOOL inflush )
{
	(void)inflush;  // para que no proteste el compilador

	BOOL ret=TRUE;
#ifdef NUSED_READY
	INT nr = (inflush) ? wav_nused : wav_nready;
#else
#define nr wav_nready
#endif

	if (n>nr) { n=nr;  ret=FALSE; }
	else if (n<0) { n=0;  ret=FALSE; }

	if (n) {
		wav_nused -= n;
#ifdef NUSED_READY
		if (wav_nready<n) wav_nready=0; else wav_nready -= n;
#else
		wav_nready -= n;
#endif

		memcpy(wav_buf-wav_nlen,wav_buf+n-wav_nlen,sizeof(DOUBLE)*(wav_nused+wav_nlen));
#ifdef HTTS_INTERFACE_WAVEMARKS
		wav_origin += n;
#endif
		shiftedWav(n);
	}
	return ret;
}

/**********************************************************/

VOID Utt2Wav::shiftedWav( INT n )
{
	if (!wav_buf) return;
	assert(wav_nused+n<=wav_len);
	if (!n) memset(wav_buf-wav_nlen,0,(wav_len+wav_nlen)*sizeof(DOUBLE));
	else memset(wav_buf+wav_nused,0,n*sizeof(DOUBLE));
}

/**********************************************************/
