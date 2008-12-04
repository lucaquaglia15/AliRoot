/**************************************************************************
 * Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

/* $Id$ */

//-------------------------------------------------------------------------
//               Implementation of the ESD V0 vertex class
//            This class is part of the Event Data Summary
//            set of classes and contains information about
//            V0 kind vertexes generated by a neutral particle
//     Origin: Iouri Belikov, IReS, Strasbourg, Jouri.Belikov@cern.ch
//     Modified by: Marian Ivanov,  CERN, Marian.Ivanov@cern.ch
//            and  Boris Hippolyte,IPHC, hippolyt@in2p3.fr 
//-------------------------------------------------------------------------

#include <TMath.h>
#include <TDatabasePDG.h>
#include <TParticlePDG.h>
#include <TVector3.h>

#include "AliLog.h"
#include "AliESDv0.h"
#include "AliESDV0Params.h"

ClassImp(AliESDv0)

const AliESDV0Params  AliESDv0::fgkParams;

AliESDv0::AliESDv0() :
  AliVParticle(),
  fParamN(),
  fParamP(),
  fEffMass(TDatabasePDG::Instance()->GetParticle(kK0Short)->Mass()),
  fDcaV0Daughters(0),
  fChi2V0(0.),
  fRr(0),
  fDistSigma(0),
  fChi2Before(0),
  fChi2After(0),
  fPointAngleFi(0),
  fPointAngleTh(0),
  fPointAngle(0),
  fPdgCode(kK0Short),
  fNidx(0),
  fPidx(0),
  fStatus(0),
  fNBefore(0),
  fNAfter(0),
  fOnFlyStatus(kFALSE)
{
  //--------------------------------------------------------------------
  // Default constructor  (K0s)
  //--------------------------------------------------------------------

  for (Int_t i=0; i<3; i++) {
    fPos[i] = 0.;
    fNmom[i] = 0.;
    fPmom[i] = 0.;
  }

  for (Int_t i=0; i<6; i++) {
    fPosCov[i]= 0.;
  }

  for (Int_t i=0;i<6;i++){fClusters[0][i]=0; fClusters[1][i]=0;}
  fNormDCAPrim[0]=fNormDCAPrim[1]=0;
  for (Int_t i=0;i<3;i++){fAngle[i]=0;}
  for (Int_t i=0;i<4;i++){fCausality[i]=0;}
}

AliESDv0::AliESDv0(const AliESDv0& v0) :
  AliVParticle(v0),
  fParamN(v0.fParamN),
  fParamP(v0.fParamP),
  fEffMass(v0.fEffMass),
  fDcaV0Daughters(v0.fDcaV0Daughters),
  fChi2V0(v0.fChi2V0),
  fRr(v0.fRr),
  fDistSigma(v0.fDistSigma),
  fChi2Before(v0.fChi2Before),
  fChi2After(v0.fChi2After),
  fPointAngleFi(v0.fPointAngleFi),
  fPointAngleTh(v0.fPointAngleTh),
  fPointAngle(v0.fPointAngle),
  fPdgCode(v0.fPdgCode),
  fNidx(v0.fNidx),
  fPidx(v0.fPidx),
  fStatus(v0.fStatus),
  fNBefore(v0.fNBefore),
  fNAfter(v0.fNAfter),
  fOnFlyStatus(v0.fOnFlyStatus)
{
  //--------------------------------------------------------------------
  // The copy constructor
  //--------------------------------------------------------------------

  for (int i=0; i<3; i++) {
    fPos[i]  = v0.fPos[i];
    fNmom[i] = v0.fNmom[i];
    fPmom[i] = v0.fPmom[i];
  }
  for (int i=0; i<6; i++) {
    fPosCov[i]  = v0.fPosCov[i];
  }

  for (Int_t i=0; i<2; i++) {
    fNormDCAPrim[i]=v0.fNormDCAPrim[i];
  }
  for (Int_t i=0;i<6;i++){
      fClusters[0][i]=v0.fClusters[0][i]; 
      fClusters[1][i]=v0.fClusters[1][i];
  }
  for (Int_t i=0;i<3;i++){
      fAngle[i]=v0.fAngle[i];
  }
  for (Int_t i=0;i<4;i++){fCausality[i]=v0.fCausality[i];}
}


