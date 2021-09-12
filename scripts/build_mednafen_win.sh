#/bin/bash

FILE=emulators/mednafen/build/Makefile
if [ -f $FILE ]; then
    echo "make mednafen"
    cd emulators/mednafen/build
else
    echo "building mednafen"
    cd emulators/mednafen/
    aclocal && autoconf && automake --add-missing --foreign
    rm -rf build
    mkdir build
    cd build
    ../configure --disable-nls
    make verbose=1 -j$(cat /proc/cpuinfo | grep -c vendor_id)
fi
