#include "StripUpConverter.h"
#include <vector>
#include <cmath>
#define degtorad 0.0174532925
#define size_strip 2.5
#define length_strip 320

int StripUpConverter::RawToIInPlate(int Asic_Id,int Channel)
{
  return (2*Channel);
}

int StripUpConverter::RawToJInPlate(int Asic_Id,int Channel)
{
  return Asic_Id;
}

int StripUpConverter::RawToI(int Dif_Id,int Asic_Id,int Channel)
{
  return RawToIInPlate(Asic_Id,Channel)+geom->GetDifPositionX(Dif_Id);
}

int StripUpConverter::RawToJ(int Dif_Id,int Asic_Id,int Channel)
{
  return RawToJInPlate(Asic_Id,Channel)+geom->GetDifPositionY(Dif_Id);
}

int StripUpConverter::RawToK(int Dif_Id,int Asic_Id,int Channel)
{
  return geom->GetDifNbrPlate(Dif_Id);
}

float StripUpConverter::IJKToX(int I,int J,int K)
{
  unsigned int NbrPlate =K-1;
  double ca=cos(geom->GetDifAlpha(NbrPlate)*degtorad);
	double sa=sin(geom->GetDifAlpha(NbrPlate)*degtorad);
  double cb=cos(geom->GetDifBeta(NbrPlate)*degtorad);
	double sb=sin(geom->GetDifBeta(NbrPlate)*degtorad);
  double cg=cos(geom->GetDifGamma(NbrPlate)*degtorad);
	double sg=sin(geom->GetDifGamma(NbrPlate)*degtorad);
	double Z= geom->GetPlatePositionZ(NbrPlate);
	if(J%2==1) return cg*cb*I*size_strip+(-sg*ca+cg*sb*sa)*J*size_strip+(sg*sa+cg*sb*ca)*Z+geom->GetPlatePositionX(NbrPlate)+1 ;
  else return cg*cb*I*size_strip+(-sg*ca+cg*sb*sa)*J*size_strip+(sg*sa+cg*sb*ca)*Z+geom->GetPlatePositionX(NbrPlate);
}

float StripUpConverter::IJKToY(int I,int J,int K)
{
  unsigned int NbrPlate =K-1;
  double ca=cos(geom->GetDifAlpha(NbrPlate)*degtorad);
	double sa=sin(geom->GetDifAlpha(NbrPlate)*degtorad);
  double cb=cos(geom->GetDifBeta(NbrPlate)*degtorad);
	double sb=sin(geom->GetDifBeta(NbrPlate)*degtorad);
  double cg=cos(geom->GetDifGamma(NbrPlate)*degtorad);
	double sg=sin(geom->GetDifGamma(NbrPlate)*degtorad);
	double Z= geom->GetPlatePositionZ(NbrPlate);
	J=length_strip*1.0/2;
  return sg*cb*I*size_strip+(cg*ca+sg*sb*sa)*J*size_strip+(-cg*sa+sg*sb*ca)*Z+geom->GetPlatePositionY(NbrPlate);
}
    
float StripUpConverter::IJKToZ(int I,int J,int K)
{
  unsigned int NbrPlate =K-1;
  double ca=cos(geom->GetDifAlpha(NbrPlate)*degtorad);
	double sa=sin(geom->GetDifAlpha(NbrPlate)*degtorad);
  double cb=cos(geom->GetDifBeta(NbrPlate)*degtorad);
	double sb=sin(geom->GetDifBeta(NbrPlate)*degtorad);
  double cg=cos(geom->GetDifGamma(NbrPlate)*degtorad);
	double sg=sin(geom->GetDifGamma(NbrPlate)*degtorad);
  double Z= geom->GetPlatePositionZ(NbrPlate);
  return -sb*I*size_strip+cb*sa*J*size_strip+cb*ca*Z;
}
