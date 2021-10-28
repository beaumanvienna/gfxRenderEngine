#/bin/bash

FILE=vendor/sdl/build/.libs/libSDL2.a
if [ -f $FILE ]; then
   echo "File $FILE exists"
   cd vendor/sdl
else
   echo "building sdl"
   cd vendor/sdl
   git update-index --assume-unchanged include/SDL_config.h include/SDL_revision.h
   ./configure  --disable-shared --disable-jack
fi
make -j$(cat /proc/cpuinfo | grep -c vendor_id)
