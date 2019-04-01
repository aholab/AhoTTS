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
/**********************************************************/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/*
Copyright: 1998 - TSR/Aholab (DET) ETSII/IT-Bilbao

Nombre fuente................ wordchop.cpp
Nombre paquete............... normalizador
Lenguaje fuente.............. C++
Estado....................... desarrollo
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Juan Luis Murugarren

Version  dd/mm/aa  Autor     Proposito de la edicion
-------  --------  --------  -----------------------
0.1.2	 12/07/10  Inaki	 Añadir celdas para modificar la prosodia desde el texto (proyecto Aritz) HTTS_PROSO_VAL
0.1.1	 12/10/04  inigos	 separada getchtype en eu/es_wrdch.cpp
														para idiomas
0.1.0	 30/06/04  inigos	 wordchop con acceso a abbreviaturas
0.0.9	 16/11/02  inigos	 Cambiados los caracteres 'Ü', 'ü' a letras
												y 'º', 'ª' a puntuación.
0.0.8    28/11/01  Yon2.     Chartype.c included in WdChop Class.
0.0.7    28/03/01  Yon2.     More Data Need.
0.0.6    11/02/01  Yon2.     Use create method WS.
0.0.5    05/02/01  Yon2.     Create method.
0.0.4    15/01/01  Yon2.     If no dic availabre no dic deletion.
0.0.3    08/01/01  Yon2.     prev -> prevGrp.
0.0.2    06/01/01  Yon2.     Real Time Norm.
0.0.1    04/01/01  Yon2.     Std Module I/O vars init.
0.0.0    31/10/00  Yon2.     Codificacion inicial.

======================== Contenido ========================
  En este m¢dulo se encuentran el constructor/destructor de
la clease funfamental del normalizador. As¡ como los m¢dulos
de preprocesado, salvo el busca-declinaciones.

  El m¢dulo 'preChop' se encarga de cortar palabras del stream por
el que viene el texto crudo. Separa el stream en palabras
conteniendo caracteres del mismo tipo y conservando la
cercanía que hay entre caracteres. Esto último mantiene
agrupadas palabras que forman unidades semánticas. Por
ejemplo: direcciones de correo, fechas, horas, ... en los
que se expresan los contenidos con combinación de
caracteres de distinto tipo.

  El m¢dulo 'puntChop' se encarga de separar de los patrones
signos de puntuaci¢n que se encuentren en el principio o final
del patr¢n. No as¡ los que se hallen en el m‚dio de los patrones.

  El m¢dulo 'show' muestra el estado actual de la lista en la
que se apoya el normalizador.

Yon2.
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

/**********************************************************/

#include <stdio.h>

#include <ctype.h>
#include "wordchop.hpp"
#include "chartype.h"
#include "httsmsg.h"
#include "hdic.hpp"
#include "es_hdic.hpp"
#include "eu_hdic.hpp"
//#include "chset.c"

String UpCase(String x);
/**********************************************************/

