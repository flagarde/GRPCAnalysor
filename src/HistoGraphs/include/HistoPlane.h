#ifndef HISTO_PLANE
#define HISTO_PLANE
#include <string>
#include <map>
#include "TObject.h"
#include "Global.h"
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
#include <set>
enum Granularity{Detector=0,Plane,Dif,Asic,Pad};
 
class TObjectUgly
{
  public:
    TObjectUgly(){};
    ~TObjectUgly(){};
    std::map<std::string,int> GranularityName{{"Detector",0},{"Plane",1},{"Dif",2},{"Asic",3},{"Pad",4}};
    std::map<std::string,double>NameToInt;
    std::map<double,std::string>IntToName;
    std::map<double,std::string>IntToType;
    std::map<std::string,int>NameToWhere;
    std::map<double,TObject*>HistGraphs;
    std::map<double,int> PointsInGraphs;
    std::map<std::string,int> Rolling;
    void setSelected(int,int,int,int,int,int);
    int getHistoGraphNbr(double&);
    int getPlateNbr(double&);
    int getDifNbr(double&);
    int getAsicNbr(double&);
    int getPadNbr(double&);
    int getThresholdNbr(double&);
    void Fill(double,double=0,double=0,double=0,double=0,double=0);
    void Fill2(TObject*,double,double,double=0,double=0,double=0,double=0,double=0);
  private:
    int Plateselected;
    int Difselected;
    int Asicselected;
    int Padselected;
    int Thresholdselected;
    int HistGraphselected;
};


class HistoPlane
{
  public:
    ~HistoPlane();
    HistoPlane()
    {
      NotWriteEmpty=true;
      num=1;
    };
    void Add(const char*,const char*,std::vector<std::string>,int=0,double=0,double=0,int=0,double=0,double=0,int=0,double=0,double=0);
    void Add(const char*,const char*,const char*,int=0,double=0,double=0,int=0,double=0,double=0,int=0,double=0,double=0);
    double ConvertToInt(std::string name,int Plate,int Dif,int Asic,int Pad,int Threshold);
    void CreateHistoGraph(std::string n, std::string type,double& number,int=0,double=0,double=0, int=0,double=0,double=0,int=0,double=0,double=0);
    TObjectUgly& operator()(const char*,int Plate,int Dif,int Asic,int Pad,int Threshold);
    void Write();
    void List();
    void setRolling(std::string, bool);
  private:
    std::vector<std::string> SplitFilename (const std::string& str);
    std::set<std::string> Names;
    TObjectUgly ugly;
    bool NotWriteEmpty;
    double num;
};
#endif
