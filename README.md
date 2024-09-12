# XtalTransforms
Code that will perform crystallographic transformations on coordinates. Uses
data from a RCSB PDB entry and coordinates in the CHARMM coordinate file. This
code has NOT been thoroughly tested and will likely not work for all PDB
structures.

## Generate Coordinates for Unit Cell and Super Cell
0.) Configure
* Adjust the output file names in the [driver file](test/Transform.cpp)

1.) Compile
```
./compile.sh
```
OR
```
mkdir build
cd build/
cmake ..
make -j
```

2.) Run the code
* For this example you can use the following command (NOTE: This should be run
outside of the build/ directory i.e. `cd ../`)

```
./bin/Transform ./data/4oph.pdb
```
* This will output two PDB coordinate files for the unit cell and the super cell
in the `data/` directory

3.) Create a CHARMM PSF and coordinate file for the unit and super cells
* This step is only if you need a CHARMM PSF and coordinate file

```
cd data/
charmm < build_4oph_unit.inp > out_build_4oph_unit.out
charmm < build_4oph_super.inp > out_build_4oph_super.out
```

4.) View the unit cell and super cell in VMD
```
vmd -e view_4oph_unit.vmd
vmd -e view_4oph_super.vmd
```