AliESDv0::AliESDv0(const AliExternalTrackParam &t1, Int_t i1,
                   const AliExternalTrackParam &t2, Int_t i2) :
  AliVParticle(),
  fParamN(t1),
  fParamP(t2),
  fEffMass(TDatabasePDG::Instance()->GetParticle(kK0Short)->Mass()),
  fDcaV0Daughters(0),
  fChi2V0(0.),
  fRr(0),
  fDistSigma(0),
  fChi2Before(0),
  fChi2After(0),
  fPointAngleFi(0),
  fPointAngleTh(0),
  fPointAngle(0),
  fPdgCode(kK0Short),
  fNidx(i1),
  fPidx(i2),
  fStatus(0),
  fNBefore(0),
  fNAfter(0),
  fOnFlyStatus(kFALSE)
{
  //--------------------------------------------------------------------
  // Main constructor  (K0s)
  //--------------------------------------------------------------------

  for (Int_t i=0; i<6; i++) {
    fPosCov[i]= 0.;
  }

  //Trivial estimation of the vertex parameters
  Double_t alpha=t1.GetAlpha(), cs=TMath::Cos(alpha), sn=TMath::Sin(alpha);
  Double_t tmp[3];
  t1.GetPxPyPz(tmp);
  Double_t px1=tmp[0], py1=tmp[1], pz1=tmp[2];
  t1.GetXYZ(tmp);
  Double_t  x1=tmp[0],  y1=tmp[1],  z1=tmp[2];
  const Double_t ss=0.0005*0.0005;//a kind of a residual misalignment precision
  Double_t sx1=sn*sn*t1.GetSigmaY2()+ss, sy1=cs*cs*t1.GetSigmaY2()+ss; 


  alpha=t2.GetAlpha(); cs=TMath::Cos(alpha); sn=TMath::Sin(alpha);
  t2.GetPxPyPz(tmp);
  Double_t px2=tmp[0], py2=tmp[1], pz2=tmp[2];
  t2.GetXYZ(tmp);
  Double_t  x2=tmp[0],  y2=tmp[1],  z2=tmp[2];
  Double_t sx2=sn*sn*t2.GetSigmaY2()+ss, sy2=cs*cs*t2.GetSigmaY2()+ss; 
    
  Double_t sz1=t1.GetSigmaZ2(), sz2=t2.GetSigmaZ2();
  Double_t wx1=sx2/(sx1+sx2), wx2=1.- wx1;
  Double_t wy1=sy2/(sy1+sy2), wy2=1.- wy1;
  Double_t wz1=sz2/(sz1+sz2), wz2=1.- wz1;
  fPos[0]=wx1*x1 + wx2*x2; fPos[1]=wy1*y1 + wy2*y2; fPos[2]=wz1*z1 + wz2*z2;

  //fPos[0]=0.5*(x1+x2); fPos[1]=0.5*(y1+y2); fPos[2]=0.5*(z1+z2);
  fNmom[0]=px1; fNmom[1]=py1; fNmom[2]=pz1; 
  fPmom[0]=px2; fPmom[1]=py2; fPmom[2]=pz2;

  for (Int_t i=0;i<6;i++){fClusters[0][i]=0; fClusters[1][i]=0;}
  fNormDCAPrim[0]=fNormDCAPrim[1]=0;
  for (Int_t i=0;i<3;i++){fAngle[i]=0;}
  for (Int_t i=0;i<4;i++){fCausality[i]=0;}
}

