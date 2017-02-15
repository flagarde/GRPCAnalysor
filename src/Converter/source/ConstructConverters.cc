#include "ConstructConverters.h"
#include "StripConverter.h"
#include "PadConverter.h"
#include <vector>
#include "Types.h"
#include "Geometry.h"
#include <string>  
void ConstructConverters::setType(int Dif_Id)
{
  if(m_converter!=nullptr) delete m_converter;
  type=geom->GetDifType(Dif_Id);
  switch(geom->GetDifType(Dif_Id)) 
  {
    case positional:
      m_converter=new StripConverter(geom);
    break;
    default:
      m_converter=new PadConverter(geom);
    break;
  }
}

std::string ConstructConverters::getTypeString()
{
  return Types_Dif[type];
}

int ConstructConverters::RawToI(int Dif_Id,int Asic_Id,int Channel)
{
  return m_converter->RawToI(Dif_Id,Asic_Id,Channel);
}

int ConstructConverters::RawToJ(int Dif_Id,int Asic_Id,int Channel)
{
  return m_converter->RawToJ(Dif_Id,Asic_Id,Channel);
}

int ConstructConverters::RawToK(int Dif_Id,int Asic_Id,int Channel)
{
  return m_converter->RawToK(Dif_Id,Asic_Id,Channel);
}

std::vector<double> ConstructConverters::IJKToXYZ(int I,int J,int K)
{
  return m_converter->IJKToXYZ(I,J,K);
}

std::vector<int> ConstructConverters::RawToIJK(int Dif_Id,int Asic_Id,int Channel)
{
  return m_converter->RawToIJK(Dif_Id,Asic_Id,Channel);
}
   
std::vector<int> ConstructConverters::IJKToRaw(int I,int J,int K)
{
  return m_converter->IJKToRaw(I,J,K);
}
