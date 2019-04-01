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
// Versión reducida de AHOcoder para hts_engine
// Daniel Erro, AHOLAB, Bilbao 2012

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <float.h>

#define PI 3.14159265358979323846
#define DB2EXP 0.11512925464970231

#define MINGENF0 10.0     // minima f0 generable (limitador por bajo pa no saturar memoria)
#define MAXGENF0 1500.0   // maxima f0 generable (limitador por alto pa no filtrar paso alto cuando no tiene sentido)
#define MINGENFV 1000.0   // minima fv generable (limitador por bajo pa que no haya falsas zonas unvoiced)
#define DEFFV 4500.0      // si no hay modelado explícito de fvoicing, este será el valor de referencia
#define ATT0HZ -32.0      // atenuacion del filtro paso-alto del ruido a 0hz
#define ATT08FV -20.0     // atenuacion del filtro paso-alto del ruido a 0.8 veces la fv
#define F0UV 100.0        // cuando haya que asignar una f0 al ruido, se tomará este valor
#define OLANOISE 0.193322 // factor para el ruido cuando se genere por overlap-add (=log(1.2132))
#define DCWINS 2.0        // factor de forma de la ventana de ruido
#define ADAPTLEVELS 1     // niveles adaptados (1) o no (0) a los de la demo straight
#define AMPNORMALIZE 1    // expandir/contraer wav hasta ocupar el rango de amplitud disponible (1) o no hacerlo (0)


/********** Funciones de pasar a wav **********/

int wavampnormalize(unsigned int Lx,double *x) {
	unsigned int k;
	double aux,aux2;
	for (k=0,aux=0.0;k<Lx;k++) if ((aux2=fabs(x[k]))>aux) aux=aux2;
	for (aux=0.99/aux,k=0;k<Lx;k++) x[k]*=aux;
	return 0;
}

int wavdouble2short(unsigned int Lx,double *x,short *s) {
	unsigned int k;
	for (k=0;k<Lx;k++) {
		if (x[k]>=0.999969482421875) s[k]=32767;
		else if (x[k]<=-1.0) s[k]=-32768;
		else s[k]=(short)(32768.0*x[k]);
	}
	return 0;
}

/********** Funciones de ifft **********/

int ifftr(int N,double *xre,double *xim) {
	int n,n1,n2,m,log2N,k,kk,M,M2;
	double aux1,aux2,cosw,sinw,cosdw,sindw,Are,Aim,Bre,Bim;
	// redistribucion del buffer
	log2N=1; while ((N>>log2N)>0) log2N++; log2N--;
	for (n1=0;n1<N;n1++) {
		for (n=n1,n2=0,m=log2N-1;m>=0;m--) if ((n>>m)==1) { n2+=(1<<(log2N-1-m)); n-=(1<<m); }
		if (n2<n1) continue;
		aux1=xre[n1]; xre[n1]=xre[n2]; xre[n2]=aux1; aux1=xim[n1]; xim[n1]=xim[n2]; xim[n2]=aux1;
	}
	// calculos
	M=2; M2=1;
	while (M<=N) {
		aux1=0.0; aux2=(2.0*PI)/(double)M;
		cosw=1.0; sinw=0.0; cosdw=cos(aux2); sindw=sin(aux2);
		for (k=0;k<M2;k++) {
			for (kk=0;kk<N;kk+=M) {
				n1=k+kk; n2=n1+M2;
				Are=xre[n1]; Aim=xim[n1]; Bre=xre[n2]*cosw-xim[n2]*sinw; Bim=xre[n2]*sinw+xim[n2]*cosw;
				xre[n1]=Are+Bre; xim[n1]=Aim+Bim; xre[n2]=Are-Bre; xim[n2]=Aim-Bim;
			}
			aux1=cosw*cosdw-sinw*sindw; aux2=cosw*sindw+sinw*cosdw; cosw=aux1; sinw=aux2;
		}
		M2=M; M=(M<<1);
	}
	return 0;
}

/********** Funciones de álgebra matricial **********/

int prodmat(double *A,double *b,double *c,unsigned int FA,unsigned int CA) {
	// producto de matriz por vector
	double *ak;
	unsigned int k,kk;
	for (k=0,ak=A;k<FA;k++,ak+=CA) for (c[k]=0.0,kk=0;kk<CA;kk++) c[k]+=ak[kk]*b[kk];
	return 0;
}

/********** Funciones de armónicos **********/

unsigned int hanoharms(double fs,double fmax,double f0) {
	// número de armónicos de una señal entre 0 y min{fmax,fs/2}
	if (f0>=fmax || f0>=0.5*fs) return 0;
	else if (fmax>0.5*fs) return (unsigned int)ceil(0.5*fs/f0)-1;
	else return (unsigned int)ceil(fmax/f0)-1;
}