CtI WdChop::preChop(pCHAR fltbuff)
{
	INT i,len;
	INT chtype;
	CtI p;

	len = strlen(fltbuff);

	for (i=0; i<len; i++)
	{
		chtype = getchtype(fltbuff[i]);

		if (chtype != CHTYPE_DELIM)
		{
			//hitzaren hasiera bada hemen sartu
			if (wordStarted == FALSE)
			{

				wordStarted = TRUE;
				chtypemem = chtype;
				j=0;
				k=0;
				wordbuff[j++] = fltbuff[i];
				pattern[k++] = getpatsym(chtype);
			}

			//ez bada hitzaren hasiera hemen sartu
			else
			{
				if (chtypemem == chtype)
				{
					wordbuff[j++] = fltbuff[i];
          				//Se supera el tama¤o maximo de buffer para palabra.
					if (j == (MAX_WORD_BUFFER - 1))
					{
						wordbuff[j]='\x0';
						#ifdef HTTS_PROSO_VAL
						if((val_Pros_break!=0)&&(cont==0))
						{
							p = ct.append(wordbuff, groupStarted, *(emo+indiceEmo), *(emoInt+indiceEmo),val_Pros_break,val_Pros_emphasis,val_Pros_pitch,val_Pros_range,val_Pros_rate,val_Pros_volume);//Aritz
							cont++;
							//p=ct.insbefore(p,wordbuff,groupStarted,*(emo+indiceEmo), *(emoInt+indiceEmo),val_Pros_break,val_Pros_emphasis,val_Pros_pitch,val_Pros_range,val_Pros_rate,val_Pros_volume);//Aritz)
						}
						else if((val_Pros_break!=0)&&(cont!=0))
						{
							val_Pros_break=0;
							cont --;
							p = ct.append(wordbuff, groupStarted, *(emo+indiceEmo), *(emoInt+indiceEmo),0,val_Pros_emphasis,val_Pros_pitch,val_Pros_range,val_Pros_rate,val_Pros_volume);//Aritz
						}
						else
						{
							p = ct.append(wordbuff, groupStarted, *(emo+indiceEmo), *(emoInt+indiceEmo),val_Pros_break,val_Pros_emphasis,val_Pros_pitch,val_Pros_range,val_Pros_rate,val_Pros_volume);//Aritz

						}
						#else
						p = ct.append(wordbuff, groupStarted, *(emo+indiceEmo), *(emoInt+indiceEmo));
						#endif

						j=0;
						wordStarted = FALSE;

						pattern[k]='\x0';
						ct.setPattern(p, pattern);

					}
					else
					{
					}
				}
				else
				{
					wordbuff[j]='\x0';
					#ifdef HTTS_PROSO_VAL
					if((val_Pros_break!=0)&&(cont==0))
					{
						ct.append(wordbuff, groupStarted, *(emo+indiceEmo), *(emoInt+indiceEmo),val_Pros_break,val_Pros_emphasis,val_Pros_pitch,val_Pros_range,val_Pros_rate,val_Pros_volume);//Aritz
						cont++;
						//ct.insbefore(p,wordbuff,groupStarted,*(emo+indiceEmo), *(emoInt+indiceEmo),val_Pros_break,val_Pros_emphasis,val_Pros_pitch,val_Pros_range,val_Pros_rate,val_Pros_volume);//Aritz)
						//fprintf(stderr,"Entro2.1\n");
					}
					else if((val_Pros_break!=0)&&(cont!=0))
					{
						val_Pros_break=0;
						cont--;
						p = ct.append(wordbuff, groupStarted, *(emo+indiceEmo), *(emoInt+indiceEmo),0,val_Pros_emphasis,val_Pros_pitch,val_Pros_range,val_Pros_rate,val_Pros_volume);//Aritz


					}
					else
					{
						p = ct.append(wordbuff, groupStarted, *(emo+indiceEmo), *(emoInt+indiceEmo),val_Pros_break,val_Pros_emphasis,val_Pros_pitch,val_Pros_range,val_Pros_rate,val_Pros_volume);//Aritz

					}
					#else
					ct.append(wordbuff, groupStarted, *(emo+indiceEmo), *(emoInt+indiceEmo));
					#endif

					j=0;
					chtypemem = chtype;
					wordbuff[j++] = fltbuff[i];
					pattern[k++] = getpatsym(chtype);

					groupStarted = FALSE;
				}
			}
		}
		else
		{
			if (wordStarted == TRUE)
			{
				wordbuff[j]='\x0';
				#ifdef HTTS_PROSO_VAL
				if((val_Pros_break!=0)&&(cont==0))
				{
					p = ct.append(wordbuff, groupStarted, *(emo+indiceEmo), *(emoInt+indiceEmo),val_Pros_break,val_Pros_emphasis,val_Pros_pitch,val_Pros_range,val_Pros_rate,val_Pros_volume);//Aritz
					cont++;
					//p=ct.insbefore(p,wordbuff,groupStarted,*(emo+indiceEmo), *(emoInt+indiceEmo),val_Pros_break,val_Pros_emphasis,val_Pros_pitch,val_Pros_range,val_Pros_rate,val_Pros_volume);//Aritz)
					//fprintf(stderr,"Entro3.1\n");
				}
				else if((val_Pros_break!=0)&&(cont!=0))
				{
					val_Pros_break=0;
					cont--;
					p = ct.append(wordbuff, groupStarted, *(emo+indiceEmo), *(emoInt+indiceEmo),0,val_Pros_emphasis,val_Pros_pitch,val_Pros_range,val_Pros_rate,val_Pros_volume);//Aritz

				}
				else
				{
					p = ct.append(wordbuff, groupStarted, *(emo+indiceEmo), *(emoInt+indiceEmo),val_Pros_break,val_Pros_emphasis,val_Pros_pitch,val_Pros_range,val_Pros_rate,val_Pros_volume);//Aritz
				}
				#else
				p = ct.append(wordbuff, groupStarted, *(emo+indiceEmo), *(emoInt+indiceEmo));
				#endif

				wordStarted = FALSE;
				pattern[k]='\x0';
				ct.setPattern(p, pattern);
			}
			groupStarted = TRUE;
		}
	}

  return ct.lastGrp();


}


