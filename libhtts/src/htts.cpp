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

Nombre fuente................ htts.cpp
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


/*<DOC>*/
/**********************************************************/
/* El constructor de la clase no tiene parametros. El usuario
debe llamar al metodo create() para inicializar la clase.
Antes de efectuar el create() se pueden configurar algunos
parametros con los métodos set correspondientes */
HTTS::HTTS( VOID )
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

HTTS::~HTTS( )
{
	destroy();
}

/**********************************************************/

VOID HTTS::destroy( VOID )
{
#define DELIT(x) if (x) { delete x; x=NULL; }
	DELIT(hdic);
	DELIT(t2u);
	DELIT(lingp);
	DELIT(utt);
	DELIT(u2w);
}

/**********************************************************/

/*<DOC>*/
/**********************************************************/
/* Inicializacion interna de la clase. {devuelve} TRUE
si la clase se ha inicializado correctamente, o FALSE en
caso de error.

El usuario debe llamar a esta funcion antes de empezar a
hacer uso de la clase. Antes del create() se pueden
inicializar algunos parametros.

Existe una version alternativa create(db) que recibe una
base de datos externa. */
BOOL HTTS::create( VOID * db )
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

/*<DOC>*/
/**********************************************************/
/* {devuelve} un puntero a la base de datos utilizada por
un modulo HTTS. El objeto debe estar ya inicializado
(el metodo create() ya tiene que haberse ejecutado. Esta
base de datos puede ser utilizada/compartida por otros
modulos HTTS (ver metodos create()). */
VOID *HTTS::getDB(VOID)
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

BOOL HTTS::advance( VOID )
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


/*<DOC>*/
/**********************************************************/
/* Interfaz de entrada de texto. El usuario envia en {str}
un segmento de texto a sintetizar. La funcion {devuelve} el
numero de caracteres que se han aceptado. En llamadas posteriores
(tras haber hecho output()) se deben enviar los caracteres que
no han sido aceptados.

La funcion puede no aceptar mas caracteres porque tiene los
bufferes internos llenos, y el usuario debe llamar a output()
para obtener la salida disponible y hacer sitio.

La interfaz de entrada input() no tiene porque generar una
salida de audio directamente. El objeto HTTS va acumulando texto
hasta que compone una frase completa siguiendo ciertas reglas
sintacticas. Para asegurar que el sintetizador se vea forzado a
generar salida de audio para TODO el texto que ha aceptado, se
puede llamar a la funcion flush(). Esto asegura un purgado
interno completo. */
INT HTTS::input( const CHAR * str )
{
	assert(created);
	assert(t2u);
	INT ret=t2u->input(str);
	advance();
	return ret;
}

/**********************************************************/

/*<DOC>*/
/**********************************************************/
/* Interfaz de entrada de texto. El usuario envia en {ch}
un caracter de texto a anyadir a la cadena a sintentizar.
La funcion {devuelve} TRUE si acepta el caracter, FALSE si
no lo acepta.

La funcion puede no aceptar mas caracteres porque tiene los
bufferes internos llenos, y el usuario debe llamar a output()
para obtener la salida disponible y hacer sitio.

La interfaz de entrada input() no tiene porque generar una
salida de audio directamente. El objeto HTTS va acumulando texto
hasta que compone una frase completa siguiendo ciertas reglas
sintacticas. Para asegurar que el sintetizador se vea forzado a
generar salida de audio para TODO el texto que ha aceptado, se
puede llamar a la funcion flush(). Esto asegura un purgado
interno completo. */
BOOL HTTS::input( CHAR ch )
{
	assert(created);
	assert(t2u);
	BOOL ret=t2u->input(ch);
	advance();
	return ret;
}

/**********************************************************/

