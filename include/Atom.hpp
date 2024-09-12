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

#include <cstddef>
#include <string>

class Atom {
public:
  Atom(void);
  Atom(const std::size_t index, const std::string &label,
       const std::string &residueName, const std::string &chain,
       const std::string &residueIdentifier, const double coordinateX,
       const double coordinataeY, const double coordinateZ,
       const double occupancy, const double temperatureFactor,
       const std::string &atype, const std::string &segmentIdentifier);
  Atom(const Atom &other);
  Atom(const Atom &&other);

public:
  Atom &operator=(const Atom &other);
  Atom &operator=(const Atom &&other);

public:
  void setIndex(const std::size_t index);
  void setLabel(const std::string &label);
  void setResidueName(const std::string &residueName);
  void setChain(const std::string &chain);
  void setResidueIdentifier(const std::string &residueIdentifier);
  void setCoordinateX(const double coordinateX);
  void setCoordinateY(const double coordinateY);
  void setCoordinateZ(const double coordinateZ);
  void setOccupancy(const double occupancy);
  void setTemperatureFactor(const double temperatureFactor);
  void setType(const std::string &atype);
  void setSegmentIdentifier(const std::string &segmentIdentifier);

public:
  std::size_t getIndex(void) const;
  const std::string &getLabel(void) const;
  const std::string &getResidueName(void) const;
  const std::string &getChain(void) const;
  const std::string &getResidueIdentifier(void) const;
  double getCoordinateX(void) const;
  double getCoordinateY(void) const;
  double getCoordinateZ(void) const;
  double getOccupancy(void) const;
  double getTemperatureFactor(void) const;
  const std::string &getType(void) const;
  const std::string &getSegmentIdentifier(void) const;

public:
  std::size_t &getIndex(void);
  std::string &getLabel(void);
  std::string &getResidueName(void);
  std::string &getChain(void);
  std::string &getResidueIdentifier(void);
  double &getCoordinateX(void);
  double &getCoordinateY(void);
  double &getCoordinateZ(void);
  double &getOccupancy(void);
  double &getTemperatureFactor(void);
  std::string &getType(void);
  std::string &getSegmentIdentifier(void);

private:
  std::size_t m_Index;
  std::string m_Label;
  std::string m_ResidueName;
  std::string m_Chain;
  std::string m_ResidueIdentifier;
  double m_CoordinateX;
  double m_CoordinateY;
  double m_CoordinateZ;
  double m_Occupancy;
  double m_TemperatureFactor;
  std::string m_Type;
  std::string m_SegmentIdentifier;
};