CtI WdChop::parserEmo(pCHAR fltbuff, BOOL flush)
{
	INT len;			//Tamaño total de fltbuff
	pCHAR labelStart;	//Posición del inicio de la etiqueta en fltbuff
	pCHAR labelEnd;		//Posición del final de la etiqueta en fltbuff

	//Buffer para contener la supuesta etiqueta cuando es excesivamente larga
	pCHAR tmpBuff=(char*)malloc((MAX_LABEL_BUFFER+30) * sizeof (char));


	//Salida de la función.
	CtI salida;

	if (flush)
	{
		if (labelStarted)
		{
			labelStarted = FALSE;
			len = strlen (labelBuff);
			labelBuff [len] = ' ';
			labelBuff [len+1] = '\0';
			salida = preChop (labelBuff);
		}
	} else {
		len = strlen(fltbuff);

		//Si la etiqueta estaba empezada de antes
		if (labelStarted == TRUE){
			if ((labelEnd = strchr (fltbuff, '>')) != NULL){
			//La etiqueta acaba: Ya no está empezada
				labelStarted = FALSE;

				//Copiamos al buffer labelBuff el cacho de etiqueta que le felata
				int labelLong = len - strlen(labelEnd);
				strncat (labelBuff, fltbuff, labelLong + 1 );

				//Pasamos la supuesta etiqueta a filterLabel, para su procesamiento
				#ifdef HTTS_PROSO_VAL
				if(EtiqPros(labelBuff))//Aritz
				{
				#endif
					if (filterLabel (labelBuff)){
					//Resulta que no era una etiqueta. Hay que pasarlo a preChop
						tmpBuff [0] = labelBuff [0];
						tmpBuff [1] = '\0';

						preChop (tmpBuff);
						labelStarted = FALSE;

						strcpy (tmpBuff, labelBuff+1);

						salida = parserEmo (tmpBuff);
					}
				#ifdef HTTS_PROSO_VAL
				}
				#endif

				//El resto de fltbuff se pasa a parserEmo, para que siga procesando posibles etiquetas más
				salida = parserEmo (labelEnd+1);

			}else{
			//La etiqueta todavía no acaba. Todo fltbuff es parte de la etiqueta
				if ((strlen(labelBuff) +  strlen (fltbuff)) > MAX_LABEL_BUFFER){
				//La etiqueta queda demasiado larga. Se supone, por tanto, que no es etiqueta.

					//Puesto que no era etiqueta:
					labelStarted = FALSE;
					//Se copia el contenido al buffer largo y se le pasa a prechop.
					strcpy (tmpBuff,labelBuff);
					strcat (tmpBuff,fltbuff);
					salida = preChop (tmpBuff);


				}else{
				//La etiqueta no es excesivamente larga. Continuamos concatenando en labelBuff
					strcat (labelBuff, fltbuff);

					//Por darle una salida, se le da la aúltima.
					salida = ct.lastGrp();
				}
			}
		}

		//Si la etiqueta NO estaba empezada de antes

		else if ((labelStart = strchr (fltbuff, '<')) != NULL) {
		//Empieza la etiqueta
			labelStarted = TRUE;

			if ((labelEnd = strchr (labelStart, '>')) != NULL){
			//Además de empezar, acaba
				labelStarted=FALSE;
				//Se copia el trozo que corresponde a la etiqueta sobre labelBuff
				int labelLong = labelEnd - labelStart +1;
				strncpy (labelBuff, labelStart, labelLong);
				labelBuff [labelLong] = '\0';

				//Trucamos fltbuff para quedarnos con el trozo ANTES de la etiqueta, y se lo pasamos a preChop
				*labelStart=' ';
				*(labelStart+1)='\0';

				preChop (fltbuff);

				//Ahora vendría la etiqueta, y hay que procesarla
				#ifdef HTTS_PROSO_VAL
				if(EtiqPros(labelBuff)){//Aritz
				#endif
				if (filterLabel (labelBuff)){
				//Resulta que no era una etiqueta. Hay que pasarlo a preChop
					tmpBuff [0] = labelBuff [0];
					tmpBuff [1] = '\0';

					preChop (tmpBuff);

					strcpy (tmpBuff, labelBuff+1);

					parserEmo (tmpBuff);
				}
				#ifdef HTTS_PROSO_VAL
				}
				#endif
				//El resto se lo pasamos a parserEmo para que siga procesando
				salida = parserEmo (labelEnd+1);

			}else{
			//Empieza la etiqueta pero no acaba
				//Se copia el trozo que corresponde a la etiqueta sobre labelBuff
				strcpy (labelBuff, labelStart);

				//Trucamos fltbuff para quedarnos con el trozo ANTES de la etiqueta, y se lo pasamos a preChop
				*labelStart=' ';
				*(labelStart+1)='\0';

				salida = preChop (fltbuff);
			}
		}


		//Ni siquiera hay etiqueta

		else {
		//Se lo pasamos directo al preChop
			salida = preChop(fltbuff);
		}
	}

	//Liberamos memoria y terminamos
	free (tmpBuff);
	return (salida);
}


//Función que analiza las etiquetas extraidas y activa los campos de emoción
//según corresponda. En caso de que el buffer que se le pasa no corresponda a una etiqueta
//válida, devuelve TRUE. FALSE en caso de que sea una etiqueta válida.

