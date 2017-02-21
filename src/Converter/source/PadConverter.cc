#include "PadConverter.h"
#include <vector>
#include "Mapping.h"
#include <cmath>
#define degtorad 0.0174532925
#define size_pad 10.4125

int PadConverter::RawToIInPlate(int Asic_Id,int Channel)
{
  return (1+MapILargeHR2[Channel]+AsicShiftI[Asic_Id]);
}

int PadConverter::RawToJInPlate(int Asic_Id,int Channel)
{
  return (32-(MapJLargeHR2[Channel]+AsicShiftJ[Asic_Id]));
}

int PadConverter::RawToI(int Dif_Id,int Asic_Id,int Channel)
{
  return RawToIInPlate(Asic_Id,Channel)+geom->GetDifPositionX(Dif_Id);
}

int PadConverter::RawToJ(int Dif_Id,int Asic_Id,int Channel)
{
  return RawToJInPlate(Asic_Id,Channel)+geom->GetDifPositionY(Dif_Id);
}

int PadConverter::RawToK(int Dif_Id,int Asic_Id,int Channel)
{
  return geom->GetDifNbrPlate(Dif_Id);
}

float PadConverter::IJKToX(int I,int J,int K)
{
  unsigned int NbrPlate =K-1;
  double ca=cos(geom->GetDifAlpha(NbrPlate)*degtorad);
	double sa=sin(geom->GetDifAlpha(NbrPlate)*degtorad);
  double cb=cos(geom->GetDifBeta(NbrPlate)*degtorad);
	double sb=sin(geom->GetDifBeta(NbrPlate)*degtorad);
  double cg=cos(geom->GetDifGamma(NbrPlate)*degtorad);
	double sg=sin(geom->GetDifGamma(NbrPlate)*degtorad);
  double Z= geom->GetPlatePositionZ(NbrPlate);
  return cg*cb*I*size_pad+(-sg*ca+cg*sb*sa)*J*size_pad+(sg*sa+cg*sb*ca)*Z+geom->GetPlatePositionX(NbrPlate);
}

float PadConverter::IJKToY(int I,int J,int K)
{
  unsigned int NbrPlate =K-1;
  double ca=cos(geom->GetDifAlpha(NbrPlate)*degtorad);
	double sa=sin(geom->GetDifAlpha(NbrPlate)*degtorad);
  double cb=cos(geom->GetDifBeta(NbrPlate)*degtorad);
	double sb=sin(geom->GetDifBeta(NbrPlate)*degtorad);
  double cg=cos(geom->GetDifGamma(NbrPlate)*degtorad);
	double sg=sin(geom->GetDifGamma(NbrPlate)*degtorad);
  double Z= geom->GetPlatePositionZ(NbrPlate);
  return sg*cb*I*size_pad+(cg*ca+sg*sb*sa)*J*size_pad+(-cg*sa+sg*sb*ca)*Z+geom->GetPlatePositionY(NbrPlate);
}
    
float PadConverter::IJKToZ(int I,int J,int K)
{
  unsigned int NbrPlate =K-1;
  double ca=cos(geom->GetDifAlpha(NbrPlate)*degtorad);
	double sa=sin(geom->GetDifAlpha(NbrPlate)*degtorad);
  double cb=cos(geom->GetDifBeta(NbrPlate)*degtorad);
	double sb=sin(geom->GetDifBeta(NbrPlate)*degtorad);
  double cg=cos(geom->GetDifGamma(NbrPlate)*degtorad);
	double sg=sin(geom->GetDifGamma(NbrPlate)*degtorad);
  double Z= geom->GetPlatePositionZ(NbrPlate);
  return -sb*I*size_pad+cb*sa*J*size_pad+cb*ca*Z;
}
