// BEGINLICENSE
//
// This file is part of ResidueSolventEnergy, which is distributed under the BSD
// 3-clause license, as described in the LICENSE file in the top level directory
// of this project.
//
// Author: James E. Gonzales II
//
// ENDLICENSE

#include "XtalTransformer.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

XtalTransformer::XtalTransformer(void)
    : m_UnitCellDimensions(), m_DimensionA(0.0), m_DimensionB(0.0),
      m_DimensionC(0.0), m_AngleAlpha(0.0), m_AngleBeta(0.0), m_AngleGamma(0.0),
      m_NumXtalOperations(0), m_UnitCellRotations(), m_UnitCellTranslations(),
      m_NumAtoms(0), m_TotNumAtoms(0), m_Atoms() {}

XtalTransformer::XtalTransformer(const std::string &pdbFileName)
    : XtalTransformer() {
  this->readCrystalTransformationsFromPdbFile(pdbFileName);
  this->readCrystalDimensionsFromPdbFile(pdbFileName);
  this->readCoordinatesFromPdbFile(pdbFileName);
}

void XtalTransformer::applyTransformations(void) {
  this->duplicateAtoms();
  for (std::size_t oper = 0; oper < m_NumXtalOperations; oper++) {
    for (std::size_t i = 0; i < m_NumAtoms; i++) {
      const std::size_t idx = oper * m_NumAtoms + i;
      this->applyTransformation(
          m_Atoms[idx].getCoordinateX(), m_Atoms[idx].getCoordinateY(),
          m_Atoms[idx].getCoordinateZ(), m_UnitCellRotations[oper],
          m_UnitCellTranslations[oper]);
    }
  }

  return;
}

void XtalTransformer::writePdbFile(const std::string &pdbFileName) {
  std::ofstream fout(pdbFileName);

  for (std::size_t i = 0; i < m_TotNumAtoms; i++) {
    fout << "ATOM " << std::setw(6) << m_Atoms[i].getIndex() << "  "
         << std::left << std::setw(3) << m_Atoms[i].getLabel() << std::right
         << " " << std::setw(3) << m_Atoms[i].getResidueName() << " "
         << std::setw(1) << m_Atoms[i].getChain() << " " << std::setw(3)
         << m_Atoms[i].getResidueIdentifier() << "    " << std::setw(8)
         << std::fixed << std::setprecision(3) << m_Atoms[i].getCoordinateX()
         << std::setw(8) << std::fixed << std::setprecision(3)
         << m_Atoms[i].getCoordinateY() << std::setw(8) << std::fixed
         << std::setprecision(3) << m_Atoms[i].getCoordinateZ() << "  "
         << std::setw(3) << std::fixed << std::setprecision(2)
         << m_Atoms[i].getOccupancy() << " " << std::setw(5) << std::fixed
         << std::setprecision(2) << m_Atoms[i].getTemperatureFactor()
         << "           " << std::setw(1) << m_Atoms[i].getType() << "\n";
  }
  fout << "TER\n";

  return;
}

void XtalTransformer::applyTransformation(double &x, double &y, double &z,
                                          const std::array<double, 9> &rot,
                                          const std::array<double, 3> &trn) {
  const double x0 = x;
  const double y0 = y;
  const double z0 = z;
  x = rot[0 * 3 + 0] * x0 + rot[0 * 3 + 1] * y0 + rot[0 * 3 + 2] * z0 + trn[0];
  y = rot[1 * 3 + 0] * x0 + rot[1 * 3 + 1] * y0 + rot[1 * 3 + 2] * z0 + trn[1];
  z = rot[2 * 3 + 0] * x0 + rot[2 * 3 + 1] * y0 + rot[2 * 3 + 2] * z0 + trn[2];

  return;
}

void XtalTransformer::duplicateAtoms(void) {
  m_TotNumAtoms = m_NumXtalOperations * m_NumAtoms;
  m_Atoms.resize(m_TotNumAtoms);
  for (std::size_t oper = 0; oper < m_NumXtalOperations; oper++) {
    for (std::size_t i = 0; i < m_NumAtoms; i++) {
      const std::size_t j = oper * m_NumAtoms + i;
      m_Atoms[j] = m_Atoms[i];
      m_Atoms[j].setIndex(m_Atoms[i].getIndex() + j);
    }
  }

  return;
}