BOOL WdChop::filterLabel(pCHAR label){
	CHAR **tabla;	//Tabla con el desglose de palabras y símbolos de la supuesta etiqueta
	int len;		//Longitud de la etiqueta
	String aux;		//cadena auxiliar
	int i = 0;
	int j = 0;
	int k = 0;
	BOOL salida;	//Salida de la función

	//Comprobamos que no haya un '<' dentro de la propia etiqueta
	if (strchr (label+1, '<'))
		return TRUE;

	len=strlen(label);
	//Reservamos memoria para la tabla

	tabla = (CHAR **) malloc (MAX_LABEL_WORDS * sizeof(char*));
	for (i = 0; i < MAX_LABEL_WORDS ;i ++ ){
		tabla [i] = (CHAR*) malloc (MAX_WORD_LEN*sizeof(char));
	}

	//Desglosamos la etiqueta en palabras y las metemos en la tabla
	for (i = 0; i < len; i ++)	{
		if ((label[i]!='<')&&(label[i]!='>')&&(label[i]!='/')&&(label[i]!=' ')&&(label[i]!='\'')&&(label[i]!='\"')&&(label[i]!='=')){
			tabla[j][k]=label[i];
			k++;
		}else if ((label[i] == ' ') && (label[i+1] != ' ') && (k != 0)){
			tabla[j][k] = '\0';
			j ++;
			k = 0;
		}else if (label[i] == '/'){
			tabla[j][k] = '/';
			tabla[j][k+1] = '\0';
			j ++;
			k = 0;
		}else if (label[i] == '='){
			if (label[i-1] != ' '){
				tabla[j][k] = '\0';
				j ++;
			}
			tabla[j][0] = '=';
			tabla[j][1] = '\0';
			j ++;
			k = 0;
		}else if (label[i] == '>'){
			if (label[i-1] != ' '){
				tabla[j][k] = '\0';
				j ++;
				k = 0;
			}
		}
	}

	//Vamos analizando la tabla para comprobar la etiqueta
	if (tabla[0][0] ==  '/'){
	//En este caso la etiqueta es de final
		//Miramos la emoción a la que correspnde
		aux= tabla[1];
		aux = UpCase (aux);
		i = indexEmo (aux);

		if ((i != -1) && (emo[indiceEmo] == i)){
		//La palabra después de '/' SI es una emoción y coincide con el de la pila. Eliminamos una emoción de la lista
			indiceEmo=indiceEmo--;
			salida = FALSE;
		}else{
		//La palabra después de '/' NO es una emoción o NO coindice con el de la pila. Hay que sintetizar
			salida = TRUE;
		}
	}else{
	//En este caso la etiqueta es de principio
		//primero miramos a ver que emocion es
		aux = tabla[0];
		aux = UpCase (aux);
		i = indexEmo (aux);

		if (i!=-1){
		//Se trata de una etiqueta válida. No hay que sintetizar
			salida = FALSE;

			//Alteramos la pila
			indiceEmo = indiceEmo ++;
			emo[indiceEmo] = i;

			//Ahora buscamos la etiqueta de intensidad
			BOOL intFound = FALSE;
			for (k = 0; k < j; k ++){
			//Recorremos cada palabra en busca de la clave de intensidad
				aux = (String)tabla[k];
				aux = UpCase (aux);
				if (aux == "INTENSITY"){
				//Se ha encontrado la clave de intensidad
					intFound = TRUE;
					if (!strcmp(tabla[k+1], "=")){
					//Hay un signo '=' Habria que mirar si lo siguiente es un valor entero y si no dar error
						if(atoi(tabla[k+2])){
						//Es un valor entero, no hay error. Actualizamos la pila de intensidades
							emoInt[indiceEmo] = atoi(tabla[k+2]);
						}else {
						//NO es un valor entero, hay error.
							htts_error("WdChop::Error EML 2");
						}
					}else{
					//NO hay un signo '=' Es un error, pues después de INTENSITY no puede haber otra cosa
						htts_error("WdChop::Error EML 3");
					}
				}
			}
			if (intFound == FALSE)
			//Resulta que no hay intensidad. Se asigna la intesidad por defecto
				emoInt[indiceEmo] = emoInt[0];
		}else{
		//No es una etiqueta válida. Hay que sintetizar
			salida = TRUE;
		}
	}

	//Liberams memoria y terminamos
	for (i = 0; i < MAX_LABEL_WORDS ;i++ ){
		free (tabla [i]);
	}
	free (tabla);

	return salida;
}
#ifdef HTTS_PROSO_VAL
BOOL WdChop::EtiqPros(pCHAR label)//Aritz
{

	char **tabla;
	int len;
	int i=0;
	int j=0;
	int k=0;
	String aux;
	BOOL salida=TRUE;




	//fprintf(stderr,"etiqueta de prosodia:%s\n",label);



	if(strchr(label+1,'<'))
	{
		return TRUE;
	}
	len=strlen(label);


	tabla = (CHAR **) malloc (MAX_LABEL_WORDS * sizeof(char*));

	for (i = 0; i < MAX_LABEL_WORDS ;i ++ ){
		tabla [i] = (CHAR*) malloc (MAX_WORD_LEN*sizeof(char));
	}

	//Desglosamos la etiqueta en palabras y las metemos en la tabla

	for (i = 0; i < len; i ++)
	{
		if ((label[i]!='<')&&(label[i]!='>')&&(label[i]!='/')&&(label[i]!=' ')&&(label[i]!='\'')&&(label[i]!='\"')&&(label[i]!='='))
		{
			tabla[j][k]=label[i];
			k++;

		}
		else if ((label[i] == ' ') && (label[i+1] != ' ') && (k != 0))
		{
			tabla[j][k] = '\0';
			j ++;
			k = 0;

		}
		else if (label[i] == '/')
		{
			tabla[j][k] = '/';
			tabla[j][k+1] = '\0';
			j ++;
			k = 0;

		}
		else if (label[i] == '=')
		{
			if (label[i-1] != ' ')
			{
				tabla[j][k] = '\0';
				j ++;

			}
			tabla[j][0] = '=';
			tabla[j][1] = '\0';
			j ++;
			k = 0;

		}
		else if (label[i] == '>')
		{
			if (label[i-1] != ' ')
			{
				tabla[j][k] = '\0';
				j ++;
				k = 0;

			}
		}
	}

	if(tabla[0][0]=='/') //Es una Etiqueta de final.
	{
		//fprintf(stderr,"entro final\n");

		aux=tabla[1];
		aux=UpCase(aux);
		i=indexProsod(aux);
		if((i==-1)&&(aux=="PROSODY"))//Si es una etiqueta de prosody hay que saber de cual se trata
		{
			if(Pros[indicePros]==UPROSODY_PITCH)
			{
				salida=FALSE;
				indicePros=indicePros--;
				val_Pros_pitch=DEFAULT;
			}
			else if(Pros[indicePros]==UPROSODY_RATE)
			{
				salida=FALSE;
				indicePros=indicePros--;
				val_Pros_rate=DEFAULT;
			}
			else if(Pros[indicePros]==UPROSODY_RANGE)
			{
				salida=FALSE;
				indicePros=indicePros--;
				val_Pros_range=DEFAULT;
			}
			else if(Pros[indicePros]==UPROSODY_VOLUME)
			{
				salida=FALSE;
				indicePros=indicePros--;
				val_Pros_volume=DEFAULT;
			}
			else
			{
				for(j=indicePros;j>=0;j--)// mirar si hay algun break anidado
				{
					if((Pros[j]==UPROSODY_PITCH)||(Pros[j]==UPROSODY_RATE)||(Pros[j]==UPROSODY_RANGE)||(Pros[j]==UPROSODY_VOLUME))
					{
						salida=FALSE;
						indicePros=indicePros--;
					}
				}
			}

		}
		else if((i!=-1)&&(Pros[indicePros]==i))//Cuando la etiqueta es de emphasis y esta sin anidar
		{
			salida=FALSE;
			indicePros=indicePros--;
			val_Pros_emphasis=DEFAULT;



		}
		else if ((i!=-1)&&(Pros[indicePros]!=i))//Cuando la etiqueta es de emphasis y esta anidada con varias etiquetas de break
		{
			for(j=indicePros;j>=0;j--)
			{
				if(Pros[j]==i)
				{
					salida=FALSE;
					indicePros=indicePros--;
				}
			}
		}
		else           //Si no es una etiqueta
		{
			salida=TRUE;
		}

	}

	else 		//Es una Etiqueta de inicio.
	{

		aux=tabla[0];
		aux=UpCase(aux);
		i=indexProsod(aux);
		if((i==-1)&&(aux=="PROSODY"))
		{



				for(k=0;k<j;k++) //Recorremos la tabla buscando la siguiente palabra.
				{
					aux=tabla[k];
					aux=UpCase(aux);
					if(aux=="RANGE")
					{



						if(*tabla[k+1]=='=')
						{


							if(ValorValRange(tabla[k+2],&val_Pros_range))
							{

								salida=FALSE;
								indicePros=indicePros++;
								aux="PROSODY RANGE";
								Pros[indicePros]=indexProsod(aux);


							}

							else
							{
								val_Pros_range=DEFAULT;
								salida=TRUE;
							}
						}
					}
					else if(aux=="PITCH")
					{

						if(*tabla[k+1]=='=')
						{

							if(ValorValPitch(tabla[k+2],&val_Pros_pitch))
							{

								salida=FALSE;
								indicePros=indicePros++;
								aux="PROSODY PITCH";
								Pros[indicePros]=indexProsod(aux);

							}
							else
							{
								val_Pros_pitch=DEFAULT;
								salida=TRUE;
							}

						}
					}
					else if(aux=="VOLUME")
					{
						if(*tabla[k+1]=='=')
						{

							if(ValorValProsodyVolum(tabla[k+2],&val_Pros_volume))
								{
										salida=FALSE;
										indicePros=indicePros++;
										aux="PROSODY VOLUME";
										Pros[indicePros]=indexProsod(aux);
								}
								else
								{
									val_Pros_volume=DEFAULT;
									salida=TRUE;
								}
						}
					}
					else if(aux=="RATE")
					{
						if(*tabla[k+1]=='=')
						{

							if(ValorValRate(tabla[k+2],&val_Pros_rate))
								{
										salida=FALSE;
										indicePros=indicePros++;
										aux="PROSODY RATE";
										Pros[indicePros]=indexProsod(aux);
								}
								else
								{
									val_Pros_rate=DEFAULT;
									salida=TRUE;
								}
						}
					}
				}
			}
			else if((i!=-1)&&(aux=="BREAK"))
			 {
				for(k=0;k<j;k++)
				{
					aux=(String)tabla[k];
					aux=UpCase(aux);

					if(aux=="STRENGTH")
					{


						if(*tabla[k+1]=='=')
						{

							if(ValorValBreakStrength(tabla[k+2],&val_Pros_break))
							{


								salida=FALSE;
								indicePros=indicePros++;
								aux="BREAK";
								Pros[indicePros]=indexProsod(aux);


							}
							else
							{
								val_Pros_break=DEFAULT;
								salida=TRUE;
							}

						}
					}

					else if(aux=="TIME")
					{
						if(*tabla[k+1]=='=')
						{
							salida=FALSE;
							if(ValorValBreakTime(tabla[k+2],&val_Pros_break))

							{
								salida=FALSE;
								indicePros=indicePros++;
								aux="BREAK";
								Pros[indicePros]=indexProsod(aux);

							}

							else
							{

								salida=TRUE;
								val_Pros_break=DEFAULT;
							}
						}
					}
				}

			}
			else if((i!=-1)&&(aux=="EMPHASIS"))
			{
				for(k=0;k<j;k++)
				{
					aux=(String)tabla[k];
					aux=UpCase(aux);
					if(aux=="LEVEL")
					{

						if(*tabla[k+1]=='=')
						{

							if(ValorValEmphasis(tabla[k+2],&val_Pros_emphasis))
							{
								salida=FALSE;
								indicePros=indicePros++;
								aux="EMPHASIS";
								Pros[indicePros]=indexProsod(aux);

							}
							else
							{
								val_Pros_emphasis=DEFAULT;
								salida=TRUE;
							}
						}
					}
				}
			}

	}

	for (i = 0;i < MAX_LABEL_WORDS;i++ )
	{
		free (tabla [i]);
	}
	free (tabla);

	//fprintf(stderr,"Es etiqueta de prosody:%d\n",salida);

	//fprintf(stderr,"tipo de etiqueta:%d\n",Pros[indicePros]);

	//fprintf(stderr,"valor de la etiqueta:%d\n",val);


	return salida;
}

