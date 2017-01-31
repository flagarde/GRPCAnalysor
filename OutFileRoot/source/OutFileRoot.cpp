#include "OutFileRoot.h"
#include "TCanvas.h"
#include <cstdlib>
#include "Colors.h"
#include "TLatex.h"
#include <iostream>
#include "TH1.h"

std::string OutFileRoot::replaceStrChar(std::string str, const std::string& replace, char ch) 
{
  // set our locator equal to the first appearance of any character in replace
  size_t found = str.find_first_of(replace);
  while (found != std::string::npos) 
  { // While our position in the sting is in range.
    str[found] = ch; // Change the character at position.
    found = str.find_first_of(replace, found+1); // Relocate again.
  }
  return str; // return our new string.
}


bool OutFileRoot::setOutputFile(std::string& outputFileName)
{
  TH1::AddDirectory(kFALSE);
  outFile_ = new TFile(outputFileName.c_str(),"UPDATE");
  outFile_->SetBufferSize(5000000);
  outFile_->SetCompressionLevel(9);
  if(!outFile_) return false;
  isOutFile_ = true;
  return true;
}

bool OutFileRoot::setProcessorName(std::string& _ProcessorName)
{
  ProcessorName=_ProcessorName;
  return true;
}


bool OutFileRoot::setRunName(std::string& _RunName)
{
  RunName=_RunName;
  return true;
}

OutFileRoot::~OutFileRoot()
{
  if(isOutFile_) 
  {
    outFile_->Close();
    delete outFile_;
  }
}

bool OutFileRoot::writeObject(std::string& dirName, TObject *object)
{
  if(!outFile_->GetDirectory((RunName+ProcessorName+dirName).c_str())) 
  {
    outFile_->mkdir((RunName+ProcessorName+dirName).c_str());
    outFile_->cd((RunName+ProcessorName+dirName).c_str());
  }
  else outFile_->cd((RunName+ProcessorName+dirName).c_str());
  if(object!=nullptr)
  { 
    object->Write();
  }
  return true;
}

bool OutFileRoot::writeObject(const char * dirName, TObject *object)
{
  if(!outFile_->GetDirectory((RunName+ProcessorName+dirName).c_str()))
  {
    outFile_->mkdir((RunName+ProcessorName+dirName).c_str());
    outFile_->cd((RunName+ProcessorName+dirName).c_str());
  }
  else outFile_->cd((RunName+ProcessorName+dirName).c_str());
  if(object!=nullptr)
  { 
    object->Write();
  }
  return true;
}
