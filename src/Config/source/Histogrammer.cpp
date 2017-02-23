#include "Histogrammer.h"
#include "Mapping.h"
#include <map>
#include <array>
#include "ConstructConverters.h"
#include "Threshold.h"
Histogrammer::Histogrammer(ConfigInfos* _conf,OutFileRoot* _out,Geometry* _geom):conf(_conf),out(_out),geom(_geom)
{
  for(unsigned int i=0;i!=geom->GetNumberPlates();++i)
  {
    int Xmin=0;
    int Xmax=0;
		int Ymin=0;
    int Ymax=0;
    if(geom->GetDifType(geom->GetDifsInPlane(i)[0])==strip||geom->GetDifType(geom->GetDifsInPlane(i)[0])==stripup||geom->GetDifType(geom->GetDifsInPlane(i)[0])==stripdown) 
    {
      Ymin=1;
			Xmax=128;
			Ymax=3;
		}
    else if(geom->GetDifType(geom->GetDifsInPlane(i)[0])==pad)
    {
      Ymin=1;
      Ymax=8*4*geom->GetNbrDifInPlate(i);
      Xmax=48*8;
    }
    std::string h="Plate Nbr "+ std::to_string(i +1 );
    std::string hh="Plate Nbr "+ std::to_string(i +1 )+"Gain";
    for(unsigned int j=0;j<ThresholdMap.size();++j)
    {
      ThresholdMap[j].push_back(new TH2F((h+"_"+Thresholds_name[j]).c_str(),(h+"_"+Thresholds_name[j]).c_str(),Xmax-Xmin,Xmin,Xmax,Ymax-Ymin,Ymin,Ymax));
      ThresholdMapInt[j].push_back(new TH2F((h+"_"+Thresholds_name[j]+"_int").c_str(),(h+"_"+Thresholds_name[j]+"_int").c_str(),Xmax-Xmin,Xmin,Xmax,Ymax-Ymin,Ymin,Ymax));
    }
  	Gain.push_back(new TH2F(hh.c_str(),hh.c_str(),Xmax-Xmin,Xmin,Xmax,Ymax-Ymin,Ymin,Ymax));
    if(geom->GetDifType(geom->GetDifsInPlane(i)[0])==strip||geom->GetDifType(geom->GetDifsInPlane(i)[0])==stripup||geom->GetDifType(geom->GetDifsInPlane(i)[0])==stripdown) 
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
        ThresholdMapInt[j][i]->GetXaxis()->SetTitle("Strip");
      	ThresholdMapInt[j][i]->GetYaxis()->SetTitle("Gap");
        ThresholdMapInt[j][i]->GetYaxis()->SetBinLabel(1,"Bottom");
        ThresholdMapInt[j][i]->GetYaxis()->SetBinLabel(2,"Top");
        ThresholdMapInt[j][i]->GetYaxis()->SetNdivisions(001013);
        ThresholdMapInt[j][i]->GetXaxis()->SetNdivisions(001013);
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
        ThresholdMapInt[j][i]->GetXaxis()->SetTitle("I");
      	ThresholdMapInt[j][i]->GetYaxis()->SetTitle("J");
        ThresholdMapInt[j][i]->GetYaxis()->SetNdivisions(000032);
        ThresholdMapInt[j][i]->GetXaxis()->SetNdivisions(000032);
			}
		}
  }
  Plot();
}

void Histogrammer::Plot()
{
  ConstructConverters* converter=new ConstructConverters(geom);
  for(std::map<unsigned int,DifInfo>::iterator it=(conf->ReturnMe()).begin();it!=(conf->ReturnMe()).end();++it)
  {
    int DIF_Id=it->first;
    converter->setType(DIF_Id);
    for(std::map<unsigned int,AsicInfo>::iterator itt=((it->second).ReturnMe()).begin();itt!=((it->second).ReturnMe()).end();++itt)
    {
      unsigned int Asic_Id=itt->first;
      std::vector<unsigned int> thee= (itt->second).getThresholds();
      for(unsigned int i=0;i!=(itt->second).ReturnMe().size();++i)
      {
				if(geom->GetDifType(it->first)!=temporal&&geom->GetDifType(it->first)!=scintillator&&geom->GetDifType(it->first)!=tcherenkov)
				{
          if(geom->GetDifNbrPlate(it->first)<int(geom->GetNumberPlates()))
          {
            int I=converter->RawToIJK(DIF_Id,Asic_Id,i)[0];
    				int J=converter->RawToIJK(DIF_Id,Asic_Id,i)[1];
           Gain[geom->GetDifNbrPlate(it->first)]->Fill(I,J,double((itt->second).ReturnMe()[i]));
            for(unsigned int h=0;h!=thee.size();++h)
            {
            	ThresholdMap[h][geom->GetDifNbrPlate(it->first)]->Fill(I,J,double(ThresholdsConv(thee[h],h)));
              ThresholdMapInt[h][geom->GetDifNbrPlate(it->first)]->Fill(I,J,double(thee[h]));
            }
          }
        }
      }
    }
  }
  for(unsigned int o=0;o!=Gain.size();++o)
  {
    std::string SlowControl="SlowControl/Plane_Nbr"+std::to_string(o+1);
    if(Gain[o]->GetEntries()!=0)out->writeObject(SlowControl,Gain[o]);
    for(unsigned int j=0;j<ThresholdMap.size();++j)
    {
      if(ThresholdMap[j][o]->GetEntries()!=0)out->writeObject(SlowControl,ThresholdMap[j][o]);
    }
     for(unsigned int j=0;j<ThresholdMapInt.size();++j)
    {
      if(ThresholdMapInt[j][o]->GetEntries()!=0)out->writeObject(SlowControl,ThresholdMapInt[j][o]);
    }
  }
  delete converter;
}

Histogrammer::~Histogrammer()
{
  for(unsigned int o=0;o!=Gain.size();++o)
  {
    delete Gain[o];
    for(unsigned int j=0;j<ThresholdMap.size();++j)
    {
      delete ThresholdMap[j][o];
      delete ThresholdMapInt[j][o];
    }
  }
}
