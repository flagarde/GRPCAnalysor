#ifndef HISTOGRAMMER_H
#define HISTOGRAMMER_H 
#include "OutFileRoot.h"
#include "TH2F.h"
#include "Geometry.h"
#include <map>
#include <array>
#include "ConfigInfo.h"
class Histogrammer
{
  public:
  Histogrammer(ConfigInfos* _conf,OutFileRoot* _out,Geometry* _geom);
  ~Histogrammer();
  void Plot();
  std::array<std::vector<TH2F*>,3> getThresholdMap(){return ThresholdMap;};
  std::vector<TH2F*> getGainMap(){return Gain;};
  double getThreshold(int I,int J, int K,int ThresholdNbr)
  {
    return ThresholdMap[ThresholdNbr][K]->GetBinContent(I,J);
  }
  double getGain(int I,int J, int K)
  {
    return Gain[K]->GetBinContent(I,J);
  }
  private:
  ConfigInfos* conf;
  OutFileRoot* out;
  Geometry* geom;
  std::array<std::vector<TH2F*>,3>ThresholdMap;
  std::array<std::vector<TH2F*>,3>ThresholdMapInt;
  std::vector<TH2F*>Gain;
};
#endif
