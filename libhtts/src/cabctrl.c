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

//2012 Inaki	prescindir de xalloc 
/**********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tdef.h"
#include "uti.h"
#include "cabdef0.h"
#include "cabctrl.h"

/**********************************************************/

FLOAT4 xlee_sfloat( FILE * f )
{
FLOAT4 sf;

xfread(&sf,sizeof(sf),1,f);
endian_fromlittle32(&sf);
return sf;
}

/**********************************************************/

void xescribe_sfloat( FILE * f,
                      FLOAT4 sf )
{
endian_tolittle32(&sf);
xfwrite(&sf,sizeof(sf),1,f);
}

/**********************************************************/

/* El fichero debe estar posicionado en la longitud.
   lee la longitud y se salta esa cabecera  */

void salta_cab( FILE * fp )
{
UINT4 l;

change_rw(fp);
xfread(&l,sizeof(l),1,fp);
endian_fromlittle32(&l);
xfseek(fp,l,SEEK_CUR);
}

/**********************************************************/
/* devuelve la mascara de cabeceras incluidas para el fichero fp.
el puntero al fichero puede estar en cualquier sitio, y al terminar
queda justo despues de leer la mascara de cabeceras */

UINT4 get_cmask( FILE * fp )
{
UINT4 mcab;

vete_cmask(fp);    /*vete a mascara*/
xfread(&mcab,sizeof(mcab),1,fp);         /*lee mascara*/
endian_fromlittle32(&mcab);
change_rw(fp);

return mcab;
}

/**********************************************************/
/* Va a la cabecera indicada en cabmask. Si cabmask=MC_NINGUNA (0) va al
   principio del fichero. Si cabmask=MC_TODAS ($FF...FF) va al principio
   de los datos.
   Devuelve 1 si todo va bien, 0 si no se puede ir a esa cabecera porque
   no existe, en cuyo caso, la posicion en el fichero vale lo que le de
   la REAL gana. Mejor si usas xvete_cab(). */

int vete_cab( FILE * fp, UINT4 cabmask )
{
UINT4 mcab;

if (cabmask==MC_NINGUNA) {
  vete_cfich(fp);
  return 1;
  }
else if (cabmask==MC_TODAS) {
  vete_cfich(fp);
  salta_cab(fp);
  return 1;
  }
else {
  mcab = get_cmask(fp);

  while (mcab) {
    if (mcab & cabmask & 0x0001)
      return 1;
    if (mcab & 0x0001)
      salta_cab(fp);

    mcab >>= 1;
    cabmask >>= 1;
    }
  }
return 0;
}

/**********************************************************/
/* Como vete_cab pero en caso de no existir la cabecera indicada
produce un error y sale al S.O */

void xvete_cab( FILE * fp, UINT4 cabmask )
{
cdie_beep(vete_cab(fp,cabmask)==0,"vete_cab error: requested file header not found" );
}

/**********************************************************/
/* lee la cabecera de fichero, estando posicionado el fichero en ella */

void lee_cfich( FILE * fp,
                struct cfich * cf_ptr)
{
xfread(cf_ptr,sizeof(*cf_ptr),1,fp);
endian_fromlittle32(&(cf_ptr->lcab));
endian_fromlittle16(&(cf_ptr->tfich));
endian_fromlittle32(&(cf_ptr->mcab));
}

/**********************************************************/
/* salva la cabecera de fichero, estando posicionado el fichero en ella */

void escribe_cfich( FILE * fp,
                    struct cfich * cf_ptr)
{
endian_tolittle32(&(cf_ptr->lcab));
endian_tolittle16(&(cf_ptr->tfich));
endian_tolittle32(&(cf_ptr->mcab));
xfwrite(cf_ptr,sizeof(*cf_ptr),1,fp);
endian_fromlittle32(&(cf_ptr->lcab));
endian_fromlittle16(&(cf_ptr->tfich));
endian_fromlittle32(&(cf_ptr->mcab));
}

