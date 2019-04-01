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

Nombre fuente................ wsdump.cpp
Nombre paquete............... normalizador
Lenguaje fuente.............. C++
Estado....................... desarrollo
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Juan Luis Murugarren

Version   dd/mm/aa  Autor     Proposito de la edicion
-------   --------  --------  -----------------------
0.0.15	  02/10/07  Inaki     Corregir posicion de signos de puntuacion en frases con " '
0.0.14    09/07/01  Yon2.     Don�t return empty sentences.
0.0.13    21/06/01  Yon2.     Pause Scape Seq added.
0.0.12    16/06/01  Yon2.     Scape Seq support.
0.0.11    04/05/01  Yon2.     Right Warning/Error info.
0.0.10    24/04/01  Yon2.     Multiling support.
0.0.09    08/04/01  Yon2.     Avoid Warnings.
0.0.08    28/03/01  Yon2.     Data Need.
0.0.07    04/02/01  Yon2.     Bug: If LastGrpProc Lix deleted? i.e. output
0.0.06    16/01/01  Yon2.     Text output support.
0.0.05    15/01/01  Yon2.     Proccess last group? Depends on completion.
0.0.04    15/01/01  Yon2.     If no Dic available no HDicRef.
0.0.03    09/01/01  Yon2.     Better Utt deletion.
0.0.02    08/01/01  Yon2.     Std Module I/O.
0.0.01    01/01/01  Yon2.     Quasi-completado.
0.0.00    26/12/00  Yon2.     Codificacion inicial.

======================== Contenido ========================

  Este m�dulo vuelca la lista en formato "texto crudo" ya
normalizado a la lista est�ndar del normalizador.
  De la lista est�ndar se rellenan los campos:
- Tipo de Sentencia.
- Palabra.
- HDicRef
- Phrase
- Tipo de normalizaci�n.

El campo Phrase junto con el de "tipo de sentencia" nos sit�an
dentro de los par�ntesis.
Para poder distingrir fronteras en "tipo de normalizaci�n" las
palabras que no hallan sufrido ninguna alteraci�n en el
proceso de normalizaci�n se han recogido con tipo de normalizaci�n
UTYPENOR_NORMAL. (Esto deber�a ir tambi�n en normal.cpp).

Las palabras que se pasan a la lista de texto normalizado se
convierten siempre a letras min�sculas.


Yon2.

===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include "tdef.h"
#include "uttws.hpp"

#include "httsmsg.h"
#include "t2l.hpp"
#include "wsdump.hpp"
#include "chset.h"

#include "string.h"

/**********************************************************/

BOOL TextToList::isPuntt(CtI p)
{
  INT patlen;

  if (!ct(p).pattern) return FALSE;
  patlen = strlen(ct(p).pattern);

  if (((ct(p).pattern)[0] == 'p') && (patlen == 1)) return TRUE;


	return FALSE;
}

/**********************************************************/


/**********************************************************/

BOOL TextToList::hasOpenedPar(CtI p)
{
  BOOL retval = FALSE;

	if (strstr(ct(p).str, "(")) retval = TRUE;

  return retval;
}

/**********************************************************/


/**********************************************************/

BOOL TextToList::hasClosedPar(CtI p)
{
  BOOL retval = FALSE;

	if (strstr(ct(p).str, ")")) retval = TRUE;

  return retval;
}

/**********************************************************/


/**********************************************************/

Sentence TextToList::infSent(CtI p)
{
  pCHAR temp;
  Sentence retval = USENTENCE_NONE;

  temp = strdup(ct(p).str);

  //Son m�s prioritarios los que m�s tarde se comparan.
	if (strstr(temp, "?")) retval = USENTENCE_QUEST;
	if (strstr(temp, "!")) retval = USENTENCE_EXCL;
	if (strstr(temp, "-")) retval = USENTENCE_NEUTR;
	if (strstr(temp, ":")) retval = USENTENCE_COLON;
	if (strstr(temp, ";")) retval = USENTENCE_SCOLON;
	if (strstr(temp, "(")) retval = USENTENCE_PAROPEN;
	if (strstr(temp, ")")) retval = USENTENCE_PAR;
	if (strstr(temp, ",")) retval = USENTENCE_PAUSE;
	if (strstr(temp, ".")) retval = USENTENCE_DECL;
	if (strstr(temp, "...")) retval = USENTENCE_SUSPEN;


  free(temp);

  return retval;
}

