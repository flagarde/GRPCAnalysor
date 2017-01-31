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
    int _TriggerTimeLow;
    int _TriggerTimeHigh;
    unsigned int TouchedEvents;
    unsigned int EventsSelected;
    unsigned int eventtotal;
    int TRIGGERSKIPPED;
    int _noiseCut;
    LCWriter* _EventWriter;
    std::string _outFileName;
    std::map<int,TH1F*>TimeDistribution;
    double TotalTimeRejected;
    double TotalTime;
    std::map<int,TH1F*>TimeDistributionRejected;
    std::map<int,TH2F*>HitsDistribution;
    std::map<int,TH2F*>HitsDistributionRejected;
    std::pair<double,double>MinMaxTimeRejected;
    std::pair<double,double>MinMaxTime;
    std::map<int,int> NumberOfEventsEfficientDIF;
    std::map<int,int> NumberOfEventsEfficientPlan;
    std::map< int,std::vector<CalorimeterHit*> > SelectedHits;
    std::map< int,std::vector<CalorimeterHit*> > RejectedHits;
};
#endif
