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
#ifndef __EU_POS_HPP__
#define __EU_POS_HPP__

/**********************************************************/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/*
(C) 1997 TSR/Aholab - ETSII/IT Bilbao (UPV/EHU)

Nombre fuente................ eu_pos.hpp
Nombre paquete............... aHoTTS
Lenguaje fuente.............. C++
Estado....................... -
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Inma Sardon
.............................



======================== Contenido ========================
<DOC>
</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

//#include "eu_lingp.hpp"
#include "eu_hdic.hpp"
#include "uttws.hpp"

#define MAX_TAM_WORD 100	//20 no es suficiente
/**********************************************************/

class LangEU_Categ {
	//	int num_Word;
	//	LangEU_HDicDB db;
	private:
		BOOL created;
	protected:
		String dbName;

	public:
		LangEU_Categ( VOID );
		~LangEU_Categ();
		BOOL create(VOID);

		BOOL set( const CHAR *param, const CHAR *val );
		const CHAR *get( const CHAR *param );

		VOID utt_categ(UttWS &ut);

	public:
		BOOL encontrado;
		VOID posdic(UttWS &u, UttI p);
		VOID aditudu(UttWS &u,UttI p,char *word_act);
//		BOOL babait(UttWS &u,UttI p, char *word_act,LangEU_HDicDB &db);
		VOID babait(UttWS &u,UttI p, char *word_act,LangEU_HDicDB &db);
		VOID atzize(UttWS &u,UttI p, char *word_act,LangEU_HDicDB &db);

		VOID atzadi(UttWS &u,UttI p, char *word_act,LangEU_HDicDB &db);

		VOID adit(UttWS &u,UttI p, char *adi,LangEU_HDicDB &db);
		VOID auxt(UttWS &u,UttI p, char *adi,LangEU_HDicDB &db);
		void detaux(UttWS &u, UttI p);
		void detior(UttWS &u, UttI p);
		void trnlgn(UttWS &u, UttI p);
		void jntazk(UttWS &u, UttI p,int numWord);
		void izejok(UttWS &u, UttI p);
		void adjjok(UttWS &u, UttI p);

};


/**********************************************************/


class LangEU_GF {
	private:
		BOOL created;
	protected:
		String dbName;

	public:
		LangEU_GF( VOID );
		~LangEU_GF();
		BOOL create(VOID);

		BOOL set( const CHAR *param, const CHAR *val );
		const CHAR *get( const CHAR *param );

		VOID utt_gf( UttWS &ut );

	public:
		typedef struct {
			char word[MAX_TAM_WORD];  //INMASS
			UttI p;		//puntero a celda
			unsigned long	word_POS,word_GF;
		} word_info;
	typedef struct{
		word_info prev;
		word_info act;
		word_info	next;
		word_info	nextn;
	}gf_info;

	typedef struct{
		BOOL phrase_last;	//detecta final de frase
		BOOL phrase_prev_last;
		BOOL utt_last;	//detecta final de utterance
		BOOL utt_prev_last;
		BOOL phrase_first; //detecta comienzo de frase
		BOOL utt_first;
	}up_flag;

	int indice2;
	int gf;
	BOOL encontrado;
	gf_info inf;
	up_flag u_p_flag;
	VOID set_inf(UttWS &u, UttI p);
	VOID set_up_flag(UttWS &u, UttI p);

	VOID enk(UttWS &u, UttI p, int *gf);
	VOID detize(UttWS &u, UttI p, int *gf);
	VOID izedet(UttWS &u, UttI p, int *gf);
	VOID izeadb(UttWS &u, UttI p, int *gf);
	VOID adbadj(UttWS &u, UttI p, int *gf);
	VOID izeize(UttWS &u, UttI p, int *gf);
	VOID baitadi(UttWS &u, UttI p, int *gf);
	VOID joklgn(UttWS &u, UttI p, int *gf);
	VOID jokjok(UttWS &u, UttI p, int *gf);
	VOID trn(UttWS &u, UttI p, int *gf);
	VOID invpau(UttWS &u, UttI p, int *gf);
	VOID jnt(UttWS &u, UttI p, int *gf);
	void galdeg(UttWS &u);
};

#endif

/**********************************************************/

/*class LangEU_Pos : public Pos {
private:
	LangEU_Categ  categ;
	LangEU_GF	grupof;

public:
	virtual ~LangEU_Pos() {}; //needed to remove a warning
	BOOL create( VOID );

	VOID utt_pos( Utt *u );
	VOID utt_categ( Utt *u );
	VOID utt_gf( Utt *u );

	BOOL set( const CHAR *param, const CHAR *val );
	const CHAR *get( const CHAR *param );
};

#endif*/