/**********************************************************/


/**********************************************************/
//En un principio volcaba toda la lista.
//Ahora solo debe volcar una utterance.
UttWS *TextToList::ct2ws(BOOL *flush)
{
	CtI p, r, lastToNoP;
	INT i = 0;
	UttI q = NULL;
	UttI sent = NULL;
	BOOL sentTypePend = TRUE;
	BOOL ntCellUsed = TRUE;
	BOOL insidePar = FALSE;
	BOOL endParse = FALSE;
	BOOL flushLocated = FALSE;
	BOOL pausa = FALSE; //inaki
	if (procLastGrp) lastToNoP = NULL;
	else lastToNoP = ct.lastGrp();

	for (p = ct.first(); !((p == lastToNoP) || endParse); p = ct.next(p)) {
		if (ntCellUsed) {
			q = nt->cellAppend();
			ntCellUsed = FALSE;
		}

		if (sentTypePend) 
		{
			sent = q;
			sentTypePend = FALSE;
		}

		if (isPuntt(p)) 
		{
			if (hasOpenedPar(p)) insidePar = TRUE;
			if (hasClosedPar(p)) insidePar = FALSE;
			//***********************************************************
			//INAKI para que los CHTYPE_PUNTT (eu_wrdch.cpp) que NO
			//consideramos como tipo de frase: " ' « »
			//no interfieran en la colocacion correcta del resto de signos
			switch((ct(p).str)[0]){
				case '"':
				case '\'':
				case '«':
				case '»':
					continue;
					break;
				default:
					break;
			}
			//****************************************************

			nt->cell(sent).setSentence(infSent(p));
			if (!insidePar) nt->cell(sent).setPhrase(TRUE);
			sentTypePend = TRUE;
			switch (infSent(p)) {
				case USENTENCE_QUEST:
				case USENTENCE_EXCL:
				case USENTENCE_COLON:
				case USENTENCE_SCOLON:
				case USENTENCE_DECL:
				case USENTENCE_SUSPEN:
					endParse = TRUE;
					break;
			/*	case USENTENCE_PAUSE:  //inaki
					endParse = FALSE;
					pausa = TRUE;*/
				default:
					endParse = FALSE;
					break;
			}
		}
		else	
		{
			if (ct.isEscSeq(p)) 
			{
				INT value;
				htts_warn("WSdump: Located scape seq");
				switch(analizeSeq(ct, p, &value))	
				{
					case SEQ_PAUSE:
#ifdef DEBUG_SCA
						htts_warn("WSdump: PAUSE scape sequence located.");
#endif
						insertPause(nt, q, value);

						break;
					default:
						htts_warn("WSdump: UNKNOWN scape sequence located.");
						break;
				}

				p=ct.lastInGrp(p);
			}
			else	
			{
				
				
				pCHAR tempStr;
				tempStr = strdup(ct.getStr(p));

				if (strlen(tempStr) != 0)
				{
					ISOLatin1_StrLower(tempStr);
					nt->cell(q).setWord(tempStr);
					//free(tempStr);
					
					//Rellenamos los campos de emoci�n
					nt->cell(q).setEmotion (ct.getEmotion (p));
					nt->cell(q).setEmoInt (ct.getEmo_int (p));
					#ifdef HTTS_PROSO_VAL
					nt->cell(q).setProso_val_break(ct.getProso_val_break(p));//Aritz
					nt->cell(q).setProso_val_emphasis(ct.getProso_val_emphasis(p));//Aritz
					nt->cell(q).setProso_val_pitch(ct.getProso_val_pitch(p));//Aritz
					nt->cell(q).setProso_val_range(ct.getProso_val_range(p));//Aritz
					nt->cell(q).setProso_val_rate(ct.getProso_val_rate(p));//Aritz
					nt->cell(q).setProso_val_volume(ct.getProso_val_volume(p));//Aritz*/
					#endif
					//Solo si se carg� dicionario rellenamos el HDicRef.
					if (dicCreated) nt->cell(q).setHDicRef(dic->search(ct.getStr(p)));
					if (ct.isStart(p) && ct.isNorm(p)) nt->cell(q).setTNor(ct.getTnor(p));
					ntCellUsed = TRUE;
				}
				free(tempStr);
			}
			//aki
			/*if(p == ct.last() && endParse == FALSE && pausa == TRUE){
				nt->cell(sent).setSentence(infSent(p));
				if (!insidePar) nt->cell(sent).setPhrase(TRUE);
				sentTypePend = TRUE;
			}*/
		}

		//Fin de frase por longitud excesiva "y ausencia de puntuaci�n".
		if ((i++ >= CRUDE_LIST_MAX_LEN) && !flushLocated)
		if (ct.next(p)) if (ct.isStart(ct.next(p))) endParse = TRUE;

		//Grupo �ltimo proviniente de un flush?
		if (ct.isFlush(p)) 
		{
			flushLocated = TRUE;
			*flush = TRUE;
		}

		//Fin de frase por flush de entrada localizado.
		if (flushLocated)
			if (ct.next(p)) if (ct.isStart(ct.next(p))) endParse = TRUE;
	}
	if (!ntCellUsed) nt->cellDel(q);

	//Esto mejor borrar grupos que celdas. Pero est� bien.
	for (r = ct.first(); r != p; r = ct.del(r))
	if (r == lastGrpProc) lastGrpProc = NULL;

	if (ct.empty()) {
		lastGrpProc = NULL;
		procLastGrp = FALSE;
	}

	if (!(nt->empty())) outAckPend=TRUE;	// A espera de confirmar la salida.


	return nt->empty()?NULL:nt;

}

