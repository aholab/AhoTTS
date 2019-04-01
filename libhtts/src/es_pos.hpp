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
#ifndef __ES_POS_HPP__
#define __ES_POS_HPP__

/**********************************************************/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/*
(C) 2004 TSR/Aholab - ETSII/IT Bilbao (UPV/EHU)

Nombre fuente................ es_pos.hpp
Nombre paquete............... aHoTTS
Lenguaje fuente.............. C++
Estado....................... -
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Inma Sardon
............................. inigos



======================== Contenido ========================
<DOC>
</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include "es_hdic.hpp"
#include "uttws.hpp"

#define MAX_TAM_WORD 100	//20 no es suficiente
typedef struct etiq{
			int pos1;
			int pos2;
			int pos3;
			int pos4;
			int pos5;
			int contador;
		}etiquetas;

typedef struct signo_posicion2{
		int pausa;
		int posicion;
		String signo;
	}signo_posic;


typedef struct estruc{
			String palab;
			bool pau;
			UttI puntero_celda;
		}palab_paus;
/**********************************************************/

class LangES_Categ {

	private:
		BOOL created;

	protected:
		String dbName;

	public:
		LangES_Categ( VOID );
		~LangES_Categ();
		BOOL create(VOID);

		BOOL set( const CHAR *param, const CHAR *val );
		const CHAR *get( const CHAR *param );

		VOID utt_categ(UttWS &ut);

	public:
		INT posdic(UttWS &u, UttI p, int i, etiquetas *pos);
		INT puede_ser_verbo(UttWS &u,UttI p,char *word_act,int i,etiquetas *pos);
		INT buscar_infinitivo(UttWS &u,UttI p, CHAR *word_act, int fin, int inicio, int longitud);
		INT es_xxeer(UttWS &u,UttI p);
		INT es_nli(UttWS &u,UttI p);
		INT es_xxponer(UttWS &u,UttI p);
		INT es_poner(UttWS &u,UttI p, CHAR *word_act);
		INT es_xxhacer(UttWS &u,UttI p);
		INT es_hacer(UttWS &u,UttI p, CHAR *word_act);
		INT es_xxvaler(UttWS &u,UttI p);
		INT es_valer(UttWS &u,UttI p, CHAR *word_act);
		INT es_xxtener(UttWS &u,UttI p);
		INT es_tener(UttWS &u,UttI p, CHAR *word_act);
		INT es_xxtraer(UttWS &u,UttI p);
		INT es_traer(UttWS &u,UttI p, CHAR *word_act);
		INT es_xxdecir(UttWS &u,UttI p);
		INT es_decir(UttWS &u,UttI p, CHAR *word_act);
		INT es_xxsalir(UttWS &u,UttI p);
		INT es_salir(UttWS &u,UttI p, CHAR *word_act);
		INT es_xxvenir(UttWS &u,UttI p);
		INT es_venir(UttWS &u,UttI p, CHAR *word_act);
		INT es_xxducir(UttWS &u,UttI p);
		INT es_verbo(UttWS &u,UttI p, CHAR *word_act, int i, etiquetas *pos);
		INT es_enclitico(UttWS &u,UttI p,int i, etiquetas *pos);
		INT es_no_personal(UttWS &u,UttI p);
		INT desambiguar(UttWS & u,etiquetas *pos);
		INT es_xxmente(UttWS &u,UttI p);
};


/**********************************************************/


class LangES_GF {
	private:
		BOOL created;
	protected:
		String dbName;

	public:
		LangES_GF( VOID );
		~LangES_GF();
		BOOL create(VOID);

		VOID set_inf(UttWS &u, UttI p);
		VOID set_up_flag(UttWS &u, UttI p);

		BOOL set( const CHAR *param, const CHAR *val );
		const CHAR *get( const CHAR *param );

		VOID utt_gf( UttWS &ut );

};

/**********************************************************/

/*class LangES_Pos : public Pos {
private:
	LangES_Categ  categ;
	LangES_GF	grupof;

public:
	virtual ~LangES_Pos() {}; //needed to remove a warning
	BOOL create( VOID );

	VOID utt_pos( Utt *u );
	VOID utt_categ( Utt *u );
	VOID utt_gf( Utt *u );

	BOOL set( const CHAR *param, const CHAR *val );
	const CHAR *get( const CHAR *param );
};*/

#endif

/**********************************************************/
