#ifndef TRIGGER
#define TRIGGER
#include <map>
#include <vector>
#include "Processor.h"
#include "EVENT/RawCalorimeterHit.h"
#include "IMPL/LCCollectionVec.h"
#include "UTIL/CellIDEncoder.h"
#include "IMPL/CalorimeterHitImpl.h"
#include "EVENT/LCRunHeader.h"
#include "IMPL/LCEventImpl.h"
#include <set>
#include <string>
#include <iomanip>
#include <fstream>
#include <iostream>

enum Threshold{Threshold_2=1,Threshold_1,Threshold_3};

class Trigger : public Processor
{
public:
    Trigger();
    ~Trigger();
    Processor *newProcessor()
    {
        return new Trigger();
    }
    void init();
    void processEvent(EVENT::LCEvent *evtP);
    void processRunHeader( LCRunHeader* run);
    void end();
    inline void FillTimes();
private:
    void processCollection(EVENT::LCEvent *evtP,LCCollection* col);    
    std::vector<std::string> _hcalCollections;
    int _TimeWin_Noise;
    std::map<int,int>Times;
    unsigned int TouchedEvents;
    unsigned int eventtotal;
    int _Time_from_Track;
    unsigned int _eventNr;
    int _trig_count;
    int _noiseCut;
    int _LayerCut;
    int _Time_before;
    int _Time_after;
    std::map< int,std::vector<CalorimeterHit*> > RawHits;
    std::map< int,int > BIFHits;
};
#endif
