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
#ifndef __SPL_H
#define __SPL_H

/**********************************************************/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/*
Copyright: 1994 - Grupo de Voz (DAET) ETSII/IT-Bilbao

Nombre fuente................ SPL.H
Nombre paquete............... SPL
Lenguaje fuente.............. C (Borland C/C++ 3.1)
Estado....................... Completado
Dependencia Hard/OS.......... -
Codigo condicional........... (def)LP_NEGSUM

Codificacion................. Borja Etxebarria

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
2.0.1    23/03/99  Borja     hz2lhz() lhz2hz()
2.1.0    20/01/98  Borja     M_PI y demas constantes
2.0.1    10/08/97  Borja     modificaciones en WIN_????_EFVAL 
2.0.0    01/07/97  Borja     incorporar units.h y otras funciones
1.2.0    24/06/97  Borja     incorporar lp_k2ta()
1.1.2    24/07/95  Borja     correccion nombre funcion bk?_fi2fc
1.1.1    30/07/95  Borja     TIPO SPL_PTR --> pTIPO
1.1.0    08/12/94  Borja     revision general (tipos,idx,nel,tnel...)
1.0.3    07/12/93  Borja     soporte LP_NEGSUM_NEG() y LP_POSSUM_NEG()
1.0.2    05/12/93  Borja     funciones para LSFs.
1.0.1    28/10/93  Borja     Calculo autovalores eigen_tdg()
1.0.0    06/07/93  Borja     Codificacion inicial.

======================== Contenido ========================
Fichero de cabecera para librerias SPL
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include <math.h>
#include "spltdef.h"

/**********************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************/
/* Definimos algunas constantes si no existen */

#ifndef M_E
#define M_E         2.71828182845904523536  /* e */
#endif
#ifndef M_LOG2E
#define M_LOG2E     1.44269504088896340736  /* log 2e */
#endif
#ifndef M_LOG10E
#define M_LOG10E    0.434294481903251827651  /* log 10e */
#endif
#ifndef M_LN2
#define M_LN2       0.693147180559945309417  /* log e2 */
#endif
#ifndef M_LN10
#define M_LN10      2.30258509299404568402  /* log e10 */
#endif
#ifndef M_PI
#define M_PI        3.14159265358979323846  /* pi */
#endif
#ifndef M_PI_2
#define M_PI_2      1.57079632679489661923  /* pi/2 */
#endif
#ifndef M_PI_4
#define M_PI_4      0.785398163397448309616  /* pi/4 */
#endif
#ifndef M_1_PI
#define M_1_PI      0.318309886183790671538  /* 1/pi */
#endif
#ifndef M_2_PI
#define M_2_PI      0.636619772367581343076  /* 2/pi */
#endif
#ifndef M_1_SQRTPI
#define M_1_SQRTPI  0.564189583547756286948  /* 1/sqrt(pi) */
#endif
#ifndef M_2_SQRTPI
#define M_2_SQRTPI  1.12837916709551257390  /* 2/sqrt(pi) */
#endif
#ifndef M_SQRT2
#define M_SQRT2     1.41421356237309504880  /* sqrt(2) */
#endif
#ifndef M_SQRT1_2
#define M_SQRT1_2    0.707106781186547524401  /* 1/sqrt(2) */
#endif

/**********************************************************/
/* Funciones de Bessel */

#define BESSEL_PRECISION 1e-10

SPL_FLOAT API bessel_I0( SPL_FLOAT x );

/**********************************************************/
/* matrices */

SPL_INT API idx_matrix_sim( SPL_INT i, SPL_INT j );
#define idx_matrix_tri(i,j) idx_matrix_sim(i,j)
SPL_INT API idx_matrix_dgn( SPL_INT i );
#define __idx_matrix_sim(i,j)  (((((i)-1)*(i))/2)+((j)-1))
#define __idx_matrix_tri(i,j)  __idx_matrix_sim(i,j)
#define __idx_matrix_dgn(i)  (i-1)

SPL_INT API nel_matrix_sim( SPL_INT n );
#define nel_matrix_tri(n)  nel_matrix_sim(n)
SPL_INT API nel_matrix_dgn( SPL_INT n );