BOOL WdChop::ValorValPitch(String valor,int *val_Pros_pitch)//Aritz
{
	valor=UpCase(valor);
	int i,len;


	len=strlen(valor);

	if(valor=="X-LOW")
	{
		*val_Pros_pitch=X_LOW;
		return TRUE;
	}
	else if(valor=="LOW")
	{
		*val_Pros_pitch=LOW;
		return TRUE;
	}
	else if(valor=="MEDIUM")
	{
		*val_Pros_pitch=MEDIUM_P;
		return TRUE;
	}
	else if(valor=="HIGH")
	{
		*val_Pros_pitch=HIGH;
		return TRUE;
	}
	else if(valor=="X-HIGH")
	{
		*val_Pros_pitch=X_HIGH;
		return TRUE;
	}
	else
	{
		for(i=0;i<len;i++)
		{
			if((valor[i]=='+')&&(valor[i+3]=='%'))
			{
				*val_Pros_pitch=atoi(valor);
				return TRUE;
			}
			else if((valor[i]=='+')&&(valor[i+2]=='%'))
			{
				*val_Pros_pitch=atoi(valor);
				return TRUE;
			}
			else if((valor[i]=='-')&&(valor[i+2]=='%'))
			{
				*val_Pros_pitch=atoi(valor);
				return TRUE;
			}
			else if((valor[i]=='+')&&(valor[i+4]=='%')&&(atoi(valor)==100))
			{
				*val_Pros_pitch=atoi(valor);
				return TRUE;
			}
			else if((valor[i]=='-')&&(valor[i+4]=='%')&&(atoi(valor)==-100))
			{
				*val_Pros_pitch=atoi(valor);
				return TRUE;
			}
			else if((valor[i]=='-')&&(valor[i+3]=='%'))
			{
				*val_Pros_pitch=atoi(valor);
				return TRUE;
			}
			else
			{
				htts_warn("WdChop::ValorValPitch() - Valor no válido(%d)",atoi(valor));
				return FALSE;
			}
		}
	}
}
BOOL WdChop::ValorValRange(String valor,int *val_Pros_range)//Aritz
{
	valor=UpCase(valor);
	int i,len;
	len =strlen(valor);
	if(valor=="X-LOW")
	{
		*val_Pros_range=X_LOW_RAN;
		return TRUE;
	}
	else if(valor=="LOW")
	{
		*val_Pros_range=LOW_RAN;
		return TRUE;
	}
	else if(valor=="MEDIUM")
	{
		*val_Pros_range=MEDIUM_RAN;
		return TRUE;
	}
	else if(valor=="HIGH")
	{
		*val_Pros_range=HIGH_RAN;
		return TRUE;
	}
	else if(valor=="X-HIGH")
	{
		*val_Pros_range=X_HIGH_RAN;
		return TRUE;
	}
	else
	{
		for(i=0;i<len;i++)
		{
			if((valor[i]=='+')&&(valor[i+3]=='%'))
			{
				*val_Pros_range=atoi(valor);
				return TRUE;
			}
			else if((valor[i]=='+')&&(valor[i+2]=='%'))
			{
				*val_Pros_range=atoi(valor);
				return TRUE;
			}
			else if((valor[i]=='-')&&(valor[i+2]=='%'))
			{
				*val_Pros_range=atoi(valor);
				return TRUE;
			}
			else if((valor[i]=='+')&&(valor[i+4]=='%')&&(atoi(valor)==100))
			{
				*val_Pros_range=atoi(valor);
				return TRUE;
			}
			else if((valor[i]=='-')&&(valor[i+4]=='%')&&(atoi(valor)==-100))
			{
				*val_Pros_range=atoi(valor);
				return TRUE;
			}
			else if((valor[i]=='-')&&(valor[i+3]=='%'))
			{
				*val_Pros_range=atoi(valor);
				return TRUE;
			}
			else
			{
				htts_warn("WdChop::ValorValRange() - Valor no válido(%d)",atoi(valor));
				return FALSE;
			}
		}

	}
}
BOOL WdChop::ValorValRate(String valor,int *val_Pros_rate)//Aritz
{

	int i,len;

	valor=UpCase(valor);
	len =strlen(valor);
	if(valor=="X-SLOW")
	{
		*val_Pros_rate=X_SLOW;
		return TRUE;
	}
	else if(valor=="SLOW")
	{
		*val_Pros_rate=SLOW;
		return TRUE;
	}
	else if(valor=="MEDIUM")
	{
		*val_Pros_rate=MEDIUM_RAT;
		return TRUE;
	}
	else if(valor=="FAST")
	{
		*val_Pros_rate=FAST;
		return TRUE;
	}
	else if(valor=="X-FAST")
	{
		*val_Pros_rate=X_FAST;
		return TRUE;
	}
	else
	{
//		fprintf(stderr,"valor=%c%c%c\n", valor[0],valor[1], valor[2]);
		for(i=0;i<len;i++)
		{
			if((valor[i]=='+')&&(valor[i+3]=='%'))
			{
				*val_Pros_rate=atoi(valor);
				return TRUE;
			}
			else if((valor[i]=='+')&&(valor[i+2]=='%'))
			{
				*val_Pros_rate=atoi(valor);
				return TRUE;
			}
			else if((valor[i]=='-')&&(valor[i+2]=='%'))
			{
				*val_Pros_rate=atoi(valor);
				return TRUE;
			}
			else if((valor[i]=='+')&&(valor[i+4]=='%')&&(atoi(valor)==100))
			{
				*val_Pros_rate=atoi(valor);
				return TRUE;
			}
			else if((valor[i]=='-')&&(valor[i+4]=='%')&&(atoi(valor)==-100))
			{
				*val_Pros_rate=atoi(valor);
				return TRUE;
			}
			else if((valor[i]=='-')&&(valor[i+3]=='%'))
			{
				*val_Pros_rate=atoi(valor);
				return TRUE;
			}
			//inaki para añadir la posibilidad de introducir marca de tiempo en ms
			else if( len > 3 && valor[len-2]=='M' && valor[len-1]=='S' ){
			        *val_Pros_rate=atoi(valor);
				return TRUE;
			}
			//////////////////////////////////////////////////////////////
			else
			{
				htts_warn("WdChop::ValorValRate() - Valor no válido(%d)",atoi(valor));
				return FALSE;
			}
		}


	}
}
BOOL WdChop::ValorValProsodyVolum(String valor,int *val_Pros_volume)//Aritz
{

	int i,len;
	len =strlen(valor);

	valor=UpCase(valor);
	if(valor=="X-SOFT")
	{
		*val_Pros_volume=X_SOFT;
		return TRUE;
	}
	else if(valor=="SOFT")
	{
		*val_Pros_volume=SOFT;
		return TRUE;
	}
	else if(valor=="MEDIUM")
	{
		*val_Pros_volume=MEDIUM_V;
		return TRUE;
	}
	else if(valor=="LOUD")
	{
		*val_Pros_volume=LOUD;
		return TRUE;
	}
	else if(valor=="X-LOUD")
	{
		*val_Pros_volume=X_LOUD;
		return TRUE;
	}
	else
	{
		for(i=0;i<len;i++)
		{
			if((valor[i]=='+')&&(valor[i+3]=='%'))
			{
				*val_Pros_volume=atoi(valor);
				return TRUE;
			}
			else if((valor[i]=='+')&&(valor[i+2]=='%'))
			{
				*val_Pros_volume=atoi(valor);
				return TRUE;
			}
			else if((valor[i]=='-')&&(valor[i+2]=='%'))
			{
				*val_Pros_volume=atoi(valor);
				return TRUE;
			}
			else if((valor[i]=='+')&&(valor[i+4]=='%')&&(atoi(valor)==100))
			{
				*val_Pros_volume=atoi(valor);
				return TRUE;
			}
			else if((valor[i]=='-')&&(valor[i+4]=='%')&&(atoi(valor)==-100))
			{
				*val_Pros_volume=atoi(valor);
				return TRUE;
			}
			else if((valor[i]=='-')&&(valor[i+3]=='%'))
			{
				*val_Pros_volume=atoi(valor);
				return TRUE;
			}
			else
			{
				htts_warn("WdChop::ValorValProsodyVolum() - Valor no válido(%d)",atoi(valor));
				return FALSE;
			}
		}
	}
}
BOOL WdChop::ValorValEmphasis(String valor,int *val_Pros_emphasis)//Aritz
{
	valor=UpCase(valor);
	if(valor=="REDUCED")
	{
		*val_Pros_emphasis=REDUCED;
		return TRUE;
	}
	else if(valor=="MODERATE")
	{
		*val_Pros_emphasis=MODERATE;
		return TRUE;
	}
	else if(valor=="STRONG")
	{
		*val_Pros_emphasis=STRONG_E;
		return TRUE;
	}
	else
	{

		return FALSE;
	}
}
BOOL WdChop::ValorValBreakStrength(String valor,int *val_Pros_break)//Aritz
{
	valor=UpCase(valor);
	if(valor=="X-WEAK")
	{
		*val_Pros_break=X_WEAK;
		return TRUE;
	}
	else if(valor=="WEAK")
	{
		*val_Pros_break=WEAK;
		return TRUE;
	}
	else if(valor=="MEDIUM")
	{
		*val_Pros_break=MEDIUM_B;
		return TRUE;
	}
	else if(valor=="STRONG")
	{
		*val_Pros_break=STRONG;
		return TRUE;
	}
	else if(valor=="X-STRONG")
	{
		*val_Pros_break=X_STRONG;
		return TRUE;
	}
	else
	{

		return FALSE;
	}
}
BOOL WdChop::ValorValBreakTime(String valor,int *val_Pros_break)//Aritz
{

	int len,i;
	bool ms=0,s=0;
	valor=UpCase(valor);
	len=strlen(valor);
	for(i=0;i<len;i++)
	{
		if((valor[i]=='M')&&(valor[i+1]=='S'))
		{
			ms=1;

		}
		else if((valor[i]=='S')&&(valor[i-1]!='N'))
		{
			s=1;
		}
		else
		{
			ms=0;
			s=0;
		}
	}

	if((atoi(valor)>0)&&(ms==1))
	{
		*val_Pros_break=atoi(valor);
		return TRUE;
	}
	else if((atoi(valor)>0)&&(s==1))
	{
		*val_Pros_break=atoi(valor)*1000;
		return TRUE;
	}
	else if((atoi(valor)<0))
	{
		htts_warn("Wdchop::ValorValBreakTime() - Valor no válido(%d) - Solo valores positivos",atoi(valor));
		return FALSE;

	}
	else
	{
		return FALSE;
	}
}
#endif
String UpCase(String x)
{
	char *s = (char *) x.chars();
	char *t = (char *) x.chars();
	while (*s)
	{
		*s=toupper(*s);
		s++;
	}

	return (String (t));
}

