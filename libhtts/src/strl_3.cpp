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
Copyright: 1996 - Grupo de Voz (DET) ETSII/IT-Bilbao

Nombre fuente................ STRL.CPP
Nombre paquete............... -
Lenguaje fuente.............. C++
Estado....................... Completado
Dependencia Hard/OS.......... -
Codigo condicional........... -

Codificacion................. Borja Etxebarria

Version  dd/mm/aa  Autor     Comentario
-------  --------  --------  ----------
1.2.6    2012	   Inaki     eliminar dependencia de xalloc
1.2.5    30/08/98  Borja     split en varios modulos strl_?.cpp

======================== Contenido ========================
Ver doc. abajo.
===========================================================
*/
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
/**********************************************************/

#include "strl.hpp"
#include "clargs.h"
#include "uti.h"

// para usar isatty()
#ifdef __OS_UNIX__
#include <unistd.h>
#else
#include <io.h>
#endif

/**********************************************************/

/*<DOC>*/
/**********************************************************/
/* Analiza la linea de comandos (argc, argv) de un programa.

{argc} y {argv} son los argumentos de main(), (incluido el argv[0] !).
Los argumentos a main pueden ser de dos tipos:

- switches: empiezan por "-" ( o por "/" en msdos), y pueden tener
valor, por ejemplo "-SRate=8000".
- files: el resto.

El switch a main "-CfgFile=<fichero>" se trata de manera especial:
se procesa <fichero> como lista de argumentos de configuracion
(es un fichero de texto con un argumento, switch o no switch, por
linea), con preferencia sobre cualquier otro argumento a main. Si hay
varios -CfgFile solo se procesa el ultimo. Si no hay ningun -CfgFile
se utiliza un fichero por defecto, que se busca primero en el
directorio actual y si no esta ahi, en el del programa ejecutable.
Tambies se procesan los switches "-ArgFile=<fichero>". Se procesa
<fichero> como lista de argumentos cuando le toque.
Puede haber -ArgFile dentro de un fichero -CfgFile, pero no al reves
(se ignorar n).
Hay otros switches estandar (como -Help) que se procesan o no en
funcion de como se configure esta funcion clargs2props(), y ademas
todos los switches que defina el usuario.

Esta funcion recibe dos cadenas, una de formato con los switches
permitidos, y otra con opciones de funcionamiento.

Parametros:

- int argc: el numero de argumentos (parametro {argc} pasado a main()).

- char *argv[]: argumentos de linea (parametro {argv} pasado a main()).

- KVStrList &props: se rellena con los switches y su valor. Se pueden
enviar inicialimente valores por defecto para los switches que se
deseen.

- StrList &files: se rellena con los ficheros (argumentos no switch).
Inicialmente debe estar vacio.

- const char *format: aqui se indica el formato  de la linea de comandos.
(descrito mas tarde).

- const char *options: aqui se indican opciones para clargs2props().
Las opciones posibles en la cadena {options} puede ser:
+ CLShow=yes/no  [por defecto a yes]: para activar el switch -CLShow
que imprime la linea de comandos (debugeo).
+ Help=yes/no  [yes]: para activar el switch -Help que genera
una ayuda a partir del formato enviado en {format}.
+ HelpExit=yes/no [yes]: para que despues de imprimir la ayuda
tras recibir un argumento -Help termine el programa.
+ HelpSort=yes/no [yes]: para ordenar alfabeticamente los switches
al imprimir la ayuda con -Help.
+ Files: para indicar que se pueden recibir varios argumentos no
switch. En caso de que tambien se indique Dest o OpDest, el ultimo
argumento no switch se considera Dest.
+ File: para indicar que se debe recibir solo un argumento no switch.
+ Dest: para indicar que se debe recibir un segundo argumento no switch
(al que consideramos fichero de destino).
+ OpDest: para indicar que se puede recibir un segundo argumento no switch
(al que consideramos fichero de destino), pero no es obligatorio.
+ MyFiles=xxx: para indicar que no se analizen los argumentos no
switch (se ignoran File, Files, y Dest). La cadena xxx enviada se
utiliza para imprimir la ayuda.
+ DefCfgFile=fname: para indicar el nombre por defecto del fichero
de configuracion. Si no se indica, por defecto es el nombre del
ejecutable y extension ".cfg".
+ NoDefCfgFile=yes/no [no]: Si se pone a Yes, se deshabilita el
fichero de configuracion por defecto.
+ HelpPause=yes/no [no]: Si se pone a Yes, hace una pausa
despues de mostrar la ayuda.
+ NoScrollPause=yes/no [no]: Si se pone a Yes, deshabilita la
pausa de pantalla al mostrar ayuda cuando las opciones no entran
en una sola pantalla.
+ Version=xxx/no: Si se envia una cadena distinta de "no", se activa
el switch -Version y cuando se envie a main, se imprime la cadena "xxx"
configurada con esta opcion. Tambien se imprime la fecha de compilacion
del programa.
+ Description=xxx: breve descripcion de lo que hace el programa. Se
imprimira al mostrar ayuda (con -Help). Para poder poner varias
palabras, se debe enstd::cerrar la cadena entre comillas dobles (utilizando
caracter de escape. Ej:  "Description=\"Programa muy bonito.\" ").
+ EraseAuto=yes/no [yes] : Si se deben borrar o no de la linea de
comando los switches autoprocesados (CLShow, Help, Version, etc).
Si es "yes", se borran y aunque el usuario los utilice, no aparecen
en {props}. Si es "no", apareceran en {props} si el usuario del
programa utiliza el switch en cuestion.

El formato de los switches del usuario se envian en la cadena {format}
donde se debe indicar "switch1=tipo switch2=tipo switch3  etc.", siendo
"switch1" el switch sin el guion, y el tipo (si aparece) indica la clase
de valores que puede tomar el switch, que puede ser uno de los siguientes:
i -> para indicar que el valor es un entero
l -> para indicar que el valor es un long
f -> un float
d -> un double
s -> una cadena (o sea cualquier cosa)
{a|b|c}  -> las cadenas "a" o "b" o "c".
Tambien puede enviarse un switch que no tenga valor (el switch3 en
el ejemplo anterior). Los switches booleanos no necesitan valor,
si aparecen se consideran TRUE aunque no tengan valor.
Si en {format} se envia "*" se acepta cualquier switch que no este
definido. Si no se envia el asterisco, cualquier switch no definido
en {format} provoca un error.
*/

