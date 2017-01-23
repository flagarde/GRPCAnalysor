#ifndef TRIVENT_TRIGGERED
#define TRIVENT_TRIGGERED
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

class TriventTriggered : public Processor
{
public:
    TriventTriggered();
    ~TriventTriggered();
    Processor *newProcessor()
    {
        return new TriventTriggered();
    }
    void init();
    void processEvent(EVENT::LCEvent *evtP);
    void processRunHeader( LCRunHeader* run);
    void end();
    inline void FillTimes();
private:  
    std::vector<std::string> _hcalCollections;
    int _TriggerTime;
    unsigned int TouchedEvents;
    unsigned int EventsSelected;
    unsigned int eventtotal;
    unsigned int _eventNr;
    int _trig_count;
    int _noiseCut;
    LCWriter* _EventWriter;
    std::map<int,bool>Warningg;
    std::string _outFileName;
    std::map<int,TH1F*>TimeDistribution;
    std::map< int,std::vector<CalorimeterHit*> > RawHits;
};
#endif
