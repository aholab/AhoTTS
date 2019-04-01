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

Nombre fuente................ es_categ.cpp
Nombre paquete............... aHoTTS
Lenguaje fuente.............. C++
Estado....................... -
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. inigos
.............................

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
1.0.0    22/03/04  inigos    Recopia adaptación desde euskara

======================== Contenido ========================
<DOC>
Metodos de {{LangES_Categ}} que implementan las diferentes
funciones de la clase de Categorización.
</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include "es_lingp.hpp"
#include "httsmsg.h"
#include "tnor.h"

//#define DEBUG() fprintf (stderr,"file..: %s -- line..: %d\n",__FILE__,__LINE__);

#define DEBUGPOSx

int mirar_etiq (int etiqueta, int i, etiquetas *pos);

/**********************************************************/

LangES_Categ::LangES_Categ( VOID )
{
	created=FALSE;
}

/**********************************************************/

LangES_Categ::~LangES_Categ( VOID )
{
}


/**********************************************************/

BOOL LangES_Categ::create( VOID )
{
	created=TRUE;
	return TRUE;
}

/**********************************************************/

BOOL LangES_Categ::set( const CHAR *param, const CHAR * value )
{
	if (!strcmp(param,"HDicDB")&& !created) { dbName=value; return TRUE; }
	else return FALSE;
}

/**********************************************************/

const CHAR *LangES_Categ::get( const CHAR *param )
{
#define VALRET(x) { sprintf(buf,"%g",(double)x); buf_get=buf; return buf_get; }
	if (!strcmp(param,"HDicDB")) return dbName;

	return FALSE;
}

/**********************************************************/

VOID LangES_Categ::utt_categ( UttWS & u )
{
	UttI p=NULL;		//indice para recorrer las celdas
	//caracteristicas de palabras
	char word_act[MAX_TAM_WORD]="\0";
	int len_string1=0;
	int numWord=0;	//numero de palabra en la phrase (entre signos puntuac.)

	HDicRef hDicRef;
	char ref[MAX_TAM_WORD]="\0";
	int num_eq=0,found=0;
	int num_palab=0, i=0;
	
	for(p=u.wordFirst(); p!=NULL; p=u.wordNext(p))
	{
		num_palab++;
	}	
	 

	etiquetas *pos;	
	pos=new etiquetas[num_palab];

	//---------------------------------------------------------------
	//  Recorrido palabra por palabra de cada frase, de cada u.      
	//---------------------------------------------------------------
	for(p=u.wordFirst(); p!=NULL; p=u.wordNext(p))
	{
		strcpy(word_act,u.cell(p).getWord());
		
		//================================================================
		//  Analisis POS: Part Of Speech                                  
		//================================================================

		u.cell(p).setPOS(POS_ES_NONE);
		hDicRef=u.cell(p).getHDicRef();
		strcpy(ref,u.getHDicDB()->hDicRefToTxt(hDicRef));
		num_eq=u.getHDicDB()->query(hDicRef,HDIC_QUERY_MATCHLEN);
		len_string1=strlen(word_act);
		
		
		
		pos[i].pos1=0,pos[i].pos2=0,pos[i].pos3=0,pos[i].pos4=0,pos[i].pos5=0,pos[i].contador=0;

		//Yon2. !!!!! Solo vale num_eq si hDicRef != 0

		if ((!num_eq) && (hDicRef != HDIC_REF_NULL))
		{ 
			// asignar POS a palabra encontrada en diccionario
			//printf("%s\n",(const char*)word_act);
			if (posdic(u,p,i,pos)){
			//	printf("ha encontrado la palabra en el diccionario %s\n",(const char*)word_act);				
				i++;			
				continue;
			}
		}
				
		// miramos si es un adverbio
		if(es_xxmente( u, p)){
			i++;
			continue;
		}

		// ¿es un verbo?
		if(es_verbo(u, p, word_act,i,pos)){
			i++;
			continue;
		}
		
		// ¿es un verbo enclítico?
		if(	es_enclitico(u, p,i ,pos))
		{			
			u.cell(p).setPOS(POS_ES_VERB_ENCL);
			i++;
			continue;
		}
		else{
			i++;
			continue;
		}
		
	}		
	
	
	desambiguar(u,pos);	
	
	
}

// ***************
// * desambiguar()

