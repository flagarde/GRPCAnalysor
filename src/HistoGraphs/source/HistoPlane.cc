#include "HistoPlane.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TProfile3D.h"
#include "TGraphErrors.h"
#include "TGraph.h"
#include "TGraph2DErrors.h"
#include "TGraph2D.h"
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "Colors.h"
#include <iomanip>
#include "TClass.h"

void TObjectUgly::setSelected(int HistoGraphNbr,int Plate,int Dif,int Asic,int Pad,int Threshold)
{
  Plateselected=Plate;
  Difselected=Dif;
  Asicselected=Asic;
  Padselected=Pad;
  Thresholdselected=Threshold;
  HistGraphselected=HistoGraphNbr;
}

void HistoPlane::setRolling(std::string name , bool rolling)
{
  ugly.Rolling[name]=rolling;
}

int TObjectUgly::getHistoGraphNbr(double& num)
{
  return int(num/10000000000);
}
int TObjectUgly::getPlateNbr(double& num)
{
  return int((num-getHistoGraphNbr(num)*10000000000)/100000000);
}

int TObjectUgly::getDifNbr(double& num)
{
  return int((num-getHistoGraphNbr(num)*10000000000-getPlateNbr(num)*100000000)/1000000);
}
    
int TObjectUgly::getAsicNbr(double& num)
{
  return int((num-getHistoGraphNbr(num)*10000000000-getPlateNbr(num)*100000000-getDifNbr(num)*1000000)/10000);
}

int TObjectUgly::getPadNbr(double& num)
{
  return int((num-getHistoGraphNbr(num)*10000000000-getPlateNbr(num)*100000000-getDifNbr(num)*1000000-getAsicNbr(num)*10000)/100);
}

int TObjectUgly::getThresholdNbr(double& num)
{
  return int((num-getHistoGraphNbr(num)*10000000000-getPlateNbr(num)*100000000-getDifNbr(num)*1000000-getAsicNbr(num)*10000-getPadNbr(num)*100));
}

void TObjectUgly::Fill2(TObject* obj,double x,double y,double z,double w,double v,double u,double a)
{
  if(obj->IsA()->InheritsFrom("TH1D")) (reinterpret_cast<TH1D*>(obj))->Fill(x,y);
  if(obj->IsA()->InheritsFrom("TH2D")) (reinterpret_cast<TH2D*>(obj))->Fill(x,y,z);
  if(obj->IsA()->InheritsFrom("TH3D")) (reinterpret_cast<TH3D*>(obj))->Fill(x,y,z,w);
  if(obj->IsA()->InheritsFrom("TGraph"))
  {
    PointsInGraphs[a]++;
    (reinterpret_cast<TGraph*>(obj))->SetPoint(PointsInGraphs[a],x,y);
  }
  if((obj)->IsA()->InheritsFrom("TGraph2D"))
  {
    PointsInGraphs[a]++;
    (reinterpret_cast<TGraph2D*>(obj))->SetPoint(PointsInGraphs[a],x,y,z);
  }
  if((obj)->IsA()->InheritsFrom("TGraphErrors")) 
  {
    PointsInGraphs[a]++;
    (reinterpret_cast<TGraphErrors*>(obj))->SetPoint(PointsInGraphs[a],x,y);
    if(z!=0&&w!=0)(reinterpret_cast<TGraphErrors*>(obj))->SetPointError(PointsInGraphs[a],z,w);
  }
  if((obj)->IsA()->InheritsFrom("TGraph2DErrors"))
  {
    PointsInGraphs[a]++;
    (reinterpret_cast<TGraph2DErrors*>(obj))->SetPoint(PointsInGraphs[a],x,y,z);
    if(w!=0&&v!=0&&u!=0)(reinterpret_cast<TGraph2DErrors*>(obj))->SetPointError(PointsInGraphs[a],w,v,u);
  }
}


