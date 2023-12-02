/******************************************************************************/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/

AhoTTS: A Text-To-Speech system for Basque*, developed by Aholab 
Signal Processing Laboratory at the University of the Basque Country (UPV/EHU). 
Its acoustic engine is based on hts_engine** and it uses AhoCoder'' as vocoder.
(Read COPYRIGHT_and_LICENSE_code.txt for more details)
--------------------------------------------------------------------------------

*Linguistic processing for Basque and Spanish, Vocoder (Ahocoder) and
integration by Aholab UPV/EHU.

''AhoCoder is an HNM-based vocoder for Statistical Synthesizers
http://aholab.ehu.es/ahocoder/

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Copyrights:
	*1997-2015  Aholab Signal Processing Laboratory, University of the Basque
	 Country (UPV/EHU)
    	''2011-2015 Aholab Signal Processing Laboratory, University of the Basque
	  Country (UPV/EHU)

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Licenses:
	*GPL-3.0+
	''Modified BSD (Compatible with GNU GPL)

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

Nombre fuente................ httsdo.cpp
Nombre paquete............... aHoTTS
Lenguaje fuente.............. C++
Estado....................... -
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Borja Etxebarria
.............................

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
2.0.3	 02/10/11  Inaki     add synthesize API (y soporte para idiomas festival)
2.0.2	 15/12/10  Inaki     integrate HTS Synthesis Method
2.0.1	 03/10/07  Inaki     integrate Corpus Synthesis Method
2.0.0    27/06/03  lander    integrate HNS Synthesis Module
1.0.1    22/06/00  richie    minor bugfix
1.0.0    31/01/00  borja     codefreeze aHoTTS v1.0
0.0.0    06/02/98  borja     Codificacion inicial.

======================== Contenido ========================
<DOC>
</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#define DEBUG_SHELLx

#include <assert.h>
#include "uti.h"

#include "htts.hpp"
#include "httsdo.hpp"
#include "httsmsg.h"

#ifdef HTTS_LANG_ES
#include "es_lingp.hpp"
#include "es_hdic.hpp"
// #include "es_tex.hpp"  // $$$ aun no hay tokenizer para castellano...
#include "es_t2l.hpp"  // $$$ EVA YA tenemos tokenizer en castellano


#endif

#ifdef HTTS_LANG_EU
#include "eu_lingp.hpp"
#include "eu_hdic.hpp"
#include "eu_t2l.hpp"
#endif


#ifdef HTTS_METHOD_HTS
#include "hts.hpp"
#endif


/**********************************************************/

#ifdef HTTS_METHVARS
#ifdef HTTS_METHOD_HTS           //INAKI
BOOL __htts_method_hts=TRUE;
#else
BOOL __htts_method_hts=FALSE;
#endif
#endif

/**********************************************************/

#ifdef HTTS_LANGVARS
#ifdef HTTS_LANG_ES
BOOL __htts_lang_es=TRUE;
#else
BOOL __htts_lang_es=FALSE;
#endif
#ifdef HTTS_LANG_EU
BOOL __htts_lang_eu=TRUE;
#else
BOOL __htts_lang_eu=FALSE;
#endif
#endif


HTTSDo::HTTSDo( VOID )
{
	created=FALSE;
	flushbuf=0;
	hdic=NULL;
	t2u=NULL;
	lingp=NULL;
	utt=NULL;
	u2w=NULL;

	localdb=TRUE;

/* El primero definido sera el idioma por defecto */
#if defined(HTTS_LANG_EU)
	lang = Lang::eu;
#elif defined(HTTS_LANG_ES)
	lang = Lang::es;
#endif

/* El primero definido sera el metodo por defecto */
#if defined(HTTS_METHOD_HTS) //INAKI
	smethod = "HTS";
#endif

	hdicdbname = "hdic";


	modelpth="";
	dbpros="";
	modelpow="";
	modeldur="";
	modelpau="";
}

/**********************************************************/

HTTSDo::~HTTSDo( )
{
	destroy();
}