AliESDv0& AliESDv0::operator=(const AliESDv0 &v0)
{
  //--------------------------------------------------------------------
  // The assignment operator
  //--------------------------------------------------------------------

  if(this==&v0)return *this;
  AliVParticle::operator=(v0);
  fParamN  = v0.fParamN;
  fParamP  = v0.fParamP;
  fEffMass = v0.fEffMass;
  fDcaV0Daughters = v0.fDcaV0Daughters;
  fChi2V0 = v0.fChi2V0;
  fRr = v0.fRr;
  fDistSigma    = v0.fDistSigma;
  fChi2Before   = v0.fChi2Before;
  fChi2After    = v0.fChi2After;
  fPointAngleFi = v0.fPointAngleFi;
  fPointAngleTh = v0.fPointAngleTh;
  fPointAngle   = v0.fPointAngle;
  fPdgCode      = v0.fPdgCode;
  fNidx         = v0.fNidx;
  fPidx         = v0.fPidx;
  fStatus       = v0.fStatus;
  fNBefore      = v0.fNBefore;
  fNAfter       = v0.fNAfter;
  fOnFlyStatus  = v0.fOnFlyStatus;

  for (int i=0; i<3; i++) {
    fPos[i]  = v0.fPos[i];
    fNmom[i] = v0.fNmom[i];
    fPmom[i] = v0.fPmom[i];
  }
  for (int i=0; i<6; i++) {
    fPosCov[i]  = v0.fPosCov[i];
  }
  for (Int_t i=0; i<2; i++) {
    fNormDCAPrim[i]=v0.fNormDCAPrim[i];
  }
  for (Int_t i=0;i<6;i++){
      fClusters[0][i]=v0.fClusters[0][i]; 
      fClusters[1][i]=v0.fClusters[1][i];
  }
  for (Int_t i=0;i<3;i++){
      fAngle[i]=v0.fAngle[i];
  }
  for (Int_t i=0;i<4;i++){fCausality[i]=v0.fCausality[i];}

  return *this;
}

void AliESDv0::Copy(TObject& obj) const {

  // this overwrites the virtual TOBject::Copy()
  // to allow run time copying without casting
  // in AliESDEvent

  if(this==&obj)return;
  AliESDv0 *robj = dynamic_cast<AliESDv0*>(&obj);
  if(!robj)return; // not an aliesesv0
  *robj = *this;
}

AliESDv0::~AliESDv0(){
  //--------------------------------------------------------------------
  // Empty destructor
  //--------------------------------------------------------------------
}

// Start with AliVParticle functions
Double_t AliESDv0::E() const {
  //--------------------------------------------------------------------
  // This gives the energy assuming the ChangeMassHypothesis was called
  //--------------------------------------------------------------------
  return E(fPdgCode);
}

Double_t AliESDv0::Y() const {
  //--------------------------------------------------------------------
  // This gives the energy assuming the ChangeMassHypothesis was called
  //--------------------------------------------------------------------
  return Y(fPdgCode);
}

// Then extend AliVParticle functions
Double_t AliESDv0::E(Int_t pdg) const {
  //--------------------------------------------------------------------
  // This gives the energy with the particle hypothesis as argument 
  //--------------------------------------------------------------------
  Double_t mass = TDatabasePDG::Instance()->GetParticle(pdg)->Mass();
  return TMath::Sqrt(mass*mass+P()*P());
}

Double_t AliESDv0::Y(Int_t pdg) const {
  //--------------------------------------------------------------------
  // This gives the rapidity with the particle hypothesis as argument 
  //--------------------------------------------------------------------
  return 0.5*TMath::Log((E(pdg)+Pz())/(E(pdg)-Pz()+1.e-13));
}

// Now the functions for analysis consistency
Double_t AliESDv0::RapK0Short() const {
  //--------------------------------------------------------------------
  // This gives the pseudorapidity assuming a K0s particle
  //--------------------------------------------------------------------
  return Y(kK0Short);
}

Double_t AliESDv0::RapLambda() const {
  //--------------------------------------------------------------------
  // This gives the pseudorapidity assuming a (Anti) Lambda particle
  //--------------------------------------------------------------------
  return Y(kLambda0);
}

