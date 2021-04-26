#/bin/bash

FILE=vendor/atlas/bin
if [ -f $FILE ]; then
   echo "File $FILE exists"
else
   echo "building sfml"
   cd vendor/atlas
   premake5 gmake2
   make -j$(cat /proc/cpuinfo | grep -c vendor_id)
fi