/**********************************************************/


/**********************************************************/
//Une dos cadenas en una sola.
//libera ademas la cadena "to"
//devuelve la nueva "to" a la que se le a�adir�n m�s caracteres
//si procede.
//el que llama a esta funci�n es el encargado de liberar esta cadena.
pCHAR TextToList::concatStr(pCHAR to, pCHAR from)
{
  INT len;
  pCHAR temp;

#ifdef HTTS_DEBUG
  if (!to) htts_error("TextToList::concatStr() - Null \"to\" String");
	if (!from) htts_error("TextToList::concatStr() - Null \"from\" String");
#endif

  len = strlen(to) + strlen(from) + 1; // El +1 para el 0 de fin de cadena.
  temp = (pCHAR)malloc(len * sizeof(CHAR));

  strcpy(temp, to);
  strcat(temp, from);

  free(to);

  return temp;
}

/**********************************************************/


/**********************************************************/

BOOL TextToList::mustInsertSpace(CtI p)
{
  BOOL retval = TRUE;
  CtI q;


#ifdef HTTS_DEBUG
  if (!p) htts_warn("TextToList::mustInsertSpace() - Null CtI");
#endif

  if (!p) return retval;

//Si el actual es un "left fragment" nada de espacio.
  if (ct.isStart(p)) if (ct.isLeftFrg(p)) retval = FALSE;

//Si el siguiente es un "rigth fragment" nada de espacio.
  q = ct.next(p);
  if (q) if (ct.isStart(q)) if (ct.isRightFrg(q)) retval = FALSE;

  return retval;
}

/**********************************************************/


