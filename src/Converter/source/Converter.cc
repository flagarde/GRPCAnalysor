#include "Converter.h"
#include <cmath>
#define degtorad 0.0174532925 

void Converter::CosSin(int K)
{
  if(Kactual==K) return;
  else 
  {
    Kactual=K;
    ca=cos(geom->GetDifAlpha(K)*degtorad);
	  sa=sin(geom->GetDifAlpha(K)*degtorad);
    cb=cos(geom->GetDifBeta(K)*degtorad);
	  sb=sin(geom->GetDifBeta(K)*degtorad);
    cg=cos(geom->GetDifGamma(K)*degtorad);
	  sg=sin(geom->GetDifGamma(K)*degtorad);
	}
}
