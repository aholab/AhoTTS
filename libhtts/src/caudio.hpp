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
#ifndef __CAUDIO_HPP__
#define __CAUDIO_HPP__

/**********************************************************/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/*
Copyright: 1996 - Grupo de Voz (DAET) ETSII/IT-Bilbao

Nombre fuente................ -
Nombre paquete............... -
Lenguaje fuente.............. C++ (Borland C/C++ 3.1)
Estado....................... desarrollo
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Borja Etxebarria

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
1.0.1    09/04/99  Borja     uso tipos UINT
1.0.0    15/04/97  Borja     infinitos cambios
0.0.0    15/07/96  Borja     codificacion inicial.

======================== Contenido ========================
Cosas a enviar en options:

SRate=xxx
NChan=xxx
FFormat=xxx
SampType
BigEndian=n/y
TxtRule
TxtName
MarkTrack

===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include "mark.hpp"
#include "strl.hpp"
#include "samp.hpp"

/**********************************************************/

#ifndef __cplusplus
#error Must use C++ compiler
#endif

/**********************************************************/

#define CAUDIO_FFORMAT  "FFormat"
#define CAUDIO_TXHRULE  "TxhRule"
#define CAUDIO_TXHNAME  "TxhName"
#define CAUDIO_FNAME "FName"

#define CAUDIO_SAMPTYPE "SampType"
#define CAUDIO_SRATE "SRate"
#define CAUDIO_NCHAN "NChan"
#define CAUDIO_NSAMPLES "NSamples"
#define CAUDIO_BIGENDIAN "BigEndian"

#define CAUDIO_ENVOPTS "CAUDIOOPTS"
#define CAUDIO_OPTS_DEFAULT "FFormat=AutoRaw SampType=PCM16 NChan=1 SRate=8000"

#ifdef __BIG_ENDIAN__
#define CAUDIO_BIGENDIAN_DEFAULT TRUE
#else
#define CAUDIO_BIGENDIAN_DEFAULT FALSE
#endif

#define CAUDIO_TXTCOMMENT "TxtComment"
#define CAUDIO_TXTCOLUMNS "TxtColumns"

/**********************************************************/

#define INFO_SPEAKER   "Speaker"
#define INFO_TEXT      "Text"
#define INFO_VERSION   "Version"
#define INFO_COMMENTS  "Comments"

#define INFO_SAMPLES   "Samples"

#define CAUDIO_PARAMSET "ParamSet"
#define CAUDIO_FRAMERATEMS "FrameRateMs"
#define CAUDIO_ORIGINMS "OriginMs"

#define CAUDIO_DIMMIN "DimMin"
#define CAUDIO_DIMMAX "DimMax"
#define CAUDIO_DIMUNITS "DimUnits"

#define CAUDIO_VALMIN "ValMin"
#define CAUDIO_VALMAX "ValMax"
#define CAUDIO_VALUNITS "ValUnits"

/**********************************************************/

class CAudio {
	private:
	public:
		CAudio ( VOID ) {};
		virtual ~CAudio ( VOID ) {};
		virtual BOOL OK( VOID ) const  = 0;

		virtual LONG getNSamples( VOID ) const = 0;
		virtual DOUBLE getSRate( VOID ) const = 0;
		virtual INT getSampType( VOID ) const = 0;
		virtual UINT getNChan( VOID ) const = 0;
		virtual BOOL getBigEndian( VOID ) const = 0;
		virtual const char *getSampType_a( VOID ) const = 0;
		virtual LONG getPos( VOID ) const = 0;
		virtual VOID setPos( LONG pos ) = 0;

