#ifndef STRIPUpConverter
#define STRIPUpConverter
#include "Converter.h"
#include <vector>
class StripUpConverter:public Converter
{
   public:
    StripUpConverter(Geometry* geom):Converter(geom){};
    ~StripUpConverter(){};
    virtual int RawToI(int Dif_Id,int Asic_Id,int Channel);
    virtual int RawToJ(int Dif_Id,int Asic_Id,int Channel);
    virtual int RawToK(int Dif_Id,int Asic_Id,int Channel);
    virtual std::vector<float> IJKToXYZ(int I,int J,int K);
    virtual std::vector<int> RawToIJK(int Dif_Id,int Asic_Id,int Channel);
    virtual std::vector<int> IJKToRaw(int I,int J,int K);
}; 
#endif
