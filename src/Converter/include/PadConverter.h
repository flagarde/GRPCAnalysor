#ifndef PADConverter
#define PADConverter
#include "Converter.h"
#include <vector>
class PadConverter:public Converter
{
   public:
    PadConverter(Geometry* geom):Converter(geom){};
    ~PadConverter(){};
    virtual int RawToI(int Dif_Id,int Asic_Id,int Channel);
    virtual int RawToJ(int Dif_Id,int Asic_Id,int Channel);
    virtual int RawToK(int Dif_Id,int Asic_Id,int Channel);
    virtual std::vector<double> IJKToXYZ(int I,int J,int K);
    virtual std::vector<int> RawToIJK(int Dif_Id,int Asic_Id,int Channel);
    virtual std::vector<int> IJKToRaw(int I,int J,int K);
}; 
#endif