		virtual UINT get( INT16& sample ) = 0;
		virtual UINT set( INT16 sample ) = 0;
		virtual UINT ins( INT16 sample ) = 0;
		virtual UINT get( DOUBLE& sample ) = 0;
		virtual UINT set( DOUBLE sample ) = 0;
		virtual UINT ins( DOUBLE sample ) = 0;
		virtual UINT del( VOID ) = 0;
		virtual UINT getBlk( INT16* block, UINT len ) = 0;
		virtual UINT setBlk( const INT16* block, UINT len ) = 0;
		virtual UINT insBlk( const INT16* block, UINT len ) = 0;
		virtual UINT getBlk( DOUBLE* block, UINT len ) = 0;
		virtual UINT setBlk( const DOUBLE* block, UINT len ) = 0;
		virtual UINT insBlk( const DOUBLE* block, UINT len ) = 0;
		virtual UINT delBlk( UINT len ) = 0;

		virtual INT16* get_i16( UINT len, BOOL reloadall=FALSE ) = 0;
		virtual DOUBLE* get_d( UINT len, BOOL reloadall=FALSE ) = 0;

		virtual VOID minmax( LONG len, DOUBLE *min, DOUBLE *max );
		virtual VOID minmax( LONG len, INT16 *min, INT16 *max );
		virtual LONG maxfr( UINT framelen, UINT winlen, LONG nframes=-1 );
};

/**********************************************************/

class AFFormat;

class CAudioFile : public CAudio {
	protected:
		KVStrList myOpts;

		KVStrList myInfo;
		Mark1DList myMarks;
		Samp mySamp;
		DOUBLE mySRate;
		INT mySampType;
		UINT myNChan;
    BOOL myBigEndian;

		AFFormat * afFormat;  // modulo dependiente del formato de fichero
		FILE* myFile;
		BOOL trackMarks;

		BOOL updatable;  // TRUE si se puede actualizar la cabecera
		BOOL txhUpdatable;  // TRUE si el fichero TxtHdr es actualizable

		AFFormat *getAFFormatObject( const CHAR *fname,const CHAR *mode );
		VOID txhGetName( const CHAR *fName );
		VOID txhLoad( const CHAR *mode );
		VOID txhWrite( VOID );
	public:
		CAudioFile( VOID );
		~CAudioFile( VOID ) { close(); };
		BOOL OK( VOID ) const ;
		const String& getOpt( const String &option ) const { return myOpts.val(option); };

		const char* getFFormat( VOID ) const { return getOpt(CAUDIO_FFORMAT); };
		const char* getFName( VOID ) const { return getOpt(CAUDIO_FNAME); };

		VOID open( const CHAR* fName, const CHAR* mode, const KVStrList &Opts);
		VOID open( const CHAR* fName, const CHAR* mode="r", const CHAR* options="", ... );
		VOID open( const CHAR* fName, const CAudioFile &fa, const CHAR* options="", ... );
		VOID open( const CHAR* fName, const CHAR* mode, const CHAR* options, va_list va );
		VOID open( const CHAR* fName, const CAudioFile &fa, const CHAR* options, va_list va );

		VOID open( const CHAR* fName, const CAudioFile &fa, const KVStrList &Opts );
		BOOL openFromTxt( const CHAR *fname, FILE *ftxt, const KVStrList &Opts );
		BOOL openFromTxt( const CHAR *fname, FILE *ftxt, const CHAR* options="", ... );
		BOOL openFromTxt( const CHAR *fname, FILE *ftxt, const CHAR* options, va_list va );

		VOID set_updatable( BOOL update );
		BOOL update( VOID );
		VOID close( VOID );

		KVStrList& info( VOID ) { return myInfo; }
		Mark1DList& marks( VOID ) { return myMarks; }

		DOUBLE getSRate( VOID ) const { return mySRate; }
		BOOL getBigEndian( VOID ) const { return myBigEndian; }
		VOID setSRate( DOUBLE srate, BOOL adjustMarks=FALSE );
		VOID setBigEndian( BOOL big ) { myBigEndian=big; mySamp.setendian(big?-1:1); }
		INT getSampType( VOID ) const { return mySampType; }
		UINT getNChan( VOID ) const { return myNChan; }
		const char *getSampType_a( VOID ) const { return SampType_i2a(mySampType); }

