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
Copyright: 1996 - Grupo de Voz (DAET) ETSII/IT-Bilbao

Nombre fuente................ STRL.CPP
Nombre paquete............... -
Lenguaje fuente.............. C++
Estado....................... Completado
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Borja Etxebarria

Version  dd/mm/aa  Autor     Comentario
-------  --------  --------  ----------
1.2.5    30/08/98  Borja     split en varios modulos strl_?.cpp
1.2.4    22/08/97  Borja     auto DefCfgFile en clargs2props()
1.2.3    12/08/97  Borja     soporte comillas dobles (key="multi palabra")
1.2.2    12/08/97  Borja     soporte Help en clargs2props()
1.2.1    07/08/97  Borja     bug en clargs2props() y tob()
1.2.0    25/04/97  Borja     usar String en vez de char*
1.1.0    14/01/97  Borja     add/remove_prefix() como en CSTR Speech Tools
1.0.0    06/05/96  Borja     Codificacion inicial.

======================== Contenido ========================
<DOC>
Listas de cadenas (String). Clases:
- StrList : Lista de cadenas
- StrSet : Conjunto de cadenas (cadenas no repetidas)
- KVStrList : Conjuntos de cadenas Clave y Valor (Key - Val ).

StrList: Tienen los metodos de listas (plantilla ListT, ver listt.cpp),
excepto el metodo create(). Ademas tiene constructores que permiten
inicializar la lista:
StrList l(otraStrList);
StrList l(otraStrSet);
StrList l("elem1 ele2 ele3 %d %f",10,5.5);
char *a[]={"elem1", "elem2", NULL};
StrList l(a);
String s="el1 el2 el3";
StrList(s);

y tambien metodos append_s() para anyadir mas elementos, de forma
similar a los constructores anteriores. Tambien tienen un metodo
sort() que ordena alfabeticamente la lista (si se envia un parametro
TRUE opcional (sort(TRUE)) se ordena inversamente.

StrSet: similar a StrList pero sin elementos repetidos. Tiene los
metodos de la plantilla SetListT (ver listt.cpp) para cadenas
String. Tambien tiene constructores similares a StrList, y en lugar
de metodo append_s(), tiene metodo add_s(), que controla que no haya
elementos duplicados en la lista.

KVStrList: es una especie de StrSet, pero para cada elemento del
conjunto (clave) hay un valor asociado (val), ambos de tipo String.
Tiene los metodos de la plantilla KeyValListT (ver listt.cpp),
y constructores similares a los de StrList y StrSet, pero indicando
clave y valor, por ejemplo:
StrList l("clave1=val1 clave2=val2 clave3=%d",10);
Tambien tiene metodos add_s() igual que StrSet, solo que aceptando
los dos parametros String clave y valor.
Tambien tiene varios metodos del tipo val(key) que {devuelven} el
valor dada una clave (val(key,defval) {devuelve} {defval} si no hay
un elemento de clave {key} en la lista). Los metodos son:
val() -> devuelve la cadena.
ival() -> devuelve un entero (evalua la cadena como entero)
fval() -> devuelve un float (evalua la cadena como float).
Y asi otros: lval() para long, dval() para double, bval para boolean.
Y para anyadir elementos, aparte de add(cadenaclave,cadenavalor)
se definen add(cadenaclave,valor) donde {valor} puede ser un
entero, long, float o double (que se almacenara realmente como
una cadena). Tambien se pueden utilizar addi(), addl(), addf(),
addd() que hacen lo mismo que las respectivas versions
sobrecargadas de add(). Ademas hay addb() para boolean, que
permite introducir valores boolean en la lista (se almacenan
como cadenas "True"/"False".
</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include <string.h>

#include "strl.hpp"

/**********************************************************/
// Uso interno a modulos strl_?.cpp

INT _strl_cmp(const String *s1, const String *s2)
{
	return strcmp(*s1,*s2);
}

/**********************************************************/
// Uso interno a modulos strl_?.cpp

INT _strl_rcmp(const String *s1, const String *s2)
{
	return strcmp(*s2,*s1);
}

/**********************************************************/
