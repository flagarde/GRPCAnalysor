#pragma once
#include"Reader.h"
#include<iostream>
#include<string>
#include "tinyxml.h"
#include "ConfigInfo.h"
class XMLReader: public Reader
{
  public:
  void Read(std::string &FileName,Geometry& geom);
  void Read(std::string &FileName,ConfigInfos* conf){};
  void Read(std::string& FileName,ConfigInfos* config,int RunNumber){};
  void Write(TiXmlElement*,const char*,unsigned int&, unsigned int&,double& ,std::string& FileName);
  Reader *Clone() { return new XMLReader(); }
  ~XMLReader(){};
};
