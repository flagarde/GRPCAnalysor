#ifndef Streamout_h
#define Streamout_h
#include "Processor.h"
#include "EventModifier.h"
#include "lcio.h"
#include <string>
#include <map>
//#include "DIFSlowControl.h"
#include "IMPL/LCFlagImpl.h"
#include "UTIL/LCTOOLS.h"
#include <cstdint>
//#include "BufferNavigator.h"
#include "ParseDataFormat.h"
#include "SDHCAL_DataFormat.h"
using namespace lcio ;

class Streamout : public Processor, public  EventModifier
{
  public:
    virtual Processor*  newProcessor()
    {
      return new Streamout ;
    }
    Streamout() ;
    ~Streamout(){};
    virtual void init() ;
    virtual void processRunHeader( LCRunHeader* run ) ;
    virtual void modifyEvent(LCEvent *evt) ;
    virtual void processEvent( LCEvent * evt ) ;
    virtual void end() ;
    virtual const std::string & name() const { return Processor::name() ; }
    virtual void modifyRunHeader(EVENT::LCRunHeader * rh);
  private:
    SDHCAL_DataFormat* data;
    IMPL::LCFlagImpl chFlag;
    EVENT::LCIO bitinfo;
    //ParseDataFormat* parseDataFormat;
    std::string  _XDAQCollectionNames ;
    std::string  _DataFormatType ;
    std::string _RawHitCollectionName;
    int _BitsToSkip;
    bool virer_full_asic;
    unsigned int _nevt;
    std::map<int,int> _CollectionSizeCounter;
    int _nWrongObj,_nProcessedObject, _hasSlowControl, _hasBadSlowControl;
    std::map<int,int> _DIFStarter;
    std::map<int,int> _DIFPtrValueAtReturnedPos;
    std::map<int,int> _SizeAfterDIFPtr;
    std::map<int,int> _SizeAfterAllData;
    std::map<int,int> _NonZeroValusAtEndOfData;
    void printCounter(std::string description, std::map<int,int> &);
} ;
#endif
