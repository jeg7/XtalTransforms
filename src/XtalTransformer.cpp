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
      m_UnitCellTranslations(), m_NumAsymmetricAtoms(0), m_NumUnitCellAtoms(0),
      m_NumSuperCellAtoms(0), m_NumAtoms(0), m_AsymmetricAtoms(),
      m_UnitCellAtoms(), m_SuperCellAtoms(), m_Atoms() {}

XtalTransformer::XtalTransformer(const std::string &pdbFileName)
    : XtalTransformer() {
  this->readCrystalTransformationsFromPdbFile(pdbFileName);
  this->readCrystalDimensionsFromPdbFile(pdbFileName);
  this->readCoordinatesFromPdbFile(pdbFileName);
}

void XtalTransformer::writeXtalFile(const std::string &fileName) {
  std::ofstream fout(fileName + ".cor");

  fout << "* UNIT CELL WIRE FRAME\n";
  fout
      << "* THIS CHARMM COORDINATE FILE IS ONLY TO BE USED FOR VISUALIZATION\n";
  fout << "* GENERATED BY XTAL TRANSFORMER (AUTHOR: JAMES E. GONZALES II)\n";
  fout << "*\n";
  fout << "         8  EXT\n";
  std::array<double, 8> x, y, z;
  this->fractionalToCartesian(x[0], y[0], z[0], 0.0, 0.0, 0.0);
  this->fractionalToCartesian(x[1], y[1], z[1], 1.0, 0.0, 0.0);
  this->fractionalToCartesian(x[2], y[2], z[2], 0.0, 1.0, 0.0);
  this->fractionalToCartesian(x[3], y[3], z[3], 1.0, 1.0, 0.0);
  this->fractionalToCartesian(x[4], y[4], z[4], 0.0, 0.0, 1.0);
  this->fractionalToCartesian(x[5], y[5], z[5], 1.0, 0.0, 1.0);
  this->fractionalToCartesian(x[6], y[6], z[6], 0.0, 1.0, 1.0);
  this->fractionalToCartesian(x[7], y[7], z[7], 1.0, 1.0, 1.0);
  for (int i = 0; i < 8; i++) {
    fout << "         1         1  ALA       O       " << std::setw(20)
         << std::fixed << std::setprecision(10) << x[i] << std::setw(20)
         << std::fixed << std::setprecision(10) << y[i] << std::setw(20)
         << std::fixed << std::setprecision(10) << z[i]
         << "  XTAL      1               0.0000000000" << "\n";
  }

  fout.close();
  fout.open(fileName + ".psf");

  fout << "PSF EXT CMAP XPLOR\n";
  fout << "\n";
  fout << "         4 !NTITLE\n";
  fout << "* UNIT CELL WIRE FRAME\n";
  fout << "* THIS CHARMM PSF IS ONLY TO BE USED FOR VISUALIZATION\n";
  fout << "* GENERATED BY XTAL TRANSFORMER (AUTHOR: JAMES E. GONZALES II)\n";
  fout << "*\n";
  fout << "\n";
  fout << "         8 !NATOM\n";
  fout
      << "         1 XTAL     1        ALA      O        O       -0.510000     "
         "  15.9994           0\n";
  fout
      << "         2 XTAL     1        ALA      O        O       -0.510000     "
         "  15.9994           0\n";
  fout
      << "         3 XTAL     1        ALA      O        O       -0.510000     "
         "  15.9994           0\n";
  fout
      << "         4 XTAL     1        ALA      O        O       -0.510000     "
         "  15.9994           0\n";
  fout
      << "         5 XTAL     1        ALA      O        O       -0.510000     "
         "  15.9994           0\n";
  fout
      << "         6 XTAL     1        ALA      O        O       -0.510000     "
         "  15.9994           0\n";
  fout
      << "         7 XTAL     1        ALA      O        O       -0.510000     "
         "  15.9994           0\n";
  fout
      << "         8 XTAL     1        ALA      O        O       -0.510000     "
         "  15.9994           0\n";
  fout << "\n";
  fout << "        12 !NBOND: bonds\n";
  fout << "         1         2         1         3         1         5        "
          " 2         4\n";
  fout << "         2         6         3         4         3         7        "
          " 4         8\n";
  fout << "         5         6         5         7         6         8        "
          " 7         8\n";
  fout << "\n";
  fout << "         0 !NTHETA: angles\n";
  fout << "\n";
  fout << "         0 !NPHI: dihedrals\n";
  fout << "\n";
  fout << "         0 !IMNPHI: impropers\n";
  fout << "\n";

  return;
}

