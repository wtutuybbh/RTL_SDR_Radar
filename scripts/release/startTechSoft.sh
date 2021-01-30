#!/bin/bash

cd ../../
CURRENT_DIR=$(pwd)/lib.linux/
LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${CURRENT_DIR}
export LD_LIBRARY_PATH

echo $LD_LIBRARY_PATH

cd bin/release/TechnologicSoft
nohup ./TechnologicSoft  > /dev/null 

