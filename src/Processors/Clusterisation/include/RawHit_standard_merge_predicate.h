#pragma once
#include "EVENT/CalorimeterHit.h"
#include "UTIL/CellIDDecoder.h"
class RawHit_standard_merge_predicate
{
 public:
 RawHit_standard_merge_predicate(CellIDDecoder<CalorimeterHit>& decodeur):decode(decodeur),m_neighbourTimeDistance(1), m_neighbourStripDistance(2), m_neighbourPadDistance(1), m_padJunctionByCorner(false){};
  unsigned int getNeighbourTimeDistance() const {return m_neighbourTimeDistance;}
  unsigned int getNeighbourStripDistance() const {return m_neighbourStripDistance;}
  unsigned int getNeighbourPadDistance() const {return m_neighbourPadDistance;}
  bool getPadJunctionByCorner() const {return m_padJunctionByCorner;}
  void setNeighbourTimeDistance(unsigned int val) {m_neighbourTimeDistance=val;}
  void setNeighbourStripDistance(unsigned int val) {m_neighbourStripDistance=val;}
  void setNeighbourPadDistance(unsigned int val) {m_neighbourPadDistance=val;}
  void setPadJunctionByCorner(bool val=true) {m_padJunctionByCorner=val;}
  bool operator()(EVENT::CalorimeterHit* A,EVENT::CalorimeterHit* B);
 private:
  CellIDDecoder<CalorimeterHit>& decode;
  unsigned int m_neighbourTimeDistance;
  unsigned int m_neighbourStripDistance;
  unsigned int m_neighbourPadDistance;
  bool m_padJunctionByCorner;
  bool IJ_connect(int I1, int J1, int I2, int J2);
};
