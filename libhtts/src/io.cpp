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
Copyright: 1998 - TSR/Aholab (DET) ETSII/IT-Bilbao

Nombre fuente................ io.cpp
Nombre paquete............... normalizador
Lenguaje fuente.............. C++
Estado....................... desarrollo
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Juan Luis Murugarren

Version   dd/mm/aa  Autor     Proposito de la edicion
-------   --------  --------  -----------------------
0.1.0	 12/07/10  Inaki	 Añadir celdas para modificar la prosodia desde el texto (proyecto Aritz) HTTS_PROSO_VAL
0.0.11    09/07/01  Yon2.     If returned list is empty return NULL.
0.0.10    06/07/01  Yon2.     Delayed Flush Support.
0.0.09    06/07/01  Yon2.     Proper Get/Set ninja programming.
0.0.08    05/07/01  Yon2.     A bit more DEBUG info added.
0.0.07    05/07/01  Yon2.     Set Method does nothing and returns FALSE.
0.0.06    04/05/01  Yon2.     Right Warning/Error info.
0.0.05    24/04/01  Yon2.     Multiling support.
0.0.04    28/03/01  Yon2.     More data need allowed.
0.0.03    05/02/01  Yon2.     Not Created -> i/o not allowed.
0.0.02    15/01/01  Yon2.     Text output support.
0.0.01    15/01/01  Yon2.     CallBack Support.
0.0.00    11/01/01  Yon2.     Codificacion inicial.

======================== Contenido ========================

  En este fichero se encuentran las primitivas de entrada/
salida que modelan el funcionamiento global de los m¢dulos
en HTTS.

  Las primitivas de entrada/salida son:
-input -> Entrada con una cadena o un caracter.
-output -> Devuelve una frase en un puntero UttWS
-outack -> Confirma la frase devuelta por "output" y
        la elimina, liber ndola de memoria.
-flush -> Hace que se propaguen las cadenas almacenadas
       en los buffers internos.

  Para modificar el funcionamiento del normalizador:
-set
-get

  Estas dos est n definidas pero no hacen nada de nada.

  Para salida de texto normalizado en formato de array de
caracteres se usa outputtext. El funcionamiento el es mismo
que el de output desde el punto de vista de uso de primitivas.

Yon2.
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/



#include "t2l.hpp"
#include "httsmsg.h"

/**********************************************************/
/* El m‚todo INPUT implementa la funcion de entrada de texto
para tokenizarlo y obtener a la salida una lista de tokens
normalizados. {str} (I) es la cadena de texto que se quiere
introducir en el modulo de paso a lista. Si se envia
{str}==NULL, se ignora.

La funcion {devuelve} el numero de caracteres de la cadena
{str} que se han aceptado y se han procesado/procesar n.
Aunque devuelve el numero exacto de caracteres que acepta
este n£mero es o todos o ninguno. :)

Si hay una salida pendiente de confirmaci¢n no se aceptan
entradas.
Si hay un flush pendiente tampoco se acepta entrada. Cosa
que me parece una txorrada inmensa, porque si no se acepta
entrada, tampoco se pueden hacer m s flushes y obliga a
esperar por pilots a que salgan todas las frases que entraron
hasta el flush, sin poder hacer nada m s. :( */
/**********************************************************/

INT TextToList::input(const CHAR *str)
{
  INT retval = 0;

#ifdef HTTS_DEBUG
  if (!created) htts_error("TextToList::input(str) - Still no create method call registered.");
#endif
  if (!created) return retval;

#ifdef HEAVY_DEBUG
#ifdef HTTS_DEBUG
//	htts_warn("TextToList::input()");
  if (!str) htts_warn("TextToList::input() - NULL input string.");
  if (outAckPend) htts_warn("TextToList::input() - There is an outack pendding.");
  if (flushPend) htts_warn("TextToList::input() - There is a flush pendding.");
  if (ctFull) htts_warn("TextToList::input() - \"Crude Text List\" is Full.");
	
#endif
#endif

  if (!str) return retval;
  if (outAckPend || flushPend || ctFull) return retval;

  retval = ct2nt(str);

  return retval;
}

/**********************************************************/


/**********************************************************/
/* Este metodo implementa la funcion de entrada de texto
para tokenizarlo y obtener a la salida una lista de tokens
normalizados. {ch} (I) es un caracter del texto que se
quiere tokenizar y normalizar. {devuelve} TRUE si lo acepta
(los procesa) o FALSE si lo rechaza.*/
/**********************************************************/

BOOL TextToList::input(CHAR ch)
{
	CHAR str[]="X";

#ifdef HTTS_DEBUG
  if (!created) htts_error("TextToList::input(char) - Still no create method call registered.");
#endif
  if (!created)
	  return FALSE;
  str[0]=ch;

  return input(str)?TRUE:FALSE;
}

/**********************************************************/