/**********************************************************/
/* anade a la mascara de cabeceras, las cabeceras identificada en cabmask.
   El apuntador de fichero puede estar en cualquier sitio, y se vuelve
   a poner ah¡ al terminar */

void anade_cmask( FILE * fp, UINT4 cabmask )
{
fpos_t pos;
UINT4 cm;

xfgetpos(fp,&pos);

cm = get_cmask(fp);

cm |= cabmask;
vete_cmask(fp);                         /* vuelve a la mascara */
endian_tolittle32(&cm);
xfwrite(&cm,sizeof(cm),1,fp);           /* y la salva modificada */

xfsetpos(fp,&pos);
change_rw(fp);
}

/**********************************************************/
/* escribe la longitud indicada, en la posicion actual del fichero */

void escribe_long( FILE * fp, UINT4 l )
{
endian_tolittle32(&l);
xfwrite(&l,sizeof(l),1,fp);
}

/**********************************************************/
/* modifica la longitud de la cabecera definida por cabmask,
   considerando que el apuntador est  al final de esa cabecera.
   El apuntador se vuelve a dejar en el final de la cabecera. */

void escribe_long_actual( FILE * fp, UINT4 cabmask )
{
fpos_t pos;
long pos0, pos1;

xfgetpos(fp,&pos);

pos1 = xftell(fp);
xvete_cab(fp,cabmask);
pos0 = xftell(fp)+sizeof(UINT4);
escribe_long(fp, pos1-pos0);

xfsetpos(fp,&pos);
}

/**********************************************************/
/* devuelve la longitud de la cabecera definida por cabmask,
   desde el principio de ella hasta la posicion actual. */

long lee_long_actual( FILE * fp, UINT4 cabmask )
{
fpos_t pos;
long pos0, pos1;

xfgetpos(fp,&pos);

pos1 = xftell(fp);
xvete_cab(fp,cabmask);
pos0 = xftell(fp)+sizeof(UINT4);

xfsetpos(fp,&pos);
return(pos1-pos0);
}

/**********************************************************/
/*Utilizado para leer campos de longitud variable.

  Recibe un puntero a un puntero, y lo modifica:

  si *cvptr==NULL crea un buffer de tamano:  nb (si nb<>0)
                                             fnb (si nb==0)
                          Se utiliza malloc===> usar free!!

  si *cvptr<>NULL no modifica *cvptr.

  la funcion devuelve cuantos debemos leer del fichero en el buffer.

  Si is_txt es TRUE se supone que se va a leer texto, de forma que al
  reservar memoria se reserva un char de mas, para meter el cero de
  fin de cadena. Si ya me mandas un buffer creado SUPONGO que hay un
  char mas de lo que me dices en nb!!!.

  Modos normal de uso: mandar cvptr=NULL y nb=NULL --> creo buffer
  adecuado, y leo TODO el campo variable. Si me limitas nb, pues
  creo buffer con ese limite (+1 si is_txt==TRUE). */

UINT4 prepara_cvariable( pVOID * cvptr,
                            UINT4 nb,
                            UINT4 fnb,
                            BOOL is_txt )
{
UINT4 n;

if (*cvptr==NULL) {
  n = ((nb) ? nb : fnb)+( (is_txt) ? 1 : 0);
  if (n)
	  *cvptr = malloc(n);
  else
	 *cvptr = NULL;

  return( ((fnb<nb)||(!nb)) ? fnb : nb );
  }
else
  return( (fnb<nb) ? fnb : nb );
}

/**********************************************************/
/* Fichero posicionado en longitud del campo.
   lee un campo de texto variable (longitud seguido de array) en *cvptr.
   *cvptr y *nelem se inicializa con prepara_cvariable() y en funcion
   del numero de elementos en el fichero (fnb en prepara_cvariable()).
   Ver prepara_cvariable para mas informacion.
   Adem s se anade un cero al final del array leido, por lo
   que si me mandas un vector ya reservado en memoria, debe tener un
   char mas de lo que me indicas en nelem. Si el vector lo reservo yo,
   pues yo me lo curro.
   Si no se leen todos los elementos (ver prepara_cvariable()) devuelve
   1. Si se leen todos, la funcion devuelve 0.
   El fichero queda posicionado al FINAL del campo. */

