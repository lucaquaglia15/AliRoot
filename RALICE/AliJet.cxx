#include "AliJet.h"
 
ClassImp(AliJet) // Class implementation to enable ROOT I/O
 
AliJet::AliJet()
{
// Default constructor
// All variables initialised to 0
// Initial maximum number of tracks is set to the default value
 fTracks=0;
 fNtinit=0;
 Reset();
 SetNtinit();
}
///////////////////////////////////////////////////////////////////////////
AliJet::AliJet(Int_t n)
{
// Create a jet to hold initially a maximum of n tracks
// All variables initialised to 0
 fTracks=0;
 fNtinit=0;
 Reset();
 if (n > 0)
 {
  SetNtinit(n);
 }
 else
 {
  cout << endl;
  cout << " *AliJet* Initial max. number of tracks entered : " << n << endl;
  cout << " This is invalid. Default initial maximum will be used." << endl;
  cout << endl;
  SetNtinit();
 }
}
///////////////////////////////////////////////////////////////////////////
AliJet::~AliJet()
{
// Default destructor
 if (fTracks) delete fTracks;
 fTracks=0;
}
///////////////////////////////////////////////////////////////////////////
void AliJet::SetNtinit(Int_t n)
{
// Set the initial maximum number of tracks for this jet
 fNtinit=n;
 fNtmax=n;
 if (fTracks) delete fTracks;
 fTracks=new TObjArray(fNtmax);
}
///////////////////////////////////////////////////////////////////////////
void AliJet::Reset()
{
// Reset all variables to 0
// The max. number of tracks is set to the initial value again
 fNtrk=0;
 fQ=0;
 Double_t a[4]={0,0,0,0};
 SetVector(a,"sph");
 if (fNtinit > 0) SetNtinit(fNtinit);
}
///////////////////////////////////////////////////////////////////////////
void AliJet::Add(AliTrack& t)
{
// Add a track to the jet
// In case the maximum number of tracks has been reached
// space will be extended to hold an additional amount of tracks as
// was initially reserved
 if (fNtrk == fNtmax) // Check if maximum track number is reached
 {
  fNtmax+=fNtinit;
  fTracks->Expand(fNtmax);
 }
 
 // Add the track to this jet
 fNtrk++;
 fTracks->Add(&t);
 (*this)+=(Ali4Vector&)t;
 fQ+=t.GetCharge();
}
///////////////////////////////////////////////////////////////////////////
void AliJet::Info(TString f)
{
// Provide jet information within the coordinate frame f
 cout << " *AliJet::Info* Invmass : " << GetInvmass() << " Charge : " << fQ
      << " Momentum : " << GetMomentum() << " Ntracks : " << fNtrk << endl;
 cout << " ";
 Ali4Vector::Info(f); 
} 
///////////////////////////////////////////////////////////////////////////
void AliJet::List(TString f)
{
// Provide jet and primary track information within the coordinate frame f

 Info(f); // Information of the current jet

 // The tracks of this jet
 AliTrack* t; 
 for (Int_t it=1; it<=fNtrk; it++)
 {
  t=GetTrack(it);
  if (t)
  {
   cout << "  ---Track no. " << it << endl;
   cout << " ";
   t->Info(f); 
  }
  else
  {
   cout << " *AliJet::List* Error : No track present." << endl; 
  }
 }
} 
///////////////////////////////////////////////////////////////////////////
void AliJet::ListAll(TString f)
{
// Provide jet and prim.+sec. track information within the coordinate frame f

 Info(f); // Information of the current jet

 // The tracks of this jet
 AliTrack* t; 
 for (Int_t it=1; it<=fNtrk; it++)
 {
  t=GetTrack(it);
  if (t)
  {
   cout << "  ---Track no. " << it << endl;
   cout << " ";
   t->ListAll(f); 
  }
  else
  {
   cout << " *AliJet::List* Error : No track present." << endl; 
  }
 }
} 
///////////////////////////////////////////////////////////////////////////
Int_t AliJet::GetNtracks()
{
// Return the current number of tracks of this jet
 return fNtrk;
}
///////////////////////////////////////////////////////////////////////////
Double_t AliJet::GetEnergy()
{
// Return the total energy of the jet
 return GetScalar();
}
///////////////////////////////////////////////////////////////////////////
Double_t AliJet::GetMomentum()
{
// Return the value of the total jet 3-momentum
 Ali3Vector p=Get3Vector();
 Double_t p2=p.Dot(p);
 return sqrt(p2);
}
///////////////////////////////////////////////////////////////////////////
Ali3Vector AliJet::Get3Momentum()
{
// Return the the total jet 3-momentum
 Ali3Vector p=Get3Vector();
 return p;
}
///////////////////////////////////////////////////////////////////////////
Double_t AliJet::GetInvmass()
{
// Return the invariant mass of the jet
 Double_t m2=Dot(*this);
 if (m2>0)
 {
  return sqrt(m2);
 }
 else
 {
  return 0;
 }
}
///////////////////////////////////////////////////////////////////////////
Float_t AliJet::GetCharge()
{
// Return the total charge of the jet
 return fQ;
}
///////////////////////////////////////////////////////////////////////////
AliTrack* AliJet::GetTrack(Int_t i)
{
// Return the i-th track of this jet
 return (AliTrack*)fTracks->At(i-1);
}
///////////////////////////////////////////////////////////////////////////