SPL_VOID API matrix_sim2tri( SPL_pFLOAT msi, SPL_pFLOAT mtr, SPL_INT n );
SPL_VOID API matrix_sim2tri_dgn( SPL_pFLOAT msi, SPL_pFLOAT mtr,
		SPL_pFLOAT mdi, SPL_INT n );


/* sistemas de ecuaciones */

SPL_VOID API ecsys_tri( SPL_pFLOAT mA, SPL_pFLOAT vc, SPL_INT n,
		SPL_pFLOAT vx );
SPL_VOID API ecsys_simtri( SPL_pFLOAT mB, SPL_pFLOAT vc, SPL_INT n,
		SPL_pFLOAT vx );
SPL_VOID API ecsys_tri_dgn( SPL_pFLOAT mB, SPL_pFLOAT md, SPL_pFLOAT vc,
		SPL_INT n, SPL_pFLOAT vx );


/* autovalores y autovectores */

#define EIGEN_MAXITER 30

SPL_BOOL API eigen_tdg( SPL_pFLOAT vd, SPL_pFLOAT ve, SPL_INT n,
		SPL_pFLOAT mz );

/**********************************************************/
/* generacion de ventanas */

SPL_pFLOAT API win_rect( SPL_pFLOAT v, SPL_INT N );
SPL_pFLOAT API win_bart( SPL_pFLOAT v, SPL_INT N );
SPL_pFLOAT API win_hann( SPL_pFLOAT v, SPL_INT N );
SPL_pFLOAT API win_hamm( SPL_pFLOAT v, SPL_INT N );
SPL_pFLOAT API win_black( SPL_pFLOAT v, SPL_INT N );
SPL_pFLOAT API win_kais( SPL_pFLOAT v, SPL_INT N, SPL_FLOAT b );

SPL_pFLOAT API win2_rect( SPL_pFLOAT v, SPL_INT N );
SPL_pFLOAT API win2_bart( SPL_pFLOAT v, SPL_INT N );
SPL_pFLOAT API win2_hann( SPL_pFLOAT v, SPL_INT N );
SPL_pFLOAT API win2_hamm( SPL_pFLOAT v, SPL_INT N );
SPL_pFLOAT API win2_black( SPL_pFLOAT v, SPL_INT N );
SPL_pFLOAT API win2_kais( SPL_pFLOAT v, SPL_INT N, SPL_FLOAT b );

SPL_FLOAT API wini_rect( SPL_INT i, SPL_INT N );
SPL_FLOAT API wini_bart( SPL_INT i, SPL_INT N );
SPL_FLOAT API wini_hann( SPL_INT i, SPL_INT N );
SPL_FLOAT API wini_hamm( SPL_INT i, SPL_INT N );
SPL_FLOAT API wini_black( SPL_INT i, SPL_INT N );
SPL_FLOAT API wini_kais( SPL_INT i, SPL_INT N, SPL_FLOAT b );

SPL_INT API nel_win( SPL_INT N );
SPL_INT API nel_win2( SPL_INT N );
SPL_INT API idx_win2( SPL_INT i, SPL_INT N );

/* longitud efectiva de una ventana (para potencia).
Realmente no es fijo. mejor usar la energia normalizada
de la ventana en cuestion (acorr_i(win,len,0)/len).  */
#define WIN_HAMM_EFVAL 0.397322

/**********************************************************/
/* cruces por cero */

SPL_INT API zero_cross( SPL_pFLOAT v, SPL_INT N );

/**********************************************************/
/* preenfasis y deenfasis */

SPL_FLOAT API preen_i( SPL_FLOAT a, SPL_FLOAT i, SPL_FLOAT i0 );
SPL_pFLOAT API preen_n( SPL_FLOAT a, SPL_pFLOAT vi, SPL_pFLOAT vo,
		SPL_INT N, SPL_FLOAT i0 );

SPL_FLOAT API deen_i( SPL_FLOAT a, SPL_FLOAT i, SPL_FLOAT o0 );
SPL_pFLOAT API deen_n( SPL_FLOAT a, SPL_pFLOAT vi, SPL_pFLOAT vo,
		SPL_INT N, SPL_FLOAT o0 );

/**********************************************************/
/* autocorrelacion */

SPL_FLOAT API acorr_i( SPL_pFLOAT v, SPL_INT N, SPL_INT i );
SPL_FLOAT API acorr_i_n( SPL_pFLOAT v, SPL_INT N, SPL_INT i );

