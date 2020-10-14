#!/bin/bash

NOW=$(date)
CURRENT_DIR=$(pwd)
echo "[$NOW] Start builder"

cd $CURRENT_DIR
(($? != 0)) && { echo "[$NOW] Error switch directory"; exit 1; }

cd ../../
(($? != 0)) && { echo "[$NOW] Error switch directory"; exit 1; }

make clean && rm ./.qmake.stash
			
qmake CONFIG+=release RTL_SDR_Radar.pro
if [ $? -ne 0 ]; then
	/usr/lib/qt5/bin/qmake CONFIG+=release RTL_SDR_Radar.pro
fi	
(($? != 0)) && { echo "[$NOW] Error prepare qmake project"; exit 1; }

make -j4
(($? != 0)) && { echo "[$NOW] Error build project"; exit 1; }

echo "[$NOW] Done"