int genharmonics(double *trama,unsigned int N12,unsigned int N23,double fs,double f0,unsigned int K,double *aa,double *pp,double alfa,char wins) {
	// genera un cacho de señal armonica y lo suma a lo que hubiera
	// ademas, si wins=1 enventana lo que hubiera antes, siendo ideal para cuando me dan un frame con ruido
	double fact,w,w0,cosdw0,sindw0,cosdw,sindw,cosw,sinw,aux1,aux2;
	unsigned int k,n,N13=N12+N23; //N12=n2-n1,N23=n3-n2,N13=n3-n1;
	if ((K==0)||(N13==0)) return 0; else if ((N12<0)||(N23<0)) return -1;
	w0=2*PI*f0/fs; cosdw0=cos(w0); sindw0=sin(w0); cosdw=cosdw0; sindw=sindw0;
	if (wins==1) {
		w=-w0*(double)N12+alfa; cosw=cos(w); sinw=sin(w); fact=1.0/sqrt(DCWINS*DCWINS+0.5);
		for (n=0;n<N13;n++) { trama[n]*=fact*(DCWINS-cosw); aux1=cosw*cosdw0-sinw*sindw0; aux2=cosw*sindw0+sinw*cosdw0; cosw=aux1; sinw=aux2; }		
	}
	for (k=0;k<K;k++) if (aa[k]!=0.0) {
		w=(double)(k+1)*(-w0*(double)N12+alfa)+pp[k]; cosw=cos(w); sinw=sin(w);
		for (n=0;n<N13;n++) { trama[n]+=aa[k]*cosw; aux1=cosw*cosdw-sinw*sindw; aux2=cosw*sindw+sinw*cosdw; cosw=aux1; sinw=aux2; }
		aux1=cosdw*cosdw0-sindw*sindw0; aux2=cosdw*sindw0+sindw*cosdw0; cosdw=aux1; sindw=aux2;
	}
	return 0;
}

int resamplelogampenv(double f01,double *logaa1,unsigned int K1,double f02,double *logaa2,unsigned int K2) {
	// remuestreo de envolvente en log-amplitud
	// nota: el intervalo k1 es el situado entre k1·f01 y (k1+1)·f01)
	unsigned int k1,k2;
	double fA,fB,aA,aB,fk2;
	if ((K1==0)||(K2==0)) return -1;
	k1=0; fA=0.0; fB=f01; aA=logaa1[0]; aB=aA;
	for (k2=0;k2<K2;k2++) {
		fk2=(double)(k2+1)*f02;
		while ((k1<=K1)&&(fB<fk2)) { k1++; fA=fB; fB+=f01; aA=aB; if (k1!=K1) aB=logaa1[k1]; }
		logaa2[k2]=aA+(aB-aA)*(fk2-fA)/f01;
	}
	return 0;
}

/********** Funciones de ruido **********/

int gennoisefromlogspectrum(double *X,unsigned int Lp2,double fs) {
	// genera trama de ruido a partir del log-espectro, poniendo fase aleatoria e invirtiendo la fft
	// ATENCION: X DEBE TRAER TAMAÑO 2·Lp2
	// ojo, en realidad basta que me pasen como entrada sólo las primeras Lp2/2 muestras (incluso sin la primera), pero que los buffers tengan tamaño para todas, claro
	double *Xbuff,ph,fact=2.0*PI/(double)RAND_MAX,scale=sqrt(fs/(double)Lp2);
	unsigned int n,Lp22=Lp2>>1;
	Xbuff=X+Lp2;
	X[0]=0.0; Xbuff[0]=0.0;
	X[Lp22]=0.0; Xbuff[Lp22]=0.0;
	for (n=1;n<Lp22;n++) {
		X[n]=scale*exp(X[n]);
		ph=fact*(double)rand(); Xbuff[n]=X[n]*sin(ph); X[n]*=cos(ph);
		X[Lp2-n]=X[n]; Xbuff[Lp2-n]=-Xbuff[n];
	}
	ifftr(Lp2,X,Xbuff);
	return 0;
}

/********** Funciones de windowing **********/

int olatriang(double *x,double *trama,unsigned int n1,unsigned int n2,unsigned int n3) {
	// le pongo la ventana triangular a la trama y la planto en x
	unsigned int n,N12=n2-n1,N13=n3-n1,N23=n3-n2;
	for (n=0;n<N12;n++) x[n1+n]+=trama[n]*(double)n/(double)N12;
	for (n=N12;n<N13;n++) x[n1+n]+=trama[n]*(double)(N13-n)/(double)N23;
	return 0;
}

unsigned int getwinlengthceilpot2(unsigned int L) {
	// devuelve el valor potencia de 2 inmediatamente superior o igual al argumento L
	unsigned int Lp2=1;
	while (Lp2<L) Lp2=Lp2<<1;
	return Lp2;
}