/**********************************************************/
//Versi�n de ct2ws con salida a texto.
pCHAR TextToList::ct2txt(BOOL *flush)
{
  CtI p, r, lastToNoP;
  INT i = 0;
  BOOL endParse = FALSE;
  BOOL flushLocated = FALSE;
  pCHAR outStr=(char *)"";

  if (procLastGrp) lastToNoP = NULL;
  else lastToNoP = ct.lastGrp();

  nttxt = strdup(outStr);

  for (p = ct.first(); !((p == lastToNoP) || endParse); p = ct.next(p)) {
      nttxt = concatStr(nttxt, ct.getStr(p));
      if (mustInsertSpace(p)) nttxt = concatStr(nttxt, (char *)" ");

      if (isPuntt(p)) {
       switch (infSent(p)) {
         case USENTENCE_QUEST:
         case USENTENCE_EXCL:
         case USENTENCE_COLON:
         case USENTENCE_SCOLON:
         case USENTENCE_DECL:
         case USENTENCE_SUSPEN:
              endParse = TRUE;
              break;
         default:
              endParse = FALSE;
              break;
       }
      }

      //Fin de frase por longitud excesiva "y ausencia de puntuaci�n".
      if ((i++ >= CRUDE_LIST_MAX_LEN) && !flushLocated)
         if (ct.next(p)) if (ct.isStart(ct.next(p))) endParse = TRUE;

      //Grupo �ltimo proviniente de un flush?
      if (ct.isFlush(p)) {
       flushLocated = TRUE;
       *flush = TRUE;
      }

      //Fin de frase por flush de entrada localizado.
      if (flushLocated)
       if (ct.next(p)) if (ct.isStart(ct.next(p))) endParse = TRUE;

  }

  //Esto mejor borrar grupos que celdas. Pero est� bien.
  for (r = ct.first(); r != p; r = ct.del(r))
     if (r == lastGrpProc) lastGrpProc = NULL;

  if (ct.empty()) {
     lastGrpProc = NULL;
     procLastGrp = FALSE;
  }

	outAckPend=TRUE;	// A espera de confirmar la salida.


  return nttxt;

}

/**********************************************************/


/**********************************************************/

BOOL TextToList::isThereAnUtt( VOID )
{
  BOOL retval = FALSE;
  BOOL endParse = FALSE;
  CtI p, lastToNoP;

	if (dataNeed) return retval;

  if (procLastGrp) lastToNoP = NULL;
  else lastToNoP = ct.lastGrp();

  for (p = ct.firstGrp(); !((p == lastToNoP) || endParse); p = ct.nextGrp(p)) {
     //Vemos si hay una frase hecha y derecha, con puntuaci�n y todo!!

     if (isPuntt(p))
       switch (infSent(p)) {
         case USENTENCE_QUEST:
         case USENTENCE_EXCL:
         case USENTENCE_COLON:
         case USENTENCE_SCOLON:
         case USENTENCE_DECL:
         case USENTENCE_SUSPEN:
              endParse = TRUE;
              retval = TRUE;
              break;
         default:
              endParse = FALSE;
              break;
       }

     //Frase debida a ausencia de puntuaci�n y longitud excesiva.
     if (ct.index(p) >= CRUDE_LIST_MAX_LEN) {
       retval = TRUE;
       endParse = TRUE;
     }

     //Frase por flush en la entrada.
     if (ct.isFlush(p)) {
       retval = TRUE;
       endParse = TRUE;
     }

  }

  return retval;
}

/**********************************************************/


/**********************************************************/

UttWS *TextToList::wsoutput(BOOL *flush, INT mode, VOID *param)
{
  UttWS *retval = NULL;

  if (isThereAnUtt()) retval = ct2ws(flush);
  if (ctFull) if (ct.index(ct.last()) < CRUDE_LIST_MAX_LEN)
    ctFull = FALSE;
  return retval;
}

/**********************************************************/


/**********************************************************/

pCHAR TextToList::wsoutputtext(BOOL *flush, INT mode, VOID *param)
{
  pCHAR retval = NULL;

  if (isThereAnUtt()) retval = ct2txt(flush);
  if (ctFull) if (ct.index(ct.last()) < CRUDE_LIST_MAX_LEN)
    ctFull = FALSE;
  return retval;
}

/**********************************************************/

