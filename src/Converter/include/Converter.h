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
    virtual int RawToIInPlate(int Asic_Id,int Channel)=0;
    virtual int RawToJInPlate(int Asic_Id,int Channel)=0;
    virtual float IJKToX(int I,int J,int K)=0;
    virtual float IJKToY(int I,int J,int K)=0;
    virtual float IJKToZ(int I,int J,int K)=0;
    void CosSin(int K);
   protected:
    Geometry* geom;
    double ca;
	  double sa;
    double cb;
	  double sb;
    double cg;
	  double sg;
}; 
#endif