INT LangES_Categ::desambiguar(UttWS & u, etiquetas *pos)
{
	UttI p=NULL;		//indice para recorrer las celdas
	int noverbo=1, tam;
	char word_act[MAX_TAM_WORD]="\0";
	char word_prev[MAX_TAM_WORD]="\0";
	int i=0;
	// Segundo recorrido para desambiguar
	// noverbo:  0 = no pre verbo o preposición; 1 = indiferente; 2 = pre verbo; 3 = no post verbo 4 = verbo

	for(p=u.wordFirst(); p!=NULL; p=u.wordNext(p))
	{		
		if (mirar_etiq(8,i,pos)==1 || mirar_etiq(200,i,pos)==1 || mirar_etiq(800,i,pos)==1 || mirar_etiq(20,i,pos)==1){ //POS_ES_DEMO_NOVERB, POS_ES_INDNUM_NOVERB, POS_ES_CARD_NOVERB, POS_ES_POSE_NOVERB
			noverbo = 0;
		}
		else if( mirar_etiq(4000,i,pos)==1){//POS_ES_PREP
			if (noverbo == 0)
			{
				u.cell(p).setPOS(POS_ES_NONE);
			}
		}
		else if (mirar_etiq(0,i,pos)==1 || mirar_etiq(2,i,pos)==1 || mirar_etiq(10,i,pos)==1 || mirar_etiq(40,i,pos)==1 || mirar_etiq(80,i,pos)==1 || mirar_etiq(100,i,pos)==1 || mirar_etiq(400,i,pos)==1 || mirar_etiq(1000,i,pos)==1 || mirar_etiq(2000,i,pos)==1 || mirar_etiq(8000,i,pos)==1 || mirar_etiq(2000,i,pos)==1){
			//POS_ES_NONE,POS_ES_SUSP_IND,POS_ES_DEMO_IND,POS_ES_POSE_IND,POS_ES_RELA, POS_ES_RELIE,POS_ES_INDNUM_IND,POS_ES_CARD_IND,POS_ES_ADVE,POS_ES_CONJ,POS_ES_OTRO_SUST
			noverbo = 1;
		}
		else if( mirar_etiq(10001,i,pos)==1){//POS_ES_VERB_SIMP_DEBIL
			if (noverbo == 2)
			{
					u.cell(p).setPOS(POS_ES_VERB_SIMP);
					noverbo = 1;
			}
		}
		else if (mirar_etiq(10000,i,pos)==1 || mirar_etiq(10010,i,pos)==1 || mirar_etiq(10020,i,pos)==1 || mirar_etiq(10040,i,pos)==1 || mirar_etiq(10100,i,pos)==1 || mirar_etiq(10200,i,pos)==1 || mirar_etiq(10400,i,pos)==1 || mirar_etiq(10800,i,pos)==1){
			//POS_ES_VERB_SIMP,POS_ES_VERB_COMP,POS_ES_VERB_GERU,POS_ES_VERB_PPIO,POS_ES_VERB_IR1C,POS_ES_VERB_IR2C,POS_ES_VERB_IR3C,POS_ES_VERB_IICC
			if (noverbo == 0)
			{
				u.cell(p).setPOS(POS_ES_NONE);
				noverbo=4;
			}
		}
		else if( mirar_etiq(1,i,pos)==1){//POS_ES_SUSP_VERB		
			noverbo = 2;
		}	
		else if( mirar_etiq(4,i,pos)==1){//POS_ES_SUSP_NPV	
			noverbo = 3;	
		}
		else{
			noverbo = 1;
		}
			
		i++;	
		
	}		
		// switch(u.cell(p).getPOS())
		// {
		// case POS_ES_DEMO_NOVERB:
		// case POS_ES_INDNUM_NOVERB:
		// case POS_ES_CARD_NOVERB:
		// case POS_ES_POSE_NOVERB:
				// noverbo = 0;
				// break;
		// case POS_ES_PREP:
			// if (noverbo == 0)
			// {
				// u.cell(p).setPOS(POS_ES_NONE);
			// }
		// case POS_ES_NONE:
		// case POS_ES_SUSP_IND:
		// case POS_ES_DEMO_IND:
		// case POS_ES_POSE_IND:
		// case POS_ES_RELA:
		// case POS_ES_RELIE:
		// case POS_ES_INDNUM_IND:
		// case POS_ES_CARD_IND:
		// case POS_ES_ADVE:
		// case POS_ES_CONJ:
		// case POS_ES_OTRO_SUST:
				// noverbo = 1;
				// break;
		// case POS_ES_VERB_SIMP_DEBIL:
			// if (noverbo == 2)
			// {
				// u.cell(p).setPOS(POS_ES_VERB_SIMP);
				// noverbo = 1;
			// }
		// case POS_ES_VERB_SIMP:
		// case POS_ES_VERB_COMP:
		// case POS_ES_VERB_GERU:
		// case POS_ES_VERB_PPIO:
		// case POS_ES_VERB_IR1C:
		// case POS_ES_VERB_IR2C:
		// case POS_ES_VERB_IR3C:
		// case POS_ES_VERB_IICC:
			// if (noverbo == 0)
			// {
				// u.cell(p).setPOS(POS_ES_NONE);
				// noverbo=4;
			// }
			// break;
		// case POS_ES_SUSP_VERB:
				// noverbo = 2;
				// break;
		// case POS_ES_SUSP_NPV:
				// noverbo = 3;
				// break;
		// default:
				// noverbo = 1;
			// break;
		// }

	//}


	// Miramos los verbos débiles (aquellos que pueden coincidir con sustantivos)
	// (por ejemplo: prueba, cazo, mente...)

	i=0;
	for(p=u.wordFirst(); p!=NULL; p=u.wordNext(p))
	{
		strcpy(word_act,u.cell(p).getWord());		
			if (mirar_etiq(10001,i,pos)==1){ //POS_ES_VERB_SIMP_DEBIL
				if(u.wordPrev(p) != NULL)
				{
					tam=strlen(word_act);
					switch(word_act[tam-1]) // miramos la terminación de la palabra
					{
					case 'a':
						if (p != u.wordFirst())
						{
							strcpy(word_prev,u.cell(u.wordPrev(p)).getWord());
							
							if (mirar_etiq(2,i-1,pos)){ //POS_ES_SUSP_IND
							// si no es "la" es verbo ("_lo prueba")
								// si es la puede serlo ("_la prueba")
								if(strcmp("la", word_prev)) 
								{
									u.cell(p).setPOS(POS_ES_VERB_SIMP);
									u.cell(u.wordPrev(p)).setPOS(POS_ES_SUSP_VERB);
								}
							}
							else{
								if (mirar_etiq(40,i-1,pos)){ //POS_ES_POSE_IND
									// si no es "nuestra" o "vuestra" es verbo ("con lo _tuyo prueba")
									// si es "nuestra" o "vuestra" puede serlo ("_nuestra prueba")
									if(strcmp("nuestra", word_prev) || strcmp("vuestra", word_prev)) 
									{
										u.cell(p).setPOS(POS_ES_VERB_SIMP);
										u.cell(u.wordPrev(p)).setPOS(POS_ES_SUSP_VERB);
									}
								}
								else{
									if (mirar_etiq(400,i-1,pos)){ //POS_ES_INDNUM_IND
										// -a  no desambigua;
										tam=strlen(word_prev);

										if(word_prev[tam-1] != 'a') 
										{
											u.cell(p).setPOS(POS_ES_VERB_SIMP);
											u.cell(u.wordPrev(p)).setPOS(POS_ES_SUSP_VERB);
										}									
									}
									else{
										if (mirar_etiq(1000,i-1,pos)){ //POS_ES_CARD_IND
											// si no es una es verbo
											if(strcmp("una", word_prev)) 
											{
												u.cell(p).setPOS(POS_ES_VERB_SIMP);
												u.cell(u.wordPrev(p)).setPOS(POS_ES_SUSP_VERB);
											}
										}
									}
								}								
							}
						}
						break;
					case 'o':
						if (p != u.wordFirst())
						{
						strcpy(word_prev,u.cell(u.wordPrev(p)).getWord());
													
							if (mirar_etiq(2,i-1,pos)){ //POS_ES_SUSP_IND
									// es verbo (_lo cojo)
									u.cell(p).setPOS(POS_ES_VERB_SIMP);
									u.cell(u.wordPrev(p)).setPOS(POS_ES_SUSP_VERB);	
							}
							else{
								if (mirar_etiq(40,i-1,pos)){ //POS_ES_POSE_IND
									// si no es "nuestro" o "vuestro" es verbo ("con lo _tuyo cazo")
									// si es "nuestro" o "vuestro" puede serlo ("_nuestro cazo")
									if(strcmp("nuestro", word_prev) || strcmp("vuestro", word_prev)) 
									{
										u.cell(p).setPOS(POS_ES_VERB_SIMP);
										u.cell(u.wordPrev(p)).setPOS(POS_ES_SUSP_VERB);
									}
								}
								else{
									if (mirar_etiq(400,i-1,pos)){ //POS_ES_INDNUM_IND
										// con "otro", "todo" y "mismo" no desambigua (_otro juego)
										if(strcmp("otro", word_prev) || strcmp("todo", word_prev)
											|| strcmp("mismo", word_prev)) 
										{
											u.cell(p).setPOS(POS_ES_VERB_SIMP);
											u.cell(u.wordPrev(p)).setPOS(POS_ES_SUSP_VERB);
										}
									}							
								}
							}
						}
						 break;
					case 's':
					if ((word_act[tam-2]=='a') && p != u.wordFirst())
					{
						strcpy(word_prev,u.cell(u.wordPrev(p)).getWord());
						if (mirar_etiq(2,i-1,pos)){ //POS_ES_SUSP_IND
							// si no es "las" es verbo ("_lo pruebas")
							// si es la puede serlo ("_las pruebas")
							if(strcmp("las", word_prev)) 
							{
								u.cell(p).setPOS(POS_ES_VERB_SIMP);
								u.cell(u.wordPrev(p)).setPOS(POS_ES_SUSP_VERB);
							}
						}
						else{
							if (mirar_etiq(40,i-1,pos)){ //POS_ES_POSE_IND						
								// si no es "nuestras" o "vuestras" es verbo ("con lo _tuyo prueba")
								// si es "nuestras" o "vuestras" puede serlo ("_nuestras pruebas")
								if(strcmp("nuestras", word_prev) || strcmp("vuestras", word_prev)) 
								{
									u.cell(p).setPOS(POS_ES_VERB_SIMP);
									u.cell(u.wordPrev(p)).setPOS(POS_ES_SUSP_VERB);
								}
							}
							else{
								if (mirar_etiq(400,i-1,pos)){ //POS_ES_INDNUM_IND							
									// -as  no desambigua;
									tam=strlen(word_prev);

									if(strcmp("as", &word_prev[tam-2])) 
									{
										u.cell(p).setPOS(POS_ES_VERB_SIMP);
										u.cell(u.wordPrev(p)).setPOS(POS_ES_SUSP_VERB);
									}
								}
								else{
									if (mirar_etiq(1000,i-1,pos)){ //POS_ES_CARD_IND						
										// si no es unas es verbo
										if(strcmp("unas", word_prev)) 
										{
											u.cell(p).setPOS(POS_ES_VERB_SIMP);
											u.cell(u.wordPrev(p)).setPOS(POS_ES_SUSP_VERB);
										}
									}
								}								
							}
						}
					}
					break;
				default:
					break;
				}
			}
		}	
		
		// if (u.cell(p).getPOS() == POS_ES_VERB_SIMP_DEBIL)
		// {
			// if(u.wordPrev(p) != NULL)
			// {
				// tam=strlen(word_act);
				// switch(word_act[tam-1]) // miramos la terminación de la palabra
				// {
				// case 'a':
					// if (p != u.wordFirst())
					// {
						// strcpy(word_prev,u.cell(u.wordPrev(p)).getWord());

						// switch (u.cell(u.wordPrev(p)).getPOS())
						// {
							// case POS_ES_SUSP_IND:
								//si no es "la" es verbo ("_lo prueba")
								//si es la puede serlo ("_la prueba")
								// if(strcmp("la", word_prev)) 
								// {
									// u.cell(p).setPOS(POS_ES_VERB_SIMP);
									// u.cell(u.wordPrev(p)).setPOS(POS_ES_SUSP_VERB);
								// }
								// break;
							// case POS_ES_POSE_IND:
								//si no es "nuestra" o "vuestra" es verbo ("con lo _tuyo prueba")
								//si es "nuestra" o "vuestra" puede serlo ("_nuestra prueba")
								// if(strcmp("nuestra", word_prev) || strcmp("vuestra", word_prev)) 
								// {
									// u.cell(p).setPOS(POS_ES_VERB_SIMP);
									// u.cell(u.wordPrev(p)).setPOS(POS_ES_SUSP_VERB);
								// }
								// break;
							// case POS_ES_INDNUM_IND:
								//-a  no desambigua;
								// tam=strlen(word_prev);

								// if(word_prev[tam-1] != 'a') 
								// {
									// u.cell(p).setPOS(POS_ES_VERB_SIMP);
									// u.cell(u.wordPrev(p)).setPOS(POS_ES_SUSP_VERB);
								// }
								// break;
							// case POS_ES_CARD_IND:
								//si no es una es verbo
								// if(strcmp("una", word_prev)) 
								// {
									// u.cell(p).setPOS(POS_ES_VERB_SIMP);
									// u.cell(u.wordPrev(p)).setPOS(POS_ES_SUSP_VERB);
								// }
								// break;
							// default:
								// break;
						// }
					// }
					// break;
				// case 'o':
					// if (p != u.wordFirst())
					// {
						// strcpy(word_prev,u.cell(u.wordPrev(p)).getWord());

						// switch (u.cell(u.wordPrev(p)).getPOS())
						// {
							// case POS_ES_SUSP_IND: // es verbo (_lo cojo)
								// u.cell(p).setPOS(POS_ES_VERB_SIMP);
								// u.cell(u.wordPrev(p)).setPOS(POS_ES_SUSP_VERB);
								// break;
							// case POS_ES_POSE_IND:
								//si no es "nuestro" o "vuestro" es verbo ("con lo _tuyo cazo")
								//si es "nuestro" o "vuestro" puede serlo ("_nuestro cazo")
								// if(strcmp("nuestro", word_prev) || strcmp("vuestro", word_prev)) 
								// {
									// u.cell(p).setPOS(POS_ES_VERB_SIMP);
									// u.cell(u.wordPrev(p)).setPOS(POS_ES_SUSP_VERB);
								// }
								// break;
							// case POS_ES_INDNUM_IND:
								//con "otro", "todo" y "mismo" no desambigua (_otro juego)
								// if(strcmp("otro", word_prev) || strcmp("todo", word_prev)
									// || strcmp("mismo", word_prev)) 
								// {
									// u.cell(p).setPOS(POS_ES_VERB_SIMP);
									// u.cell(u.wordPrev(p)).setPOS(POS_ES_SUSP_VERB);
								// }
								// break;
							// default:
								// break;
						// }
					// }
				// case 's':
					// if ((word_act[tam-2]=='a') && p != u.wordFirst())
					// {
						// strcpy(word_prev,u.cell(u.wordPrev(p)).getWord());

						// switch (u.cell(u.wordPrev(p)).getPOS())
						// {
							// case POS_ES_SUSP_IND:
								//si no es "las" es verbo ("_lo pruebas")
								//si es la puede serlo ("_las pruebas")
								// if(strcmp("las", word_prev)) 
								// {
									// u.cell(p).setPOS(POS_ES_VERB_SIMP);
									// u.cell(u.wordPrev(p)).setPOS(POS_ES_SUSP_VERB);
								// }
								// break;
							// case POS_ES_POSE_IND:
								//si no es "nuestras" o "vuestras" es verbo ("con lo _tuyo prueba")
								//si es "nuestras" o "vuestras" puede serlo ("_nuestras pruebas")
								// if(strcmp("nuestras", word_prev) || strcmp("vuestras", word_prev)) 
								// {
									// u.cell(p).setPOS(POS_ES_VERB_SIMP);
									// u.cell(u.wordPrev(p)).setPOS(POS_ES_SUSP_VERB);
								// }
								// break;
							// case POS_ES_INDNUM_IND:
								//-as  no desambigua;
								// tam=strlen(word_prev);

								// if(strcmp("as", &word_prev[tam-2])) 
								// {
									// u.cell(p).setPOS(POS_ES_VERB_SIMP);
									// u.cell(u.wordPrev(p)).setPOS(POS_ES_SUSP_VERB);
								// }
								// break;
							// case POS_ES_CARD_IND:
								//si no es unas es verbo
								// if(strcmp("unas", word_prev)) 
								// {
									// u.cell(p).setPOS(POS_ES_VERB_SIMP);
									// u.cell(u.wordPrev(p)).setPOS(POS_ES_SUSP_VERB);
								// }
								// break;
							// default:
								// break;
						// }
					// }
					// break;
				// default:
					// break;
				// }
			// }
		// }

		
		
		// Miramos si es un artículo o un sustantivo personal
		// sustantivo personal + verbo

		
		if (mirar_etiq(2,i,pos)) //POS_ES_SUSP_IND
		{
			strcpy(word_act,u.cell(p).getWord());

			if(u.wordNext(p) != NULL)
			{
				if((!strcmp(word_act, "la")) || (!strcmp(word_act, "las")) ||
					(!strcmp(word_act, "los")))
				{
					if (mirar_etiq(10000,i+1,pos)==0 && mirar_etiq(10001,i+1,pos)==0 && mirar_etiq(10010,i+1,pos)==0 && mirar_etiq(10020,i+1,pos)==0 && mirar_etiq(10040,i+1,pos)==0 && mirar_etiq(10100,i+1,pos)==0 && mirar_etiq(10200,i+1,pos)==0 || mirar_etiq(10400,i+1,pos)==0 && mirar_etiq(10800,i+1,pos)==0){
					//POS_ES_VERB_SIMP,POS_ES_VERB_SIMP_DEBIL,POS_ES_VERB_COMP,POS_ES_VERB_GERU,POS_ES_VERB_PPIO,POS_ES_VERB_IR1C,POS_ES_VERB_IR2C,POS_ES_VERB_IR3C,POS_ES_VERB_IICC
					u.cell(p).setPOS(POS_ES_DEMO_NOVERB);
					}
					
				}
			}
		}
		i++;	
	}	
		
		
		// if (u.cell(p).getPOS() == POS_ES_SUSP_IND) 
		// {
			// strcpy(word_act,u.cell(p).getWord());

			// if(u.wordNext(p) != NULL)
			// {
				// if((!strcmp(word_act, "la")) || (!strcmp(word_act, "las")) ||
					// (!strcmp(word_act, "los")))
				// {
					// switch(u.cell(u.wordNext(p)).getPOS())
					// {
					// case POS_ES_VERB_SIMP:
					// case POS_ES_VERB_SIMP_DEBIL:
					// case POS_ES_VERB_COMP:
					// case POS_ES_VERB_GERU:
					// case POS_ES_VERB_PPIO:
					// case POS_ES_VERB_IR1C:
					// case POS_ES_VERB_IR2C:
					// case POS_ES_VERB_IR3C:
					// case POS_ES_VERB_IICC:
						// break;
					// default:
						// u.cell(p).setPOS(POS_ES_DEMO_NOVERB);
						// break;
					// }
				// }
			// }
		// }
		
	//desambiguar en los casos en los que hay mas de una etiqueta	
		//desambiguacion entre verbos,adjetivos y sustantivos 
	
	i=0;
	for(p=u.wordFirst(); p!=NULL; p=u.wordNext(p))
	{		
		if(u.wordPrev(p) != NULL)
			strcpy(word_prev,u.cell(u.wordPrev(p)).getWord());
			
		if (mirar_etiq(10000,i,pos)==1 && mirar_etiq(40000,i,pos)==1){ //POS_ES_VERB_SIMP,POS_ES_ADJ			
				if (mirar_etiq(20000,i,pos)==1){//POS_ES_OTRO_SUST 
				//si una palabra tiene la etiqueta de verbo, adjetivo y sustantivo
					if(u.wordPrev(p) != NULL){
						if (mirar_etiq(8,i-1,pos)==1 || mirar_etiq(200,i-1,pos)==1 || mirar_etiq(800,i-1,pos)==1 ||mirar_etiq(20,i-1,pos)==1 ||  //POS_ES_DEMO_NOVERB ,POS_ES_INDNUM_NOVERB,POS_ES_CARD_NOVERB,POS_ES_POSE_NOVER
							strcmp("un", word_prev) == 0 || strcmp("una", word_prev) == 0 || strcmp("unos", word_prev) == 0 || strcmp("unas", word_prev) == 0 ) {
							//si la palabra anterior es de tipo no pre verbo , o "un, "una, "unos" o "unas",la palabra sera  adjetivo o sustantivo
							if (mirar_etiq(20000,i-1,pos)==1){ //POS_ES_OTRO_SUST
								u.cell(p).setPOS(POS_ES_OTRO_SUST); //si la palabra anterior no es sustantivo, la palabra  sera sustantivo ya que por lo general los adjetivos van detras de los sustantivos
							}	
							else{
								u.cell(p).setPOS(POS_ES_ADJ); //si la palabra anterior es sustantivo, por lo general la siguiente palabra sera adjetivo y no otro sustantivo
							}
						}
						else{
							u.cell(p).setPOS(POS_ES_VERB_SIMP); // si la palabra anterior no es de tipo no pre verbo, podra ser verbo
						}
					}
					else{
						u.cell(p).setPOS(POS_ES_ADJ); //si no hay palabra anterior, dejamos la primera etiqueta que se encuentra, que es adjetivo
					}
				}
				else{ //si solo tiene la etiqueta de verbo y adjetivo (y no la de sustantivo)
					if(u.wordPrev(p) != NULL){
						if (mirar_etiq(8,i-1,pos)==1 || mirar_etiq(200,i-1,pos)==1 || mirar_etiq(800,i-1,pos)==1 ||mirar_etiq(20,i-1,pos)==1 ||  //POS_ES_DEMO_NOVERB ,POS_ES_INDNUM_NOVERB,POS_ES_CARD_NOVERB,POS_ES_POSE_NOVER
							strcmp("un", word_prev) == 0 || strcmp("una", word_prev) == 0 || strcmp("unos", word_prev) == 0 || strcmp("unas", word_prev) == 0 ) {
								//si la palabra anterior es de tipo no pre verbo , o "un, "una, "unos" o "unas",la palabra sera  adjetivo 
							u.cell(p).setPOS(POS_ES_ADJ);
						}
						else{
							u.cell(p).setPOS(POS_ES_VERB_SIMP); //en caso contrario podra ser verbo
						}
					}
					else{
						u.cell(p).setPOS(POS_ES_ADJ); //si no hay palabra anterior, dejamos la primera etiqueta que se encuentra, que es adjetivo
					}
				}
		}		
		else{
				if (mirar_etiq(10000,i,pos)==1 && mirar_etiq(20000,i,pos)==1 && mirar_etiq(40000,i,pos)==0  ){//POS_ES_VERB_SIMP,POS_ES_OTRO_SUST , no POS_ES_ADJ 
				//si solo tiene la etiqueta de verbo y sustantivo
					if(u.wordPrev(p) != NULL){
						if (mirar_etiq(8,i-1,pos)==1 || mirar_etiq(200,i-1,pos)==1 || mirar_etiq(800,i-1,pos)==1 ||mirar_etiq(20,i-1,pos)==1 ||  //POS_ES_DEMO_NOVERB ,POS_ES_INDNUM_NOVERB,POS_ES_CARD_NOVERB,POS_ES_POSE_NOVER
							strcmp("un", word_prev) == 0 || strcmp("una", word_prev) == 0 || strcmp("unos", word_prev) == 0 || strcmp("unas", word_prev) == 0 ) {
								//si la palabra anterior es de tipo no pre verbo , o "un, "una, "unos" o "unas",la palabra sera   sustantivo
							u.cell(p).setPOS(POS_ES_OTRO_SUST);
						}
						else{
							u.cell(p).setPOS(POS_ES_VERB_SIMP); //en caso contrario podra ser verbo
						}						 
					}
					else{
						u.cell(p).setPOS(POS_ES_VERB_SIMP);//si no hay palabra anterior, dejamos la primera etiqueta que se encuentra, que es verbo simple
					}
				}				
						
				else{		
				//desambiguacion entre sustantivos y adjetivos
				if (mirar_etiq(40000,i,pos)==1){//POS_ES_ADJ
					if (mirar_etiq(20000,i,pos)==1){//POS_ES_OTRO_SUST 				
						//si la palabra tiene la etiqueta de sustantivo y adjetivo
						if(u.wordPrev(p) != NULL){
							if (mirar_etiq(20000,i-1,pos)==0 || strcmp("la", word_prev) == 0 || strcmp("las", word_prev) == 0 || strcmp("los", word_prev) == 0 ){// no POS_ES_OTRO_SUST 
								u.cell(p).setPOS(POS_ES_OTRO_SUST); //si la palabra anterior no es sustantivo, (o es "la","las", "lo" que una de sus etiquetas es sustantivo)la palabra  sera sustantivo ya que por lo general los adjetivos van detras de los sustantivos
							}	
							else{
								u.cell(p).setPOS(POS_ES_ADJ); //si la palabra anterior es sustantivo, por lo general la siguiente palabra sera adjetivo y no otro sustantivo
							}
						}
						else{
							u.cell(p).setPOS(POS_ES_ADJ); //si no hay palabra anterior, dejamos la primera etiqueta que se encuentra, que es adjetivo
						}
					}
					else{
						u.cell(p).setPOS(POS_ES_ADJ); 
					}
				}
				else{
					if (mirar_etiq(800,i,pos)==1){//POS_ES_CARD_NOVERB
						if(mirar_etiq(400,i,pos)==1){//POS_ES_INDNUM_IND 							
							if(u.wordNext(p) != NULL){
								if (mirar_etiq(20000,i+1,pos)==1 || mirar_etiq(40000,i+1,pos)==1){//POS_ES_OTRO_SUST , POS_ES_ADJ																
									u.cell(p).setPOS(POS_ES_CARD_NOVERB); //para desambiguar cardinales no pre verbo de indefinidos numerales indiferente (un)
								}
								else{
									u.cell(p).setPOS(POS_ES_INDNUM_IND);
								}
							}
							else{
								u.cell(p).setPOS(POS_ES_INDNUM_IND);
							}
						}
						else{
							u.cell(p).setPOS(POS_ES_CARD_NOVERB);
						}
					}			
					else{
						if (mirar_etiq(1000,i,pos)==1 && mirar_etiq(10000,i,pos)==1 ){//POS_ES_CARD_IND,POS_ES_VERB_SIMP 							
								//si tiene la etiqueta de cardinal indiferente y verbo (una, unas, uno)							
								if(u.wordPrev(p) != NULL){
									if (mirar_etiq(8,i-1,pos)==1 || mirar_etiq(200,i-1,pos)==1 || mirar_etiq(800,i-1,pos)==1 ||mirar_etiq(20,i-1,pos)==1 ){  //POS_ES_DEMO_NOVERB ,POS_ES_INDNUM_NOVERB,POS_ES_CARD_NOVERB,POS_ES_POSE_NOVER					
										u.cell(p).setPOS(POS_ES_CARD_IND); //si la palabra anterior es de tipo no pre verbo, la palabra actual no podra ser verbo, sera cardinal
									}
									else{
										if(u.wordNext(p) != NULL){
											if (mirar_etiq(20000,i+1,pos)==1 || mirar_etiq(40000,i+1,pos)==1){  //POS_ES_OTRO_SUST,POS_ES_ADJ																				
												u.cell(p).setPOS(POS_ES_CARD_IND); //si la siguiente palabra es sustantivo o adjetivo, lo mas probable es que la palabra actual sea cardinal y no verbo
											}
											else{
												u.cell(p).setPOS(POS_ES_VERB_SIMP); //si no se cumplen ninguna de las condiciones anteriores, podra ser verbo
											}
										}
									}
								}						
								else{
									if(u.wordNext(p) != NULL){
										if (mirar_etiq(20000,i+1,pos)==1 || mirar_etiq(40000,i+1,pos)==1){  //POS_ES_OTRO_SUST,POS_ES_ADJ											
											u.cell(p).setPOS(POS_ES_CARD_IND); //si la siguiente palabra es sustantivo o adjetivo, lo mas probable es que la palabra actual sea cardinal y no verbo
										}
										else{
											u.cell(p).setPOS(POS_ES_VERB_SIMP); //si no se cumplen la condicion anterior, podra ser verbo
										}
									}					
								}
						}						
						else{
							if (mirar_etiq(10040,i,pos)==1 && mirar_etiq(20000,i,pos)==1){//POS_ES_VERB_PPIO,POS_ES_OTRO_SUST							
									//para desambiaguar entre participio y sustantivo (por ejemplo, conocido)
									if(u.wordPrev(p) != NULL){
										if (strcmp("la", word_prev)!= 0 && strcmp("las", word_prev)!= 0 && strcmp("los", word_prev)!= 0){//la, las, lo tambien tienen como etiqueta el sustantivo, pero por lo general no tienen esa funcion
											if (mirar_etiq(10000,i-1,pos)==1 || mirar_etiq(10010,i-1,pos)==1 || mirar_etiq(20000,i-1,pos)==1 ){ //POS_ES_VERB_SIMP,POS_ES_VERB_COMP,POS_ES_OTRO_SUST																		
												u.cell(p).setPOS(POS_ES_VERB_PPIO); //si la palabra anterior es de tipo verbo(simple o compuesto "fue conocido" "habia conocido") o sustantivo (el pueblo "conocido") y no es de tipo pre verbo, entonces la palabra actual sera participio
											}
											else{
												u.cell(p).setPOS(POS_ES_OTRO_SUST); //si no se cumple las condiciones anteriores, podra ser sustantivo ("un conocido")
											}
										}
										else{
											u.cell(p).setPOS(POS_ES_OTRO_SUST);
										}
									}
									else{
										u.cell(p).setPOS(POS_ES_VERB_PPIO);//si no hay palabra anterior, por lo general sera participio
									}
							}						
							else{// para el resto de casos se deja la primera etiqueta que se encuentre				
								if (mirar_etiq(1,i,pos)==1){//POS_ES_SUSP_VERB
									u.cell(p).setPOS(POS_ES_SUSP_VERB);
								}
								else if (mirar_etiq(2,i,pos)==1){//POS_ES_SUSP_IND								
									u.cell(p).setPOS(POS_ES_SUSP_IND);
								}	
								else if (mirar_etiq(4,i,pos)==1){//POS_ES_SUSP_NPV
										u.cell(p).setPOS(POS_ES_SUSP_NPV);
								}
								else if (mirar_etiq(8,i,pos)==1){//POS_ES_DEMO_NOVERB
									u.cell(p).setPOS(POS_ES_DEMO_NOVERB);
								}
								else if (mirar_etiq(10,i,pos)==1){//POS_ES_DEMO_IND
									u.cell(p).setPOS(POS_ES_DEMO_IND);
								}
								else if (mirar_etiq(20,i,pos)==1){//POS_ES_POSE_NOVERB
									u.cell(p).setPOS(POS_ES_POSE_NOVERB);
								}
								else if (mirar_etiq(40,i,pos)==1){//POS_ES_POSE_IND
									u.cell(p).setPOS(POS_ES_POSE_IND);
								}
								else if (mirar_etiq(80,i,pos)==1){//POS_ES_RELA
									u.cell(p).setPOS(POS_ES_RELA);
								}
								else if (mirar_etiq(100,i,pos)==1){//POS_ES_RELIE
									u.cell(p).setPOS(POS_ES_RELIE);
								}
								else if (mirar_etiq(200,i,pos)==1){//POS_ES_INDNUM_NOVERB
									u.cell(p).setPOS(POS_ES_INDNUM_NOVERB);
								}
								else if (mirar_etiq(400,i,pos)==1){//POS_ES_INDNUM_IND
									u.cell(p).setPOS(POS_ES_INDNUM_IND);
								}
								else if (mirar_etiq(800,i,pos)==1){//POS_ES_CARD_NOVERB
									u.cell(p).setPOS(POS_ES_CARD_NOVERB);
								}
								else if (mirar_etiq(1000,i,pos)==1){//POS_ES_CARD_IND
									u.cell(p).setPOS(POS_ES_CARD_IND);
								}
								else if (mirar_etiq(2000,i,pos)==1){//POS_ES_ADVE
									u.cell(p).setPOS(POS_ES_ADVE);
								}
								else if (mirar_etiq(4000,i,pos)==1){//POS_ES_PREP
									u.cell(p).setPOS(POS_ES_PREP);
								}
								else if (mirar_etiq(8000,i,pos)==1){//POS_ES_CONJ
									u.cell(p).setPOS(POS_ES_CONJ);
								}
								else if (mirar_etiq(10000,i,pos)==1){//POS_ES_VERB_SIMP
									u.cell(p).setPOS(POS_ES_VERB_SIMP);
								}
								else if (mirar_etiq(10001,i,pos)==1){//POS_ES_VERB_SIMP_DEBIL
									u.cell(p).setPOS(POS_ES_VERB_SIMP_DEBIL);
								}
								else if (mirar_etiq(10010,i,pos)==1){//POS_ES_VERB_COMP
									u.cell(p).setPOS(POS_ES_VERB_COMP);
								}
								else if (mirar_etiq(10020,i,pos)==1){//POS_ES_VERB_GERU
									u.cell(p).setPOS(POS_ES_VERB_GERU);
								}	
								else if (mirar_etiq(10040,i,pos)==1){//POS_ES_VERB_PPIO
									u.cell(p).setPOS(POS_ES_VERB_PPIO);
								}	
								else if (mirar_etiq(10100,i,pos)==1){//POS_ES_VERB_IR1C
									u.cell(p).setPOS(POS_ES_VERB_IR1C);
								}													
								else if (mirar_etiq(10200,i,pos)==1){//POS_ES_VERB_IR2C
									u.cell(p).setPOS(POS_ES_VERB_IR2C);
								}													
								else if (mirar_etiq(10400,i,pos)==1){//POS_ES_VERB_IR3C
									u.cell(p).setPOS(POS_ES_VERB_IR3C);
								}														
								else if (mirar_etiq(10800,i,pos)==1){//POS_ES_VERB_IICC
									u.cell(p).setPOS(POS_ES_VERB_IICC);
								}																				
								else if (mirar_etiq(20000,i,pos)==1){//POS_ES_OTRO_SUST
									u.cell(p).setPOS(POS_ES_OTRO_SUST);
								}																	
								else if (mirar_etiq(40000,i,pos)==1){//POS_ES_ADJ
									u.cell(p).setPOS(POS_ES_ADJ);
								}																
																															
							}																											
						}
					}
				}
			}
		}
		i++;
	}
																								
		
	// for(p=u.wordFirst(); p!=NULL; p=u.wordNext(p))
	// {
		// if(u.wordPrev(p) != NULL)
			// strcpy(word_prev,u.cell(u.wordPrev(p)).getWord());
			
		// if (u.cell(p).queryPOS(POS_ES_VERB_SIMP) == 1 ){		
			// if (u.cell(p).queryPOS(POS_ES_ADJ) == 1 ){
				// if (u.cell(p).queryPOS(POS_ES_OTRO_SUST) == 1 ){ //si una palabra tiene la etiqueta de verbo, adjetivo y sustantivo
					// if(u.wordPrev(p) != NULL){
						// if (u.cell(u.wordPrev(p)).queryPOS(POS_ES_DEMO_NOVERB) == 1  || u.cell(u.wordPrev(p)).queryPOS(POS_ES_INDNUM_NOVERB) == 1 || u.cell(u.wordPrev(p)).queryPOS(POS_ES_CARD_NOVERB) == 1  || u.cell(u.wordPrev(p)).queryPOS(POS_ES_POSE_NOVERB) == 1 || strcmp("un", word_prev) == 0 || strcmp("una", word_prev) == 0 || strcmp("unos", word_prev) == 0 || strcmp("unas", word_prev) == 0 ) {
							//si la palabra anterior es de tipo no pre verbo , la palabra sera  adjetivo o sustantivo
							// if (u.cell(u.wordPrev(p)).queryPOS(POS_ES_OTRO_SUST) == 0){
								// u.cell(p).setPOS(POS_ES_OTRO_SUST); //si la palabra anterior no es sustantivo, la palabra  sera sustantivo ya que por lo general los adjetivos van detras de los sustantivos
							// }	
							// else{
								// u.cell(p).setPOS(POS_ES_ADJ); //si la palabra anterior es sustantivo, por lo general la siguiente palabra sera adjetivo y no otro sustantivo
							// }
						// }
						// else{
							// u.cell(p).setPOS(POS_ES_VERB_SIMP); // si la palabra anterior no es de tipo no pre verbo, podra ser verbo
						// }
					// }
					// else{
						// u.cell(p).setPOS(POS_ES_ADJ); //si no hay palabra anterior, dejamos la primera etiqueta que se encuentra, que es adjetivo
					// }
				// }
				// else{ //si solo tiene la etiqueta de verbo y adjetivo (y no la de sustantivo)
					// if(u.wordPrev(p) != NULL){
						// if (u.cell(u.wordPrev(p)).queryPOS(POS_ES_DEMO_NOVERB) == 1  || u.cell(u.wordPrev(p)).queryPOS(POS_ES_INDNUM_NOVERB) == 1 || u.cell(u.wordPrev(p)).queryPOS(POS_ES_CARD_NOVERB) == 1  || u.cell(u.wordPrev(p)).queryPOS(POS_ES_POSE_NOVERB) == 1 || strcmp("un", word_prev) == 0 || strcmp("una", word_prev) == 0 || strcmp("unos", word_prev) == 0 || strcmp("unas", word_prev) == 0 ) {
							//si la palabra anterior es de tipo no pre verbo , la palabra sera  adjetivo 
							// u.cell(p).setPOS(POS_ES_ADJ);
						// }
						// else{
							// u.cell(p).setPOS(POS_ES_VERB_SIMP); //en caso contrario podra ser verbo
						// }
					// }
					// else{
						// u.cell(p).setPOS(POS_ES_ADJ); //si no hay palabra anterior, dejamos la primera etiqueta que se encuentra, que es adjetivo
					// }
				// }
			// }
			// else{
				// if (u.cell(p).queryPOS(POS_ES_OTRO_SUST) == 1 ){ //si solo tiene la etiqueta de verbo y sustantivo
					// if(u.wordPrev(p) != NULL){
						// if (u.cell(u.wordPrev(p)).queryPOS(POS_ES_DEMO_NOVERB) == 1  || u.cell(u.wordPrev(p)).queryPOS(POS_ES_INDNUM_NOVERB) == 1 || u.cell(u.wordPrev(p)).queryPOS(POS_ES_CARD_NOVERB) == 1  || u.cell(u.wordPrev(p)).queryPOS(POS_ES_POSE_NOVERB) == 1 || strcmp("un", word_prev) == 0 || strcmp("una", word_prev) == 0 || strcmp("unos", word_prev) == 0 || strcmp("unas", word_prev) == 0 ) {
							//si la palabra anterior es de tipo no pre verbo, la palabra sera  sustantivo 
							// u.cell(p).setPOS(POS_ES_OTRO_SUST);
						// }
						// else{
							// u.cell(p).setPOS(POS_ES_VERB_SIMP); //en caso contrario podra ser verbo
						// }						 
						// if(strcmp("un", word_prev))
							// u.cell(p).setPOS(POS_ES_OTRO_SUST);
					// }
					// else{
						// u.cell(p).setPOS(POS_ES_VERB_SIMP);//si no hay palabra anterior, dejamos la primera etiqueta que se encuentra, que es verbo simple
					// }
				// }
				// else{ //si tiene la etiqueta de verbo pero no la de sustantivo o adjetivo
					// u.cell(p).setPOS(POS_ES_VERB_SIMP);
				// }
			// }						
		// }	
		// else{		
			//desambiguacion entre sustantivos y adjetivos
			// if (u.cell(p).queryPOS(POS_ES_ADJ) == 1){
				// if (u.cell(p).queryPOS(POS_ES_OTRO_SUST) == 1 ){ //si la palabra tiene la etiqueta de sustantivo y adjetivo
					// if(u.wordPrev(p) != NULL){
						// if (u.cell(u.wordPrev(p)).queryPOS(POS_ES_OTRO_SUST) == 0 ){
							// u.cell(p).setPOS(POS_ES_OTRO_SUST); //si la palabra anterior no es sustantivo, la palabra  sera sustantivo ya que por lo general los adjetivos van detras de los sustantivos
						// }	
						// else{
							// u.cell(p).setPOS(POS_ES_ADJ); //si la palabra anterior es sustantivo, por lo general la siguiente palabra sera adjetivo y no otro sustantivo
						// }
					// }
					// else{
						// u.cell(p).setPOS(POS_ES_ADJ); //si no hay palabra anterior, dejamos la primera etiqueta que se encuentra, que es adjetivo
					// }
				// }
				// else{
					// u.cell(p).setPOS(POS_ES_ADJ); 
				// }
			// }
			// else{
				// if (u.cell(p).queryPOS(POS_ES_CARD_NOVERB) == 1) {
					// if (u.cell(p).queryPOS(POS_ES_INDNUM_IND) == 1 ){
						// if(u.wordNext(p) != NULL){
							// if (u.cell(u.wordNext(p)).queryPOS(POS_ES_OTRO_SUST) == 1 || u.cell(u.wordNext(p)).queryPOS(POS_ES_ADJ) == 1){					
								// u.cell(p).setPOS(POS_ES_CARD_NOVERB); //para desambiguar cardinales no pre verbo de indefinidos numerales indiferente (un)
							// }
							// else{
								// u.cell(p).setPOS(POS_ES_INDNUM_IND);
							// }
						// }
						// else{
							// u.cell(p).setPOS(POS_ES_INDNUM_IND);
						// }
					// }
					// else{
						// u.cell(p).setPOS(POS_ES_CARD_NOVERB);
					// }
				// }			
				// else{
					// if (u.cell(p).queryPOS(POS_ES_CARD_IND) == 1 && u.cell(p).queryPOS(POS_ES_VERB_SIMP) == 1 ){ //si tiene la etiqueta de cardinal indiferente y verbo (una, unas, uno)
						// if(u.wordPrev(p) != NULL){
							// if (u.cell(u.wordPrev(p)).queryPOS(POS_ES_DEMO_NOVERB) == 1 || u.cell(u.wordPrev(p)).queryPOS(POS_ES_INDNUM_NOVERB) == 1  || u.cell(u.wordPrev(p)).queryPOS(POS_ES_CARD_NOVERB) == 1  || u.cell(u.wordPrev(p)).queryPOS(POS_ES_POSE_NOVERB) == 1 ){ 
								// u.cell(p).setPOS(POS_ES_CARD_IND); //si la palabra anterior es de tipo no pre verbo, la palabra actual no podra ser verbo, sera cardinal
							// }
							// else{
								// if(u.wordNext(p) != NULL){
									// if (u.cell(u.wordNext(p)).queryPOS(POS_ES_OTRO_SUST) == 1 || u.cell(u.wordNext(p)).queryPOS(POS_ES_ADJ) == 1 ){
										// u.cell(p).setPOS(POS_ES_CARD_IND); //si la siguiente palabra es sustantivo o adjetivo, lo mas probable es que la palabra actual sea cardinal y no verbo
									// }
									// else{
										// u.cell(p).setPOS(POS_ES_VERB_SIMP); //si no se cumplen ninguna de las condiciones anteriores, podra ser verbo
									// }
								// }
							// }
						// }
						// else{
							// if(u.wordNext(p) != NULL){
								// if (u.cell(u.wordNext(p)).queryPOS(POS_ES_OTRO_SUST) == 1 || u.cell(u.wordNext(p)).queryPOS(POS_ES_ADJ) == 1 ){
									// u.cell(p).setPOS(POS_ES_CARD_IND); //si la siguiente palabra es sustantivo o adjetivo, lo mas probable es que la palabra actual sea cardinal y no verbo
								// }
								// else{
									// u.cell(p).setPOS(POS_ES_VERB_SIMP); //si no se cumplen la condicion anterior, podra ser verbo
								// }
							// }					
						// }
					// }
					// else{
						// if(u.cell(p).queryPOS(POS_ES_VERB_PPIO) == 1 && u.cell(p).queryPOS(POS_ES_OTRO_SUST) == 1 ){ //para desambiaguar entre participio y sustantivo (por ejemplo, conocido)
							// if(u.wordPrev(p) != NULL){
								// if (u.cell(u.wordPrev(p)).queryPOS(POS_ES_VERB_SIMP) == 1 || u.cell(u.wordPrev(p)).queryPOS(POS_ES_VERB_COMP) == 1 || u.cell(u.wordPrev(p)).queryPOS(POS_ES_OTRO_SUST) == 1 ){
									// u.cell(p).setPOS(POS_ES_VERB_PPIO); //si la palabra anterior es de tipo verbo(simple o compuesto "fue conocido" "habia conocido") o sustantivo (el pueblo "conocido") y no es de tipo pre verbo, entonces la palabra actual sera participio
								// }
								// else{
									// u.cell(p).setPOS(POS_ES_OTRO_SUST); //si no se cumple las condiciones anteriores, podra ser sustantivo ("un conocido")
								// }
							// }
							// else{
								// u.cell(p).setPOS(POS_ES_VERB_PPIO);//si no hay palabra anterior, por lo general sera participio
							// }
						// }
						// else{// para el resto de casos se deja la primera etiqueta que se encuentre				
							// if (u.cell(p).queryPOS(POS_ES_SUSP_VERB) == 1){
								// u.cell(p).setPOS(POS_ES_SUSP_VERB);
							// }
								
						// }
					// }
				// }
			// }
		// }
	// }		
	
	delete pos;
	return 1;
}