/*<DOC>*/
/**********************************************************/
/* La interfaz de entrada input() no tiene porque generar una
salida de audio directamente. El objeto HTTS va acumulando texto
hasta que compone una frase completa siguiendo ciertas reglas
sintacticas. Para asegurar que el sintetizador se vea forzado a
generar salida de audio para TODO el texto que ha aceptado, se
puede llamar a esta funcion flush(). Esto asegura un purgado
interno completo. El caso de que el flush() se acepte, se
{devuelve} TRUE. En caso de que no se acepte, se {devuelve} FALSE.*/

BOOL HTTS::flush( VOID )
{
	assert(created);
	flushbuf++;
	advance();
	return TRUE;
}

/**********************************************************/

/*<DOC>*/
/**********************************************************/
/* Esta es la funcion principal para obtener una salida del
modulo de sintesis HTTS. Adicionalmente, los metodos mrkget(),
isFlush(), y get_timeev() permiten obtener informacion adicional
de salida.

output() recibe cuatro argumentos:

{len} es un puntero a un entero INT en el que se devuelve el numero
de muestras de salida.

{flush} es un valor a un booleano BOOL en el que se indica si
la salida es un flush (TRUE) o es audio (FALSE). Este valor
se puede no enviar poniendolo a NULL, entonces se puede saber si
hubo un flush con el metodo isFlush().

{mode} es un codigo en el que el usuario indica el modo de trabajo
de la funcion output(), descrito posteriormente.

{cb_n} es un puntero que recibe la funcion y que se pasa directamente
al metodo callback() si se le llama.

La funcion {devuelve} un puntero a un array interno que mantiene
las muestas de audio en formato DOUBLE (en principio en el rango
[-1, +1], que puede ser ocasionalmente superado si se ha configurado
un volumen muy elevado. Ver los metodos convDOUBLE2INT16() y
convDOUBLE2INT16_clip() para convertir este array DOUBLE[] a INT16[]).
Este array interno NO debe ser modificado. El usuario debe copiar
tantas muestas como desee (se indican cuantas en outack()). Llamadas
posteriores a outack() y output() modificaran este array interno..

Cuando no hay audio de salida disponible (porque no queda texto de
entrada que procesar), la funcion {devuelve} NULL. Tambien se
{devuelve} NULL cuando se recibe un flush, situacion que se puede
comprobar porque ademas en {flush} se escribe TRUE, o mediante la
funcion isFlush()..

Despues de output() el usuario debe llamar a outack() para indicar
cuantas muestras acepta, o para indicar que se acepta el flush(). En
caso de que no hay muestras de salida ni se trate de un flush,
({devuelve}=NULL y  {flush}=FALSE) no es necesario llamar a outack().

Modos de funcionamiento del metodo output(): Se controlan con el
parametro {mode} que indica si se debe o no utilizar el metodo
callback(). En caso de que el objeto HTTS tenga texto suficiente
para generar una salida, el callback no se ejecuta y el usuario
obtendra muestas o flushes de salida. Cuando el objeto no pueda
generar una salida, si se ha configurado con {mode} que se pueda
llamar al callback(), este se ejecuta asumiendo que probablemente
alimentara la entrada input() con nuevo texto que permita generar
salida. Es responsabilidad del usuario definir el callback() en
una clase derivada con codigo que llame a input() con nuevo texto
a sintetizar (o llamar a flush() para propagar un flush).

El parametro {mode} puede valer:

* mode=HTTS_CB_NOCALL  --> no se llama al callback. El usuario
debe llamar en la secuencia principal del programa al metodo
input() para alimentar con texto de entrada al modulo HTTS.

* mode=HTTS_CB_CALL1  --> se llama al callback como mucho una vez.
Si es suficiente para obtener salida, estupendo. Si no lo es, output()
{devuelve} NULL y el usuario tendra que seguir haciendo llamadas a
output() hasta que se obtenga una salida.

* mode=HTTS_CB_BLOCKCALL  --> llamada bloqueante a output(), llamadas
infinitas al callback() hasta obtener una salida. El metodo
output() por tanto siempre devuelve una salida no nula (sean
muestras o flushes) si el usuario envia texto de entrada, o el
programa quedara bloqueado si no se envia una entrada valida!
*/
const DOUBLE* HTTS::output( INT *len, BOOL *flush, INT mode, VOID *cb_n )
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

