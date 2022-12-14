Text-to-Speech conversor for Basque and Spanish

----------

LICENSE: Read COPYRIGHT_and_LICENSE_code.txt and COPYRIGHT_and_LICENSE_voices.txt

INSTALATION (in linux environments)
	sh script_compile_all_linux.sh

FOLDERS:
	libhtts: code of the library
	tts: code of examples using the library
	lib: where the library is stored
	bin: where example binaries are installed
	bin/data_tts: all the necessary data files for each language
	
The instalation script compiles two versions of AhoTTS: a command line executable (bin/tts with an example of its use in bin/cmd_tts_example.sh) and a client-server version (bin/tts_client & bin/tts_server).
Input text should be in WINDOWS-1252 encoding
