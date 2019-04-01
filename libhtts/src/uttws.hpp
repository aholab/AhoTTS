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
#ifndef __UTTWS_HPP__
#define __UTTWS_HPP__

/**********************************************************/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/*
(C) 1997 TSR/Aholab - ETSII/IT Bilbao (UPV/EHU)

Nombre fuente................ uttws.hpp
Nombre paquete............... aHoTTS
Lenguaje fuente.............. C++
Estado....................... -
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Borja Etxebarria
.............................

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
1.0.5	 12/07/10  Inaki	 Añadir celdas para modificar la prosodia desde el texto (proyecto Aritz) HTTS_PROSO_VAL
1.0.4    xx/12/00  Yon2.     A¤adir nuevos tipos de sentencia.
1.0.3    28/09/00  Yon2.     A¤adir nuevo campo TYPENOR,
															cambio orden de campos.
1.0.2    13/05/00  borja     poner campos AGrp, FGrp
1.0.1    08/05/00  borja     activar POS
1.0.0    31/01/00  borja     codefreeze aHoTTS v1.0
0.0.0    24/11/97  borja     Codificacion inicial.

======================== Contenido ========================
<DOC>
</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include "utt.hpp"
#include "hdic.hpp"

/**********************************************************/
/* diferentes niveles a los que se puede trabajar: celda,
fonema, silaba, palabra, frase, sentencia, todo (utterance), etc.
Son potencias de dos!!!, para poder enviar OR de varios de ellos
en un rango de busqueda (ver URANGE_???). Tienen que ser
compatibles con otros bits definidos en otros ficheros derivados
de {{utt}} (utt, uttph, etc)  */

enum {
	ULEVEL_SENTENCE =0x0002,
	ULEVEL_PHRASE   =0x0004,
	ULEVEL_WORD     =0x0008,
	ULEVEL_PAUSE    =0x0010,
	ULEVEL_AGRP     =0x0020,
	ULEVEL_FGRP     =0x0040,
	ULEVEL_TNOR     =0x0080,

/* ahora vienen los rangos jerarquizados, en los que
el comienzo de una palabra implica comienzo de silaba,
el comienzo de una frase implica comienzo de palabra, etc etc. */
	URANGE_SENTENCE = URANGE_UTT | ULEVEL_SENTENCE,
	URANGE_PHRASE = URANGE_SENTENCE | ULEVEL_PHRASE,
	URANGE_WORD = URANGE_PHRASE | ULEVEL_WORD,
	URANGE_PAUSE = ULEVEL_PAUSE,
	URANGE_AGRP = ULEVEL_AGRP,
	URANGE_TNOR = ULEVEL_TNOR,
	URANGE_FGRP = ULEVEL_FGRP
};

/**********************************************************/

/* tipos de sentences: declarativa, interrogativa, exclamativa,
pausa */
enum Sentence {
	USENTENCE_NONE=' ', USENTENCE_DECL='.', USENTENCE_QUEST='?',
	USENTENCE_EXCL='!', USENTENCE_PAUSE=',', USENTENCE_NEUTR='-',
  USENTENCE_COLON=':', USENTENCE_SCOLON=';', USENTENCE_SUSPEN='=',
  USENTENCE_PAR=')', USENTENCE_PAROPEN='('
};

#define USENTENCE_ALLCODES " .?!,-:;=()"

// tipo de sentencia por defecto cuando no hay ninguno
#define UTT_DEF_SENTENCE USENTENCE_NEUTR

/* tipos de pausas.
Estos mismos pero con signo negativo se usan para indicar
que hay un breath (respiracion audible). Por eso UPAUSE_NONE
usamos un 0 en vez de un blanco ' '. */
typedef INT Pause;
enum _Pause {

	UPAUSE_NONE=0,
	UPAUSE_UBEGIN='i', /* inicio de utterance */
	UPAUSE_UEND='f', /* fin de utterance */

	UPAUSE_SHORT='o',
	UPAUSE_LONG='l',