/*<DOC>*/
/**********************************************************/
/* Indica si el ultimo output() NULL fue un flush o no.
En el parametro {flush} de la funcion output() se puede
obtener el mismo resultado que {devuelve} esta funcion. */

BOOL HTTS::isFlush( VOID )
{
	assert(created);
	assert(u2w);
	return u2w->isFlush();
}

/**********************************************************/


/*<DOC>*/
/**********************************************************/
/* {devuelve} la lista de marcas disponibles hasta el momento.
Cuando se hace outack() el usuario puede indicar que se borren
si ya las ha procesado. */
#ifdef HTTS_INTERFACE_WAVEMARKS
const Mark1DList & HTTS::mrkget( VOID )
{
	assert(created);
	assert(u2w);
	return u2w->mrkget();
}
#endif

/**********************************************************/

/*<DOC>*/
/**********************************************************/
/* Borra la lista de marcas actual. La lista se borra
automaticamente tras el outack() de cada flush, pero el resto
del tiempo las marcas se van acumulando salvo si se borran
en outack() o con esta funcion mrkdel().

Por tanto es importante leer las marcas con mrkget() y
borrarlas con esta funcion, o si no pasar el parametro
{mrkdel}==TRUE a la funcion outack() (es el valor por defecto
de dicho parametro en caso de que no se indique), si no
las marcas se van acumulando hasta el infinito! */
#ifdef HTTS_INTERFACE_WAVEMARKS
VOID HTTS::mrkdel( VOID )
{
	assert(created);
	assert(u2w);
	u2w->mrkdel();
}
#endif

/**********************************************************/

/*<DOC>*/
/**********************************************************/
/* El usuario debe llamar a outack() tras llamar a output()
para indicar al objeto de sintesis cuantas muestras de
salida (o flush) acepta. Mientras no se haga esto, output()
devolvera siempre las mismas muestras (o flush)!!!.

En caso de que el ACK se acepte, la funcion {devuelve} TRUE,
en caso de que no se acepte, FALSE (esto solo puede pasar si
el usuario no ha llamado previamente a output()).

{n} es el numero de muestras que se aceptan (o cualquier valor
si se trata de aceptar un flush).

Si las marcas WAVEMARKS estan disponibles, tambien se debe
indicar con el parametro {mrkdel} si se debe borrar la lista
de marcas acumuladas hasta el momento (antes de borrarlas
se pueden obtener para copiarlas a otro sitio con el
metodo mrkget(). Por defecto, si no se indica {mrkdel}, se
toma valor TRUE.  */

#ifdef HTTS_INTERFACE_WAVEMARKS
BOOL HTTS::outack( INT n, BOOL mrkdel )
{
	assert(created);
	assert(u2w);
	return u2w->outack(n,mrkdel);
}
#else
BOOL HTTS::outack( INT n )
{
	assert(created);
	assert(u2w);
	return u2w->outack(n);
}
#endif

/**********************************************************/

/*<DOC>*/
/**********************************************************/
/* get_timeev() permite obtener las marcas de
tiempo "ligeras", TimeEvs.

NOTA NOTA NOTA: actualmente HTTS no tiene mecanismos para
incrustar eventos temporales TimeEvs en el texto de entrada, por
lo que por ahora este metodo nunca devolvera ningun evento,
y se {devuelve} siempre el valor 0. Se incluye solamente por
completar el interfaz de salida HTTS, con vistas al futuro.

La funcion {devuelve} el valor del evento temporal actual.
El valor 0 indica que no hay marcas temporales. El valor que
se {devuelve} es siempre el del ultimo evento temporal marcado.
Puede suceder que el sistema se salte algun evento temporal,
pasando directamente al siguiente, por ello a la hora de insertar
eventos temporales conviene utilizar valores enteros consecutivos
(siempre distintos de 0), de forma que siempre sabemos HASTA que
evento hemos llegado ya. */

