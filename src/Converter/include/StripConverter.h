#ifndef STRIPConverter
#define STRIPConverter
#include "Converter.h"
#include <vector>
class StripConverter:public Converter
{
   public:
    StripConverter(Geometry* geom):Converter(geom){};
    ~StripConverter(){};
    virtual int RawToI(int Dif_Id,int Asic_Id,int Channel);
    virtual int RawToJ(int Dif_Id,int Asic_Id,int Channel);
    virtual int RawToK(int Dif_Id,int Asic_Id,int Channel);
    virtual std::vector<double> IJKToXYZ(int I,int J,int K);
    virtual std::vector<int> RawToIJK(int Dif_Id,int Asic_Id,int Channel);
    virtual std::vector<int> IJKToRaw(int I,int J,int K);
}; 
#endif