/**********************************************************/

VOID HTTSDo::destroy( VOID )
{
#define DELIT(x) if (x) { delete x; x=NULL; }
	DELIT(hdic);
	DELIT(t2u);
	DELIT(lingp);
	DELIT(utt);
	DELIT(u2w);
}

/**********************************************************/

BOOL HTTSDo::create( VOID * db )
{
	int numerror=0;

	assert(!created);

	assert(!hdic);
	assert(!t2u);
	assert(!lingp);
	assert(!utt);
	assert(!u2w);

	BOOL ok;
	const CHAR* npth=NULL;
	DOUBLE d=0;

#ifdef HTTS_LANG_EU
	if (lang == Lang::eu) {
		ok=TRUE;
		lingp = new LangEU_LingP;
		hdic = new LangEU_HDicDB;
		t2u = new LangEU_TextToList;
	}
#endif
#ifdef HTTS_LANG_ES
	if (lang == Lang::es) {
		ok=TRUE;
		lingp = new LangES_LingP;
		hdic = new LangES_HDicDB;
		t2u = new LangES_TextToList;  // $$$ EVA como YA hay tokenizer ES ...
//		t2u = new LangEU_TextToList;  // $$$ ...usamos el tokenizer EU

	}
#endif

	if (!(lingp && t2u)) {numerror=1;goto error;}

	if (!hdic)  goto error;


	ok=FALSE;
#ifdef HTTS_METHOD_HTS //INAKI
	if (!strcmp(smethod,"HTS")) {
		ok=TRUE;
		utt = new UttPh;
		u2w = new HTS_U2W;
		if (!(utt&&u2w))  {numerror=31;goto error;}
		hts=TRUE;
	}
	else
		hts =FALSE;
#endif
	if (!ok) htts_error("Invalid synthesis method (%s)",(const CHAR*)smethod);

	hdic->create(hdicdbname);

	//$$$ Richie added. Muru Changed HDicDBName -> HDicDB
	lingp->set("HDicDB",hdicdbname);

	if (!lingp->create())  {numerror=15;goto error;}
	if (!utt->create())  {numerror=16;goto error;}
	((UttWS*)utt)->setHDicDB(hdic);
	if (!t2u->create((UttWS*)utt,hdic))  {numerror=17;goto error;}

#ifdef HTTS_METHOD_HTS
	if (!strcmp(smethod,"HTS")) if (! ((HTS_U2W*)u2w)->create(lingp->get("Lang")))  {numerror=32;goto error;} //INAKI
#endif

/* Configurar pitch nominal si es posible, si no, pitch 100Hz */
	/*npth=get("NominalPth");
	d=npth?atof(npth):0;
	if (!set("PthMean",d?npth:"100"))  {numerror=28;goto error;}*/

	/* cableamos un par de opciones... */
	lingp->set("PhMap_jw2iu","n");
	lingp->set("Lang",Lang2Str(lang)); // XXX


	ackpending=FALSE;
	created=TRUE;
	flushbuf=0;

	return TRUE;

error:
    htts_error("Error {d}. Can't create HTTS module", numerror);
	//ts_error("Can't create HTTS module");
	destroy();
	return FALSE;
}

/**********************************************************/

VOID *HTTSDo::getDB(VOID)
{
#ifdef HTTS_THREADSAFE
	// si no se puede obtener el modo o es distinto de Preload, no es thread-safe
		//const CHAR *s=dphdb->get("DBLoadMode");
		//if (!s || strcmp(s,"Preload")) return NULL;

#endif
		//return (VOID*)dphdb;
	//}
	// si es de formantes o no esta creada la base de datos, NULL
	return NULL;
}

/**********************************************************/

