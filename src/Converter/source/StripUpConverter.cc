#include "StripUpConverter.h"
#include <vector>
#include <cmath>
#define degtorad 0.0174532925
#define size_strip 2.5
#define length_strip 320
int StripUpConverter::RawToI(int Dif_Id,int Asic_Id,int Channel)
{
  return (2*Channel)+geom->GetDifPositionX(Dif_Id);
}

int StripUpConverter::RawToJ(int Dif_Id,int Asic_Id,int Channel)
{
  return Asic_Id;
}

int StripUpConverter::RawToK(int Dif_Id,int Asic_Id,int Channel)
{
  return geom->GetDifNbrPlate(Dif_Id);
}

std::vector<float> StripUpConverter::IJKToXYZ(int I,int J,int K)
{
  std::vector<float> Pos;
  unsigned int NbrPlate =K-1;
  double ca=cos(geom->GetDifAlpha(NbrPlate)*degtorad);
	double sa=sin(geom->GetDifAlpha(NbrPlate)*degtorad);
  double cb=cos(geom->GetDifBeta(NbrPlate)*degtorad);
	double sb=sin(geom->GetDifBeta(NbrPlate)*degtorad);
  double cg=cos(geom->GetDifGamma(NbrPlate)*degtorad);
	double sg=sin(geom->GetDifGamma(NbrPlate)*degtorad);
  double Z= geom->GetPlatePositionZ(NbrPlate);
  if(J%2==1) Pos.push_back(cg*cb*I*size_strip+(-sg*ca+cg*sb*sa)*J*size_strip+(sg*sa+cg*sb*ca)*Z+geom->GetPlatePositionX(NbrPlate)+1);
  else Pos.push_back(cg*cb*I*size_strip+(-sg*ca+cg*sb*sa)*J*size_strip+(sg*sa+cg*sb*ca)*Z+geom->GetPlatePositionX(NbrPlate));
  J=length_strip*1.0/2;
  Pos.push_back(sg*cb*I*size_strip+(cg*ca+sg*sb*sa)*J*size_strip+(-cg*sa+sg*sb*ca)*Z+geom->GetPlatePositionY(NbrPlate));
  Pos.push_back(-sb*I*size_strip+cb*sa*J*size_strip+cb*ca*Z);
  return Pos;
}

std::vector<int> StripUpConverter::RawToIJK(int Dif_Id,int Asic_Id,int Channel)
{
  return {RawToI(Dif_Id,Asic_Id,Channel),RawToJ(Dif_Id,Asic_Id,Channel),RawToK(Dif_Id,Asic_Id,Channel)};
}

std::vector<int> StripUpConverter::IJKToRaw(int I,int J,int K)
{
  return {0,0,0};
}
