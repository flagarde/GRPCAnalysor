#include "Plate.h"
#include <vector>
Plate::Plate(const Plate& _Plate):_PositionX(_Plate._PositionX),_PositionY(_Plate._PositionY),_PositionZ(_Plate._PositionZ),_AngleXY(_Plate._AngleXY),_AngleXZ(_Plate._AngleXZ),_AngleYZ(_Plate._AngleYZ),_DifInPlate(_Plate._DifInPlate),_SizeX(_Plate._SizeX),_SizeY(_Plate._SizeY),m_glass_type(_Plate.m_glass_type),m_Gaz_number(_Plate.m_Gaz_number),m_HV_channel(_Plate.m_HV_channel),m_Gaz_channel(_Plate.m_Gaz_channel){}

Plate::Plate(const double& x,const double& y,const double& z, const double& xy,const double& xz,const double& yz,std::vector<int>DifInPlate,const double& sizeX, const double& sizeY,const double& glass_type, const double& Gaz_number,const std::string& HV_channel,const std::string& Gaz_channel):_PositionX(x),_PositionY(y),_PositionZ(z),_AngleXY(xy),_AngleXZ(xz),_AngleYZ(yz),_DifInPlate(DifInPlate),_SizeX(sizeX),_SizeY(sizeY),m_glass_type(glass_type),m_Gaz_number(Gaz_number),m_HV_channel(HV_channel),m_Gaz_channel(Gaz_channel){}

double Plate::GetPositionX()
{
  return _PositionX;
}

double Plate::GetPositionY()
{
  return _PositionY;
}

double Plate::GetPositionZ()
{
  return _PositionZ;
}

double Plate::GetAngleXY()
{
  return _AngleXY;
}

double Plate::GetAngleXZ()
{
  return _AngleXZ;
}

double Plate::GetAngleYZ()
{
  return _AngleYZ;
}

int Plate::GetNbrDifInPlate()
{
  return _DifInPlate.size();
}

std::vector<int> Plate::GetDifInPlate()
{
  return _DifInPlate;
}

double Plate::GetSizeX()
{
  return _SizeX;
}

double Plate::GetSizeY()
{
  return _SizeY;
}

double Plate::GetGlassType()
{
  return m_glass_type;
}

double Plate::GetGazNumber()
{
  return m_Gaz_number;
}

std::string Plate::GetHVChannel()
{
  return m_HV_channel;
}

std::string Plate::GetGazChannel()
{
  return m_Gaz_channel;
} 
