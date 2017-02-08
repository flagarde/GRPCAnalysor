#include "Geometry.h"
#include "Colors.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <limits>
#include "Utilities.h"

void Geometry::PrintGeom()
{ 
  std::vector<std::string>glass_types{"standard","chinese"};
  for(unsigned int i=0;i<GetNumberPlates();++i)
  {
    std::cout<<"Plate : "<<Shift(i+1)
             <<"  "<<green<<"X : "<<Shift(GetPlatePositionX(i))<<"  Y : "<<Shift(GetPlatePositionY(i))<<"  Z : "<<Shift(GetPlatePositionZ(i))
             <<" Alpha : "<<Shift(GetDifPlateAlpha(i))<<" Beta : "<<Shift(GetDifPlateBeta(i))<<" Gamma : "<<Shift(GetDifPlateGamma(i))
             <<" SizeX : "<<Shift(GetSizeX(i))<<" SizeY : "<<Shift(GetSizeY(i))<<normal<<std::endl;
    std::cout<<green<<" Electrode type : "<<GetGlassType(i)<<" "<<normal;
    if(GetHVChannel(i)!="")std::cout<<green<<" HV_Channel : "<<GetHVChannel(i)<<" "<<normal;
    if(GetGazChannel(i)!="")std::cout<<green<<" Gaz_Channel : "<<GetGazChannel(i)<<" "<<normal;
    if(GetGazNumber(i)!=-1)std::cout<<green<<" ,Position in the gaz circuit : "<<GetGazNumber(i)<<" "<<normal;
    std::cout<<std::endl;
    for(std::map<int,Dif>::iterator it=Difs.begin();it!=Difs.end();++it)
    {
      if((unsigned int)( GetDifNbrPlate((it->first))-1)==i)
        std::cout<<green<<" DifId : "<<Shift((it->second).GetDifId())
        <<" I : "<<Shift((it->second).GetPositionX())<<"  J : "<<Shift((it->second).GetPositionY())
        <<" DifType : "<<GetDifTypeName((it->second).GetDifId())<<normal<<std::endl;
    }
  }  
}

void Geometry::AddDif(const double& x,const double& y, int DifId,const double& xy ,const double& xz,const double& yz,const int& nbr, const int& up_down,const int& DifType)
{
  Difs[DifId]=Dif(x,y,DifId,xy,xz,yz,nbr,up_down,DifType);
}

void Geometry::AddPlate(const double& x,const double& y,const double& z,const double& xy ,const double& xz ,const double& yz , std::vector<int>&DifInPlate,const double& sizeX, const double& sizeY,const double& glass_type, const double& Gaz_number,const std::string& HV_channel,const std::string& Gaz_channel)
{
  Plates.push_back(Plate(x,y,z,xy,xz,yz,DifInPlate,sizeX,sizeY,glass_type,Gaz_number,HV_channel,Gaz_channel));
}
  
const double Geometry::GetDifPositionX( int& i)
{
  return ((Difs.find(i))->second).GetPositionX();
}

const double Geometry::GetDifPositionY( int& i)
{
  return ((Difs.find(i))->second).GetPositionY();
}
    
const double Geometry::GetDifAlpha( const int& i)
{ 
  return ((Difs.find(i))->second).GetAngleXY();
}

const double Geometry::GetDifBeta( const int& i)
{ 
  return ((Difs.find(i))->second).GetAngleXZ();
}
    
const double Geometry::GetDifGamma( const int& i)
{ 
  return ((Difs.find(i))->second).GetAngleYZ();
}

const double Geometry::GetDifPositionXMax(const int& i )
{
  unsigned currentMax = 0;
  for(std::map<int, Dif >::iterator it = Difs.begin(); it != Difs.end(); ++it ) 
  {
    if ((it->second).GetNbrPlate()==i && (it ->second).GetPositionX() > currentMax) 
    {
      currentMax = (it ->second).GetPositionX();
    }
  } 
  return currentMax;
}
    
const double Geometry::GetDifPositionYMax(const int& i )
{
  unsigned currentMax = 0;
  for(std::map<int, Dif >::iterator it = Difs.begin(); it != Difs.end(); ++it ) 
  {
    if ((it->second).GetNbrPlate()==i && (it ->second).GetPositionY() > currentMax) 
    {
      currentMax = (it ->second).GetPositionY();
    }
  } 
  return currentMax;
}
    
