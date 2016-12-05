#ifndef DIFINFO
#define DIFINFO
#include <iostream>
#include <string>
#include <map>
#include <array> 
#include "AsicInfo.h"
class DifInfo
{
   public:
   DifInfo();
   DifInfo(std::string& name,std::string& dif_type,unsigned int& id,bool& enabled);
   std::string getDifType();
   std::string getName();
   bool getEnabled();
   unsigned int getID();
   void AddAsic(AsicInfo zz);
   AsicInfo getAsicInfo(unsigned int asic_id);
   std::map<unsigned int,AsicInfo> ReturnMe();
   private:
   std::string NAME, DIF_TYPE;
   unsigned int ID;
   bool ENABLED;
   std::map<unsigned int,AsicInfo> AsicInfos;
};
#endif