SPL_pFLOAT API acorr_ab( SPL_pFLOAT v, SPL_INT N, SPL_pFLOAT vac,
		SPL_INT a, SPL_INT b );
SPL_FLOAT API acorr_ab_n( SPL_pFLOAT v, SPL_INT N, SPL_pFLOAT vac,
		SPL_INT a, SPL_INT b );

SPL_INT API nel_acorr_ab( SPL_INT a, SPL_INT b );

/**********************************************************/
/* covarianza */

SPL_FLOAT API covar_ij( SPL_pFLOAT v, SPL_INT N, SPL_INT p,
		SPL_INT i, SPL_INT j );
SPL_FLOAT API covar_ij_n( SPL_pFLOAT v, SPL_INT N, SPL_INT p,
		SPL_INT i, SPL_INT j );

SPL_pFLOAT API xcovm_st( SPL_pFLOAT v, SPL_INT N, SPL_INT p, SPL_pFLOAT mcv );
SPL_FLOAT API xcovm_st_n( SPL_pFLOAT v, SPL_INT N, SPL_INT p,
		SPL_pFLOAT mcv );

SPL_pFLOAT API xcovm_lt( SPL_pFLOAT v, SPL_INT N, SPL_INT fN,
		SPL_INT p, SPL_INT M, SPL_pFLOAT mcv );
SPL_FLOAT API xcovm_lt_n( SPL_pFLOAT v, SPL_INT N, SPL_INT fN,
		SPL_INT p, SPL_INT M, SPL_pFLOAT mcv );

#define idx_covm(i,j)  idx_matrix_sim(i,j)
#define nel_covm(p)  nel_matrix_sim(p)
#define __idx_covm(i,j)  __idx_matrix_sim(i,j)

SPL_INT API idx_xcovm( SPL_INT i, SPL_INT j, SPL_INT p );
SPL_INT API nel_xcovm( SPL_INT p );
#define __idx_xcovm(i,j,p)  (((j)==0)?(i):(p)+1+(__idx_covm(i,j)))

SPL_FLOAT API covar_w_ij( SPL_pFLOAT v, SPL_INT N, SPL_INT p,
		SPL_INT i, SPL_INT j, SPL_pFLOAT vw );
SPL_FLOAT API covar_w_ij_n( SPL_pFLOAT v, SPL_INT N, SPL_INT p,
		SPL_INT i, SPL_INT j, SPL_pFLOAT vw );

SPL_pFLOAT API xcovm_w_st( SPL_pFLOAT v, SPL_INT N, SPL_INT p,
		SPL_pFLOAT mcv, SPL_pFLOAT vw );
SPL_FLOAT API xcovm_w_st_n( SPL_pFLOAT v, SPL_INT N, SPL_INT p,
		SPL_pFLOAT mcv, SPL_pFLOAT vw );

/**********************************************************/
/* Prediccion lineal (LP) */

/* Control de signo en las rutinas LP */
/* desconectado, utilizamos sumatorio positivo */
/* #define LP_NEGSUM */

/* macros LP_NEGSUM_NEG(n) y LP_POSSUM_NEG(n) para gestion de signo
en funcion de LP_NEGSUM.
LP_NEGSUM_NEG pone signo menos (NEG) solo si LP_NEGSUM esta definido.
LP_POSSUM_NEG pone signo menos (NEG) solo si LP_NEGSUM _no_ esta definido. */
#ifdef LP_NEGSUM
#define LP_NEGSUM_NEG(n) (-(n))
#define LP_POSSUM_NEG(n) (n)
#else
#define LP_NEGSUM_NEG(n) (n)
#define LP_POSSUM_NEG(n) (-(n))
#endif


/* funciones de analisis de prediccion lineal */

SPL_FLOAT API lpa_cor_dur_ka( SPL_pFLOAT vac, SPL_pFLOAT vki,
		SPL_pFLOAT vai, SPL_INT p, SPL_pFLOAT vtmp );
SPL_FLOAT API lpa_cor_dur_k( SPL_pFLOAT vac, SPL_pFLOAT vki,
		SPL_INT p, SPL_pFLOAT vtmp );
SPL_FLOAT API lpa_cor_dur_a( SPL_pFLOAT vac, SPL_pFLOAT vai,
		SPL_INT p, SPL_pFLOAT vtmp );
