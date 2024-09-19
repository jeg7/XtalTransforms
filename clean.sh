#!/bin/bash

set -e

find . -name '*~' -delete
find . -name '*.*~' -delete

rm -rf gmon.out
rm -rf data/4oph_xtal.psf data/4oph_xtal.cor
rm -rf data/4oph_unit.pdb data/4oph_unit.psf data/4oph_unit.cor
rm -rf data/4oph_super.pdb data/4oph_super.psf data/4oph_super.cor
rm -rf data/2y2a_xtal.psf data/2y2a_xtal.cor
rm -rf data/2y2a_unit.pdb data/2y2a_unit.psf data/2y2a_unit.cor
rm -rf data/2y2a_super.pdb data/2y2a_super.psf data/2y2a_super.cor
rm -rf data/2y2a_stack.pdb data/2y2a_stack.psf data/2y2a_stack.cor

cd build/
make clean
cd ../
rm -rf build/ bin/