		VOID setPos( LONG pos ) { mySamp.setPos(pos); }
		LONG getPos( VOID ) const { return mySamp.getPos(); }
		LONG getNSamples( VOID ) const { return mySamp.getN(); }
		VOID setPosNoCh( LONG pos ) { mySamp.setPosNoCh(pos); }
		LONG getPosNoCh( VOID ) const { return mySamp.getPosNoCh(); }
		LONG getNSamplesNoCh( VOID ) const { return mySamp.getNNoCh(); }

		UINT get( INT16& mySample ) { return mySamp.get(mySample); }
		UINT set( INT16 mySample ) { return mySamp.set(mySample); }
		UINT ins( INT16 mySample ) { if (trackMarks) myMarks.adjust_ins(getPos()); return mySamp.ins(mySample); }
		UINT get( DOUBLE& mySample ) { return mySamp.get(mySample); }
		UINT set( DOUBLE mySample ) { return mySamp.set(mySample); }
		UINT ins( DOUBLE mySample ) {  if (trackMarks) myMarks.adjust_ins(getPos()); return mySamp.ins(mySample); }
		UINT del( VOID ) { if (trackMarks) myMarks.adjust_del(getPos()); return mySamp.del(); }
		UINT getBlk( INT16* block, UINT len ) { return mySamp.getBlk(block,len); }
		UINT setBlk( const INT16* block, UINT len ) { return mySamp.setBlk(block,len); }
		UINT insBlk( const INT16* block, UINT len ) { if (trackMarks) myMarks.adjust_ins(getPos(),len); return mySamp.insBlk(block,len); }
		UINT getBlk( DOUBLE* block, UINT len ) { return mySamp.getBlk(block,len); }
		UINT setBlk( const DOUBLE* block, UINT len ) { return mySamp.setBlk(block,len); }
		UINT insBlk( const DOUBLE* block, UINT len ) { if (trackMarks) myMarks.adjust_ins(getPos(),len); return mySamp.insBlk(block,len); }
		UINT delBlk( UINT len ) { if (trackMarks) myMarks.adjust_del(getPos(),len); return mySamp.delBlk(len); }

		UINT getCh( INT16& mySample, UINT ch ) { return mySamp.getCh(mySample,ch); }
		UINT setCh( INT16 mySample, UINT ch ) { return mySamp.setCh(mySample,ch); }
		UINT insCh( INT16 mySample, UINT ch ) { if (trackMarks) myMarks.adjust_ins(getPos()); return mySamp.insCh(mySample,ch); }
		UINT getCh( DOUBLE& mySample, UINT ch ) { return mySamp.getCh(mySample,ch); }
		UINT setCh( DOUBLE mySample, UINT ch ) { return mySamp.setCh(mySample,ch); }
		UINT insCh( DOUBLE mySample, UINT ch ) {  if (trackMarks) myMarks.adjust_ins(getPos()); return mySamp.insCh(mySample,ch); }
		UINT delCh( UINT ch ) { if (trackMarks) myMarks.adjust_del(getPos()); return mySamp.delCh(ch); }
		UINT getBlkCh( INT16* block, UINT len, UINT ch ) { return mySamp.getBlkCh(block,len,ch); }
		UINT setBlkCh( const INT16* block, UINT len, UINT ch ) { return mySamp.setBlkCh(block,len,ch); }
		UINT insBlkCh( const INT16* block, UINT len, UINT ch ) { if (trackMarks) myMarks.adjust_ins(getPos(),len); return mySamp.insBlkCh(block,len,ch); }
		UINT getBlkCh( DOUBLE* block, UINT len, UINT ch ) { return mySamp.getBlkCh(block,len,ch); }
		UINT setBlkCh( const DOUBLE* block, UINT len, UINT ch ) { return mySamp.setBlkCh(block,len,ch); }
		UINT insBlkCh( const DOUBLE* block, UINT len, UINT ch ) { if (trackMarks) myMarks.adjust_ins(getPos(),len); return mySamp.insBlkCh(block,len,ch); }
		UINT delBlkCh( UINT len, UINT ch ) { if (trackMarks) myMarks.adjust_del(getPos(),len); return mySamp.delBlkCh(len,ch); }