BOOL HTTSDo::advance( VOID )
{
	BOOL flush;
	Utt* u;
	const DOUBLE* vec;
	INT len;
	BOOL ret;

	vec = u2w->output(&len,&flush);
	if (vec||flush) return TRUE;  // si u2w genera salida valida, OK

/*... si no hay mas tramas de formantes o si es el sinte de
difonemas, intentamos obtener una nueva utt de t2u, y sintetizarla.
pero primero, borramos la lista vieja del primer modulo (t2u)*/

	if (ackpending) {  // solo si hace falta
		t2u->outack();
		ackpending= FALSE;
	}

	/* manejamos el buffer de flushes {flushbuf}. Lo necesitamos porque
	los flushes no llegan a t2u mientras se esta procesando una frase,
	ya que el ack() a este modulo se envia cuando se termina
	con la frase en los demas modulos, y no tras el tu2->output() */
	while (flushbuf>0) {
		if (t2u->flush()) flushbuf--; else break;
	}

	u = t2u->output(&flush);
	if (u) {  // estupendo, obtuvimos una utt
		lingp->utt_lingp(u);  // la procesamos
#ifdef HTTS_METHOD_HTS
			if(hts){
				ret=u2w->input(u);
			}
			else{
#endif

#ifdef HTTS_METHOD_HTS
		}
#endif

		assert(ret);
		ackpending = TRUE;
		return TRUE;
	}
	if (flush) {  // estupendo, obtuvimos un flush
			ret=u2w->flush();
		assert(ret);
		ackpending = TRUE;
		return TRUE;
	}

	(void)ret;
	// si llegamos aqui, no hemos avanzado nada, :(
	return FALSE;
}

/**********************************************************/

INT HTTSDo::input( const CHAR * str )
{
	assert(created);
	assert(t2u);
	INT ret=t2u->input(str);
	advance();
	return ret;
}

/**********************************************************/

BOOL HTTSDo::input( CHAR ch )
{
	assert(created);
	assert(t2u);
	BOOL ret=t2u->input(ch);
	advance();
	return ret;
}

/**********************************************************/

BOOL HTTSDo::flush( VOID )
{
	assert(created);
	flushbuf++;
	advance();
	return TRUE;
}

/**********************************************************/

const DOUBLE* HTTSDo::output( INT *len, BOOL *flush, INT mode, VOID *cb_n )
{
	const DOUBLE *p = NULL;

	assert(created);
	assert(u2w);
	while (1) {
		BOOL flu=FALSE;
		p = u2w->output(len,&flu);  // leer salida
		if (flush) *flush=flu;  // *flush puede ser NULL, por eso usamos flu temporalmente

		if (p || flu) break;  // si hay salida valida, return
		// si no hay salida valida:
		if (advance()) continue;  // si hay flujo entre modulos => repite
		// si no, nada que hacer...
		if (mode != HTTS_CB_NOCALL) {  // ...salvo llamar al callback y repetir
			callback(cb_n);
			if (mode==HTTS_CB_CALL1) mode=HTTS_CB_NOCALL; // cancelar el callback si esta en modo de 1-vez
			continue;
		}
		break;
	}
	return p;
}

/**********************************************************/

BOOL HTTSDo::isFlush( VOID )
{
	assert(created);
	assert(u2w);
	return u2w->isFlush();
}

/**********************************************************/

#ifdef HTTS_INTERFACE_WAVEMARKS
const Mark1DList & HTTSDo::mrkget( VOID )
{
	assert(created);
	assert(u2w);
	return u2w->mrkget();
}
#endif

/**********************************************************/

#ifdef HTTS_INTERFACE_WAVEMARKS
VOID HTTSDo::mrkdel( VOID )
{
	assert(created);
	assert(u2w);
	u2w->mrkdel();
}
#endif

/**********************************************************/

#ifdef HTTS_INTERFACE_WAVEMARKS
BOOL HTTSDo::outack( INT n, BOOL mrkdel )
{
	assert(created);
	assert(u2w);
	return u2w->outack(n,mrkdel);
}
#else
BOOL HTTSDo::outack( INT n )
{
	assert(created);
	assert(u2w);
	return u2w->outack(n);
}
#endif

/**********************************************************/

