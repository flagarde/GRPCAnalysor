#include<iostream>
#include<vector>
#include "XMLReaderElog.h"
#include "XMLReaderConfig.h"
#include "Colors.h"
#include "Global.h"
#include "Types.h"
#include <string>
#include <iostream>
#include <sstream>
#include <utility>
#ifdef OCCI_SUPPORT
#include "db/DBInit.h"
#include "configObjects/Setup.h"
#include "daq/RunInfo.h"
using namespace oracle::occi;
#endif
void XMLReaderElog::Read(std::string &FileName,ConfigInfos* Conf,int RunNumber)
{
  std::string DAQ_Name="";
  TiXmlDocument doc(FileName.c_str());
  if(!doc.LoadFile())
  {
    std::cout<<red<<"Error loading file "<<FileName<<normal<< std::endl;
    std::cout<<red<<"Error #" << doc.ErrorId() << " : " << doc.ErrorDesc()<<normal<<std::endl;
  }
  else
  {
    std::cout<<"File : "<<FileName<<std::endl;
    TiXmlHandle hdl(&doc);
    TiXmlElement*  element = hdl.FirstChildElement().FirstChildElement().Element();
    bool founded=false;
    while (element&&founded==false)
    {
      std::string name="";
      if(element->FirstChildElement("Run")->GetText()!=nullptr)name = element->FirstChildElement("Run")->GetText();
      if(name==std::to_string(RunNumber))
      {
        founded=true;
        DAQ_Name=element->FirstChildElement("DAQ_Name")->GetText();
        TiXmlElement*  ele = element->FirstChildElement();
        while (ele!=nullptr)
		    {
		      if(ele->GetText()!=nullptr)Global::eloginf->AddBruteValue(ele->Value(),ele->GetText());
		      ele=ele->NextSiblingElement();
		    }
		    Global::eloginf->ConstructMaps();
		    std::cout<<green<<"Run Number "<<RunNumber<<" found in "<<FileName<<" I extract the data :) "<<normal<<std::endl;
      }
      element=element->NextSiblingElement();
    }
  }
  std::string path="./SlowControl/"+DAQ_Name+".xml";
  //First Try to find the SlowControl File in the folder SlowControl 
  TiXmlDocument docc(path.c_str());
  if(!docc.LoadFile()&&DAQ_Name!="")
  {
    #ifdef OCCI_SUPPORT
    std::cout<<green<<"SlowControl file not found int SlowControl folder"<<normal<<std::endl;
    std::cout<<green<<"Now I will connect to the Database to download SlowControl :) with DAQ_Name "<<DAQ_Name<<normal<<std::endl;
    DBInit::init();
    State* s = State::download(DAQ_Name); // download the state with name 'MyState'
    s->saveToXML(path.c_str()); // creates the XML file
    DBInit::terminate();
    XMLReaderConfig slowcontrolreader;
    slowcontrolreader.Read(path,Conf);
    #else
    std::cout<<red<<"SlowControl file not found int SlowControl folder and I can't connect me to the Database :("<<normal<<std::endl;
    #endif
  }
  else if(DAQ_Name!="")
  {
	std::cout<<green<<"SlowControl file found in SlowControl folder"<<normal<<std::endl;
	XMLReaderConfig slowcontrolreader;
    	slowcontrolreader.Read(path,Conf);
  }
  else
  {
   std::cout<<red<<"I can't do nothing to connect to database "<<normal<<std::endl;
  }
}
