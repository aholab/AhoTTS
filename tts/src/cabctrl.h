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
#ifndef __CABCTRL_H
#define __CABCTRL_H


/************************************/
/*   1992 (c) Grupo de Voz - DAET   */
/*         ETSII/IT Bilbao          */
/************************************/


/**********************************************************/

#include <stdio.h>
#include <string.h>

#include "tdef.h"
#include "uti.h"
#include "cabcod.h"
#include "cabdef0.h"
#include "cabdef1.h"

/**********************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************/
/* permite pasar de modo lectura a escritura y al reves */

#define change_rw(f) fseek(f,0,SEEK_CUR)

/**********************************************************/

FLOAT4 xlee_sfloat( FILE * f );

/**********************************************************/

void xescribe_sfloat( FILE * f,
                      FLOAT4 sf );

/**********************************************************/
/* comprueba que en la cabecera de fichero cfich existe o no
la cabecera definida por la mascara mcab (solo una cabecera).
cfich es de tipo struct cfich */

#define hay_cab(cfich,mc)  ((((cfich).mcab)&(mc))!=0)

/**********************************************************/
/* comprueba que en la cabecera de fichero cfich existen o no
las cabeceras definidas por la mascara mcab. Devuelve 1 solo si
existen TODAS las indicadas en mcab.
cfich es de tipo struct cfich */

#define hay_cabs(cfich,mcab) ((((cfich).mcab)&(mcab))==(mcab))

/**********************************************************/
/* comprueba que en cfich esta indicado que hay cabecera de senal.
cfich es de tipo struct cfich */

#define hay_csenal(cfich)  hay_cab(cfich,MC_SENAL)

/**********************************************************/
/* comprueba que en cfich esta indicado que hay cabecera de parametros.
cfich es de tipo struct cfich */

#define hay_cparam(cfich)  hay_cab(cfich,MC_PARAM)

/**********************************************************/
/* posiciona el fichero fp en la mascara de cabeceras */

#define vete_cmask(fp) \
  xfseek(fp,sizeof(UINT4)+sizeof(UINT2)+sizeof(UCHAR1),SEEK_SET)

/**********************************************************/
/* devuelve la mascara de cabeceras incluidas para el fichero fp.
el puntero al fichero puede estar en cualquier sitio, y al terminar
queda justo despues de leer la mascara de cabeceras */

UINT4 get_cmask( FILE * fp );

/**********************************************************/
/* Va a la cabecera indicada en cabmask. Si cabmask=MC_NINGUNA (0) va al
   principio del fichero. Si cabmask=MC_TODAS ($FF...FF) va al principio
   de los datos.
   Devuelve 1 si todo va bien, 0 si no se puede ir a esa cabecera porque
   no existe, en cuyo caso, la posicion en el fichero vale lo que le de
   la REAL gana. Mejor si usas xvete_cab(). */

int vete_cab( FILE * fp, UINT4 cabmask );

/**********************************************************/
/* Como vete_cab pero en caso de no existir la cabecera indicada
produce un error y sale al S.O */

void xvete_cab( FILE * fp, UINT4 cabmask );

/**********************************************************/
/* va al principio del fichero */

#define vete_principio(file) xfseek((file),0,SEEK_SET)

/**********************************************************/
/* va a la cabecera de fichero (cfich) */

#define vete_cfich(file)  vete_principio(file)

/**********************************************************/
/* va a la cabecera de senal */

#define vete_csenal(file) xvete_cab(file,MC_SENAL)

/**********************************************************/
/* va a la cabecera de parametros */

#define vete_cparam(file) xvete_cab(file,MC_PARAM)

/**********************************************************/
/* va a los datos, saltando todas las cabeceras */

#define vete_datos(file) xvete_cab(file,MC_TODAS)

/**********************************************************/
/* salta el campo longitud de cabecera, estando el fichero posicionado
en este campo */

#define salta_long(file) xfseek((file),sizeof(UINT4),SEEK_CUR)

/**********************************************************/
/* va a la cabecera general de senal: va a la cabecera de senal y
salta el campo longitud de cabecera */

#define vete_cgsenal(file) {  \
  xvete_cab(file,MC_SENAL);  \
  salta_long(file); }

/**********************************************************/
/* va a la cabecera general de parametros: va a la cabecera de
parametros  y salta el campo longitud de cabecera */

#define vete_cgparam(file) {  \
  xvete_cab(file,MC_PARAM);  \
  salta_long(file); }

/**********************************************************/
/* lee la cabecera de fichero, estando posicionado el fichero en ella */

void lee_cfich( FILE * fp,
                struct cfich * cf_ptr);

/**********************************************************/
/* salva la cabecera de fichero, estando posicionado el fichero en ella */

void escribe_cfich( FILE * fp,
                    struct cfich * cf_ptr);