		UINT getChAll( INT16* mySamplev ) { return mySamp.getChAll(mySamplev); }
		UINT setChAll( const INT16* mySamplev ) { return mySamp.setChAll(mySamplev); }
		UINT insChAll( const INT16* mySamplev ) { if (trackMarks) myMarks.adjust_ins(getPos()); return mySamp.insChAll(mySamplev); }
		UINT getChAll( DOUBLE* mySamplev ) { return mySamp.getChAll(mySamplev); }
		UINT setChAll( const DOUBLE* mySamplev ) { return mySamp.setChAll(mySamplev); }
		UINT insChAll( const DOUBLE* mySamplev ) {  if (trackMarks) myMarks.adjust_ins(getPos()); return mySamp.insChAll(mySamplev); }
		UINT delChAll( VOID ) { if (trackMarks) myMarks.adjust_del(getPos()); return mySamp.delChAll(); }
		UINT getBlkChAll( INT16* block, UINT len ) { return mySamp.getBlkChAll(block,len); }
		UINT setBlkChAll( const INT16* block, UINT len ) { return mySamp.setBlkChAll(block,len); }
		UINT insBlkChAll( const INT16* block, UINT len ) { if (trackMarks) myMarks.adjust_ins(getPos(),len); return mySamp.insBlkChAll(block,len); }
		UINT getBlkChAll( DOUBLE* block, UINT len ) { return mySamp.getBlkChAll(block,len); }
		UINT setBlkChAll( const DOUBLE* block, UINT len ) { return mySamp.setBlkChAll(block,len); }
		UINT insBlkChAll( const DOUBLE* block, UINT len ) { if (trackMarks) myMarks.adjust_ins(getPos(),len); return mySamp.insBlkChAll(block,len); }
		UINT delBlkChAll( UINT len ) { if (trackMarks) myMarks.adjust_del(getPos(),len); return mySamp.delBlkChAll(len); }

		UINT getChRange( INT16* mySample, UINT ch1, UINT ch2 ) { return mySamp.getChRange(mySample,ch1,ch2); }
		UINT setChRange( const INT16* mySample, UINT ch1, UINT ch2 ) { return mySamp.setChRange(mySample,ch1,ch2); }
		UINT insChRange( const INT16* mySample, UINT ch1, UINT ch2 ) { if (trackMarks) myMarks.adjust_ins(getPos()); return mySamp.insChRange(mySample,ch1,ch2); }
		UINT getChRange( DOUBLE* mySample, UINT ch1, UINT ch2 ) { return mySamp.getChRange(mySample,ch1,ch2); }
		UINT setChRange( const DOUBLE* mySample, UINT ch1, UINT ch2 ) { return mySamp.setChRange(mySample,ch1,ch2); }
		UINT insChRange( const DOUBLE* mySample, UINT ch1, UINT ch2 ) {  if (trackMarks) myMarks.adjust_ins(getPos()); return mySamp.insChRange(mySample,ch1,ch2); }
		UINT delChRange( UINT ch1, UINT ch2 ) { if (trackMarks) myMarks.adjust_del(getPos()); return mySamp.delChRange(ch1,ch2); }
		UINT getBlkChRange( INT16* block, UINT len, UINT ch1, UINT ch2 ) { return mySamp.getBlkChRange(block,len,ch1,ch2); }
		UINT setBlkChRange( const INT16* block, UINT len, UINT ch1, UINT ch2 ) { return mySamp.setBlkChRange(block,len,ch1,ch2); }
		UINT insBlkChRange( const INT16* block, UINT len, UINT ch1, UINT ch2 ) { if (trackMarks) myMarks.adjust_ins(getPos(),len); return mySamp.insBlkChRange(block,len,ch1,ch2); }
		UINT getBlkChRange( DOUBLE* block, UINT len, UINT ch1, UINT ch2 ) { return mySamp.getBlkChRange(block,len,ch1,ch2); }
		UINT setBlkChRange( const DOUBLE* block, UINT len, UINT ch1, UINT ch2 ) { return mySamp.setBlkChRange(block,len,ch1,ch2); }
		UINT insBlkChRange( const DOUBLE* block, UINT len, UINT ch1, UINT ch2 ) { if (trackMarks) myMarks.adjust_ins(getPos(),len); return mySamp.insBlkChRange(block,len,ch1,ch2); }
		UINT delBlkChRange( UINT len, UINT ch1, UINT ch2 ) { if (trackMarks) myMarks.adjust_del(getPos(),len); return mySamp.delBlkChRange(len,ch1,ch2); }

