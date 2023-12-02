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

Nombre fuente................ es_phtr.cpp
Nombre paquete............... aHoTTS
Lenguaje fuente.............. C++
Estado....................... -
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Borja Etxebarria
.............................

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
1.1.2    23/02/12  Inaki     speaker dependent transcription
1.1.1    12/12/11  Inaki     añadir opcion phtkatamotz para que no pronuncie con r fuerte las r de principio de palabra
1.1.0    20/10/09  Inaki     Utilizar reglas de Mariño - Txillardegui, corregir bugs (coarticulación)
1.0.1    03/10/07  Inaki     Aplicar reglas solo dentro de palabra, añadir ü ...
1.0.0    31/01/00  borja     codefreeze aHoTTS v1.0
0.0.0    24/11/97  borja     Codificacion inicial.

======================== Contenido ========================
$$$ hielo --> jjelo. o sea j al principio de palabra pasa a jj ???
$$$ conyuge:  dj en vez de jj??  dj en castellano...
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/
//INAKI
#define xSIN_APROX	//sin B,D,G
#include <string.h>

#include "chset.h"
#include "es_lingp.hpp"

/**********************************************************/
/* Trabajamos entre pausas (pause group).
No contemplamos vocales aproximantes (j,w) hasta despues de la
silabificacion! */

