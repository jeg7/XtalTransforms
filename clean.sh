#!/bin/bash

set -e

find . -name '*~' -delete
find . -name '*.*~' -delete

rm -rf gmon.out
rm -rf data/4oph_unit.pdb data/4oph_unit.psf data/4oph_unit.cor
rm -rf data/4oph_super.pdb data/4oph_super.psf data/4oph_super.cor

cd build/
make clean
cd ../
rm -rf build/ bin/
