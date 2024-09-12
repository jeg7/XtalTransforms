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

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

XtalTransformer::XtalTransformer(void)
    : m_UnitCellDimensions(), m_NumXtalOperations(0), m_UnitCellRotations(),
      m_UnitCellTranslations(), m_NumAtoms(0), m_TotNumAtoms(0), m_Atoms(),
      m_HasUnitCellBeenGenerated(false), m_HasSuperCellBeenGenerated(false) {}

XtalTransformer::XtalTransformer(const std::string &pdbFileName)
    : XtalTransformer() {
  this->readCrystalTransformationsFromPdbFile(pdbFileName);
  this->readCrystalDimensionsFromPdbFile(pdbFileName);
  this->readCoordinatesFromPdbFile(pdbFileName);
}

void XtalTransformer::generateUnitCell(void) {
  if (m_HasUnitCellBeenGenerated)
    return;

  m_HasUnitCellBeenGenerated = true;

  m_TotNumAtoms = m_NumXtalOperations * m_NumAtoms;
  m_Atoms.resize(m_TotNumAtoms);
  for (std::size_t oper = 0; oper < m_NumXtalOperations; oper++) {
    for (std::size_t i = 0; i < m_NumAtoms; i++) {
      const std::size_t j = oper * m_NumAtoms + i;
      m_Atoms[j] = m_Atoms[i];
      m_Atoms[j].setIndex(m_Atoms[i].getIndex() + j);
      std::string segi = std::string(m_Atoms[i].getChain());
      if (oper < 10)
        segi += ("00" + std::to_string(oper));
      else if (oper < 100)
        segi += ("0" + std::to_string(oper));
      else if (oper < 1000)
        segi += std::to_string(oper);
      else
        segi = "XXXX";
      m_Atoms[j].setSegmentIdentifier(segi);
      this->applyTransformation(
          m_Atoms[j].getCoordinateX(), m_Atoms[j].getCoordinateY(),
          m_Atoms[j].getCoordinateZ(), m_UnitCellRotations[oper],
          m_UnitCellTranslations[oper]);
    }
  }

  return;
}

void XtalTransformer::generateSuperCell(void) {
  if (m_HasSuperCellBeenGenerated)
    return;

  if (!m_HasUnitCellBeenGenerated)
    this->generateUnitCell();

  m_HasSuperCellBeenGenerated = true;

  // The super cell is generated through translations of the unit cell
  constexpr std::array<double, 9> rotation = {1.0, 0.0, 0.0, 0.0, 1.0,
                                              0.0, 0.0, 0.0, 1.0};
  constexpr std::array<std::array<double, 3>, 7> translations = {
      1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0,
      0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0};

  m_NumAtoms = m_TotNumAtoms;
  m_TotNumAtoms = 8 * m_NumAtoms;
  m_Atoms.resize(m_TotNumAtoms);
  for (std::size_t oper = 1; oper < 8; oper++) {
    for (std::size_t i = 0; i < m_NumAtoms; i++) {
      const std::size_t j = oper * m_NumAtoms + i;
      m_Atoms[j] = m_Atoms[i];
      m_Atoms[j].setIndex(m_Atoms[i].getIndex() + j);
      std::string chain = m_Atoms[i].getChain();
      std::string segi = m_Atoms[i].getSegmentIdentifier();
      std::size_t segidx =
          std::stoull(segi.substr(1, 3)) + oper * m_NumXtalOperations;
      if (segidx < 10)
        segi = chain + "00" + std::to_string(segidx);
      else if (segidx < 100)
        segi = chain + "0" + std::to_string(segidx);
      else if (segidx < 1000)
        segi = chain + std::to_string(segidx);
      else
        segi = "XXXX";
      m_Atoms[j].setSegmentIdentifier(segi);
      double x = m_Atoms[j].getCoordinateX(), u = 0.0;
      double y = m_Atoms[j].getCoordinateY(), v = 0.0;
      double z = m_Atoms[j].getCoordinateZ(), w = 0.0;
      this->cartesianToFractional(u, v, w, x, y, z);
      this->applyTransformation(u, v, w, rotation, translations[oper - 1]);
      this->fractionalToCartesian(x, y, z, u, v, w);
      m_Atoms[j].setCoordinateX(x);
      m_Atoms[j].setCoordinateY(y);
      m_Atoms[j].setCoordinateZ(z);
    }
  }

  return;
}

