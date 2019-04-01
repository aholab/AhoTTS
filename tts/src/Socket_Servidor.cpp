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

#define MAXQUEUE 5 /*Tamanio maximo de la cola*/

/*
* Se le pasa un socket de servidor y acepta en el una conexion de cliente.
* devuelve el descriptor del socket del cliente o -1 si hay problemas.
* Esta funcion vale para socket AF_INET o AF_UNIX.
*/
int ServerConnection::AcceptClientConnection()
{
	socklen_t Longitud_Cliente;
	struct sockaddr Cliente;

	/*
	* La llamada a la funcion accept requiere que el parametro 
	* Longitud_Cliente contenga inicialmente el tamano de la
	* estructura Cliente que se le pase. A la vuelta de la
	* funcion, esta variable contiene la longitud de la informacion
	* util devuelta en Cliente
	*/
	Longitud_Cliente = sizeof (Cliente);
	socket_client = accept (descriptor, &Cliente, &Longitud_Cliente);
	if (socket_client == -1)
		return -1;

	/* Si todo va bien devuelve 0*/
	//printf("Se ha abierto el socket cliente %d\n",socket_client);
	return 0;
}

/*
* Abre un socket servidor de tipo AF_INET. Devuelve el descriptor
*	del socket o -1 si hay probleamas
* Se pasa como parametro el nombre del servicio. Debe estar dado
* de alta en el fichero /etc/services
*/
int ServerConnection::OpenInetConnection (const char *IPServidor, const int PuertoServicio)
{
	struct sockaddr_in Direccion;
	struct sockaddr Cliente;
	socklen_t Longitud_Cliente;
	//struct servent *Puerto;


	/* se abre el socket*/
	descriptor = socket (AF_INET, SOCK_STREAM, 0);
	if (descriptor == -1){
		printf("Error en la funcion socket\n");
	 	return -1;}

	//Se obtiene el servicio del fichero /etc/services
	//Como ahora se ha puesto el puerto a pelo esto sobra*/
	//Puerto = getservbyname (Servicio, "tcp");
	//if (Puerto == NULL){
	//	printf("Error al obtener el servicio\n");
	//	return -1;}

	Direccion.sin_family = AF_INET;
	Direccion.sin_port = htons(PuertoServicio); //Puesto a pelo, archivo config.h define PUERTO 15557
	Direccion.sin_addr.s_addr =inet_addr(IPServidor);
	if (bind (descriptor,(struct sockaddr *)&Direccion,sizeof (Direccion)) == -1)
	{
		printf("Error en la funcion bind\n");
		close (descriptor);
		return -1;
	}

	/*
	* Se avisa al sistema que comience a atender llamadas de clientes
	*/
	if (listen (descriptor, MAXQUEUE) == -1)
	{
		printf("Error en la funcion listen\n");
		close (descriptor);
		return -1;
	}

	/* Si todo va bien se devuelve 0*/
	//printf("Abierto el socket server %d\n",descriptor);
	return 0;
}

/*Cerrar la conexion con el cliente*/
void ServerConnection::CloseClientConnection()
{
	//printf("Cerrando conexion %d... ",socket_client);
	close(socket_client);
	//printf("cerrada.\n");
}

/*Cerrar la conexion del servidor*/
void ServerConnection::CloseConnection()
{
	//printf("Cerrando conexion %d... ",descriptor);
	close(descriptor);
	//printf("cerrada.\n");
}

/*Mostrar la utterance y la lenga, por si apetece verlo
 * DEPRECATED*/
 /*
void ServerConnection::Show()
{
	//printf("La utterance es: %s\n",utt);
	printf("La lengua es: %s\n",language);
}
*/
/*Leer la lengua mandada y guardarlo en el el miembro language
 * Si todo va bien devuelve el numero de bytes leidos,
 * en caso de error devuelve -1
 * DEPRECATED*/
