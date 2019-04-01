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

Nombre fuente................ tts_io.cpp
Nombre paquete............... Sinte
Lenguaje fuente.............. C++
Estado....................... -
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Borja Etxebarria
.............................

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
1.0.1    02/10/11  inaki     add synthesize API
1.0.0    31/01/00  borja     codefreeze aHoTTS v1.0
0.0.0    06/02/98  borja     Codificacion inicial.

======================== Contenido ========================
<DOC>
Interfaz de usuario del modulo de sintesis HTTS.


Conjunto de parametros SET/GET soportados:
------------------------------------------
Ocupa bastante, ver el fichero setget.txt.
</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include <assert.h>
#include "htts.hpp"
#include "httsdo.hpp"
#include "httsmsg.h"

/*<DOC>*/
/**********************************************************/
/* El constructor de la clase no tiene parametros. El usuario
debe llamar al metodo create() para inicializar la clase.
Antes de efectuar el create() se pueden configurar algunos
parametros con el metodo set() */

HTTS::HTTS( VOID )
/*</DOC>*/
{
	data = new HTTSDo;
	assert(data);
}

/*<DOC>*/
/**********************************************************/
/* Destructor de la clase */

HTTS::~HTTS( )
/*</DOC>*/
{
	if (data) delete data;
}

/*<DOC>*/
/**********************************************************/
/* Inicializacion interna de la clase. {devuelve} TRUE
si la clase se ha inicializado correctamente, o FALSE en
caso de error.

El usuario debe llamar a esta funcion antes de empezar a
hacer uso de la clase. Antes del create() se puede llamar
a set() con algunos parametros.

Existe una version alternativa create(db) que recibe una
base de datos externa. */

BOOL HTTS::create( VOID )
/*</DOC>*/
{
	return data->create();
}

/*<DOC>*/
/**********************************************************/
/* Inicializacion interna de la clase. {devuelve} TRUE
si la clase se ha inicializado correctamente, o FALSE en
caso de error.

El usuario debe llamar a esta funcion antes de empezar a
hacer uso de la clase. Antes del create() se puede llamar
a set() con algunos parametros.

Este metodo recibe una base de datos externa {db} obtenida
de otro modulo HTTS previamente creado, para permitir asi
que varios modulos HTTS compartan una misma base de datos.

Los objetos HTTS inicializados con este metodo solo leen la base
de datos. El objeto inicial del que se ha obtenido la base de
datos debe existir siempre mientras exista algun otro objeto que
utilice su base de datos. */

BOOL HTTS::create( HTTS_DB *db )
/*</DOC>*/
{
	return data->create((VOID*)db);
}

/*<DOC>*/
/**********************************************************/
/* {devuelve} un puntero a la base de datos utilizada por
un modulo HTTS. El objeto debe estar ya inicializado
(el metodo create() ya tiene que haberse ejecutado. Esta
base de datos puede ser utilizada/compartida por otros
modulos HTTS (ver metodos create()). */

