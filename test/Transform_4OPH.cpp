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

#include <iostream>
#include <memory>

int main(int argc, char **argv) {
  if (argc != 1) {
    std::cerr << "Usage: " << argv[0] << std::endl;
    return 1;
  }

  auto xtalTransformer = std::make_shared<XtalTransformer>("./data/4oph.pdb");

  xtalTransformer->writeXtalFile("./data/4oph_xtal");

  xtalTransformer->generateUnitCell();
  xtalTransformer->writePdbFile("./data/4oph_unit.pdb");

  xtalTransformer->generateSuperCell();
  xtalTransformer->writePdbFile("./data/4oph_super.pdb");

  return 0;
}
