/******************************************************************************/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/

AhoTTS Multilingual: A Text-To-Speech system for Basque*, Spanish*, Galician',
Catalan^ and English^^, developed by Aholab Signal Processing Laboratory at the
University of the Basque Country (UPV/EHU). Its acoustic engine is based on
hts_engine** and it uses AhoCoder'' as vocoder.
(Read COPYRIGHT_and_LICENSE_code.txt for more details)
--------------------------------------------------------------------------------

*Linguistic processing for Basque and Spanish, Vocoder (Ahocoder) and
integration by Aholab UPV/EHU.

''AhoCoder is an HNM-based vocoder for Statistical Synthesizers
http://aholab.ehu.es/ahocoder/

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Copyrights:
	*1997-2015  Aholab Signal Processing Laboratory, University of the Basque
	 Country (UPV/EHU)
    	''2011-2015 Aholab Signal Processing Laboratory, University of the Basque
	  Country (UPV/EHU)

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Licenses:
	*GPL-3.0+
	''Modified BSD (Compatible with GNU GPL)

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
/*****************************************************************************/
/*                                                                           */
/*                                \m/(-.-)\m/                                */
/*                                                                           */
/*****************************************************************************/
/*
Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
1.1.0	 03/05/12  Agustin    Implementación del tts64 version 1.2.0, KVStrList
1.0.0  	 20/01/12  Agustin	  Codificación inicial
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Socket_Cliente.hpp"
#include "strl.hpp"
#include "string.hpp"

//#define SERVER "localhost"
//#define SERVICE "ahotts"
/*
void usage (void);
*/
int main (int argc, char* argv[])
{
	
	KVStrList pro("InputFile=input.txt Lang=eu OutputFile=output.wav Speed=100 IP=NULL Port=0 SetDur=n");
	StrList files;

	clargs2props(argc, argv, pro, files,
			"InputFile=s Lang={es|eu} OutputFile=s Speed=s IP=s Port=i SetDur=b");
	
	
	const char *lang = pro.val("Lang");
	const char *inputfile = pro.val("InputFile");
	const char *outputfile = pro.val("OutputFile");
	const char *speed = pro.val("Speed");
	//const char *gender = pro.val("Gender");
	const char *ip=pro.val("IP");
	const int puerto=pro.ival("Port");
	bool setdur=pro.bbval("SetDur");

	if (!strcmp(ip,"NULL")){
		fprintf(stderr,"IP direction is mandatory\n");
		exit (-1);
	}
	if(puerto<1024 || puerto>65535){
		fprintf(stderr,"The port must be between 1024 and 65535 (WellKnown ports are forbidden)\n");
		exit (-1);
	}
	
	//Sin argumentos se devuelve como usarlo
	
	//Objeto cliente
	Options op;
	strcpy(op.language,lang);
	strcpy(op.speed,speed);
	op.setdur=setdur;
	//strcpy(op.gender,gender);
	
	ClientConnection *cliente = new ClientConnection (op);
	
	//if(!strcmp(argv[2],"cat")||!strcmp(argv[2],"gl")||!strcmp(argv[2],"es")||!strcmp(argv[2],"eu")){
	//	strcpy(lang,argv[2]);}
	//else{
	//	printf("Error, lengua no soportada\n");
	//	usage();return -1;}
	int aux;
	aux=cliente->OpenInetConnection(ip,puerto);
	if(aux==-1)
	{
		fprintf(stderr,"Unable to establish server connection\n");
		exit(-1);
	}

	FILE *fp=NULL;
	fp=fopen(inputfile,"r");
	if(fp!=NULL){
		fclose(fp);
	}else{
		fprintf(stderr,"Please check that the %s file exists\n", inputfile);
		exit(-1);
	}


	//printf("El archivo a sintetizar es: %s, por el descriptor: %d\n",inputfile,cliente->ObtainSSocket());
	//cliente->SendLanguage(lang);
	cliente->SendOptions();
	fprintf(stderr,"Sending file to synthesize\n");
	cliente->SendFile(inputfile,cliente->ObtainSSocket());
	
	
	//FILE* fd;
	//fd=fopen("out.wav","wb");
	//if(fd!=NULL){
	fprintf(stderr,"Receiving synthesized file\n");
	cliente->ReceiveFile(outputfile,cliente->ObtainSSocket());
	//	}
	//fclose(fd);
	
	cliente->CloseConnection();
	delete (cliente);
	return 0;
}
/*
void usage (void)
{
	printf("Uso: ./Cliente \"Archivo a sintetizar\" \"lengua\"\n");
	printf("Texto a enviar: máximo de 9999 caracteres\n");
	printf("Lengua: gl, cat, es, eu\n");
}
*/
