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
(C) 1999 TSR/Aholab - ETSII/IT Bilbao (UPV/EHU) // Euskaltel

Nombre fuente................ eu_ptuti.cpp
Nombre paquete............... aHoTTS
Lenguaje fuente.............. C++
Estado....................... -
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion: ............... I¤igo Salbidea
............................. Ana Armenta
..............................Nora Barroso

Version  dd/mm/aa  Autor     Comentario
-------      --------     ------       ----------
2.0.0		04/02/05	  Nora	 Salbuespen berriak sartu
1.0.0    31/01/00     borja     codefreeze aHoTTS v1.0
0.0.0    13/05/99   inigo/ana Codificacion inicial

======================== Contenido ========================
<DOC>

</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include <string.h>

#include "eu_lingp.hpp"
#include "uti.h"

/**********************************************************/
/* esta mira si {palabra} esta dentro de una lista en funcion
	 de {caso}  */

/*************de***str_match****(uti_misc.c)***************/
/*int str_match( const char * const *strarr, int nstrs,
	const char * str, int lcode, int case_sensitive )*/
/* busca {str} en el array de {nstrs} cadenas {strarr}. Si lo encuentra,
{devuelve} el indice en el array. Si no lo encuentra, {devuelve} -1.
Si se encuentra con una etiqueta ambigua, {devuelve} -2.
Codigo para las longitudes, {lcode} =
	 -2 como -1, pero si hay un match exacto se admite aunque sea ambiguo
	 -1 si se permite strlen(strarr{})<strlen(str)
		0 si strlen(strarr{})==strlen(str)
		1 si se permite strlen(strarr{})>strlen(str)
		2 como 1, pero si hay un match exacto se admite aunque sea ambiguo
Si {case_sensitive}==0 no distingue mayusculas de minusculas */
/*******fin***de***str_match****(uti_misc.c)***************/

/**********************************************************/

BOOL LangEU_PhTrans::es_bai_aditz(UttPh & u,UttI p)
{
BOOL encontrado=FALSE;
UttI wo;
static const CHAR *bai="bai";
CHAR tarte1[10];
const CHAR *hitza;
INT i;
	wo=u.wordThis(p, URANGE_PHRASE);
	hitza = u.cell(wo).getWord(); // palabra actual
		for(i=0;i<3;i++) // primeras tres letras de actual
			tarte1[i]=hitza[i];
		tarte1[i]='\0';
		//si empieza por bai,preguntamos si lo que sigue es verbo
		if(!strcmp(tarte1,bai)){
			if(hitza[3]=='n'){
				if(es_verbo_jok(u,wo)||es_verbo_lgn(u,wo)) encontrado=TRUE;
			}
			if(hitza[3]=='l'){
				if(es_verbo_jok(u,wo)||es_verbo_lgn(u,wo)) encontrado=TRUE;
			}
		}
	return(encontrado);
}