#ifdef HTTS_TIMEEVS
INT HTTS::get_timeev( VOID )
{
	assert(created);
	assert(u2w);
	return u2w->get_timeev();
}
#endif

/**********************************************************/

/*<DOC>*/
/**********************************************************/
/* Permite configurar parametros con el valor {val}.
En caso de que el parametro no sea reconocido, no pueda
configurarse en dicho momento, o {val} no sea valido, la
funcion {devuelve} FALSE. Si el parametro se configura
correctamente {devuelve} TRUE. */

BOOL HTTS::setLang(Lang l){
    if (created) return FALSE;
    lang = l;
#ifdef DEBUG_SHELL
		htts_warn("HTTS::set - Language value captured [%s]", Lang2Str(l));
#endif
    return TRUE;
}

BOOL HTTS::setMethod(const CHAR* val){
    if (created) return FALSE;
    smethod= val;
#ifdef DEBUG_SHELL
    htts_warn("HTTS::set - Method value captured [%s]", smethod.chars());
#endif
    return TRUE;
}

BOOL HTTS::setPthModel(const CHAR* val){
    if (created) return FALSE;
    modelpth= val;
#ifdef DEBUG_SHELL
    htts_warn("HTTS::set - PthModel value captured [%s]", modelpth.chars());
#endif
    return TRUE;
}

BOOL HTTS::setProsDBName(const CHAR* val){
    if (created) return FALSE;
    dbpros= val;
#ifdef DEBUG_SHELL
    htts_warn("HTTS::set - ProsDBName value captured [%s]", dbpros.chars());
#endif
    return TRUE;
}

BOOL HTTS::setPowModel(const CHAR* val){
    if (created) return FALSE;
    modelpow= val;
#ifdef DEBUG_SHELL
    htts_warn("HTTS::set - PowModel value captured [%s]", modelpow.chars());
#endif
    return TRUE;
}

BOOL HTTS::setDurModel(const CHAR* val){
    if (created) return FALSE;
    modeldur= val;
#ifdef DEBUG_SHELL
    htts_warn("HTTS::set - DurModel value captured [%s]", modeldur.chars());
#endif
    return TRUE;
}

BOOL HTTS::setPauModel(const CHAR* val){
    if (created) return FALSE;
    modelpau= val;
#ifdef DEBUG_SHELL
    htts_warn("HTTS::set - PauModel value captured [%s]", modelpau.chars());
#endif
    return TRUE;
}

#ifdef HTTS_DIPHONE
BOOL HTTS::setDBName(const CHAR* val){
    if (created) return FALSE;
    dbname= val;
#ifdef DEBUG_SHELL
    htts_warn("HTTS::set - DBName value captured [%s]", dbname.chars());
#endif
    return TRUE;
}
#endif

#ifdef HTTS_DIPHONE
BOOL HTTS::setDBLoadMode(const CHAR* val){
    if (created) return FALSE;
    dbmode= val;
#ifdef DEBUG_SHELL
    htts_warn("HTTS::set - DBLoadMode value captured [%s]", dbmode.chars());
#endif
    return TRUE;
}
#endif

BOOL HTTS::setHDicDBName(const CHAR* val){
    if (created) return FALSE;
    hdicdbname= val;
#ifdef DEBUG_SHELL
    htts_warn("HTTS::set - HDicDBName value captured [%s]", hdicdbname.chars());
#endif
    return TRUE;
}

BOOL HTTS::setDefEmo(const CHAR* val){
    if (!created) return FALSE;
    t2u->set("DefEmo", val);
    return TRUE;
}

