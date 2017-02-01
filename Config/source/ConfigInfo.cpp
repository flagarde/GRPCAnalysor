#include <iostream>
#include "Colors.h"
#include <string>
#include <sstream>
#include <utility>
#include <map>
#include <array> 
#include "DifInfo.h"
#include "ConfigInfo.h"
#include <string>
void ConfigInfos::AddElogParam(std::string key ,std::string value)
{
  ElogParams.insert(std::pair<std::string,std::string>(key,value));
}

std::string ConfigInfos::ElogParam(std::string key)
{
  if(ElogParams.find(key)!=ElogParams.end()) return ElogParams[key];
  else return "";
}
  
void ConfigInfos::AddDif(DifInfo zz)
{
  DifInfos.insert(std::pair<unsigned int,DifInfo>(zz.getID(),zz)); 
}
  
void ConfigInfos::AddAsic(AsicInfo zz)
{
  DifInfos[zz.getDif_ID()].AddAsic(zz);
} 
  
std::vector<unsigned int> ConfigInfos::getThresholds(unsigned int& dif_id,unsigned int& asic_id)
{
  return (DifInfos.find(dif_id)->second).getAsicInfo(asic_id).getThresholds();
}
  
unsigned int ConfigInfos::getGain(unsigned int& dif_id,unsigned int& asic_id, unsigned& pad_id)
{
  return (DifInfos.find(dif_id)->second).getAsicInfo(asic_id).getGain(pad_id);
}
  
std::map<unsigned int,DifInfo> ConfigInfos::ReturnMe()
{
  return DifInfos;
} 
