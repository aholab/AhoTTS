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

Nombre fuente................ LISTT.CPP
Nombre paquete............... -
Lenguaje fuente.............. C++
Estado....................... Completado
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Borja Etxebarria

Version  dd/mm/aa  Autor     Comentario
-------  --------  --------  ----------
1.1.0    07/05/99  Borja     modif ??_mv()  $$$$ doc. sin actualizar!!
1.0.1    30/08/98  Borja     split varios modulos listt_?.cpp
1.0.0    26/03/98  Borja     recodificado, templates mas sencillas.
0.0.2    26/08/97  Borja     uso LIINT (INT o LONG) en vez de INT
0.0.1    26/08/97  Borja     documentacion anyadir sin indicar elemento
0.0.0    06/05/96  Borja     Codificacion inicial.

======================== Contenido ========================
<DOC>
Manejo de listas (doblemente encadenadas) y variantes:

ListT       template para lista
SetListT    template para lista set o conjunto (elementos no repetidos)
KeyValListT template para lista clave K, valor V (K no repetida)

Nota:
Si LIST_PTRVERIFY esta definido, se hace un chequeo bastante completo
de inconsistencias a la hora de manejar las listas. Es un poco lento
pero mas seguro. Si LIST_PTRVERIFY no esta definido, no se efectuan
chequeos, y puede ser peligroso. Para chequear algunos problemas,
utilizando OK() se puede comprobar que la lista sigue siendo valida.

Por ejemplo, si haces un exchange() entre elementos de listas
diferentes, utilizas un Lix invalido, etc, o si anyades un elemento
duplicado a un SetListT la lista deja de ser consistente.

ATENCION ATENCION ATENCION!!!!!!!!!!!
Las plantillas SetListT y KeyValListT solo se puede aplicar sobre
clases que tengan definido el operador de comparacion igual (==) (para
el tipo clave K en la plantilla KeyValListT). Si la clase no lo tiene
implementado, la plantilla provocara un error de compilacion!
Un ejemplo de dicho operador para una clase MiTipo
	friend int operator==(const MiTipo &a,const MiTiop &b)
	{
		return a.i == b.i;  // por poner algo...
	};

###########################################################
Muchos metodos pueden recibir una referencia a un elemento de la lista
bien a traves de un pseudo-indice de lista (Lix) {p} o bien a traves
de un numero entero {i} que indica el numero de orden dentro de la lista.
Trabajar con {i} es mucho mas lento y no es recomendable. En cualquier
caso cuando se use {i} debe ser un valor valido dentro del rango de
elementos de la lista (empezando en 0 para el primer elemento).

*** Los constructores no reciben parametros. Para inicializar o copiar
una lista a partir de otra, se utilizan los metodos create() o el
operador de asignacion (=). Los SetList o KeyValList inicializados
con create() no copian elementos duplicados.
*** l1=l2 : copia los elementos de la lista {l2} en la lista {l1} (primero
se vacia {l1}). Los SetList o KeyValList no copian elementos duplicados.
*** OK() : {devuelve} TRUE si la lista esta bien, o FALSE si esta mal
(por ejemplo si esta corrupta o si se detectan elementos duplicados en
un Set o una lista KeyVal).
*** empty() : {devuelve} TRUE si la lista esta vacia
*** length() : {devuelve} el numero de elementos de la lista
*** clear() : borra todos los elementos de la lista

Las siguientes mejor no usarlas con Set y KeyVal. Estos metodos pueden
funcionar con indices de lista (Lix) o con indices enteros (cuando sea
aplicable). En el caso de KeyVal en vez de enviar un unico valor {x}
se envian dos valores, la clave {k} y el valor {v}, por lo demas igual.
El elemento/lista recibido se copia usa para inicializar el elemento de
la lista (cuidado, puede tener efectos secundarios sutiles si los
elementos contienen punteros a arrays, etc, pues se copia la
referencia-puntero, no el array, a no ser que el operador = este
adecuadamente definido). Tambien hay versiones que no reciben elemento
inicial, y utilizan el constructor por defecto del elemento para
inicializar el contenido del elemento en la lista.
Para el caso de que se envie una lista, tambien hay versiones ???_mv
(p.ej. append_mv()) que anyaden una lista completa de elementos,
pero no copiando sino moviendo los elementos de la lista enviada
(que queda vacia).
*** prepend() : anyade un elemento al principio de la lista. {devuelve}
indice Lix al primer elemento (link de {x}).
*** append() : anyade elemento al final de la lista. {devuelve} indice Lix
al ultimo elemento (link de {x})
*** insbefore() : anyade {x} antes del apuntado por {p}, y {devuelve}
el link de {x}. Si {p} es null, anyade al final.
*** insafter() : anyade {x} despues del apuntado por {p}, y {devuelve}
el link de {x}. Si {p} es null, anyade al principio