void TObjectUgly::Fill(double x,double y,double z,double w,double v,double u)
{
  for(std::map<double,TObject*>::iterator it=HistGraphs.begin();it!=HistGraphs.end();++it)
  {
    double a=it->first;
    if(getHistoGraphNbr(a)>HistGraphselected)break;
    else if(getHistoGraphNbr(a)<HistGraphselected)continue;
    if(Rolling[IntToName[getHistoGraphNbr(a)]]==true)
   {
    if(getPlateNbr(a)==Plateselected||getPlateNbr(a)==0)
    {
      if(getDifNbr(a)==Difselected||getDifNbr(a)==0)
      {
        if(getAsicNbr(a)==Asicselected||getAsicNbr(a)==0)
        {
          if(getPadNbr(a)==Padselected||getPadNbr(a)==0)
          {
            if(getThresholdNbr(a)==Thresholdselected||getThresholdNbr(a)==0)
            {
              Fill2(it->second,x,y,z,w,v,u,a);
            } 
          } 
        } 
      } 
    } 
   }
   else
   {
    if(getThresholdNbr(a)==Thresholdselected)
    {
      std::string b=IntToName[getHistoGraphNbr(a)];
      if(NameToWhere[b]==0)Fill2(it->second,x,y,z,w,v,u);
      else if(NameToWhere[b]==1&&getPlateNbr(a)==Plateselected)
      {
        std::cout<<"Here"<<std::endl;
        Fill2(it->second,x,y,z,w,v,u);
      }
      else if(NameToWhere[b]==2&&getPlateNbr(a)==Plateselected&&getDifNbr(a)==Difselected)
      {
        Fill2(it->second,x,y,z,w,v,u);
      }
      else if(NameToWhere[b]==3&&getPlateNbr(a)==Plateselected&&getDifNbr(a)==Difselected&&getAsicNbr(a)==Asicselected)
      {
        //std::cout<<std::setprecision(12)<<it->first<<std::endl;
        Fill2(it->second,x,y,z,w,v,u);
        //std::cout<<blue<<NameToWhere[b]<<getPlateNbr(a)<<" "<<Plateselected<<"  "<<getDifNbr(a)<<"  "<<Difselected<<normal<<std::endl;
      }
      else if(NameToWhere[b]==4&&getPlateNbr(a)==Plateselected&&getDifNbr(a)==Difselected&&getAsicNbr(a)==Asicselected&&getPadNbr(a)==Padselected)
      {
        Fill2(it->second,x,y,z,w,v,u);
      }
    }
   }
  }
}

std::vector<std::string> HistoPlane::SplitFilename (const std::string& str)
{
  std::size_t found = str.find_last_of("/\\");
  std::vector<std::string> a;
  a.push_back(str.substr(0,found)) ;
  a.push_back(str.substr(found+1));
  return a;
}

TObjectUgly& HistoPlane::operator()(const char* name,int Plate,int Dif,int Asic,int Pad,int Threshold)
{
  ugly.setSelected(ugly.NameToInt[name],Plate,Dif,Asic,Pad,Threshold);
  return ugly;
}


double HistoPlane::ConvertToInt(std::string name,int Plate,int Dif,int Asic,int Pad,int Threshold)
{
  if(ugly.NameToInt.find(name)==ugly.NameToInt.end())
  {
    ugly.NameToInt[name]=num;
    ugly.IntToName[num]=name;
    num++;
  }
  return Threshold+100*Pad+10000*Asic+1000000*Dif+100000000*Plate+10000000000*ugly.NameToInt[name];
}

