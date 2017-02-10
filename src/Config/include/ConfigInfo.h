#ifndef CONFIGINFO
#define CONFIGINFO
#include <iostream>
#include "Colors.h"
#include <string>
#include <sstream>
#include <utility>
#include <string>
#include <map>
#include <array> 
#include "DifInfo.h"
#include "AsicInfo.h"
class ConfigInfos
{
  public:
  ConfigInfos(){};
  ~ConfigInfos()
  {
    DifInfos.clear();
    ElogParams.clear();   
  };
  void AddElogParam(std::string key ,std::string value);
  std::string ElogParam(std::string key);
  void AddDif(DifInfo zz);
  void AddAsic(AsicInfo zz);
  std::vector<unsigned int> getThresholds(unsigned int& dif_id,unsigned int& asic_id);
  unsigned int getGain(unsigned int& dif_id,unsigned int& asic_id, unsigned& pad_id);
  std::map<unsigned int,DifInfo>& ReturnMe();
  private:
  std::map<unsigned int,DifInfo>DifInfos;
  std::map<std::string,std::string>ElogParams;
};
#endif 
