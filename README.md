# XtalTransforms
Code that will perform crystallographic transformations on coordinates. Uses
data from a RCSB PDB entry and coordinates in the CHARMM coordinate file. This
code has NOT been thoroughly tested and will likely not work for all PDB
structures.

## Generate Coordinates for Unit Cell and Super Cell
0.) Configure
* Adjust the output file names in the [driver file](test/Transform_4OPH.cpp)

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
./bin/Transform_4OPH
```
* This will output a CHARMM formatted PSF and coordinate file for visualizing
the unit crystal
* This will also output two PDB coordinate files for the unit cell and the super
cell
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

## Generate Coordinates for Unit Cell, Super Cell and Perform Arbitrary
Transformations
0.) Configure
* Adjust the output file names in the [driver file](test/Transform_2Y2A.cpp)

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
./bin/Transform_2Y2A
```
* This will output a CHARMM formatted PSF and coordinate file for visualizing
the unit crystal
* This will also output three PDB coordinate files for the unit cell and the
super cell
in the `data/` directory

3.) Create a CHARMM PSF and coordinate file for the unit and super cells and
stacked unit cell
* This step is only if you need a CHARMM PSF and coordinate file

```
cd data/
charmm < build_2y2a_unit.inp > out_build_2y2a_unit.out
charmm < build_2y2a_super.inp > out_build_2y2a_super.out
charmm < build_2y2a_stack.inp > out_build_2y2a_stack.out
```

4.) View the unit cell and super cell in VMD
```
vmd -e view_2y2a_unit.vmd
vmd -e view_2y2a_super.vmd
vmd -e view_2y2a_stack.vmd
```
