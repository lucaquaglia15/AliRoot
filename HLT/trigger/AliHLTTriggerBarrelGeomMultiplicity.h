#ifndef ALIHLTTRIGGERBARRELGEOMMULTIPLICITY_H
#define ALIHLTTRIGGERBARRELGEOMMULTIPLICITY_H
//* This file is property of and copyright by the ALICE HLT Project        * 
//* ALICE Experiment at CERN, All rights reserved.                         *
//* See cxx source for full Copyright notice                               *

/// @file   AliHLTTriggerBarrelGeomMultiplicity.h
/// @author Oystein Djuvsland
/// @date   2009-10-08
/// @brief  HLT trigger component for charged particle multiplicity in
///         a geometrical selection of the central barrel.

#include "AliHLTTrigger.h"

/**
 * @class  AliHLTTriggerBarrelGeomMultiplicity
 * HLT trigger component for charged particle multiplicity in the
 * central barrel.
 * 
 * Triggers on charged particle number in a certain pt range.
 *
 * <h2>General properties:</h2>
 *
 * Component ID: \b BarrelGeomMultiplicityTrigger                             <br>
 * Library: \b libAliHLTTrigger.so                                        <br>
 * Input Data Types:  kAliHLTDataTypeESDObject, kAliHLTDataTypeESDTree
 *                    kAliHLTDataTypeTrack                                <br>
 * Output Data Types: ::kAliHLTAnyDataType                                <br>
 *
 * <h2>Mandatory arguments:</h2>
 * <!-- NOTE: ignore the \li. <i> and </i>: it's just doxygen formatting -->
 *
 * <h2>Optional arguments:</h2>
 * <!-- NOTE: ignore the \li. <i> and </i>: it's just doxygen formatting -->
 *
 * <h2>Configuration:</h2>
 * <!-- NOTE: ignore the \li. <i> and </i>: it's just doxygen formatting -->
 * \li -mintracks     <i> n   </i> <br>
 *      required number of tracks for a trigger
 * \li -minpt    <i> pt  </i> <br>
 *      required minimum pt for a trigger
 * \li -maxpt    <i> pt  </i> <br>
 *      required maximum pt for a trigger
 * \li -dca-reference    <i> x,y,z  </i> <br>
 *      reference point for the transverse and longitudinal dca cut
 * \li -min-ldca    <i> dca  </i> <br>
 *      minimum longitudinal dca to reference point
 * \li -max-ldca    <i> dca  </i> <br>
 *      maximum longitudinal dca to reference point
 * \li -min-tdca    <i> dca  </i> <br>
 *      minimum transverse dca to reference point
 * \li -max-tdca    <i> dca  </i> <br>
 *      maximum transverse dca to reference point
 * \li  -solenoidBz    <i> field  </i> <br>
 *      magnetic field needed if the input is not an ESD object
 *
 * By default, configuration is loaded from OCDB, can be overridden by
 * component arguments.
 *
 * <h2>Default CDB entries:</h2>
 * HLT/ConfigHLT/BarrelGeomMultiplicityTrigger: TObjString storing the arguments
 * HLT/ConfigHLT/Solenoidbz: TObjString -solenoidBz field
 *
 * <h2>Performance:</h2>
 * 
 *
 * <h2>Memory consumption:</h2>
 * 
 *
 * <h2>Output size:</h2>
 * 
 *
 * \ingroup alihlt_trigger_components
 */
class AliHLTTriggerBarrelGeomMultiplicity : public AliHLTTrigger
{
 public:
  AliHLTTriggerBarrelGeomMultiplicity();
  ~AliHLTTriggerBarrelGeomMultiplicity();

  /// inherited from AliHLTTrigger: name of this trigger
  virtual const char* GetTriggerName() const;
  /// inherited from AliHLTComponent: create an instance
  virtual AliHLTComponent* Spawn();

 protected:
  /// inherited from AliHLTComponent: handle the initialization
  int DoInit(int argc, const char** argv);

  /// inherited from AliHLTComponent: handle cleanup
  int DoDeinit();

  /// inherited from AliHLTComponent: handle re-configuration event
  int Reconfigure(const char* cdbEntry, const char* chainId);

  /// inherited from AliHLTComponent: handle dcs update event
  int ReadPreprocessorValues(const char* modules);

  /// inherited from AliHLTComponent, scan one argument and
  /// its parameters
  int ScanConfigurationArgument(int argc, const char** argv);

 private:

  /// inherited from AliHLTTrigger: calculate the trigger
  virtual int DoTrigger

  /// check whether a track meets the criteria  
  template<class T>
  bool CheckCondition(T* track, float b);

  // check whether a track is in the desired detectors
  Template<Class T>
  bool IsInDetector(T* track, float b);

  // abstract method for setting the correct trigger parameters
  void ApplyTrigger() = 0;

  ClassDef(AliHLTTriggerBarrelMultiplicity, 0);

};

#endif ALIHLTTRIGGERBARRELGEOMMULTIPLICITY_H
