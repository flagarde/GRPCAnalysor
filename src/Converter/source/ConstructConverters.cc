#include "ConstructConverters.h"
#include "StripDownConverter.h"
#include "StripUpConverter.h"
#include "PadConverter.h"
#include <vector>
#include "Types.h"
#include "Geometry.h"
#include <string>  
#include <iostream>
#include "Colors.h"
void ConstructConverters::setType(int Dif_Id)
{
  type=geom->GetDifType(Dif_Id);
  if(m_converter.find(type)==m_converter.end())
  {
    switch(type) 
    {
      case stripup:
        m_converter[type]=new StripUpConverter(geom);
      break;
      case stripdown:
        m_converter[type]=new StripDownConverter(geom);
      break;
      default:
        m_converter[type]=new PadConverter(geom);
      break;
    }
  }
}

std::string ConstructConverters::getTypeString()
{
  return Types_Dif[type];
}

int ConstructConverters::RawToI(int Dif_Id,int Asic_Id,int Channel)
{
  return m_converter[type]->RawToI(Dif_Id,Asic_Id,Channel);
}

int ConstructConverters::RawToJ(int Dif_Id,int Asic_Id,int Channel)
{
  return m_converter[type]->RawToJ(Dif_Id,Asic_Id,Channel);
}

int ConstructConverters::RawToK(int Dif_Id,int Asic_Id,int Channel)
{
  return m_converter[type]->RawToK(Dif_Id,Asic_Id,Channel);
}

std::vector<float> ConstructConverters::IJKToXYZ(int I,int J,int K)
{
  return m_converter[type]->IJKToXYZ(I,J,K);
}

std::vector<int> ConstructConverters::RawToIJK(int Dif_Id,int Asic_Id,int Channel)
{
  return m_converter[type]->RawToIJK(Dif_Id,Asic_Id,Channel);
}
   
std::vector<int> ConstructConverters::IJKToRaw(int I,int J,int K)
{
  return m_converter[type]->IJKToRaw(I,J,K);
}
