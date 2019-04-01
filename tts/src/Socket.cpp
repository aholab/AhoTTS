/*
* Javier Abellan, 20 Jun 2000
*
* Programa Cliente de un socket INET, como ejemplo de utilizacion
* de las funciones de sockets
*/

/*****************************************************************************/
/*                                                                           */
/*                                \m/(-.-)\m/                                */
/*                                                                           */
/*****************************************************************************/
/*
Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
1.1.0	 03/05/12  Agustin    Implementación del tts64 version 1.2.0, funciones generales
* 								Send/ReceiveFile
1.0.0  	 20/01/12  Agustin	  Codificación inicial
*/


#include "Socket.hpp"


/*
* Lee datos del socket. Supone que se le pasa un buffer con hueco 
* suficiente para los datos. Devuelve el numero de bytes leidos o
* 0 si se cierra fichero o -1 si hay error.
*/
int Lee_Socket (int fd, char *Datos, int Longitud)
{
	int Leido = 0;
	int Aux = 0;

	/*
	* Comprobacion de que los parametros de entrada son correctos
	*/
	if ((fd == -1) || (Datos == NULL) || (Longitud < 1))
		return -1;

	/*
	* Mientras no hayamos leido todos los datos solicitados
	*/
	while (Leido < Longitud)
	{
		Aux = read (fd, Datos + Leido, Longitud - Leido);
		if (Aux > 0)
		{
			/* 
			* Si hemos conseguido leer datos, incrementamos la variable
			* que contiene los datos leidos hasta el momento
			*/
			Leido = Leido + Aux;
		}
		else
		{
			/*
			* Si read devuelve 0, es que se ha cerrado el socket. Devolvemos
			* los caracteres leidos hasta ese momento
			*/
			if (Aux == 0) 
				return Leido;
			if (Aux == -1)
			{
				/*
				* En caso de error, la variable errno nos indica el tipo
				* de error. 
				* El error EINTR se produce si ha habido alguna
				* interrupcion del sistema antes de leer ningun dato. No
				* es un error realmente.
				* El error EGAIN significa que el socket no esta disponible
				* de momento, que lo intentemos dentro de un rato.
				* Ambos errores se tratan con una espera de 100 microsegundos
				* y se vuelve a intentar.
				* El resto de los posibles errores provocan que salgamos de 
				* la funcion con error.
				*/
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

	/*
	* Se devuelve el total de los caracteres leidos
	*/
	return Leido;
}

/*
* Escribe dato en el socket cliente. Devuelve numero de bytes escritos,
* o -1 si hay error.
*/
int Escribe_Socket (int fd, char *Datos, int Longitud)
{
	int Escrito = 0;
	int Aux = 0;

	/*
	* Comprobacion de los parametros de entrada
	*/
	if ((fd == -1) || (Datos == NULL) || (Longitud < 1))
		return -1;

	/*
	* Bucle hasta que hayamos escrito todos los caracteres que nos han
	* indicado.
	*/
	while (Escrito < Longitud)
	{
		Aux = write (fd, Datos + Escrito, Longitud - Escrito);
		if (Aux > 0)
		{
			/*
			* Si hemos conseguido escribir caracteres, se actualiza la
			* variable Escrito
			*/
			Escrito = Escrito + Aux;
		}
		else
		{
			/*
			* Si se ha cerrado el socket, devolvemos el numero de caracteres
			* leidos.
			* Si ha habido error, devolvemos -1
			*/
			if (Aux == 0)
				return Escrito;
			else
				return -1;
		}
	}

	/*
	* Devolvemos el total de caracteres leidos
	*/
	return Escrito;
}

/*
 *  filename -> nombre del fichero a mandar
 *  fildes -> open file descriptor donde escribir el archivo
 * */
int Connection::SendFile(const char* filename, int fildes)
{
	//fprintf(stderr,"Mandando archivo %s por conexion %d",filename,fildes);
	int longitud=1024;
	int result=0,aux=0,tamanio=0;
	char *buff=new char[longitud];
	SizeFile file;
	FILE* fd=NULL;
	
	fd=fopen(filename,"rb");
	if(fd!=NULL){
		//saber el tamaño del archivo
		fseek (fd , 0 , SEEK_END);
		tamanio = ftell(fd);
		//printf("Tamanio: %d\n",tamanio);
		rewind (fd);

		//Mandar el tamaño del archivo
		snprintf(file.size,sizeof(SizeFile),"%d",tamanio);
		if(write(fildes,&file.size,sizeof(SizeFile))<0){
			printf("\tError sending the file size\n");
			return -1;
		}

		int i=0;
		int read;		
		while(tamanio>0){
			if(tamanio<longitud){longitud=tamanio;}
			//fprintf(stderr,"Envío del archivo, paquete: %d .",++i);
			//printf("Puntero %d.\n", ftell(fd));
			read=fread(buff,1,longitud,fd);
			if(read!=0){
				result=0;
			while(result<read){
				aux=write(fildes,buff+result,longitud-result);
				
				if(aux>0){//Se ha conseguido escribir algo
					result+=aux;
				}else{
					if(aux==0){//Se ha acabado de escribir
					;//return result;
					}else{//Error
						printf("\tError sending the file %s\n",filename);
						return -1;
					}
				}
			}

			//El ultimo tramo puede ser menos de 1024
			tamanio-=longitud;
			
		}i++;
		}
		fclose(fd);
	}else
	{
		fprintf(stderr,"Error when trying to send the file %s\n",filename);
	}	

	delete(buff);
	return 0;
}

/*
 * filename -> nombre del archivo a escribir lo que recibas
 * fildes -> descriptor de la conexion desde donde lees lo que te mandan
 * */
int Connection::ReceiveFile(const char * filename, int fildes)
{
	//fprintf(stderr,"Recibiendo archivo\n");
	
	int longitud=1024;
	int leido=0,aux=0,tamanio=0;
	char *buff=new char[longitud];
	
	//Obtener el tamaño del archivo 
	SizeFile file;
	aux=read(fildes,file.size,sizeof(SizeFile));
	//fprintf(stderr,"Recibiendo tamanio archivo %s\n", file.size);
	if(aux==-1){
		printf("Error receiving the file size\n");
		return -1;
	}
	/*else{
		printf("Recibido el tamanio del archivo");
	}*/
	sscanf(file.size,"%d",&tamanio);
	//fprintf(stderr,"%d\n",tamanio);
	//int i=0;
	FILE* fd=NULL;
	fd=fopen(filename,"wb");
	if(fd!=NULL){
	//Como saber hasta donde leer
		while(tamanio>0){
			if(tamanio<longitud){longitud=tamanio;}
			
			leido=0;
			while (leido < longitud)
			{
				//printf("Iteracion %d\n",++i);
				aux = read (fildes, buff + leido, longitud - leido);
				if (aux > 0){
					leido += aux;
				}else
				{
					//Si read devuelve 0, es que se ha cerrado el socket. Devolvemos
					//los caracteres leidos hasta ese momento*/
					if (aux == 0){ 
						//fprintf(stderr,"Ganas de matar aumentando\n");
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
			
		}fclose(fd);
	}
	delete(buff);
	return 0;
}