	/* pausas de sentencias (tienes que estar definidos
	todos los tipos de sentencias) y usar los MISMOS codigos */
	UPAUSE_SDECL='.',
	UPAUSE_SQUEST='?',
	UPAUSE_SEXCL='!',
	UPAUSE_SPAUSE=',',
	UPAUSE_SNEUTR='-',
  UPAUSE_COLON=':',
  UPAUSE_SCOLON=';',
  UPAUSE_SUSPEN='=',
  UPAUSE_PAR=')',
  UPAUSE_PAROPEN='('

	/* no usar el codigo ntilde (~) porque se usa en IOTXT para indicar
	signo negativo (breath)! */
};

#define UPAUSE_ALLCODES "ifol.?!,-:;=()"


/* tipos de emoción. */
	typedef INT Emotion;
	enum _Emotion {

	UEMOTION_DEFAULT=0,
	UEMOTION_NEUTRAL=1,
	UEMOTION_AFRAID=2,
	UEMOTION_ANGRY=3,
	UEMOTION_CONFUSED=4,		//Sin implementar
	UEMOTION_DAZED=5,			//Sin implementar
	UEMOTION_DISGUSTED=6,	 	//Sin implementar
	UEMOTION_HAPPY=7,
	UEMOTION_SAD=8,
	UEMOTION_SURPRISED=9

};
/*Tipos de etiquetas de prosodia*/
#ifdef HTTS_PROSO_VAL
typedef INT Prosodya;
enum _Prosodya {//Aritz

	UDEFAULT=0,
	UEMPHASIS=10,//
	UPROSODY_PITCH=11,//
	UPROSODY_RANGE=12,//
	UPROSODY_RATE=13,//
	UPROSODY_VOLUME=14,//
	UBREAK=15//
};
#endif
#define MIN_EMOTION 0
#define MAX_EMOTION 9
#define MIN_INTENSITY 0
#define MAX_INTENSITY 100
#ifdef HTTS_PROSO_VAL
#define MIN_VAL 0//Aritz
#define MAX_VAL 100//Aritz
#define MAX_VAL_BREAK 300//Aritz
#define MIN_VAL_BREAK 100 //Aritz
#define MIN_VAL_PITCH -200//Aritz
#define MAX_VAL_PITCH 300//Aritz
#define MIN_VAL_EMPHASIS -20
#define MAX_VAL_EMPHASIS 20
#endif
//Definiciones de nombres de emociones
#define EMO_DEFAULT_STR "DEFAULT"
#define EMO_NEUTRAL_STR "NEUTRAL"
#define EMO_AFRAID_STR "AFRAID"
#define EMO_ANGRY_STR "ANGRY"
#define EMO_CONFUSED_STR "CONFUSED"
#define EMO_DAZED_STR "DAZED"
#define EMO_DISGUSTED_STR "DISGUSTED"
#define EMO_HAPPY_STR "HAPPY"
#define EMO_SAD_STR "SAD"
#define EMO_SURPRISED_STR "SURPRISED"

#ifdef HTTS_PROSO_VAL
#define PROS_EMPHASIS       "EMPHASIS"	//Aritz
#define PROS_PROSODY_PITCH  "PROSODY PITCH"	//
#define PROS_PROSODY_RANGE	"PROSODY RANGE"	//
#define PROS_PROSODY_RATE	"PROSODY RATE"//
#define PROS_PROSODY_VOLUME	"PROSODY VOLUME" //
#define PROS_BREAK			"BREAK"//
#endif
#ifdef HTTS_IOTXT
// anchura por defecto para imprimir las palabras (por foutput())
#define UTT_DEF_WORD_WIDTH 8
#endif

class UttWS;

