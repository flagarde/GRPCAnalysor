#include "Dif.h"

Dif::Dif(const double& x,const double& y, int DifId, const double& xy, const double& xz,const double& yz, const int& nbr, const int& up_down, const int& DifType):_PositionX(x),_PositionY(y),_DifId(DifId),_AngleXY(xy),_AngleXZ(xz),_AngleYZ(yz),_nbrPlate(nbr),_up_down(up_down),_DifType(DifType){};

double Dif::GetPositionX()
{
  return _PositionX;
}
  
double Dif::GetPositionY()
{
  return _PositionY;
}
  
int Dif::GetDifId()
{
  return _DifId;
}
  
double Dif::GetAngleXY()
{
  return _AngleXY;
}
  
double Dif::GetAngleXZ()
{
  return _AngleXZ;
}
  
double Dif::GetAngleYZ()
{
  return _AngleYZ;
}
  
int Dif::GetNbrPlate()
{
  return _nbrPlate;
}
  
int Dif::GetDifType()
{ 
  return _DifType;
}
  
int Dif::GetDifUpDown()
{
  return _up_down;
}
 
