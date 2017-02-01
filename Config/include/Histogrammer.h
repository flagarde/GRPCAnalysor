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
  Histogrammer(ConfigInfos* _conf,OutFileRoot* _out,Geometry* _geom)
  {
    conf=_conf;
    out=_out;
    geom=_geom;
    for(unsigned int i=0;i!=geom->GetNumberPlates();++i)
    {
      int X=geom->GetSizeX(i)+1;
      int Y=geom->GetSizeY(i)+1;
      std::string h="Threshold_"+ std::to_string(i +1 );
      std::string hh="gain_"+ std::to_string(i +1 );
      for(unsigned int j=0;j<ThresholdMap.size();++j)
      {
        ThresholdMap[j].push_back(new TH2F((h+"_"+Thresholds_name[j]).c_str(),(h+"_"+Thresholds_name[j]).c_str(),X,0,X,Y,0,Y));
      }
      Gain.push_back(new TH2F(hh.c_str(),hh.c_str(),X,0,X,Y,0,Y));
    }
    Plot();
  }
  void Plot();
  private:
  ConfigInfos* conf;
  OutFileRoot* out;
  Geometry* geom;
  std::array<std::vector<TH2F*>,3>ThresholdMap;
  std::vector<TH2F*>Gain;
};
#endif