Double_t AliESDv0::AlphaV0() const {
  //--------------------------------------------------------------------
  // This gives the Armenteros-Podolanski alpha
  //--------------------------------------------------------------------
  TVector3 momNeg(fNmom[0],fNmom[1],fNmom[2]);
  TVector3 momPos(fPmom[0],fPmom[1],fPmom[2]);
  TVector3 momTot(Px(),Py(),Pz());

  Double_t lQlNeg = momNeg.Dot(momTot)/momTot.Mag();
  Double_t lQlPos = momNeg.Dot(momTot)/momTot.Mag();

  return 1.-2./(1.+lQlNeg/lQlPos);
}

Double_t AliESDv0::PtArmV0() const {
  //--------------------------------------------------------------------
  // This gives the Armenteros-Podolanski ptarm
  //--------------------------------------------------------------------
  TVector3 momNeg(fNmom[0],fNmom[1],fNmom[2]);
  TVector3 momTot(Px(),Py(),Pz());

  return momNeg.Perp(momTot);
}

// Eventually the older functions
Double_t AliESDv0::ChangeMassHypothesis(Int_t code) {
  //--------------------------------------------------------------------
  // This function changes the mass hypothesis for this V0
  // and returns the "kinematical quality" of this hypothesis 
  //--------------------------------------------------------------------
  static
  Double_t piMass=TDatabasePDG::Instance()->GetParticle(kPiPlus)->Mass();
  static
  Double_t prMass=TDatabasePDG::Instance()->GetParticle(kProton)->Mass();
  static
  Double_t k0Mass=TDatabasePDG::Instance()->GetParticle(kK0Short)->Mass();
  static
  Double_t l0Mass=TDatabasePDG::Instance()->GetParticle(kLambda0)->Mass();

  Double_t nmass=piMass, pmass=piMass, mass=k0Mass, ps=0.206;

  fPdgCode=code;

  switch (code) {
  case kLambda0:
    nmass=piMass; pmass=prMass; mass=l0Mass; ps=0.101; break;
  case kLambda0Bar:
    pmass=piMass; nmass=prMass; mass=l0Mass; ps=0.101; break;
  case kK0Short: 
    break;
  default:
    AliError("invalide PDG code ! Assuming K0s...");
    fPdgCode=kK0Short;
    break;
  }

  Double_t pxn=fNmom[0], pyn=fNmom[1], pzn=fNmom[2]; 
  Double_t pxp=fPmom[0], pyp=fPmom[1], pzp=fPmom[2];

  Double_t en=TMath::Sqrt(nmass*nmass + pxn*pxn + pyn*pyn + pzn*pzn);
  Double_t ep=TMath::Sqrt(pmass*pmass + pxp*pxp + pyp*pyp + pzp*pzp);
  Double_t pxl=pxn+pxp, pyl=pyn+pyp, pzl=pzn+pzp;
  Double_t pl=TMath::Sqrt(pxl*pxl + pyl*pyl + pzl*pzl);

  fEffMass=TMath::Sqrt((en+ep)*(en+ep)-pl*pl);

  Double_t beta=pl/(en+ep);
  Double_t pln=(pxn*pxl + pyn*pyl + pzn*pzl)/pl;
  Double_t plp=(pxp*pxl + pyp*pyl + pzp*pzl)/pl;

  Double_t pt2=pxp*pxp + pyp*pyp + pzp*pzp - plp*plp;

  Double_t a=(plp-pln)/(plp+pln);
  a -= (pmass*pmass-nmass*nmass)/(mass*mass);
  a = 0.25*beta*beta*mass*mass*a*a + pt2;

  return (a - ps*ps);
  
}

void AliESDv0::GetPxPyPz(Double_t &px, Double_t &py, Double_t &pz) const {
  //--------------------------------------------------------------------
  // This function returns V0's momentum (global)
  //--------------------------------------------------------------------
  px=fNmom[0]+fPmom[0]; 
  py=fNmom[1]+fPmom[1]; 
  pz=fNmom[2]+fPmom[2]; 
}