/**********************************************************/
BOOL LangEU_PhTrans::es_aux_o_sint(UttPh & u,UttI p, char caso)
{
	const char *bex_lista_adlgn[]=
	{//de estos verbos se realizar  b£squeda exacta
	 "bada", // -> badela
	 "beza",
	NULL
	};
	const char *b_lista_adlgn[]=
	{//esta lista se utilizar  para saber
	//si nuestra palabra contiene alguna de las raices de esta lista
	"badadi",
	"badait",
	"badago",
	"badaki",
	"badela",
	"badeza",
	"badiagu",
	"badiat",
	"badida",
	"badie",
	"badigu",
	"badik",
	"badin",
	"badio",
	"badira",
	"badit",
	"badizki",
	"badizu",
	"badu",
	"bagait ",
	"bagakizk ",
	"bagara",
	"bagatzaizk",
	"bagen",
	"bagin",
	"baha",
	"bahen",
	"baheza",
	"bahida",
	"bahie",
	"bahigu",
	"bahin",
	"bahio",
	"bahit", //$$$$$$
	"bahizki",
	"bahu",
	"baledi",
	"baleki",
	"baleza",
	"baliate",
	"balidate",
	"balie",
	"baligu",
	"balik",
	"balin",
	"balio", //$$$$$$
	"balira",
	"balit",
	"balizki",
	"balizu",
	"balu",
	"banadi",
	"banaiteke",
	"banaiz",
	"banaki",
	"banatzaie",
	"banatzaik",
	"banatzain",
	"banatzaio",
	"banatzaiz",
	"banau",
	"banaz",
	"bane",
	"banian",
	"banie",
	"banik",
	"banin",
	"banio",
	"banitu",
	"baniz",
	"banu",
	"bazai",
	"bazak",
	"bazara",
	"bazare",
	"bazat",
	"bazela",
	"bazen",
	"bazia",
	"bazida",
	"bazie",
	"bazig",
	"bazin",
	"bazio",
	"bazir",
	"bazit",
	"baziz",
	"bazu",
	"bedi",
	"beki",
	"bezate",
	"bieza",
	"bitez",
	"bitza",
	NULL
	};
	const char * dex_lista_adlgn[]=
	{//b£squeda exacta
	"da", // -> dela
	"diat", // -> diadala
	"die", // -> diela
	"diet", // -> diedala
	"dik", // -> diala
	"din", // -> dinala
	"dira",
	"dio", // -> diola
	"dit", // -> dida
	"du", // -> duela
	"duk", // -> duala
	"dun", // -> dunala
	"dut", // -> dudala
	NULL
	};
	const char * d_lista_adlgn[]=
	{//b£squeda sobre raices
	"dadi",
	"daite",
	"dago",
	"daki",
	"dator",
	"dela",
	"deza",
	"diadala",
	"diagu",
	"diala",
	"diate",
	"dida",
	"diegu",
	"diek",
	"diela",
	"dien",
	"diete",
	"dieza",
	"diezu",
	"dire",
	"digu",
	"dinala",
	"dinagu",
	"dinat",
	"diogu",
	"diok",
	"dion",
	"diot",
	"diozu",
	"dira",
	"ditu",
	"ditza",
	"dizki",
	"dizu",
	"duala",
	"dudala",
	"dugu",
	"dunala",
	"dute",
	"duzu",
	NULL
	};
	const char *gex_lista_adlgn[]=
	{
	"gara", // -> garela
	"ginen", // -> ginela
	NULL
	};
	const char * g_lista_adlgn[]= /* osatzeko */
	{
		"gaitezke",
		"gaitza",
		"garela",
		"gakizki",
		"gatzaizki",
		"geneki",
		"genez",
		"genia",
		"genie",
		"genina",
		"genio",
		"genitu",
		"genitza",
		"geniz",
		"genkizki",
		"genu",
		"ginat",
		"ginela",
		"ginte",
		"gintu",
		"gintz",
		NULL
	};
	const char *lex_lista_adlgn[]=
	{
	NULL
	};
	const char * l_lista_adlgn[]=
	{
		"lebil",
		"ledi",
		"legoke",
		"letor",
		"letoz",
		"letzake",
		"leude",
		"leutz",
		"lezake",
		"lezan",
		"lezat",
		"lihoake",
		"lirate",
		"liteke",
		"litez",
		"litza",
		"luke",
		NULL
	};

	const char * nex_lista_adlgn[] =
	{
	 "noa", // -> noala
	NULL
	};
	const char * n_lista_adlgn[] =
	{
		"nabil",
		"nadi",
		"nago",
		"nait",
		"naiz",
		"nator",
		"natza",
		"nekar",
		"neki",
		"nelab",
		"nenb",
		"nend",
		"neng",
		"nent",
		"nerabile",
		"nerama",
		"neri",
		"neuka",
		"neuzka",
		"neza",
		"nindoa",
		"nint",
		"nio",
		"nirudie",
		"nitu",
		"nitza",
		"noala",
		"nue",
		"nuke",
		NULL
	};
	const char *zex_lista_adlgn[]=
	{
	"zaik", // -> zaiala
	"zain", // -> zainala
	"zara", // -> zare
	"zen", // -> zela
	"zela",
	"zian", // -> ziana
	"zien", // -> ziela
	"zuen", // -> zuela
	NULL
	};

	const char * z_lista_adlgn[]= /* egiteko */
	{
	"zaiala",
	"zaie",
	"zaigu",
	"zainala",
	"zaio",
	"zait",
	"zaizki",
	"zare",
	"zatekee",
	"zatzaizki",
	"zedi",
	"zego",
	"zeki",
	"zeneza",
	"zenida",
	"zenie", // zenit,zenital
	"zenigu",
	"zenio",
	"zenitu",
	"zenitz",
	"zenizki",
	"zenkizki",
	"zenu",
	"zezak",
	"ziakee",
	"ziana",
	"ziate",
	"zida",
	"zieke",
	"ziela",
	"ziete",
	"zieza",
	"zigu",
	"zinake",
	"zinan",
	"zinate",
	"zinen", // zinela
	"zinela",
	"zinete",
	"zintez",
	"zintu",
	"zintza",
	"zioke",
	"zion",
	"ziote",
	"zirateke",
	"ziren",
	"zite",
	"zitue",
	"zituz",
	"zitza",
	"zizki",
	"zizue",
	"zizun",
	"zizuela",
	"zizute",
	"zuela"
	"zukee",
	"zukete",
	"zute",
		NULL
	};

	BOOL encontrado=FALSE;
	const CHAR * palabra;
	char temp[10];
	size_t i;
	UttI wo;

	wo=u.wordThis(p, URANGE_PHRASE);
	palabra = u.cell(wo).getWord(); // palabra actual

	switch(caso){
		case 'b':
			if(str_match(bex_lista_adlgn, nptrs((const void * const *)bex_lista_adlgn), palabra, 0, 0)!=-1){
			 encontrado=TRUE;
			 break;
			}
			if(str_match(b_lista_adlgn, nptrs((const void * const *)b_lista_adlgn), palabra, -2, 0)!=-1) {
				encontrado=TRUE;
			}
			break;
		case 'd':
			if(str_match(dex_lista_adlgn, nptrs((const void * const *)dex_lista_adlgn), palabra, 0, 0)!=-1) {
				encontrado=TRUE;
				break;
			}
			if(str_match(d_lista_adlgn, nptrs((const void * const *)d_lista_adlgn), palabra, -2, 0)!=-1){
				 encontrado=TRUE;
			}
			break;
		case 'g':
			if(str_match(gex_lista_adlgn, nptrs((const void * const *)gex_lista_adlgn), palabra,0, 0)!=-1){
				encontrado=TRUE;
				break;
			}
			if(str_match(g_lista_adlgn, nptrs((const void * const *)g_lista_adlgn), palabra, -2, 0)!=-1) {
				encontrado=TRUE;
			}
			break;
		case 'L': // caso bail...
			for(i=3;i<strlen(palabra);i++)
			{
				temp[i-3]=palabra[i];
			}
			if(str_match(lex_lista_adlgn, nptrs((const void * const *)lex_lista_adlgn), temp,0 , 0)!=-1){
				encontrado=TRUE;
			}
			if(!encontrado)
				if(str_match(l_lista_adlgn, nptrs((const void * const *)l_lista_adlgn), temp, -2, 0)!=-1){
					encontrado=TRUE;
				}
			break;
		case 'l':
			if(str_match(lex_lista_adlgn, nptrs((const void * const *)lex_lista_adlgn), palabra,0 , 0)!=-1){
				encontrado=TRUE;
				break;
			}
			if(str_match(l_lista_adlgn, nptrs((const void * const *)l_lista_adlgn), palabra, -2, 0)!=-1) {
				encontrado=TRUE;
			}
			break;
		case 'N': // caso bain...
			for(i=3;i<strlen(palabra);i++)
			{
				temp[i-3]=palabra[i];
			}
			temp[i]='\0';
			if(str_match(nex_lista_adlgn, nptrs((const void * const *)nex_lista_adlgn), temp, 0, 0)!=-1){
				encontrado=TRUE;
			}
			if(!encontrado)
				if(str_match(n_lista_adlgn, nptrs((const void * const *)n_lista_adlgn), temp, -2, 0)!=-1) {
					encontrado=TRUE;
			}
			break;
		case 'n':
			if(str_match(nex_lista_adlgn, nptrs((const void * const *)nex_lista_adlgn), palabra,0 , 0)!=-1){
				encontrado=TRUE;
				break;
			}
			if(str_match(n_lista_adlgn, nptrs((const void * const *)n_lista_adlgn), palabra, -2, 0)!=-1) {
				encontrado=TRUE;
			}
			break;
		case 'z':
			if(str_match(zex_lista_adlgn, nptrs((const void * const *)zex_lista_adlgn), palabra,0 , 0)!=-1){
				encontrado=TRUE;
				break;
			}
			if(str_match(z_lista_adlgn, nptrs((const void * const *)z_lista_adlgn), palabra, -2, 0)!=-1) {
				encontrado=TRUE;
			}
			break;
		default:
			break;
	}
	return(encontrado);
}

/**********************************************************/
