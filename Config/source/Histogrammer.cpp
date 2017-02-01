#include "Histogrammer.h"
#include "Mapping.h"
#include <map>
#include <array>
void Histogrammer::Plot()
{
  for(std::map<unsigned int,DifInfo>::iterator it=(conf->ReturnMe()).begin();it!=(conf->ReturnMe()).end();++it)
  {
    int dif_id=it->first;
    std::cout<<green<<dif_id<<normal<<std::endl;
    for(std::map<unsigned int,AsicInfo>::iterator itt=(it->second).ReturnMe().begin();itt!=(it->second).ReturnMe().end();++itt)
    {
      int asic_id=itt->first;
      std::cout<<blue<<asic_id<<normal<<std::endl;
      std::vector<unsigned int> thee= (itt->second).getThresholds();
      for(unsigned int i=0;i!=(itt->second).ReturnMe().size();++i)
      {
				if(geom->GetDifType(it->first)!=temporal&&geom->GetDifType(it->first)!=scintillator&&geom->GetDifType(it->first)!=tcherenkov)
				{
          if(geom->GetDifNbrPlate(it->first)-1>=0)
          {
            Gain[geom->GetDifNbrPlate(it->first)-1]->Fill((1+MapILargeHR2[i]+AsicShiftI[asic_id])+geom->GetDifPositionX(dif_id),(32-(MapJLargeHR2[i]+AsicShiftJ[asic_id]))+geom->GetDifPositionY(dif_id),(itt->second).ReturnMe()[i]);
            for(unsigned int h=0;h!=thee.size();++h)
            {
              ThresholdMap[h][geom->GetDifNbrPlate(it->first)-1]->Fill((1+MapILargeHR2[i]+AsicShiftI[asic_id])+geom->GetDifPositionX(dif_id),(32-(MapJLargeHR2[i]+AsicShiftJ[asic_id]))+geom->GetDifPositionY(dif_id),thee[h]);
              std::cout<<red<<thee[h]<<normal<<std::endl;
            }
          }
        }
      }
    }
  }
  for(unsigned int o=0;o!=Gain.size();++o)
  {
    std::string SlowControl="SlowControl/Plane_Nbr"+std::to_string(o+1);
    out->writeObject(SlowControl,Gain[o]);
    for(unsigned int j=0;j<ThresholdMap.size();++j)
    {
      out->writeObject(SlowControl,ThresholdMap[j][o]);
    }
  }
}