// *****************************************
// * es_verbo()

INT LangES_Categ::es_verbo(UttWS &u, UttI p, CHAR *word_act, int i, etiquetas *pos)
{

	// Para los enclíticos no nos podemos fiar de la detección del módulo anterior

//	LangES_HDicDB db;
//	db.create("es");	//dbName contiene el nombre del diccionario

//	HDicRef hdb = db.search(word_act);

	HDicRef href;

	href = u.getHDicDB()->search(word_act); // Buscamos en el diccionario el verbo

	// Verbos
	switch (u.getHDicDB()->query(href ,HDIC_QUERY_ES_VERB)) 
	{
		case HDIC_QUERY_ES_VERB_SIMP: // Simple (_soy)
			u.cell(p).setPOS(POS_ES_VERB_SIMP);
			return 1;
		case HDIC_QUERY_ES_VERB_COMP: // Compuesto (_he venido)
			u.cell(p).setPOS(POS_ES_VERB_COMP);
			return 1;
		case HDIC_QUERY_ES_VERB_GERU: // GERUndio (_comiendo)
			u.cell(p).setPOS(POS_ES_VERB_GERU);
			return 1;
		case HDIC_QUERY_ES_VERB_IR1C: // Infinitivos Regulares 1ª Conjugación
			u.cell(p).setPOS(POS_ES_VERB_IR1C);
			return 1;
		case HDIC_QUERY_ES_VERB_IR2C: // Infinitivos Regulares 2ª Conjugación
			u.cell(p).setPOS(POS_ES_VERB_IR2C);
			return 1;
		case HDIC_QUERY_ES_VERB_IR3C: // Infinitivos Regulares 1ª Conjugación
			u.cell(p).setPOS(POS_ES_VERB_IR3C);
			return 1;
		case HDIC_QUERY_ES_VERB_IICC: // Infinitivos Regulares 1ª Conjugación
			u.cell(p).setPOS(POS_ES_VERB_IICC);
			return 1;
	}

	if(es_no_personal(u, p))	return 1;
	if(es_xxeer(u, p))	return 1;
	if(es_nli(u, p))	return 1;
	if(es_xxponer(u, p))	return 1;
	if(es_xxhacer(u, p))	return 1;
	if(es_xxvaler(u, p))	return 1;
	if(es_xxtener(u, p))	return 1;
	if(es_xxtraer(u, p))	return 1;
	if(es_xxdecir(u, p))	return 1;
	if(es_xxsalir(u, p))	return 1;
	if(es_xxvenir(u, p))	return 1;
	if(es_xxducir(u, p))	return 1;

	if(puede_ser_verbo(u,p,word_act,i,pos))	return 1;

	return 0;
}