int lee_cvariable_txt( FILE * fp,
                       pCHAR1 * cvptr,
                       UINT2 * nelem )
{
UINT2 w;

xfread(&w,sizeof(w),1,fp);
endian_fromlittle16(&w);
*nelem = (UINT2)prepara_cvariable((pVOID *)cvptr,((UINT4)*nelem)*sizeof(char),
                            ((UINT4)w)*sizeof(char),TRUE)/sizeof(char);
xfread(*cvptr,sizeof(char),*nelem,fp);
xfseek(fp,((UINT4)(w-(*nelem)))*sizeof(char),SEEK_CUR);

*((*cvptr)+(*nelem)) = 0;

return ( (*nelem<w) ? 1 : 0 );
}

/**********************************************************/
/* Fichero posicionado en longitud del campo.
   lee las marcas, de longitud variable (longitud seguido de array) en *cvptr.
   *cvptr y *nelem se inicializa con prepara_cvariable() y en funcion
   del numero de elementos en el fichero (fnb en prepara_cvariable()).
   Ver prepara_cvariable para mas informacion.
   Si no se leen todos los elementos (ver prepara_cvariable()) devuelve
   1. si se leen todos, la funcion devuelve 0.
   El fichero queda posicionado al FINAL del campo. */

int lee_cvariable_marcas( FILE * fp,
                          pUINT4 * cvptr,
                          UINT4 * nelem )
{
UINT4 l,i;
pUINT4 tmpptr;

xfread(&l,sizeof(l),1,fp);
endian_fromlittle32(&l);
*nelem = prepara_cvariable( (pVOID *)cvptr,(*nelem)*sizeof(UINT4),
														l*sizeof(UINT4),FALSE)/sizeof(UINT4);
tmpptr=(*cvptr);
for (i=0; i<*nelem;i++) {
	xfread((void *)(tmpptr),sizeof(UINT4),1,fp);
	endian_fromlittle32(tmpptr);
	tmpptr++;
	}
xfseek(fp,(l-(*nelem))*sizeof(UINT4),SEEK_CUR);

return ( (*nelem<l) ? 1 : 0 );
}

/**********************************************************/
/* salva un campo de texto variable (longitud seguido de array). */

void escribe_cvariable_txt( FILE * fp,
                            pCHAR1 cvptr,
                            UINT2 nelem )
{
UINT2 n=nelem;
endian_tolittle16(&nelem);
xfwrite(&nelem,sizeof(nelem),1,fp);
xfwrite(cvptr,sizeof(*cvptr),n,fp);
}

/**********************************************************/
/* salva las marcas. (longitud seguido de array). */

void escribe_cvariable_marcas( FILE * fp,
                               pUINT4 cvptr,
                               UINT4 nelem )
{
UINT4 i;

UINT4 n=nelem;
endian_tolittle32(&nelem);
xfwrite(&nelem,sizeof(nelem),1,fp);

for (i=0;i<n;i++) {
		endian_tolittle32(cvptr);
		xfwrite((void *)cvptr,sizeof(*cvptr),1,fp);
		endian_fromlittle32(cvptr);
		cvptr++;
	}
}

/**********************************************************/
/* devuelve la longitud de una cabecera general de senal */

UINT4 long_cgsenal( struct cgsenal * cgs )
{
#define SIZE(sf)  ((UINT4)sizeof(cgs->sf))
#define TSIZE(tf) (UINT4)(sizeof(cgs->tf.ltxt)+ \
                  ((UINT4)(cgs->tf.ltxt))*sizeof(*(cgs->tf.txt)))

return( SIZE(nm)+SIZE(fm)+SIZE(rango)+TSIZE(texto)+TSIZE(version)+
        TSIZE(locu)+TSIZE(obs)+sizeof(cgs->marc.unimarc)+
        sizeof(cgs->marc.nmarc)+
				(cgs->marc.nmarc)*sizeof(*(cgs->marc.marcas)) );
}

