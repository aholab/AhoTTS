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

Nombre fuente................ eu_phtr.cpp
Nombre paquete............... aHoTTS
Lenguaje fuente.............. C++
Estado....................... -
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion: ............... Borja Etxebarria
............................. Inigo Salbidea
..............................Ana Armenta
..............................Nora Barroso

Version  dd/mm/aa  Autor     Comentario
-------  --------  --------  ----------
2.0.6	 13/12/11  Inaki	 Añadir la opcion phtkatamotz para que no pronuncie como rr las palabras que empiece con r 
2.0.5	 14/02/11  Inaki	 Corregir palatalización 'l' en aditz trinkoak y aditzlagunak que no comiencen por "bail"
2.0.4	 11/01/10  Inaki	 Añadir transcripción para amaia
2.0.3    20/10/08  Inaki     Añadir soporte para transcripción en diccionario (Nora)
2.0.2    03/10/07  Inaki     phtsimple (no coarticula entre palabras, no comprobado..) 
			     PhTSpeaker (modifica comportamiento para locutor, Karolina)
2.0.1	 27/11/06  Eva       Arreglar los problemas que introdujo Nora en la 1.0.0
2.0.0		23/02/2005	Nora	Transkripzio fonetikoaren salbuespenak hiztegitik hartuta
1.0.0		01/01/05	Nora	   Arau berriak sartu
1.0.0    31/01/00  borja     codefreeze aHoTTS v1.0
0.2.2    30/12/99  borja     eliminar keyword "s t a t i c" redundante
0.2.1    04/xx/99  Ana       completar la adaptacion al euskera
0.2.0    04/05/99  Inigos    Funcion de excepciones
0.1.0    04/05/99  Inigos    Adaptacion al euskera
0.1.0    24/11/97  Borja     Codificacion inicial (castellano)

======================== Contenido ========================
<DOC>
Metodos de {{LangEU_PhTrans}} que efectuan la transcripcion fonetica
en si. Se analizan los caracteres de cada palabra y su contexto, y se
definen los fonemas que les corresponden.

"ch2ph" coge dos inicios de lista: fonemas {u} y caracteres {senp}.
Va asignando fonemas en funcion de los caracteres que le llegen
mediante un switch.
</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/


/**********************************************************/

#include <string.h>

#include "chset.h"
#include "eu_lingp.hpp"
#include "uti.h"

/**********************************************************/
/*convierte letras a fonemas para cada grupo de
palabras entre pausas*/