// ***********************************************************
INT LangES_Categ::es_enclitico(UttWS &u, UttI p,int i, etiquetas *pos)
{
	CHAR word_act[MAX_TAM_WORD]="\0";
	CHAR raiz[MAX_TAM_WORD]="\0";
	INT tam=0;
	pCHAR word_orig;

	strcpy(word_act,u.cell(p).getWord());
	word_orig = strdup(word_act);

	// Hay que quitar los acentos, si los tubiera, a word_act para los casos súpolo, dícese....

	quitar_acentos(word_act);

	strcpy(raiz, word_act);

	tam = strlen(word_act);

	if(tam > 3)
	{
		if(!strcmp("la", &word_act[tam-2]) || !strcmp("lo", &word_act[tam-2]) ||
			!strcmp("le", &word_act[tam-2]))
		{
			if(tam >5)
			{
				// ###################
				// #  -nosla -noslo  #
				// ###################

				strcpy(raiz, word_act);
				raiz[tam-2]='\0';

				if(!strcmp("nos", &raiz[tam-5]))
				{
					raiz[tam-5]='\0';
					u.cell(p).setWord(raiz);
					if(es_verbo(u, p, raiz,i,pos))
					{
						u.cell(p).setWord(word_orig);
						free(word_orig);
						return 1;
					}
				}
			}

			// ####################
			// #  -la   -lo   -le #
			// ####################

			strcpy(raiz, word_act);
			raiz[tam-2]='\0';
			u.cell(p).setWord(raiz);
			if(es_verbo(u, p, raiz,i,pos)) 
			{
				u.cell(p).setWord(word_orig);
				free(word_orig);
				return 1;
			}


			// #############################
			// #  -mela -tela -sela -osla  #
			// #  -melo -telo -selo -oslo  #
			// #############################

			if(!strcmp("me", &raiz[tam-4]) || !strcmp("te", &raiz[tam-4]) ||
				!strcmp("se", &raiz[tam-4]) || !strcmp("os", &raiz[tam-4]))
			{
				strcpy(raiz, word_act);
				raiz[tam-4]='\0';
				u.cell(p).setWord(raiz);
				if(es_verbo(u, p, raiz,i,pos))
				{
					u.cell(p).setWord(word_orig);
					free(word_orig);
					return 1;
				}
			}
		}

		if(!strcmp("las", &word_act[tam-3]) || !strcmp("los", &word_act[tam-3]) ||
			!strcmp("les", &word_act[tam-3]))
		{
			// #######################
			// #  -las   -los   -les #
			// #######################

			strcpy(raiz, word_act);
			raiz[tam-3]='\0';
			u.cell(p).setWord(raiz);
			if(es_verbo(u, p, raiz,i,pos))
			{
				u.cell(p).setWord(word_orig);
				free(word_orig);
				return 1;
			}
			
			if(tam >6)
			{
				if(!strcmp("nos", &raiz[tam-6]) )
				{

				// #####################
				// #  -noslas -noslos  #
				// #####################

					strcpy(raiz, word_act);
					raiz[tam-6]='\0';
					u.cell(p).setWord(raiz);
					if(es_verbo(u, p, raiz,i,pos)) 
					{
						u.cell(p).setWord(word_orig);
						free(word_orig);
						return 1;
					}
				}
				if(!strcmp("me", &raiz[tam-5]) || !strcmp("te", &raiz[tam-5]) ||
					!strcmp("se", &raiz[tam-5]) || !strcmp("os", &raiz[tam-5]))
				{

					// ###################
					// #  -melas -melos  #
					// #  -telas -telos  #
					// #  -selas -selos  #
					// #  -oslas -oslos  #
					// ###################

					strcpy(raiz, word_act);
					raiz[tam-5]='\0';
					u.cell(p).setWord(raiz);
					if(es_verbo(u, p, raiz,i,pos))
					{
						u.cell(p).setWord(word_orig);
						free(word_orig);
						return 1;
					}
				}
			}
		}

		// ##########
		// #  -nos  #
		// ##########

		if(!strcmp("nos", &raiz[tam-3]) )
		{
			strcpy(raiz, word_act);
			raiz[tam-3]='\0';
			u.cell(p).setWord(raiz);
			if(es_verbo(u, p, raiz,i,pos)) 
			{
				u.cell(p).setWord(word_orig);
				free(word_orig);
				return 1;
			}
			if(!strcmp("se", &raiz[tam-5]))
			{
				strcpy(raiz, word_act);
				raiz[tam-5]='\0';
				u.cell(p).setWord(raiz);
				if(es_verbo(u, p, raiz,i,pos)) 
				{
					u.cell(p).setWord(word_orig);
					free(word_orig);
					return 1;
				}
			}

		}
		// ########################
		// #  -me  -te  -se  -os  #
		// ########################

		if(!strcmp("me", &raiz[tam-2]) || !strcmp("te", &raiz[tam-2]) ||
			!strcmp("se", &raiz[tam-2]) || !strcmp("os", &raiz[tam-2]))
		{
			strcpy(raiz, word_act);
			raiz[tam-2]='\0';
			u.cell(p).setWord(raiz);
			if(es_verbo(u, p, raiz,i,pos)) 
			{
				u.cell(p).setWord(word_orig);
				free(word_orig);
				return 1;
			}
		}
	}
	u.cell(p).setWord(word_orig);
	free(word_orig);
	return 0;
}
// ***********************************************************

INT LangES_Categ::posdic(UttWS &u, UttI p,int i, etiquetas *pos)
{
	int etiq=0; 
	
	// Sustantivos Personales
	switch (u.getHDicDB()->query(u.cell(p).getHDicRef(),HDIC_QUERY_ES_SUSP)) 
	{
		case HDIC_QUERY_ES_SUSP_VERB: // pre-post verbo (_me voy; vamonos)
			// u.cell(p).addPOS(POS_ES_SUSP_VERB);
			if (pos[i].contador==0){
				pos[i].pos1=1;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==1){
				pos[i].pos2=1;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==2){
				pos[i].pos3=1;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==3){
				pos[i].pos4=1;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==4){
				pos[i].pos5=1;
				pos[i].contador=pos[i].contador+1;
			}
			etiq=1;
			break;
		case HDIC_QUERY_ES_SUSP_IND: // indiferente (_yo voy; voy _yo)
			//u.cell(p).addPOS(POS_ES_SUSP_IND);
			if (pos[i].contador==0){
				pos[i].pos1=2;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==1){
				pos[i].pos2=2;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==2){
				pos[i].pos3=2;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==3){
				pos[i].pos4=2;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==4){
				pos[i].pos5=2;
				pos[i].contador=pos[i].contador+1;
			}
			etiq=1;
			break;
		case HDIC_QUERY_ES_SUSP_NPV: // no post verbo (a _mí)
			//u.cell(p).addPOS(POS_ES_SUSP_NPV);
			if (pos[i].contador==0){
				pos[i].pos1=4;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==1){
				pos[i].pos2=4;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==2){
				pos[i].pos3=4;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==3){
				pos[i].pos4=4;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==4){
				pos[i].pos5=4;
				pos[i].contador=pos[i].contador+1;
			}
			etiq=1;
			break;
	}

	// Demostrativos
	switch (u.getHDicDB()->query(u.cell(p).getHDicRef(),HDIC_QUERY_ES_DEMO)) 
	{
		case HDIC_QUERY_ES_DEMO_NOVERB: // no pre verbo (_esta casa)
			//u.cell(p).addPOS(POS_ES_DEMO_NOVERB);
			if (pos[i].contador==0){
				pos[i].pos1=8;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==1){
				pos[i].pos2=8;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==2){
				pos[i].pos3=8;
				pos[i].contador=pos[i].contador+1;;
			}
			else if (pos[i].contador==3){
				pos[i].pos4=8;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==4){
				pos[i].pos5=8;
				pos[i].contador=pos[i].contador+1;
			}
			etiq=1;
			break;
		case HDIC_QUERY_ES_DEMO_IND: // indiferente (_esto es; _eso que no sabes)
			//u.cell(p).addPOS(POS_ES_DEMO_IND);
			//printf("etiq=POS_ES_DEMO_IND \n");
			if (pos[i].contador==0){
				pos[i].pos1=10;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==1){
				pos[i].pos2=10;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==2){
				pos[i].pos3=10;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==3){
				pos[i].pos4=10;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==4){
				pos[i].pos5=10;
				pos[i].contador=pos[i].contador+1;
			}
			etiq=1;
			break;
		case HDIC_QUERY_ES_ADJ: // adjetivos
			//u.cell(p).addPOS(POS_ES_ADJ);
			//printf("etiq=POS_ES_ADJ \n");
			if (pos[i].contador==0){
				pos[i].pos1=40000;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==1){
				pos[i].pos2=40000;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==2){
				pos[i].pos3=40000;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==3){
				pos[i].pos4=40000;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==4){
				pos[i].pos5=40000;
				pos[i].contador=pos[i].contador+1;
			}
			etiq=1;
			break;
	}

	// Posesivos
	switch (u.getHDicDB()->query(u.cell(p).getHDicRef(),HDIC_QUERY_ES_POSE)) 
	{
		case HDIC_QUERY_ES_POSE_NOVERB: // no pre verbo (_mi casa)
			//u.cell(p).addPOS(POS_ES_POSE_NOVERB);
			if (pos[i].contador==0){
				pos[i].pos1=20;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==1){
				pos[i].pos2=20;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==2){
				pos[i].pos3=20;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==3){
				pos[i].pos4=20;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==4){
				pos[i].pos5=20;
				pos[i].contador=pos[i].contador+1;;
			}
			etiq=1;
			break;
		case HDIC_QUERY_ES_POSE_IND: // indiferente (es _mío; el _mío es mejor)
			//u.cell(p).addPOS(POS_ES_POSE_IND);
			if (pos[i].contador==0){
				pos[i].pos1=40;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==1){
				pos[i].pos2=40;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==2){
				pos[i].pos3=40;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==3){
				pos[i].pos4=40;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==4){
				pos[i].pos5=40;
				pos[i].contador=pos[i].contador+1;
			}
			etiq=1;
			break;
	}

	// Relativos
	switch (u.getHDicDB()->query(u.cell(p).getHDicRef(),HDIC_QUERY_ES_REL)) 
	{
		case HDIC_QUERY_ES_REL_AT:
			//u.cell(p).addPOS(POS_ES_RELA);
			if (pos[i].contador==0){
				pos[i].pos1=80;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==1){
				pos[i].pos2=80;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==2){
				pos[i].pos3=80;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==3){
				pos[i].pos4=80;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==4){
				pos[i].pos5=80;
				pos[i].contador=pos[i].contador+1;
			}
			etiq=1;
			break;
		case HDIC_QUERY_ES_REL_IE:
			//u.cell(p).addPOS(POS_ES_RELIE);
			if (pos[i].contador==0){
				pos[i].pos1=100;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==1){
				pos[i].pos2=100;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==2){
				pos[i].pos3=100;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==3){
				pos[i].pos4=100;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==4){
				pos[i].pos5=100;
				pos[i].contador=pos[i].contador+1;
			}
			etiq=1;
			break;
	}

	// Indefinidos y Numerales
	switch (u.getHDicDB()->query(u.cell(p).getHDicRef(),HDIC_QUERY_ES_INDNUM)) 
	{
		case HDIC_QUERY_ES_INDNUM_NOVERB: // no pre verbo (_algún comentario)
			//u.cell(p).addPOS(POS_ES_INDNUM_NOVERB);
			if (pos[i].contador==0){
				pos[i].pos1=200;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==1){
				pos[i].pos2=200;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==2){
				pos[i].pos3=200;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==3){
				pos[i].pos4=200;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==4){
				pos[i].pos5=200;
				pos[i].contador=pos[i].contador+1;
			}
			etiq=1;
			break;
		case HDIC_QUERY_ES_INDNUM_IND: // indiferente (no es _nada; _nada es mejor)
			//u.cell(p).addPOS(POS_ES_INDNUM_IND);
			//printf("etiq=POS_ES_INDNUM_IND \n");
			if (pos[i].contador==0){
				pos[i].pos1=400;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==1){
				pos[i].pos2=400;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==2){
				pos[i].pos3=400;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==3){
				pos[i].pos4=400;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==4){
				pos[i].pos5=400;
				pos[i].contador=pos[i].contador+1;
			}
			etiq=1;
			break;
	}

	// Numerales Cardinales
	switch (u.getHDicDB()->query(u.cell(p).getHDicRef(),HDIC_QUERY_ES_CARD)) 
	{
		case HDIC_QUERY_ES_CARD_NOVERB: // no pre verbo (_un comentario)
			//u.cell(p).addPOS(POS_ES_CARD_NOVERB);
			if (pos[i].contador==0){
				pos[i].pos1=800;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==1){
				pos[i].pos2=800;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==2){
				pos[i].pos3=800;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==3){
				pos[i].pos4=800;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==4){
				pos[i].pos5=800;
				pos[i].contador=pos[i].contador+1;
			}
			etiq=1;
			break;
		case HDIC_QUERY_ES_CARD_IND: // indiferente (_dos es par; son _dos de esos)
			//u.cell(p).addPOS(POS_ES_CARD_IND);
			if (pos[i].contador==0){
				pos[i].pos1=1000;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==1){
				pos[i].pos2=1000;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==2){
				pos[i].pos3=1000;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==3){
				pos[i].pos4=1000;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==4){
				pos[i].pos5=1000;
				pos[i].contador=pos[i].contador+1;
			}
			etiq=1;
			break;
	}

	// Advervios
	switch (u.getHDicDB()->query(u.cell(p).getHDicRef(),HDIC_QUERY_ES_ADVE)) 
	{
		case HDIC_QUERY_ES_ADVER: 
			//u.cell(p).addPOS(POS_ES_ADVE);
			if (pos[i].contador==0){
				pos[i].pos1=2000;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==1){
				pos[i].pos2=2000;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==2){
				pos[i].pos3=2000;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==3){
				pos[i].pos4=2000;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==4){
				pos[i].pos5=2000;
				pos[i].contador=pos[i].contador+1;
			}
			etiq=1;
			break;
	}

	// Preposiciones
	switch (u.getHDicDB()->query(u.cell(p).getHDicRef(),HDIC_QUERY_ES_PREP)) 
	{
		case HDIC_QUERY_ES_PREPOS: 
			//u.cell(p).addPOS(POS_ES_PREP);
			if (pos[i].contador==0){
				pos[i].pos1=4000;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==1){
				pos[i].pos2=4000;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==2){
				pos[i].pos3=4000;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==3){
				pos[i].pos4=4000;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==4){
				pos[i].pos5=4000;
				pos[i].contador=pos[i].contador+1;
			}
			etiq=1;
			break;
	}

	// Conjunciones
	switch (u.getHDicDB()->query(u.cell(p).getHDicRef(),HDIC_QUERY_ES_CONJ)) 
	{
		case HDIC_QUERY_ES_CONJUN: 
			//u.cell(p).addPOS(POS_ES_CONJ);
			if (pos[i].contador==0){
				pos[i].pos1=8000;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==1){
				pos[i].pos2=8000;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==2){
				pos[i].pos3=8000;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==3){
				pos[i].pos4=8000;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==4){
				pos[i].pos5=8000;
				pos[i].contador=pos[i].contador+1;
			}
			etiq=1;
			break;
	}
	
	// Verbos
	switch (u.getHDicDB()->query(u.cell(p).getHDicRef(),HDIC_QUERY_ES_VERB)) 
	{
		case HDIC_QUERY_ES_VERB_SIMP: // Simple (_soy)
			//u.cell(p).addPOS(POS_ES_VERB_SIMP);
			//printf("etiq=POS_ES_VERB_SIMP \n");
			if (pos[i].contador==0){
				pos[i].pos1=10000;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==1){
				pos[i].pos2=10000;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==2){
				pos[i].pos3=10000;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==3){
				pos[i].pos4=10000;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==4){
				pos[i].pos5=10000;
				pos[i].contador=pos[i].contador+1;
			}
			etiq=1;
			break;
		case HDIC_QUERY_ES_VERB_COMP: // Compuesto (_he venido)
			//u.cell(p).addPOS(POS_ES_VERB_COMP);
			if (pos[i].contador==0){
				pos[i].pos1=10010;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==1){
				pos[i].pos2=10010;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==2){
				pos[i].pos3=10010;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==3){
				pos[i].pos4=10010;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==4){
				pos[i].pos5=10010;
				pos[i].contador=pos[i].contador+1;
			}
			etiq=1;
			break;
		case HDIC_QUERY_ES_VERB_GERU: // GERUndio (_comiendo)
			//u.cell(p).addPOS(POS_ES_VERB_GERU);
			if (pos[i].contador==0){
				pos[i].pos1=10020;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==1){
				pos[i].pos2=10020;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==2){
				pos[i].pos3=10020;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==3){
				pos[i].pos4=10020;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==4){
				pos[i].pos5=10020;
				pos[i].contador=pos[i].contador+1;
			}
			etiq=1;
			break;
		case HDIC_QUERY_ES_VERB_PPIO: // ParticiPIOs (_comido)
			//u.cell(p).addPOS(POS_ES_VERB_PPIO);
			if (pos[i].contador==0){
				pos[i].pos1=10040;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==1){
				pos[i].pos2=10040;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==2){
				pos[i].pos3=10040;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==3){
				pos[i].pos4=10040;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==4){
				pos[i].pos5=10040;
				pos[i].contador=pos[i].contador+1;
			}
			etiq=1;
			break;
		case HDIC_QUERY_ES_VERB_IR1C: // Infinitivos Regulares 1ª Conjugación
			//u.cell(p).addPOS(POS_ES_VERB_IR1C);
			if (pos[i].contador==0){
				pos[i].pos1=10100;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==1){
				pos[i].pos2=10100;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==2){
				pos[i].pos3=10100;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==3){
				pos[i].pos4=10100;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==4){
				pos[i].pos5=10100;
				pos[i].contador=pos[i].contador+1;
			}
			etiq=1;
			break;
		case HDIC_QUERY_ES_VERB_IR2C: // Infinitivos Regulares 2ª Conjugación
			//u.cell(p).addPOS(POS_ES_VERB_IR2C);
			if (pos[i].contador==0){
				pos[i].pos1=10200;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==1){
				pos[i].pos2=10200;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==2){
				pos[i].pos3=10200;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==3){
				pos[i].pos4=10200;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==4){
				pos[i].pos5=10200;
				pos[i].contador=pos[i].contador+1;
			}
			etiq=1;
			break;
		case HDIC_QUERY_ES_VERB_IR3C: // Infinitivos Regulares 1ª Conjugación
			//u.cell(p).addPOS(POS_ES_VERB_IR3C);
			if (pos[i].contador==0){
				pos[i].pos1=10400;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==1){
				pos[i].pos2=10400;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==2){
				pos[i].pos3=10400;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==3){
				pos[i].pos4=10400;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==4){
				pos[i].pos5=10400;
				pos[i].contador=pos[i].contador+1;
			}
			etiq=1;
			break;
	}

	// Otros (sustantivos y adjetivos para desambiüar verbos)
	switch (u.getHDicDB()->query(u.cell(p).getHDicRef(),HDIC_QUERY_ES_OTRO)) 
	{
		case HDIC_QUERY_ES_OTRO_SUST: 
			//u.cell(p).addPOS(POS_ES_OTRO_SUST);
			//printf("etiq=POS_ES_OTRO_SUST \n");
			if (pos[i].contador==0){
				pos[i].pos1=20000;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==1){
				pos[i].pos2=20000;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==2){
				pos[i].pos3=20000;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==3){
				pos[i].pos4=20000;
				pos[i].contador=pos[i].contador+1;
			}
			else if (pos[i].contador==4){
				pos[i].pos5=20000;
				pos[i].contador=pos[i].contador+1;
			}
			etiq=1;
			break;
	}
	
	i++;
	if (etiq==1) //si tiene una o mas etiquetas de pos
		return 1; 
	else
		return 0;
}