#ifdef HTTS_TIMEEVS
INT HTTSDo::get_timeev( VOID )
{
	assert(created);
	assert(u2w);
	return u2w->get_timeev();
}
#endif

/**********************************************************/

BOOL HTTSDo::setLang(Lang l){
    if (created) return FALSE;
    lang = l;
#ifdef DEBUG_SHELL
		htts_warn("HTTSDo::set - Language value captured [%s]", Lang2Str(l));
#endif
    return TRUE;
}

BOOL HTTSDo::setMethod(const CHAR* val){
    if (created) return FALSE;
    smethod= val;
#ifdef DEBUG_SHELL
    htts_warn("HTTSDo::set - Method value captured [%s]", smethod.chars());
#endif
    return TRUE;
}

BOOL HTTSDo::setPthModel(const CHAR* val){
    if (created) return FALSE;
    modelpth= val;
#ifdef DEBUG_SHELL
    htts_warn("HTTSDo::set - PthModel value captured [%s]", modelpth.chars());
#endif
    return TRUE;
}

BOOL HTTSDo::setProsDBName(const CHAR* val){
    if (created) return FALSE;
    dbpros= val;
#ifdef DEBUG_SHELL
    htts_warn("HTTSDo::set - ProsDBName value captured [%s]", dbpros.chars());
#endif
    return TRUE;
}

BOOL HTTSDo::setPowModel(const CHAR* val){
    if (created) return FALSE;
    modelpow= val;
#ifdef DEBUG_SHELL
    htts_warn("HTTSDo::set - PowModel value captured [%s]", modelpow.chars());
#endif
    return TRUE;
}

BOOL HTTSDo::setDurModel(const CHAR* val){
    if (created) return FALSE;
    modeldur= val;
#ifdef DEBUG_SHELL
    htts_warn("HTTSDo::set - DurModel value captured [%s]", modeldur.chars());
#endif
    return TRUE;
}

BOOL HTTSDo::setPauModel(const CHAR* val){
    if (created) return FALSE;
    modelpau= val;
#ifdef DEBUG_SHELL
    htts_warn("HTTSDo::set - PauModel value captured [%s]", modelpau.chars());
#endif
    return TRUE;
}

#ifdef HTTS_DIPHONE
BOOL HTTSDo::setDBName(const CHAR* val){
    if (created) return FALSE;
    dbname= val;
#ifdef DEBUG_SHELL
    htts_warn("HTTSDo::set - DBName value captured [%s]", dbname.chars());
#endif
    return TRUE;
}
#endif

#ifdef HTTS_DIPHONE
BOOL HTTSDo::setDBLoadMode(const CHAR* val){
    if (created) return FALSE;
    dbmode= val;
#ifdef DEBUG_SHELL
    htts_warn("HTTSDo::set - DBLoadMode value captured [%s]", dbmode.chars());
#endif
    return TRUE;
}
#endif

BOOL HTTSDo::setHDicDBName(const CHAR* val){
    if (created) return FALSE;
    hdicdbname= val;
#ifdef DEBUG_SHELL
    htts_warn("HTTSDo::set - HDicDBName value captured [%s]", hdicdbname.chars());
#endif
    return TRUE;
}

BOOL HTTSDo::setDefEmo(const CHAR* val){
    if (!created) return FALSE;
    t2u->set("DefEmo", val);
    return TRUE;
}

BOOL HTTSDo::setDefIntEmo(const CHAR* val){
    if (!created) return FALSE;
    t2u->set("DefIntEmo", val);
    return TRUE;
}


BOOL HTTSDo::set( const CHAR* param, const CHAR* val ){
	if (t2u && t2u->set(param,val)){ return TRUE; }
	if (lingp && lingp->set(param,val)){ return TRUE; }
	if (u2w && u2w->set(param,val)){ return TRUE;}
	return FALSE;
}

/**********************************************************/