		UINT getNoCh( INT16& mySample ) { return mySamp.getNoCh(mySample); }
		UINT setNoCh( INT16 mySample ) { return mySamp.setNoCh(mySample); }
		UINT insNoCh( INT16 mySample ) { if (trackMarks) myMarks.adjust_ins(getPos()); return mySamp.insNoCh(mySample); }
		UINT getNoCh( DOUBLE& mySample ) { return mySamp.getNoCh(mySample); }
		UINT setNoCh( DOUBLE mySample ) { return mySamp.setNoCh(mySample); }
		UINT insNoCh( DOUBLE mySample ) {  if (trackMarks) myMarks.adjust_ins(getPos()); return mySamp.insNoCh(mySample); }
		UINT delNoCh( VOID ) { if (trackMarks) myMarks.adjust_del(getPos()); return mySamp.delNoCh(); }
		UINT getBlkNoCh( INT16* block, UINT len ) { return mySamp.getBlkNoCh(block,len); }
		UINT setBlkNoCh( const INT16* block, UINT len ) { return mySamp.setBlkNoCh(block,len); }
		UINT insBlkNoCh( const INT16* block, UINT len ) { if (trackMarks) myMarks.adjust_ins(getPos(),len); return mySamp.insBlkNoCh(block,len); }
		UINT getBlkNoCh( DOUBLE* block, UINT len ) { return mySamp.getBlkNoCh(block,len); }
		UINT setBlkNoCh( const DOUBLE* block, UINT len ) { return mySamp.setBlkNoCh(block,len); }
		UINT insBlkNoCh( const DOUBLE* block, UINT len ) { if (trackMarks) myMarks.adjust_ins(getPos(),len); return mySamp.insBlkNoCh(block,len); }
		UINT delBlkNoCh( UINT len ) { if (trackMarks) myMarks.adjust_del(getPos(),len); return mySamp.delBlkNoCh(len); }

		INT16* get_i16( UINT len, BOOL reloadall=FALSE ) { return mySamp.get_i16(len,reloadall); }
		DOUBLE* get_d( UINT len, BOOL reloadall=FALSE ) { return mySamp.get_d(len,reloadall); }
		INT16* getChAll_i16( UINT len, BOOL reloadall=FALSE ) { return mySamp.getChAll_i16(len,reloadall); }
		DOUBLE* getChAll_d( UINT len, BOOL reloadall=FALSE ) { return mySamp.getChAll_d(len,reloadall); }

		VOID txtWriteAllInfo( FILE *f, BOOL force_nsamp=TRUE );
		VOID writeToTxt( FILE *f );

		VOID sndCopy( CAudioFile &fromf, LONG n );
};

/**********************************************************/


/**********************************************************/

#endif
