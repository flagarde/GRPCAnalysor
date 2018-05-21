#pragma once
#include <vector> 
#include "Geometry.h"
#include "Converter.h"
class ConstructConverters
{
  public:
    ConstructConverters(Geometry* _geom):geom(_geom){};
    ~ConstructConverters()
    {
      for(std::map<int,Converter*>::iterator it=m_converter.begin();it!=m_converter.end();++it)
      {
        delete it->second;
      }
    };
    std::string getTypeString();
    void setType(int i);
    int RawToIInPlate(int Asic_Id,int Channel);
    int RawToJInPlate(int Asic_Id,int Channel);
    int RawToI(int Dif_Id,int Asic_Id,int Channel);
    int RawToJ(int Dif_Id,int Asic_Id,int Channel);
    int RawToK(int Dif_Id,int Asic_Id,int Channel);
    float IJKToX(int I,int J,int K);
    float IJKToY(int I,int J,int K);
    float IJKToZ(int I,int J,int K);
    std::vector<float> IJKToXYZ(int I,int J,int K);
    std::vector<int> RawToIJK(int Dif_Id,int Asic_Id,int Channel);
    /*std::vector<int> IJKToRaw(int I,int J,int K);*/
   /* std::vector<int> XYZToIJK(float X,float Y,float Z);*/
    int getType(){return type;};
  private:
    int type;
    Geometry* geom;
    std::map<int,Converter*>m_converter;
};