/**********************************************************/
/* borra (con free) los campos variables de una cabecera
general de senal, y pone a cero su longitud */

void free_cgsenal( struct cgsenal * cgs )
{
#define FREE(tf)  if (cgs->tf.txt!=NULL)  \
            free((void *)cgs->tf.txt); \
        cgs->tf.ltxt=0

FREE(locu);
FREE(texto);
FREE(version);
FREE(obs);
if (cgs->marc.marcas!=NULL)
    free((void *)cgs->marc.marcas);
cgs->marc.nmarc = 0;
}

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

int lee_cgsenal( FILE * fp, struct cgsenal * cgs )
{
#define READCGS32(sf)  { INT32 i32; xfread(&i32,sizeof(i32),1,fp); endian_fromlittle32(&i32); cgs->sf=i32; }
#define FREADCGS(sf)  cgs->sf = xlee_sfloat(fp)
#define TREADCGS(tf) { pCHAR1 pch=cgs->tf.txt; UINT2 l=cgs->tf.ltxt; retval|=lee_cvariable_txt(fp,&pch,&l); cgs->tf.txt=pch; cgs->tf.ltxt=l; }

int retval = 0;
pUINT4 p;
UINT4 nelem;

READCGS32(nm);
FREADCGS(fm);
FREADCGS(rango.min);
FREADCGS(rango.max);
TREADCGS(locu);
TREADCGS(texto);
TREADCGS(version);
TREADCGS(obs);
READCGS32(marc.unimarc);
p=cgs->marc.marcas;
nelem=cgs->marc.nmarc;
retval |= lee_cvariable_marcas(fp,&p,&nelem);
cgs->marc.marcas=p;
cgs->marc.nmarc=nelem;
return retval;
}

/**********************************************************/
/* salva una cabecera general de senal */

void escribe_cgsenal( FILE * fp, struct cgsenal * cgs )
{
#define WRITECGS32(sf)  { INT32 i32=cgs->sf; endian_tolittle32(&i32); xfwrite(&i32,sizeof(i32),1,fp); }
#define FWRITECGS(sf)  xescribe_sfloat(fp,cgs->sf)
#define TWRITECGS(tf) { pCHAR1 pch=cgs->tf.txt; UINT2 l=cgs->tf.ltxt; escribe_cvariable_txt( fp,pch,l); }
pUINT4 p;
UINT4 nelem;

WRITECGS32(nm);
FWRITECGS(fm);
FWRITECGS(rango.min);
FWRITECGS(rango.max);
TWRITECGS(locu);
TWRITECGS(texto);
TWRITECGS(version);
TWRITECGS(obs);
WRITECGS32(marc.unimarc);
p=cgs->marc.marcas;
nelem=cgs->marc.nmarc;
escribe_cvariable_marcas(fp,p,nelem);
}

/**********************************************************/
/* salva una cabecera general de senal, pero se queda en las marcas(no
llega a salvarlas */

void escribe_cgsenal_marc( FILE * fp, struct cgsenal * cgs )
{
#define WRITEM32(sf)  { INT32 i32=cgs->sf; endian_tolittle32(&i32); xfwrite(&i32,sizeof(i32),1,fp); }
#define FWRITEM(sf)  xescribe_sfloat(fp,cgs->sf)
#define TWRITEM(tf) { pCHAR1 pch=cgs->tf.txt; UINT2 l=cgs->tf.ltxt; escribe_cvariable_txt( fp,pch,l); }
WRITEM32(nm);
FWRITEM(fm);
FWRITEM(rango.min);
FWRITEM(rango.max);
TWRITEM(locu);
TWRITEM(texto);
TWRITEM(version);
TWRITEM(obs);
}

/**********************************************************/
/* quita los espacios finales al texto definido en txtd y actualiza
la nueva longitud */

void limpia_texto( pCHAR1 p, UINT2 *l )
{
pCHAR1 str;

str = p;

while ((str[0])&&(str[strlen(str)-1]<=' '))
	str[strlen(str)-1]=0;

*l = strlen(str);
}

