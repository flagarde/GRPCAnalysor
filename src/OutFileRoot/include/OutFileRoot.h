#pragma once
#include <TROOT.h>
#include <TFile.h>
#include <TDirectory.h>
#include <TObject.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TH3F.h>
#include "TGraphErrors.h"
#include "tdrStyle.h"
#include "TLine.h"
#include "TLegend.h"
#include <string>
#include <iostream>
class OutFileRoot
{
  public:
    ~OutFileRoot();
    OutFileRoot()
    {
      RunName="";
      ProcessorName="";
      setTDRStyle();
    };
    OutFileRoot(std::string& outputFileName)
    {
      setTDRStyle();
      setOutputFile(outputFileName);
    }
    std::string replaceStrChar(std::string, const std::string&,char);
    bool setOutputFile(std::string&);
    bool setProcessorName(std::string&);
    bool setRunName(std::string&);
    bool writeObject(std::string&, TObject*);
    bool writeObject(const char *, TObject*);
  protected:
    std::string _outputFileName;
    bool isOutFile_;
    TFile* outFile_;
    std::string ProcessorName;
    std::string RunName;
};