/********** Funciones de reconstrucción sobre CC **********/

double fwarp(double w,double alfa) {
	// warpea una determinada w segun alfa
	return atan2((1-alfa*alfa)*sin(w),(1+alfa*alfa)*cos(w)-2.0*alfa);
}

int ccmatrixcreate(unsigned int p,unsigned int K,double f0,double fs,double alfa,double *Hc,double *Hs) {
	// crea la matriz del regularized discrete cepstrum, en amplitud (cosenos) y en fase (senos)
	unsigned int k,n;
	double wk,coswk,sinwk,cosnwk,sinnwk,aux1,aux2,w0,*hc,*hs;
	if (Hc==NULL && Hs==NULL) return -1;
	w0=2.0*PI*f0/fs;
	hc=Hc; hs=Hs;
	for (k=0;k<K;k++) {
		wk=fwarp((double)(k+1)*w0,alfa);
		coswk=cos(wk); sinwk=sin(wk); cosnwk=1.0; sinnwk=0.0;
		hc[0]=cosnwk; if (Hs!=NULL) hs[0]=-sinnwk;
		for (n=1;n<=p;n++) {
			aux1=cosnwk*coswk-sinnwk*sinwk; aux2=cosnwk*sinwk+sinnwk*coswk; cosnwk=aux1; sinnwk=aux2;
			if (ADAPTLEVELS==1) { hc[n]=cosnwk; if (Hs!=NULL) hs[n]=-sinnwk; }
			else { hc[n]=2.0*cosnwk; if (Hs!=NULL) hs[n]=-2.0*sinnwk; }
		}
		hc+=p+1; if (Hs!=NULL) hs+=p+1;
	}	
	return 0;
}

int applyhpfilterlog(unsigned int K,double f0,double *logaa,double fv) {
	// filtrar con fmax variable (logaa[0] se asume que esta en f=f0)
	unsigned int k;
	double fk,at1=DB2EXP*ATT0HZ,at2=DB2EXP*ATT08FV,finterm=0.8*fv;
	for (k=0,fk=f0;(k<K)&&(fk<fv);k++,fk+=f0) {
		if (fk<finterm) logaa[k]+=at1+(at2-at1)*fk/finterm;
		else logaa[k]+=at2*(1.0-(fk-finterm)/(fv-finterm));
	}
	return 0;
}

int applyantihpfilter(unsigned int K,double f0,double *aa,double fv) {
	// filtrar con el complementario del filtro paso alto de fmax variable (aa[0] se asume que esta en f=f0)
	unsigned int k;
	double fk,at1=DB2EXP*ATT0HZ,at2=DB2EXP*ATT08FV,finterm=0.8*fv,h;
	for (k=0,fk=f0;(k<K)&&(fk<fv);k++,fk+=f0) {
		if (fk<finterm) h=exp(at1+(at2-at1)*fk/finterm);
		else h=exp(at2*(1.0-(fk-finterm)/(fv-finterm)));
		aa[k]*=sqrt(1.0-h*h);
	}
	return 0;
}