SPL_FLOAT API lpa_cor_dur_e( SPL_pFLOAT vac, SPL_INT p, SPL_pFLOAT vtmp );

SPL_FLOAT API lpa_cor_atl_a( SPL_pFLOAT vac, SPL_pFLOAT vai,
		SPL_INT p, SPL_pFLOAT vtmp );

SPL_FLOAT API lpa_cor_sls_ka( SPL_pFLOAT vac, SPL_pFLOAT vki,
		SPL_pFLOAT vai, SPL_INT p, SPL_pFLOAT vtmp );
SPL_FLOAT API lpa_cor_sls_k( SPL_pFLOAT vac, SPL_pFLOAT vki,
		SPL_INT p, SPL_pFLOAT vtmp );
SPL_FLOAT API lpa_cor_sls_a( SPL_pFLOAT vac, SPL_pFLOAT vai,
		SPL_INT p, SPL_pFLOAT vtmp );
SPL_FLOAT API lpa_cor_sls_e( SPL_pFLOAT vac, SPL_INT p, SPL_pFLOAT vtmp );
SPL_FLOAT API lpa_cor_sls_xa( SPL_pFLOAT vac, SPL_pFLOAT vxs,
		SPL_INT p, SPL_pFLOAT vtmp );
SPL_VOID API lpa_cor_sls_xa_ne( SPL_pFLOAT vac, SPL_pFLOAT vxs,
		SPL_INT p, SPL_pFLOAT vtmp );

SPL_FLOAT API lpa_cor_sla_ka( SPL_pFLOAT vac, SPL_pFLOAT vki,
		SPL_pFLOAT vai, SPL_INT p, SPL_pFLOAT vtmp );
SPL_FLOAT API lpa_cor_sla_k( SPL_pFLOAT vac, SPL_pFLOAT vki,
		SPL_INT p, SPL_pFLOAT vtmp );
SPL_FLOAT API lpa_cor_sla_a( SPL_pFLOAT vac, SPL_pFLOAT vai,
		SPL_INT p, SPL_pFLOAT vtmp );
SPL_FLOAT API lpa_cor_sla_e( SPL_pFLOAT vac, SPL_INT p, SPL_pFLOAT vtmp );
SPL_FLOAT API lpa_cor_sla_xa( SPL_pFLOAT vac, SPL_pFLOAT vxa,
		SPL_INT p, SPL_pFLOAT vtmp );
SPL_VOID API lpa_cor_sla_xa_ne( SPL_pFLOAT vac, SPL_pFLOAT vxa,
		SPL_INT p, SPL_pFLOAT vtmp );

SPL_BOOL API lpa_cor_slv_lx( SPL_pFLOAT vac, SPL_pFLOAT vlx,
		SPL_INT p, SPL_pFLOAT E, SPL_pFLOAT vtmp );

SPL_VOID API lpa_cov_chl_a_ne( SPL_pFLOAT mcv, SPL_pFLOAT vai,
		SPL_INT p, SPL_pFLOAT vtmp );
SPL_FLOAT API lpa_cov_chl_a( SPL_pFLOAT mcv, SPL_pFLOAT vai,
		SPL_INT p, SPL_pFLOAT vtmp );

SPL_FLOAT API lpa_cov_sch_k( SPL_pFLOAT mcv, SPL_pFLOAT vki,
		SPL_INT p, SPL_pFLOAT vtmp );


/* funciones varias para prediccion lineal */

SPL_pFLOAT API lp_k2a( SPL_pFLOAT vki, SPL_pFLOAT vai,
		SPL_INT p, SPL_pFLOAT vtmp );
SPL_pFLOAT API lp_a2k( SPL_pFLOAT vai, SPL_pFLOAT vki,
		SPL_INT p, SPL_pFLOAT vtmp );
SPL_pFLOAT API lp_k2ta( SPL_pFLOAT vki, SPL_pFLOAT vti, SPL_INT p );
SPL_FLOAT API lp_a2rh( SPL_pFLOAT vai, SPL_pFLOAT vrh,
		SPL_INT p, SPL_pFLOAT vtmp );
