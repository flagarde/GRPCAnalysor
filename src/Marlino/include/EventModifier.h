#ifndef EventModifier_h
#define EventModifier_h 1

namespace EVENT{ class LCEvent ; }

class EventModifier 
{
  public:
    /** Implement to modify the event */
    virtual void modifyEvent( EVENT::LCEvent* ) = 0 ;
    /** Implement to modify the run header */
    virtual void modifyRunHeader(EVENT::LCRunHeader * rh) =0 ;
    /** Return  name of this  event modifier */
    virtual const std::string & name() const = 0 ;
    virtual ~EventModifier() {}
};
#endif
