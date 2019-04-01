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
#ifndef __UTTPH_HPP__
#define __UTTPH_HPP__

/**********************************************************/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/*
(C) 1997 TSR/Aholab - ETSII/IT Bilbao (UPV/EHU)

Nombre fuente................ uttph.hpp
Nombre paquete............... aHoTTS
Lenguaje fuente.............. C++
Estado....................... -
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Borja Etxebarria
.............................

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
1.1.0 	 12/07/10  Inaki	 Añadir celdas para modificar la prosodia desde el texto (proyecto Aritz) HTTS_PROSO_VAL
1.0.1    13/05/00  borja     update ULEVEL, docs
1.0.0    31/01/00  borja     codefreeze aHoTTS v1.0
0.0.0    24/11/97  borja     Codificacion inicial.

======================== Contenido ========================
<DOC>
</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include "uttws.hpp"
#include "phone.h"

/**********************************************************/
/* diferentes niveles a los que se puede trabajar: celda,
fonema, silaba, palabra, frase, sentencia, todo (utterance), etc.
Tienen que ser compatibles con otros bits definidos en otros
ficheros derivados de {{utt}} (utt, uttws, etc)  */

enum {
	ULEVEL_SYLLABLE =0x0100,
	ULEVEL_CHAR     =0x0200,
	ULEVEL_PHONE    =0x0400,
	ULEVEL_EMPHASIS =0x0800,

/* ahora vienen los rangos jerarquizados, en los que
el comienzo de una palabra implica comienzo de silaba,
el comienzo de una frase implica comienzo de palabra, etc etc. */
	URANGE_SYLLABLE = URANGE_WORD | ULEVEL_SYLLABLE,
	URANGE_CHAR = URANGE_SYLLABLE | ULEVEL_CHAR,
	URANGE_PHONE = URANGE_SYLLABLE | ULEVEL_PHONE,
};

/**********************************************************/
/*La clase UttPh maneja practicamente todo lo necesario
para la sintesis hasta nivel de fonemas: fonema, ch, silaba,
palabras, frases(sintagma) y sentencias, y dos grupos 
genericos (group1 y group2).
Se incluye ademas:
Stress: bits para indicar el acento.
Emphasis: bits para indicar punto de enfasis (acento prosodico...)
El codigo de la clase chequea que los campos de bits tengan bits
suficientes para representar los valores que se intenten salvar,
pero solo si HTTS_DEBUG esta definido.

Los campos Phone, Char, Stress y Emphasis,  *no* se transladan
automaticamente cuando se borra una celda! tampoco los de
prosodia (pitch duracion energia) */

/* tipos de acento que tenemos por ahora. TEXT es el ortografico,
AUTO el que asignado automaticamente */
enum Stress {
	USTRESS_NONE=' ', USTRESS_TEXT='\'', USTRESS_AUTO='`'
};

/* tipos de emfasis: por ahora solo el tipo STRESS, debido al acento
propio de las palabras (las palabras atonas tendran Stress en alguna
vocal, pero no les ponemos emfasis). STRONG no se usa, pudiera
servir para indicar enfasis mas marcados en puntos criticos (o
respondiendo a codigo <EMPH> de SSML...) */
enum Emphasis {
	UEMPHASIS_NONE=' ', UEMPHASIS_STRESS='^', UEMPHASIS_STRONG='*'
};

