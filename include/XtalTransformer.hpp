// BEGINLICENSE
//
// This file is part of ResidueSolventEnergy, which is distributed under the BSD
// 3-clause license, as described in the LICENSE file in the top level directory
// of this project.
//
// Author: James E. Gonzales II
//
// ENDLICENSE

#pragma once

#include "Atom.hpp"

#include <array>
#include <string>
#include <vector>

class XtalTransformer {
public:
  XtalTransformer(void);
  XtalTransformer(const std::string &pdbFileName);

public:
  void generateUnitCell(void);
  void generateSuperCell(void);
  // void writeCharmmCoordinateFile(const std::string &fileName);
  void writePdbFile(const std::string &pdbFileName);

private:
  void applyTransformation(double &x, double &y, double &z,
                           const std::array<double, 9> &rot,
                           const std::array<double, 3> &trn);

private:
  void fractionalToCartesian(double &x, double &y, double &z, const double u,
                             const double v, const double w);
  void cartesianToFractional(double &u, double &v, double &w, const double x,
                             const double y, const double z);

private:
  void readCrystalTransformationsFromPdbFile(const std::string &pdbFileName);
  void readCrystalDimensionsFromPdbFile(const std::string &pdbFileName);
  void readCoordinatesFromPdbFile(const std::string &pdbFileName);

private:
  std::array<double, 6> m_UnitCellDimensions;
  std::size_t m_NumXtalOperations;
  std::vector<std::array<double, 9>> m_UnitCellRotations;
  std::vector<std::array<double, 3>> m_UnitCellTranslations;
  std::size_t m_NumAtoms;
  std::size_t m_TotNumAtoms;
  std::vector<Atom> m_Atoms;
  bool m_HasUnitCellBeenGenerated;
  bool m_HasSuperCellBeenGenerated;
};