void HistoPlane::CreateHistoGraph(std::string n,std::string type,double& number,int bx,double xd,double xu, int by,double yd,double yu,int bz,double zd,double zu)
{
  if(type=="TH1") ugly.HistGraphs[number]=new TH1D(n.c_str(),n.c_str(),bx,xd,xu);
  else if(type=="TH2") ugly.HistGraphs[number]=new TH2D(n.c_str(),n.c_str(),bx,xd,xu,by,yd,yu);
  else if(type=="TH3") ugly.HistGraphs[number]=new TH3D(n.c_str(),n.c_str(),bx,xd,xu,by,yd,yu,bz,zd,zu);
  else if(type=="TProfile") ugly.HistGraphs[number]=new TProfile(n.c_str(),n.c_str(),bx,xd,xu);
  else if(type=="TProfile2D") ugly.HistGraphs[number]=new TProfile2D(n.c_str(),n.c_str(),bx,xd,xu,by,yd,yu);
  else if(type=="TProfile3D") ugly.HistGraphs[number]=new TProfile3D(n.c_str(),n.c_str(),bx,xd,xu,by,yd,yu,bz,zd,zu);
  else if(type=="TGraph") 
  {
    ugly.HistGraphs[number]=new TGraph(0);
    ugly.PointsInGraphs[number]=-1;
  }
  else if(type=="TGraphErrors") 
  {
    ugly.HistGraphs[number]=new TGraphErrors(0);
    ugly.PointsInGraphs[number]=-1;
  }
  else if(type=="TGraph2D") 
  {
    ugly.HistGraphs[number]=new TGraph2D(0);
    ugly.PointsInGraphs[number]=-1;
  }
  else if(type=="TGraph2DErrors") 
  {
    ugly.HistGraphs[number]=new TGraph2DErrors(0);
    ugly.PointsInGraphs[number]=-1;
  }
  if(type=="TGraph2DErrors"||type=="TGraph2D"||type=="TGraphErrors"||type=="TGraph")
  {
    (reinterpret_cast<TNamed*>(ugly.HistGraphs[number]))->SetName(n.c_str());
    (reinterpret_cast<TNamed*>(ugly.HistGraphs[number]))->SetTitle(n.c_str());
  }
  ugly.IntToType[number]=type;
}


void HistoPlane::Add(const char* type ,const char* where,const char* vec,int bx,double xd,double xu, int by,double yd,double yu,int bz,double zd,double zu)
{
    std::cout<<"ggggg"<<std::endl;
    if(Names.find(std::string(vec))!=Names.end())
    {
      std::cout<<red<<"Error:: "<<vec<<" exists and so will no be recreated"<<std::endl;
      return;
    }
    ugly.Rolling[std::string(vec)]=true;
    Names.insert(std::string(vec));
    if(ugly.GranularityName.find(where)==ugly.GranularityName.end())
    {
      std::cout<<red<<"Error:: Granularity is not known"<<normal<<std::endl;
    }
    else
    {
      ugly.NameToWhere[std::string(vec)]=ugly.GranularityName[where];
      if(ugly.GranularityName[where]==Pad)
      {
        for(unsigned int j=0;j!=Global::geom->GetNumberPlates();++j)
        {
          for(unsigned int k=0;k!=Global::geom->GetDifsInPlane(j).size();++k)
          {
            int numberAsic=Global::geom->GetAsicNbr(Global::geom->GetDifsInPlane(j)[k])+1;
            for(unsigned int l=1;l!=numberAsic;++l)
            {
              for(unsigned int m=1;m!=65;++m)
              {
                for(unsigned int o=1;o!=7;++o)
                {
                  int difnbr=Global::geom->GetDifsInPlane(j)[k];
                  std::string n="Plate"+std::to_string(j+1)+"/Dif"+std::to_string(difnbr)+"/Asic"+std::to_string(l);
                  n+="/Pad"+std::to_string(m-1)+"/"+Thresholds_name[o-1]+"/"+std::string(vec);
                  double number=ConvertToInt(std::string(vec),j+1,difnbr,l,m,o);
                  CreateHistoGraph(n,type,number,bx,xd,xu,by,yd,yu,bz,zd,zu);
                }
              }
            }
          }
        }
      }
      if(ugly.GranularityName[where]>=Asic)
      {
        for(unsigned int j=0;j!=Global::geom->GetNumberPlates();++j)
        {
          for(unsigned int k=0;k!=Global::geom->GetDifsInPlane(j).size();++k)
          {
            int numberAsic=Global::geom->GetAsicNbr(Global::geom->GetDifsInPlane(j)[k])+1;
            for(unsigned int l=1;l!=numberAsic;++l)
            {
                for(unsigned int o=1;o!=7;++o)
                {
                  int difnbr=Global::geom->GetDifsInPlane(j)[k];
                  std::string n="Plate"+std::to_string(j+1)+"/Dif"+std::to_string(difnbr)+"/Asic"+std::to_string(l);
                  n+="/"+Thresholds_name[o-1]+"/"+std::string(vec);
                  double number=ConvertToInt(std::string(vec),j+1,difnbr,l,0,o);
                  CreateHistoGraph(n,type,number,bx,xd,xu,by,yd,yu,bz,zd,zu);
                }
            }
          }
        }
      }
      if(ugly.GranularityName[where]>=Dif)
      {
        for(unsigned int j=0;j!=Global::geom->GetNumberPlates();++j)
        {
          for(unsigned int k=0;k!=Global::geom->GetDifsInPlane(j).size();++k)
          {
            for(unsigned int o=1;o!=7;++o)
            {
              int difnbr=Global::geom->GetDifsInPlane(j)[k];
              std::cout<<red<<difnbr<<normal<<std::endl;
              std::string n="Plate"+std::to_string(j+1)+"/Dif"+std::to_string(difnbr)+"/"+Thresholds_name[o-1]+"/"
              +std::string(vec);
              double number=ConvertToInt(std::string(vec),j+1,difnbr,0,0,o);
              std::cout<<std::setprecision(12)<<"           "<<number<<std::endl;
              CreateHistoGraph(n,type,number,bx,xd,xu,by,yd,yu,bz,zd,zu);
            }
          }
        }
      }
      if(ugly.GranularityName[where]>=Plane)
      {
        for(unsigned int j=0;j!=Global::geom->GetNumberPlates();++j)
        {
          for(unsigned int o=1;o!=7;++o)
          {
            std::string n="Plate"+std::to_string(j+1)+"/"+Thresholds_name[o-1]+"/"+std::string(vec);
            double number=ConvertToInt(std::string(vec),j+1,0,0,0,o);
            CreateHistoGraph(n,type,number,bx,xd,xu,by,yd,yu,bz,zd,zu);
          }
        }
      }
      if(ugly.GranularityName[where]>=Detector)
      {
        for(unsigned int o=1;o!=6;++o)
        {
          std::string n=Thresholds_name[o-1]+"/"+std::string(vec);
          double number=ConvertToInt(std::string(vec),0,0,0,0,o);
          CreateHistoGraph(n,type,number,bx,xd,xu,by,yd,yu,bz,zd,zu);
        }
      }
    }
}