Metodos para listas KeyVal, y algunas tambien para Sets. Cuando
intervienen listas KeyVal con listas normales o sets, solo se utiliza
el campo clave Key (en busquedas, eliminaciones, etc):
*** add() : anyade un elemento {x} (Sets) o pareja {k}{v} (KeyVal)
a la lista, salvo que ya exista. En el caso de KeyVal, si {k} ya existe,
se actualiza con el nuevo valor {v} (salvo si se especifica {rewr}=FALSE).
Se {devuelve} el link al elemento. Existen variantes para anyadir
otra lista/set/keyval completo. Por ejemplo t.add(t2) anyade todos
los elementos de la lista-KeyVal {t2} a la lista-KeyVal {t}, los
elementos {k} que ya existieran en {t} se actualiza con los nuevos
valores {v} de la lista {t2}, salvo si {rewr} se envia a FALSE.
Tambien hay version add_mv() que mueve la lista enviada en vez de
copiarla (si algun elemento no se debe mover, queda en la lista
enviada, los demas desaparecen de la lista enviada).
*** val() : {devuelve} el valor (Val) para una clave {k}. Si {k} no existe
se produce error, salvo que se especifique un valor por defecto {vdef}.
*** seek() : busca un elemento (o clave {k}) y {devuelve} su link {p}, o
{devuelve} null si no esta en la lista.
*** contains() : {devuelve} TRUE si la lista contiene al elemento {x}
(o clave {k}). Hay versiones para comprobar si una lista contiene
todos los elementos de otra lista (o lista de claves).
*** equals() : {devuelve} TRUE si las dos listas son iguales (en el caso
de listas KeyVal, solo se comprueba la clave Key). Cuando se compare
un set o lista KeyVal con una lista normal, hay un segundo parametro
{allowrep} que si es TRUE ignora elementos repetidos en la lista normal
a efectos de comparacion.
*** erase() : elimina el elemento indicado si existe (y se {devuelve} TRUE.
Si no, no hace nada (y {devuelve} FALSE). Hay versiones para eliminar
todos los elementos indicados en una lista.
*** keep() : Mantiene solo los elementos que esten en otra lista indicada.

Metodos para borrar elementos (tambien hay versiones con indices Lix {p}
y con indices enteros {i} para algunas de ellas):
*** delfirst() : quita el primer elemento de la lista, error si no hay
elementos
*** dellast() : quita el ultimo elemento de la lista, error si no hay
elementos
*** del() quita el indicado en {p} y {devuelve} puntero al
siguiente (dir>=0) o anterior (dir<0), error si {p} es null.
*** delnext() : quita el siguiente al indicado en {p}, error si
no existe ese siguiente. Si {p} es null, quita al principio.
*** delprev() : quita el anterior al indicado en {p}, error si no
existe ese anterior. Si {p} es null, quita al final.

Estas sirven para moverse por la lista con punteros Lix {p} o enteros {i}
y demas funciones relacionadas:
*** first() : {devuelve} el link al primer elemento, null cuando no hay
ninguno.
*** last() : {devuelve} el link al ultimo elemento, null cuando no hay
ninguno.
*** prev() : {devuelve} el link del elemento anterior a {p}, null cuando
ya no hay mas.
*** next() : {devuelve} el link del siguiente elemento a {p}, null cuando
ya no hay mas.
*** owns() : {devuelve} TRUE si la lista contiene el link {p}
*** index() : {devuelve} el numero de orden en la lista para el link {p},
o -1 si no esta en la lista.
*** lix() : {devuelve} el link correspondiente al elemento {i}esimo
de la lista, o null si no existe dicho elemento.


Lectura/escritura de elementos ya existentes. {devueven} una referencia
asi que se pueden usar tanto para leer como para modificar el elemento.
En el caso de Sets tener cuidado, pues al modificar el elemento
(la clave en el caso de listas KeyVal) la lista puede ser inconsistente
si aparecen elementos duplicados. En listas KeyVal se debe utilizar
itemkey() en vez de item(). Hay versiones con Lix {p} o con enteros {i}:
*** item_first() : {devuelve} el primer elemento, error si no hay
*** item_last() : {devuelve} el ultimo elemento, error si no hay
*** item(p) : {devuelve} el elemento del link {p}, error si no hay
*** operador () : {devuelve} el elemento del link {p}  (idem a item()), o
error si no hay. Se usa por ejemplo asi:  q=l(p). No hay version para
left-hand (no se puee hacer l(p)=q).


Estas son similares a las anteriores, pero especificas para listas KeyVal.
Tambien permiten leer o escribir, aunque modificar asi los campos Key
es peligroso pues se pueden duplicar elementos...
*** itemkey()
*** itemkey_first()
*** itemkey_last()
*** itemval()
*** itemval_first()
*** itemval_last()


Metodos varios:
*** exchange() : intercambia los links {p1} y {p2} dentro de la lista. error
si alguno es null. (hay version con indices {i}). Se intercambian
contenidos, por lo que los indices Lix siguen siendo validos, pero
los contenidos estan intercambiados.
*** reverse() : invierte el orden de la lista (cabeza <-> cola).
*** sortf() : ordena la lista siguiendo el criterio de la funcion de
comparacion enviada como parametro (ver qsort() en stdlib para saber
como funciona la funcion de comparacion). En listas KeyVal, los
parametros comparados son las claves {k}.

###########################################################
Ejemplos de uso.

Ejemplo de uso de la plantilla ListT

	int x, y;
	ListT<int> l;
	Lix p;

	for (x=0; x<100; x++)   // metemos unos cuantos elementos
		l.append(x);

	y=0;
	for (p=l.first(); p!=0; p=l.next(p))   // recorremos la lista
		y += l(p);   // vamos sumando en {y} los elementos de la lista.

Para la plantilla SetListT usar add(x) para anyadir elementos. Para
la plantilla KeyValListT usar add(k,v). Si no se hace asi la lista
puede tener inconsistencias.  KeyVal y Set tienen metodos de busqueda
(seek) y comprobacion de que existe un elemento (contains) entre otros.
KeyVal tiene posibilidad de localizar un Val a partir de una Key (val(k))
entre otros.

###########################################################
Tabla de metodos definidos en cada template:

Dadas estas variables:

MiTipo x;         // dato del usuario, del tipo que sea (ej. MiTipo)
MiClave k;        // dato de tipo clave
MiValor v;        // dato de tipo valor;
ListT<MiTipo> l;  // lista (double-linked) de datos del tipo MiTipo
SetListT<MiTipo> s; // set-lista (double-linked) de datos del tipo MiTipo
KeyValListT<MiClave,MiValor> t; // conjunto clave-valor para dos tipos dados
Lix p;            // "List index", link a la lista, "indice" para recorrerla
LIINT i;          // numero entero (INT o LONG) (indice entero para recorrer listas)
LIINT n;          // numero entero (INT o LONG)
BOOL b;           // valor booleano

Estas son las operaciones que se pueden hacer con cada template (las
que tienen una admiracion (!) detras es porque se deben usar con
cuidado, pues pueden producir inconsistencia en los datos de la lista,
no deben usarse salvo sabiendo exactamente lo que se hace y el nombre
del metodo va precedido por un underscore (_) aunque aqui no aparezca
(vamos, que en vez de t.append() realmente el nombre del metodo que
se debe usar es t._append() ).


ListT                     SetListT                  KeyValListT
--------------------------------------------------------------------------
ListT<tipo> l;            SetListT<tipo> s          KeyValListT<tk,tv> t

l.create(l2)        			s.create(s2)     					t.create(t2)
l.create(s)		   					s.create(l)       				-

l=l2;                    	s=s2                      t=t2
l=s                       s=l                       -

b=l.OK()                  b=s.OK()                  b=t.OK()

b=l.empty()               b=s.empty()               b=t.empty()
n=l.length()              n=s.length()              n=t.length()
l.clear()                 s.clear()                 t.clear()

p=l.prepend()             p=s.prepend()!            p=t.prepend()!
p=l.prepend(x)            p=s.prepend(x)!           p=t.prepend(k,v)!
p=l.prepend(l2)           p=s.prepend(s2)!          p=t.prepend(t2)!
p=l.prepend(s)            p=s.prepend(l)!           -
p=l.prepend_mv(l2)        p=s.prepend_mv(s2)!       p=t.prepend_mv(t2)!
p=l.prepend_mv(s)         p=s.prepend_mv(l)!        -

p=l.append()              p=s.append()!             p=t.append()!
p=l.append(x)             p=s.append(x)!            p=t.append(k,v)!
p=l.append(l2)            p=s.append(s2)!           p=t.append(t2)!
p=l.append(s)             p=s.append(l)!            -
p=l.append_mv(l2)         p=s.append_mv(s2)!        p=t.append_mv(t2)!
p=l.append_mv(s)          p=s.append_mv(l)!         -

p=l.insbefore(p)          p=s.insbefore(p)!         p=t.insbefore(p)!
p=l.insbefore(p,x)        p=s.insbefore(p,x)!       p=t.insbefore(p,k,v)!
p=l.insbefore(p,l2)       p=s.insbefore(p,s2)!      p=t.insbefore(p,t2)!
p=l.insbefore(s)          p=s.insbefore(l)!         -
p=l.insbefore_mv(l2)      p=s.insbefore_mv(s2)!     p=t.insbefore_mv(t2)!
p=l.insbefore_mv(s)       p=s.insbefore_mv(l)!      -

p=l.insafter(p)           p=s.insafter(p)!          p=t.insafter(p)!
p=l.insafter(p,x)         p=s.insafter(p,x)!        p=t.insafter(p,k,v)!
p=l.insafter(p,l2)        p=s.insafter(p,s2)!       p=t.insafter(p,t2)!
p=l.insafter(s)           p=s.insafter(l)!          -
p=l.insafter_mv(l2)       p=s.insafter_mv(s2)!      p=t.insafter_mv(t2)!
p=l.insafter_mv(s)        p=s.insafter_mv(l)!       -

-                         p=s.add(x)                p=t.add(k,v,rewr)
-                         s.add(s2)                 t.add(t2,rewr)
-                         s.add(l)                  -
-                         s.add_mv(s2,rewr)         t.add_mv(t2,rewr)

-                         -                         v=t.val(k)
-                         -                         v=t.val(k,vdef)

-                         p=s.seek(x)               p=t.seek(k)

-                         b=s.contains(x)           b=t.contains(k)
-                         b=s.contains(s2)          b=t.contains(t2)
-                         b=s.contains(l)           b=t.contains(l)
-                         -                         b=t.contains(s)

-                         b=s.equals(s,allowrep)    b=t.equals(t2,allowrep)
-                         b=s.equals(l,allowrep)    b=t.equals(l,allowrep)
-                         -                         b=t.equals(s,allowrep)

-                         b=s.erase(x)              b=t.erase(k)
-                         b=s.erase(s2)             b=t.erase(t2)
-                         b=s.erase(l)              b=t.erase(l)
                          -                         b=t.erase(s)

-                         s.keep(s2)                t.keep(t2)
-                         s.keep(l)                 t.keep(l)
-                         -                         t.keep(s)

p=l.del(p,dir=1)          p=s.del(p,dir=1)          p=t.del(p,dir=1)
l.delprev(p)              s.delprev(p)              t.delprev(p)
l.delnext(p)              s.delnext(p)              t.delnext(p)
p=l.delfirst()            p=s.delfirst()            p=t.delfirst()
p=l.dellast()             p=s.dellast()             p=t.dellast()

p=l.first()               p=s.first()               p=t.first()
p=l.last()                p=s.last()                p=t.last()
p=l.next(p)               p=s.next(p)               p=t.next(p)
p=l.prev(p)               p=s.prev(p)               p=t.prev(p)
b=l.owns(p)               b=s.owns(p)               b=t.owns(p)
i=l.index(p)              i=s.index(p)              i=t.index(p)
p=l.lix(i)                p=s.lix(i)                p=t.lix(i)

x=l.item(p)=x             x=s.item(p)=x!            -
x=l(p)                    x=s(p)                    -
x=l.item_first()=x        x=s.item_first()=x!       -
x=l.item_last()=x         x=s.item_last()=x!        -

-                         -                         k=t.itemkey(p)=k!
-                         -                         k=t.itemkey_first()=k!
-                         -                         k=t.itemkey_last()=k!
-                         -                         v=t.itemval(p)=v!
-                         -                         v=t.itemval_first(p)=v!
-                         -                         v=t.itemval_last(p)=v!

l.exchange(p1,p2)         s.exchange(p1,p2)         t.exchange(p1,p2)
l.reverse()               s.reverse()               t.reverse()
l.sortf(cmpfunc)          s.sortf(cmpfunc)          t.sortf(kcmpfunc)


Las funciones anteriores que reciben pseudoindices (p, p1, p2) tambien
pueden trabajar con indices enteros.
</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include "listt_i.hpp"

/**********************************************************/

#ifdef LIST_PTRVERIFY
Lix _PList::__lix_safe( LIINT i, CHAR*func, BOOL allow0) const
{
	Lix p=lix(i);
	if ((p==0)&&(!allow0 || i)) {
		LISTERROR(func," invalid index");
	}
	return p;
}
#endif

/**********************************************************/

_PListNode *_PList::__newNode_d( const VOID *data )
{
	VOID * t = _newData(data);
	if (!t)
		LISTERROR("__newNode_d","can't create data object (null new)");
	return __newNode_p(t);
}

/**********************************************************/

_PListNode *_PList::__newNode_p( VOID *data )
{
	_PListNode * t = new _PListNode(data);
	if (!t)
		LISTERROR("__newNode_p","can't create node object (null new)");
	return t;
}

/**********************************************************/

_PListNode *_PList::__newNode_d( const _PListNode *node )
{
	return __newNode_d(node->dp);
}

/**********************************************************/

_PListNode *_PList::__newNode_p( _PListNode *node )
{
	return __newNode_p(node->dp);
}

/**********************************************************/

VOID _PList::__deleteNode_d( _PListNode *p )
{
	_deleteData(p->dp);
	delete p;
}

/**********************************************************/

Lix _PList::__prepend(_PListNode *t)
{
	if (h == 0) t->f = t->b = h = t;
	else {
		t->f = h;
		t->b = h->b;
		h->b->f = t;
		h->b = t;
		h = t;
	}
	return Lix(t);
}

/**********************************************************/

Lix _PList::__append(_PListNode *t)
{
	if (h == 0) t->f = t->b = h = t;
	else {
		t->b = h->b;
		t->b->f = t;
		t->f = h;
		h->b = t;
	}
	return Lix(t);
}

/**********************************************************/

Lix _PList::__insbefore(Lix p, _PListNode *t)
{
	if (p == 0) return __append(t);
	LISTVERIFY(p,"__insbefore");
	_PListNode* u = (_PListNode*) p;
	t->b = u->b;
	t->f = u;
	u->b->f = t;
	u->b = t;
	if (u == h) h = t;
	return Lix(t);
}

/**********************************************************/

Lix _PList::__insafter(Lix p, _PListNode *t)
{
	if (p == 0) return __prepend(t);
	LISTVERIFY(p,"__insafter");
	_PListNode* u = (_PListNode*) p;
	t->b = u;
	t->f = u->f;
	u->f->b = t;
	u->f = t;
	return Lix(t);
}

/**********************************************************/

Lix _PList::__del(Lix p, INT dir)
{
	if (p == 0) { LISTNONULL(p,"__del"); }
	else {
		LISTVERIFY(p,"__del");
		_PListNode* t = (_PListNode*) p;
		if (t->f == t) { h = 0; p = 0; }
		else {
			if (dir < 0) { if (t == h) p = 0; else p = Lix(t->b); }
			else { if (t == h->b) p = 0; else p = Lix(t->f); }
			t->b->f = t->f;
			t->f->b = t->b;
			if (t == h) h = t->f;
		}
	}
	return p;
}

/**********************************************************/

VOID _PList::__clear( BOOL cdata )
{
	if (h == 0) return;
	_PListNode* p = h->f;
	h->f = 0;
	h = 0;
	while (p != 0) {
		_PListNode* nxt = p->f;
		if (cdata) __deleteNode_d(p);
		else __deleteNode_p(p);
		p = nxt;
	}
}

/**********************************************************/