// #########################################################################
// ##  Para desechar aquellas palabras que no pueden ser verbos 
// ## y para marcar los posibles verbos. Aplicar la gramática y
// ## maldecir las irregularidades
// #########################################################################

INT LangES_Categ::puede_ser_verbo(UttWS &u,UttI p,char *word_act,int i,etiquetas *pos)
{
	
	CHAR letra_final = 0;
	INT tam = 0;
	CHAR temporal[MAX_TAM_WORD]="\0";
	
	tam = strlen(word_act);
	letra_final = word_act[tam-1];

	HDicRef hDicRef;

	switch(letra_final)
	{
		case 'a':
			if(tam>4) {
				if (!strcmp("aría", &word_act[tam-4]) ||
					!strcmp("ería", &word_act[tam-4]) ||
					!strcmp("iría", &word_act[tam-4]))
				{
					// ##########################
					// # "aría", "ería", "iría" # // 1ª, 2ª o 3ª
					// ##########################
				
					if(buscar_infinitivo(u, p, word_act,  3, 0, 4)) return 1;
				}
				if(!strcmp("iera", &word_act[tam-4])) {
					// ##########
					// # 'iera' # // 2ª o 3ª
					// ##########
					
					if(buscar_infinitivo(u, p, word_act,  3, 1, 4)) return 1;
				}
			}
			
			if(tam>3){
				if( !strcmp("aba", &word_act[tam-3])||
					!strcmp("ara", &word_act[tam-3])) 
				{
					// ###############
					// # "aba" "ara" # / 1ª
					// ###############

					if(buscar_infinitivo(u, p, word_act,  1, 0, 3)) return 1;
				}
			}
			
			if(tam>2) {
				if(!strcmp("ía", &word_act[tam-2])) // para no salirnos de madre
				{
					// ########
					// # 'ía' # // 2ª o 3ª
					// ########

					if(buscar_infinitivo(u, p, word_act,  3, 1, 2)) return 1;
				}
				
					// #######
					// # 'a' # // 1ª, 2ª o 3ª
					// #######
			}
			if(tam>2) {
				if(buscar_infinitivo(u, p, word_act,  3, 0, 1))
				{
					u.cell(p).setPOS(POS_ES_VERB_SIMP_DEBIL);
					return 1;
				}

				// ********************************************
				// * irregularidades gráficas "finge" "finja" *
				// ********************************************

				if(word_act[tam-2] == 'j'){
					strcpy(temporal, word_act);
					temporal[tam-2] = 'g';
					if(buscar_infinitivo(u, p, temporal,  3, 1, 1))
					{
						//u.cell(p).setPOS(POS_ES_VERB_SIMP_DEBIL);
						pos[i].pos1=10001;//POS_ES_VERB_SIMP_DEBIL
						return 1;
					}
				}
								
				// ****************************************************
				// * irregularidades gráficas "distinga" "distinguir" *
				// ****************************************************

				if(word_act[tam-2] == 'g'){
					strcpy(temporal, word_act);
					temporal[tam-1] = 'u';
					if(buscar_infinitivo(u, p, temporal,  3, 2, 0)) 
					{
						//u.cell(p).setPOS(POS_ES_VERB_SIMP_DEBIL);
						pos[i].pos1=10001;//POS_ES_VERB_SIMP_DEBIL
						return 1;
					}
				}
			}
			break;

		case 'á':
			if(tam>3) {
				if( !strcmp("ará", &word_act[tam-3]) ||
					!strcmp("erá", &word_act[tam-3]) ||
					!strcmp("irá", &word_act[tam-3])) 
				{
					// #######################
					// # "ará", "erá", "irá" # // 1ª, 2ª o 3ª
					// #######################

					if(buscar_infinitivo(u, p, word_act,  3, 0, 3)) return 1;
				}
			}
			break;

		case 'd':
			if(tam>2) {
				if( !strcmp("ad", &word_act[tam-2]) ||
					!strcmp("ed", &word_act[tam-2]) ||
					!strcmp("id", &word_act[tam-2])) 
				{
					// ###################
					// # "ad" "ed" "id"" # // 1ª, 2ª o 3ª
					// ###################
				
					if(buscar_infinitivo(u, p, word_act,  3, 0, 2)) return 1;
				}
			}
			break;

		case 'e':
			if(tam>4) {
				if( !strcmp("iste", &word_act[tam-4]) ||
					!strcmp("iese", &word_act[tam-4]) ||
					!strcmp("iere", &word_act[tam-4])) 
				{
					// ##########################
					// # "iste", "iese", "iere" # // 2ª o 3ª
					// ##########################

					if(buscar_infinitivo(u, p, word_act,  3, 1, 4)) return 1;

				}
				if( !strcmp("aste", &word_act[tam-4]))
				{
					// ##########
					// # "aste" # // 1ª
					// ##########

					if(buscar_infinitivo(u, p, word_act,  1, 0, 4)) return 1;
				}
			}
			
			if(tam>3) {
				if( !strcmp("ase", &word_act[tam-3]) ||
					!strcmp("are", &word_act[tam-3])) 
				{
					// ###############
					// # "ase" "are" # 	// 1ª
					// ###############
				
						
					if(buscar_infinitivo(u, p, word_act,  1, 0, 3)) return 1;
				}
			}
			if(tam>1)
			{
				// #######
				// # 'e' #	// 1ª, 2ª o 3ª
				// #######

				if(buscar_infinitivo(u, p, word_act,  3, 0, 1)) return 1;

				// *********************************************
				// * irregularidades gráficas "cargue" "carga" *
				// *********************************************

				if(!strcmp("gue", &word_act[tam-3])){
					if(buscar_infinitivo(u, p, word_act,  1, 0, 2))
					{
						//u.cell(p).setPOS(POS_ES_VERB_SIMP_DEBIL);
						pos[i].pos1=10001;//POS_ES_VERB_SIMP_DEBIL
						return 1;
					}
				}

				// *******************************************
				// * irregularidades gráficas "goce" "gozar" *
				// *******************************************

				if(!strcmp("oce", &word_act[tam-3]) ||
					!strcmp("ace", &word_act[tam-3])){
					strcpy(temporal, word_act);
					temporal[tam-2] = 'z';
					if(buscar_infinitivo(u, p, temporal,  1, 0, 1))
					{
						//u.cell(p).setPOS(POS_ES_VERB_SIMP_DEBIL);
						pos[i].pos1=10001;//POS_ES_VERB_SIMP_DEBIL
						return 1;
					}
				}
				
				// ************************************************
				// * irregularidades gráficas "aplique" "aplicar" *
				// ************************************************

				if(!strcmp("que", &word_act[tam-3])){
					strcpy(temporal, word_act);
					temporal[tam-3] = 'c';
					if(buscar_infinitivo(u, p, temporal,  1, 0, 2))
					{
						//u.cell(p).setPOS(POS_ES_VERB_SIMP_DEBIL);
						pos[i].pos1=10001;//POS_ES_VERB_SIMP_DEBIL
						return 1;
					}
				}

		
			}
			break;

		case 'é':
			if(tam>3) {
				if( !strcmp("aré", &word_act[tam-3]) ||
					!strcmp("eré", &word_act[tam-3]) ||
					!strcmp("iré", &word_act[tam-3])) 
				{
					// #######################
					// # "aré", "eré", "iré" # // 1ª, 2ª o 3ª
					// #######################
				
					if(buscar_infinitivo(u, p, word_act,  3, 0, 3)) return 1;
				}
			}
					// #######
					// # 'é' # // 1ª
					// #######

			if(tam>1)
			{
				if(buscar_infinitivo(u, p, word_act,  1, 0, 1)) return 1;
								
				// *********************************************
				// * irregularidades gráficas "cargué" "carga" *
				// *********************************************

				if(!strcmp("gué", &word_act[tam-3])){
					if(buscar_infinitivo(u, p, word_act,  1, 0, 2)) return 1;
				}

				// *******************************************
				// * irregularidades gráficas "gocé" "gozar" *
				// *******************************************

				if(!strcmp("océ", &word_act[tam-3]) ||
					!strcmp("acé", &word_act[tam-3])){
					strcpy(temporal, word_act);
					temporal[tam-2] = 'z';
					if(buscar_infinitivo(u, p, temporal,  1, 0, 1)) return 1;
				}
		
				// ************************************************
				// * irregularidades gráficas "apliqué" "aplicar" *
				// ************************************************

				if(!strcmp("qué", &word_act[tam-3])){
					strcpy(temporal, word_act);
					temporal[tam-3] = 'c';
					if(buscar_infinitivo(u, p, temporal,  1, 0, 2)) return 1;
				}
			}
			break;

		case 'í':
					// #######
					// # 'í' # // 2ª o 3ª
					// #######

			if(tam>1) {
				if(buscar_infinitivo(u, p, word_act,  3, 1, 1)) return 1;
			}
			break;

		case 'n':
			if(tam>5) {
				if( !strcmp("ieren", &word_act[tam-5]) ||
					!strcmp("iesen", &word_act[tam-5]) ||
					!strcmp("ieran", &word_act[tam-5]) ||
					!strcmp("ieron", &word_act[tam-5])) 
				{
					// ###################################
					// # "ieren" "iesen" "ieran" "ieron" # // 2ª o 3ª
					// ###################################
				
					if(buscar_infinitivo(u, p, word_act,  3, 1, 5)) return 1;
				}
				
				if( !strcmp("arían", &word_act[tam-5]) ||
					!strcmp("erían", &word_act[tam-5]) ||
					!strcmp("irían", &word_act[tam-5])) 
				{
					// ###########################
					// # "arían" "erían" "irían" # // 1ª, 2ª o 3ª
					// ###########################
				
					if(buscar_infinitivo(u, p, word_act,  3, 0, 5)) return 1;
				}
			}
			if(tam>4) {
				if( !strcmp("aban", &word_act[tam-4]) ||
					!strcmp("aron", &word_act[tam-4]) ||
					!strcmp("arán", &word_act[tam-4]) ||
					!strcmp("aran", &word_act[tam-4]) ||
					!strcmp("asen", &word_act[tam-4]) ||
					!strcmp("aren", &word_act[tam-4]) )
				{
					// ############################################
					// # "aban" "aron" "arán" "aran" "asen" "aren"# // 1ª
					// ############################################
				
					if(buscar_infinitivo(u, p, word_act,  1, 0, 4)) return 1;
				}

				if( !strcmp("erán", &word_act[tam-4]) ||
					!strcmp("irán", &word_act[tam-4]) )
				{
					// #################
					// # "erán" "irán" # // 2ª o 3ª
					// #################
				
					if(buscar_infinitivo(u, p, word_act,  3, 1, 4)) return 1;
				}
			}

			if(tam>3) {
				if( !strcmp("ían", &word_act[tam-3]) ){
					// #########
					// # "ían" # // 2ª o 3ª
					// #########
				
					if(buscar_infinitivo(u, p, word_act,  3, 1, 3)) return 1;
				}
			}

			if(tam>2) {
				if( !strcmp("an", &word_act[tam-2]) ||
					!strcmp("en", &word_act[tam-2]))
				{
					// #############
					// # "an" "en" # // 1ª, 2ª o 3ª
					// #############
				
					if(buscar_infinitivo(u, p, word_act,  3, 0, 2)) return 1;

					// **********************************************
					// * irregularidades gráficas "carguen" "carga" *
					// **********************************************

					if(!strcmp("guen", &word_act[tam-4])){
						if(buscar_infinitivo(u, p, word_act,  1, 0, 3)) return 1;
					}
					
					// *****************************************************
					// * irregularidades gráficas "distingan" "distinguir" *
					// *****************************************************

					if(!strcmp("gan", &word_act[tam-3])){
						strcpy(temporal, word_act);
						temporal[tam-2] = 'u';
						if(buscar_infinitivo(u, p, temporal,  3, 2, 1)) return 1;
					}
						
					// ********************************************
					// * irregularidades gráficas "gocen" "gozar" *
					// ********************************************

					if(!strcmp("ocen", &word_act[tam-4]) ||
						!strcmp("acen", &word_act[tam-4])){
						strcpy(temporal, word_act);
						temporal[tam-3] = 'z';
						if(buscar_infinitivo(u, p, temporal,  1, 0, 2)) return 1;
					}
										
					// *************************************************
					// * irregularidades gráficas "apliquen" "àplicar" *
					// *************************************************

					if(!strcmp("quen", &word_act[tam-4])){
						strcpy(temporal, word_act);
						temporal[tam-4] = 'c';
						if(buscar_infinitivo(u, p, temporal,  1, 0, 3)) return 1;
					}
				}
			}
			break;

		case 'o':
			if(tam>2)
			{
				// ########
				// # "o" # // 1ª, 2ª o 3ª
				// ########

				if(buscar_infinitivo(u, p, word_act,  3, 0, 1))
				{
					//u.cell(p).setPOS(POS_ES_VERB_SIMP_DEBIL);
					pos[i].pos1=10001;//POS_ES_VERB_SIMP_DEBIL
					return 1;
				}

				if(buscar_infinitivo(u, p, word_act,  3, 0, 1))
				{
					//u.cell(p).setPOS(POS_ES_VERB_SIMP_DEBIL);
					pos[i].pos1=10001;//POS_ES_VERB_SIMP_DEBIL
					return 1;
				}

				// ********************************************
				// * irregularidades gráficas "finge" "finjo" *
				// ********************************************

				if(word_act[tam-2] == 'j'){
					strcpy(temporal, word_act);
					temporal[tam-2] = 'g';
					if(buscar_infinitivo(u, p, temporal,  3, 1, 1))
					{
						//u.cell(p).setPOS(POS_ES_VERB_SIMP_DEBIL);
						pos[i].pos1=10001;//POS_ES_VERB_SIMP_DEBIL
						return 1;
					}
				}

				// ****************************************************
				// * irregularidades gráficas "distingo" "distinguir" *
				// ****************************************************

				if(word_act[tam-2] == 'g'){
					strcpy(temporal, word_act);
					temporal[tam-1] = 'u';
					if(buscar_infinitivo(u, p, temporal,  3, 2, 0))
					{
						//u.cell(p).setPOS(POS_ES_VERB_SIMP_DEBIL);
						pos[i].pos1=10001;//POS_ES_VERB_SIMP_DEBIL
						return 1;
					}
				}
			}
			break;

		case 'ó':
			if(tam>2)
			{
				if(!strcmp("ió", &word_act[tam-2]))
				{
					// ########
					// # "ió" # // 2ª o 3ª
					// ########

					if(buscar_infinitivo(u, p, word_act,  3, 1, 2)) return 1;
				}

					// #######
					// # 'ó' # // 1ª
					// #######
			
				if(buscar_infinitivo(u, p, word_act,  1, 0, 1)) return 1;
			}
			break;

		case 's':
			if(tam>7) {
				if( !strcmp("eríamos", &word_act[tam-7]) ||
					!strcmp("iríamos", &word_act[tam-7]) ||
					!strcmp("iéramos", &word_act[tam-7]) ||
					!strcmp("iéremos", &word_act[tam-7]) ||
					!strcmp("iésemos", &word_act[tam-7])) 
				{
					// #####################################################
					// # "eríamos" "iríamos" "iéramos" "iéremos" "iésemos" # // 2ª o 3ª
					// #####################################################
				
					if(buscar_infinitivo(u, p, word_act,  3, 1, 7)) return 1; // metemos uno de sólo 2ª y otro de sólo 3ª
				}

				if( !strcmp("aríamos", &word_act[tam-7]) ) 
				{
					// #############
					// # "aríamos" # // 1ª
					// #############
				
					if(buscar_infinitivo(u, p, word_act,  1, 0, 7)) return 1; 
				}
			}
			if(tam>6) {
				if( !strcmp("ierais", &word_act[tam-6]) ||
					!strcmp("iereis", &word_act[tam-6]) ||
					!strcmp("ieseis", &word_act[tam-6]) ||
					!strcmp("isteis", &word_act[tam-6]) ) 
				{
					// ########################################
					// # "ierais" "iereis" "ieseis" "isteis"  # // 2ª o 3ª
					// ########################################
				
					if(buscar_infinitivo(u, p, word_act,  3, 1, 6)) return 1; // metemos uno de sólo 2ª y otro de sólo 3ª
				}
				
				if( !strcmp("asteis", &word_act[tam-6]) ||
					!strcmp("aríais", &word_act[tam-6]) ||
					!strcmp("ábamos", &word_act[tam-6]) ||
					!strcmp("aremos", &word_act[tam-6]) ||
					!strcmp("áramos", &word_act[tam-6]) ||
					!strcmp("ásemos", &word_act[tam-6]) || 
					!strcmp("áremos", &word_act[tam-6]) ) 
				{
					// ##################################################################
					// # "asteis" "ábamos" "aríais" "aremos" "áramos" "ásemos" "áremos" # // 1ª
					// ##################################################################
				
					if(buscar_infinitivo(u, p, word_act,  1, 0, 6)) return 1; 
				}
				if( !strcmp("eríais", &word_act[tam-6]) ||
					!strcmp("eremos", &word_act[tam-6]) ) 
				{
					// #####################
					// # "eríais" "eremos" # // 2ª
					// #####################
				
					if(buscar_infinitivo(u, p, word_act,  2, 1, 6)) return 1; 
				}
				if( !strcmp("iríais", &word_act[tam-6]) ||
					!strcmp("iremos", &word_act[tam-6]) ) 
				{
					// #####################
					// # "iríais" "iremos" # // 3ª
					// #####################
				
					if(buscar_infinitivo(u, p, word_act,  3, 2, 6)) return 1; 
				}
			}
			if(tam>5) {
				if( !strcmp("abais", &word_act[tam-5]) ||
					!strcmp("arías", &word_act[tam-5]) ||
					!strcmp("areis", &word_act[tam-5]) ||
					!strcmp("arais", &word_act[tam-5]) ||
					!strcmp("aseis", &word_act[tam-5]) ||
					!strcmp("aréis", &word_act[tam-5]) ) 
				{
					// ###################################################
					// # "abais" "arías" "areis" "arais" "aseis" "aréis" # // 1ª
					// ###################################################
				
					if(buscar_infinitivo(u, p, word_act,  1, 0, 5)) return 1; 
				}
				if( !strcmp("ieras", &word_act[tam-5]) ||
					!strcmp("ieres", &word_act[tam-5]) ||
					!strcmp("ieses", &word_act[tam-5]) ||
					!strcmp("íamos", &word_act[tam-5]) ) 
				{
					// ####################################
					// # "ieras" "ieres" "ieses" "íamos"  # // 2ª o 3ª
					// ####################################
				
					if(buscar_infinitivo(u, p, word_act,  3, 1, 5)) return 1; 
				}
				if( !strcmp("erías", &word_act[tam-5]) ||
					!strcmp("eréis", &word_act[tam-5]) ) 
				{
					// ###################
					// # "erías" "eréis" # // 2ª
					// ###################
				
					if(buscar_infinitivo(u, p, word_act,  2, 1, 5)) return 1; 
				}
				if( !strcmp("irías", &word_act[tam-5]) ||
					!strcmp("iréis", &word_act[tam-5]) ) 
				{
					// ###################
					// # "irías" "iréis" # // 3ª
					// ###################
				
					if(buscar_infinitivo(u, p, word_act,  3, 2, 5)) return 1; 
				}
			}
			
			if(tam>4) {
				if( !strcmp("amos", &word_act[tam-4]) ) 
				{
					// ##########
					// # "amos" # // 1ª, 2ª o 3ª
					// ##########
				
					if(buscar_infinitivo(u, p, word_act,  3, 0, 4)) return 1;

					// ***********************************************
					// * irregularidades gráficas "finge" "finjamos" *
					// ***********************************************

					if(word_act[tam-5] == 'j'){
						strcpy(temporal, word_act);
						temporal[tam-5] = 'g';
						if(buscar_infinitivo(u, p, temporal,  3, 1, 4)) return 1;
					}

					// *******************************************************
					// * irregularidades gráficas "distingamos" "distinguir" *
					// *******************************************************

					if(!strcmp("gamos", &word_act[tam-5])){
						strcpy(temporal, word_act);
						temporal[tam-4] = 'u';
						if(buscar_infinitivo(u, p, temporal,  3, 2, 3)) return 1;
					}

				}
				if( !strcmp("imos", &word_act[tam-4]) ) 
				{
					// ###########
					// # "imos"  # // 2ª o 3ª
					// ###########
				
					if(buscar_infinitivo(u, p, word_act,  3, 1, 4)) return 1; 
				}
				if( !strcmp("abas", &word_act[tam-4]) ||
					!strcmp("arás", &word_act[tam-4]) ||
					!strcmp("ases", &word_act[tam-4]) ||
					!strcmp("ares", &word_act[tam-4]) ||
					!strcmp("aras", &word_act[tam-4]) ||
					!strcmp("emos", &word_act[tam-4]) ) 
				{
					// #############################################
					// # "abas" "arás" "ases" "ares" "aras" "emos" # // 1ª
					// #############################################
				
					if(buscar_infinitivo(u, p, word_act,  1, 0, 4)) return 1; 
				}
				if( !strcmp("erás", &word_act[tam-4]) ||
					!strcmp("emos", &word_act[tam-4]) ) 
				{
					// #################
					// # "erás" "emos" # // 2ª
					// #################
				
					if(buscar_infinitivo(u, p, word_act,  2, 1, 4)) return 1;

					// ************************************************
					// * irregularidades gráficas "carguemos" "carga" *
					// ************************************************
	
					if((tam>5) && !strcmp("guemos", &word_act[tam-6])){
						if(buscar_infinitivo(u, p, word_act,  1, 0, 5)) return 1;
					}
					
					// **********************************************
					// * irregularidades gráficas "gocemos" "gozar" *
					// **********************************************

					if(!strcmp("ocemos", &word_act[tam-6]) ||
						!strcmp("acemos", &word_act[tam-6])){
						strcpy(temporal, word_act);
						temporal[tam-5] = 'z';
						if(buscar_infinitivo(u, p, temporal,  1, 0, 4)) return 1;
					}
					
					// ***************************************************
					// * irregularidades gráficas "apliquemos" "àplicar" *
					// ***************************************************

					if(!strcmp("quemos", &word_act[tam-6])){
						strcpy(temporal, word_act);
						temporal[tam-6] = 'c';
						if(buscar_infinitivo(u, p, temporal,  1, 0, 5)) return 1;
					}
				}
				if( !strcmp("irás", &word_act[tam-4]) ||
					!strcmp("íais", &word_act[tam-4]) ) 
				{
					// #################
					// # "irás" "íais" # // 3ª
					// #################
				
					if(buscar_infinitivo(u, p, word_act,  3, 2, 4)) return 1; 
				}
			}
			if(tam>3) {
				if( !strcmp("áis", &word_act[tam-3]) ) 
				{
					// #########
					// # "áis" # // 1ª, 2ª o 3ª
					// #########
				
					if(buscar_infinitivo(u, p, word_act,  3, 0, 3)) return 1; 

					// **********************************************
					// * irregularidades gráficas "finge" "finjáis" *
					// **********************************************

					if(word_act[tam-4] == 'j'){
						strcpy(temporal, word_act);
						temporal[tam-4] = 'g';
						if(buscar_infinitivo(u, p, temporal,  3, 1, 3)) return 1;
					}
					// ******************************************************
					// * irregularidades gráficas "distingáis" "distinguir" *
					// ******************************************************

					if(!strcmp("gáis", &word_act[tam-4])){
						strcpy(temporal, word_act);
						temporal[tam-3] = 'u';
						if(buscar_infinitivo(u, p, temporal,  3, 2, 2)) return 1;
					}
				}
				if( !strcmp("éis", &word_act[tam-3]) ) 
				{
					// #########
					// # "éis" # // 1ª o 2ª 
					// #########
				
					if(buscar_infinitivo(u, p, word_act,  2, 0, 3)) return 1;

					// ***********************************************
					// * irregularidades gráficas "carguéis" "carga" *
					// ***********************************************

					if((tam>4) && !strcmp("guéis", &word_act[tam-5])){
						if(buscar_infinitivo(u, p, word_act,  1, 0, 4)) return 1;
					}

					// *********************************************
					// * irregularidades gráficas "gocéis" "gozar" *
					// *********************************************

					if(!strcmp("océis", &word_act[tam-5]) ||
						!strcmp("acéis", &word_act[tam-5])){
						strcpy(temporal, word_act);
						temporal[tam-4] = 'z';
						if(buscar_infinitivo(u, p, temporal,  1, 0, 3)) return 1;
					}
					
					// *********************************************
					// * irregularidades gráficas "gocéis" "gozar" *
					// *********************************************

					if(!strcmp("quéis", &word_act[tam-5])){
						strcpy(temporal, word_act);
						temporal[tam-5] = 'c';
						if(buscar_infinitivo(u, p, temporal,  1, 0, 4)) return 1;
					}
				}
				if( !strcmp("ías", &word_act[tam-3]) ) 
				{
					// #########
					// # "ías" # //  2ª o 3ª
					// #########
				
					if(buscar_infinitivo(u, p, word_act,  3, 1, 3)) return 1; 
				}
			}
			
			if(tam>2) {
				if( !strcmp("as", &word_act[tam-2]) ||
					!strcmp("es", &word_act[tam-2]) )
				{
					// #############
					// # "as" "es" # // 1ª, 2ª o 3ª
					// #############
				
					if(buscar_infinitivo(u, p, word_act,  3, 0, 2))
					{
						//u.cell(p).setPOS(POS_ES_VERB_SIMP_DEBIL);
						pos[i].pos1=10001;//POS_ES_VERB_SIMP_DEBIL
						return 1;
					}
					
					// **********************************************
					// * irregularidades gráficas "fingir" "finjas" *
					// **********************************************

					if(word_act[tam-3] == 'j' && !strcmp("as", &word_act[tam-2])){
						strcpy(temporal, word_act);
						temporal[tam-3] = 'g';
						if(buscar_infinitivo(u, p, temporal,  3, 1, 2))
						{
							//u.cell(p).setPOS(POS_ES_VERB_SIMP_DEBIL);
							pos[i].pos1=10001;//POS_ES_VERB_SIMP_DEBIL
							return 1;
						}
					}
				
					// *****************************************************
					// * irregularidades gráficas "distingas" "distinguir" *
					// *****************************************************

					if(!strcmp("gas", &word_act[tam-3])){
						strcpy(temporal, word_act);
						temporal[tam-2] = 'u';
						if(buscar_infinitivo(u, p, temporal,  3, 2, 1)) 
						{
							//u.cell(p).setPOS(POS_ES_VERB_SIMP_DEBIL);
							pos[i].pos1=10001;//POS_ES_VERB_SIMP_DEBIL
							return 1;
						}
					}
					
					// ********************************************
					// * irregularidades gráficas "goces" "gozar" *
					// ********************************************

					if(!strcmp("oces", &word_act[tam-4]) ||
						!strcmp("aces", &word_act[tam-4])){
						strcpy(temporal, word_act);
						temporal[tam-3] = 'z';
						if(buscar_infinitivo(u, p, temporal,  1, 0, 2)) return 1;
					}
					
					if(buscar_infinitivo(u, p, word_act,  3, 0, 2))
					{
						//u.cell(p).setPOS(POS_ES_VERB_SIMP_DEBIL);
						pos[i].pos1=10001;//POS_ES_VERB_SIMP_DEBIL
						return 1;
					}


					// ***********************************************
					// * irregularidades gráficas "cargues" "cargar" *
					// **********************************************

					if(!strcmp("gues", &word_act[tam-4])){
						if(buscar_infinitivo(u, p, word_act,  1, 0, 3)) 
						{
							//u.cell(p).setPOS(POS_ES_VERB_SIMP_DEBIL);
							pos[i].pos1=10001;//POS_ES_VERB_SIMP_DEBIL
							return 1;
						}

					}
										
					// *************************************************
					// * irregularidades gráficas "apliques" "àplicar" *
					// *************************************************

					if(!strcmp("ques", &word_act[tam-4])){
						strcpy(temporal, word_act);
						temporal[tam-4] = 'c';
						if(buscar_infinitivo(u, p, temporal,  1, 0, 3))
						{
							//u.cell(p).setPOS(POS_ES_VERB_SIMP_DEBIL);
							pos[i].pos1=10001;//POS_ES_VERB_SIMP_DEBIL
							return 1;
						}
					}
				}
				if( !strcmp("ís", &word_act[tam-2]) ) 
				{
					// #######
					// # "í" # // 3ª 
					// #######
				
					if(buscar_infinitivo(u, p, word_act,  3, 2, 2)) return 1; 
				}
			}
			break;

		default:
			break;
	}
	return 0;
}

