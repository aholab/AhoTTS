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
#ifndef __HTTS_CFG_H__
#define __HTTS_CFG_H__

/**********************************************************/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/*
(C) 1997 TSR/Aholab - ETSII/IT Bilbao (UPV/EHU)

Nombre fuente................ htts_cfg.h
Nombre paquete............... aHoTTS
Lenguaje fuente.............. C
Estado....................... -
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Borja Etxebarria

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
1.0.3	 08/11/11  Inaki	 Añadir Catalan (con Festcat)	
1.0.2	 15/12/10  Inaki	 Añadir Método HTS	
1.0.1	 03/10/07  Inaki     Añadir Metodo Corpus
1.0.1    18/05/01  Yon2.     Incluir USE_TOKENIZER
1.0.0    31/01/00  borja     codefreeze aHoTTS v1.0
0.0.0    06/02/98  borja     Codificacion inicial.

======================== Contenido ========================
Configuracion general para activar/desactivar ciertos modulos,
opciones, etc que influyen durante la compilacion.
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

/* Usamos el tokenizer. */
#define USE_TOKENIZER

/* acceso a informacion de marcas para etiquetado del audio de salida */
#define HTTS_INTERFACE_WAVEMARKS

/* acceso a variables globales que identifican los metodos
de sintesis linkados. */
#define HTTS_METHVARS

/* acceso a variables globales que identifican los idiomas linkados */
#define HTTS_LANGVARS

/* conversion lista_de_celda<-->texto (para debugeo y funcionamiento
en modulos separados) */
#define HTTS_IOTXT

/* HTTS_DEBUG esta definido en las etapas de desarrollo. */
#ifndef NDEBUG
#define HTTS_DEBUGx
#endif
#define HTTS_DEBUGx  // $$$ forzada

/* Si HTTS_THREADSAFE se verifica que todo pueda funcionar
con varios threads. Si no, algunas cosas se optimizan, pero
ya no es thread-safe. */
#define HTTS_THREADSAFE

/* Si se define HTTS_MULTIPITCH, se incluye soporte para
varios valores de pitch en un unico fonema. Si no se define,
solo hay un valor de pitch por fonema (aunque las funciones
para multipitch siguen estando ahi, pero solo manejan un valor) */
#define HTTS_MULTIPITCH

/* Si HTTS_TIMEEVS esta definido, se incluye soporte para insertar
eventos de tiempo en el texto, y detectarlos en el wav de salida */
#define HTTS_TIMEEVS

/*su HTTS_PROSO_VAL está definido, se incluye soporte para modificar la prosodia
desde el texto de entrada*///INAKI
#define HTTS_PROSO_VALx

/******* Idiomas */
#define HTTS_LANG_ES     // castellano
#define HTTS_LANG_EU     // euskara
#define HTTS_LANG_FESTx     // lenguajes que usan festival, catalan e inglés
#define HTTS_LANG_GLx      // gallego


/******* Metodos de prosodia-pitch */
/* castellano */
#define HTTS_ES_PAU2

/* euskara */
#define HTTS_EU_PAU1


/******* Metodos de motor de sintesis */
#define HTTS_METHOD_HTS //INAKI

/**********************************************************/
/**********************************************************/
/**********************************************************/
/**********************************************************/
/**********************************************************/
/**********************************************************/
/* Otros defines y verificaciones de validez, en funcion de
los defines anteriores */

#if !defined(HTTS_METHOD_FRANKY)&&!defined(HTTS_METHOD_TDPSOLA)&&!defined(HTTS_METHOD_LPPSOLA)&&!defined(HTTS_METHOD_MBROLA)&&!defined(HTTS_METHOD_FORMANT)&&!defined(HTTS_METHOD_CORPUS)&&!defined(HTTS_METHOD_HTS)
#error  No synthesis method (motor) is compiled in!
#endif

#if !defined(HTTS_LANG_ES)&&!defined(HTTS_LANG_EU)&&!defined(HTTS_LANG_FEST)&&!defined(HTTS_LANG_GL)
#error  No language is compiled in!
#endif


/**********************************************************/

#endif

