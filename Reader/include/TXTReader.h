#ifndef TXTREADER_H
#define TXTREADER_H
#include"Reader.h"
#include<iostream>
#include "ConfigInfo.h"
class TXTReader: public Reader
{
  public:
    void Read(std::string& FileName,Geometry& geom);
    void Read(std::string& FileName,ConfigInfos* config){};
    void Read(std::string& FileName,ConfigInfos* config,int RunNumber){};
    Reader *Clone() { return new TXTReader(); }
    ~TXTReader(){};
};
#endif