void AliESDv0::GetXYZ(Double_t &x, Double_t &y, Double_t &z) const {
  //--------------------------------------------------------------------
  // This function returns V0's position (global)
  //--------------------------------------------------------------------
  x=fPos[0]; 
  y=fPos[1]; 
  z=fPos[2]; 
}

Float_t AliESDv0::GetD(Double_t x0, Double_t y0, Double_t z0) const {
  //--------------------------------------------------------------------
  // This function returns V0's impact parameter
  //--------------------------------------------------------------------
  Double_t x=fPos[0],y=fPos[1],z=fPos[2];
  Double_t px=fNmom[0]+fPmom[0];
  Double_t py=fNmom[1]+fPmom[1];
  Double_t pz=fNmom[2]+fPmom[2];

  Double_t dx=(y0-y)*pz - (z0-z)*py; 
  Double_t dy=(x0-x)*pz - (z0-z)*px;
  Double_t dz=(x0-x)*py - (y0-y)*px;
  Double_t d=TMath::Sqrt((dx*dx+dy*dy+dz*dz)/(px*px+py*py+pz*pz));
  return d;
}

Float_t AliESDv0::GetV0CosineOfPointingAngle(Double_t refPointX, Double_t refPointY, Double_t refPointZ) const {
  // calculates the pointing angle of the V0 wrt a reference point

  Double_t momV0[3]; //momentum of the V0
  GetPxPyPz(momV0[0],momV0[1],momV0[2]);

  Double_t deltaPos[3]; //vector between the reference point and the V0 vertex
  deltaPos[0] = fPos[0] - refPointX;
  deltaPos[1] = fPos[1] - refPointY;
  deltaPos[2] = fPos[2] - refPointZ;

  Double_t momV02    = momV0[0]*momV0[0] + momV0[1]*momV0[1] + momV0[2]*momV0[2];
  Double_t deltaPos2 = deltaPos[0]*deltaPos[0] + deltaPos[1]*deltaPos[1] + deltaPos[2]*deltaPos[2];

  Double_t cosinePointingAngle = (deltaPos[0]*momV0[0] +
				  deltaPos[1]*momV0[1] +
				  deltaPos[2]*momV0[2] ) /
    TMath::Sqrt(momV02 * deltaPos2);
  
  return cosinePointingAngle;
}


// **** The following functions need to be revised

void AliESDv0::GetPosCov(Double_t cov[6]) const {

  for (Int_t i=0; i<6; ++i) cov[i] = fPosCov[i];

}

Double_t AliESDv0::GetSigmaY(){
  //
  // return sigmay in y  at vertex position  using covariance matrix 
  //
  const Double_t * cp  = fParamP.GetCovariance();
  const Double_t * cm  = fParamN.GetCovariance();
  Double_t sigmay = cp[0]+cm[0]+ cp[5]*(fParamP.GetX()-fRr)*(fParamP.GetX()-fRr)+ cm[5]*(fParamN.GetX()-fRr)*(fParamN.GetX()-fRr);
  return (sigmay>0) ? TMath::Sqrt(sigmay):100;
}

Double_t AliESDv0::GetSigmaZ(){
  //
  // return sigmay in y  at vertex position  using covariance matrix 
  //
  const Double_t * cp  = fParamP.GetCovariance();
  const Double_t * cm  = fParamN.GetCovariance();
  Double_t sigmaz = cp[2]+cm[2]+ cp[9]*(fParamP.GetX()-fRr)*(fParamP.GetX()-fRr)+ cm[9]*(fParamN.GetX()-fRr)*(fParamN.GetX()-fRr);
  return (sigmaz>0) ? TMath::Sqrt(sigmaz):100;
}