BOOL HTTS::setDefIntEmo(const CHAR* val){
    if (!created) return FALSE;
    t2u->set("DefIntEmo", val);
    return TRUE;
}


// XXX: REMOVE ME
BOOL HTTS::set( const CHAR* param, const CHAR* val ){
	if (t2u && t2u->set(param,val)){ return TRUE; }
	if (lingp && lingp->set(param,val)){ return TRUE; }
	if (u2w && u2w->set(param,val)){ return TRUE;}
	return FALSE;
}

/**********************************************************/

/*<DOC>*/
/**********************************************************/
/* Metodos que {devuelven} una cadena con el valor actual del
parametro.

La cadena devuelta es un valor interno. El usuario NO debe bajo
ningun concepto modificarla. Llamadas posteriores a esta funcion
borran el valor anterior.
*/

Lang HTTS::getLang(){
    return lang;
}
const CHAR* HTTS::getDefEmo(){ return emo; }
const CHAR* HTTS::getDefIntEmo(){ return emoint; }
const CHAR* HTTS::getMethod(){ return smethod; }
const CHAR* HTTS::getPthModel(){ return lingp?lingp->get("PthModel"):(const CHAR *)modelpth; }
const CHAR* HTTS::getProsDBName(){ return lingp?lingp->get("ProsDBName"):(const CHAR *)dbpros; }
const CHAR* HTTS::getPowModel(){ return lingp?lingp->get("PowModel"):(const CHAR *)modelpow; }
const CHAR* HTTS::getDurModel(){ return lingp?lingp->get("DurModel"):(const CHAR *)modeldur; }
const CHAR* HTTS::getPauModel(){ return lingp?lingp->get("HDicDBName"):(const CHAR *)modelpau; }
const CHAR* HTTS::getHDicDBName(){ return hdicdbname; }
const CHAR* HTTS::getQueryMethods(){ return
// XXX: if it's not defined this breaks!!
#ifdef HTTS_METHOD_HTS
		"[HTS]"    //INAKI
#endif
    ; }