SPL_pFLOAT API lp_a2ra( SPL_pFLOAT vai, SPL_pFLOAT vra, SPL_INT p );
SPL_pFLOAT API lp_a2c( SPL_pFLOAT vai, SPL_pFLOAT vci, SPL_INT p );
SPL_FLOAT API lp_c0( SPL_FLOAT e );
SPL_pFLOAT API lp_a2bwea( SPL_pFLOAT vai, SPL_pFLOAT vbwea,
		SPL_INT p, SPL_FLOAT gamma );
SPL_VOID API lp_a2pq( SPL_pFLOAT vai, SPL_INT p, SPL_pFLOAT vpi,
		SPL_pFLOAT vqi, SPL_pFLOAT vtmp );
SPL_pFLOAT API lp_lx2lw( SPL_pFLOAT vlx, SPL_pFLOAT vlw, SPL_INT p );
SPL_pFLOAT API lp_lw2lx( SPL_pFLOAT vlw, SPL_pFLOAT vlx, SPL_INT p );
SPL_BOOL API lp_a2lx( SPL_pFLOAT vai, SPL_pFLOAT vlx,
		SPL_INT p, SPL_pFLOAT vtmp );
SPL_pFLOAT API lp_lx2a( SPL_pFLOAT vlx, SPL_pFLOAT vai,
		SPL_INT p, SPL_pFLOAT vtmp );


/* funciones de sintesis/reconstruccion para prediccion lineal */

SPL_FLOAT API lps_P_a( SPL_pFLOAT vs, SPL_pFLOAT vai, SPL_INT p );
SPL_FLOAT API lps_A_a( SPL_pFLOAT vs, SPL_pFLOAT vai, SPL_INT p,
		SPL_FLOAT sn );
SPL_FLOAT API lps_H_a( SPL_pFLOAT vs, SPL_pFLOAT vai, SPL_INT p,
		SPL_FLOAT en );

SPL_pFLOAT API lps_P_pn_a( SPL_pFLOAT vai, SPL_INT p,
	SPL_pFLOAT vpn, SPL_INT N );
SPL_pFLOAT API lps_A_an_a( SPL_pFLOAT vai, SPL_INT p,
	SPL_pFLOAT van, SPL_INT N );
SPL_pFLOAT API lps_H_hn_a( SPL_pFLOAT vai, SPL_INT p,
	SPL_pFLOAT vhn, SPL_INT N );


/* longitud de vectores temporales para funciones LP */

SPL_INT API tnel_lpa_cor_dur_ka( SPL_INT p );
SPL_INT API tnel_lpa_cor_dur_k( SPL_INT p );
SPL_INT API tnel_lpa_cor_dur_a( SPL_INT p );
SPL_INT API tnel_lpa_cor_dur_e( SPL_INT p );

SPL_INT API tnel_lpa_cor_atl_a( SPL_INT p );

SPL_INT API tnel_lpa_cor_sls( SPL_INT p );
#define tnel_lpa_cor_sls_ka(p)  tnel_lpa_cor_sls(p)
#define tnel_lpa_cor_sls_k(p)  tnel_lpa_cor_sls(p)
#define tnel_lpa_cor_sls_a(p)  tnel_lpa_cor_sls(p)
#define tnel_lpa_cor_sls_e(p)  tnel_lpa_cor_sls(p)
#define tnel_lpa_cor_sls_xa(p)  tnel_lpa_cor_sls(p)
#define tnel_lpa_cor_sls_xa_ne(p)  tnel_lpa_cor_sls(p)

SPL_INT API tnel_lpa_cor_sla( SPL_INT p );
#define tnel_lpa_cor_sla_ka(p)  tnel_lpa_cor_sla(p)
#define tnel_lpa_cor_sla_k(p)  tnel_lpa_cor_sla(p)
#define tnel_lpa_cor_sla_a(p)  tnel_lpa_cor_sla(p)
#define tnel_lpa_cor_sla_e(p)  tnel_lpa_cor_sla(p)
#define tnel_lpa_cor_sla_xa(p)  tnel_lpa_cor_sla(p)
#define tnel_lpa_cor_sla_xa_ne(p)  tnel_lpa_cor_sla(p)

SPL_INT API tnel_lpa_cor_slv_lx( SPL_INT p );

