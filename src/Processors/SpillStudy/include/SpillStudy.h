#pragma once
#include "Processor.h"
using namespace lcio ;

class SpillStudy : public Processor
{
public:
    SpillStudy();
    ~SpillStudy();
    virtual SpillStudy *newProcessor()
    {
        return new SpillStudy();
    }
    virtual void init();
    virtual void processEvent(EVENT::LCEvent*);
    virtual void processRunHeader( LCRunHeader*);
    virtual void end();
    std::vector<std::string> _hcalCollections;
};
