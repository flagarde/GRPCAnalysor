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
  void Plot();
  private:
  ConfigInfos* conf;
  OutFileRoot* out;
  Geometry* geom;
  std::array<std::vector<TH2F*>,3>ThresholdMap;
  std::vector<TH2F*>Gain;
};
#endif