Lang HTTSDo::getLang(){
    return lang;
}
const CHAR* HTTSDo::getDefEmo(){ return emo; }
const CHAR* HTTSDo::getDefIntEmo(){ return emoint; }
const CHAR* HTTSDo::getMethod(){ return smethod; }
const CHAR* HTTSDo::getPthModel(){ return lingp?lingp->get("PthModel"):(const CHAR *)modelpth; }
const CHAR* HTTSDo::getProsDBName(){ return lingp?lingp->get("ProsDBName"):(const CHAR *)dbpros; }
const CHAR* HTTSDo::getPowModel(){ return lingp?lingp->get("PowModel"):(const CHAR *)modelpow; }
const CHAR* HTTSDo::getDurModel(){ return lingp?lingp->get("DurModel"):(const CHAR *)modeldur; }
const CHAR* HTTSDo::getPauModel(){ return lingp?lingp->get("HDicDBName"):(const CHAR *)modelpau; }
const CHAR* HTTSDo::getHDicDBName(){ return hdicdbname; }
const CHAR* HTTSDo::getQueryMethods(){ return
// XXX: if it's not defined this breaks!!
#ifdef HTTS_METHOD_HTS
		"[HTS]"    //INAKI
#endif
    ; }
const CHAR* HTTSDo::getQueryLanguages(){ return
// XXX: obtain this from langs.hpp
// XXX: if both are defined this breaks!!
#ifdef HTTS_LANG_ES
		"[es]"
#endif
#ifdef HTTS_LANG_EU
		"[eu]"
#endif
    ; }

#ifdef HTTS_METHOD_HTS //INAKI
uint HTTSDo::getSRate(){ return 16000; }
#endif


const CHAR* HTTSDo::get( const CHAR* param )
{
    const CHAR * ret;
	if (u2w) ret = u2w->get(param); if (ret) return ret;
	if (lingp) ret = lingp->get(param); if (ret) return ret;
	if (t2u) ret = t2u->get(param); if (ret) return ret;
// Por ahora hdic no tiene get
//	if (hdic) ret = hdic->get(param); if (ret) return ret;
	return ret;
}

/**********************************************************/
/**********************************************************/
//inaki
//devuelve número de muestras sintetizadas y las almacena en short **samples
int HTTSDo::synthesize_do_next_sentence( const CHAR *lang, short **samples){
	String labels_string="";
	int num_muestras=0;
	Utt* u=NULL;
	BOOL flush=FALSE;
	u = t2u->output(&flush);
	if (u) {  // estupendo, obtuvimos una utt
		ackpending = TRUE;
		lingp->utt_lingp(u);  // la procesamos


		//String labels_string_tmp;
		((HTS_U2W*)u2w)->pho2hts((UttPh*)u, labels_string, TRUE);	//convertimos a labels
		//labels_string+=labels_string_tmp;
		t2u->outack();
		//	u = t2u->output(&flush);
		*samples=((HTS_U2W*)u2w)->xinput_labels(labels_string, &num_muestras);


	}
	return num_muestras;
	//*out=strdup(Silabificado);
	
}
/**********************************************************/
/**********************************************************/
//inaki
BOOL HTTSDo::synthesize_do_input( const CHAR *str, const CHAR *lang , BOOL InputIsFile /*=FALSE*/, const CHAR *data_path){

	assert(created);
	strcpy(DataPath, data_path);
	//para euskera y castellano usamos código ahoTTS
	
		assert(t2u);
		INT ret=t2u->input(str);
		flushbuf++;
		BOOL flush=FALSE;


		String Silabificado="";
		UttI pa, pi;
	  if (ackpending) {  // solo si hace falta
			t2u->outack();
			ackpending= FALSE;
		}

		/* manejamos el buffer de flushes {flushbuf}. Lo necesitamos porque
		los flushes no llegan a t2u mientras se esta procesando una frase,
		ya que el ack() a este modulo se envia cuando se termina
		con la frase en los demas modulos, y no tras el tu2->output() */
		while (flushbuf>0) {
			if (t2u->flush()) flushbuf--; else break;
		}

		return TRUE;
	

	
}



/////////////////////////////////////////////////////////////////