/**********************************************************/
/* Este metodo fuerza un flush de los buffers internos del
modulo. Esto significa que cualquier token en proceso de
formancion se considera completo y se anade a la lista de
tokens sin esperar a nuevos caracteres.

Si existen flushes pendientes de propagarse, se fuerza a
que si la lista no esta vacia y no existe un frase
completa, a la hora de analizar la existencia de frases se
considere a los tokens existentes como una frase valida.

Esto implica que si la lista no esta vacia y hay uno o mas
flushes pendientes, al llamar a output() se tenga asegurado
que se obtendra una utterance de salida, aunque los tokens
no formen una frase. Si existe mas de una frase, se
obtendra mediante sucesivas llamadas a output() las
diferentes frases, y se existen flushes pendientes de
propagarse, se tiene asegurado que con la ultima llamada
que se hubiera ralizado a output() en la que se hubiera
obtenido una frase, la lista quedaria vacia.

La funcion {devuelve} TRUE si se ha aceptado el flush y
con FALSE que no. No se aceptan nuevos flush si hay una
salida (utterance o flush) pendiente de ser aceptada (ACK),
pero si se aceptan unque existan flush sin propagar, frases
sin detectar (para detectarlas es necesario llamar a
output()), o se halla sobrepasado el limite de
almacenamiento.*/


/**********************************************************/

BOOL TextToList::flush(VOID)
{
  CtI p;

#ifdef HTTS_DEBUG
  if (!created) htts_error("TextToList::flush() - Still no create method call registered.");
#endif
  if (!created) return FALSE;

#ifdef HEAVY_DEBUG
#ifdef HTTS_DEBUG
  htts_warn("TextToList::flush()");
	if (outAckPend) htts_warn("TextToList::flush() - There is an outack pendding.");
#endif
#endif

	if (outAckPend) return(FALSE);	// Salida pendiente de confirmar.
//	flushing++;

  //Sacamos lo que pudiera haber en el Word Chopper ...
  if (wordStarted == TRUE)	{
		wordbuff[j]='\x0';
		#ifdef HTTS_PROSO_VAL
		p = ct.append(wordbuff, groupStarted, *(emo+indiceEmo), *(emoInt+indiceEmo),val_Pros_break,val_Pros_emphasis,val_Pros_pitch,val_Pros_range,val_Pros_rate,val_Pros_volume);//Aritz
		#else
		p = ct.append(wordbuff, groupStarted, *(emo+indiceEmo), *(emoInt+indiceEmo));
		#endif
		wordStarted = FALSE;
		pattern[k]='\x0';
		ct.setPattern(p, pattern);
	}
  //... y lo dejamos en un estado consistente.
	groupStarted = TRUE;

  procLastGrp = TRUE;

	//No permitimos demora por necesidad de mas datos.
	moreDataNeedAllowed = FALSE;

  //Procesamos el posible extra
  ct2nt(TRUE);

  flushPend = TRUE;
  ct.setFlush(ct.lastGrp());

  return TRUE;
}

/**********************************************************/


/**********************************************************/

pCHAR TextToList::outputtext(BOOL *flush, INT mode, VOID *param)
{
	pCHAR retval = NULL;

#ifdef HTTS_DEBUG
	if (!created) htts_error("TextToList::outputtext() - Still no create method call registered.");
#endif
	if (!created) return retval;

	switch(mode)  {
		case TEX_CB_BLOCKCALL:
			while (!retval)  {
				callback(param);
				retval = wsoutputtext(flush, mode, param);
				if (flushPend && ct.empty()) {
					*flush = TRUE;
					break;
				}
			}
			break;
		case TEX_CB_CALL1:
			callback(param);
			retval = wsoutputtext(flush, mode, param);
			if (flushPend && ct.empty()) {
				*flush = TRUE;
				break;
			}
			break;
		case TEX_CB_NOCALL:
			retval = wsoutputtext(flush, mode, param);
			break;
		default:
#ifdef HTTS_DEBUG
			htts_error("TextToList::outputtext() - No valid CallBack mode selected.");
#endif
			break;

	}


#ifdef HEAVY_DEBUG
#ifdef HTTS_DEBUG
		if (!retval) htts_warn("TextToList::outputtext() - No utterance generated.");
#endif
#endif

if (*flush) flushPend = FALSE;

outputUTTWS = FALSE;

return retval;

}

/**********************************************************/


/**********************************************************/
/* Si se ha sobrepasado la capacidad de almacenamiento, y en
el texto almacenado no hay ninguna frase como Dios manda,
el objeto considera todo el texto almacenado como una
frase. */

/**********************************************************/

