#include <iostream>
#include "Colors.h"
#include <string>
#include <sstream>
#include <utility>
#include <map>
#include <array>
#include <vector>
#include "AsicInfo.h" 

AsicInfo::AsicInfo():ASIC_TYPE(""),B0(0),B1(0),B2(0),DIF_ID(0),ID(0),HEADER(0),NUM(0)
{
  PAGAIN.fill(0);
}

AsicInfo::AsicInfo(std::string& asic_type,std::array<unsigned int,64> pagain,unsigned int& b0,unsigned int& b1,unsigned int& b2,unsigned int& dif_id,unsigned int& id, unsigned int& header,unsigned int& num)
{
  ASIC_TYPE=asic_type;
  PAGAIN=pagain;
  B0=b0;
  B1=b1;
  B2=b2;
  DIF_ID=dif_id;
  ID=id;
  HEADER=header;
  NUM=num;
}

std::array<unsigned int,64>& AsicInfo::ReturnMe()
{
  return PAGAIN;
}

std::string AsicInfo::getAsicType()
{
  return ASIC_TYPE;
}

std::array<unsigned int,64>& AsicInfo::getGain()
{
  return PAGAIN;
}
   
unsigned int AsicInfo::getGain(unsigned int& index_pad)
{
  return PAGAIN[index_pad];
};
   
std::vector<unsigned int> AsicInfo::getThresholds()
{
  return std::vector<unsigned int>{B0,B1,B2};
}
   
unsigned int AsicInfo::getThreshold(int& threshold)
{
  if(threshold==0) return getThreshold0();
  if(threshold==1) return getThreshold1();
  if(threshold==2) return getThreshold2();
  else return -1;
}
   
unsigned int AsicInfo::getThreshold0()
{
  return B0;
}

unsigned int AsicInfo::getThreshold1()
{
  return B1;
}
   
unsigned int AsicInfo::getThreshold2()
{
  return B2;
}
   
unsigned int AsicInfo::getDif_ID()
{
  return DIF_ID;
}
   
unsigned int AsicInfo::getID()
{
  return ID;
}
   
unsigned int AsicInfo::getHeader()
{
  return HEADER;
}
   
unsigned int AsicInfo::getNUM()
{
  return NUM;
}