/**********************************************************/
/* La clase UttWS maneja palabras concretas divididas en
sentencias. Las propias variables internas {sentence}
y {word} que almacenan el tipo de frase y la palabra, se
utilizan como indicadores de frontera.
Se incluye ademas campos WStatus (estado de la palabra:
si implica castellano estando en euskera, si esta hecha
la acentuacion, etc) y POS (part of speech: sustantivo,
verbo, etc). Los dos campos WStatus y POS se transladan
automaticamente de celda junto con la palabra, cuando
se borra la primera celda de la palabra.
El codigo de la clase chequea que los campos
de bits WStatus y POS tengan bits suficientes para representar
los valores que se intenten salvar, pero solo si HTTS_DEBUG
esta definido. */

class UttCellWS: public UttCell {
public:
	virtual BOOL isStartOf( UttLevel range ) const;
private:
	Sentence sentence;  // tipo de sentence
	BOOL phrase : 1;   // frontera de sintagma.
	Pause pause;  // tipo de pausa
	CHAR *word;  // cadena con la palabra

	Emotion emotion;
	INT emo_intensity;

	/* {wstatus} por ahora no se usan, asi que eliminamos todo
	lo referente a ellas al no estar definido UTT_DUMMY */
#ifdef UTT_DUMMY
	INT wstatus;  // estado actual de la palabra
#endif
	INT pos,pos1,pos2;  // pos: part of speech
	INT agrp;  // grupo acentual
	INT tnor;  // tipo de normalizaci¢n
	INT fgrp;  // grupo funcional

	HDicRef dref; // referencia a diccionario
#ifdef HTTS_TIMEEVS
	INT timeev;  // codigo evento
#endif
public:
	#ifdef HTTS_PROSO_VAL
	INT val_break;//Aritz
	INT val_emphasis;//Aritz
	INT val_pitch;//Aritz
	INT val_range;//Aritz
	INT val_rate;//Aritz
	INT val_volume;//Aritz
	#endif

	UttCellWS ( VOID );
	virtual ~UttCellWS ( VOID );
	VOID setSentence( Sentence code ) { sentence=code; }
	VOID setPause( Pause code ) { pause=code; }
	VOID setWord( const CHAR * code );
	VOID setPhrase( BOOL code ) { phrase = (code != FALSE); }
	#ifdef HTTS_PROSO_VAL
	VOID setProso_val_break( int code);//Aritz
	VOID setProso_val_emphasis( int code);//Aritz
	VOID setProso_val_pitch( int code);//Aritz
	VOID setProso_val_range( int code);//Aritz
	VOID setProso_val_rate( int code);//Aritz
	VOID setProso_val_volume( int code);//Aritz*/
	#endif
	VOID setEmotion ( int code) { emotion = code;}
	VOID setEmoInt ( int code) { emo_intensity = code;}

#ifdef UTT_DUMMY
	VOID setWStatus( INT status );
#endif
	VOID setPOS( INT pos );
	VOID addPOS( INT pos2 );
	VOID subPOS( INT pos2 );
	INT queryPOS(INT pos2);
	VOID setAGrp( INT agrp );
	VOID setTNor( INT tnor );
	VOID setFGrp( INT fgrp );
	VOID setHDicRef( HDicRef r ) { dref=r; }
#ifdef HTTS_TIMEEVS
	VOID setTimeEv( INT ev ) { timeev=ev; }
#endif
	Sentence getSentence( VOID ) const { return sentence; }
	Pause getPause( VOID ) const { return pause; }
	const CHAR * getWord( VOID ) const { return word; }
	BOOL getPhrase( VOID ) const { return phrase; }
#ifdef UTT_DUMMY
	 INT getWStatus( VOID ) const { return wstatus; }
#endif
	 INT getPOS( VOID ) const { return pos; }
	 INT getAGrp( VOID ) const { return agrp; }
	 INT getTNor( VOID ) const { return tnor; }
	 INT getFGrp( VOID ) const { return fgrp; }
	 HDicRef getHDicRef( VOID ) const { return dref; }
#ifdef HTTS_TIMEEVS
	 INT getTimeEv( VOID ) const { return timeev; }
#endif