void HistoPlane::Add(const char* type ,const char* where,std::vector<std::string> vec,int bx,double xd,double xu, int by,double yd,double yu,int bz,double zd,double zu)
{
  const char a=*type;
  const char b=*where;
  for(unsigned int i=0;i!=vec.size();++i)
  {
    Add(&a,&b,(const char*)&vec[i],int(bx),double(xd),double(xu),int(by),double(yd),double(yu),int(bz),double(zd),double(zu));
  } 
}
   
void HistoPlane::List()
{
  for(std::map<double,TObject*>::iterator it=ugly.HistGraphs.begin();it!=ugly.HistGraphs.end();++it)
  {
    (reinterpret_cast<TNamed*>(it->second))->GetTitle();
  }
}

void HistoPlane::Write()
{
  for(std::map<double,TObject*>::iterator it=ugly.HistGraphs.begin();it!=ugly.HistGraphs.end();++it)
  {
    std::string folder="HistoGraphs/"+SplitFilename((reinterpret_cast<TNamed*>(it->second))->GetTitle())[0];
    if(NotWriteEmpty==true)
    {
      if((it->second)->IsA()->InheritsFrom("TH1")&&(reinterpret_cast<TH1*>(it->second))->GetEntries()!=0) Global::out->writeObject(folder,it->second);
     else if((it->second)->IsA()->InheritsFrom("TGraph")&&(reinterpret_cast<TGraph*>(it->second))->GetN()!=0)Global::out->writeObject(folder,it->second);
     else if((it->second)->IsA()->InheritsFrom("TGraph2D")&&(reinterpret_cast<TGraph2D*>(it->second))->GetN()!=0)Global::out->writeObject(folder,it->second);
    }
    else  Global::out->writeObject(folder,it->second);
  }
}

HistoPlane::~HistoPlane()
{
  for(std::map<double,TObject*>::iterator it=ugly.HistGraphs.begin();it!=ugly.HistGraphs.end();++it)
  {
    delete it->second;
  }
  ugly.NameToInt.clear();
  ugly.HistGraphs.clear();
}
