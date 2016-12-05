#include "DifInfo.h"
#include "AsicInfo.h"
#include <iostream>
#include "Colors.h"
#include <string>
#include <sstream>
#include <utility>
#include <map>
#include <array> 

DifInfo::DifInfo():NAME(""),DIF_TYPE(""),ID(-1),ENABLED(false){};

DifInfo::DifInfo(std::string& name,std::string& dif_type,unsigned int& id,bool& enabled):NAME(name),DIF_TYPE(dif_type),ID(id),ENABLED(enabled){};

std::string DifInfo::getDifType()
{
  return DIF_TYPE;
}

std::string DifInfo::getName()
{
  return NAME;
}
   
bool DifInfo::getEnabled()
{
  return ENABLED;
}
   
unsigned int DifInfo::getID()
{
  return ID;
}
   
void DifInfo::AddAsic(AsicInfo zz)
{
  AsicInfos.insert(std::pair<unsigned int,AsicInfo>(zz.getHeader(),zz));
};
   
AsicInfo DifInfo::getAsicInfo(unsigned int asic_id)
{
  if(AsicInfos.find(asic_id)!=AsicInfos.end())return AsicInfos.find(asic_id)->second;
  else return AsicInfo();
}
   
std::map<unsigned int,AsicInfo> DifInfo::ReturnMe()
{
  return AsicInfos;
}