/* *
void XtalTransformer::writeCharmmCoordinateFile(const std::string &fileName) {
  constexpr double ZERO = 0.0;

  std::ofstream fout(fileName);

  fout << "* UNIT CELL\n";
  fout << "* GENERATED BY XTAL TRANSFORMER (AUTHOR: JAMES E. GONZALES II)\n";
  fout << "*\n";
  fout << std::setw(10) << m_TotNumAtoms << "  EXT\n";
  std::string oldResi = "";
  std::size_t iresi = 0;
  for (std::size_t i = 0; i < m_TotNumAtoms; i++) {
    const std::size_t idx = i + 1;
    const std::string resi = m_Atoms[i].getResidueIdentifier();
    std::string resn = m_Atoms[i].getResidueName();
    if (resn == "HIS")
      resn = "HSD";
    std::string albl = m_Atoms[i].getLabel();
    if ((resn == "ILE") && (albl == "CD1"))
      albl = "CD";
    const double xpos = m_Atoms[i].getCoordinateX();
    const double ypos = m_Atoms[i].getCoordinateY();
    const double zpos = m_Atoms[i].getCoordinateZ();
    const std::string segi = m_Atoms[i].getSegmentIdentifier();
    if (oldResi != resi) {
      iresi++;
      oldResi = resi;
    }
    fout << std::setw(10) << idx << std::setw(10) << resi << "  " << std::left
         << std::setw(8) << resn << "  " << std::setw(8) << albl << std::right
         << std::setw(20) << std::fixed << std::setprecision(10) << xpos
         << std::setw(20) << std::fixed << std::setprecision(10) << ypos
         << std::setw(20) << std::fixed << std::setprecision(10) << zpos << "  "
         << std::left << std::setw(8) << segi << "  " << std::setw(8) << iresi
         << std::right << std::setw(20) << std::fixed << std::setprecision(10)
         << ZERO << "\n";
  }

  return;
}
* */