INT LangES_Categ::buscar_infinitivo(UttWS &u,UttI p, CHAR *word_act, int fin, int inicio, int longitud)
{
	INT tam = 0;
	CHAR aei[3][3] = {"ar", "er", "ir"};
	INT i, temp;
	CHAR raiz[MAX_TAM_WORD]="\0";

	HDicRef href;

	tam = strlen(word_act);

	strcpy(raiz, word_act);

	for(i=inicio; i<fin; i++) 
	{
	strcpy(&raiz[tam-longitud], aei[i]); // Sobreescribimos la palabra.   comió -> comer

	href = u.getHDicDB()->search(raiz); // Buscamos en el diccionario el verbo

	temp = u.getHDicDB()->query(href, HDIC_QUERY_ES_VERB);

		switch(u.getHDicDB()->query(href, HDIC_QUERY_ES_VERB)) 
		{
		case HDIC_QUERY_ES_VERB_IR1C:	// 1ª conjugación
		case HDIC_QUERY_ES_VERB_IR2C:	// 2ª conjugación
		case HDIC_QUERY_ES_VERB_IR3C:	// 3ª conjugación
		case HDIC_QUERY_ES_VERB_IICC:	// 3ª conjugación
			u.cell(p).setPOS(POS_ES_VERB_SIMP);
			return 1;		
		default:
			break;
		}
	}
	return 0;
}