#ifdef HTTS_PROSO_VAL
INT WdChop::indexProsod(String Pros)//Aritz
{
	Pros=UpCase(Pros);

	if(Pros==PROS_EMPHASIS)
	{
		return UEMPHASIS;
	}
	else if(Pros==PROS_PROSODY_PITCH)
	{
		return UPROSODY_PITCH;
	}
	else if ( Pros == PROS_PROSODY_RANGE)
	{
		return UPROSODY_RANGE;
	}
	else if ( Pros ==PROS_PROSODY_RATE) {
		return UPROSODY_RATE;
	}
	else if ( Pros ==PROS_PROSODY_VOLUME )
	{
		return UPROSODY_VOLUME;
	}
	else if ( Pros == PROS_BREAK)
	{
		return UBREAK;

	}
	else
	{
		return(-1);
	}
}
#endif
INT WdChop::indexEmo (String & Emo)
{
	Emo = UpCase (Emo);
	if ( Emo == EMO_DEFAULT_STR) {
		return UEMOTION_DEFAULT;
	}
	else if ( Emo == EMO_NEUTRAL_STR) {
		return UEMOTION_NEUTRAL;
	}
	else if ( Emo == EMO_AFRAID_STR) {
		return UEMOTION_AFRAID;
	}
	else if ( Emo == EMO_ANGRY_STR) {
		return UEMOTION_ANGRY;
	}
	else if ( Emo == EMO_CONFUSED_STR) {
		return UEMOTION_CONFUSED;
	}
	else if ( Emo == EMO_DAZED_STR) {
		return UEMOTION_DAZED;
	}
	else if ( Emo == EMO_DISGUSTED_STR) {
		return UEMOTION_DISGUSTED;
	}
	else if ( Emo == EMO_HAPPY_STR) {
		return UEMOTION_HAPPY;
	}
	else if ( Emo == EMO_SAD_STR) {
		return UEMOTION_SAD;
	}
	else if ( Emo == EMO_SURPRISED_STR) {
		return UEMOTION_SURPRISED;
	}
	else
		return (-1);
}