void XtalTransformer::readCrystalTransformationsFromPdbFile(
    const std::string &pdbFileName) {
  std::ifstream file(pdbFileName);
  if (!file.is_open())
    throw std::runtime_error("Could not open file \"" + pdbFileName + "\"");

  while (!file.eof()) {
    std::string line = "";
    std::getline(file, line);
    std::stringstream ss(line);
    std::string token = "";
    ss >> token;
    if (token == "REMARK") {
      token.clear();
      ss >> token;
      if (token == "290") {
        token.clear();
        ss >> token;
        if (token == "SMTRY1") {
          std::array<double, 9> rotation = {0.0, 0.0, 0.0, 0.0, 0.0,
                                            0.0, 0.0, 0.0, 0.0};
          std::array<double, 3> translation = {0.0, 0.0, 0.0};
          int symIdx = -1;
          ss >> symIdx >> rotation[0] >> rotation[1] >> rotation[2] >>
              translation[0];
          line.clear();
          std::getline(file, line);
          ss.clear();
          ss.str(line);
          ss >> token >> token >> token >> symIdx >> rotation[3] >>
              rotation[4] >> rotation[5] >> translation[1];
          line.clear();
          std::getline(file, line);
          ss.clear();
          ss.str(line);
          ss >> token >> token >> token >> symIdx >> rotation[6] >>
              rotation[7] >> rotation[8] >> translation[2];
          m_UnitCellRotations.push_back(rotation);
          m_UnitCellTranslations.push_back(translation);
          m_NumXtalOperations++;
        }
      }
    }
  }

  std::cout << "No. of crystallographic transformations to produce unit cell: "
            << m_NumXtalOperations << std::endl;

  return;
}

void XtalTransformer::readCrystalDimensionsFromPdbFile(
    const std::string &pdbFileName) {
  std::ifstream file(pdbFileName);
  if (!file.is_open())
    throw std::runtime_error("Could not open file \"" + pdbFileName + "\"");

  while (!file.eof()) {
    std::string line = "";
    std::getline(file, line);
    std::stringstream ss(line);
    std::string token = "";
    ss >> token;
    if (token == "CRYST1") {
      ss >> m_DimensionA >> m_DimensionB >> m_DimensionC >> m_AngleAlpha >>
          m_AngleBeta >> m_AngleGamma;
    }
  }

  if ((m_DimensionA == 0.0) || (m_DimensionB == 0.0) || (m_DimensionC == 0.0) ||
      (m_AngleAlpha == 0.0) || (m_AngleBeta == 0.0) || (m_AngleGamma == 0.0))
    throw std::runtime_error("Could not read crystal dimensions from PDB file");

  std::cout << "Crystal dimensions: " << m_DimensionA << ", " << m_DimensionB
            << ", " << m_DimensionC << "; " << m_AngleAlpha << ", "
            << m_AngleBeta << ", " << m_AngleGamma << std::endl;

  return;
}

void XtalTransformer::readCoordinatesFromPdbFile(
    const std::string &pdbFileName) {
  std::ifstream file(pdbFileName);
  if (!file.is_open())
    throw std::runtime_error("Could not open file \"" + pdbFileName + "\"");

  while (!file.eof()) {
    std::string line = "";
    std::getline(file, line);
    std::stringstream ss(line);
    std::string token = "";
    ss >> token;
    if (token == "ATOM") {
      int idx = -1;
      std::string albl = "", resn = "", chain = "", resi = "", atype = "";
      double x = 0.0, y = 0.0, z = 0.0, occ = 0.0, temp = 0.0;
      ss >> idx >> albl >> resn >> chain >> resi >> x >> y >> z >> occ >>
          temp >> atype;
      m_Atoms.emplace_back(
          Atom(idx, albl, resn, chain, resi, x, y, z, occ, temp, atype));
      m_NumAtoms++;
    }
  }
  m_TotNumAtoms = m_NumAtoms;

  std::cout << "Read coordinates for " << m_NumAtoms << " atoms" << std::endl;

  return;
}
