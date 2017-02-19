#include "RawHit_standard_merge_predicate.h"
#include "Global.h"
#include <iostream>
#include "EVENT/CalorimeterHit.h"
#include "Types.h"
bool RawHit_standard_merge_predicate::operator()(CalorimeterHit *A,CalorimeterHit *B)
{
  if (abs(A->getTime()-B->getTime())>m_neighbourTimeDistance) return false;
  if (Global::geom->GetDifType(decode(A)["DIF_Id"])==bif && Global::geom->GetDifType(decode(B)["DIF_Id"])==bif ) return true;
  if (Global::geom->GetDifType(decode(A)["DIF_Id"])==bif|| Global::geom->GetDifType(decode(B)["DIF_Id"])==bif ) return false;
  if (Global::geom->GetDifNbrPlate(decode(A)["DIF_Id"])!= Global::geom->GetDifNbrPlate(decode(B)["DIF_Id"])) return false; 
  if (Global::geom->GetDifType(decode(A)["DIF_Id"])==pad)
  {
      return IJ_connect(decode(A)["I"],decode(A)["J"],decode(B)["I"],decode(B)["J"]);
  }
  if (Global::geom->GetDifType(decode(A)["DIF_Id"])==stripup||Global::geom->GetDifType(decode(A)["DIF_Id"])==stripdown)
  {
    if (decode(A)["Asic_Id"]!= decode(B)["Asic_Id"]) return false;
    return abs(decode(A)["I"]-decode(B)["I"]) <= m_neighbourStripDistance;
  }
  return decode(A)["DIF_Id"]==decode(B)["DIF_Id"] && decode(A)["Asic_Id"]==decode(A)["Asic_Id"] && decode(A)["Channel"]==decode(A)["Channel"];
}

bool RawHit_standard_merge_predicate::IJ_connect(int I1, int J1, int I2, int J2)
{
  unsigned int distI=abs(I1-I2);
  unsigned int distJ=abs(J1-J2);
  if (m_padJunctionByCorner) return distI <= m_neighbourPadDistance && distJ <= m_neighbourPadDistance;
  else return distI+distJ <= m_neighbourPadDistance;
}