//---------------------------------------------------------


/**********************************************************/


/**********************************************************/
//Corta el tipo más a la izquierda del patrón haciéndolo independiente.
//Lo corta sea lo que sea luego hay que saber de antemano que es el que
//queremos cortar.
//Devuelve el indice de lista al nuevo elemento independiente.
CtI WdChop::patCutLeft(CtI p)
{
	CtI q;
	CHAR patindep[]="X";
	pCHAR patnew;

	q = ct.next(p);

	patindep[0] = (ct(p).pattern)[0];
	patnew = strdup(&((ct(p).pattern)[1]));

	ct.setPattern(p, patindep);
	ct.setLeftFrg(p);
	ct.setStart(q, TRUE); /* Nueva Celda independiente del grupo */
	ct.setPattern(q, patnew);

	free(patnew);

	return p;
}

/***********************************************************************/


/***********************************************************************/
//Corta el tipo más a la derecha del patrón haciéndolo independiente.
//Devuelve el indice de lista al nuevo elemento independiente.
//Hay que indicar que el corte proviene de algún sitio!!!!!!!!!
CtI WdChop::patCutRight(CtI p)
{

	CtI q = p;
	CHAR patindep[]="X";
	pCHAR patnew;
	INT len, i;

	len = strlen(ct(p).pattern);

	for(i=0; i < (len - 1); i++)
		q = ct.next(q); /* Apuntamos al futuro independiente */

	patindep[0] = (ct(p).pattern)[len - 1];
	patnew = strdup(ct(p).pattern);
	patnew[len - 1] = '\x0';

	ct.setPattern(p, patnew);
	ct.setStart(q, TRUE); /* Nueva Celda independiente del grupo */
	ct.setPattern(q, patindep);
	ct.setRightFrg(q);

	free(patnew);

	return q;

}

