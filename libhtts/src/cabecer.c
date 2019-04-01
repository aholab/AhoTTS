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
/************************************/
/*   1992 (c) Grupo de Voz - DAET   */
/*         ETSII/IT Bilbao          */
/************************************/


/**********************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "uti.h"
#include "cabdef0.h"
#include "cabdef1.h"
#include "cabcod.h"
#include "cabctrl.h"
#include "cabecer.h"

/**********************************************************/
/* lee en cgs la cabecera general de senal de un fichero de
senal  de numeros enteros si el fichero tiene cabeceras viejas */

void vleecfs( FILE * fp, struct cgsenal * cgs )
{
reset_cgsenal(cgs);
xfseek(fp,0,SEEK_SET);
lee_csenal_vieja(fp,cgs);
xfseek(fp,512,SEEK_SET);
}

/**********************************************************/
/* salva la cabecera general de senal de un fichero original
de senal de numeros enteros, con formato de cabeceras viejas */

void vescribecfs( FILE * fp, struct cgsenal * cgs )
{
xfseek(fp,0,SEEK_SET);
escribe_csenal_vieja(fp,cgs);
}

/**********************************************************/
/* lee en cgs la cabecera general de senal de un fichero de
senal. Devuelve el tipo de dato.
Si tdato es -1, cualquier tipo vale. Si se especifica algun tipo
concreto y no es, da error y sale al SO */

UCHAR1 leecfs( FILE * fp, struct cgsenal * cgs, int tdato )
{
struct cfich cf;

vete_cfich(fp);
lee_cfich(fp,&cf);

cdie_beep(!es_fsenal(cf.tfich),"error: no es fichero de senal");
cdie_beep((tdato!=-1)&&(cf.tdato!=tdato),"error: tipo de datos no valido");

reset_cgsenal(cgs);
vete_cgsenal(fp);
lee_cgsenal(fp,cgs);
vete_datos(fp);
return cf.tdato;
}

/**********************************************************/
/* salva la cabecera general de senal de un fichero de senal
de tipo ftyp, de datos tipo tdata */

void escribecfs_ftyp( FILE * fp,
                      struct cgsenal * cgs,
                      UINT2 ftyp,
                      UCHAR1 tdata )
{
struct cfich cf;

cf.tfich = ftyp;
cf.tdato = tdata;
cf.mcab = MC_SENAL;

vete_principio(fp);
escribe_cfich(fp,&cf);

escribe_long(fp,long_cgsenal(cgs));
escribe_cgsenal(fp,cgs);
escribe_long_actual(fp,MC_NINGUNA);
}

/**********************************************************/
/* salva la cabecera general de senal de un fichero original
de senal de datos de tipo tdata */

void escribecfs( FILE * fp,
                 struct cgsenal * cgs,
                 UCHAR1 tdata )
{
escribecfs_ftyp(fp,cgs,TF_SORIG,tdata);
}

/**********************************************************/
/* salva la cabecera general de senal de un fichero de senal
preenfatizada */

void escribe_cab_preenf( FILE * fp,
                         struct cgsenal * cgs,
                         UCHAR1 tdata )
{
escribecfs_ftyp(fp,cgs,TF_SENF,tdata);
}

/**********************************************************/
/* salva la cabecera general de senal de un fichero de senal
deenfatizada */

void escribe_cab_deenf( FILE * fp,
                        struct cgsenal * cgs,
                        UCHAR1 tdata )
{
escribecfs_ftyp(fp,cgs,TF_SDENF,tdata);
}

/**********************************************************/
/* salva la cabecera general de senal de un fichero de senal
filtrada */

void escribe_cab_filt( FILE * fp,
                       struct cgsenal * cgs,
                       UCHAR1 tdata )
{
escribecfs_ftyp(fp,cgs,TF_FILT,tdata);
}

/**********************************************************/
/* Escribe la cabecera de un fichero de parcors, TF_PARCOR con
   tipo dato short float TD_FLOAT4 y cabeceras de senal cgs y
   parametros parcor cgpp  (MC_SENAL | MC_PARAM) */

void escribe_cab_parcor( FILE * fp,
                         struct cgsenal * cgs,
                         struct cgp_parcor * cgpp )
{
struct cfich cf;

cf.tfich = TF_PARCOR;
cf.tdato = TD_FLOAT4;
cf.mcab = MC_SENAL | MC_PARAM;

vete_principio(fp);
escribe_cfich(fp,&cf);

escribe_long(fp,long_cgsenal(cgs));
escribe_cgsenal(fp,cgs);

escribe_long(fp,long_cgp_parcor());
escribe_cgp_parcor(fp,cgpp);

escribe_long_actual(fp,MC_NINGUNA);
}

/**********************************************************/
/* salva la cabecera general de senal y la de parametros lpc
de un fichero de tipo ftyp, y tipo dato tdat */