int cc2waveform(double *x,unsigned int Lx,double fs,unsigned int Lframe,unsigned int Nframes,double **f0s,double **fvs,unsigned int ord,double **CC,double alfa) {
	unsigned int k,kk,Kmax,K,Kuv,pm,Lp2;
	double *Huv,*Hc,*Hs,*aa,*pp,*ee,*cc,*trama,f0min,c0max,c0min,fv,fact,phlin,f0,f0ant;
	// inicializo la señal a ceros
	for (k=0;k<Lx;k++) x[k]=0.0;
	// miro el pitch minimo encontrado para determinar Kmax y reservar espacio pa la matriz de voiced
	for (f0min=DBL_MAX,k=0;k<Nframes;k++) if (f0s[k][0]>0.0 && f0s[k][0]<f0min) f0min=f0s[k][0]; f0min=exp(f0min); if (f0min<MINGENF0) f0min=MINGENF0;
	// calculo los valores maximo y minimo de c0, con los que mapearé la maximum voicing frequency si es caso
	if (fvs==NULL) for (k=0,c0min=DBL_MAX,c0max=-DBL_MAX;k<Nframes;k++) { cc=CC[k]; if (f0s[k][0]>0.0 && cc[0]>c0max) c0max=cc[0]; if (cc[0]<c0min) c0min=cc[0]; }
	// saco el maximo numero esperable de armonicos para hacer reserva de memoria
	Kmax=hanoharms(fs,fs,f0min); Kuv=hanoharms(fs,fs,F0UV);
	// matrices
	Hc=(double *)malloc(((Kmax<<1)+Kuv)*(ord+1)*sizeof(double)); Hs=Hc+Kmax*(ord+1); Huv=Hs+Kmax*(ord+1);
	ccmatrixcreate(ord,Kuv,F0UV,fs,alfa,Huv,NULL); // la estocastica la hago como muestrear a 100hz
	// reservo memoria para las cosillas que iré sacando
	aa=(double *)malloc(((Kmax<<1)+Kuv)*sizeof(double)); pp=aa+Kmax; ee=pp+Kmax;
	Lp2=getwinlengthceilpot2(Lframe<<1);
	trama=(double *)malloc((Lp2<<1)*sizeof(double));
	// empezamos a operar
	for (k=0,pm=Lframe,f0ant=0.0;k<Nframes;k++,pm+=Lframe) {
		// tomo el cc actual y la f0 actual y la limito si es caso
		cc=CC[k]; f0=exp(f0s[k][0]); if (f0<MINGENF0 || f0>MAXGENF0) f0=0.0;
		// reescalar c0 si es caso (los coefs restantes ya se reescalan implicitamente en ccmatrixcreate)
		if (ADAPTLEVELS==1) cc[0]-=5.0;
		// primero vamos con la parte del ruido, común a todos los frames, incluyendo factor ruido overlap-add
		cc[0]+=OLANOISE; prodmat(Huv,cc,ee,Kuv,ord+1); cc[0]-=OLANOISE;
		// filtrado paso alto a partir de fv si es voiced
		if (f0>0.0) {
			// determinar maximum voiced frequency
			if (fvs!=NULL) fv=fvs[k][0]; else fv=DEFFV*(cc[0]-c0min)/(c0max-c0min);
			// aplicarle el valor minimo porsiaca
			if (fv<MINGENFV) fv=MINGENFV;
			// aplicar el highpass (siempre que haya armonicos, claro)
			if (fv<=f0) f0=0.0; else applyhpfilterlog(Kuv,F0UV,ee,fv);
		}
		// remuestreo al tamaño de la fft de sintesis
		resamplelogampenv(F0UV,ee,Kuv,fs/(double)Lp2,trama+1,(Lp2>>1)-1);
		// generacion del trocito de ruido
		gennoisefromlogspectrum(trama,Lp2,fs);
		// luego ya los armónicos
		if (f0>0.0) {
			// numero de armonicos
			K=hanoharms(fs,fv,f0);
			// matriz de muestreo de envolvente
			ccmatrixcreate(ord,K,f0,fs,alfa,Hc,Hs);
			// amplitudes
			prodmat(Hc,cc,aa,K,ord+1);
			// desnormalizar por la f0
			for (fact=2.0*sqrt(f0),kk=0;kk<K;kk++) aa[kk]=fact*exp(aa[kk]);
			// antifiltro de ruido
			applyantihpfilter(K,f0,aa,fv);
			// fase mínima por muestreo de la envolvente complementaria
			prodmat(Hs,cc,pp,K,ord+1);
			// termino lineal de fase a partir de f0
			if (f0ant>0.0) phlin+=(f0+f0ant)*PI*(double)Lframe/fs; else phlin=0.0;
			// añado los armonicos al ruido ya generado
			genharmonics(trama,Lframe,Lframe,fs,f0,K,aa,pp,phlin,1);
		}
		// overlap-add de la trama final
		olatriang(x,trama,pm-Lframe,pm,pm+Lframe);
		// reescalo inversamente
		if (ADAPTLEVELS==1) cc[0]+=5.0;
		// acumulo f0 pa la siguiente
		f0ant=f0;
	}
	// libero memoria (recuerda que había una sola reserva para varias matrices y vectores)
	free(Hc); free(aa); free(trama);
	// normalizar si es caso
	if (AMPNORMALIZE==1) wavampnormalize(Lx,x);
	// ya ta
	return 0;
}

/********** Funciones visibles desde fuera **********/

int gen_ahocoder_waveform(short *s,unsigned int Ls,unsigned int sr,unsigned int Lframe,unsigned int Nframes,double **lf0s,double **fv,unsigned int ord,double alfa,double **CC) {
	// descarte de casos patológicos
	if (s==NULL || Ls==0 || Lframe==0 || Nframes==0 || lf0s==NULL || CC==NULL) return -1;	
	// llamo a la funcion de generacion convirtiendo las entradas
	cc2waveform((double *)s,Ls,(double)sr,Lframe,Nframes,lf0s,fv,ord,CC,alfa);
	// sobreescribo convirtiendo los doubles en shorts como procede
	wavdouble2short(Ls,(double *)s,s);
	// listo
	return 0;
}

unsigned int get_ahocoder_waveform_length(unsigned int Lframe,unsigned int Nframes) {
	// devuelve el tamaño que ha de tener la señal x si viene caracterizada por [Nframes] vectores sacados cada [Lframe] muestras
	return (Nframes+1)*Lframe+1;
}
