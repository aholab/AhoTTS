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


#ifndef _SOCKET_SERVIDOR_H
#define _SOCKET_SERVIDOR_H
/*
* Includes del sistema
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>

#include "Socket.hpp"

class ServerConnection : public Connection{
	public:
		//~ServerConnection();
		int OpenInetConnection(const char *IPServidor, const int PuertoServicio);
		int AcceptClientConnection();
		//int ReadUtt();
		//int ReadLanguage();
		int ReadOptions();
		//int SendFile(FILE* fd);
		void CloseClientConnection();
		void CloseConnection();
		//void Show();
		//char* utt;
		int ObtainCSocket(void){return socket_client;}
		//char* ObtainLanguage(void){return language;}
		char* ObtainLanguage(void){return opciones.language;}
		char* ObtainSpeed(void){return opciones.speed;}
		char* ObtainGender(void){return opciones.gender;}
		bool ObtainSetDur(void){return opciones.setdur;}
	private:
		Options opciones;
		//char* language;
		int descriptor;
		int socket_client;
		//int str2num(const char *cadena);
};


#endif