const CHAR* HTTS::getQueryLanguages(){ return
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
uint HTTS::getSRate(){ return 16000; }
#endif


const CHAR* HTTS::get( const CHAR* param )
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
int HTTS::synthesize_do_next_sentence(short **samples){
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
BOOL HTTS::synthesize_do_input( const CHAR *str, BOOL InputIsFile /*=FALSE*/, const CHAR *data_path){

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


/*<DOC>*/
/**********************************************************/
/* Interfaz de entrada de texto. El usuario envia en {ch}
un caracter de texto a anyadir a la cadena a sintentizar.
La funcion {devuelve} TRUE si acepta el caracter, FALSE si
no lo acepta.

La funcion puede no aceptar mas caracteres porque tiene los
bufferes internos llenos, y el usuario debe llamar a output()
para obtener la salida disponible y hacer sitio.

La interfaz de entrada input() no tiene porque generar una
salida de audio directamente. El objeto HTTS va acumulando texto
hasta que compone una frase completa siguiendo ciertas reglas
sintacticas. Para asegurar que el sintetizador se vea forzado a
generar salida de audio para TODO el texto que ha aceptado, se
puede llamar a la funcion flush(). Esto asegura un purgado
interno completo. */

BOOL HTTS::input_multilingual( const CHAR * str, const CHAR *data_path ,BOOL InputIsFile) //inaki
/*</DOC>*/
{
    return synthesize_do_input(str, InputIsFile, data_path);
}

int HTTS::output_multilingual(short **samples){
		return synthesize_do_next_sentence(samples);
}

/*<DOC>*/
/**********************************************************/
/* Este metodo permite convertir una muestra DOUBLE en un
INT16. La funcion NO comprueba que la muestra este en el
rango [-1,1]. En caso de salirse de este rango, se producira
wrap-arround del valor de salida, lo que en general produce
sonido muy distorsionado.

En caso de preferir que se produzca un clipping (distorsion
mucho menos molesta) en vez de wrap-arround, se puede
utilizar el metodo convDOUBLE2INT16_clip(). En principio
solo hay riesgo de clipping cuando se configura un valor
de volumen muy elevado. */

INT16 HTTS::convDOUBLE2INT16( DOUBLE d )
/*</DOC>*/
{
	return (INT16)(32767.*d);
}

/*<DOC>*/
/**********************************************************/
/* Este metodo permite convertir una muestra DOUBLE en un
INT16. La funcion comprueba que si la muestra supera el rango
de [-1,1] el valor convertido se sature al [minimo,maximo] del
rango INT16 (-32767,+32767).

Computacionalmente este metodo supone algo mas de carga que
el metodo convDOUBLE2INT16(), pero en caso de ser necesario
aplicar el clipping la calidad del sonido resulta mucho mayor.
En principio solo hay riesgo de clipping cuando se configura
un valor de volumen muy elevado. */

INT16 HTTS::convDOUBLE2INT16_clip( DOUBLE d )
/*</DOC>*/
{
	if (d<-1)  return (INT16)(-32767);
	else if (d>1) return (INT16)32767;
	else return (INT16)(32767.*d);
}

/*<DOC>*/
/**********************************************************/
/* Version de convDOUBLE2INT16() para arrays de muestras.
El array de entrada es {d} y el de salida convertido es {i16}.
La longitud de los dos arrays es {n}. */

VOID HTTS::convDOUBLE2INT16( DOUBLE d[], INT16 i16[], INT n )
/*</DOC>*/
{
	for (INT i=0; i<n; i++) i16[i] = (INT16)(32767.*d[i]);
}

/*<DOC>*/
/**********************************************************/
/* Version de convDOUBLE2INT16() para arrays de muestras.
El array de entrada es {d} y el de salida convertido es {i16}.
La longitud de los dos arrays es {n}.

En caso de que se produzca clipping en una o mas muestras,
la funcion {devuelve} TRUE, pero si no hay clippings,
{devuelve} FALSE. */

BOOL HTTS::convDOUBLE2INT16_clip( DOUBLE d[], INT16 i16[], INT n )
/*</DOC>*/
{
	BOOL over=FALSE;
	for (INT i=0; i<n; i++) {
		if (d[i]<-1) { i16[i]=(INT16)(-32767); over=TRUE; }
		else if (d[i]>1) { i16[i]=(INT16)32767; over=TRUE; }
		else i16[i] = (INT16)(32767.*d[i]);
	}
	return over;
}

/*<DOC>*/
/**********************************************************/
/* Esta funcion permite configurar la rutina de gestion de
mensajes de error que utilizan todos los modulos del
sintetizador. Recibe un puntero {f} a una funcion con el
siguiente prototipo:

VOID user_error_handler( const CHAR *fmt, va_list arglist );

La funcion se comporta como vprintf() de stdlib, pero debe
terminar el programa o lanzar una excepcion (aunque si
no lo hace, a su regreso HTTS efectua una llamada a exit(1) */

VOID HTTS_SetErrorFunc( HTTS_MsgFunc *f )
/*</DOC>*/
{
	htts_set_error_func(f);
}

/*<DOC>*/
/**********************************************************/
/* Esta funcion permite configurar la rutina de gestion de
mensajes de aviso (warning) que utilizan todos los modulos
del sintetizador. Recibe un puntero {f} a una funcion con el
siguiente prototipo:

VOID user_warn_handler( const CHAR *fmt, va_list arglist );

La funcion se comporta como vprintf() de stdlib, simplemente
debe mostrar o salvar en algun sitio el warning indicado. */

VOID HTTS_SetWarnFunc( HTTS_MsgFunc *f )
/*</DOC>*/
{
	htts_set_warn_func(f);
}
/////////////////////////////////////////////////////////////////
