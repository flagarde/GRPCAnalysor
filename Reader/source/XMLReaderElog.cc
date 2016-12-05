#include<iostream>
#include<vector>
#include "XMLReaderElog.h"
#include "XMLReaderConfig.h"
#include "Colors.h"
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
  doc.LoadFile();
  if(!doc.LoadFile())
  {
    std::cout<<"Error loading file"<< std::endl;
    std::cout<<"Error #" << doc.ErrorId() << " : " << doc.ErrorDesc()<<std::endl;
  }
  else
  {
    std::cout<<"File : "<<FileName<<std::endl;
    TiXmlHandle hdl(&doc);
    TiXmlElement*  element = hdl.FirstChildElement().FirstChildElement().Element();
    std::string MID="";
    bool founded=false;
    int number=0;
    while (element&&founded==false)
    {
      number++;
      std::string name="";
      std::string MID1="";
      if(element->FirstChildElement("Run")->GetText()!=nullptr)name = element->FirstChildElement("Run")->GetText();
      if(element->FirstChildElement("MID")->GetText()!=nullptr)MID1 = element->FirstChildElement("MID")->GetText();
      if(name==std::to_string(RunNumber))
      {
        founded=true;
        MID=MID1;
		    DAQ_Name=element->FirstChildElement("DAQ_Name")->GetText();
		    std::cout<<green<<"Run Number "<<RunNumber<<" found in "<<FileName<<" I extract the data :) "<<normal<<std::endl;
      }
      element=element->NextSiblingElement();
    }
  }
  std::string path="./SlowControl/"+DAQ_Name+".xml";
  //First Try to find the SlowControl File in the folder SlowControl 
  TiXmlDocument docc(path.c_str());
  docc.LoadFile();
  if(!docc.LoadFile())
  {
    #ifdef OCCI_SUPPORT
    std::cout<<green<<"SlowControl file not found int SlowControl folder"<<normal<<std::endl;
    std::cout<<green<<"Now I will connect to the Database to download SlowControl :) with DAQ_Name "<<DAQ_Name<<normal<<std::endl;
    //DBInit::init();
    //State* s = State::download(DAQ_Name); // download the state with name 'MyState'
    //s->saveToXML(path.c_str()); // creates the XML file
    //DBInit::terminate();
    //XMLReaderConfig slowcontrolreader;
    //slowcontrolreader.Read(path,Conf);
    #else
    std::cout<<red<<"SlowControl file not found int SlowControl folder and I can't connect me to the Database :("<<normal<<std::endl;
    #endif
  }
  else
  {
	std::cout<<green<<"SlowControl file found in SlowControl folder"<<normal<<std::endl;
	XMLReaderConfig slowcontrolreader;
    	slowcontrolreader.Read(path,Conf);
  }
}
