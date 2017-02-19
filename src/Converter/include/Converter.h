#ifndef Converter_h
#define Converter_h
#include "Geometry.h"
#include <vector>
class Converter
{
   public:
    Converter(Geometry* _geom):geom(_geom){};
    virtual ~Converter(){};
    virtual int RawToI(int Dif_Id,int Asic_Id,int Channel)=0;
    virtual int RawToJ(int Dif_Id,int Asic_Id,int Channel)=0;
    virtual int RawToK(int Dif_Id,int Asic_Id,int Channel)=0;
    virtual std::vector<float> IJKToXYZ(int I,int J,int K)=0;
    virtual std::vector<int> RawToIJK(int Dif_Id,int Asic_Id,int Channel)=0;
    virtual std::vector<int> IJKToRaw(int I,int J,int K)=0;
   protected:
    Geometry* geom;
}; 
#endif
