//
// $Id$
//
#ifndef ALIFMD1_H
#define ALIFMD1_H

#ifndef ALIFMDSUBDETECTOR_H
# include "AliFMDSubDetector.h"
#endif

class AliFMD1 : public AliFMDSubDetector 
{
public:
  AliFMD1();
  virtual ~AliFMD1();
  virtual void   SetupGeometry(Int_t airId, Int_t alId, Int_t cId=0);  
  virtual void   Geometry(const char* mother, Int_t pbRotId, 
			  Int_t idRotId, Double_t z=0);
protected:
  Int_t    fVolumeId;   // Volume ID
  Double_t fDz;         // Half-length in Z
  ClassDef(AliFMD1,1);  // Geometry of FMD1 
};

#endif
//____________________________________________________________________
//
// Local Variables:
//   mode: C++
// End:
//
// EOF
//