void escribecfslpc( FILE * fp,
                    struct cgsenal * cgs,
                    struct cgp_lpc * cpl,
                    UINT2 ftyp,
                    UINT2 tdat )
{
struct cfich cf;

cf.tfich = ftyp;
cf.tdato = (UCHAR1)tdat;
cf.mcab = MC_SENAL | MC_PARAM;

vete_principio(fp);
escribe_cfich(fp,&cf);

escribe_long(fp,long_cgsenal(cgs));
escribe_cgsenal(fp,cgs);

escribe_long(fp,long_cgp_lpc());
escribe_cgp_lpc(fp,cpl);

escribe_long_actual(fp,MC_NINGUNA);
}

/**********************************************************/
/* lee la cabecera general de senal y la de parametros lpc (si
existe) de un fichero de senal. Devuelve el tipo de dato.
Si tdato es -1 cualquier tipo vale. Si se especifica algun tipo
concreto y no es, da error y sale al SO.
Si hay cabecera lpc hayclpc==TRUE si no, FALSE */

UINT2 leecfslpc( FILE * fp,
                struct cgsenal * cgs,
                struct cgp_lpc * cpl,
                int tdato,
                BOOL * hayclpc )
{
struct cfich cf;

vete_cfich(fp);
lee_cfich(fp,&cf);

cdie_beep(!es_fsenal(cf.tfich),"error: no es fichero de senal");
cdie_beep((tdato!=-1)&&(cf.tdato!=tdato),"error: tipo de datos no valido");

reset_cgsenal(cgs);
vete_cgsenal(fp);
lee_cgsenal(fp,cgs);
*hayclpc = ((((cf).mcab)&(MC_PARAM))!=0);
*hayclpc = hay_cparam(cf);
if (*hayclpc) {
  vete_cgparam(fp);
  lee_cgp_lpc(fp,cpl);
  }
else
  memset(cpl,0,sizeof(*cpl));

vete_datos(fp);
return cf.tdato;
}

/**********************************************************/
/* Escribe la cabecera de un fichero de lpcs, TF_LPC con
   tipo dato short float TD_FLOAT4 y cabeceras de senal cgs y
   parametros lpc cgpl  (MC_SENAL | MC_PARAM) */

void escribe_cab_lpc( FILE * fp,
                      struct cgsenal * cgs,
                      struct cgp_lpc * cgpl )
{
escribecfslpc(fp,cgs,cgpl,TF_LPC,TD_FLOAT4);
}

/**********************************************************/

void lee_cab_lpc( FILE * fp,
                  struct cgp_lpc * cgpl )
{
struct cfich cf;

vete_cfich(fp);
lee_cfich(fp,&cf);

cdie_beep(cf.tfich!=TF_LPC,"error: not an LPC file");
cdie_beep(cf.tdato!=TD_FLOAT4,"error: file data must be 4-UCHARs floats");

vete_cgparam(fp);
lee_cgp_lpc(fp,cgpl);

vete_datos(fp);
}

/**********************************************************/
/* escribe la cabecera de los ficheros de error para sintesis LPC
para datos de tipo tdata */

void escribe_cab_err( FILE * fp,
                      struct cgsenal * cgs,
                      struct cgp_lpc * cgpl,
                      UCHAR1 tdata )
{
escribecfslpc(fp,cgs,cgpl,TF_SERR,tdata);
}

/**********************************************************/
/* escribe la cabecera de las senales sintetizadas LPC */

void escribe_cab_lpcsint( FILE * fp,
                          struct cgsenal * cgs,
                          struct cgp_lpc * cgpl,
                          UCHAR1 tdata )
{
escribecfslpc(fp,cgs,cgpl,TF_SLPC,tdata);
}

/**********************************************************/
/* salva la cabecera general de senal sintetizada por formantes,
de numeros enteros. Cabeceras nuevas solo */

void escribecfs_form( FILE * fp,
                      struct cgsenal * cgs,
                      int espera_en_marcas )
{
struct cfich cf;

cf.tfich = TF_SFOR;
cf.tdato = TD_INT2;
cf.mcab = MC_SENAL;

vete_principio(fp);
escribe_cfich(fp,&cf);

escribe_long(fp,long_cgsenal(cgs));
if (espera_en_marcas) {
	escribe_cgsenal_marc(fp,cgs);
	endian_tolittle32(&(cgs->marc.unimarc));
	xfwrite(&(cgs->marc.unimarc),sizeof(cgs->marc.unimarc),1,fp);
	endian_fromlittle32(&(cgs->marc.unimarc));
  }
else {
  escribe_cgsenal(fp,cgs);
  escribe_long_actual(fp,MC_NINGUNA);
  }
}

/**********************************************************/
