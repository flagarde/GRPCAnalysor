#include "Dif.h"
#include "Types.h"
Dif::Dif(const double& x,const double& y, int DifId, const double& xy, const double& xz,const double& yz, const int& nbr, const int& DifType):_PositionX(x),_PositionY(y),_DifId(DifId),_AngleXY(xy),_AngleXZ(xz),_AngleYZ(yz),_nbrPlate(nbr),_DifType(DifType)
{
  //In cm
  switch ( DifType ) 
  {
    case pad:
      _AsicNbr=24;
      _WidthCell=1.0;
      _LengthCell=1.0;
      _InterCellWidth=0.04125;
      _InterCellLength=0.04125;
      break;
    case strip:
      _AsicNbr=2;
      _WidthCell=0.2;
      _LengthCell=32.0;
      _InterCellWidth=0.05;
      _InterCellLength=0.;
      break;
    case stripup:
      _AsicNbr=2;
      _WidthCell=0.2;
      _LengthCell=32.0;
      _InterCellWidth=0.05;
      _InterCellLength=0.;
      break;
    case stripdown:
      _AsicNbr=2;
      _WidthCell=0.2;
      _LengthCell=32.0;
      _InterCellWidth=0.05;
      _InterCellLength=0.;
      break;
    case  temporal:
      _AsicNbr=2;
      _WidthCell=1;
      _LengthCell=1;
      _InterCellWidth=0;
      _InterCellLength=0;
      break;
    case tcherenkov:
      _AsicNbr=1;
      _WidthCell=1;
      _LengthCell=1;
      _InterCellWidth=0;
      _InterCellLength=0;
      break;
    case tricot:
      _AsicNbr=3;
      _WidthCell=1;
      _LengthCell=1;
      _InterCellWidth=0;
      _InterCellLength=0;
      break;
    case scintillator:
      _AsicNbr=1;
      _WidthCell=1;
      _LengthCell=1;
      _InterCellWidth=0;
      _InterCellLength=0;
      break;
    case bif:
      _AsicNbr=1;
      _WidthCell=1;
      _LengthCell=1;
      _InterCellWidth=0;
      _InterCellLength=0;
      break;
    case m3:
      _AsicNbr=48;
      _WidthCell=1.0;
      _LengthCell=1.0;
      _InterCellWidth=0.04125;
      _InterCellLength=0.04125;
      break;
    case CMS:
      _AsicNbr=4;
      _WidthCell=6;
      _LengthCell=160.5;
      _InterCellWidth=0.0;
      _InterCellLength=0.0;
    default:
      _AsicNbr=48;
      _WidthCell=1.0;
      _LengthCell=1.0;
      _InterCellWidth=0.04125;
      _InterCellLength=0.04125;
      break;
  }
};

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

int Dif::GetAsicNbr()
{
  return _AsicNbr;
}

double Dif::GetWidthCell()
{
  return _WidthCell;
}

double Dif::GetLengthCell()
{
  return _LengthCell;
}

double Dif::GetAreaCell()
{
  return _LengthCell*_WidthCell;
}

double Dif::GetInterCellWidth()
{
  return _InterCellWidth;
}

double Dif::GetInterCellLength()
{
  return _InterCellLength;
}

double Dif::GetArea()
{
  return (_LengthCell+_InterCellLength)*(_WidthCell+_InterCellWidth);
}

double Dif::GetTotalArea()
{
  return (_LengthCell+_InterCellLength)*(_WidthCell+_InterCellWidth)*_AsicNbr*64;
}
