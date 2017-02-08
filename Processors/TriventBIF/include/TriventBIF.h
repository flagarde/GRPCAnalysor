#ifndef TRIVENTBIF
#define TRIVENTBIF
#include <map>
#include <vector>
#include "Processor.h"
#include "EVENT/RawCalorimeterHit.h"
#include "Geometry.h"
#include "IMPL/LCCollectionVec.h"
#include "UTIL/CellIDEncoder.h"
#include "IMPL/CalorimeterHitImpl.h"
#include <EVENT/LCRunHeader.h>
#include "UTIL/LCTOOLS.h"
#include "IMPL/LCEventImpl.h"
#include <set>
#include <string>
#include "Mapping.h"
#include "THnSparse.h"
#include "TCanvas.h"
#include <iomanip>
#include <fstream>
#include <iostream>

class TriventProcessor : public Processor
{
public:
    TriventProcessor();
    ~TriventProcessor();
    Processor *newProcessor()
    {
        return new TriventProcessor();
    }
    void init();
    void Writer(IO::LCWriter* file,const char* name,std::map<int,std::vector<EVENT::RawCalorimeterHit *>> & vec,EVENT::LCEvent * event,unsigned int& nbr,unsigned int IsNoise);
    void processEvent(EVENT::LCEvent *evtP);
    void processRunHeader( LCRunHeader* run);
    void end();
    void CalculateEfficiencyScinti(std::vector<RawCalorimeterHit *>vec);
private:
    void processCollection(EVENT::LCEvent *evtP,LCCollection* col);    
protected:
    std::string _noiseFileName;
    std::string _outFileName;
    LCWriter* _EventWriter;
    unsigned int TouchedEvents;
    int _timeWin;
    int _TimeWin_Noise;
    int _Time_from_Track;
    unsigned int _eventNr;
    int _trig_count;
    int _noiseCut;
    int _LayerCut;
    int _TriggerTime;
    unsigned int _GlobalEvents;
    std::string _Delimiters;
    double _efficiencyFrontScintillator;
    double _efficiencyBackScintillator;
    std::vector<int>EffiwithDiscri;
    unsigned int _Front_scintillator;
    unsigned int _Back_scintillator;
    unsigned int _Both_scintillator;
    std::map< int,std::vector<EVENT::RawCalorimeterHit*> > RawHits;
    std::map<int ,std::vector<double> >Delimiter;
 private:
    //helper methods
    void processEvent_ProcessCollectionNamed_Scintillator(LCEvent *evtP);
    void Loop_On_Scintillator_Collection(LCCollection *col3);
};

void TriventProcessor::CalculateEfficiencyScinti(std::vector<RawCalorimeterHit *>vec)
{
  unsigned roll=0;
  for(std::vector<RawCalorimeterHit *>::iterator it=vec.begin(); it!=vec.end(); ++it) 
  {
    
    TOTALNUMBERHITSCINTI++;
    int dif_id  = (*it)->getCellID0() & 0xFF ;
    int asic_id = ((*it)->getCellID0() & 0xFF00)>>8;
    int chan_id = ((*it)->getCellID0() & 0x3F0000)>>16;
    unsigned int NbrPlate =geom.GetDifNbrPlate(dif_id)-1;
    float Z= geom.GetPlatePositionZ(NbrPlate);
    unsigned int K =geom.GetDifNbrPlate(dif_id);
    unsigned int I=0;
    unsigned int J=0;
    if(geom.GetDifType(dif_id)==pad) 
    {
      I =(1+MapILargeHR2[chan_id]+AsicShiftI[asic_id])+geom.GetDifPositionX(dif_id);
      J =(32-(MapJLargeHR2[chan_id]+AsicShiftJ[asic_id]))+geom.GetDifPositionY(dif_id);
    }
    if(geom.GetDifType(dif_id)==positional) 
    {
      if(asic_id%2==0) Z= geom.GetPlatePositionZ(NbrPlate)+2;
      //if((asic_id%2==0&&geom.GetDifUpDown(dif_id)==1)||(asic_id%2==1&&geom.GetDifUpDown(dif_id)==0))
      if(geom.GetDifUpDown(dif_id)==1) 
      {
        I =(2*chan_id)+geom.GetDifPositionX(dif_id);
      } 
      else I =2*(64-chan_id)-1+geom.GetDifPositionX(dif_id);
      J =0;
    }
      unsigned int a,b,c,d;
      
	    if(Delimiter.find(dif_id)==Delimiter.end()){a=Delimiter[1][0];b=Delimiter[1][1];c=Delimiter[1][2];d=Delimiter[1][3];}
	    else {a=Delimiter[dif_id][0];b=Delimiter[dif_id][1];c=Delimiter[dif_id][2];d=Delimiter[dif_id][3];}
	    
      if(a<=I&&b>=I&&c<=J&&d>=J)
	    {
	   
	    roll++;
	    if(roll==1)EffiwithDiscri[geom.GetDifNbrPlate(dif_id)-1]+=1;
     // if(roll==1)std::cout<<geom.GetDifNbrPlate(dif_id)-1<<"  "<<EffiwithDiscri[geom.GetDifNbrPlate(dif_id)-1]<<std::endl;
      
      }
    }
    }
#endif
