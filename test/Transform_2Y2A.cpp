// BEGINLICENSE
//
// This file is part of ResidueSolventEnergy, which is distributed under the BSD
// 3-clause license, as described in the LICENSE file in the top level directory
// of this project.
//
// Author: James E. Gonzales II
//
// ENDLICENSE

#include <XtalTransformer.hpp>

// #include <array>
#include <iostream>
#include <memory>
// #include <vector>

int main(int argc, char **argv) {
  if (argc != 1) {
    std::cerr << "Usage: " << argv[0] << std::endl;
    return 1;
  }

  auto xtalTransformer = std::make_shared<XtalTransformer>("./data/2y2a.pdb");

  xtalTransformer->writeXtalFile("./data/2y2a_xtal");

  xtalTransformer->generateUnitCell();
  xtalTransformer->writePdbFile("./data/2y2a_unit.pdb");

  xtalTransformer->generateSuperCell();
  xtalTransformer->writePdbFile("./data/2y2a_super.pdb");

  std::vector<std::array<double, 9>> rotations = {
      {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0},
      {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0},
      {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0},
      {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0}};
  std::vector<std::array<double, 3>> translations = {
      {1.0, 0.0, 0.0}, {2.0, 0.0, 0.0}, {3.0, 0.0, 0.0}, {4.0, 0.0, 0.0}};
  xtalTransformer->transformUnitCellReplicasFractional(rotations, translations);
  xtalTransformer->writePdbFile("./data/2y2a_stack.pdb");

  return 0;
}