	INT getEmotion ( VOID) const {return emotion;}
	INT getEmoInt ( VOID ) const {return emo_intensity;}
	#ifdef HTTS_PROSO_VAL
	INT getProso_val_break(VOID) const;//Aritz
	INT getProso_val_emphasis(VOID) const;//Aritz
	INT getProso_val_pitch(VOID) const;//Aritz
	INT getProso_val_range(VOID) const;//Aritz
	INT getProso_val_rate(VOID) const;//Aritz
	INT getProso_val_volume(VOID) const;//Aritz
	#endif

	 virtual VOID reset( VOID );

protected:
	 // asignacion directa al campo word. ver nota1!!
	 VOID setWord_ptr( CHAR * code_ptr ) { word=code_ptr; }

	 friend class UttWS;

public:
#ifdef HTTS_IOTXT
	virtual VOID __foutput(FILE *f) const;
	virtual VOID foutput( FILE *f ) const;
	virtual CHAR *__finput( CHAR *buf );
	virtual BOOL finput( FILE *f );

	static VOID setGlobalHDicDB( HDicDB* dbase ) { db=dbase; }
protected:
	static HDicDB* db;

#endif

	KINDOF_DECL();
};

/**********************************************************/

class UttWS : public Utt {
protected:
	// al crear nuevas celdas, que sean del tipo adecuado (WS)
	virtual UttCell * newCell( VOID );
	// Al borrar celdas hay que preservas el comienzo de sentencias/palabras
	virtual UttI deleteCell( UttI p );


	HDicDB* dicdb;  // puntero a base de datos de palabras

public:
	UttWS( VOID );
	// Cast automatico de las celdas al tipo adecuado
	UttCellWS& cell( UttI p ) const { assert(Utt::cell(p).isKindOf("UttCellWS")); return (UttCellWS&)(Utt::cell(p)); }
	UttCellWS& operator ()( UttI p ) const { return cell(p); }

	/* IMPORTANTE: para que el importar a texto y exportar a texto
	lea/escriba tambien el HDicRef es necesario configurar una
	base de datos con esta funcion setHDicDB(). Si no, el HDicRef de
	las celdas estara vacio!!!! */
	VOID setHDicDB( HDicDB * db ) { dicdb=db; } // acceso a base de datos
	HDicDB *getHDicDB( VOID ) { return dicdb; }
#ifdef HTTS_TIMEEVS
	VOID expandTimeEvs( VOID );
#endif

	UTT_LEVELSHORTCUTS(phrase,ULEVEL_PHRASE);
	UTT_LEVELSHORTCUTS(word,ULEVEL_WORD);
	UTT_LEVELSHORTCUTS(sentence,ULEVEL_SENTENCE);
	UTT_LEVELSHORTCUTS(pausegr,ULEVEL_PAUSE);
	UTT_LEVELSHORTCUTS(fgrp,ULEVEL_FGRP);
	UTT_LEVELSHORTCUTS(agrp,ULEVEL_AGRP);
	UTT_LEVELSHORTCUTS(tnor,ULEVEL_TNOR);

	// algunas funciones tipicas
	BOOL wordIsFirstOfUtt( UttI p ) const { return wordIsFirst(p,URANGE_UTT); }
	BOOL wordIsLastOfUtt( UttI p ) const { return wordIsLast(p,URANGE_UTT); }
	// esta para movernos al comienzo de la ultima sentencia, lo normal
	// es que esta indique el tipo de toda la utterance (interrogativa, etc.)
	UttI lastSentenceBeginning( VOID ) const { return sentenceLast(); }

	KINDOF_DECL();

#ifdef HTTS_IOTXT
	virtual VOID foutput( FILE *f ) const;
	virtual BOOL finput( FILE *f );
#endif
};

/**********************************************************/
/*
nota1: setWord_ptr() no libera valor antiguo ni copia el
contenido del nuevo, se queda directamente con el puntero!
ojo, que al borrar celda se libera esta cadena (con xfree)!
Esta pensado por eficiencia, se usa para transpasar la palabra
de una celda a otra cuando se borra la primera celda de la
palabra.
---
*/
/**********************************************************/

#endif