/***********************************************************************/


/***********************************************************************/

BOOL WdChop::hasLeftPuntt(pCHAR pattern)
{
	if (!pattern) return FALSE;

	if ((pattern[0] == SYCHTYPE_PUNTT) && (strlen(pattern) > 1)) return TRUE;

	return FALSE;

}

/***********************************************************************/


/***********************************************************************/

BOOL WdChop::hasRightPuntt(pCHAR pattern)
{
	INT len;

	if (!pattern) return FALSE;

	len = strlen(pattern);
	if ((pattern[len - 1] == SYCHTYPE_PUNTT) && (len > 1)) return TRUE;

	return FALSE;

}

/***********************************************************************/
/**********************************************************/
//Une dos cadenas en una sola.
//libera ademas la cadena "to"
//devuelve la nueva "to" a la que se le a¤adir n m s caracteres
//si procede.
//el que llama a esta funci¢n es el encargado de liberar esta cadena.

pCHAR WdChop::concatStr(pCHAR to, pCHAR from)
{
  INT len;
  pCHAR temp;

#ifdef HTTS_DEBUG
  if (!to) htts_error("TextToList::concatStr() - Null \"to\" String");
	if (!from) htts_error("TextToList::concatStr() - Null \"from\" String");
#endif

  len = strlen(to) + strlen(from) + 1; // El +1 para el 0 de fin de cadena.
  temp = (pCHAR)malloc(len * sizeof(CHAR));

  strcpy(temp, to);
  strcat(temp, from);

  free(to);

  return temp;
}


/***********************************************************************/

CHAR WdChop::getpatsym(INT chtype)
{
	CHAR symbol;
	switch (chtype)
	{
		case CHTYPE_NULL:
			symbol = SYCHTYPE_NONE;
			break;
		case CHTYPE_DELIM:
			symbol = SYCHTYPE_DELIM;
			break;
		case CHTYPE_PUNTT:
			symbol = SYCHTYPE_PUNTT;
			break;
		case CHTYPE_SYMBL:
			symbol = SYCHTYPE_SYMBL;
			break;
		case CHTYPE_DIGIT:
			symbol = SYCHTYPE_DIGIT;
			break;
		case CHTYPE_LETTR:
			symbol = SYCHTYPE_LETTR;
			break;
		default:
			symbol = SYCHTYPE_NONE;
			break;
	}

	return symbol;

}

/***********************************************************************/


/***********************************************************************/

pCHAR WdChop::strChType(INT chtype)
{
	static CHAR strType[80];

	switch (chtype)	{
		case CHTYPE_NULL:
			sprintf(strType, "NULL");
			break;
		case CHTYPE_DELIM:
			sprintf(strType, "delimitador");
			break;
		case CHTYPE_PUNTT:
			sprintf(strType, "signo puntuación");
			break;
		case CHTYPE_SYMBL:
			sprintf(strType, "símbolo");
			break;
		case CHTYPE_DIGIT:
			sprintf(strType, "número");
			break;
		case CHTYPE_LETTR:
			sprintf(strType, "letra");
			break;
		default:
			sprintf(strType, "!!!No es nada!!!");
			break;
	}

	return strType;
}

/***********************************************************************/
