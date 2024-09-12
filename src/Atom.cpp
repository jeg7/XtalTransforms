// BEGINLICENSE
//
// This file is part of ResidueSolventEnergy, which is distributed under the BSD
// 3-clause license, as described in the LICENSE file in the top level directory
// of this project.
//
// Author: James E. Gonzales II
//
// ENDLICENSE

#include "Atom.hpp"

Atom::Atom(void)
    : m_Index(0), m_Label(""), m_ResidueName(""), m_Chain(""),
      m_ResidueIdentifier(""), m_CoordinateX(0.0), m_CoordinateY(0.0),
      m_CoordinateZ(0.0), m_Occupancy(0.0), m_TemperatureFactor(0.0),
      m_Type(""), m_SegmentIdentifier("") {}

Atom::Atom(const std::size_t index, const std::string &label,
           const std::string &residueName, const std::string &chain,
           const std::string &residueIdentifier, const double coordinateX,
           const double coordinateY, const double coordinateZ,
           const double occupancy, const double temperatureFactor,
           const std::string &atype, const std::string &segmentIdentifier)
    : m_Index(index), m_Label(label), m_ResidueName(residueName),
      m_Chain(chain), m_ResidueIdentifier(residueIdentifier),
      m_CoordinateX(coordinateX), m_CoordinateY(coordinateY),
      m_CoordinateZ(coordinateZ), m_Occupancy(occupancy),
      m_TemperatureFactor(temperatureFactor), m_Type(atype),
      m_SegmentIdentifier(segmentIdentifier) {}

Atom::Atom(const Atom &other)
    : m_Index(other.m_Index), m_Label(other.m_Label),
      m_ResidueName(other.m_ResidueName), m_Chain(other.m_Chain),
      m_ResidueIdentifier(other.m_ResidueIdentifier),
      m_CoordinateX(other.m_CoordinateX), m_CoordinateY(other.m_CoordinateY),
      m_CoordinateZ(other.m_CoordinateZ), m_Occupancy(other.m_Occupancy),
      m_TemperatureFactor(other.m_TemperatureFactor), m_Type(other.m_Type),
      m_SegmentIdentifier(other.m_SegmentIdentifier) {}

Atom::Atom(const Atom &&other)
    : m_Index(other.m_Index), m_Label(other.m_Label),
      m_ResidueName(other.m_ResidueName), m_Chain(other.m_Chain),
      m_ResidueIdentifier(other.m_ResidueIdentifier),
      m_CoordinateX(other.m_CoordinateX), m_CoordinateY(other.m_CoordinateY),
      m_CoordinateZ(other.m_CoordinateZ), m_Occupancy(other.m_Occupancy),
      m_TemperatureFactor(other.m_TemperatureFactor), m_Type(other.m_Type),
      m_SegmentIdentifier(other.m_SegmentIdentifier) {}

Atom &Atom::operator=(const Atom &other) {
  m_Index = other.m_Index;
  m_Label = other.m_Label;
  m_ResidueName = other.m_ResidueName;
  m_Chain = other.m_Chain;
  m_ResidueIdentifier = other.m_ResidueIdentifier;
  m_CoordinateX = other.m_CoordinateX;
  m_CoordinateY = other.m_CoordinateY;
  m_CoordinateZ = other.m_CoordinateZ;
  m_Occupancy = other.m_Occupancy;
  m_TemperatureFactor = other.m_TemperatureFactor;
  m_Type = other.m_Type;
  m_SegmentIdentifier = other.m_SegmentIdentifier;
  return *this;
}

Atom &Atom::operator=(const Atom &&other) {
  m_Index = other.m_Index;
  m_Label = other.m_Label;
  m_ResidueName = other.m_ResidueName;
  m_Chain = other.m_Chain;
  m_ResidueIdentifier = other.m_ResidueIdentifier;
  m_CoordinateX = other.m_CoordinateX;
  m_CoordinateY = other.m_CoordinateY;
  m_CoordinateZ = other.m_CoordinateZ;
  m_Occupancy = other.m_Occupancy;
  m_TemperatureFactor = other.m_TemperatureFactor;
  m_Type = other.m_Type;
  m_SegmentIdentifier = other.m_SegmentIdentifier;
  return *this;
}

void Atom::setIndex(const std::size_t index) {
  m_Index = index;
  return;
}

void Atom::setLabel(const std::string &label) {
  m_Label = label;
  return;
}

void Atom::setResidueName(const std::string &residueName) {
  m_ResidueName = residueName;
  return;
}

void Atom::setChain(const std::string &chain) {
  m_Chain = chain;
  return;
}

void Atom::setResidueIdentifier(const std::string &residueIdentifier) {
  m_ResidueIdentifier = residueIdentifier;
  return;
}

void Atom::setCoordinateX(const double coordinateX) {
  m_CoordinateX = coordinateX;
  return;
}

void Atom::setCoordinateY(const double coordinateY) {
  m_CoordinateY = coordinateY;
  return;
}

void Atom::setCoordinateZ(const double coordinateZ) {
  m_CoordinateZ = coordinateZ;
  return;
}

void Atom::setOccupancy(const double occupancy) {
  m_Occupancy = occupancy;
  return;
}

void Atom::setTemperatureFactor(const double temperatureFactor) {
  m_TemperatureFactor = temperatureFactor;
  return;
}

void Atom::setType(const std::string &atype) {
  m_Type = atype;
  return;
}

void Atom::setSegmentIdentifier(const std::string &segmentIdentifier) {
  m_SegmentIdentifier = segmentIdentifier;
  return;
}

std::size_t Atom::getIndex(void) const { return m_Index; }

const std::string &Atom::getLabel(void) const { return m_Label; }

const std::string &Atom::getResidueName(void) const { return m_ResidueName; }

const std::string &Atom::getChain(void) const { return m_Chain; }

const std::string &Atom::getResidueIdentifier(void) const {
  return m_ResidueIdentifier;
}

double Atom::getCoordinateX(void) const { return m_CoordinateX; }

double Atom::getCoordinateY(void) const { return m_CoordinateY; }

double Atom::getCoordinateZ(void) const { return m_CoordinateZ; }

double Atom::getOccupancy(void) const { return m_Occupancy; }

double Atom::getTemperatureFactor(void) const { return m_TemperatureFactor; }

const std::string &Atom::getType(void) const { return m_Type; }

const std::string &Atom::getSegmentIdentifier(void) const {
  return m_SegmentIdentifier;
}

std::size_t &Atom::getIndex(void) { return m_Index; }

std::string &Atom::getLabel(void) { return m_Label; }

std::string &Atom::getResidueName(void) { return m_ResidueName; }

std::string &Atom::getChain(void) { return m_Chain; }

std::string &Atom::getResidueIdentifier(void) { return m_ResidueIdentifier; }

double &Atom::getCoordinateX(void) { return m_CoordinateX; }

double &Atom::getCoordinateY(void) { return m_CoordinateY; }

double &Atom::getCoordinateZ(void) { return m_CoordinateZ; }

double &Atom::getOccupancy(void) { return m_Occupancy; }

double &Atom::getTemperatureFactor(void) { return m_TemperatureFactor; }

std::string &Atom::getType(void) { return m_Type; }

std::string &Atom::getSegmentIdentifier(void) { return m_SegmentIdentifier; }