SPL_INT API tnel_lpa_cov_chl( SPL_INT p );
#define tnel_lpa_cov_chl_a(p)  tnel_lpa_cov_chl(p)
#define tnel_lpa_cov_chl_a_ne(p)  tnel_lpa_cov_chl(p)

SPL_INT API tnel_lpa_cov_sch_k( SPL_INT p );

SPL_INT API tnel_lp_k2a( SPL_INT p );
SPL_INT API tnel_lp_a2k( SPL_INT p );
SPL_INT API tnel_lp_a2rh( SPL_INT p );
#define tnel_lp_rh2a(p)  tnel_lpa_cor_sls_a(p)
SPL_INT API tnel_lp_a2pq( SPL_INT p );
SPL_INT API tnel_lp_a2lx( SPL_INT p );
SPL_INT API tnel_lp_lx2a( SPL_INT p );

/**********************************************************/
/* ruido gaussiano */

SPL_FLOAT API noise_i_m( SPL_INT c );
SPL_pFLOAT API noise_n_m( SPL_pFLOAT v, SPL_INT N, SPL_INT c );
SPL_FLOAT API noise_mvar( SPL_INT c );

SPL_FLOAT API noise_i_v( SPL_INT c );
SPL_pFLOAT API noise_n_v( SPL_pFLOAT v, SPL_INT N, SPL_INT c );

SPL_FLOAT API gnoise_i( SPL_INT c, SPL_FLOAT mean, SPL_FLOAT var );
SPL_pFLOAT API gnoise_n( SPL_pFLOAT v, SPL_INT N, SPL_INT c,
		SPL_FLOAT mean, SPL_FLOAT var );

/**********************************************************/
/* filtros FIR lineales */

SPL_INT API fil_get_L( SPL_FLOAT deltaA, SPL_FLOAT deltaF );
SPL_FLOAT API fil_get_deltaA( SPL_INT L, SPL_FLOAT deltaF );
SPL_FLOAT API fil_get_deltaF( SPL_INT L, SPL_FLOAT deltaA );

SPL_pFLOAT API fil_lpf( SPL_pFLOAT vh, SPL_FLOAT deltaA,
		SPL_FLOAT fc, SPL_INT L, SPL_FLOAT g );
SPL_pFLOAT API fil_hpf( SPL_pFLOAT vh, SPL_FLOAT deltaA,
		SPL_FLOAT fc, SPL_INT L, SPL_FLOAT g );
SPL_pFLOAT API fil_bpf( SPL_pFLOAT vh, SPL_FLOAT deltaA,
		SPL_FLOAT fc1, SPL_FLOAT fc2,
		SPL_INT L, SPL_FLOAT g );
SPL_pFLOAT API fil_rbf( SPL_pFLOAT vh, SPL_FLOAT deltaA,
		SPL_FLOAT fc1, SPL_FLOAT fc2,
		SPL_INT L, SPL_FLOAT g );

SPL_INT API nel_fil( SPL_INT L );

SPL_FLOAT API fil_fil( SPL_pFLOAT vx, SPL_pFLOAT vh, SPL_INT L );

/**********************************************************/
/* DFT, IDFT,  utilizando FFT */

/* factor de escala: mul a DFT, div a IDFT */
#define FFT_FACTOR  1.0

/* umbral: abs(n)<FFT_ZERO==>n=0 */
#define FFT_ZERO  1.0e-6

/* constantes utiles */
#define FFT_DIRECT  SPL_FALSE
#define FFT_INVERSE  SPL_TRUE

/* primitivas basicas para la fft */
SPL_INT API fft_n_bits( SPL_INT np );
SPL_BOOL API fft_test_2pow( SPL_INT n );
SPL_VOID API fft_fill_half_tsin( SPL_pFLOAT vhts, SPL_INT tp );
SPL_VOID API fft_fill_tsin_tcos( SPL_pFLOAT vts,
		SPL_pFLOAT vtc, SPL_INT tp );
SPL_VOID API fft_htsin_fill_tsin_tcos( SPL_pFLOAT vts,
		SPL_pFLOAT vtc, SPL_pFLOAT vhts, SPL_INT tp );
SPL_VOID API fft_fill_tinv( SPL_pINT vti, SPL_INT tp );
SPL_VOID API fft_inverse_vecs( SPL_pFLOAT vre,
		SPL_pFLOAT vim, SPL_INT np );
