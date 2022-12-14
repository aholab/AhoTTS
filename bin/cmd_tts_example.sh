#!/bin/bash
function bname(){
	basename $1 | cut -f1 -d.
}

# de los textos. 
DIRTXT=./txt
#de los wavs
DIRWAV=./wav

for i in $DIRTXT/*.txt;do
	echo $i
	f=`bname $i`
	echo $f
	echo 'Speed='$1
    	cat $i | iconv -f UTF-8 -t WINDOWS-1252 >input.txt

	./tts -Speed=$1 -Lang=eu 
	
	mv Output.wav $DIRWAV/$f.$1.wav

done
