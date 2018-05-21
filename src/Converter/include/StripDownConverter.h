#pragma once
#include "Converter.h"
#include <vector>
class StripDownConverter:public Converter
{
   public:
    StripDownConverter(Geometry* geom):Converter(geom){};
    ~StripDownConverter(){};
    virtual int RawToI(int Dif_Id,int Asic_Id,int Channel);
    virtual int RawToJ(int Dif_Id,int Asic_Id,int Channel);
    virtual int RawToK(int Dif_Id,int Asic_Id,int Channel);
    virtual int RawToIInPlate(int Asic_Id,int Channel);
    virtual int RawToJInPlate(int Asic_Id,int Channel);
    virtual float IJKToX(int I,int J,int K);
    virtual float IJKToY(int I,int J,int K);
    virtual float IJKToZ(int I,int J,int K);
}; 
