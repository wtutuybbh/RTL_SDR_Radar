#!/bin/bash
if [ $# -eq 0 ] 
then
       	printf "Автономная работа. Запуск в режиме подключения к серверу: \n startRadarApp.sh <ip server> <port server> <interval_ms_send_data_to_server>\n Например: ./startRaspberryApp.sh 127.0.0.1 60000 1000\n"
fi

cd ../../
CURRENT_DIR=$(pwd)/lib.linux/
LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${CURRENT_DIR}
export LD_LIBRARY_PATH

#echo $LD_LIBRARY_PATH

cd bin/debug/RadarApp
./RadarApp $1 $2 $3

