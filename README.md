# XtalTransforms
Code that will perform crystallographic transformations on coordinates. Uses data from a RCSB PDB entry and coordinates in the CHARMM coordinate file

## Unit Cell Generation
0.) Build individual molecule using CHARMM. You will need a PSF and CHARMM formatted coordinate file

1.) "Prepare" the system for performing crystallographic transformations
* Adjust the input file names and number of duplicate segments that you need to create to generate the entire unit cell
```
charmm < prep_xtal.inp > out/out_prep_xtal.out
```
* This should output the files `*_xtal.psf` and `*_xtal_prep.cor`
  * `*_xtal.psf` is the PSF that you will use for the unit cell
  * `*_xtal_prep.cor` should have however many segments you created copies of with the same coordinates that were built using CHARMM (i.e. they should all be overlapping)

2.) Perform crystallographic transformations to generate the unit cell
* Adjust the input file to specify the transformations you wish to perform on which segments
```
Some code here that runs our code
```

## Super Cell Generation
If creating a "super cell" (i.e. tiled unit cells) is desired, a similar process that was used to generate the unit cell cna be used to generate super cells

0.) "Prepare" the unit cell system for performing transformations
* Adjust the input file names and number of duplicate segments that you need to create to generate entire unit cells
```
charmm < prep_supercell.inp > out/out_prep_supercell.out
```
* This should output the files `*_supercell.psf` and `*_supercell_prep.cor`
  * `*_supercell.psf` is the PSF that you will use for the super cell
  * `*_supercell_prep.cor` should have however many segments you created copies of with the same coordinates that were generated when creating the unit cell (i.e. multiple unit cells overlapping exactly)

1.) Perform integer translations of the unit cell to generate the super cell
* Adjust the input file to specify the transformations you wish to perform on which segments
```
Some code here that runs our code
```