Double_t AliESDv0::GetSigmaD0(){
  //
  // Sigma parameterization using covariance matrix
  //
  // sigma of distance between two tracks in vertex position 
  // sigma of DCA is proportianal to sigmaD0
  // factor 2 difference is explained by the fact that the DCA is calculated at the position 
  // where the tracks as closest together ( not exact position of the vertex)
  //
  const Double_t * cp      = fParamP.GetCovariance();
  const Double_t * cm      = fParamN.GetCovariance();
  Double_t sigmaD0   = cp[0]+cm[0]+cp[2]+cm[2]+fgkParams.fPSigmaOffsetD0*fgkParams.fPSigmaOffsetD0;
  sigmaD0           += ((fParamP.GetX()-fRr)*(fParamP.GetX()-fRr))*(cp[5]+cp[9]);
  sigmaD0           += ((fParamN.GetX()-fRr)*(fParamN.GetX()-fRr))*(cm[5]+cm[9]);
  return (sigmaD0>0)? TMath::Sqrt(sigmaD0):100;
}


Double_t AliESDv0::GetSigmaAP0(){
  //
  //Sigma parameterization using covariance matrices
  //
  Double_t prec  = TMath::Sqrt((fNmom[0]+fPmom[0])*(fNmom[0]+fPmom[0])
			      +(fNmom[1]+fPmom[1])*(fNmom[1]+fPmom[1])
			      +(fNmom[2]+fPmom[2])*(fNmom[2]+fPmom[2]));
  Double_t normp = TMath::Sqrt(fPmom[0]*fPmom[0]+fPmom[1]*fPmom[1]+fPmom[2]*fPmom[2])/prec;  // fraction of the momenta
  Double_t normm = TMath::Sqrt(fNmom[0]*fNmom[0]+fNmom[1]*fNmom[1]+fNmom[2]*fNmom[2])/prec;  
  const Double_t * cp      = fParamP.GetCovariance();
  const Double_t * cm      = fParamN.GetCovariance();
  Double_t sigmaAP0 = fgkParams.fPSigmaOffsetAP0*fgkParams.fPSigmaOffsetAP0;                           // minimal part
  sigmaAP0 +=  (cp[5]+cp[9])*(normp*normp)+(cm[5]+cm[9])*(normm*normm);          // angular resolution part
  Double_t sigmaAP1 = GetSigmaD0()/(TMath::Abs(fRr)+0.01);                       // vertex position part
  sigmaAP0 +=  0.5*sigmaAP1*sigmaAP1;                              
  return (sigmaAP0>0)? TMath::Sqrt(sigmaAP0):100;
}

Double_t AliESDv0::GetEffectiveSigmaD0(){
  //
  // minimax - effective Sigma parameterization 
  // p12 effective curvature and v0 radius postion used as parameters  
  //  
  Double_t p12 = TMath::Sqrt(fParamP.GetParameter()[4]*fParamP.GetParameter()[4]+
			     fParamN.GetParameter()[4]*fParamN.GetParameter()[4]);
  Double_t sigmaED0= TMath::Max(TMath::Sqrt(fRr)-fgkParams.fPSigmaRminDE,0.0)*fgkParams.fPSigmaCoefDE*p12*p12;
  sigmaED0*= sigmaED0;
  sigmaED0*= sigmaED0;
  sigmaED0 = TMath::Sqrt(sigmaED0+fgkParams.fPSigmaOffsetDE*fgkParams.fPSigmaOffsetDE);
  return (sigmaED0<fgkParams.fPSigmaMaxDE) ? sigmaED0: fgkParams.fPSigmaMaxDE;
}


Double_t AliESDv0::GetEffectiveSigmaAP0(){
  //
  // effective Sigma parameterization of point angle resolution 
  //
  Double_t p12 = TMath::Sqrt(fParamP.GetParameter()[4]*fParamP.GetParameter()[4]+
			     fParamN.GetParameter()[4]*fParamN.GetParameter()[4]);
  Double_t sigmaAPE= fgkParams.fPSigmaBase0APE;
  sigmaAPE+= fgkParams.fPSigmaR0APE/(fgkParams.fPSigmaR1APE+fRr);
  sigmaAPE*= (fgkParams.fPSigmaP0APE+fgkParams.fPSigmaP1APE*p12);
  sigmaAPE = TMath::Min(sigmaAPE,fgkParams.fPSigmaMaxAPE);
  return sigmaAPE;
}


