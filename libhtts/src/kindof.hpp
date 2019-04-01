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
#ifndef __KINDOF_HPP__
#define __KINDOF_HPP__

/**********************************************************/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/*
Copyright: 1997 - Grupo de Voz (DAET) ETSII/IT-Bilbao

Nombre fuente................ KINDOF.HPP
Nombre paquete............... -
Lenguaje fuente.............. C++ (Borland C/C++ 3.1)
Estado....................... Utilizable
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Borja Etxebarria

Version  dd/mm/aa  Autor     Comentario
-------  --------  --------  ----------
1.0.1    08/10/98  Borja     Punto y coma en DECL(), versiones INLINE;
1.0.0    17/09/97  Borja     Codificacion inicial.

======================== Contenido ========================
<DOC>
Macros para implementar metodo isKindOf() y className
en una clase.

Estos metodos tienen estos prototipos:
virtual BOOL isKindOf(const CHAR *s );
virtual const CHAR *className( VOID );

El primero permite comprobar si un objeto es de la clase nombrada
por la cadena {s} (funciona tambien con clases derivadas). El
segundo {devuelve} una cadena estatica interna que contiene el
nombre de la clase del objeto.

Se utiliza asi:
El la declaracion de la clase, en una seccion public
anyadir esto:
        KINDOF_DECL();

En algun fichero de implementacion de la clase, anyadir esto:
        KINDOF_BDEF(cname)
o bien esto:
        KINDOF_DDEF(cname,cbase)
El primero es para clases base, y el segundo para clases derivadas,
siendo {cname} el nombre de la clase y {cbase} el nombre de la clase
de la que derivamos.
Cuidado con el punto y coma final, no hay que ponerlo salvo
en KINDOF_DECL !!

Tambien hay versiones inline para declarar directamente en el .hpp.
En tal caso no hace falta usar KINDOF_DECL() y directamente se puede
poner en la declaracion de la clase:
        KINDOF_BDEF_INLINE(cname);
o bien esto:
        KINDOF_DDEF_INLINE(cname,cbase);

</DOC>
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include <string.h>

/**********************************************************/

#define KINDOF_DECL() \
	virtual BOOL isKindOf(const CHAR *s ) const; \
        virtual const CHAR *className( VOID ) const

#define KINDOF_BDEF(cname) \
	BOOL cname::isKindOf(const CHAR *s ) const { return !strcmp(#cname,s); } \
	const CHAR * cname::className( VOID ) const { return #cname; }

#define KINDOF_DDEF(cname,cbase) \
	BOOL cname::isKindOf(const CHAR *s ) const { return strcmp(#cname,s)?cbase::isKindOf(s):TRUE; } \
	const CHAR * cname::className( VOID ) const { return #cname; }

#define KINDOF_BDEF_INLINE(cname) \
	virtual BOOL isKindOf(const CHAR *s ) const { return !strcmp(#cname,s); } \
	virtual const CHAR * className( VOID ) const { return #cname; }

#define KINDOF_DDEF_INLINE(cname,cbase) \
	virtual BOOL isKindOf(const CHAR *s ) const { return strcmp(#cname,s)?cbase::isKindOf(s):TRUE; } \
	virtual const CHAR * className( VOID ) const { return #cname; }

/**********************************************************/

#endif