void XtalTransformer::writePdbFile(const std::string &pdbFileName) {
  std::ofstream fout(pdbFileName);

  for (std::size_t i = 0; i < m_TotNumAtoms; i++) {
    std::string resn = m_Atoms[i].getResidueName();
    if (resn == "HIS")
      resn = "HSD";
    std::string albl = m_Atoms[i].getLabel();
    if ((resn == "ILE") && (albl == "CD1"))
      albl = "CD";
    fout << "ATOM " << std::setw(6) << m_Atoms[i].getIndex() << "  "
         << std::left << std::setw(3) << albl << std::right << " "
         << std::setw(3) << resn << " " << std::setw(1) << m_Atoms[i].getChain()
         << " " << std::setw(3) << m_Atoms[i].getResidueIdentifier() << "    "
         << std::setw(8) << std::fixed << std::setprecision(3)
         << m_Atoms[i].getCoordinateX() << std::setw(8) << std::fixed
         << std::setprecision(3) << m_Atoms[i].getCoordinateY() << std::setw(8)
         << std::fixed << std::setprecision(3) << m_Atoms[i].getCoordinateZ()
         << "  " << std::setw(3) << std::fixed << std::setprecision(2)
         << m_Atoms[i].getOccupancy() << " " << std::setw(5) << std::fixed
         << std::setprecision(2) << m_Atoms[i].getTemperatureFactor()
         << "      " << std::setw(4) << m_Atoms[i].getSegmentIdentifier() << " "
         << std::setw(1) << m_Atoms[i].getType() << "\n";
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

void XtalTransformer::fractionalToCartesian(double &x, double &y, double &z,
                                            const double u, const double v,
                                            const double w) {
  const double a = m_UnitCellDimensions[0];
  const double b = m_UnitCellDimensions[1];
  const double c = m_UnitCellDimensions[2];
  const double alpha = (M_PI / 180.0) * m_UnitCellDimensions[3];
  const double beta = (M_PI / 180.0) * m_UnitCellDimensions[4];
  const double gamma = (M_PI / 180.0) * m_UnitCellDimensions[5];

  const double cosAlpha = std::cos(alpha);
  const double cosBeta = std::cos(beta);
  const double cosGamma = std::cos(gamma);
  const double sinGamma = std::sin(gamma);

  const double cosAlpha2 = cosAlpha * cosAlpha;
  const double cosBeta2 = cosBeta * cosBeta;
  const double cosGamma2 = cosGamma * cosGamma;

  const double r = a * b * c *
                   std::sqrt(1.0 - cosAlpha2 - cosBeta2 - cosGamma2 +
                             2.0 * cosAlpha * cosBeta * cosGamma);

  x = a * u + (b * cosGamma) * v + (c * cosBeta) * w;
  y = (b * sinGamma) * v +
      (c * ((cosAlpha - cosBeta * cosGamma) / sinGamma)) * w;
  z = (r / (a * b * sinGamma)) * w;

  return;
}

void XtalTransformer::cartesianToFractional(double &u, double &v, double &w,
                                            const double x, const double y,
                                            const double z) {
  const double a = m_UnitCellDimensions[0];
  const double b = m_UnitCellDimensions[1];
  const double c = m_UnitCellDimensions[2];
  const double alpha = (M_PI / 180.0) * m_UnitCellDimensions[3];
  const double beta = (M_PI / 180.0) * m_UnitCellDimensions[4];
  const double gamma = (M_PI / 180.0) * m_UnitCellDimensions[5];

  const double cosAlpha = std::cos(alpha);
  const double cosBeta = std::cos(beta);
  const double cosGamma = std::cos(gamma);
  const double sinGamma = std::sin(gamma);

  const double cosAlpha2 = cosAlpha * cosAlpha;
  const double cosBeta2 = cosBeta * cosBeta;
  const double cosGamma2 = cosGamma * cosGamma;

  const double r = a * b * c *
                   std::sqrt(1.0 - cosAlpha2 - cosBeta2 - cosGamma2 +
                             2.0 * cosAlpha * cosBeta * cosGamma);

  u = (1.0 / a) * x - (cosGamma / (a * sinGamma)) * y +
      (b * c * ((cosAlpha * cosGamma - cosBeta) / (r * sinGamma))) * z;
  v = (1.0 / (b * sinGamma)) * y +
      (a * c * ((cosBeta * cosGamma - cosAlpha) / (r * sinGamma))) * z;
  w = (a * b * sinGamma / r) * z;

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
      for (std::size_t i = 0; i < 6; i++)
        ss >> m_UnitCellDimensions[i];
    }
  }

  if ((m_UnitCellDimensions[0] == 0.0) || (m_UnitCellDimensions[1] == 0.0) ||
      (m_UnitCellDimensions[2] == 0.0) || (m_UnitCellDimensions[3] == 0.0) ||
      (m_UnitCellDimensions[4] == 0.0) || (m_UnitCellDimensions[5] == 0.0))
    throw std::runtime_error("Could not read crystal dimensions from PDB file");

  std::cout << "Crystal dimensions: " << m_UnitCellDimensions[0] << ", "
            << m_UnitCellDimensions[1] << ", " << m_UnitCellDimensions[2]
            << "; " << m_UnitCellDimensions[3] << ", "
            << m_UnitCellDimensions[4] << ", " << m_UnitCellDimensions[5]
            << std::endl;

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
      std::string segi = std::string(chain) + "000";
      m_Atoms.emplace_back(
          Atom(idx, albl, resn, chain, resi, x, y, z, occ, temp, atype, segi));
      m_NumAtoms++;
    }
  }
  m_TotNumAtoms = m_NumAtoms;

  std::cout << "Read coordinates for " << m_NumAtoms << " atoms" << std::endl;

  return;
}