Double_t  AliESDv0::GetMinimaxSigmaAP0(){
  //
  // calculate mini-max effective sigma of point angle resolution
  //
  //compv0->fTree->SetAlias("SigmaAP2","max(min((SigmaAP0+SigmaAPE0)*0.5,1.5*SigmaAPE0),0.5*SigmaAPE0+0.003)");
  Double_t    effectiveSigma = GetEffectiveSigmaAP0();
  Double_t    sigmaMMAP = 0.5*(GetSigmaAP0()+effectiveSigma);
  sigmaMMAP  = TMath::Min(sigmaMMAP, fgkParams.fPMaxFractionAP0*effectiveSigma);
  sigmaMMAP  = TMath::Max(sigmaMMAP, fgkParams.fPMinFractionAP0*effectiveSigma+fgkParams.fPMinAP0);
  return sigmaMMAP;
}
Double_t  AliESDv0::GetMinimaxSigmaD0(){
  //
  // calculate mini-max sigma of dca resolution
  // 
  //compv0->fTree->SetAlias("SigmaD2","max(min((SigmaD0+SigmaDE0)*0.5,1.5*SigmaDE0),0.5*SigmaDE0)");
  Double_t    effectiveSigma = GetEffectiveSigmaD0();
  Double_t    sigmaMMD0 = 0.5*(GetSigmaD0()+effectiveSigma);
  sigmaMMD0  = TMath::Min(sigmaMMD0, fgkParams.fPMaxFractionD0*effectiveSigma);
  sigmaMMD0  = TMath::Max(sigmaMMD0, fgkParams.fPMinFractionD0*effectiveSigma+fgkParams.fPMinD0);
  return sigmaMMD0;
}


Double_t AliESDv0::GetLikelihoodAP(Int_t mode0, Int_t mode1){
  //
  // get likelihood for point angle
  //
  Double_t sigmaAP = 0.007;            //default sigma
  switch (mode0){
  case 0:
    sigmaAP = GetSigmaAP0();           // mode 0  - covariance matrix estimates used 
    break;
  case 1:
    sigmaAP = GetEffectiveSigmaAP0();  // mode 1 - effective sigma used
    break;
  case 2:
    sigmaAP = GetMinimaxSigmaAP0();    // mode 2 - minimax sigma
    break;
  }
  Double_t apNorm = TMath::Min(TMath::ACos(fPointAngle)/sigmaAP,50.);  
  //normalized point angle, restricted - because of overflow problems in Exp
  Double_t likelihood = 0;
  switch(mode1){
  case 0:
    likelihood = TMath::Exp(-0.5*apNorm*apNorm);   
    // one component
    break;
  case 1:
    likelihood = (TMath::Exp(-0.5*apNorm*apNorm)+0.5* TMath::Exp(-0.25*apNorm*apNorm))/1.5;
    // two components
    break;
  case 2:
    likelihood = (TMath::Exp(-0.5*apNorm*apNorm)+0.5* TMath::Exp(-0.25*apNorm*apNorm)+0.25*TMath::Exp(-0.125*apNorm*apNorm))/1.75;
    // three components
    break;
  }
  return likelihood;
}

Double_t AliESDv0::GetLikelihoodD(Int_t mode0, Int_t mode1){
  //
  // get likelihood for DCA
  //
  Double_t sigmaD = 0.03;            //default sigma
  switch (mode0){
  case 0:
    sigmaD = GetSigmaD0();           // mode 0  - covariance matrix estimates used 
    break;
  case 1:
    sigmaD = GetEffectiveSigmaD0();  // mode 1 - effective sigma used
    break;
  case 2:
    sigmaD = GetMinimaxSigmaD0();    // mode 2 - minimax sigma
    break;
  }

  //Bo:  Double_t dNorm = TMath::Min(fDist2/sigmaD,50.);
  Double_t dNorm = TMath::Min(fDcaV0Daughters/sigmaD,50.);//Bo:
  //normalized point angle, restricted - because of overflow problems in Exp
  Double_t likelihood = 0;
  switch(mode1){
  case 0:
    likelihood = TMath::Exp(-2.*dNorm);   
    // one component
    break;
  case 1:
    likelihood = (TMath::Exp(-2.*dNorm)+0.5* TMath::Exp(-dNorm))/1.5;
    // two components
    break;
  case 2:
    likelihood = (TMath::Exp(-2.*dNorm)+0.5* TMath::Exp(-dNorm)+0.25*TMath::Exp(-0.5*dNorm))/1.75;
    // three components
    break;
  }
  return likelihood;

}