VOID LangES_PhTrans::pausegr_ch2ph(UttPh & u, UttI senp)
{
#define CH(p)  u.cell(p).getChar()
#define SETPH(p,ph)  u.cell(p).setPhone(ph)
#define SETSTRESS(p) u.cell(p).setStress(USTRESS_TEXT)
#define NEXT(p)  u.charNext(p,URANGE_PAUSE)
#define PREV(p)  u.charPrev(p,URANGE_PAUSE)
#define DEL(p)  NEXT(p)

	CHAR *eietit = (char *)"ei" CS_S_etilde CS_S_itilde;
	CHAR *lns = (char *)"lns";
	CHAR *nml = (char *)"nml";
	CHAR *nm = (char *)"nm";
	CHAR *vow = (char *)"aeiou" CS_S_atilde CS_S_etilde CS_S_itilde CS_S_otilde CS_S_utilde CS_S_uuml;

	UttI p, p2, p3;
	INT ch, ch2, ch3;

	p = u.charFirst(senp, URANGE_PAUSE);
	while (p) {
		ch = CH(p);		// caracter actual
		p2 = NEXT(p);
		ch2 = (p2) ? CH(p2) : 0;	// caracter 2 (0 si no hay mas)

		switch (ch) {
		case 'a': SETPH(p, PHES_a); break;
		case 'e': SETPH(p, PHES_e); break;
		case 'i': SETPH(p, PHES_i); break;
		case 'o': SETPH(p, PHES_o); break;
		case 'u': SETPH(p, PHES_u); break;
		case CS_atilde: SETPH(p, PHES_a); SETSTRESS(p); break;
		case CS_etilde: SETPH(p, PHES_e); SETSTRESS(p); break;
		case CS_itilde: SETPH(p, PHES_i); SETSTRESS(p); break;
		case CS_otilde: SETPH(p, PHES_o); SETSTRESS(p); break;
		case CS_utilde: SETPH(p, PHES_u); SETSTRESS(p); break;
		case CS_uuml:	SETPH(p, PHES_u); break;
		case 'v':
		case 'b':
			p3 = PREV(p);  ch3 = p3 ? CH(p3) : 0;
			if (u.charIsFirst(p,URANGE_PAUSE)|| (!ch3) || strchr(nm, ch3)) SETPH(p, PHES_b); // [nm#]b -> b
			else
				#ifdef SIN_APROX
				SETPH(p, PHES_b); // b -> b
				#else
				SETPH(p, PHES_baprox); // b -> B
				#endif
			break;
		case 'c':
			if (ch2) {
				if (strchr(eietit, ch2)) { SETPH(p, PHES_z); break; } // ce, ci
				//INAKI: interior de palabra
				else if (!u.charIsLast(p, URANGE_WORD) && ch2 == 'h') { SETPH(p, PHES_ch); break; } // ch: "h" la saltamos despues
			}
			SETPH(p, PHES_k); // resto de casos, c->k
			break;
		case 'd':
			p3 = PREV(p);  ch3 = p3 ? CH(p3) : 0;
		/*	if(!strcmp(pht_speaker,"Amaia")){ //la d final de palabra se elimina
				if(u.charIsLast(p,URANGE_WORD)){
					p2=DEL(p);
					break;
				}
			}*/
			if (u.charIsFirst(p,URANGE_PAUSE) || (!ch3) || strchr(nml, ch3)) SETPH(p, PHES_d); // [nml#]d -> d
			else
			#ifdef SIN_APROX
				SETPH(p, PHES_d); // d -> d
			#else
				SETPH(p, PHES_daprox);  // d -> D
			#endif
			break;
		case 'f': SETPH(p, PHES_f); break;
		case 'g':
			if (ch2) {
				if (strchr(eietit, ch2)) { SETPH(p, PHES_j); break; }	// ge, gi
				if (ch2 == 'u') {
					p3 = NEXT(p2); ch3 = p3 ? CH(p3) : 0;	// caracter 3 (0 si no hay mas)
					if (ch3 && strchr(eietit, ch3)) p2 = DEL(p2);	// gue, gui -> ge, gi (saltamos la u, la g se hace abajo)
					// g* -> g *  (esto se hace justo aqui abajo)
				}
			}
			p3 = PREV(p);  ch3 = p3 ? CH(p3) : 0;
			if (u.charIsFirst(p,URANGE_PAUSE)|| (!ch3) || strchr(nm, ch3)) SETPH(p, PHES_g); // [nm#]g -> g
			else
				#ifdef SIN_APROX
				SETPH(p, PHES_g); // g -> g
				#else
				SETPH(p, PHES_gaprox); // g -> G
				#endif
			break;
		case 'h': //INAKI: h+ie : hi->y
				p3 = NEXT(p2);  ch3 = p3 ? CH(p3) : 0;
				if(u.charIsFirst(p,URANGE_WORD) && ch2 == 'i' && ch3 =='e'){
					p2 = DEL(p); //borrar la h
					SETPH(p2, PHES_y); // i -> y
					p2 = DEL(p2);
					break;
				}
				if(!strcmp(pht_speaker,"Amaia")){//INAKI: h+u+e --> G+u+e
					if(u.charIsFirst(p,URANGE_WORD) && ch2 == 'u' && ch3 =='e'){
						p3 = PREV(p);  ch3 = p3 ? CH(p3) : 0;
						if (u.charIsFirst(p,URANGE_PAUSE)|| (!ch3)) SETPH(p, PHES_g); //g
						else
							#ifdef SIN_APROX
			                                SETPH(p, PHES_g); // g -> g
                        			        #else
			                                SETPH(p, PHES_gaprox); // g -> G
                        			        #endif
						break;
					}
				}
				p2 = DEL(p); break;	// saltamos la h y preparamos p2 al siguiente.
		case 'j': SETPH(p, PHES_j); break;
		case 'k': SETPH(p, PHES_k); break;
		case 'l'://INAKI: interior de palabra
			if (!u.charIsLast(p, URANGE_WORD) && ch2 == 'l') { SETPH(p, PHES_ll); p2 = DEL(p2); }	// ll
			else SETPH(p, PHES_l);  // l->l
			break;
		case 'm': SETPH(p, PHES_m); break;
		case 'n'://INAKI n seguida de b --> m (p.e. tan bueno)
			// n seguida de m --> m (p.e. conmover)
			// n seguida de p --> m (p.e. sin parar)
			// n seguida de v --> m (p.e. convencer)
			// n seguida de f --> m (p.e. anfora)
			if(ch2 == 'b' || ch2 == 'm' || ch2 == 'p' || ch2 == 'v' || ch2 == 'f')
				 SETPH(p, PHES_m);
			else
				SETPH(p, PHES_n);
			break;
		case CS_ntilde: SETPH(p, PHES_ntilde); break;
		case 'p'://INAKI: interior de palabra
			if (!u.charIsLast(p, URANGE_WORD) && ch2 == 'h') {
				p3 = NEXT(p2);
				ch3 = p3 ? CH(p3) : 0;	// caracter 3 (0 si no hay mas)
				// ph->f cuando despues haya vocal, "y", o fin de palabra. la h nos la cargamos despues
				if (!ch3 || strchr(vow, ch3) || (ch3 == 'y')) {
					SETPH(p, PHES_f);
					break;
				}
			}
			SETPH(p, PHES_p);
			break;	// resto de casos, p->p
		case 'q':
			if (ch2 == 'u') {
				p3 = NEXT(p2);
				ch3 = p3 ? CH(p3) : 0;	// caracter 3 (0 si no hay mas)
				if (!((ch3 == 'a') || (!ch3)))
					p2 = DEL(p2);	// qu -> k excepto si qua o qu# (#=fin)
			}
			SETPH(p, PHES_k);
			break;	// q -> k
		case 'r':
			//version AHOLAB
	/*		//en el interior de palabra y entre vocales se pronuncia alveolar sonora
			p3 = PREV(p);  ch3 = p3 ? CH(p3) : 0;
			if (!(u.charIsFirst(p, URANGE_WORD)) && !(u.charIsLast(p, URANGE_WORD))
			 && strchr(vow,ch3) && strchr(vow, ch2)){
					SETPH(p, PHES_r); // V+r+V -> r
					break;
			}
			//resto de casos alveolar vibrante
			SETPH(p, PHES_rr);  // r -> rr
			//si viene una segunda r no hay que procesarla
			if (ch2=='r') p2=DEL(p2); // rr -> saltamos una r
			break;
			*/
			//version LLISTERRI MARIÑO
			p3 = PREV(p);  ch3 = p3 ? CH(p3) : 0;
			if(phtkatamotz && u.charIsFirst(p, URANGE_WORD)){
				SETPH(p, PHES_r);
				break;
			}
			if ((u.charIsFirst(p, URANGE_WORD)) || (!u.charIsLast(p, URANGE_WORD) && (ch3 == 'l' || ch3 == 'n' || ch3 == 's')))
				SETPH(p, PHES_rr);
			else if(!u.charIsLast(p, URANGE_WORD) && ch2 == 'r'){
				 SETPH(p, PHES_rr);
				 p2=DEL(p2); // rr -> saltamos una r
			}
			else
				 SETPH(p, PHES_r);
			break;

		case 's':
		/*	 if(!strcmp(pht_speaker,"Amaia")){ //INAKI
				//si es fin de palabra y la siguiente empieza por s, omitimos esta s
				if(u.charIsLast(p,URANGE_WORD) && ch2 == 's'){
					p2 = DEL(p);
					break;
				}
			} */
			 SETPH(p, PHES_s); break;
		case 't': SETPH(p, PHES_t); break;
		case 'w':
			if ((u.charIsFirst(p, URANGE_WORD)) && !(u.charIsLast(p, URANGE_WORD))
			 && strchr(vow,ch2))
			{
				p3 = PREV(p);  ch3 = p3 ? CH(p3) : 0;
	                        if (u.charIsFirst(p,URANGE_PAUSE)|| (!ch3) || strchr(nm, ch3)) // [nm#]g -> g
					SETPH(p, PHES_g);   	//g aproximante
				else
					SETPH(p, PHES_gaprox);	// g aproximante
				p = u.cellAddAfter(p);	// creamos celda para la [u]
				SETPH(p, PHES_u);	// [u]
			}
			else
			{
				SETPH(p, PHES_u);
			}
			break;
		case 'x':
			if (!(u.charIsFirst(p, URANGE_WORD))) // Si no es principio
			{
				// la x se expande a 2 fonemas: [k][s]
				SETPH(p, PHES_k);	// [k]
				p = u.cellAddAfter(p);	// creamos celda para la [s]
				SETPH(p, PHES_s);	// [s]
			}
			else
			{
				SETPH(p, PHES_s);	// []
			}
			break;
		case 'y': //INAKI: que no sea fin de palabra
			if (!ch2 || !strchr(vow, ch2) || u.charIsLast(p,URANGE_WORD)) SETPH(p, PHES_i); // y[C#] -> i  (C=conson.)
			else SETPH(p, PHES_y);  // y -> y
			break;
		case 'z': SETPH(p, PHES_z); break;

		// caracteres no contemplados, los saltamos directamente
		default: p2 = DEL(p); break;
		}
		p = p2;		// siguiente caracter
	}

#undef DEL
#undef SETPH
#undef NEXT
#undef PREV
}

