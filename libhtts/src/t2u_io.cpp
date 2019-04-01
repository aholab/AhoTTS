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

Nombre fuente................ t2u_io.cpp
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
0.1.0    --/--/97  borja     codificacion inicial

======================== Contenido ========================
<DOC>
Interfaz de entrada salida con otros modulos para el
modulo de paso de cadena a lista.

Clase que permiten convertir un texto de entrada en una utterance
de palabras (text to utterance, t2u).

Es una clase sencillita, tiene en cuenta los espacios y tabs para separar
las palabras, y tambien almacena los fines de frase (punto, interrogacion,
exclamacion) y otras pausas (comas, paentesis, etc).

Se supone que el texto de entrada esta normalizado.


sets/gets soportados (pre/pos create):
---------------------

* TxtMode={Utt|Word|Spell} (pre/pos) Permite seleccionar el
modo del trabajo: frase a frase, palabra a palabra, o deletreo.
Actualmente solo funcionan el modo Utt y el Word, ya que
el Spell se ignora.
</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include <string.h>

#include "uti.h"
#include "t2u.hpp"
#include "httsmsg.h"

/**********************************************************/

T2ULst::T2ULst( VOID )
{
	utt = 0;
	flushing = 0;
	espera_outack = FALSE;
	txtMode = ModeUtt;
	uttcompleta = FALSE;

	reset();
}

/**********************************************************/

T2ULst::~T2ULst()
{
	if (utt) utt->clear();
}

/**********************************************************/

BOOL T2ULst::create( UttWS *ut )
{
	assert(!utt);
	utt = ut;
	return TRUE;
}

/**********************************************************/
/* Interfaz de entrada de texto al modulo de paso de cadena a lista.

   Parametros:
   - {str} (I) cadena de texto que se quiere introducir en el
   modulo de paso a lista. Si se envia {str}==NULL, se ignora.

   La funcion {devuelve} el numero de
   caracteres de la cadena {str} que se han aceptado. Queda en
   manos del usuario volver a enviar posteriormente el resto de
   caracteres de la cadena {str} que no se han aceptado, pero
   antes de enviar estos caracteres deberemos leer la salida del
   modulo para hacer sitio, que si no, no entran!

   Cuando se ha enviado un flush con la funcion flush()
   el modulo no acepta _ningun_ caracter de entrada hasta que el
   flush se haya propagado a la salida (aunque si se aceptan mas
   flush) */

INT T2ULst::input(const CHAR * str)
{
	assert(utt);

	if (!str)
		return 0;
	if (espera_outack)
		return 0;

	// flushing pendiente o propagacion de flushing==> no acepta entrada
	if (flushing)
		return 0;

	return entrada_cadena(str);		// acepta texto
}

/**********************************************************/
/* Interfaz de entrada de texto al modulo de paso de cadena a lista.
Similar al otro metodo input, pero recibe un unico char (y {devuelve}
TRUE si lo acepta o FALSE si lo rechaza). */

BOOL T2ULst::input(CHAR ch)
{
	assert(utt);

	CHAR str[2] = { '\0', '\0' };
	str[0]=ch;
	return (input(str) > 0);
}

/**********************************************************/
/* Funcion que fuerza un flush de los buffers internos del
   modulo. Cualquier utterance pendiente de procesar se considera
   completada y se envia a la salida sin esperar mas caracteres.

   El modulo no aceptara mas caracteres de entrada hasta que
   se lea y acepte la salida.

   La funcion devuelve si se ha aceptado el flush o no. */

BOOL T2ULst::flush(VOID)
{
	assert(utt);

	if (espera_outack)
		return FALSE;
	flushing++;
	entrada_cadena(NULL);	// flush de lo que tengamos buferizado

	return TRUE;
}

/**********************************************************/
/* Interfaz de salida de texto del modulo de paso a lista.
   La cadena que se ha ido introduciendo con input() se
   extrae como lista.
   Se implementan varios modos de funcionamiento, incluyendo una
   llamada automatica a un callback para
   solicitar datos de entrada en caso de no haber ningun dato de
   salida disponible. Ademas el proceso puede ser bloqueante si
   se desea (no se sale del metodo hasta disponer de algun
   valor de salida).

   Parametros:
   - {mode} (I)  modo de funcionamiento en caso de no haber
	 salida disponible (valores T2U_CB_???? )
   - {cb_n} (I)  es un parametro que se pasa al callback y que el
   usuario puede usar para lo que le convenga.
   Retorno:
   Se {devuelve} una referencia a la utterance de salida (que podremos
   copiar, o transpasar (===borrar)). Se {devuelve} NULL si no hay lista
   completa, o si es un flush.
   En {flush} se devuelve si es o no un flush.
   Cuando hay una salida valida (tanto lista con contenido como flush)
   el usuario debe despues llamar a la funcion outack()
   para indicar que ha aceptado la lista o el flush.
   Mientras no se llame a outack(), no se aceptan entradas,
   y output() devolvera continuamente lo mismo (cuidado, que
   si el usuario ya a borrado la lista, obtendra una lista
   vacia).
   En el momento en que se llame a outack() la lista interna
   se vacia (si es que el usuario no la ha transferido ya a
   otro sitio. */

Utt *T2ULst::output(BOOL * flush, INT mode, VOID *cb_n)
{
	assert(utt);

	if (espera_outack) {
		*flush = ((uttcompleta == FALSE) && (flushing > 0));
		return (*flush) ? (UttWS*)NULL : utt;
	}
	*flush = 0;
	while (1) {
		if (uttcompleta) {  //si tiene una frase completa pasala
			espera_outack = TRUE;
			return utt;
		}
		if (flushing) {	 //si hay un flushing mandalo
			espera_outack = TRUE;
			*flush = 1;
			return NULL;
		}
		assert(!espera_outack);  // nunca deberia suceder

		// si hay que llamar al call-back
		if (mode != T2U_CB_NOCALL) {
			callback(cb_n);
			if (mode == T2U_CB_CALL1)
				mode = T2U_CB_NOCALL;
		}
		else
			break;  // salir del while
	}

	return NULL;
}

/**********************************************************/
/* Interfaz de salida de texto del modulo de paso a lista.
   Esta funcion sirve para enviar el ACK de utterance de salida
   aceptada o de flush aceptado. Si la lista contiene algo,
   se borra definitivamente! */

VOID T2ULst::outack(VOID)
{
	assert(utt);

	if (!espera_outack)
		return;

	espera_outack = FALSE;

	if (uttcompleta) {	// si habiamos devuelto una lista, reinicializamos
		utt->clear();
		uttcompleta = FALSE;
	}
	else if (flushing)
		flushing--;		// si hay flushing, lo borramos

}

/**********************************************************/

BOOL T2ULst::set(const CHAR * param, const CHAR * val)
{
	if (!strcmp(param,"TxtMode")) {
		if (!strcmp(val,"Utt")) txtMode=ModeUtt;
		else if (!strcmp(val,"Word")) txtMode=ModeWord;
		else if (!strcmp(val,"Spell")) txtMode=ModeSpell;
		else htts_error("T2ULst: invalid %s '%s'",param,val);
		return TRUE;
	}
	return FALSE;
}

/**********************************************************/

const CHAR *T2ULst::get(const CHAR * param)
{
	if (!strcmp(param,"TxtMode")) {
		switch (txtMode) {
		case ModeUtt : return "Utt";
		case ModeWord : return "Word";
		case ModeSpell : return "Spell";
		default : htts_error("T2ULst: invalid %s %ld",param,(long)txtMode);
		}
	}
	return NULL;
}

/**********************************************************/