SPL_VOID API fft_scramble_re_in( SPL_pFLOAT vre,
		SPL_pFLOAT vim, SPL_INT np );
SPL_VOID API fft_unscramble_cx_out( SPL_pFLOAT vre,
		SPL_pFLOAT vim, SPL_INT np,
		SPL_pFLOAT vhts, SPL_BOOL inv );
SPL_VOID API fft_expand_cx_out( SPL_pFLOAT vre,
		SPL_pFLOAT vim, SPL_INT np );
SPL_VOID API fft_fft( SPL_INT nb, SPL_BOOL inv, SPL_pFLOAT vre,
		SPL_pFLOAT vim, SPL_pFLOAT vts,
		SPL_pFLOAT vtc, SPL_FLOAT ufac );

/* funciones de aplicacion basicas */
SPL_VOID API fft_cxfft( SPL_pFLOAT vre, SPL_pFLOAT vim, SPL_INT np,
		SPL_BOOL inv, SPL_FLOAT ufac, SPL_pFLOAT vtmp );
SPL_INT API tnel_fft_cxfft( SPL_INT np );

SPL_VOID API fft_refft( SPL_pFLOAT vre, SPL_pFLOAT vim, SPL_INT np,
		SPL_BOOL inv, SPL_FLOAT ufac, SPL_pFLOAT vtmp );
SPL_INT API tnel_fft_refft( SPL_INT np );

/* Relaciones polares-rectangulares, utilizando el umbral FFT_ZERO.
Solo es recomendable utilizar fft_zcx_arg() */
SPL_FLOAT API fft_zcx_norm( SPL_FLOAT re, SPL_FLOAT im );
SPL_FLOAT API fft_zcx_arg( SPL_FLOAT re, SPL_FLOAT im );
#define fft_zcx_mag(re,im)  sqrt(fft_zcx_norm(re,im))

/* Relaciones polares-rectangulares.
Se recomienda fft_zcx_arg() frente a fft_cx_arg() */
SPL_FLOAT API fft_cx_norm( SPL_FLOAT re, SPL_FLOAT im );
#define fft_cx_arg(re,im)  atan2(im,re)
#define fft_cx_mag(re,im)  sqrt(fft_cx_norm(re,im))

#define fft_cx_re(mag,arg)  ((mag)*cos(arg))
#define fft_cx_im(mag,arg)  ((mag)*sin(arg))

/* macros rapidos */
#define __fft_cx_norm(re,im) ((re)*(re)+(im)*(im))
#define __fft_cx_mag(re,im) sqrt(__fft_cx_norm(re,im))

/* FFT: varias */
VOID API spectral_color( SPL_INT colornum,
	SPL_FLOAT ncolors, SPL_FLOAT *r, SPL_FLOAT *g, SPL_FLOAT *b );

/**********************************************************/
/* tramas y ventanas */

SPL_INT API bki_env( SPL_INT fl, SPL_INT prefr, SPL_INT posfr,
		SPL_INT wl, SPL_INT prewi, SPL_INT poswi,
		SPL_pINT tprefr, SPL_pINT tposfr,
		SPL_pINT tprewi, SPL_pINT tposwi );
SPL_LONG API bki_n2fn( SPL_INT fl, SPL_LONG n );
SPL_LONG API bki_fn2n( SPL_INT fl, SPL_LONG fn );
SPL_LONG API bki_fi2fs( SPL_INT fl, SPL_LONG fi );
SPL_LONG API bki_fi2fe( SPL_INT fl, SPL_LONG fi );
SPL_LONG API bki_fi2fc( SPL_INT fl, SPL_LONG fi );
SPL_LONG API bki_i2fi( SPL_INT fl, SPL_LONG i );

SPL_FLOAT API bkd_env( SPL_FLOAT fl, SPL_FLOAT prefr, SPL_FLOAT posfr,
		SPL_FLOAT wl, SPL_FLOAT prewi, SPL_FLOAT poswi,
		SPL_pFLOAT tprefr, SPL_pFLOAT tposfr,
		SPL_pFLOAT tprewi, SPL_pFLOAT tposwi );