/*
int ServerConnection::ReadLanguage()
{
	
	int aux=0;
	Language lang;
	
	aux=read(socket_client,&lang.language,sizeof(Language));
	if(aux<0){
		printf("Error al recibir la lengua\n");
		return aux;
	}
	//printf("Lengua: %s\n",lang.language);
	language=new char[aux];
	
	strcpy(language,lang.language);
	fprintf(stderr,"Lengua a sintetizar: %s\n",language);
	return aux;
}
*/
int ServerConnection::ReadOptions()
{
	int aux=0;
	Options opaux;
	aux=read(socket_client,&opaux,sizeof(Options));
	if (aux<0){
		fprintf(stderr,"Error al recibir las opciones");
		return aux;
	}
	strcpy(opciones.language,opaux.language);
	strcpy(opciones.gender,opaux.gender);
	strcpy(opciones.speed,opaux.speed);
	opciones.setdur=opaux.setdur;
	//fprintf(stderr,"Idioma recibido %s, velocidad de lectura: %s\n",opciones.language,opciones.speed);
	return aux;
}

/*Lee y devuelve una utt de un socket. Primero tiene que recibir la 
 * longitud ,que no puede ser superior a 9999 caracteres,en una 
 * estructura predefinida*/
 /*
 int ServerConnection::ReadUtt()
{
	LongUtt longcadena;
	int longitud=0;
	int leido = 0;
	int aux = 0;	
	
	read(socket_client,&longcadena.longitud,sizeof(LongUtt));

	//Convertir longcadena.longitud en int
	sscanf(longcadena.longitud,"%d",&longitud);
	//longitud=this->str2num(longcadena.longitud);
	//printf("Longcadena %d\n", longitud);
	utt=new char[longitud];
	
	if ((socket_client == -1) || (utt == NULL) || (longitud < 1))
		return -1;

	//Mientras no hayamos leido todos los datos solicitados
	while (leido < longitud)
	{
		aux = read (socket_client, utt + leido, longitud - leido);
		if (aux > 0){
			leido += aux;
		}else
		{
			
			//Si read devuelve 0, es que se ha cerrado el socket. Devolvemos
			//los caracteres leidos hasta ese momento
			
			if (aux == 0) 
				return leido;
			if (aux == -1)
			{
				
				//En caso de error, la variable errno nos indica el tipo
				//de error. 
				//El error EINTR se produce si ha habido alguna
				//interrupcion del sistema antes de leer ningun dato. No
				//es un error realmente.
				//El error EGAIN significa que el socket no esta disponible
				//de momento, que lo intentemos dentro de un rato.
				//Ambos errores se tratan con una espera de 100 microsegundos
				//y se vuelve a intentar.
				//El resto de los posibles errores provocan que salgamos de 
				//la funcion con error.
				
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
	//printf("La cadena leida es: %s\n",*str);

	return leido;	
	
}
*/



/*Una vez sintetizada la oracion se envía mediante send por bloques de 1kbyte
 * (igual es muy poco), el tamaño máximo a mandar es de 2Gbytes (determinado
 * porque tamanio se calcula con un int*/
 /*
int ServerConnection::SendFile(FILE *fd)
{
	//printf("Mandando archivo por socket: ");
	int longitud=1024;
	int result=0,aux=0,tamanio=0;
	char *buff=new char[longitud];
	SizeFile file;
	
	//saber el tamaño del archivo/
	fseek (fd , 0 , SEEK_END);
	tamanio = ftell (fd);
	//printf("%d\n",tamanio);
	rewind (fd);
	
	//Mandar el tamaño del archivo/
	snprintf(file.size,sizeof(SizeFile),"%d",tamanio);
	if(write(socket_client,&file.size,sizeof(SizeFile))<0){
		printf("Error al enviar el tamanio del archivo de audio\n");
		return -1;
	}

	int i=0;
	int read;		
	while(!feof(fd)){
		if(tamanio<longitud){longitud=tamanio+1;}
		//printf("Envío del archivo, paquete: %d .",++i);
		//printf("Puntero %d.\n", ftell(fd));
		read=fread(buff,1,longitud,fd);
		if(read!=0){
			result=0;
		while(result<read){
			aux=write(socket_client,buff+result,longitud-result);
			
			if(aux>0){//Se ha conseguido escribir algo
				result+=aux;
			}else{
				if(aux==0){//Se ha acabado de escribir
				;//return result;
				}else{//Error
					printf("Error enviar el archivo por socket\n");
					return -1;
				}
			}
		}

		//El ultimo tramo puede ser menos de 1024
		tamanio-=longitud;
		
	}i++;
	}	

	delete(buff);
	return 0;
}
*/
/*
ServerConnection::~ServerConnection()
{
	delete(language);
}
*/
