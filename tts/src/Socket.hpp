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
1.1.0	 03/05/12  Agustin    Implementación del tts64 version 1.2.0, funciones generales
* 								Send/ReceiveFile
1.0.0  	 20/01/12  Agustin	  Codificación inicial
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <math.h>

//#include "config.h"

#ifndef _SOCKET_H
#define _SOCKET_H

#define	SPEED_MAX 300 //Valores máximos y mínimos para cambiar la velocidad de lectura
#define	SPEED_MIN 25

//Estas dos funciones sobran, las dejo por ahora para referencia
int Lee_Socket (int fd, char *Datos, int Longitud);
int Escribe_Socket (int fd, char *Datos, int Longitud);

class Connection{
public:
	int SendFile (const char* filename, int fildes);
	int ReceiveFile(const char * filename, int fildes);
};
/*
typedef struct {
	//longitud admisible de la utt de 0001 a 9999 caracteres
	char longitud[4];
} LongUtt;
*/
typedef struct{
	char language[4];//"es", "eu", "cat" "en" o "gl"
} Language;

typedef struct{
	char size[11]; //Como máximo 2^31-1 mas '/0'
} SizeFile;

/*En el futuro cuando se mande language en lugar de eso se mandarán una
 * con todas las opciones elegidas de golpe en  una estructura, de esta
 * manera con una soloa instrucción SendOptions se manda todo de golpe*/
typedef struct{
	char language [4];
	char gender [4];
	char speed [4];
	char data_path[1024];
	bool setdur;
} Options;


#endif
