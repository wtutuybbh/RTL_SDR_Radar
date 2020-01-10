#!/bin/bash

if [ $# -eq 0 ] 
then
       	printf "[$NOW] Error usage.\n Usage: \n startRaspberryApp.sh <ip server> <port server> \n Example: ./startRaspberryApp.sh 127.0.0.1 60000\n"
	exit 0
fi

cd ../
CURRENT_DIR=$(pwd)/lib.linux/
LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${CURRENT_DIR}
export LD_LIBRARY_PATH

echo $LD_LIBRARY_PATH

cd bin/release/RaspberryApp
nohup ./RaspberryApp $1 $2 > /dev/null 2>&1& 