UttWS *TextToList::output(BOOL *flush, INT mode, VOID *param)
{
  UttWS *retval = NULL;

#ifdef HTTS_DEBUG
  if (!created) htts_error("TextToList::output() - Still no create method call registered.");
#endif
  if (!created) return retval;

  switch(mode)  {
    case TEX_CB_BLOCKCALL:
         while (!retval)  {
               callback(param);
               retval = wsoutput(flush, mode, param);
               if (flushPend && ct.empty()) {
                 *flush = TRUE;
                 break;
               }
         }
         break;
    case TEX_CB_CALL1:
         callback(param);
         retval = wsoutput(flush, mode, param);
         if (flushPend && ct.empty()) {
           *flush = TRUE;
           break;
         }
         break;
    case TEX_CB_NOCALL:
         retval = wsoutput(flush, mode, param);
         if (flushPend && ct.empty()) {
           *flush = TRUE;
           break;
         }
         break;
    default:
#ifdef HTTS_DEBUG
       htts_error("TextToList::output() - No valid CallBack mode selected.");
#endif
        break;

  }


#ifdef HEAVY_DEBUG
#ifdef HTTS_DEBUG
//htts_warn("TextToList::output()");
  if (!retval) htts_warn("TextToList::output() - No utterance generated.");
	else if (retval->empty()) htts_warn("TextToList::output() - Empty list returned.");
	if (retval) htts_warn("TextToList::output() - UTTWS output type returned.");
	if (*flush) htts_warn("TextToList::output() - FLUSH output type returned.");
#endif
#endif

  if (*flush) flushPend = FALSE;

  outputUTTWS = TRUE;

	//Hay un flush retardado?
	if (delayFlush && !retval)	{
		delayFlush = FALSE;
		*flush = TRUE;
#ifdef HEAVY_DEBUG
#ifdef HTTS_DEBUG
		htts_warn("TextToList::output() - outputting delayed FLUSH.");
#endif
#endif
	}

	//No se puede devolver audio y FLUSH a la vez, primero uno y luego otro.
	if ( retval && (*flush) )	{
		*flush = FALSE;
		delayFlush = TRUE;
#ifdef HEAVY_DEBUG
#ifdef HTTS_DEBUG
		htts_warn("TextToList::output() - delaying FLUSH.");
#endif
#endif
	}

//	if (retval) if (retval->empty()) return NULL;
	return retval;
}

/**********************************************************/


/**********************************************************/
/* Interfaz de salida de texto del modulo de paso a lista.
Esta funcion sirve para enviar el ACK salida que
anteriormente se ha leido con output() (se informa que
realmente se acepta la utterance o el flush).
Si la salida era una utterance se borra definitivamente.
La lista que la contenia. Si era un flush, se decrementa
el contador de flushes.

Si no se utiliza este metodo, output() estara devolviendo
continuamente la misma salida en sucesivas llamadas. Ademas
la entrada queda bloqueda si existe una salida pendiente
de ser aceptada (se a obtenido una salida con output() pero
no se a aceptado con esta funcion). Tampoco se aceptan
nuevos flushes si existe una salida pendiente de ser
aceptada. */

/**********************************************************/

VOID TextToList::outack(VOID)
{

#ifdef HTTS_DEBUG
  if (!created) htts_error("TextToList::outack() - Still no create method call registered.");
#endif
  if (!created) return;

#ifdef HEAVY_DEBUG
#ifdef HTTS_DEBUG
  if (!outAckPend) htts_warn("TextToList::outact() - There is no outack pendding.");
	htts_warn("TextToList::outack()");
#endif
#endif

	if (!outAckPend) return;	// Si no hay ACK pendiente temina.
	outAckPend=FALSE;	// Borra el ACK, ya que lo acepta.
  if (outputUTTWS)  nt->clear();
  if (!outputUTTWS) if (nttxt) free(nttxt);


}

/**********************************************************/


/**********************************************************/

BOOL TextToList::set(const CHAR *param, const CHAR *val)
{

	BOOL retval = FALSE;

#ifdef HTTS_DEBUG
  if (!created) htts_error("TextToList::set() - Still no create method call registered.");
	if ((!*param)||(!*val)) {
		htts_warn("TextToList::set() - NULL param and/or val argument/s.");
		return FALSE;
	}
#endif

  if (!created) return FALSE;
/*
	if (!strcmp(param, "TxtMode")) {
		if (!strcmp(val, "Utt")) retval=TRUE;
		if (!strcmp(val, "Word")) retval=TRUE;
		if (!strcmp(val, "Spell")) retval=TRUE;
	}
*/
	if (!strcmp(param,"DefEmo")) {
		String temp;	//IÑAKI
		temp = val;	//IÑAKI
		emo[0] = indexEmo ((String &)temp); //IÑAKI
		//emo[0] = indexEmo ((String &)val); //IÑAKI
		indiceEmo = 0;
		return TRUE;
	}
	if (!strcmp(param,"DefIntEmo")) {
		emoInt [0] = atoi((const char*)val);
		return TRUE;
	}

  return retval;
}

/**********************************************************/


/**********************************************************/

const CHAR *TextToList::get(const CHAR *param)
{

#ifdef HTTS_DEBUG
  if (!created) htts_warn("TextToList::get() - Still no create method call registered.");
#endif
  if (!created) return NULL;

	if (!strcmp(param, "TxtMode")) return (const CHAR *)"Utt";

#ifdef HTTS_DEBUG
  htts_warn("TextToList::get() - Unknown parameter [%s].", param);
#endif

  return FALSE;
}

/**********************************************************/
