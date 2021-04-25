#/bin/bash

FILE=vendor/ppsspp/build
if [ -f $FILE ]; then
   echo "creating spritesheet"
   cd vendor/ppsspp
   cd build 
   make -j$(cat /proc/cpuinfo | grep -c vendor_id)
else
   echo "creating atlas tool and spritesheet"
   cd vendor/ppsspp
   mkdir -p build
   cd build 
   cmake .. 
   make -j$(cat /proc/cpuinfo | grep -c vendor_id)
fi