HTTS_DB *HTTS::getDB(VOID)
/*</DOC>*/
{
	return (HTTS_DB*)(data->getDB());
}

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
/*</DOC>*/
{
	return data->input(str);
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

BOOL HTTS::input( CHAR ch )
/*</DOC>*/
{
	return data->input(ch);
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

BOOL HTTS::input_multilingual( const CHAR * str, const CHAR *lang , const CHAR *data_path ,BOOL InputIsFile) //inaki
/*</DOC>*/
{
    return data->synthesize_do_input( str, lang, InputIsFile, data_path);
}

int HTTS::output_multilingual(const char * lang, short **samples){
		return data->synthesize_do_next_sentence(lang, samples);
}


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
/*</DOC>*/
{
	return data->flush();
}

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
/*</DOC>*/
{
	return data->output(len,flush,mode,cb_n);
}

/*<DOC>*/
/**********************************************************/
/* Indica si el ultimo output() NULL fue un flush o no.
En el parametro {flush} de la funcion output() se puede
obtener el mismo resultado que {devuelve} esta funcion. */

BOOL HTTS::isFlush( VOID )
/*</DOC>*/
{
	return data->isFlush();
}

/*<DOC>*/
/**********************************************************/
/* {devuelve} la lista de marcas disponibles hasta el momento.
Cuando se hace outack() el usuario puede indicar que se borren
si ya las ha procesado. */

#ifdef HTTS_INTERFACE_WAVEMARKS
const Mark1DList & HTTS::mrkget( VOID )
/*</DOC>*/
{
	return data->mrkget();
}
#endif

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
/*</DOC>*/
{
	data->mrkdel();
}
#endif

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
/*</DOC>*/
{
	return data->outack(n,mrkdel);
}
/*<DOC>*/
#else
BOOL HTTS::outack( INT n )
/*</DOC>*/
{
	return data->outack(n);
}
#endif

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
/*</DOC>*/
{
	return data->get_timeev();
}
#endif

/*<DOC>*/
/**********************************************************/
/* Permite configurar el parametro {param} con el valor {val}.
En caso de que el parametro no sea reconocido, no pueda
configurarse en dicho momento, o {val} no sea valido, la
funcion {devuelve} FALSE. Si el parametro se configura
correctamente {devuelve} TRUE.
Hay una version con {val} de tipo DOUBLE y otra con INT.

Ver el apartado "SET/GET soportados" para informacion sobre
los parametros que se pueden enviar. */

BOOL HTTS::set( const CHAR* param, const CHAR* val )
/*</DOC>*/
{
	return data->set(param,val);
}

/*<DOC>*/
/**********************************************************/
/* Version de set() con {val} de tipo DOUBLE. */

BOOL HTTS::set( const CHAR* param, DOUBLE val )
/*</DOC>*/
{
	CHAR buf[20];
	sprintf(buf,"%g",val);
	return data->set(param,buf);
}


/*<DOC>*/
/**********************************************************/
/* Version de set() con {val} de tipo INT. */

BOOL HTTS::set( const CHAR* param, INT val )
/*</DOC>*/
{
	CHAR buf[20];
	sprintf(buf,"%ld",(long)val);
	return data->set(param,buf);
}

/*<DOC>*/
/**********************************************************/
/* Este metodo {devuelve} una cadena con el valor actual del
parametro {param}. En caso de que {param} no sea un parametro
reconocido por el sintetizador, se {devuelve} NULL.

La cadena devuelta es un valor interno. El usuario NO debe bajo
ningun concepto modificarla. Llamadas posteriores a esta funcion
borran el valor anterior.

Ver el apartado "SET/GET soportados" para informacion sobre
los parametros que se pueden enviar. */

const CHAR* HTTS::get( const CHAR* param )
/*</DOC>*/
{
	return data->get(param);
}

/**********************************************************/
/* Como get(param) pero convierte el valor a DOUBLE y lo
almacena donde apunta {val}. Si no es un DOUBLE, se escribe
un 0. Tambien se {devuelve} el puntero a la cadena interna
que almacena el valor, o NULL si no es un parametro
reconocido (en cuyo caso {val} toma el valor 0). */

const CHAR* HTTS::get( const CHAR* param, DOUBLE *val )
/*</DOC>*/
{
	const CHAR *ret=data->get(param);
	if (val) *val= ret?atof(ret) : 0;
	return ret;
}

/*<DOC>*/
/**********************************************************/
/* Este metodo {devuelve} la frecuencia de muestreo en Hz del
audio de salida. Se puede obtener el mismo resultado a traves
del metodo get("SRate") (que devuelve el valor en formato de
cadena alfanumerica). En caso de error se obtendra un valor
0 para la frecuencia de muestreo, pero realmente esto no
deberia suceder nunca. */

DOUBLE HTTS::getSRate( VOID )
/*</DOC>*/
{
	const CHAR *ssrate = get("SRate");
	return ssrate ? atof(ssrate) : 0;
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

/**********************************************************/
//inaki
//API para sintesis, recibe texto de entrada y realiza procesado
//linguistico para el idioma que se indique
//VOID HTTS::synthesize(const CHAR * str, CHAR ** out, const CHAR *options, BOOL InputIsFile/*=FALSE*/)//INAKI

//{
    //const char *text=str;
    ////return ret;
    //data->synthesize_do( str, out, options, InputIsFile);
//}

/***********************************************************/
