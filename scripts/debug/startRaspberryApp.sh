#!/bin/bash

if [ $# -eq 0 ] 
then
       	printf "[$NOW] Error usage.\n Usage: \n startRaspberryApp.sh <ip server> <port server> <interval_ms_send_data_to_server>\n Example: ./startRaspberryApp.sh 127.0.0.1 60000 1000\n"
	exit 0
fi

cd ../../
CURRENT_DIR=$(pwd)/lib.linux/
LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${CURRENT_DIR}
export LD_LIBRARY_PATH

echo $LD_LIBRARY_PATH

bin/debug/RaspberryApp/RaspberryApp $1 $2 $3

