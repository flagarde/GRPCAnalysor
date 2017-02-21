#include <iostream>
#include <vector>
#include "XMLReader.h"
#include "Colors.h"
#include "Types.h"

void XMLReader::Write(TiXmlElement* elem,const char* who,unsigned int &wherePlate,unsigned int &whereDif,double& var,std::string &FileName)
{
	if(elem->Attribute(who)!=nullptr) var=atof(elem->Attribute(who));
  else
  {
		if(whereDif>0)std::cout<<red<<who<<" argument mising in Plate : "<<wherePlate<<" Dif number : "<<whereDif<<" !!!"<<normal<<std::endl;  
    else std::cout<<red<<who<<" argument mising in Plate : "<<wherePlate<<" !!!"<<normal<<std::endl;  
    std::cout<<red<<"Please check the file : "<<FileName<<normal<<std::endl;
		std::exit(1);	
	}
}

void XMLReader::Read(std::string& FileName, Geometry& geom)
{
  TiXmlDocument doc(FileName.c_str());
  if(!doc.LoadFile())
  {
    std::cout<<"Error loading file"<< std::endl;
    std::cout<<"Error #" << doc.ErrorId() << " : " << doc.ErrorDesc()<<std::endl;
  }
  else
  {
    std::vector<int>DifM;
    std::cout<<"File : "<<FileName<<std::endl;
    TiXmlHandle hdl(&doc);
    TiXmlElement* Platee = hdl.FirstChildElement().FirstChildElement().Element();
    TiXmlElement* Diff=nullptr;
    unsigned int PlateNumber=-1;
    unsigned int DifNumber=0;
    double x,y,z,xy,xz,yz,SizeX,SizeY,I,J,DifId;
    double ElectrodeType=-1;
    double Gaz_number=-1;
    std::string HV_channel="";
    std::string Gaz_channel="";
    while (Platee)
    {
      DifNumber=0;
      PlateNumber++;
      Write(Platee,"x",PlateNumber,DifNumber,x,FileName);
      Write(Platee,"y",PlateNumber,DifNumber,y,FileName);
      Write(Platee,"z",PlateNumber,DifNumber,z,FileName);
      Write(Platee,"alpha",PlateNumber,DifNumber,xy,FileName);
      Write(Platee,"beta",PlateNumber,DifNumber,xz,FileName);
      Write(Platee,"gamma",PlateNumber,DifNumber,yz,FileName);
      Write(Platee,"SizeX",PlateNumber,DifNumber,SizeX,FileName);
      Write(Platee,"SizeY",PlateNumber,DifNumber,SizeY,FileName);
      if(Platee->Attribute("ElectrodeType")!=nullptr)
	    {
	        for(unsigned int i=0;i!=Types_Electrode.size();++i)
	        {
	          if(strcmp(Platee->Attribute("ElectrodeType"),Types_Electrode[i].c_str()) == 0)
	          {
	            ElectrodeType=i;
	            break;
	          }
	          else ElectrodeType=-1;
	        }
	        if(ElectrodeType==-1) std::cout<<"Error defining ElectrodeType"<<std::endl;
	    }
	    if(Platee->Attribute("Gaz_number")!=NULL)
	    {
	      std::string e=Platee->Attribute("Gaz_number");
	      if(e.size()!=0) Gaz_number=atof(Platee->Attribute("Gaz_number"));
	    }
	    else Gaz_number=-1;
	    if(Platee->Attribute("HV_channel")!=NULL) HV_channel=Platee->Attribute("HV_channel");
	    else HV_channel="";
	    if(Platee->Attribute("Gaz_channel")!=NULL) Gaz_channel=Platee->Attribute("Gaz_channel");
	    else Gaz_channel="";
      Diff=Platee->FirstChildElement();
      while (Diff)
      {
        DifNumber++;
        int DifType=m3;
	      if(Diff->Attribute("DifType")!=nullptr)
	      {
	        for(unsigned int i=0;i!=Types_Dif.size();++i)
	        {
	          if(strcmp(Diff->Attribute("DifType"),Types_Dif[i].c_str()) == 0)
	          {
	            DifType=i;
	            break;
	          }
	          else DifType=-1;
	        }
	        if(DifType==-1) std::cout<<"Error defining the type of Dif"<<std::endl;
	      }
        Write(Diff,"DifId",PlateNumber,DifNumber,DifId,FileName);
        Write(Diff,"I",PlateNumber,DifNumber,I,FileName);
        Write(Diff,"J",PlateNumber,DifNumber,J,FileName);
	      DifM.push_back(atof(Diff->Attribute("DifId")));
	      geom.AddDif(I,J,DifId,xy,xz,yz,PlateNumber,DifType);
	      Diff= Diff->NextSiblingElement();
      }
      geom.AddPlate(x,y,z,xy,xz,yz,DifM,SizeX,SizeY,ElectrodeType,Gaz_number,HV_channel,Gaz_channel);
      DifM.clear();
      Platee=Platee->NextSiblingElement(); 
    }
  }
}
