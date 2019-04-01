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
#ifndef __TNOR_H__
#define __TNOR_H__

/**********************************************************/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/*
Copyright: 1998 - TSR/Aholab (DET) ETSII/IT-Bilbao

Nombre fuente................ tnor.h
Nombre paquete............... normalizador
Lenguaje fuente.............. C++
Estado....................... desarrollo
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Juan Luis Murugarren

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
0.0.6    01/10/04  inigos    Tipos de normalizaci¢n NAME y CAPITAL
0.0.5    07/05/01  Yon2.     Tipo de normalizaci¢n UNIT.
0.0.4    07/05/01  Yon2.     Tipo de normalizaci¢n palabra con ap¢strofe -> t'erdi
0.0.3    10/03/01  Yon2.     ..._ACRO -> ..._ACROABBUNI
0.0.2    03/03/01  Yon2.     Grupo palabra compuesta.
0.0.1    02/01/01  Yon2.     Grupo desconocido.
0.0.0    11/12/00  Yon2.     Codificacion inicial.

======================== Contenido ========================

  Definici¢n de los tipos posibles de normalizaci¢n.
La normalizaci¢n en s¡ se hace en el m¢dulo normal.cpp.
El soporte para normalizaci¢n est  en el m¢dulo "ctlist"
ver tanto el 'hpp' como el 'cpp'.

Yon2.
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

/**********************************************************/

enum TNOR	{
	UTYPENOR_UNKNOWN=0,
	UTYPENOR_NORMAL,
	UTYPENOR_NUMBER,
	UTYPENOR_TIME,
	UTYPENOR_DATE,
	UTYPENOR_ACROABBUNI,
	UTYPENOR_UNIT,
	UTYPENOR_SPELL,
	UTYPENOR_COMP,
	UTYPENOR_WEB,
	UTYPENOR_APOST,
	UTYPENOR_UNKGROUP,
	UTYPENOR_NAME,
	UTYPENOR_CAPITAL,
};


/**********************************************************/

#endif
