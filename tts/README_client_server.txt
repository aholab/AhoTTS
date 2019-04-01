Aholab Signal Processing Laboratory
University of the Basque Country
---------------------------------

Both programs, tts_server (server) and tts_client (client), are for Linux systems.


/********************************************/
tts_server
/********************************************/

First of all the server must be started up. This program will listen to one port of the machine. In case you want to use more than one port to run different processes, one for each port. Also, if you want to use more than one IP address in the same machine start up a different process for each address. 

Usage of tts_server
	./tts_server -IP="value" -Port="value" -DataPath="value"
	Parameters:
		IP: IPv4 address of the server. Default value: none
		Port: TCP port for the service. The value must be between 1024 and 65535, well known ports aren't allowed. Default value: none
		DataPath: Path where the libraries, voices, dictionaries and other important files are stored. Default value: current directory
	
	For each request the server will save the received text file and a copy of the audio file sent. Both files will share the name, each one with the respective extension. This name is the date and hour of the request in the following format:
		Www Mmm dd hh:mm:ss yyyy
	Where Www is the weekday, Mmm the month in letters, dd the day of the month, hh:mm:ss the time, and yyyy the year. 


/********************************************/
tts_client
/********************************************/

Once the server process is running you can use the client to send requests from anywhere. You only need to know the IP address and the port the server is listening to.
	
Usage of tts_client
	./tts_client -InputFile="value" -OuputFile="value" -Lang="value" -Speed="value" -IP="value" -Port="value"
	Parameters:
		InputFile: File name, with extension, of plain text coded in ISO-8859-15. Default name: input.txt
		OutputFile: Name of the audio file with the text synthesized. Default value: output.wav
		Lang: Language to synthesize the file text. Default value: eu
			es -> Spanish
			eu -> Basque
		Speed: numeric value between 25 and 300 indicating the speed of lecture, with 100 being the normal rate. The higher the value the higher the speed of lecture. Default value is 100
		IP: IPv4 address of the server. Default value: none
		Port: TCP port the server is listening to. Default value: none
	
	In tts_client program you can omit the parameters with default value, but neither the IP nor the Port.

---------------------------------
