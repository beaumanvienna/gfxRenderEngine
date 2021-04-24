#/bin/bash

FILE=vendor/glfw/build/src/libglfw3.a
if [ -f $FILE ]; then
   echo "File $FILE exists"
else
   echo "building glfw"
   mkdir -p vendor/glfw/build
   cd vendor/glfw/build
   cmake .. 
   make -j$(cat /proc/cpuinfo | grep -c vendor_id)
fi
