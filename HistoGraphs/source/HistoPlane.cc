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

int TObjectUgly::getHistoGraphNbr(double& num)
{
  return int(num/100000000);
}

int TObjectUgly::getPlateNbr(double& num)
{
  return int((num-getHistoGraphNbr(num)*100000000)/1000000);
}

int TObjectUgly::getDifNbr(double& num)
{
  return int((num-getHistoGraphNbr(num)*100000000-getPlateNbr(num)*1000000)/100000);
}
    
int TObjectUgly::getAsicNbr(double& num)
{
  return int((num-getHistoGraphNbr(num)*100000000-getPlateNbr(num)*1000000-getDifNbr(num)*100000)/1000);
}

int TObjectUgly::getPadNbr(double& num)
{
  return int((num-getHistoGraphNbr(num)*100000000-getPlateNbr(num)*1000000-getDifNbr(num)*100000-getAsicNbr(num)*1000)/10);
}

int TObjectUgly::getThresholdNbr(double& num)
{
  return int((num-getHistoGraphNbr(num)*100000000-getPlateNbr(num)*1000000-getDifNbr(num)*100000-getAsicNbr(num)*1000-getPadNbr(num)*10));
}

void TObjectUgly::Fill(double x,double y,double z,double w,double v,double u)
{
  for(std::map<double,TObject*>::iterator it=HistGraphs.begin();it!=HistGraphs.end();++it)
  {
    double a=it->first;
    if(getHistoGraphNbr(a)>HistGraphselected)break;
    else if(getHistoGraphNbr(a)<HistGraphselected)continue;
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
              if((it->second)->IsA()->InheritsFrom("TH1")) (reinterpret_cast<TH1*>(it->second))->Fill(x,y);
              if((it->second)->IsA()->InheritsFrom("TH2")) (reinterpret_cast<TH2*>(it->second))->Fill(x,y,z);
              if((it->second)->IsA()->InheritsFrom("TH3")) (reinterpret_cast<TH3*>(it->second))->Fill(x,y,z,w);
              if((it->second)->IsA()->InheritsFrom("TGraph"))
              {
                PointsInGraphs[a]++;
                (reinterpret_cast<TGraph*>(it->second))->SetPoint(PointsInGraphs[a],x,y);
              }
              if((it->second)->IsA()->InheritsFrom("TGraph2D"))
              {
                PointsInGraphs[a]++;
                (reinterpret_cast<TGraph2D*>(it->second))->SetPoint(PointsInGraphs[a],x,y,z);
              }
              if((it->second)->IsA()->InheritsFrom("TGraphErrors")) 
              {
                PointsInGraphs[a]++;
                (reinterpret_cast<TGraphErrors*>(it->second))->SetPoint(PointsInGraphs[a],x,y);
                if(z!=0&&w!=0)(reinterpret_cast<TGraphErrors*>(it->second))->SetPointError(PointsInGraphs[a],z,w);
              }
              if((it->second)->IsA()->InheritsFrom("TGraph2DErrors"))
              {
                PointsInGraphs[a]++;
                (reinterpret_cast<TGraph2DErrors*>(it->second))->SetPoint(PointsInGraphs[a],x,y,z);
                if(w!=0&&v!=0&&u!=0)(reinterpret_cast<TGraph2DErrors*>(it->second))->SetPointError(PointsInGraphs[a],w,v,u);
              }
            } 
          } 
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
  static double num=0;
  if(ugly.NameToInt.find(name)==ugly.NameToInt.end())
  {
    ugly.NameToInt[name]=++num;
  }
  return Threshold+10*Pad+1000*Asic+100000*Dif+1000000*Plate+100000000*ugly.NameToInt[name];
}

void HistoPlane::CreateHistoGraph(std::string n,std::string type,int& number,int bx,double xd,double xu, int by,double yd,double yu,int bz,double zd,double zu)
{
  if(type=="TH1") ugly.HistGraphs[number]=new TH1D(n.c_str(),n.c_str(),bx,xd,xu);
  if(type=="TH2") ugly.HistGraphs[number]=new TH2D(n.c_str(),n.c_str(),bx,xd,xu,by,yd,yu);
  if(type=="TH3") ugly.HistGraphs[number]=new TH3D(n.c_str(),n.c_str(),bx,xd,xu,by,yd,yu,bz,zd,zu);
  if(type=="TProfile") ugly.HistGraphs[number]=new TProfile(n.c_str(),n.c_str(),bx,xd,xu);
  if(type=="TProfile2D") ugly.HistGraphs[number]=new TProfile2D(n.c_str(),n.c_str(),bx,xd,xu,by,yd,yu);
  if(type=="TProfile3D") ugly.HistGraphs[number]=new TProfile3D(n.c_str(),n.c_str(),bx,xd,xu,by,yd,yu,bz,zd,zu);
  if(type=="TGraph") 
  {
    TGraph* a=new TGraph(1000);
    a->SetName(n.c_str());
    a->SetTitle(n.c_str());
    ugly.HistGraphs[number]=a;
    ugly.PointsInGraphs[number]=0;
  }
  if(type=="TGraphErrors") 
  {
    TGraphErrors* a=new TGraphErrors(1000);
    a->SetName(n.c_str());
    a->SetTitle(n.c_str());
    ugly.HistGraphs[number]=a;
    ugly.PointsInGraphs[number]=0;
  }
  if(type=="TGraph2D") 
  {
    TGraph2D* a=new TGraph2D(1000);
    a->SetName(n.c_str());
    a->SetTitle(n.c_str());
    ugly.HistGraphs[number]=a;
    ugly.PointsInGraphs[number]=0;
  }
  if(type=="TGraph2DErrors") 
  {
    TGraph2DErrors* a=new TGraph2DErrors(1000);
    a->SetName(n.c_str());
    a->SetTitle(n.c_str());
    ugly.HistGraphs[number]=a;
    ugly.PointsInGraphs[number]=0;
  }
  ugly.IntToType[number]=type;
}