// *********************************************************************
// * Buscamos patrones en el inicio de la palabra, TODO lo que empiece *
// * Así será verbo (las excepciones al diccionario)                   *
// *                     le+ió -> leyó                                  *
// *********************************************************************

INT LangES_Categ::es_xxeer(UttWS &u,UttI p)
{
	INT tam = 0;
	CHAR word_act[MAX_TAM_WORD]="\0";
	CHAR raiz[MAX_TAM_WORD]="\0";
	strcpy(word_act,u.cell(p).getWord());

	HDicRef hDicRef;

	tam = strlen(word_act);

	strcpy(raiz, word_act);

	if (tam > 3) // Para ir limitando y no salirnos
	{
		raiz[3]='\0'; // vamos a mirar el comienzo de la palabra

		if(!strcmp(raiz, "ley") || !strcmp(raiz, "cay")) // es el verbo leer o caer
		{
			u.cell(p).setPOS(POS_ES_VERB_IICC);
			return 1;
		}
		if (tam > 4) 
		{
			strcpy(raiz, word_act);

			raiz[4]='\0'; // vamos a mirar el comienzo de la palabra

			if(!strcmp(raiz, "crey") || !strcmp(raiz, "caig")) // es el verbo creer
			{
				u.cell(p).setPOS(POS_ES_VERB_IICC);
				return 1;
			}
			if (tam > 5) 
			{
				strcpy(raiz, word_act);

				raiz[5]='\0'; // vamos a mirar el comienzo de la palabra

				 // es el verbo releer, poseer, decaer o recaer
				if(!strcmp(raiz, "reley") || !strcmp(raiz, "posey") ||
					!strcmp(raiz, "decay") || !strcmp(raiz, "recay"))
				{
					u.cell(p).setPOS(POS_ES_VERB_IICC);
					return 1;
				}
				if (tam > 6) 
				{
					strcpy(raiz, word_act);

					raiz[6]='\0'; // vamos a mirar el comienzo de la palabra

					// es el verbo proveer, decaer o recaer
					if(!strcmp(raiz, "provey") || !strcmp(raiz, "decaig") ||
						!strcmp(raiz, "recaig")) 
					{
						u.cell(p).setPOS(POS_ES_VERB_IICC);
						return 1;
					}
					if (tam > 7) 
					{
						strcpy(raiz, word_act);

						raiz[7]='\0'; // vamos a mirar el comienzo de la palabra

						if(!strcmp(raiz, "descrey")) // es el verbo descreer
						{
							u.cell(p).setPOS(POS_ES_VERB_IICC);
							return 1;
						}
						if (tam > 8) 
						{
							strcpy(raiz, word_act);

							raiz[8]='\0'; // vamos a mirar el comienzo de la palabra

							// es el verbo sobreseer o desposeer
							if(!strcmp(raiz, "desposey") || !strcmp(raiz, "sobresey")) 
							{
								u.cell(p).setPOS(POS_ES_VERB_IICC);
								return 1;
							}
						}

					}

				}
			}
		}
	}
	return 0;
}


// *********************************************************************
// * Buscamos patrones en el inicio de la palabra, TODO lo que empiece *
// * Así será verbo (las excepciones al diccionario)                   *
// *                         tañ+ió -> tañó                            *
// *********************************************************************

INT LangES_Categ::es_nli(UttWS &u,UttI p)
{
	INT tam = 0;
	CHAR word_act[MAX_TAM_WORD]="\0";
	CHAR raiz[MAX_TAM_WORD]="\0";
	strcpy(word_act,u.cell(p).getWord());

	HDicRef hDicRef;

	tam = strlen(word_act);

	strcpy(raiz, word_act);

	if (tam > 3) // Para ir limitando y no salirnos
	{
		raiz[3]='\0'; // vamos a mirar el comienzo de la palabra

		if(!strcmp(raiz, "tañ") || !strcmp(raiz, "ciñ")) // tañer o ceñir
		{
			u.cell(p).setPOS(POS_ES_VERB_IICC); // dá igual, pero ceñir es 3ª
			return 1;
		}
		strcpy(raiz, word_act);

		raiz[4]='\0'; // vamos a mirar el comienzo de la palabra

		if(!strcmp(raiz, "atañ") || !strcmp(raiz, "riñó") || !strcmp(raiz, "riñe") ||
			!strcmp(raiz, "tiñó") || !strcmp(raiz, "tiñe"))
			// es el verbo atañer, reñir o tenir
		{
			u.cell(p).setPOS(POS_ES_VERB_IICC); // puede ser 3ª
			return 1;
		}
		if (tam > 5) 
		{
			strcpy(raiz, word_act);

			raiz[5]='\0'; // vamos a mirar el comienzo de la palabra

			 // es el verbo griñir, plañir o bullir
			if(!strcmp(raiz, "gruñó") || !strcmp(raiz, "gruñe") ||
				!strcmp(raiz, "plañó") || !strcmp(raiz, "plañe") ||
				!strcmp(raiz, "bulló") || !strcmp(raiz, "bulle"))
			{
				u.cell(p).setPOS(POS_ES_VERB_IICC);
				return 1;
			}
			if (tam > 6) 
			{
				strcpy(raiz, word_act);

				raiz[6]='\0'; // vamos a mirar el comienzo de la palabra

				// es el verbo desteñir, estreñir o engullir
				if(!strcmp(raiz, "destiñ") || !strcmp(raiz, "estriñ") ||
					!strcmp(raiz, "engull")) 
				{
					u.cell(p).setPOS(POS_ES_VERB_IICC);
					return 1;
				}
				if (tam > 8) 
				{
					strcpy(raiz, word_act);

					raiz[8]='\0'; // vamos a mirar el comienzo de la palabra

					// es el verbo constreñir, rebullir, escabullir o zambullir
					if(!strcmp(raiz, "constriñ") || !strcmp(raiz, "rebulle")||
						!strcmp(raiz, "escabull") || !strcmp(raiz, "zambulle")) 
					{
						u.cell(p).setPOS(POS_ES_VERB_IICC);
						return 1;
					}
				}
			}
		}
	}
	return 0;
}

// *********************************************************************
// * Buscamos patrones en el inicio de la palabra, TODO lo que empiece *
// *  (las excepciones al diccionario) (lo regular`por otro camino)    *
// *                   pongo puse pondrá                               *
// *********************************************************************

INT LangES_Categ::es_poner(UttWS &u,UttI p, CHAR *word_act)
{
	INT tam = 0;
	CHAR raiz[MAX_TAM_WORD]="\0";

	HDicRef hDicRef;
	tam = strlen(word_act);

	strcpy(raiz, word_act);

	if (tam > 3) // Para ir limitando y no salirnos
	{
		raiz[3]='\0'; // vamos a mirar el comienzo de la palabra

		if(!strcmp(raiz, "pus"))
		{
			u.cell(p).setPOS(POS_ES_VERB_IICC);
			return 1;
		}
		if (tam > 4) 
		{
			strcpy(raiz, word_act);

			raiz[4]='\0'; // vamos a mirar el comienzo de la palabra

			if(!strcmp(raiz, "pong"))
			{
				u.cell(p).setPOS(POS_ES_VERB_IICC);
				return 1;
			}
			if (tam > 5) 
			{
				strcpy(raiz, word_act);

				raiz[5]='\0'; // vamos a mirar el comienzo de la palabra

				if(!strcmp(raiz, "pondr"))
				{
					u.cell(p).setPOS(POS_ES_VERB_IICC);
					return 1;
				}
			}
		}
	}
	return 0;
}


INT LangES_Categ::es_xxponer(UttWS &u,UttI p)
{
	INT tam = 0;
	CHAR word_act[MAX_TAM_WORD]="\0";
	CHAR raiz[MAX_TAM_WORD]="\0";
	strcpy(word_act,u.cell(p).getWord());

	HDicRef hDicRef;

	tam = strlen(word_act);

	strcpy(raiz, word_act);

	if(es_poner( u, p, raiz))
	{
		return 1;
	}
	if(raiz[0]=='o') // oponer
	{
		if(es_poner( u, p, &raiz[1]))
		{
			return 1;
		}
	}
	if(tam>5)
	{
		// deponer exponer imponer reponer suponer

		if(es_poner( u, p, &raiz[2]))
		{
			return 1; // OJO!! Suponemos que lo son todas (nos da igual cuál)
		}

		// 	componer disponer posponer proponer

		if(es_poner( u, p, &raiz[3]))
		{
			return 1; // OJO!! Suponemos que lo son todas (nos da igual cuál)
		}

		// anteponer trasponer

		if(es_poner( u, p, &raiz[4]))
		{
			return 1; // OJO!! Suponemos que lo son todas (nos da igual cuál)
		}

		// 	indisponer interponer presuponer recomponer sobreponer superponer transponer

		if(es_poner( u, p, &raiz[5]))
		{
			return 1; // OJO!! Suponemos que lo son todas (nos da igual cuál)
		}

		// 	contraponer descomponer predisponer

		if(es_poner( u, p, &raiz[6]))
		{
			return 1; // OJO!! Suponemos que lo son todas (nos da igual cuál)
		}
	}
	return 0;
}

// *********************************************************************
// * Buscamos patrones en el inicio de la palabra, TODO lo que empiece *
// *  (las excepciones al diccionario) (lo regular`por otro camino)    *
// *          hago hice hizo hará haré haremos harían                  *
// *********************************************************************

INT LangES_Categ::es_hacer(UttWS &u,UttI p, CHAR *word_act)
{
	INT tam = 0;
	CHAR raiz[MAX_TAM_WORD]="\0";

	HDicRef hDicRef;
	tam = strlen(word_act);

	strcpy(raiz, word_act);

	if (tam > 3) // Para ir limitando y no salirnos
	{
		raiz[3]='\0'; // vamos a mirar el comienzo de la palabra

		if(!strcmp(raiz, "hag") || !strcmp(raiz, "hiz") || !strcmp(raiz, "hic"))
		{
			u.cell(p).setPOS(POS_ES_VERB_IICC);
			return 1;
		}
		strcpy(raiz, word_act);

		raiz[4]='\0'; // vamos a mirar el comienzo de la palabra

		if(!strcmp(raiz, "haré") || !strcmp(raiz, "hará") ||
			!strcmp(raiz, "harí") || !strcmp(raiz, "hare")) 
		{
			u.cell(p).setPOS(POS_ES_VERB_IICC);
			return 1;
		}
		if (tam == 7) 
		{
			if(!strcmp("haremos", word_act) )
			{
				u.cell(p).setPOS(POS_ES_VERB_IICC);
				return 1;
			}
		}
	}
	return 0;
}


INT LangES_Categ::es_xxhacer(UttWS &u,UttI p)
{
	INT tam = 0;
	CHAR word_act[MAX_TAM_WORD]="\0";
	CHAR raiz[MAX_TAM_WORD]="\0";
	strcpy(word_act,u.cell(p).getWord());

	HDicRef hDicRef;

	tam = strlen(word_act);

	strcpy(raiz, word_act);

	if(es_hacer( u, p, raiz))
	{
		return 1;
	}
	if(tam>5)
	{
		// rehacer

		if(es_hacer( u, p, &raiz[2]))
		{
			return 1; 
		}

		// 	deshacer

		if(es_hacer( u, p, &raiz[3]))
		{
			return 1; 
		}

		// 	satisfacer

		raiz[5] = 'h';

		if(es_hacer( u, p, &raiz[5]))
		{
			return 1; // OJO!! Suponemos que lo son todas (nos da igual cuál)
		}
	}
	return 0;
}


// *********************************************************************
// * Buscamos patrones en el inicio de la palabra, TODO lo que empiece *
// *                  (lo regular por otro camino)                     *
// *                          valdrá valgamos                          *
// *********************************************************************

INT LangES_Categ::es_valer(UttWS &u,UttI p, CHAR *word_act)
{
	INT tam = 0;
	CHAR raiz[MAX_TAM_WORD]="\0";

	HDicRef hDicRef;
	tam = strlen(word_act);

	strcpy(raiz, word_act);

	if (tam > 4) 
	{
		strcpy(raiz, word_act);

		raiz[4]='\0'; // vamos a mirar el comienzo de la palabra

		if(!strcmp(raiz, "valg")) 
		{
			u.cell(p).setPOS(POS_ES_VERB_IICC);
			return 1;
		}
		if (tam >5) 
		{
			strcpy(raiz, word_act);

			raiz[5]='\0'; // vamos a mirar el comienzo de la palabra

			if(!strcmp("valdr", raiz) )
			{

				u.cell(p).setPOS(POS_ES_VERB_IICC);
				return 1;
			}
		}
	}
	return 0;
}


