#include "Histogrammer.h"
#include "Mapping.h"
#include <map>
#include <array>

Histogrammer::Histogrammer(ConfigInfos* _conf,OutFileRoot* _out,Geometry* _geom):conf(_conf),out(_out),geom(_geom)
{
  for(unsigned int i=0;i!=geom->GetNumberPlates();++i)
  {
    int Xmin=0;
    int Ymin=0;
    int Xmax=geom->GetSizeX(i)+1;
    int Ymax=geom->GetSizeY(i)+1;
    if(geom->GetDifType(geom->GetDifsInPlane(i)[0])==positional) 
    {
			Xmin=0;
			Ymin=1;
			Xmax=128;
			Ymax=3;
		}
    else if(geom->GetDifType(geom->GetDifsInPlane(i)[0])==pad)
    {
			Xmin=1;
      Ymin=0;
      Ymax=8*4*geom->GetDifNbrPlate(i);
      Xmax=48*8;
    }
    std::string h="Plate Nbr "+ std::to_string(i +1 );
    std::string hh="Plate Nbr "+ std::to_string(i +1 )+"Gain";
    for(unsigned int j=0;j<ThresholdMap.size();++j)
    {
      ThresholdMap[j].push_back(new TH2F((h+"_"+Thresholds_name[j]).c_str(),(h+"_"+Thresholds_name[j]).c_str(),Xmax-Xmin,Xmin,Xmax,Ymax-Ymin,Ymin,Ymax));
    }
  	Gain.push_back(new TH2F(hh.c_str(),hh.c_str(),Xmax-Xmin,Xmin,Xmax,Ymax-Ymin,Ymin,Ymax));
    if(geom->GetDifType(geom->GetDifsInPlane(i)[0])==positional)
    {
			Gain[i]->GetXaxis()->SetTitle("Strip");
      Gain[i]->GetYaxis()->SetTitle("Gap");
      Gain[i]->GetYaxis()->SetBinLabel(1,"Bottom");
      Gain[i]->GetYaxis()->SetBinLabel(2,"Top");
      Gain[i]->GetYaxis()->SetNdivisions(001013);
      Gain[i]->GetXaxis()->SetNdivisions(001013);
      for(unsigned int j=0;j<ThresholdMap.size();++j)
      {
      	ThresholdMap[j][i]->GetXaxis()->SetTitle("Strip");
      	ThresholdMap[j][i]->GetYaxis()->SetTitle("Gap");
        ThresholdMap[j][i]->GetYaxis()->SetBinLabel(1,"Bottom");
        ThresholdMap[j][i]->GetYaxis()->SetBinLabel(2,"Top");
        ThresholdMap[j][i]->GetYaxis()->SetNdivisions(001013);
        ThresholdMap[j][i]->GetXaxis()->SetNdivisions(001013);
			}
		}
    else if(geom->GetDifType(geom->GetDifsInPlane(i)[0])==pad)
    {
			Gain[i]->GetXaxis()->SetTitle("I");
      Gain[i]->GetYaxis()->SetTitle("J");
      Gain[i]->GetYaxis()->SetNdivisions(001032);
      Gain[i]->GetXaxis()->SetNdivisions(001032);
      for(unsigned int j=0;j<ThresholdMap.size();++j)
      {
      	ThresholdMap[j][i]->GetXaxis()->SetTitle("I");
      	ThresholdMap[j][i]->GetYaxis()->SetTitle("J");
        ThresholdMap[j][i]->GetYaxis()->SetNdivisions(000032);
        ThresholdMap[j][i]->GetXaxis()->SetNdivisions(000032);
			}
		}
  }
  Plot();
}

void Histogrammer::Plot()
{
  for(std::map<unsigned int,DifInfo>::iterator it=(conf->ReturnMe()).begin();it!=(conf->ReturnMe()).end();++it)
  {
    int DIF_Id=it->first;
    for(std::map<unsigned int,AsicInfo>::iterator itt=((it->second).ReturnMe()).begin();itt!=((it->second).ReturnMe()).end();++itt)
    {
      unsigned int Asic_Id=itt->first;
      std::vector<unsigned int> thee= (itt->second).getThresholds();
      for(unsigned int i=0;i!=(itt->second).ReturnMe().size();++i)
      {
				if(geom->GetDifType(it->first)!=temporal&&geom->GetDifType(it->first)!=scintillator&&geom->GetDifType(it->first)!=tcherenkov)
				{
          if(geom->GetDifNbrPlate(it->first)-1>=0)
          {
            int I=0;
    				int J=0;
    				if(geom->GetDifType(DIF_Id)==pad) 
    				{
      				I =(1+MapILargeHR2[i]+AsicShiftI[Asic_Id])+geom->GetDifPositionX(DIF_Id);
      				J =(32-(MapJLargeHR2[i]+AsicShiftJ[Asic_Id]))+geom->GetDifPositionY(DIF_Id);
    				}
    				if(geom->GetDifType(DIF_Id)==positional) 
    				{
      				if(geom->GetDifUpDown(DIF_Id)==1) I =(2*i)+geom->GetDifPositionX(DIF_Id);
      				else I =2*(64-i)-1+geom->GetDifPositionX(DIF_Id);
      				J =Asic_Id;
    				}
            Gain[geom->GetDifNbrPlate(it->first)-1]->Fill(I,J,(itt->second).ReturnMe()[i]);
            for(unsigned int h=0;h!=thee.size();++h)
            {
              ThresholdMap[h][geom->GetDifNbrPlate(it->first)-1]->Fill(I,J,thee[h]);
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
