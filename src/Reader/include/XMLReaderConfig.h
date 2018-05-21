#pragma once
#include"Reader.h"
#include<iostream>
#include<string>
#include "tinyxml.h"
#include "ConfigInfo.h"
class XMLReaderConfig: public Reader
{
  public:
  void Read(std::string &FileName,ConfigInfos* config);
  void Read(std::string& FileName,Geometry& geom){};
  void Read(std::string& FileName,ConfigInfos* config,int RunNumber){};
  Reader *Clone() { return new XMLReaderConfig(); }
  ~XMLReaderConfig(){};
};
