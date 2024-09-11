#!/bin/bash

set -e

find . -name '*~' -delete
find . -name '*.*~' -delete

rm -rf gmon.out tmp.pdb

cd build/
make clean
cd ../
rm -rf build/ bin/