/**********************************************************/
/* lee cabecera vieja, pero con el formato nuevo */

void lee_csenal_vieja( FILE * fp, struct cgsenal * cgs )
{
#define oREAD32(sf)  { INT32 i32; xfread(&i32,sizeof(i32),1,fp); endian_fromlittle32(&i32); cgs->sf=i32; }
#define oFREAD(sf)  cgs->sf = xlee_sfloat(fp)
#define oTREAD(tf,n) { \
	pCHAR1 pch=cgs->tf.txt; \
	UINT2 l=cgs->tf.ltxt; \
	l= (UINT2)prepara_cvariable( (pVOID *)&pch,l,n,TRUE); \
	xfread(pch,1,l,fp); \
	xfseek(fp,l,SEEK_CUR); \
	limpia_texto(pch,&l); \
	cgs->tf.txt=pch; \
	cgs->tf.ltxt=l; }

#define M(f) (cgs->marc.f)

INT2 n,u,m, i;
pUINT4 p;
UINT4 nelem;

oREAD32(nm);
oFREAD(fm);
cgs->rango.min=0;
cgs->rango.max=4096;
oTREAD(locu,39);
oTREAD(texto,56*3);
oTREAD(version,3);
oTREAD(obs,56*3);

xfread(&n,sizeof(n),1,fp); endian_fromlittle16(&n);
xfread(&u,sizeof(u),1,fp); endian_fromlittle16(&u);
M(unimarc)=u;
p=M(marcas);
nelem=M(nmarc);
nelem=prepara_cvariable( (pVOID *)&p,nelem*sizeof(UINT4),
														n*sizeof(UINT4),FALSE) / sizeof(UINT4);
for (i=0; i<n; i++) {
	xfread(&m,sizeof(INT2),1,fp);
	endian_fromlittle16(&m);
	p[i] = m-1;
	}
M(marcas)=p;
M(nmarc)=nelem;

xfseek(fp,512,SEEK_SET);
}

/**********************************************************/
/* escribe cabecera vieja, pero enviandosela con el formato nuevo */

void escribe_csenal_vieja( FILE * fp, struct cgsenal * cgs )
{
#define oWRITE32(sf) { INT32 i=cgs->sf; endian_tolittle32(&i); xfwrite(&i,sizeof(i),1,fp); }
#define oFWRITE(sf)  xescribe_sfloat(fp,cgs->sf)
#define oTWRITE(tf,n)  {                         \
	UINT2 tmp;                                      \
	INT8 c0=0;                                     \
																								 \
	tmp = cgs->tf.ltxt;                            \
	if (tmp>n)                                     \
		tmp = n;                                     \
	xfwrite(cgs->tf.txt,sizeof(INT8),tmp,fp);      \
  while (tmp++<n)                                \
		xfwrite(&c0,sizeof(INT8),1,fp);              \
  }

#define M(f) (cgs->marc.f)

UINT2 n,nn, u,m, i;

oWRITE32(nm);
oFWRITE(fm);
oTWRITE(locu,39);
oTWRITE(texto,56*3);
oTWRITE(version,3);
oTWRITE(obs,56*3);

n=(UINT2)M(nmarc);
if (n>40) n = 40;
nn=n;
endian_tolittle16(&n);
xfwrite(&n,sizeof(n),1,fp);
u=(UINT2)M(unimarc);
endian_tolittle16(&u);
xfwrite(&u,sizeof(u),1,fp);
for (i=0; i<nn; i++) {
	m=(UINT2)M(marcas)[i]+1;
	endian_tolittle16(&m);
	xfwrite(&m,sizeof(INT2),1,fp);
	}

i = 0;
while (xftell(fp)<512)
	xfwrite(&i,1,1,fp);
}

/**********************************************************/
/* lee una cabecera de parcors del fichero fp a partir de la posicion
actual */