Double_t AliESDv0::GetLikelihoodC(Int_t mode0, Int_t /*mode1*/) const {
  //
  // get likelihood for Causality
  // !!!  Causality variables defined in AliITStrackerMI !!! 
  //      when more information was available
  //  
  Double_t likelihood = 0.5;
  Double_t minCausal  = TMath::Min(fCausality[0],fCausality[1]);
  Double_t maxCausal  = TMath::Max(fCausality[0],fCausality[1]);
  //  minCausal           = TMath::Max(minCausal,0.5*maxCausal);
  //compv0->fTree->SetAlias("LCausal","(1.05-(2*(0.8-exp(-max(RC.fV0rec.fCausality[0],RC.fV0rec.fCausality[1])))+2*(0.8-exp(-min(RC.fV0rec.fCausality[0],RC.fV0rec.fCausality[1]))))/2)**4");
  
  switch(mode0){
  case 0:
    //normalization 
    likelihood = TMath::Power((1.05-2*(0.8-TMath::Exp(-maxCausal))),4.);
    break;
  case 1:
    likelihood = TMath::Power(1.05-(2*(0.8-TMath::Exp(-maxCausal))+(2*(0.8-TMath::Exp(-minCausal))))*0.5,4.);
    break;
  }
  return likelihood;
  
}

void AliESDv0::SetCausality(Float_t pb0, Float_t pb1, Float_t pa0, Float_t pa1)
{
  //
  // set probabilities
  //
  fCausality[0] = pb0;     // probability - track 0 exist before vertex
  fCausality[1] = pb1;     // probability - track 1 exist before vertex
  fCausality[2] = pa0;     // probability - track 0 exist close after vertex
  fCausality[3] = pa1;     // probability - track 1 exist close after vertex
}
void  AliESDv0::SetClusters(const Int_t *clp, const Int_t *clm)
{
  //
  // Set its clusters indexes
  //
  for (Int_t i=0;i<6;i++) fClusters[0][i] = clp[i]; 
  for (Int_t i=0;i<6;i++) fClusters[1][i] = clm[i]; 
}

Double_t AliESDv0::GetEffMass(UInt_t p1, UInt_t p2) const{
  //
  // calculate effective mass
  //
  const Float_t kpmass[5] = {5.10000000000000037e-04,1.05660000000000004e-01,1.39570000000000000e-01,
		      4.93599999999999983e-01, 9.38270000000000048e-01};
  if (p1>4) return -1;
  if (p2>4) return -1;
  Float_t mass1 = kpmass[p1]; 
  Float_t mass2 = kpmass[p2];   
  const Double_t *m1 = fPmom;
  const Double_t *m2 = fNmom;
  //
  //if (fRP[p1]+fRM[p2]<fRP[p2]+fRM[p1]){
  //  m1 = fPM;
  //  m2 = fPP;
  //}
  //
  Float_t e1    = TMath::Sqrt(mass1*mass1+
                              m1[0]*m1[0]+
                              m1[1]*m1[1]+
                              m1[2]*m1[2]);
  Float_t e2    = TMath::Sqrt(mass2*mass2+
                              m2[0]*m2[0]+
                              m2[1]*m2[1]+
                              m2[2]*m2[2]);  
  Float_t mass =  
    (m2[0]+m1[0])*(m2[0]+m1[0])+
    (m2[1]+m1[1])*(m2[1]+m1[1])+
    (m2[2]+m1[2])*(m2[2]+m1[2]);
  
  mass = TMath::Sqrt((e1+e2)*(e1+e2)-mass);
  return mass;
}