VOID LangEU_PhTrans::pausegr_ch2ph(UttPh & u, UttI senp)
{
//obtiene el caracter apuntado:
#define CH(p)  u.cell(p).getChar()

//escribe el fonema en el campo correspondiente del caracter apuntado por p:
#define SETPH(p,ph)  u.cell(p).setPhone(ph)

//acent£a el elemento apuntado por p:
#define SETSTREUS(p) u.cell(p).setStress(USTRESS_TEXT)

//adelanta el apuntador una posici¢n a nivel de letras:
#define NEXT(p)  u.charNext(p,URANGE_PAUSE)

//retrocede el apuntador una posici¢n a nivel de letras:
#define PREV(p)  u.charPrev(p,URANGE_PAUSE)

//adelanta el apuntador una posici¢n
#define DEL(p)  NEXT(p)
//escribe caracter
#define SETCHAR(p,ch) u.cell(p).setChar(ch)

	//int i;

	CHAR *ei = (char *)"ei";
	CHAR *aeo =(char *)"aeo";
	CHAR *aeou = (char *)"aeou";
	CHAR *aeiou = (char *)"aeiou";
	CHAR *nml = (char *)"nml";
	CHAR *nlsz = (char *)"nlsz";
	CHAR *nl = (char *)"nl";
	CHAR *mn = (char *)"mn";
	CHAR *sz = (char *)"sz";
	CHAR *sxz = (char *)"sxz";
//regla desconectada.	CHAR *ptkbdg = "ptkbdg";
	CHAR *tk =(char *)"tk";
	(void)mn;  // no se usa, asi quitamos el warning
	(void)nml;  // no se usa, asi quitamos el warning

	UttI p, p2, p3,p4,p5;
	INT ch, ch2, ch3,ch4,ch5;
	const CHAR * hitza;

	UttI wo;

	p = u.charFirst(senp, URANGE_PAUSE);

	while (p) {	//para todos las letras de un pausegr (grupo entre pausas)
		ch = CH(p);	// caracter actual
		p2 = NEXT(p);  //apuntador al caracter siguiente
		ch2 = (p2) ? CH(p2) : 0;	// caracter siguiente (0 si no hay siguiente)

 /***********Hitzaren transkripzio fonetikoa hiztegian agertzen bada**2005/03/16****************/
 		 wo=u.charFirst(p, URANGE_WORD);    
		 if (trans_fonet_hitza(u,wo))
		 {
		 //	fprintf(stderr, "MARKATURIK DAGO\n");
			//idazteko funtzioa erabili 
			p=tf_mrk_ch2ph(u,wo); 
			     // fprintf(stderr, "TRANSKRIBATZEN DENA:  %c\t\n",u.cell(p).getChar()); 
			p2 = NEXT(p);  //apuntador al caracter siguiente   
			ch2 = (p2) ? CH(p2) : 0;        // caracter siguiente (0 si no hay siguiente)
			if (ch2==0) 
			{
				break;
			}
			else
			{
				p=p2; 
				ch = CH(p);             // caracter actual   
				p2 = NEXT(p);  //apuntador al caracter siguiente  
				ch2 = (p2) ? CH(p2) : 0;        // caracter siguiente (0 si no hay siguiente) 
			}
		}
 /********************************************************************************/  
	

	
		switch (ch) {

		case 'a': SETPH(p, PHEU_a); break;

		case 'e': SETPH(p, PHEU_e); break;

		case 'i':
			 //en caso de formar parte de diptongo,seguida de n ¢ l,a su vez seguidas de
			 //vocal, la i se omite y n y l se palatalizan salvo excepciones.
			 //cuando se trata de bait+auxliar/sintetico contraido no se palataliza
			p3 = PREV(p);  ch3 = p3 ? CH(p3) : 0;//anterior a la i
			Phone ph3;
			if(p3) ph3=u.cell(p3).getPhone();
			else ph3=0;
			p4 = NEXT(p2);  ch4 = p4 ? CH(p4) : 0;//posterior a n ¢ l
			if (!u.charIsFirst(p,URANGE_WORD)&&!u.charIsLast(p, URANGE_WORD))
			{

/********************{g,k}+u+i+{n,l} Orduan i-> [j]*************************************************/
/*2005/01/31 - Nora*/
				if (!u.charIsFirst(p3,URANGE_WORD)&&(ch3=='u')&&strchr(nl, ch2))
				{
					p5 = PREV(p3);  ch5 = p5 ? CH(p5) : 0;
					if((ch5=='g')||(ch5=='k'))
					{
						SETPH(p,PHEU_iaprox);
						break;
					}				
				}
/****************************************************************************************************/
			
				if(strchr(aeou, ph3)&&strchr(nl, ch2)&&!u.charIsLast(p2, URANGE_WORD)
				&&strchr(aeiou, ch4))
				{
					wo=u.wordThis(p, URANGE_PAUSE);					
					if(trans_fonet_salb_i_0_j(u,wo)||es_verbo_trn(u,wo)||es_verbo_lgn(u,wo))
					{						
						SETPH(p,PHEU_iaprox);
						break;
					}
					break;
					
					
/*				
					//excepciones a la palatalizaci¢n:
					if(es_excepcion(u,p,'i')||es_bai_aditz(u,p))
					{
						SETPH(p,PHEU_iaprox);
						break;
					}
					break;
*/

				}

			}
/********************{a,e,o,u}+i+{a,e,o,u} Orduan i-> [jj]*************************************************/
			if(!u.charIsFirst(p,URANGE_WORD)&&!u.charIsLast(p, URANGE_WORD)&&strchr(aeou,ph3)&&strchr(aeou, ch2))
			{
				SETPH(p,PHEU_jj);
				break;
			}
/****************************************************************************************************/
			if(!u.charIsFirst(p,URANGE_WORD)&&strchr(aeou,ph3)){
				SETPH(p,PHEU_iaprox);
				break;
			}
			SETPH(p, PHEU_i); //i -> i
			break;

		case 'o': SETPH(p, PHEU_o); break;
		case 'u':
			//miro si es diptongo
			p3 = PREV(p);  ch3 = p3 ? CH(p3) : 0;
			if (strchr(aeo, ch3)&&!u.charIsLast(p3, URANGE_WORD)){
				SETPH(p,PHEU_uaprox);
				break;
			}
			//si no es diptongo, normal
			SETPH(p, PHEU_u); break;
		case CS_atilde: SETCHAR(p,'a');SETPH(p, PHEU_a); SETSTREUS(p); break;
		case CS_etilde: SETCHAR(p,'e');SETPH(p, PHEU_e); SETSTREUS(p); break;
		case CS_itilde: SETCHAR(p,'i');SETPH(p, PHEU_i); SETSTREUS(p); break;
		case CS_otilde: SETCHAR(p,'o');SETPH(p, PHEU_o); SETSTREUS(p); break;
		case CS_utilde: SETCHAR(p,'u');SETPH(p, PHEU_u); SETSTREUS(p); break;
		case CS_uuml:	  SETCHAR(p,'u');SETPH(p, PHEU_u); break;

		case 'b':
			p3 = PREV(p);  ch3 = p3 ? CH(p3) : 0;
			if(!ch3){
				SETPH(p,PHEU_b);
				break;
			}
			//si es principio de palabra y va tras 'ez' la b se ensordece
			if (ch3&&u.charIsFirst(p,URANGE_WORD)&&(!phtsimple))
			{
				wo=u.wordPrev(p, URANGE_PAUSE); // palabra previa
				hitza = u.cell(wo).getWord();
				wo=u.wordThis(p,URANGE_PAUSE);
				if(!(strcmp(hitza,"ez")) && strcmp(pht_speaker,"Amaia")) // previa = ez
				{
					if(es_verbo_trn(u,wo)||es_verbo_lgn(u,wo))
					{
						SETPH(p,PHEU_p); // b -> p
						break;
					}
				}
			}
			if( ( (ch3=='l') && ( (ch2=='r') || (strchr(aeiou,ch2)) ) )
				|| (strchr(aeiou,ch3) && ( strchr(aeiou,ch2) || (ch2=='l') || (ch2=='r') ) ) )
			{
				SETPH(p, PHEU_baprox);
				break;
			}
			if(((ch3=='o')&&(ch2=='s'))
				||((ch3=='u')&&(ch2=='s')))
			{
				SETPH(p, PHEU_baprox);
				break;
			}

			//resto de casos
			SETPH(p,PHEU_b);
			break; // b -> b;

		case 'c':
			if (!(u.charIsLast(p, URANGE_WORD)))
			{ //si no es £ltimo caracter de palabra
				//seguido de e ¢ i se pronuncia PHEU_T
				if (strchr(ei, ch2) || ch2 == CS_itilde || ch2 == CS_etilde) { SETPH(p, PHEU_T); break; } // ce, ci; c -> T
				//seguido de h se pronuncia PHEU_tx
				if (ch2 == 'h') { SETPH(p, PHEU_tx); break; }
			}
			//en el resto de los casos se pronuncia k
			SETPH(p, PHEU_k);
			break;

		case 'd':
			p3 = PREV(p);  ch3 = p3 ? CH(p3) : 0;
			//si es comienzo de palabra pero no tras pausa absoluta
			if(!(u.wordIsFirst(p, URANGE_PAUSE)) && u.charIsFirst(p, URANGE_WORD)&&(!phtsimple)&& strcmp(pht_speaker,"Karolina") && strcmp(pht_speaker,"Amaia"))
			{
				wo=u.wordPrev(p, URANGE_PAUSE);
				hitza = u.cell(wo).getWord();
				wo=u.wordThis(p,URANGE_PAUSE);
				//y si la palabra anterior es 'ez' ¢ 'bait'
				if(!(strcmp(hitza,"ez")) || !(strcmp(hitza,"bait")))
				{
					if(es_verbo_trn(u,p)||es_verbo_lgn(u,p))
					{
						SETPH(p,PHEU_t); // d -> t
						break;
					}
				}
			}

			if(!(u.charIsLast(p, URANGE_WORD))){
				if(ch2=='d')
				{
/********************hitz zehatz batetan dd -> [d]*************************************************/
				/*	if(es_excepcion(u,p,'d'))
					{
						SETPH(p,PHEU_d);
						p2=DEL(p2);
						break;
					}*/
/*********************************************************************************************/
					//si viene la graf¡a 'dd'
					if(u.charIsFirst(p, URANGE_PAUSE)||strchr(nlsz,ch3)||strchr(tk,ch3))
					{
						//y es comienzo absoluto o va precedida de n,l,s,z,t ¢ k
						SETPH(p, PHEU_dj); // dd -> dj
					}
					else
					{
						//en cualquier otro caso se pronuncia PHEU_jj
						SETPH(p, PHEU_jj); // dd -> jj
					}
					//nos saltamos la 2¦ d (porque su fonema ya est  procesado)
					p2=DEL(p2);
					break;
				}
			}
			//si no es ppio absoluto, y la anterior es vocal:
			if(!u.charIsFirst(p,URANGE_PAUSE)&&strchr(aeiou,ch3))
			{
				SETPH(p, PHEU_daprox);//d -> D
				break;
			}

			//en el resto de casos se pronuncia d oclusiva
			SETPH(p,PHEU_d); // d -> d
			break;

		case 'f': SETPH(p, PHEU_f); break;

		case 'g':
			//caracter anterior
			p3 = PREV(p);  ch3 = p3 ? CH(p3) : 0;
			//si no es comienzo absoluto y es comienzo de palabra cuando se trata
			//de verbo auxiliar ¢ sint‚tico precedido de ez o bait, se pronuncia k
			if(!(u.wordIsFirst(p, URANGE_PAUSE)) && u.charIsFirst(p, URANGE_WORD)&&(!phtsimple)&& strcmp(pht_speaker,"Amaia") )
			{
				//miro si la palabra anterior es 'ez' o 'bait'
				wo=u.wordPrev(p, URANGE_PAUSE);
				hitza = u.cell(wo).getWord();
				wo=u.wordThis(p,URANGE_PAUSE);
				if(!(strcmp(hitza,"ez")) || !(strcmp(hitza,"bait")))
				{
					if(es_verbo_trn(u,wo)||es_verbo_lgn(u,wo))
					{
						SETPH(p,PHEU_k); // g -> k
						break;
					}
				}
			}

			if(!u.charIsFirst(p,URANGE_PAUSE)&&(strchr(aeiou,ch3)||ch3=='l')){
				SETPH(p, PHEU_gaprox);
				break;
			}
			//en el resto de los casos se pronuncia oclusiva
			else SETPH(p, PHEU_g); // g -> G
			break;

		case 'h':
			 // saltamos la h.
			 break;

		case 'j':
			//para ciertas excepciones se pronuncia como en castellano
			wo=u.wordThis(p, URANGE_PAUSE);
			if(trans_fonet_salb_j_0_x(u,wo))
			{
				SETPH(p, PHEU_j); // j->x
				break;
			}
			p3 = PREV(p);  ch3 = p3 ? CH(p3) : 0;//caracter anterior

/********************{a,e,i,o,u}+j+{a,e,i,o,u} Orduan j-> [x]*************************************************/
/* Eva: 27/11/2006*/
/* Esta regla no es verdad. Muchas de las j entre vocales se pronuncian como jj: burujabe, gizajale... Solo hay que mirar
 * Lo que nos pasaron Amaia y Ziortza. No se de donde ha salido esta regla y creo que habría que desconectarla y añadir
 * en el diccionario todas las excepciones a la jj que hagan falta */
			
/*			if(!u.charIsFirst(p,URANGE_WORD)&&!u.charIsLast(p, URANGE_WORD)&&strchr(aeiou,ch3)&&strchr(aeiou, ch2))
			{
				SETPH(p,PHEU_j); // j->x
				break;
			}
*/
/****************************************************************************************************/
		
		 //si es comienzo absoluto o va precedido de n,l,s,z se pronuncia PHEU_dj
			if(u.charIsFirst(p, URANGE_PAUSE) || strchr(nlsz,ch3)|| strchr(tk,ch3))
			{
				SETPH(p, PHEU_dj); // j -> dj;
				break;
			}
			//en el resto de los casos
			SETPH(p, PHEU_jj); // j -> jj
			break;

		case 'k':
			//si es el £ltimo caracter de la palabra y la siguiente comienza por
			// p,t,k,b,d,g desaparece la k y {b,d,g}se pronuncian plosivas.
/* Ya no hacemos lo de quitar la k antes de otra oclusiva.


			if(u.charIsLast(p, URANGE_WORD)&&(!phtsimple))
			{
				if(!(u.wordIsLast(p, URANGE_PAUSE)))
				{
					if(strchr(ptkbdg,ch2))
					{
						// k -> [/]
						if(ch2=='b'){SETPH(p2,PHEU_b);p2=DEL(p2);}
						if(ch2=='d'){SETPH(p2,PHEU_d);p2=DEL(p2);}
						if(ch2=='g'){SETPH(p2,PHEU_g);p2=DEL(p2);}
						break;
					}
				}
			}
*/
			//en el resto de los casos se pronuncia k.


/********************k+k Orduan k-> [k]*************************************************/
			if(!u.charIsLast(p, URANGE_PAUSE)&&ch2=='k'&&(!phtsimple))
			{
				SETPH(p,PHEU_k);
				p2=DEL(p2);
				break;
			}
/***************************************************************************************/

			SETPH(p, PHEU_k); // k -> k
			break;

		case 'l':
			//si va en interior de palabra,precedida de 'i',y seguida de aeou
			// se palataliza ,salvo si se trata de excepci¢n o
			//de expresi¢n (bait+auxiliar\sintatico) contraida
			p3 = PREV(p);  ch3 = p3 ? CH(p3) : 0;
			if (!u.charIsLast(p, URANGE_WORD) && !u.charIsFirst(p, URANGE_WORD)) //interior de palabra
			{
				if(ch3=='i') //caracter anterior
				{
					//si no es bait+auxiliar/sintetico (caso contemplado en el case'i')
					//precedida de 'i' y seguida de vocal [aeou] se palataliza
					if (strchr(aeou, ch2))
					{
						wo=u.wordThis(p, URANGE_PAUSE);
						if(trans_fonet_salb_l_L_l(u,wo))
						{
							SETPH(p, PHEU_l); // l -> l ; no palatalizacion
							break;
						}	
						///INAKI
						if (es_verbo_trn(u,wo)||es_verbo_lgn(u,wo)){
							//si la l se corresponde al siguiente inicio de palabra: "bail", no se palataliza
							const CHAR *hitza;
							hitza = u.cell(wo).getWord(); // palabra actual
							if( hitza[0] == 'b' && hitza[1] == 'a' && hitza[2] == 'i' && hitza[3] == 'l'){
								SETPH(p, PHEU_l); // l -> l ; no palatalizacion
								break;
							}
							else{
								SETPH(p,PHEU_ll);
								break;
							}
						}
						///////////////
/*
						if(es_excepcion(u,p,'l')||es_bai_aditz(u,p))
						{
							SETPH(p, PHEU_l); // l -> l ; no palatalizacion
							break;
						}
*/

/********************"filo" duten hitzetan l -> l egitea******************************************/

/*
						p4 = PREV(p3);  ch4 = p4 ? CH(p4) : 0;
						if(ch4=='f'&& ch2=='o')
						{
							SETPH(p, PHEU_l); // l -> l ; no palatalizacion
							break;
						}
*/
/*****************************************************************************************/
						SETPH(p,PHEU_ll);
						break; // siguiente vocal; l -> [L]
					}
				}
			}
			//dos 'l' seguidas dentro de la misma palabra
			if (!(u.charIsLast(p, URANGE_WORD)) && ch2 == 'l')
			{
				SETPH(p, PHEU_ll);
				p2 = DEL(p2);	// l -> ll
				break;
			}
			//en el resto de los casos
			SETPH(p, PHEU_l);  // l->l
			break;

		case 'm':
/********************m+m Orduan m-> [m]*************************************************/
			if(!u.charIsLast(p, URANGE_PAUSE)&&ch2=='m')
			{
				SETPH(p,PHEU_m);
				p2=DEL(p2);
				break;
			}
/**************************************************************************************/

			SETPH(p, PHEU_m);
			break;

		case 'n':
			//si va en interior de palabra,precedida de 'i',y seguida de vocal
			// se palataliza ,salvo si se trata de excepci¢n o
			//de expresi¢n (bait+auxiliar\sint‚tico) contraida
/*Eva 4/12/2006
 * Efectivamente la transcripción debería hacerse como indica el comentario anterior. Pero cuando Nora rehizo las reglas 
 * de transcripcion eliminó la palatalizacion para todos los verbos, sean bainaiz o ginen. Como hay verbos que se 
 * tienen que palatalizar, dejo la regla en que sólo se deja de palatalizar si es excepcion y he añadido en el diccionario
 * a todos los verbos bain* la etiqueta de excepcion a la palatalizacion de la n */
			p3 = PREV(p);  ch3 = p3 ? CH(p3) : 0;
			if (!(u.charIsFirst(p, URANGE_WORD)) && !(u.charIsLast(p, URANGE_WORD))) //interior de palabra
			{
				if(ch3=='i') //precedida de i
				{
					if (!(u.charIsLast(p, URANGE_WORD)) && strchr(aeiou, ch2)) // interior de palabra & sig vocal.
					{
						wo=u.wordThis(p, URANGE_PAUSE);
						//Eva 4/12/2006
						//if(trans_fonet_salb_n_J_n(u,wo)||es_verbo_trn(u,wo)||es_verbo_lgn(u,wo))
						if(trans_fonet_salb_n_J_n(u,wo))
						{
							SETPH(p, PHEU_n); // n -> n ; no palatalizacion
							break;
						}

							
/*						
							if(es_excepcion(u,p,'n')||es_bai_aditz(u,p))
							{
								SETPH(p, PHEU_n); // n -> n ; no palatalizacion
								break;
							}
*/
						//si no es bai+auxiliar/sintetico ni excepci¢n
						//precedida de 'i' y seguida de vocal se palataliza
						SETPH(p,PHEU_ntilde);
						break; // siguiente vocal; n -> [J]
					}
				}
				//en los grupos nst y nzt de interior de palabra desaparece la n
				if(!(u.charIsLast(p, URANGE_WORD)) && strchr(sz,ch2)&&(!phtsimple))
				// siguiente s o z
				{
					p3 = NEXT(p2); ch3 = p3 ? CH(p3) : 0;
					if(ch3=='t')//la siguiente a [sz] es 't'
						break; // [aeiou]n[sz]t -> [aeiou][sz]t
				}
			}
			//INAKI: si 'n' es final de palabra, precedida de 'i' y la siguiente palabra empieza por vocal: n->J
			//Ej. orain arte
			else if(u.charIsLast(p, URANGE_WORD) && ch3=='i' && !u.charIsFirst(p3, URANGE_WORD) && !strcmp(pht_speaker,"Karolina")){
				p4 = PREV(p3);  ch4 = p4 ? CH(p4) : 0; //si la palabra acaba en "kin", no se palataliza
				if(ch4 != 'k'){
					p5 = NEXT(p2); ch5 = p5 ? CH(p5) : 0; //si la siguiente palabra empieza por 'h', miramos si la siguiente letra es vocal
					if((strchr(aeiou, ch2) && ch2!=0 )|| (ch2=='h' && strchr(aeiou, ch5) && ch5!=0)){
						SETPH(p,PHEU_ntilde);
						p5 = PREV(p4);  ch5 = p5 ? CH(p5) : 0; 
						//eliminamos la i anterior si hay otra vocal antes (orain arte --> oraJ arte) (egin arte --> egiJ arte)
						if((strchr(aeiou, ch4) && ch4!=0) || (ch4 =='h' && strchr(aeiou,ch5) && ch5!=0) )
							SETPH(p3,' ');
						break; // siguiente vocal; n -> [J]
					}
				}
			}															/*********************************************************/

			//si la n precede a m,b,¢ p se pronuncia m
			p3 = NEXT(p); ch3 = p3 ? CH(p3) : 0;
			if((ch3=='b' || ch3=='p' || ch3=='m' )&&(!phtsimple))
			{
				SETPH(p, PHEU_m); // n [mbp] -> m
				break;
			}
			//en el resto de los casos se pronuncia n
			SETPH(p, PHEU_n); // n -> n
			break;

		case CS_ntilde: SETPH(p, PHEU_ntilde); break;

		case 'p':
			//al final de palabra, si la siguiente empieza con p,t,k,b,d,g,
			//y ambas palabras no est n separadas por pausa, desaparece la p.
			//en ese caso {b,d,g} plosivas
/*Ya no hacemos lo de quitar la p antes de oclusiva
			if(u.charIsLast(p, URANGE_WORD) && !(u.wordIsLast(p, URANGE_PAUSE))&& (!phtsimple))
			{
							if(strchr(ptkbdg,ch2)){
								if(ch2=='b'){SETPH(p2,PHEU_b);p2=DEL(p2);}
								if(ch2=='d'){SETPH(p2,PHEU_d);p2=DEL(p2);}
								if(ch2=='g'){SETPH(p2,PHEU_g);p2=DEL(p2);}
								break;
							}
				// p -> [/]
			}
*/
			// resto de casos se pronuncia p
			SETPH(p, PHEU_p);
			break;
		case 'q':
			//si va seguida de 'u'nos saltamos la 'u' y se pronuncia k
			//a no ser que sea 'qua' que no saltamos la u(la pronunciamos)
			if (ch2 == 'u') {
				p3 = NEXT(p2);
				ch3 = p3 ? CH(p3) : 0;	// caracter 3 (0 si no hay mas)
				if (!((ch3 == 'a')||(ch3=='o')||u.charIsLast(p2, URANGE_WORD)))
				p2 = DEL(p2);	// qu -> k excepto si qua o qu# (#=fin)
			}
			//siempre se pronuncia k
			SETPH(p, PHEU_k);
			break;	// q -> k */

		case 'r':
			//en el interior de palabra y entre vocales se pronuncia alveolar sonora
			p3 = PREV(p);  ch3 = p3 ? CH(p3) : 0;
			if (!(u.charIsFirst(p, URANGE_WORD)) && !(u.charIsLast(p, URANGE_WORD))
			 && strchr(aeiou,ch3) && strchr(aeiou, ch2)){
					SETPH(p, PHEU_r); // V+r+V -> r
					break;
			}
			if(phtkatamotz && u.charIsFirst(p, URANGE_WORD)){	//si esta activada esta opción y es inicio de palabra, r suave
				SETPH(p, PHEU_r);
				break;
			}
			//resto de casos alveolar vibrante
			SETPH(p, PHEU_rr);  // r -> rr
			//si viene una segunda r no hay que procesarla
			if (ch2=='r') p2=DEL(p2); // rr -> saltamos una r
			break;

		case 's':
			/*******caso ts*******/
			//si el caracter anterior es 't', se pronuncia ts excepto al final de
			//palabra si la siguiente comienza por consonante que se pierde la t.
			//ej: akats gehiegi--->akasgeieGi
			p3 = PREV(p);  ch3 = p3 ? CH(p3) : 0;
			//si delante de la s hay una 't' y est  en la misma palabra que la 's'
			if(ch3=='t' && !(u.charIsFirst(p, URANGE_WORD)))
			{ //si la s es £ltima de palabra pero no es final de frase
				if(u.charIsLast(p, URANGE_WORD) && !u.wordIsLast(p, URANGE_PAUSE)&& (!phtsimple) && strcmp(pht_speaker,"Amaia") )
				{
					//miro si la siguiente palabra empieza por consonante
					wo=u.wordNext(p, URANGE_PAUSE);
					hitza = u.cell(wo).getWord();
					ch3=hitza[0];

					if(ch3!='h' && !(strchr(aeiou,ch3)))
					{
						//en caso afirmativo pronuncio s.
						SETPH(p,PHEU_s);
						break;
					}
				}
				//resto de casos pronuncio ts.
				//asigno el fonema al primer caracter de la digraf¡a(la t)
				SETPH(p3, PHEU_ts);
				break;
			}
			
/********************s+s Orduan s-> [s]*************************************************/
/*2005/01/31 - Nora*/
			if(!u.charIsLast(p, URANGE_PAUSE)&&ch2=='s'&&(!phtsimple))
			{
				SETPH(p,PHEU_s);
				p2=DEL(p2);
				break;
			}
/**************************************************************************************/

			/*************caso s******************/
		 //si no hay t delante dentro de la misma palabra
			SETPH(p, PHEU_s);
			break;

		case 't':
		/**************** caso tt **********************/
			//si van dos 't' seguidas en la misma palabra se pronuncia PHEU_Q
			if(!(u.charIsLast(p, URANGE_WORD)) && ch2=='t')
			{
				SETPH(p, PHEU_tt); // tt -> Q
				p2 = DEL(p2);
				break;
			}
		/*************** t al final de palabra **************/
			//si la t va al final de palabra (no al final de frase) hay que mirar
			//si la siguiente palabra comienza por b,d,g,p,t,k --> la t desaparece
			//y b,d,g oclusivas
			if(u.charIsLast(p, URANGE_WORD) && !u.wordIsLast(p, URANGE_PAUSE)&& (!phtsimple))
			{
/*Ya no hacemos lo de quitar la t antes de oclusiva

					if(strchr(ptkbdg,ch2)) // siguiente bodega-petaka
					{
						// t -> [/]
						if(ch2=='b'){SETPH(p2,PHEU_b);p2=DEL(p2);}
						if(ch2=='d'){SETPH(p2,PHEU_d);p2=DEL(p2);}
						if(ch2=='g'){SETPH(p2,PHEU_g);p2=DEL(p2);}
						break;
					}
*/
					/********* caso lehen bait lehen ***********/
					wo=u.wordThis(p, URANGE_PAUSE);
					hitza = u.cell(wo).getWord();
					if(!strcmp(hitza,"bait")) // actual = bait
					{
						//el caso "lehen bait lehen" es una excepci¢n
						//se pronuncia lembailen
						if(!(u.wordIsFirst(p, URANGE_PAUSE)))
						{ //si al bait le sigue lehen
							wo=u.wordNext(p, URANGE_PAUSE);
							hitza = u.cell(wo).getWord(); // palabra siguiente
							if(!strcmp(hitza,"lehen"))
							{ //y si va precedido de lehen
								wo=u.wordPrev(p, URANGE_PAUSE);
								hitza = u.cell(wo).getWord(); // palabra anterior
								if(!strcmp(hitza,"lehen"))
									// lehen bait lehen  t->[/]
								break;
							}
						}
					/************ caso bait + aux/sint ************/

					//si la t corresponde a bait seguido de auxiliar o sintetico
					//que comience por n,l,z desaparece la t en los dos primeros casos
					//y se pronuncia tz en el caso de la z.

/********************bait + lgn/trn izanda *******************************************************/
/* Bai(t)naiz, bai(t)haiz, bai(t)kara, baitzara********************************************************/
/*************Aldaketak sartu ditut************************************************************/
						wo=u.wordNext(p, URANGE_PAUSE);
						hitza = u.cell(wo).getWord();
						if((hitza[0]!='z')&&(es_verbo_trn(u,wo)||es_verbo_lgn(u,wo))) break;

/**************LEHEN EGITEN ZENA***********************************************************/					
/*
						//bait seguido de auxiliar o sint‚tico que comience por n
						//nos saltamos la t
						// bait + [n]adz -> bai n
						if((hitza[0]=='n')&&(es_verbo_trn(u,wo)||es_verbo_lgn(u,wo))) break;
						//bait seguido de auxiliar o sint‚tico que comience por l
						//nos saltamos la t
						// bait + [l]adz -> bai l
						if((hitza[0]=='l')&&(es_verbo_trn(u,wo)||es_verbo_lgn(u,wo))) break;
						//bait seguido de auxiliar o sint‚tico que comience por z
						//pronunciamos tz y nos saltamos la z
*/
/******************************************************************************************/


						if((hitza[0]=='z')&&(es_verbo_trn(u,wo)||es_verbo_lgn(u,wo)))
						{
							SETPH(p2, PHEU_tZ); // t -> tz // bait + [z]adz -> bai+[tZ]adz
							p2=DEL(p2);//nos saltamos la z para no procesarla
							break;
						}
					}
				/*********** st + consonante *****************/
				//si la t va al final de palabra precedida de s ,y si la siguiente
				//palabra comienza por consonante, omitimos la t.
				//ejemplo:bost gizon-->bosgizon
				p3 = PREV(p);  ch3 = p3 ? CH(p3) : 0;
				if(ch3=='s') // anterior s
				{
						wo=u.wordNext(p, URANGE_PAUSE);
						hitza = u.cell(wo).getWord(); // siguiente palabra
						ch3=hitza[0]; // comienzo siguiente palabra
						if(!strchr(aeiou,ch3)) // siguiente consonante
						 // t -> [/]
							break;
				}
			}

			//si la t no est  al final de palabra miro si va seguida de s,x,z
			//en caso afirmativo me salto la t, no la proceso porque se procesan
			//las digraf¡as ts, tx y tz en la s,x y z respectivamente
			p3 = NEXT(p);  ch3 = p3 ? CH(p3) : 0;
			if(!(u.charIsLast(p,URANGE_WORD)) && strchr(sxz,ch3)) break;// st -> s

			SETPH(p, PHEU_t); // t -> t
			break;
		case 'v':
			p3 = PREV(p);  ch3 = p3 ? CH(p3) : 0;
			if(u.charIsFirst(p,URANGE_PAUSE)||(ch3=='n'||ch3=='m'||ch3=='¤')){
				SETPH(p,PHEU_b); // v -> b
				break;
			}
			//resto de casos
			SETPH(p,PHEU_baprox);
			break; // v -> B;

		case 'w': 
/********************"w", "u" bezela tratatu************************************************************/
			p3 = PREV(p);  ch3 = p3 ? CH(p3) : 0;
			if (strchr(aeo, ch3)&&!u.charIsLast(p3, URANGE_WORD)){
				SETPH(p,PHEU_uaprox); // w -> w
				break;
			}			
/*************************************************************************************************/
SETPH(p, PHEU_u); break;  // w -> u


		case 'x':
		/****************** caso tx ************************/
			//si el caracter anterior es 't', se pronuncia tS excepto al final de
			//palabra si la siguiente comienza por consonante que se pierde la t.
			//ej: antx pixka--->anxpixka
			p3 = PREV(p);  ch3 = p3 ? CH(p3) : 0;

			if(ch3=='t' && !(u.charIsFirst(p, URANGE_WORD)))
			{ //si el anterior es t y est  dentro de la misma palabra
				//y si no es la £ltima palabra de la frase
				if(u.charIsLast(p, URANGE_WORD) && !(u.wordIsLast(p, URANGE_PAUSE))&& (!phtsimple))
				{
				 //miro la siguiente palabra
					wo=u.wordNext(p, URANGE_PAUSE);
					hitza = u.cell(wo).getWord();
					ch3=hitza[0];
					//si comienza por consonante
					if( ch3!='h' && !(strchr(aeiou,ch3)))
					{
						SETPH(p,PHEU_x);
						break;
					}
				}
				SETPH(p3, PHEU_tx); // tx -> tS
				break;
			}
			/**************** caso x ********************/
			//si la x no va precedida de t se pronuncia x
			SETPH(p, PHEU_x); // x -> S
			break;

		case 'y': // como la j
			p3 = PREV(p);  ch3 = p3 ? CH(p3) : 0;

/********************"y" azkeneko garfia bada y=>[i]*************************/
			if(!u.charIsFirst(p, URANGE_PAUSE)&&!u.charIsFirst(p3, URANGE_WORD)&&!(strchr(aeiou,ch3)||ch3=='h'))
			{		
				p5 = PREV(p3);  ch5 = p5 ? CH(p5) : 0;
				if(!(strchr(aeiou,ch5)||ch5=='h'))
				{					
					SETPH(p, PHEU_i); 
					break;
				}
			}	
			if(u.charIsLast(p, URANGE_PAUSE)&&!u.charIsFirst(p, URANGE_WORD))
			{				
				SETPH(p, PHEU_i); 
				break;
			}			

/**********************************************************************/
								
			//en comienzo absoluto ¢ seguida de n,l,s,z se pronuncia dj			
			if(u.charIsFirst(p, URANGE_PAUSE) || strchr(nlsz,ch3)|| strchr(tk,ch3))
			{
				//fprintf(stderr, "zein daaa nlsztk eta bakarra\n");
				SETPH(p, PHEU_dj); // y -> dj
				break;
			}
			//en el resto de los casos se pronuncia jj.


			else
			{
				SETPH(p, PHEU_jj); // y -> jj
				break;
			}
		case 'z':
			if(trans_fonet_salb_z_0_t(u,u.wordThis(p, URANGE_PAUSE))){//INAKI salbuespena z->T (ad. Zaragoza)
				SETPH(p, PHEU_T); // z -> T
				break;
			}
			/************ caso tz **************/
			p3 = PREV(p);  ch3 = p3 ? CH(p3) : 0;
			//si el caracter anterior es 't', se pronuncia tZ excepto al final de
			//palabra si la siguiente comienza por consonante que se pierde la t.
			//ej: bihotz taupadak---->bioztaupaDak
			if(ch3=='t' && !(u.charIsFirst(p, URANGE_WORD))) // anterior t
			{
				if(u.charIsLast(p, URANGE_WORD)&&!(u.wordIsLast(p, URANGE_PAUSE))&& (!phtsimple)&&strcmp(pht_speaker,"Karolina")&& strcmp(pht_speaker,"Amaia") ) // no es la ultima palabra
				{
					wo=u.wordNext(p, URANGE_PAUSE);
					hitza = u.cell(wo).getWord();
					ch3=hitza[0];
					if(ch3!='h' && !strchr(aeiou,ch3))
					{
						 SETPH(p, PHEU_z); // tz + C -> z + C
						 break;
					}
				}
				//asignamos el fonema al primer caracter de la digraf¡a(la t)
				SETPH(p3, PHEU_tZ); // tz -> tZ
				break;
			}
			/*************** caso z ********************/
			//si la z no va tras t,y es la £ltima de palabra en interior de frase
			//comprobamos si se trata de 'ez'

/********************z+z Salbuespen bat baldin bada, orduan z-> [z]**********************************/
/*
			if(es_excepcion(u,p,'z'))
			{				
				SETPH(p, PHEU_z); // zz->z
				p2=DEL(p2);
				break;
			}			
*/
/*********************************************************************************************/

			if(!(u.wordIsLast(p, URANGE_PAUSE)) && u.charIsLast(p,URANGE_WORD)&&(!phtsimple))
			{
				wo=u.wordThis(p, URANGE_PAUSE);
				hitza = u.cell(wo).getWord(); // palabra actual
				if(!strcmp(hitza,"ez"))
				{
					wo=u.wordNext(p, URANGE_PAUSE);
					hitza = u.cell(wo).getWord();
					//si "ez"va seguido de palabra que empieza por 'n'
					if(hitza[0]=='n' && strcmp(pht_speaker,"Amaia"))
						//miro si se trata de verbo auxiliar o sintetico
						if(es_verbo_trn(u,wo)||es_verbo_lgn(u,wo)) break;
						 //en caso afirmativo se omite la z:
						 // 'ez' + [n]adz lagn    z -> [/]

					//si "ez" va seguido de palabra que empieza por 'l'
					if(hitza[0]=='l' && strcmp(pht_speaker,"Amaia"))
						//miro si se trata de verbo auxiliar o sintetico
						if(es_verbo_trn(u,wo)||es_verbo_lgn(u,wo)) break;
						//en caso afirmativo se omite la z:
						 // 'ez' + [l]adz lagn    z -> [/]

					//si "ez"va seguido de palabra que empieza por 'z'
					if(hitza[0]=='z' && strcmp(pht_speaker,"Amaia"))
					//miro si se trata de verbo auxiliar o sintetico
						if(es_verbo_trn(u,wo)||es_verbo_lgn(u,wo))
						{//en caso afirmativo la z se convierte en tz
							//y nos saltamos la siguiente z
							SETPH(p2, PHEU_tZ); // z -> z
							p2 = DEL(p2); // 'ez' + [z]adz lagn    z + z ->[/]+[tz]
							break;
						}
				}
			}
			//en el resto de los casos pronunciamos z
			SETPH(p, PHEU_z); // z -> z
			break;

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

VOID LangEU_PhTrans::iu2jw(UttPh & u)
{ /*
	UttI p;

	for (p=u.phoneFirst(); p!=0; p=u.phoneNext(p)) {
		Phone ph = u.cell(p).getPhone();

		if (((ph == PHEU_i) || (ph == PHEU_u))
				&& (u.cell(p).getStress() == USTRESS_NONE)) {	// solo si es vocal debil no acentuada

			UttI pl = u.phonePrev(p, URANGE_SYLLABLE);
			UttI pr = u.phoneNext(p, URANGE_SYLLABLE);

			Phone phl = pl ? u.cell(pl).getPhone() : 0;		// fonema anterior en silaba

			Phone phr = pr ? u.cell(pr).getPhone() : 0;		// fonema siguiente en silaba

			if (LangEU_PhUti::phIsVowel(phl) || LangEU_PhUti::phIsVowel(phr))	// si hay vocal a izqda. o dcha. de vocal debil -> aproximante

				{} //$$$$$$$				u.cell(p).setPhone((ph == PHEU_i) ? PHEU_iaprox : PHEU_uaprox);

		}
	}*/
}

/**********************************************************/
