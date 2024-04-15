#!/bin/bash

FOLDER=$(pwd)

result=${PWD##*/}          # to assign to a variable
result=${result:-/}

cleanString="clean"

if [ "$1" == "$cleanString" ]
then
	echo "$FOLDER"
	cd $FOLDER
	cd "./Builds/LinuxMakefile"
	make clean
fi

echo "$FOLDER"
cd $FOLDER
cd "./Builds/LinuxMakefile"
make
cd "./build"
./"$result"