const double Geometry::GetDifPlateAlpha( const int& i)
{ 
  return Plates[i].GetAngleXY();
}

const double Geometry::GetDifPlateBeta( const int& i)
{ 
  return Plates[i].GetAngleXZ();
}
    
const double Geometry::GetDifPlateGamma( const int& i)
{ 
  return Plates[i].GetAngleYZ();
}

const int Geometry::GetDifId( int& i)
{ 
  return ((Difs.find(i))->second).GetDifId();
}

const int Geometry::GetNbrDifInPlate(int& i)
{
  return  Plates[i].GetNbrDifInPlate();
}
    
const double Geometry::GetPlatePositionX(const unsigned int& i)
{ 
  return Plates[i].GetPositionX();
}
    
const double Geometry::GetSizeX(const unsigned int& i)
{ 
  return Plates[i].GetSizeX();
}
    
const double Geometry::GetSizeY(const unsigned int& i)
{ 
  return Plates[i].GetSizeY();
}

const double Geometry::GetPlatePositionY(const unsigned int& i)
{ 
  return Plates[i].GetPositionY();
}
    
const double Geometry::GetPlatePositionZ(const unsigned int& i)
{ 
  return Plates[i].GetPositionZ();
}

const int Geometry::GetDifNbrPlate( const int& i)
{ 
  if(Difs.find(i)==Difs.end())return -1; 
  else return ((Difs.find(i))->second).GetNbrPlate();
}
    
const int Geometry::GetDifType( const int& i)
{ 
  return ((Difs.find(i))->second).GetDifType();
}

const std::string Geometry::GetDifTypeName( const int& i)
{
  if(Difs.find(i)==Difs.end())return "Unknown Difs";
  else if (((Difs.find(i))->second).GetDifType()==-1)return red+"UNKNOWN"+normal;
  else return Types_Dif[((Difs.find(i))->second).GetDifType()];
}
    
const int Geometry::GetDifUpDown( int& i)
{ 
  return ((Difs.find(i))->second).GetDifUpDown();
}

const unsigned int Geometry::GetNumberDifs()
{
  return Difs.size();
}
    
const unsigned int Geometry::GetNumberPlates()
{
  return Plates.size();
}
    
const std::vector<Plate> Geometry::GetPlates()
{
  return Plates;
}
    
const Plate Geometry::GetPlate(const int& i)
{
  return Plates[i];
}
    
const std::map<int,Dif> Geometry::GetDifs()
{
  return Difs;
}
    
const std::vector<int> Geometry::GetDifsInPlane(unsigned int& i)
{
  return Plates[i].GetDifInPlate();
}
    
const std::vector<int> Geometry::GetDifsList()
{
  std::vector<int>DifsList;
  for(unsigned int y=0;y!=Plates.size();++y)
  {
    DifsList.insert(DifsList.end(), Plates[y].GetDifInPlate().begin(), Plates[y].GetDifInPlate().end());
  }
  return DifsList;
}
  
const std::string Geometry::GetGlassType(const unsigned int& i)
{
  if(Plates[i].GetGlassType()==-1) return red+"UNKNOWN"+normal;
  else return Types_Electrode[Plates[i].GetGlassType()];
}
    
double Geometry::GetGazNumber(const unsigned int& i)
{
  return Plates[i].GetGazNumber();
}
    
std::string Geometry::GetHVChannel(const unsigned int& i)
{
  return Plates[i].GetHVChannel();
}
    
std::string Geometry::GetGazChannel(const unsigned int& i)
{
  return Plates[i].GetGazChannel();
}

int Geometry::GetXMax()
{
  int max=std::numeric_limits<int>::min();
  for(unsigned int i=0;i!=Plates.size();++i)
  {
    if(Plates[i].GetSizeX()>max) max=Plates[i].GetSizeX();
  }
  return max;
}
    
int Geometry::GetYMax()
{
  int max=std::numeric_limits<int>::min();
  for(unsigned int i=0;i!=Plates.size();++i)
  {
    if(Plates[i].GetSizeY()>max) max=Plates[i].GetSizeY();
  }
  return max;
}