void XtalTransformer::generateUnitCell(void) {
  if (m_NumAsymmetricAtoms == 0) {
    throw std::runtime_error(
        "Cannot build unit cell without coordinates of the asymmetric unit");
  }

  if (m_NumUnitCellAtoms == m_NumXtalOperations * m_NumAsymmetricAtoms) {
    m_NumAtoms = m_NumUnitCellAtoms;
    m_Atoms = m_UnitCellAtoms;
  }

  m_NumUnitCellAtoms = m_NumXtalOperations * m_NumAsymmetricAtoms;
  m_UnitCellAtoms = m_AsymmetricAtoms;
  m_UnitCellAtoms.resize(m_NumUnitCellAtoms);
  for (std::size_t oper = 0; oper < m_NumXtalOperations; oper++) {
    for (std::size_t i = 0; i < m_NumAsymmetricAtoms; i++) {
      const std::size_t j = oper * m_NumAsymmetricAtoms + i;
      m_UnitCellAtoms[j] = m_AsymmetricAtoms[i];
      m_UnitCellAtoms[j].setIndex(m_AsymmetricAtoms[i].getIndex() + j);
      std::string segi = std::string(m_AsymmetricAtoms[i].getChain());
      if (oper < 10)
        segi += ("00" + std::to_string(oper));
      else if (oper < 100)
        segi += ("0" + std::to_string(oper));
      else if (oper < 1000)
        segi += std::to_string(oper);
      else
        segi = "XXXX";
      m_UnitCellAtoms[j].setSegmentIdentifier(segi);
      this->applyTransformation(m_UnitCellAtoms[j].getCoordinateX(),
                                m_UnitCellAtoms[j].getCoordinateY(),
                                m_UnitCellAtoms[j].getCoordinateZ(),
                                m_UnitCellRotations[oper],
                                m_UnitCellTranslations[oper]);
    }
  }

  m_NumAtoms = m_NumUnitCellAtoms;
  m_Atoms = m_UnitCellAtoms;

  return;
}

void XtalTransformer::generateSuperCell(void) {
  if (m_NumSuperCellAtoms == 8 * m_NumUnitCellAtoms) {
    m_NumAtoms = m_NumSuperCellAtoms;
    m_Atoms = m_SuperCellAtoms;
    return;
  }

  if (m_NumUnitCellAtoms == 0)
    this->generateUnitCell();

  // The super cell is generated through translations of the unit cell
  constexpr std::array<double, 9> rotation = {1.0, 0.0, 0.0, 0.0, 1.0,
                                              0.0, 0.0, 0.0, 1.0};
  constexpr std::array<std::array<double, 3>, 7> translations = {
      1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0,
      0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0};

  m_NumSuperCellAtoms = 8 * m_NumUnitCellAtoms;
  m_SuperCellAtoms = m_UnitCellAtoms;
  m_SuperCellAtoms.resize(m_NumSuperCellAtoms);
  for (std::size_t oper = 1; oper < 8; oper++) {
    for (std::size_t i = 0; i < m_NumUnitCellAtoms; i++) {
      const std::size_t j = oper * m_NumUnitCellAtoms + i;
      m_SuperCellAtoms[j] = m_UnitCellAtoms[i];
      m_SuperCellAtoms[j].setIndex(m_UnitCellAtoms[i].getIndex() + j);
      std::string chain = m_UnitCellAtoms[i].getChain();
      std::string segi = m_UnitCellAtoms[i].getSegmentIdentifier();
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
      m_SuperCellAtoms[j].setSegmentIdentifier(segi);
      double x = m_SuperCellAtoms[j].getCoordinateX(), u = 0.0;
      double y = m_SuperCellAtoms[j].getCoordinateY(), v = 0.0;
      double z = m_SuperCellAtoms[j].getCoordinateZ(), w = 0.0;
      this->cartesianToFractional(u, v, w, x, y, z);
      this->applyTransformation(u, v, w, rotation, translations[oper - 1]);
      this->fractionalToCartesian(x, y, z, u, v, w);
      m_SuperCellAtoms[j].setCoordinateX(x);
      m_SuperCellAtoms[j].setCoordinateY(y);
      m_SuperCellAtoms[j].setCoordinateZ(z);
    }
  }

  m_NumAtoms = m_NumSuperCellAtoms;
  m_Atoms = m_SuperCellAtoms;

  return;
}

void XtalTransformer::transformUnitCellReplicasFractional(
    const std::vector<std::array<double, 9>> &rotations,
    const std::vector<std::array<double, 3>> &translations) {
  const std::size_t numOperations = rotations.size();
  if (numOperations != translations.size()) {
    throw std::runtime_error(
        "Number of rotations and translations must be equal");
  }

  if (m_NumUnitCellAtoms == 0)
    this->generateUnitCell();

  m_NumAtoms = (numOperations + 1) * m_NumUnitCellAtoms;
  m_Atoms = m_UnitCellAtoms;
  m_Atoms.resize(m_NumAtoms);
  for (std::size_t oper = 0; oper < numOperations; oper++) {
    for (std::size_t i = 0; i < m_NumUnitCellAtoms; i++) {
      const std::size_t j = (oper + 1) * m_NumUnitCellAtoms + i;
      m_Atoms[j] = m_UnitCellAtoms[i];
      m_Atoms[j].setIndex(m_UnitCellAtoms[i].getIndex() + j);
      std::string chain = m_UnitCellAtoms[i].getChain();
      std::string segi = m_UnitCellAtoms[i].getSegmentIdentifier();
      std::size_t segidx =
          std::stoull(segi.substr(1, 3)) + (oper + 1) * m_NumXtalOperations;
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
      this->applyTransformation(u, v, w, rotations[oper], translations[oper]);
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

  for (std::size_t i = 0; i < m_NumAtoms; i++) {
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
      m_AsymmetricAtoms.emplace_back(
          Atom(idx, albl, resn, chain, resi, x, y, z, occ, temp, atype, segi));
      m_NumAsymmetricAtoms++;
    }
  }

  m_NumAtoms = m_NumAsymmetricAtoms;
  m_Atoms = m_AsymmetricAtoms;

  std::cout << "Read coordinates for " << m_NumAtoms << " atoms" << std::endl;

  return;
}
