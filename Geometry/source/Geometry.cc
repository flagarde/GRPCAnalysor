#include"Geometry.h"
#include"Colors.h"
#include<iostream>
#include<vector>
#include<string>
#include <sstream>
inline std::string Shift(double val)
{
  std::stringstream ss;
	if(val<10) ss<<"  "<<val;
	if(val>=10&&val<1000) ss<<" "<<val;
	if(val>=1000&&val<10000) ss<<val;
  return ss.str();
}

void Geometry::PrintGeom()
{ 
  std::vector<std::string>glass_types{"standard","chinese"};
  for(unsigned int i=0;i<GetNumberPlates();++i)
  {
    std::cout<<"Plate : "<<Shift(i+1)
             <<"  "<<green<<"X : "<<Shift(GetPlatePositionX(i))<<"  Y : "<<Shift(GetPlatePositionY(i))<<"  Z : "<<Shift(GetPlatePositionZ(i))
             <<" Alpha : "<<Shift(GetDifPlateAlpha(i))<<" Beta : "<<Shift(GetDifPlateBeta(i))<<" Gamma : "<<Shift(GetDifPlateGamma(i))
             <<" SizeX : "<<Shift(GetSizeX(i))<<" SizeY : "<<Shift(GetSizeY(i))<<normal<<std::endl;
    if(GetGlassType(i)!=-1)std::cout<<green<<" Glass type : "<<glass_types[GetGlassType(i)]<<" "<<normal;
    else std::cout<<red<<" Glass type : UNKNOW "<<normal;
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
