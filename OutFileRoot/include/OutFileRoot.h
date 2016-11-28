#ifndef OUTFILEROOT_h
#define OUTFILEROOT_h
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
      setTDRStyle();
    };
    OutFileRoot(std::string& outputFileName)
    {
      setTDRStyle();
      setOutputFile(outputFileName);
    }
    std::string replaceStrChar(std::string str, const std::string& replace, char ch);
    bool setOutputFile(std::string& outputFileName);
    bool writeObject(std::string& dirName, TObject *object);
    bool writeObject(const char * dirName, TObject *object);
  protected:
    std::string _outputFileName;
    bool isOutFile_;
    TFile* outFile_;
};
#endif