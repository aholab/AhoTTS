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
#include <Socket_Cliente.hpp>

/*
/ Conecta con un servidor remoto a traves de socket INET
*/
int ClientConnection::OpenInetConnection(const char *IPServidor, const int PuertoServicio)
{
	struct sockaddr_in Direccion;
	//struct servent *Puerto;
	//struct hostent *Host;

	//printf("Inicio de conexion INET\n");
	
	//Puerto = getservbyname (Servicio, "tcp");
	//if (Puerto == NULL)
	//	return -1;

	//Host = gethostbyname (Host_Servidor);
	//if (Host == NULL)
	//	return -1;
	
	
	Direccion.sin_family = AF_INET;
	Direccion.sin_addr.s_addr = inet_addr(IPServidor);
	Direccion.sin_port = htons(PuertoServicio);
	
	//printf("apertura de socket, contra la direccion %s == %d\n",IPServidor,inet_addr(IPServidor));
	socket_server = socket (AF_INET, SOCK_STREAM, 0);

	if (socket_server == -1)
		return -1;
	//printf("conexion del socket al server\n");
	if (connect(socket_server,(struct sockaddr *)&Direccion,sizeof (Direccion)) == -1)
	{
		return -1;
	}
	//printf("Conexion INET establecida, descriptor %d\n",socket_server);
	
	return 0;
}

/*Envia una utt, primero calcula la longitud (que no puede ser mayor de
 * 9999 caracteres, y la envía en una estructura predefinida 
 * para que el servidor sepa cuantos bytes tiene que leer*/
 /*
int ClientConnection::SendUtt(const char* str )
{
	int longitud=0;
	int result=0,aux;
	LongUtt longcadena;
	
	longitud=strlen(str)+1;
	//itoa(longitud,longcadena.longitud,10);
	
	snprintf(longcadena.longitud,sizeof(LongUtt),"%d",longitud);
	//Falta comprobar que str no sea NULL
	
	//Se manda la longitud de la cadena
	if(write(socket_server,&longcadena.longitud,sizeof(LongUtt))<0){
		printf("Error al enviar la longitud de la utt\n");
		return -1;
		}else
		{printf("Enviada la longitud\n");}
		
	while(result<longitud){
		aux=write(socket_server,str+result,longitud-result);
		if(aux>0){//Se ha conseguido escribir algo
			result+=aux;
		}else{
			if(aux==0){//Se ha acabado de escribir
			return result;
			}else{
				printf("Error\n");
				return -1;
			}
		}
	}
	return 0;
}
*/

/*Envia por el socket la lengua en la que se quiere sintetizar la 
 * utterance mandada previamente
 * DEPRECATED ahora es send options*/
 /*
int ClientConnection::SendLanguage(const char *language)
{
	fprintf(stderr,"Enviando la lenga: %s\n",language);
	int aux=0;
	Language lang;
	strcpy(lang.language,language);
	aux=write(socket_server, &lang.language, sizeof(Language));
	if(aux== -1){
		printf("Error al mandar la lengua\n");
	}

	return aux;
}
*/
int ClientConnection::SendOptions()
{
	//fprintf(stderr,"Mandando las opciones");
	int aux=0;
	aux=write(socket_server,&opciones,sizeof(Options));
		if(aux== -1){
		printf("Error al mandar las opciones\n");
	}

	return aux;
}

/*
int ClientConnection::ReadFile(FILE* fd)
{
	printf("Recibiendo archivo\n");
	
	int longitud=1024;
	int leido=0,aux=0,tamanio=0;
	char *buff=new char[longitud];
	
	//Obtener el tamaño del archivo de audio
	SizeFile file;
	aux=read(socket_server,file.size,sizeof(SizeFile));
	if(aux==-1){
		printf("Error al recibir el tamanio del archivo de audio\n");
		return -1;
	}else{
		printf("Recibido el tamanio del archivo de audio ");
	}
	tamanio=str2num(file.size);
	printf("%d\n",tamanio);
	int i=0;
	//Como saber hasta donde leer
	while(tamanio>0){
		if(tamanio<longitud){longitud=tamanio+1;}
		
		leido=0;
		while (leido < longitud)
		{
			//printf("Iteracion %d\n",++i);
			aux = read (socket_server, buff + leido, longitud - leido);
			if (aux > 0){
				leido += aux;
			}else
			{
				//Si read devuelve 0, es que se ha cerrado el socket. Devolvemos
				//los caracteres leidos hasta ese momento
				if (aux == 0){ 
					printf("Ganas de matar aumentando\n");
					break;//return leido;
				}
				if (aux == -1)
				{
					//Posibles errores
					switch (errno)
					{
						case EINTR:
						case EAGAIN:
							usleep (100);
							break;
						default:
							return -1;
					}
				}
			}
		}
		fwrite(buff,1,longitud,fd);
		
		tamanio-=longitud;
		
	}
	
	delete(buff);
	return 0;
}
*/

void ClientConnection::WriteOptions(const Options op)
{
	strcpy(opciones.language,op.language);
	strcpy(opciones.speed,op.speed);
	strcpy(opciones.gender,op.gender);
//	strcpy(opciones.data_path,op.data_path);
	opciones.setdur=op.setdur;
}

void ClientConnection::CloseConnection()
{
	close(socket_server);
}

ClientConnection::ClientConnection()
{
	strcpy(opciones.language,"eu");
	strcpy(opciones.speed,"100");
	strcpy(opciones.gender,"F");
	opciones.setdur=0;
}

ClientConnection::ClientConnection(const Options op)
{
	strcpy(opciones.language,op.language);
	strcpy(opciones.gender,op.gender);
//	strcpy(opciones.data_path,op.data_path);
	int f;
	sscanf(op.speed,"%d",&f);
	if(f<SPEED_MIN||f>SPEED_MAX){
		strcpy(opciones.speed,"100");
	}else{
		strcpy(opciones.speed,op.speed);
	}
	opciones.setdur=op.setdur;
}


/*Convertir una cadena de caracteres con numeros en su equivalente int.
 * Hay que tener cuidado con que los caracteres sean numeros y no letras
 * pues lo que hace es restar 48 a su valor ASCII, con letras daria
 * valores admisibles pero ilogicos*/
 //Sería mejor que fuera una función amiga
 /*
int ClientConnection::str2num(const char *cadena)
{
	int ret_val=0, lon,i;
	lon=strlen(cadena);
	for(i=0;i<lon;i++){
		ret_val+=((int)(cadena[lon-1-i]-'0'))*pow(10,i);
	}
	//printf("La cadena: %s es el numero %d\n",cadena,ret_val);
	return ret_val;
}
*/
