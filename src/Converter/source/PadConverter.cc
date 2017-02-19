#include "PadConverter.h"
#include <vector>
#include "Mapping.h"
#include <cmath>
#define degtorad 0.0174532925
#define size_pad 10.4125
int PadConverter::RawToI(int Dif_Id,int Asic_Id,int Channel)
{
  return (1+MapILargeHR2[Channel]+AsicShiftI[Asic_Id])+geom->GetDifPositionX(Dif_Id);
}

int PadConverter::RawToJ(int Dif_Id,int Asic_Id,int Channel)
{
  return (32-(MapJLargeHR2[Channel]+AsicShiftJ[Asic_Id]))+geom->GetDifPositionY(Dif_Id);
}

int PadConverter::RawToK(int Dif_Id,int Asic_Id,int Channel)
{
  return geom->GetDifNbrPlate(Dif_Id);
}

std::vector<float> PadConverter::IJKToXYZ(int I,int J,int K)
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
  Pos.push_back(cg*cb*I*size_pad+(-sg*ca+cg*sb*sa)*J*size_pad+(sg*sa+cg*sb*ca)*Z+geom->GetPlatePositionX(NbrPlate));
  Pos.push_back(sg*cb*I*size_pad+(cg*ca+sg*sb*sa)*J*size_pad+(-cg*sa+sg*sb*ca)*Z+geom->GetPlatePositionY(NbrPlate));
  Pos.push_back(-sb*I*size_pad+cb*sa*J*size_pad+cb*ca*Z);
  return Pos;
}

std::vector<int> PadConverter::RawToIJK(int Dif_Id,int Asic_Id,int Channel)
{
  return {RawToI(Dif_Id,Asic_Id,Channel),RawToJ(Dif_Id,Asic_Id,Channel),RawToK(Dif_Id,Asic_Id,Channel)};
}

std::vector<int> PadConverter::IJKToRaw(int I,int J,int K)
{
  return {0,0,0};
}