/**********************************************************/
/* esta transforma fonemas i/u en j/w en los
diptongos/triptongos (convierte vocales -> aproximantes).
Se debe de hacer una vez silabificadas las palabras */

VOID LangES_PhTrans::iu2jw(UttPh & u)
{
	UttI p;

	for (p=u.phoneFirst(); p!=0; p=u.phoneNext(p)) {
		Phone ph = u.cell(p).getPhone();

		if (((ph == PHES_i) || (ph == PHES_u))
				&& (u.cell(p).getStress() == USTRESS_NONE)) {	// solo si es vocal debil no acentuada

			UttI pl = u.phonePrev(p, URANGE_SYLLABLE);
			UttI pr = u.phoneNext(p, URANGE_SYLLABLE);

			Phone phl = pl ? u.cell(pl).getPhone() : 0;		// fonema anterior en silaba

			Phone phr = pr ? u.cell(pr).getPhone() : 0;		// fonema siguiente en silaba

			if (LangES_PhUti::phIsVowel(phl) || LangES_PhUti::phIsVowel(phr))	// si hay vocal a izqda. o dcha. de vocal debil -> aproximante

				u.cell(p).setPhone((ph == PHES_i) ? PHES_iaprox : PHES_uaprox);
		}
	}
}

/**********************************************************/