void HistoPlane::Add(const char* type ,const char* where,const char* vec,int bx,double xd,double xu, int by,double yd,double yu,int bz,double zd,double zu)
{
    if(Names.find(std::string(vec))!=Names.end())
    {
      std::cout<<red<<"Error:: "<<vec<<" exists and so will no be recreated"<<std::endl;
      return;
    }
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
        std::cout<<"Pads"<<std::endl;
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
                  std::string n="Plate"+std::to_string(j+1)+"/Dif"+std::to_string(Global::geom->GetDifsInPlane(j)[k])+"/Asic"+std::to_string(l);
                  n+="/Pad"+std::to_string(m-1)+"/"+Thresholds_name[o-1]+"/"+std::string(vec);
                  int number=ConvertToInt(std::string(vec),j+1,k,l,m,o);
                  CreateHistoGraph(n,type,number,bx,xd,xu,by,yd,yu,bz,zd,zu);
                }
              }
            }
          }
        }
      }
      if(ugly.GranularityName[where]>=Asic)
      {
        std::cout<<"Asics"<<std::endl;
        for(unsigned int j=0;j!=Global::geom->GetNumberPlates();++j)
        {
          for(unsigned int k=0;k!=Global::geom->GetDifsInPlane(j).size();++k)
          {
            int numberAsic=Global::geom->GetAsicNbr(Global::geom->GetDifsInPlane(j)[k])+1;
            for(unsigned int l=1;l!=numberAsic;++l)
            {
                for(unsigned int o=1;o!=7;++o)
                {
                  std::string n="Plate"+std::to_string(j+1)+"/Dif"+std::to_string(Global::geom->GetDifsInPlane(j)[k])+"/Asic"+std::to_string(l);
                  n+="/"+Thresholds_name[o-1]+"/"+std::string(vec);
                  int number=ConvertToInt(std::string(vec),j+1,k,l,0,o);
                  CreateHistoGraph(n,type,number,bx,xd,xu,by,yd,yu,bz,zd,zu);
                }
            }
          }
        }
      }
      if(ugly.GranularityName[where]>=Dif)
      {
        std::cout<<"Difs"<<std::endl;
        for(unsigned int j=0;j!=Global::geom->GetNumberPlates();++j)
        {
          for(unsigned int k=0;k!=Global::geom->GetDifsInPlane(j).size();++k)
          {
            for(unsigned int o=1;o!=7;++o)
            {
              std::string n="Plate"+std::to_string(j+1)+"/Dif"+std::to_string(Global::geom->GetDifsInPlane(j)[k])+"/"+Thresholds_name[o-1]+"/"
              +std::string(vec);
              int number=ConvertToInt(std::string(vec),j+1,k,0,0,o);
              CreateHistoGraph(n,type,number,bx,xd,xu,by,yd,yu,bz,zd,zu);
            }
          }
        }
      }
      if(ugly.GranularityName[where]>=Plane)
      {
        std::cout<<"Plane"<<std::endl;
        for(unsigned int j=0;j!=Global::geom->GetNumberPlates();++j)
        {
          for(unsigned int o=1;o!=7;++o)
          {
            std::string n="Plate"+std::to_string(j+1)+"/"+Thresholds_name[o-1]+"/"+std::string(vec);
            int number=ConvertToInt(std::string(vec),j+1,0,0,0,o);
            CreateHistoGraph(n,type,number,bx,xd,xu,by,yd,yu,bz,zd,zu);
          }
        }
      }
      if(ugly.GranularityName[where]>=Detector)
      {
        for(unsigned int o=1;o!=6;++o)
        {
          std::string n=Thresholds_name[o-1]+"/"+std::string(vec);
          int number=ConvertToInt(std::string(vec),0,0,0,0,o);
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
    Global::out->writeObject(folder,it->second);
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