SPL_FLOAT API bkd_n2fn( SPL_FLOAT fl, SPL_FLOAT n );
SPL_FLOAT API bkd_fn2n( SPL_FLOAT fl, SPL_FLOAT fn );
SPL_FLOAT API bkd_fi2fs( SPL_FLOAT fl, SPL_FLOAT fi );
SPL_FLOAT API bkd_fi2fe( SPL_FLOAT fl, SPL_FLOAT fi );
SPL_FLOAT API bkd_fi2fc( SPL_FLOAT fl, SPL_FLOAT fi );
SPL_FLOAT API bkd_i2fi( SPL_FLOAT fl, SPL_FLOAT i );

/**********************************************************/
/* conversion de unidades: lineal, dB, ms, muestras, etc. */

/* defines para a2db() y aa2db() */
#define DB_MIN_A 1e-15
#define DB_MIN_AA (DB_MIN_A*DB_MIN_A)

/* defines para lin2log2() y log22lin() */
#define LOG2_MIN 1e-15

/* defines para lin2ln() y ln2lin() */
#define LN_MIN 1e-15

/* Constantes que definen los rangos de las muestras de 16 bits */
#define INT16_RANGE 32768L
#define INT16_MIN -32768L
#define INT16_MAX 32767L

SPL_FLOAT API samples2ms( SPL_FLOAT samples, SPL_FLOAT freq );
SPL_FLOAT API ms2samples( SPL_FLOAT ms, SPL_FLOAT freq );
SPL_FLOAT API samples2hz( SPL_FLOAT samples, SPL_FLOAT freq );
SPL_FLOAT API hz2samples( SPL_FLOAT hz, SPL_FLOAT freq );
SPL_FLOAT API hz2ms( SPL_FLOAT hz );
SPL_FLOAT API ms2hz( SPL_FLOAT ms );
SPL_FLOAT API hz2rad( SPL_FLOAT hz, SPL_FLOAT freq );
SPL_FLOAT API rad2hz( SPL_FLOAT rad, SPL_FLOAT freq );
SPL_FLOAT API hz2lhz( SPL_FLOAT hz );
SPL_FLOAT API lhz2hz( SPL_FLOAT l2 );
SPL_FLOAT API hz2lnhz( SPL_FLOAT hz );
SPL_FLOAT API lnhz2hz( SPL_FLOAT lnhz );
#define ms2lms(x) hz2lhz(x)
#define lms2ms(x) lhz2hz(x)
#define samples2lsamples(x) hz2lhz(x)
#define lsamples2samples(x) lhz2hz(x)

SPL_FLOAT API r2bw( SPL_FLOAT r, SPL_FLOAT freq );
SPL_FLOAT API bw2r( SPL_FLOAT bw, SPL_FLOAT freq );

SPL_FLOAT API a2db( SPL_FLOAT a );
SPL_FLOAT API db2a( SPL_FLOAT db );
SPL_FLOAT API aa2db( SPL_FLOAT aa );
SPL_FLOAT API db2aa( SPL_FLOAT db );

SPL_FLOAT API a2bd( SPL_FLOAT a );
SPL_FLOAT API bd2a( SPL_FLOAT bd );
SPL_FLOAT API aa2bd( SPL_FLOAT aa );
SPL_FLOAT API bd2aa( SPL_FLOAT bd );

SPL_FLOAT API normalize( SPL_FLOAT n, SPL_FLOAT neg1, SPL_FLOAT pos1 );
SPL_FLOAT API unnormalize( SPL_FLOAT n, SPL_FLOAT neg1, SPL_FLOAT pos1 );
SPL_FLOAT API snormalize( SPL_FLOAT n, SPL_FLOAT neg1, SPL_FLOAT pos1 );
SPL_FLOAT API sunnormalize( SPL_FLOAT n, SPL_FLOAT neg1, SPL_FLOAT pos1 );

SPL_FLOAT API norm_i16( INT16 i16 );
INT16 API unnorm_i16( SPL_FLOAT f );
#define snorm_i16(i16)  norm_i16(i16)
INT16 API sunnorm_i16( SPL_FLOAT f );

SPL_FLOAT API lin2log2( SPL_FLOAT x );
SPL_FLOAT API log22lin( SPL_FLOAT x );

SPL_FLOAT API lin2ln( SPL_FLOAT x );
#define ln2lin(x) exp(x)

/**********************************************************/

#ifdef __cplusplus
}  /* extern "C" */
#endif

/**********************************************************/

#endif

