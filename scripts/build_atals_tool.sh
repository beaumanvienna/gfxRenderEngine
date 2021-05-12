#/bin/bash

FILE=vendor/atlas/bin
if [ -f $FILE ]; then
   echo "File $FILE exists"
else
   echo "building sfml"
   cd vendor/atlas
   premake5 gmake2
   export MAKEFLAGS=-j8
   make 
fi
