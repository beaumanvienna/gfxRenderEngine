#/bin/bash


FILE=vendor/glew/lib/libGLEW.a
if [ -f $FILE ]; then
   echo "File $FILE exists"
else
   echo "building glew"
   cd vendor/glew
   make -j$(cat /proc/cpuinfo | grep -c vendor_id)
fi