void lee_cgp_parcor( FILE * fp,
                     struct cgp_parcor * cgp_pc_ptr )
{
xfread(cgp_pc_ptr,sizeof(*(cgp_pc_ptr)),1,fp);
endian_fromlittle16(&(cgp_pc_ptr->cgp.vecd.dimv));
endian_fromlittle32(&(cgp_pc_ptr->cgp.vecd.nvec));
endian_fromlittle16(&(cgp_pc_ptr->cgp.pventa.lvent));
endian_fromlittle16(&(cgp_pc_ptr->cgp.pventa.despv));
endian_fromlittle32(&(cgp_pc_ptr->preenf));
endian_fromlittle16(&(cgp_pc_ptr->orden));
}

/**********************************************************/
/* salva una cabecera de parcors en la posicion actual del fichero fp */

void escribe_cgp_parcor( FILE * fp,
												 struct cgp_parcor * cgp_pc_ptr)
{
	endian_tolittle16(&(cgp_pc_ptr->cgp.vecd.dimv));
	endian_tolittle32(&(cgp_pc_ptr->cgp.vecd.nvec));
	endian_tolittle16(&(cgp_pc_ptr->cgp.pventa.lvent));
	endian_tolittle16(&(cgp_pc_ptr->cgp.pventa.despv));
	endian_tolittle32(&(cgp_pc_ptr->preenf));
	endian_tolittle16(&(cgp_pc_ptr->orden));
	xfwrite(cgp_pc_ptr,sizeof(*(cgp_pc_ptr)),1,fp);
	endian_fromlittle16(&(cgp_pc_ptr->cgp.vecd.dimv));
	endian_fromlittle32(&(cgp_pc_ptr->cgp.vecd.nvec));
	endian_fromlittle16(&(cgp_pc_ptr->cgp.pventa.lvent));
	endian_fromlittle16(&(cgp_pc_ptr->cgp.pventa.despv));
	endian_fromlittle32(&(cgp_pc_ptr->preenf));
	endian_fromlittle16(&(cgp_pc_ptr->orden));
}

/**********************************************************/
/* lee una cabecera de lpcs del fichero fp a partir de la posicion
actual */

void lee_cgp_lpc( FILE * fp,
                  struct cgp_lpc * cgp_lpc_ptr)
{
xfread(cgp_lpc_ptr,sizeof(*(cgp_lpc_ptr)),1,fp);
endian_fromlittle16(&(cgp_lpc_ptr->cgp.vecd.dimv));
endian_fromlittle32(&(cgp_lpc_ptr->cgp.vecd.nvec));
endian_fromlittle16(&(cgp_lpc_ptr->cgp.pventa.lvent));
endian_fromlittle16(&(cgp_lpc_ptr->cgp.pventa.despv));
endian_fromlittle32(&(cgp_lpc_ptr->preenf));
endian_fromlittle16(&(cgp_lpc_ptr->orden));
}

/**********************************************************/
/* salva una cabecera de lpcs en la posicion actual del fichero fp */

void escribe_cgp_lpc( FILE * fp,
                      struct cgp_lpc * cgp_lpc_ptr)
{
endian_tolittle16(&(cgp_lpc_ptr->cgp.vecd.dimv));
endian_tolittle32(&(cgp_lpc_ptr->cgp.vecd.nvec));
endian_tolittle16(&(cgp_lpc_ptr->cgp.pventa.lvent));
endian_tolittle16(&(cgp_lpc_ptr->cgp.pventa.despv));
endian_tolittle32(&(cgp_lpc_ptr->preenf));
endian_tolittle16(&(cgp_lpc_ptr->orden));
xfwrite(cgp_lpc_ptr,sizeof(*(cgp_lpc_ptr)),1,fp);
endian_fromlittle16(&(cgp_lpc_ptr->cgp.vecd.dimv));
endian_fromlittle32(&(cgp_lpc_ptr->cgp.vecd.nvec));
endian_fromlittle16(&(cgp_lpc_ptr->cgp.pventa.lvent));
endian_fromlittle16(&(cgp_lpc_ptr->cgp.pventa.despv));
endian_fromlittle32(&(cgp_lpc_ptr->preenf));
endian_fromlittle16(&(cgp_lpc_ptr->orden));
}

/**********************************************************/
