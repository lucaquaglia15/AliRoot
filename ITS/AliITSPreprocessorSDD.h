#ifndef ALIITSPREPROCESSORSDD_H
#define ALIITSPREPROCESSORSDD_H

////////////////////////////////////////////////////
//  Class for the                                 //
//  SDD beam test digit preprocessing             //
//  Origin: E. Crescio - crescio@to.infn.it       //
//                                                //
////////////////////////////////////////////////////


#include "AliPreprocessor.h"


class AliITSPreprocessorSDD : public AliPreprocessor { 
 

 public:
 
  AliITSPreprocessorSDD( AliShuttleInterface* shuttle):
    AliPreprocessor("SDD", shuttle){}
  virtual ~AliITSPreprocessorSDD(){;}



 protected:      


  
  virtual UInt_t Process(TMap* dcsAliasMap);

  static const Int_t fgkNumberOfSDD;       // number of SDD modules 
  static const Int_t fgkNumberOfChannels;  // number of channels per module

  static const TString fgkNameHistoPedestals; //name of ped. histo
  static const TString fgkNameHistoNoise;  //name of noise histo
  ClassDef(AliITSPreprocessorSDD,3)  // Alice ITS-SDD preprocessor.

 };



#endif

    