class UttCellPh: public UttCellWS {
public:
	BOOL isStartOf( UttLevel range ) const;
private:
	BOOL syllable : 1; // frontera de silaba.
	BOOL group1 : 1;   // frontera de grupo1.
	BOOL group2 : 1;   // frontera de grupo2.
	Stress stress;     // acento.
	Emphasis emph;     // punto de enfasis (acento prosodico).
	CHAR ch;           // codigo caracter escrito
	Phone ph;          // codigo fonema (SAMPA, en phones.h)
	DOUBLE dur;        // duracion
	DOUBLE pow;        // factor potencia
#ifdef HTTS_MULTIPITCH
	INT npth;           // numero puntos de pitch
	DOUBLE *pth;        // parejas posicion/pitch
#else
	DOUBLE pth;        // factor pitch
#endif

public:
	UttCellPh ( VOID );
	virtual ~UttCellPh ( VOID );
	VOID setPhone( Phone code ) { ph = code; }
	VOID setChar( CHAR code ) { ch = code; }
	VOID setSyllable( BOOL code ) {	syllable = (code != FALSE); }
	VOID setGroup1( BOOL code ) {	group1 = (code != FALSE); }
	VOID setGroup2( BOOL code ) { group2 = (code != FALSE); }
	VOID setStress( Stress code );
	VOID setEmphasis( Emphasis code );
	VOID setDur( DOUBLE val ) { dur = val; }
	VOID setPow( DOUBLE val ) { pow = val; }
#ifdef HTTS_MULTIPITCH
	VOID setPth( DOUBLE val ) { set_Pth(0,val); set_PthX(0,0.5); }
#else
	VOID setPth( DOUBLE val ) { pth = val; }
#endif
	Phone getPhone( VOID ) const { return ph; }
	CHAR getChar( VOID ) const { return ch; }
	BOOL getSyllable( VOID ) const { return syllable; }
	BOOL getGroup1( VOID ) const { return group1; }
	BOOL getGroup2( VOID ) const { return group2; }
	Stress getStress( VOID ) const { return stress; }
	Emphasis getEmphasis( VOID ) const { return emph; }
	DOUBLE getDur( VOID ) const { return dur; }
	DOUBLE getPow( VOID ) const { return pow; }
#ifdef HTTS_MULTIPITCH
	DOUBLE getPth( VOID ) const { return get_Pth(0); }
#else
	DOUBLE getPth( VOID ) const { return pth; }
#endif

#ifdef HTTS_MULTIPITCH
	VOID set_PthN( INT n );
	VOID set_PthX( INT i, DOUBLE x ) { assert(i>=0); assert(i<npth); pth[i*2+1]=x; }
	VOID set_Pth( INT i, DOUBLE val ) { assert(i>=0); assert(i<npth); pth[i*2]=val; }
	INT get_PthN( VOID ) const { return npth; }
	DOUBLE get_PthX( INT i ) const { assert(i>=0); assert(i<npth); return pth[i*2+1]; }
	DOUBLE get_Pth( INT i ) const { assert(i>=0); assert(i<npth); return pth[i*2]; }
#else
	VOID set_PthN( INT n ) { assert(n==1); }
	VOID set_PthX( INT i, DOUBLE x ) { assert(!i); (void)x; }
	VOID set_Pth( INT i, DOUBLE val ) { assert(!i); setPth(val); }
	INT get_PthN( VOID ) const { return 1; }
	DOUBLE get_PthX( INT i ) const { assert(!i); return 0.5; }
	DOUBLE get_Pth( INT i ) const { assert(!i); return getPth(); }
#endif

	VOID set_PowN( INT n ) { assert(n==1); }
	VOID set_PowX( INT i, DOUBLE x ) { assert(!i); (void)x; }
	VOID set_Pow( INT i, DOUBLE val ) { assert(!i); setPow(val); }
	INT get_PowN( VOID ) { return 1; }
	DOUBLE get_PowX( INT i ) { assert(!i); return 0.5; }
	DOUBLE get_Pow( INT i ) { assert(!i); return getPow(); }

 public:
#ifdef HTTS_IOTXT
	virtual VOID __foutput(FILE *f) const;
	virtual CHAR *__finput( CHAR *buf );
#endif

	virtual VOID reset( VOID );

	KINDOF_DECL();
};

/**********************************************************/

class UttPh : public UttWS {
protected:
	// al crear nuevas celdas, que sean del tipo adecuado (Ph)
	UttCell * newCell( VOID );
	// Al borrar celdas hay que preservas el comienzo de silabas, etc
	#ifndef HTTS_PROSO_VAL
	UttI deleteCell( UttI p );
	#endif

public:
	// Al borrar celdas hay que preservas el comienzo de silabas, etc
	#ifdef HTTS_PROSO_VAL
	UttI deleteCell( UttI p );//Aritz
	#endif
	// Cast automatico de las celdas al tipo adecuado
	UttCellPh& cell( UttI p ) const { assert(Utt::cell(p).isKindOf("UttCellPh")); return (UttCellPh&)(Utt::cell(p)); }
	UttCellPh& operator()( UttI p ) const { return cell(p); }

	UTT_LEVELSHORTCUTS(char,ULEVEL_CHAR);
	UTT_LEVELSHORTCUTS(phone,ULEVEL_PHONE);
	UTT_LEVELSHORTCUTS(syllable,ULEVEL_SYLLABLE);
	UTT_LEVELSHORTCUTS(emphasis,ULEVEL_EMPHASIS);
	#ifdef HTTS_PROSO_VAL
	INT getProso_val_pitch(UttI p);//Aritz
	#endif
public:
	VOID word2Char( UttI wordp ); // este metodo expande una palabra

	// algunas funciones tipicas
	BOOL syllableIsFirstOfWord( UttI p ) const { return syllableIsFirst(p,URANGE_WORD); }
	BOOL syllableIsLastOfWord( UttI p ) const { return syllableIsLast(p,URANGE_WORD); }
	BOOL phoneIsFirstOfWord( UttI p ) const { return phoneIsFirst(p,URANGE_WORD); }
	BOOL phoneIsLastOfWord( UttI p ) const { return phoneIsLast(p,URANGE_WORD); }

	KINDOF_DECL();
};

/**********************************************************/

#endif