/**********************************************************/
/* anade a la mascara de cabeceras, las cabeceras identificada en cabmask.
   El apuntador de fichero puede estar en cualquier sitio, y se vuelve
   a poner ah¡ al terminar */

void anade_cmask( FILE * fp, UINT4 cabmask );

/**********************************************************/
/* escribe la longitud indicada, en la posicion actual del fichero */

void escribe_long( FILE * fp, UINT4 l );

/**********************************************************/
/* modifica la longitud de la cabecera definida por cabmask,
   considerando que el apuntador est  al final de esa cabecera.
   El apuntador se vuelve a dejar en el final de la cabecera. */

void escribe_long_actual( FILE * fp, UINT4 cabmask );

/**********************************************************/
/* devuelve la longitud de la cabecera definida por cabmask,
   desde el principio de ella hasta la posicion actual. */

long lee_long_actual( FILE * fp, UINT4 cabmask );

/**********************************************************/
/* devuelve la longitud de una cabecera general de senal */

UINT4 long_cgsenal( struct cgsenal * cgs );

/**********************************************************/
/* borra (con xfreemem) los campos variables de una cabecera
general de senal, y pone a cero su longitud */

void free_cgsenal( struct cgsenal * cgs );

/**********************************************************/
/* resetea la cabecera general de senal apuntada por cgs_ptr (la pone
a ceros, y punteros a campos variables apuntando a NULL */

#define reset_cgsenal(cgs_ptr) memset(cgs_ptr,0,sizeof(*(cgs_ptr)))

/**********************************************************/
/* leo una cabecera general de senal.  Devuelvo 0 si todo se ha leido,
   o 1 si algun campo variable no se ha leido al completo.

   Campos variables inicializados segun prepara_cvariable(): si se rellena
   antes el campo variable a ceros: ptr=NULL y nelem=0, se crea un buffer
   adecuado y se leen todos los elementos del campo. Si nelem<>0, es el
   limite a leer, y si ptr<>NULL, el array se supone creado (si no, se crea),
   y nelem es siempre el limite de elementos a leer (en los campos de
   texto se anade un cero al final, por lo que DEBES crear los arrays con
   un char mas de lo que me dices a mi. */

int lee_cgsenal( FILE * fp, struct cgsenal * cgs );

/**********************************************************/
/* salva una cabecera general de senal */

void escribe_cgsenal( FILE * fp, struct cgsenal * cgs );

/**********************************************************/
/* salva una cabecera general de senal, pero se queda en las marcas(no
llega a salvarlas */

void escribe_cgsenal_marc( FILE * fp, struct cgsenal * cgs );

/**********************************************************/
/* lee cabecera vieja, pero con el formato nuevo */

void lee_csenal_vieja( FILE * fp, struct cgsenal * cgs );

/**********************************************************/
/* escribe cabecera vieja, pero enviandosela con el formato nuevo */

void escribe_csenal_vieja( FILE * fp, struct cgsenal * cgs );

/**********************************************************/
/* devuelve la longitud de una cabecera general de parametros */

#define long_cgparam() sizeof(struct cgparam)

/**********************************************************/
/* lee una cabecera general de parametros del fichero fp, estando
el fichero posicionado en ella */

#define lee_cgparam(fp,cgp_ptr) xfread(cgp_ptr,sizeof(*(cgp_ptr)),1,fp)

/**********************************************************/
/* salva la cabecera general de parametros en la posicion actual del
fichero fp */

#define escribe_cgparam(fp,cgp_ptr) xfwrite(cgp_ptr,sizeof(*(cgp_ptr)),1,fp)

/**********************************************************/
/* devuelve la longitud de una cabecera general de parametros parcor */

#define long_cgp_parcor() sizeof(struct cgp_parcor)

/**********************************************************/
/* lee una cabecera de parcors del fichero fp a partir de la posicion
actual */

void lee_cgp_parcor( FILE * fp,
                     struct cgp_parcor * cgp_pc_ptr );

/**********************************************************/
/* salva una cabecera de parcors en la posicion actual del fichero fp */

void escribe_cgp_parcor( FILE * fp,
                         struct cgp_parcor * cgp_pc_ptr);

/**********************************************************/
/* devuelve la longitud de una cabecera general de parametros lpc*/

#define long_cgp_lpc() sizeof(struct cgp_lpc)

/**********************************************************/
/* lee una cabecera de lpcs del fichero fp a partir de la posicion
actual */

void lee_cgp_lpc( FILE * fp,
                  struct cgp_lpc * cgp_lpc_ptr);

/**********************************************************/
/* salva una cabecera de lpcs en la posicion actual del fichero fp */

void escribe_cgp_lpc( FILE * fp,
                      struct cgp_lpc * cgp_lpc_ptr);

/**********************************************************/

#ifdef __cplusplus
}  /* extern "C" */
#endif

/**********************************************************/

#endif