INT LangES_Categ::es_xxvaler(UttWS &u,UttI p)
{
	INT tam = 0;
	CHAR word_act[MAX_TAM_WORD]="\0";
	CHAR raiz[MAX_TAM_WORD]="\0";
	strcpy(word_act,u.cell(p).getWord());

	HDicRef hDicRef;

	tam = strlen(word_act);

	strcpy(raiz, word_act);

	if(es_valer( u, p, raiz))
	{
		return 1;
	}
	if(tam>5)
	{
		// equivaler

		if(es_valer( u, p, &raiz[4]))
		{
			return 1; 
		}
	}
	return 0;
}

// *********************************************************************
// * Buscamos patrones en el inicio de la palabra, TODO lo que empiece *
// *  (las excepciones al diccionario) (lo regular`por otro camino)    *
// *                    tuve tengo tendrá                              *
// *********************************************************************

INT LangES_Categ::es_tener(UttWS &u,UttI p, CHAR *word_act)
{
	INT tam = 0;
	CHAR raiz[MAX_TAM_WORD]="\0";

	HDicRef hDicRef;
	tam = strlen(word_act);

	strcpy(raiz, word_act);

	if (tam > 3) // Para ir limitando y no salirnos
	{
		raiz[3]='\0'; // vamos a mirar el comienzo de la palabra

		if(!strcmp(raiz, "tuv"))
		{
			u.cell(p).setPOS(POS_ES_VERB_IICC);
			return 1;
		}
		if (tam > 4) 
		{
			strcpy(raiz, word_act);

			raiz[4]='\0'; // vamos a mirar el comienzo de la palabra

			if(!strcmp(raiz, "teng"))
			{
				u.cell(p).setPOS(POS_ES_VERB_IICC);
				return 1;
			}
			if (tam > 5) 
			{
				strcpy(raiz, word_act);

				raiz[5]='\0'; // vamos a mirar el comienzo de la palabra

				if(!strcmp(raiz, "tendr"))
				{
					u.cell(p).setPOS(POS_ES_VERB_IICC);
					return 1;
				}
			}
		}
	}
	return 0;
}


INT LangES_Categ::es_xxtener(UttWS &u,UttI p)
{
	INT tam = 0;
	CHAR word_act[MAX_TAM_WORD]="\0";
	CHAR raiz[MAX_TAM_WORD]="\0";
	strcpy(word_act,u.cell(p).getWord());

	HDicRef hDicRef;

	tam = strlen(word_act);

	strcpy(raiz, word_act);

	if(es_tener( u, p, raiz))
	{
		return 1;
	}
	if(raiz[0]=='a') // atener
	{
		if(es_tener( u, p, &raiz[1]))
		{
			return 1;
		}
	}
	if(tam>5)
	{
		// detener obtener retener

		if(es_tener( u, p, &raiz[2]))
		{
			return 1; // OJO!! Suponemos que lo son todas (nos da igual cuál)
		}

		// 	contener mantener sostener

		if(es_tener( u, p, &raiz[3]))
		{
			return 1; // OJO!! Suponemos que lo son todas (nos da igual cuál)
		}

		// entretener

		if(es_tener( u, p, &raiz[5]))
		{
			return 1; // OJO!! Suponemos que lo son todas (nos da igual cuál)
		}
	}
	return 0;
}


// *********************************************************************
// * Buscamos patrones en el inicio de la palabra, TODO lo que empiece *
// *  (las excepciones al diccionario) (lo regular`por otro camino)    *
// *                    traigo traje                             *
// *********************************************************************

INT LangES_Categ::es_traer(UttWS &u,UttI p, CHAR *word_act)
{
	INT tam = 0;
	CHAR raiz[MAX_TAM_WORD]="\0";

	HDicRef hDicRef;
	tam = strlen(word_act);

	strcpy(raiz, word_act);

	if (tam > 3) // Para ir limitando y no salirnos
	{
		strcpy(raiz, word_act);

		raiz[4]='\0'; // vamos a mirar el comienzo de la palabra

		if(!strcmp(raiz, "traj"))
		{
			u.cell(p).setPOS(POS_ES_VERB_IICC);
			return 1;
		}
		if (tam > 5) 
		{
			strcpy(raiz, word_act);

			raiz[5]='\0'; // vamos a mirar el comienzo de la palabra

			if(!strcmp(raiz, "traig"))
			{
				u.cell(p).setPOS(POS_ES_VERB_IICC);
				return 1;
			}
		}
	}
	return 0;
}


INT LangES_Categ::es_xxtraer(UttWS &u,UttI p)
{
	INT tam = 0;
	CHAR word_act[MAX_TAM_WORD]="\0";
	CHAR raiz[MAX_TAM_WORD]="\0";
	strcpy(word_act,u.cell(p).getWord());

	HDicRef hDicRef;

	tam = strlen(word_act);

	strcpy(raiz, word_act);

	if(es_traer( u, p, raiz))
	{
		return 1;
	}
	if(raiz[0]=='a') // atraer
	{
		if(es_traer( u, p, &raiz[1]))
		{
			return 1;
		}
	}
	if(tam>5)
	{
		// detraer extraer retraer

		if(es_traer( u, p, &raiz[2]))
		{
			return 1; // OJO!! Suponemos que lo son todas (nos da igual cuál)
		}

		// 	abstraer contraer distraer sustraer

		if(es_traer( u, p, &raiz[3]))
		{
			return 1; // OJO!! Suponemos que lo son todas (nos da igual cuál)
		}

		// retrotraer

		if(es_traer( u, p, &raiz[5]))
		{
			return 1; // OJO!! Suponemos que lo son todas (nos da igual cuál)
		}
	}
	return 0;
}

// *********************************************************************
// * Buscamos patrones en el inicio de la palabra, TODO lo que empiece *
// *  (las excepciones al diccionario) (lo regular`por otro camino)    *
// *          dijo dijera (diga -> digir ; dice -> dicir)              *
// *********************************************************************

INT LangES_Categ::es_decir(UttWS &u,UttI p, CHAR *word_act)
{
	INT tam = 0;
	CHAR raiz[MAX_TAM_WORD]="\0";

	HDicRef hDicRef;
	tam = strlen(word_act);

	strcpy(raiz, word_act);

	if (tam > 3) // Para ir limitando y no salirnos
	{
		strcpy(raiz, word_act);

		raiz[3]='\0'; // vamos a mirar el comienzo de la palabra

		if(!strcmp(raiz, "dij"))
		{
			u.cell(p).setPOS(POS_ES_VERB_IICC);
			return 1;
		}
	}
	return 0;
}


INT LangES_Categ::es_xxdecir(UttWS &u,UttI p)
{
	INT tam = 0;
	CHAR word_act[MAX_TAM_WORD]="\0";
	CHAR raiz[MAX_TAM_WORD]="\0";
	strcpy(word_act,u.cell(p).getWord());

	HDicRef hDicRef;

	tam = strlen(word_act);

	strcpy(raiz, word_act);

	if(es_decir( u, p, raiz))
	{
		return 1;
	}
	if(tam>5)
	{
		// bendecir maldecir

		if(es_decir( u, p, &raiz[3]))
		{
			return 1; // OJO!! Suponemos que lo son todas (nos da igual cuál)
		}

		// 	contradecir

		if(es_traer( u, p, &raiz[5]))
		{
			return 1; // OJO!! Suponemos que lo son todas (nos da igual cuál)
		}
	}
	return 0;
}


// *********************************************************************
// * Buscamos patrones en el inicio de la palabra, TODO lo que empiece *
// *  (las excepciones al diccionario) (lo regular`por otro camino)    *
// *                             saldría                               *
// *********************************************************************

INT LangES_Categ::es_salir(UttWS &u,UttI p, CHAR *word_act)
{
	INT tam = 0;
	CHAR raiz[MAX_TAM_WORD]="\0";

	HDicRef hDicRef;
	tam = strlen(word_act);

	if (tam > 5) 
	{
		strcpy(raiz, word_act);

		raiz[5]='\0'; // vamos a mirar el comienzo de la palabra

		if(!strcmp(raiz, "saldr"))
		{
			u.cell(p).setPOS(POS_ES_VERB_IICC);
			return 1;
		}
	}
	return 0;
}


INT LangES_Categ::es_xxsalir(UttWS &u,UttI p)
{
	INT tam = 0;
	CHAR word_act[MAX_TAM_WORD]="\0";
	CHAR raiz[MAX_TAM_WORD]="\0";
	strcpy(word_act,u.cell(p).getWord());

	HDicRef hDicRef;

	tam = strlen(word_act);

	strcpy(raiz, word_act);

	if(es_salir( u, p, raiz))
	{
		return 1;
	}
	if(tam>5)
	{
		// sobresalir

		if(es_salir( u, p, &raiz[5]))
		{
			return 1; // OJO!! Suponemos que lo son todas (nos da igual cuál)
		}
	}
	return 0;
}

// *********************************************************************
// * Buscamos patrones en el inicio de la palabra, TODO lo que empiece *
// *  (las excepciones al diccionario) (lo regular por otro camino)    *
// *                              vendré                               *
// *********************************************************************

INT LangES_Categ::es_venir(UttWS &u,UttI p, CHAR *word_act)
{
	INT tam = 0;
	CHAR raiz[MAX_TAM_WORD]="\0";

	HDicRef hDicRef;
	tam = strlen(word_act);

	strcpy(raiz, word_act);

	if (tam > 5) 
	{
		strcpy(raiz, word_act);

		raiz[5]='\0'; // vamos a mirar el comienzo de la palabra

		if(!strcmp(raiz, "vendr"))
		{
			u.cell(p).setPOS(POS_ES_VERB_IICC);
			return 1;
		}
	}
	return 0;
}


INT LangES_Categ::es_xxvenir(UttWS &u,UttI p)
{
	INT tam = 0;
	CHAR word_act[MAX_TAM_WORD]="\0";
	CHAR raiz[MAX_TAM_WORD]="\0";
	strcpy(word_act,u.cell(p).getWord());

	HDicRef hDicRef;

	tam = strlen(word_act);

	strcpy(raiz, word_act);

	if(es_venir( u, p, raiz))
	{
		return 1;
	}
	if(raiz[0]=='a') // avenir
	{
		if(es_venir( u, p, &raiz[1]))
		{
			return 1;
		}
	}
	if(tam>5)
	{
		// devenir

		if(es_venir( u, p, &raiz[2]))
		{
			return 1; // OJO!! Suponemos que lo son todas (nos da igual cuál)
		}

		// 	prevenir provenir subvenir

		if(es_venir( u, p, &raiz[3]))
		{
			return 1; // OJO!! Suponemos que lo son todas (nos da igual cuál)
		}

		// 	desavenir

		if(es_venir( u, p, &raiz[4]))
		{
			return 1; // OJO!! Suponemos que lo son todas (nos da igual cuál)
		}

		// intervenir reconvenir sobrevenir

		if(es_venir( u, p, &raiz[5]))
		{
			return 1; // OJO!! Suponemos que lo son todas (nos da igual cuál)
		}

		// contravenir

		if(es_venir( u, p, &raiz[6]))
		{
			return 1; // OJO!! Suponemos que lo son todas (nos da igual cuál)
		}
	}
	return 0;
}

// *********************************************************************
// * Buscamos patrones en el inicio de la palabra, TODO lo que empiece *
// *  (las excepciones al diccionario) (lo regular`por otro camino)    *
// *          *ducir -> todos los *duz* y *duj* son de estos           *
// *********************************************************************

INT LangES_Categ::es_xxducir(UttWS &u,UttI p)
{
	INT tam = 0;
	CHAR word_act[MAX_TAM_WORD]="\0";
	CHAR raiz[MAX_TAM_WORD]="\0";
	strcpy(word_act,u.cell(p).getWord());

	HDicRef hDicRef;

	tam = strlen(word_act);

	strcpy(raiz, word_act);

	if(tam>5)
	{
		raiz[4]='\0';
		// aducir

		if(!strcmp("duz", &raiz[1]) || !strcmp("duj", &raiz[1]))
		{
			u.cell(p).setPOS(POS_ES_VERB_IICC);
			return 1;
		}

		strcpy(raiz, word_act);

		raiz[5]='\0';

		// deducir inducir reducir

		if(!strcmp("duz", &raiz[2]) || !strcmp("duj", &raiz[2]))
		{
			u.cell(p).setPOS(POS_ES_VERB_IICC);
			return 1;
		}
		
		strcpy(raiz, word_act);

		raiz[6]='\0';

		// producir traducir

		if(!strcmp("duz", &raiz[3]) || !strcmp("duj", &raiz[3]))
		{
			u.cell(p).setPOS(POS_ES_VERB_IICC);
			return 1;
		}
		if(tam>7)
		{
			strcpy(raiz, word_act);

			raiz[8]='\0';

			// reconducir introducir reproducir

			if(!strcmp("duz", &raiz[5]) || !strcmp("duj", &raiz[5]))
			{
				u.cell(p).setPOS(POS_ES_VERB_IICC);
				return 1;
			}

		}
	}
	return 0;
}
// *********************************************************************
// * Buscamos patrones en el inicio de la palabra, TODO lo que empiece *
// *                   (las excepciones al diccionario)                *
// *                        participios y gerundios                    *
// *********************************************************************

INT LangES_Categ::es_no_personal(UttWS &u,UttI p)
{
	CHAR letra_final = 0;
	INT tam = 0;
	CHAR word_act[MAX_TAM_WORD]="\0";
	CHAR raiz[MAX_TAM_WORD]="\0";
	strcpy(word_act,u.cell(p).getWord());

	tam = strlen(word_act);
	letra_final = word_act[tam-1];

	HDicRef hDicRef;


	if(letra_final == 'o')
	{
		// ##########################
		// # Gerundios -ando -iendo #
		// ##########################

		if(tam>4)
		{
			if (!strcmp("ando", &word_act[tam-4]) ||
				!strcmp("iendo", &word_act[tam-5]))
				{
					u.cell(p).setPOS(POS_ES_VERB_GERU);
					return 1;
			}
		}

		if(tam>3)
		{
			// #########################
			// # Participios -ado -ido #
			// #########################


			if (!strcmp("ado", &word_act[tam-3]) ||
				!strcmp("ido", &word_act[tam-3]))
				{
				
					if(buscar_infinitivo(u, p, word_act,  3, 0, 3))
					{
						u.cell(p).setPOS(POS_ES_VERB_PPIO);
						return 1;
					}
			}
		}
	}
	return 0;
}

int quitar_acentos(CHAR *word_act)
{
	INT i, tam = 0;

	tam = strlen(word_act);

	for (i=0; i< tam; i++)
	{
		switch (word_act[i])
		{
		case 'á':
			word_act[i] = 'a';
			break;
		case 'é':
			word_act[i] = 'e';
			break;
		case 'í':
			word_act[i] = 'i';
			break;
		case 'ó':
			word_act[i] = 'o';
			break;
		case 'ú':
			word_act[i] = 'u';
			break;
		}
	}
	return 1;
}

		
// *********************************************************************
// * Buscamos patrones en el inicio de la palabra, TODO lo que empiece *
// *                   (las excepciones al diccionario)                *
// *                        adverbios tipo   -mente                    *
// *********************************************************************

INT LangES_Categ::es_xxmente(UttWS &u,UttI p)
{
	INT tam = 0;
	CHAR word_act[MAX_TAM_WORD]="\0";
	strcpy(word_act,u.cell(p).getWord());

	tam = strlen(word_act);

	HDicRef hDicRef;


	if(tam>5)
	{
		// ####################
		// # adverbios -mente #
		// ####################

		if (!strcmp("mente", &word_act[tam-5]))
		{
				u.cell(p).setPOS(POS_ES_ADVE);
				return 1;
		}
	}
	return 0;
}

int mirar_etiq (int etiqueta, int i, etiquetas *pos){
	
	int etiq_encontrada=0;
	
	if(pos[i].pos1==etiqueta || pos[i].pos2==etiqueta || pos[i].pos3==etiqueta || pos[i].pos4==etiqueta || pos[i].pos5==etiqueta ){
		etiq_encontrada=1;
	}
	else{
		etiq_encontrada=0;
	}	
	return etiq_encontrada;
}
