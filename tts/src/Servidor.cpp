/******************************************************************************/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/

AhoTTS: A Text-To-Speech system for Basque* and Spanish*,
developed by Aholab Signal Processing Laboratory at the
University of the Basque Country (UPV/EHU). Its acoustic engine is based on
hts_engine' and it uses AhoCoder* as vocoder.
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
1.1.0	 03/05/12  Agustin    Implementación del tts64 version 1.2.0
1.0.0  	 20/01/12  Agustin	  Codificación inicial
*/
#include <Socket_Servidor.hpp>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

#include "htts.hpp"
#include "strl.hpp"
#include "caudio.hpp"
//#define SERVICE "ahotts"

int main (int argc, char* argv[])
{

	KVStrList pro("IP=NULL Port=0 DataPath=data_tts");
	StrList files;

	clargs2props(argc, argv, pro, files, "IP=s Port=i");

	const int puerto=pro.ival("Port");
	const char* ip=pro.val("IP");
	const char* data_path=pro.val("DataPath");

	if (!strcmp(ip,"NULL")){
		fprintf(stderr,"IP direction is mandatory\n");
		exit (-1);
	}
	if(puerto<1024 || puerto>65535){
		fprintf(stderr,"The port must be between 1024 and 65535 (WellKnown ports are forbidden)\n");
		exit (-1);
	}

	ServerConnection *servidor = new ServerConnection;
	/*
	* Se abre el socket servidor, con el servicio "cpp_java" dado de
	* alta en /etc/services.
	*/
	if(servidor->OpenInetConnection(ip,puerto)==-1)
	{
		fprintf (stderr,"Unable to open server socket\n");
		exit (-1);
	}
	printf("Service open\n");
	/*
	* Se espera un cliente que quiera conectarse
	*/
	while(1){
		int pid; //identificador del proceso para el fork
		if(servidor->AcceptClientConnection()==-1)
		{
			fprintf (stderr,"Unable to open client socket\n");
			exit (-1);
		}
		//printf("Conexión aceptada\n");
		pid=fork();
		if(pid<0){
			fprintf(stderr,"FORK error\n");
			exit(-1);
		}
		system("mkdir -p txt");
		system("mkdir -p wav");
		if(pid==0){
			//printf("Proceso hijo\n");
			//proceso hijo
				fprintf(stderr,"Attending request\n");
				time_t tiempo;
				time(&tiempo);
				char *fecha=ctime(&tiempo);

				char archivotxt[128]="txt/";
				char archivowav[128]="wav/";
				strncat(archivotxt,fecha,24);strcat(archivotxt,".txt");
				strncat(archivowav,fecha,24);strcat(archivowav,".wav");

				//servidor->ReadLanguage();
				servidor->ReadOptions();
				//fprintf(stderr,"Guardando el archivo recibido en: %s",archivotxt);
				servidor->ReceiveFile(archivotxt,servidor->ObtainCSocket());

				//servidor->Show();

				char *out=NULL;
				HTTS *tts = new HTTS;
				char* lang=servidor->ObtainLanguage();
//				char* data_path=servidor->ObtainDataPath();
				char tmp_string_dic[1024];

				if(!strcmp("eu",lang)||!strcmp("cat",lang)||!strcmp("gl",lang)||!strcmp("en",lang)){
					tts->set("Lang", "eu");
					sprintf(tmp_string_dic, "%s/dicts/eu_dicc", data_path);
					tts->set("HDicDBName",tmp_string_dic);}
				else if(!strcmp("es",lang)){
					tts->set("Lang", "es");
					sprintf(tmp_string_dic, "%s/dicts/es_dicc", data_path);
					tts->set("HDicDBName",tmp_string_dic);}
				tts->set("PthModel", "Pth1");
				tts->set("Method", "HTS");
				if (!tts->create()) {
					delete tts;
					return 0;
				}
				char tmp_string_voice[1024];
				//char *gender=servidor->ObtainGender();
				if(!strcmp("eu",lang)){
					//if(!strcmp(gender,"F"))
						sprintf(tmp_string_voice, "%s/voices/aholab_eu_female/", data_path);
					//else
					//	sprintf(tmp_string_voice, "%s/voices/aholab_eu_male/", data_path);
				}
				else if (!strcmp("es",lang)){
					//if(!strcmp(gender,"F"))
						sprintf(tmp_string_voice, "%s/voices/aholab_es_female/", data_path);	
					//else
					//	sprintf(tmp_string_voice, "%s/voices/aholab_es_male/", data_path);
				}
				
				tts->set("voice_path", tmp_string_voice);

				bool setdur=servidor->ObtainSetDur();
				char* speed=servidor->ObtainSpeed();
				if(setdur)
					tts->set("vp","yes");
				else if(strcmp(speed,"100")&&speed!=NULL){
					int f;
					sscanf(speed,"%d",&f);
					if(f>=SPEED_MIN && f<=SPEED_MAX){
						char *tmp_speed = new char [5];
						sprintf(tmp_speed, "%.2f", f/100.0);
							tts->set("r",tmp_speed);
						delete []tmp_speed;
					}else{fprintf(stderr,"WARNING: parametro -Speed=%d ignorado, valor entero entre %d y %d\n",f,SPEED_MIN,SPEED_MAX);}

				}


				//Cambia el nombre de salida del archivo de audio a algo controlable (ej:fecha y hora más número aleatorio)
			//	tts->set("ow",archivowav);

				//fprintf(stderr,"Sintetizando...");
				char *str;
				//abrir fichero wav de salida
				CAudioFile fout;
				fout.open(archivowav,"w", "SRate=16000.0 NChan=1 FFormat=Wav");
				
					FILE *fp=NULL;
					fp=fopen(archivotxt,"r");
					if(fp!=NULL)
					{
						int tamanio=0;
						fseek(fp,0,SEEK_END);
						tamanio=ftell(fp);
						rewind(fp);
						str=new char [tamanio];
						fread(str,1,tamanio,fp);
						fclose(fp);
					}else{
						fprintf(stderr,"Problem with text file\n");
						exit(-1);
					}
					if(tts->input_multilingual(str, lang, data_path, FALSE)){
						short *samples;
						int len=0;
						while((len = tts->output_multilingual(lang, &samples)) != 0){
							fout.setBlk(samples, len);
							free(samples);
						}
					}
				
				fout.close();

				//tts->trans(str, &out, lang,1);
				//fprintf(stderr,"... sintetizado\n");
				//fprintf(stderr,"trans= %s\n", out);
				//free(out);
				delete []str;
				//Hay que liberar el espacio del objeto tts ahora porque si no el tamanio del archivo .wav
				// se calcula erroneamente
				delete tts;

				//FILE* fd;
				//fd=fopen("out.wav","rb");
				//if(fd!=NULL){
				servidor->SendFile(archivowav,servidor->ObtainCSocket());
				//}
				//fclose(fd);
				/*El  hijo cierra el descriptor de cliente y servidor*/
				servidor->CloseClientConnection();
				servidor->CloseConnection();
				fprintf(stderr,"Request finished\n");
				exit(0);
			}else{
				//fprintf(stderr,"Proceso padre ");
				//proceso padre
				//El proceso padre cierra el descriptor de cliente
				servidor->CloseClientConnection();
				signal(SIGCHLD, SIG_IGN);
				//fprintf(stderr,"Cerrada la conexion\n");
			}

	}

	servidor->CloseConnection();
	delete (servidor);

	return 0;
}

