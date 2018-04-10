#include "Converter.h"
#include <cmath>
#include <iostream>
#define degtorad 0.0174532925 

void Converter::CosSin(int K)
{
  if(Kactual==K) return;
  else 
  {
    Kactual=K;
    if(std::fabs(cos(geom->GetDifPlateAlpha(K)*degtorad))>0.001)ca=cos(geom->GetDifPlateAlpha(K)*degtorad);
    else ca=0;
    if(std::fabs(sin(geom->GetDifPlateAlpha(K)*degtorad))>0.001)sa=sin(geom->GetDifPlateAlpha(K)*degtorad);
    else sa=0;
    if(std::fabs(cos(geom->GetDifPlateBeta(K)*degtorad))>0.001)cb=cos(geom->GetDifPlateBeta(K)*degtorad);
    else cb=0;
    if(std::fabs(sin(geom->GetDifPlateBeta(K)*degtorad))>0.001)sb=sin(geom->GetDifPlateBeta(K)*degtorad);
    else sb=0;
    if(std::fabs(cos(geom->GetDifPlateGamma(K)*degtorad))>0.001)cg=cos(geom->GetDifPlateGamma(K)*degtorad);
    else cg=0;
    if(std::fabs(sin(geom->GetDifPlateGamma(K)*degtorad))>0.001)sg=sin(geom->GetDifPlateGamma(K)*degtorad);
    else sg=0;
  }
}
