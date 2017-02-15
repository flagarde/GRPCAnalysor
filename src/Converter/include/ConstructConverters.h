#ifndef ConstructConverter
#define ConstructConverter
#include <vector> 
#include "Geometry.h"
#include "Converter.h"
class ConstructConverters
{
  public:
    ConstructConverters(Geometry* _geom):geom(_geom){};
    ~ConstructConverters()
    {
      if(m_converter!=nullptr) delete m_converter  ;
    };
    std::string getTypeString();
    void setType(int i);
    int RawToI(int Dif_Id,int Asic_Id,int Channel);
    int RawToJ(int Dif_Id,int Asic_Id,int Channel);
    int RawToK(int Dif_Id,int Asic_Id,int Channel);
    std::vector<double> IJKToXYZ(int I,int J,int K);
    std::vector<int> RawToIJK(int Dif_Id,int Asic_Id,int Channel);
    std::vector<int> IJKToRaw(int I,int J,int K);
    int getType(){return type;};
  private:
    int type;
    Converter* m_converter;
    Geometry* geom;
};
#endif