VOID clargs2props( int argc, char *argv[], KVStrList &props,
	StrList &files, const char *format, const char *options, const char*date )
/*</DOC>*/
{
	INT i,nsw;
	KVStrList opts(options);
	KVStrList fmt(format);
	Lix p;
	clasw *cl;
	char *val, *df;
	char **sw;
	BOOL any, clshow, help, version;

	fmt.remove_prefix("-");
	p=fmt.seek("*"); any=(p!=NULL); if (any) fmt.del(p);
	clshow=opts.bval("CLShow",TRUE);
	help=opts.bval("Help",TRUE);
	version=strcmp(opts.val("Version","?"),"no");
	if (clshow) fmt.add("CLShow","b"); // si esta permitido el swith CLShow
	if (help) fmt.add("Help","b"); // si esta permitido el swith Help
	if (version) fmt.add("Version","b"); // si esta permitido el swith Version
	LONG nnsw=fmt.length();
	nsw = (INT)nnsw;
	cdie_beep(nsw!=nnsw,"clargs2props: too many entries (%ld) for this machine's word length",(long)nnsw);
	sw = (char**)malloc(sizeof(char*)*(nsw+1));
	for (i=0, p=fmt.first(); i<nsw; i++, p=fmt.next(p))
		sw[i] = strdup((const char *)(fmt.itemkey(p)));
	sw[nsw]=NULL;

	_clargs_case_sensitive=1;
	df=NULL;
	if (opts.contains("DefCfgFile")) df = strdup(opts.val("DefCfgFile"));  // fich. config.
	else if (! opts.bval("NoDefCfgFile",FALSE)) { 
		df=path_src2destp(argv[0]+path_namepos(argv[0]),".cfg","");  // auto fich. config.
	}

  cl = clasw_construct(argc, argv, (const char **)sw, df );

	clasw_restart(cl);
	while (1) {
		String k;
		i=clasw_getsw(cl,&val,any);
		if (i==-1) break;
		if (i>=0) {
			p=fmt.seek(sw[i]);
			k=fmt.itemkey(p);
			if (fmt.itemval(p).length()==0) {
				cdie_beep(*val!=0,"Error: command line arg doesn't need value (-%s=%s)",(const char*)k,val);
				props.add(k,"");
			}
			else {
				if (((const char *)fmt.itemval(p))[0]=='b') // bool es un tanto especial
          props.add(k,props.tob(k,val));
				else {
					cdie_beep(*val==0,"Error: incomplete command line arg (-%s=???)",(const char *)k);
					switch (((const char *)fmt.itemval(p))[0]) {
					case 'i': props.add(k,props.toi(k,val)); break;
					case 'l': props.add(k,props.tol(k,val)); break;
					case 'f': props.add(k,props.tof(k,val)); break;
					case 'd': props.add(k,props.tod(k,val)); break;
					case 's': props.add(k,val); break;
					case '{': props.add(k,val); break;   /*}*/
					default: die_beep("Error: unknown command line switch type (%c)",((const char *)fmt.itemval(p))[0]);
					}
				}
			}
		}
		else {  // any=TRUE  (*)  and unknown found
			char *ss=strchr(val,'=');
			cdie_beep(ss==val,"Error: invalid command line arg (-%s)",val);
			if (!ss)	props.add(val,"");
			else { String k(val,(int)(ss-val)); props.add(k,ss+1); }
		}
    
		if (help && props.bval("Help",FALSE) && opts.bval("HelpExit",TRUE)) break;
	}

	if (version) {
		if (props.bval("Version",FALSE)) {
			std::cerr << "Version " << opts.val("Version","?") << " compiled "
					<< date << std::endl;
		}
		if (opts.bval("EraseAuto",TRUE)) props.erase("Version");
	}

	if (help) {
		if (props.bval("Help",FALSE)) {
			if (opts.contains("Description"))
				std::cerr << opts.val("Description") << std::endl;
			std::cerr << "Usage: " << argv[0] << " [-switches]";
			if (opts.contains("MyFiles")) std::cerr << " " << opts.val("MyFiles");
			if (opts.contains("Files")) std::cerr << " <files>";
			else if (opts.contains("File")) std::cerr << " <file>";
			if (opts.contains("Dest")) std::cerr << " <dest>";
			else if (opts.contains("OpDest")) std::cerr << " [<dest>]";
			std::cerr << std::endl << std::endl;
			if (opts.bval("HelpSort",TRUE)) fmt.sort();
			INT lines=0;
			for (Lix q=fmt.first(); q!=0; q=fmt.next(q)) {
				std::cerr << "   -"<< fmt.itemkey(q);
				switch (((const char *)fmt.itemval(q))[0]) {
					case 'i': std::cerr << "=<integer>"; break;
					case 'l': std::cerr << "=<long integer>"; break;
					case 'f': std::cerr << "=<float>"; break;
					case 'd': std::cerr << "=<double float>"; break;
					case 'b': std::cerr << "=<boolean>"; break;
					case 's': std::cerr << "=<string>"; break;
					case '{': std::cerr << "=" << fmt.itemval(q); break;  /* } */
					case '\0': break;
					default: die_beep("Error: unknown command line switch type (%c)",((const char *)fmt.itemval(q))[0]);
				}
				std::cerr << std::endl;
				lines++;
				if ((lines>=15)&&(!opts.bval("NoScrollPause",FALSE))&&isatty(2)) {
					lines=0;
					fprintf(stderr,  "-- Press ENTER key to continue ---");
					getchar();
				}
			}
			std::cerr << "   -CfgFile=<fname>" << std::endl;
			std::cerr << "   -ArgFile=<fname>" << std::endl;

			if (any) std::cerr << "   -???=???"<< std::endl;
			std::cerr << "----" << std::endl;
			if (opts.bval("HelpPause",FALSE)&&isatty(2)) {
				fprintf(stderr,  "-- Press ENTER key to continue ---");
				getchar();
			}
			if (opts.bval("HelpExit",TRUE)) exit(0);
		}
		if (opts.bval("EraseAuto",TRUE)) props.erase("Help");

	}

	clasw_restart(cl);
	while ((val=clasw_getparg(cl))!=NULL) files.append(val);

	clasw_destruct(cl);
	for (i=0; i<nsw; i++) free(sw[i]);
	free(sw);
	if (df) free(df);

	if (clshow) {
		if (props.bval("CLShow",FALSE)) {
			if (opts.bval("EraseAuto",TRUE)) props.erase("CLShow");
			std::cerr << "Command Line flags:\n" << props
					<< "Command Line files:\n" << files << "----\n";
		}
		if (opts.bval("EraseAuto",TRUE)) props.erase("CLShow");
	}

	const char *hmsg= (help)? ", use -Help to get help":"";
	if (opts.contains("MyFiles")) {
	}
	else if (opts.contains("Files")) {
		cdie_beep(files.length()<1,"Error: must specify one or more files%s.",hmsg);
		cdie_beep(opts.contains("Dest") && (files.length()<2),"Error: must specify dest%s.",hmsg);
	}
	else if (opts.contains("File")) {
		cdie_beep(files.length()<1,"Error: must specify one file%s.",hmsg);
		cdie_beep( (files.length()>2) ||
			(!(opts.contains("Dest")||opts.contains("OpDest")) && (files.length()==2)),
			"Error: must specify only one file%s.",hmsg);
		cdie_beep( (files.length()==1) && opts.contains("Dest"),
			"Error: must specify one dest%s.",hmsg);
	}
	else if (opts.contains("Dest")) {
		cdie_beep(files.length()<1,"Error: must specify one dest%s.",hmsg);
		cdie_beep(files.length()>1,"Error: must specify only one dest%s.",hmsg);
	} 
	else {
		cdie_beep(files.length()>0,"Error: commandline files are not allowed.");
	}
}

/**********************************************************/
