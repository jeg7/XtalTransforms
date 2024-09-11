#!/bin/bash

set -e

# Prepare
find . -name '*~' -delete
find . -name '*.*~' -delete

git submodule update --init --remote --recursive

mkdir -p build
cd build/

# Configure
echo ""
echo " Release (1)"
echo "   Debug (2)"
echo ""
echo -n " Which build configuration would you like to use? "
read -n 1 a
echo ""
case $a in
    1*)
	echo "cmake -DCMAKE_BUILD_TYPE=Release .."
	cmake -DCMAKE_BUILD_TYPE=Release ..
	;;
    2*)
	echo "cmake -DCMAKE_BUILD_TYPE=Debug .."
	cmake -DCMAKE_BUILD_TYPE=Debug ..
	;;
    * ) # Default
	echo " Invalid input, no building being done."
	;;
esac
echo ""

# Compile
if [ "$#" -ne 1 ]; then
    make -j
elif [ "$1" -le 1 ]; then
    make -j
else
    make -j$1
fi
