#ifndef ProcessorMgr_h
#define ProcessorMgr_h 1
#include "lcio.h"
#include "Processor.h"
#include "IO/LCRunListener.h"
#include "IO/LCEventListener.h"
#include "EVENT/LCEvent.h"
#include "EVENT/LCRunHeader.h"
#include "LogicalExpressions.h"
#include <map>
#include <set>
#include <list>

using namespace lcio ;

class SkipEventException : public lcio::Exception
{
  protected:
    SkipEventException() {  /*no_op*/ ; } 
  public: 
    SkipEventException(const Processor* proc)
    {
      message = proc->name()  ;
    }
    virtual ~SkipEventException() throw() { /*no_op*/; } 
}; 

typedef std::map< const std::string , Processor* > ProcessorMap ;
typedef std::list< Processor* > ProcessorList ;
typedef std::map< const std::string , int > SkippedEventMap ;

/** Processor manager singleton class. Holds references to all registered Processors. 
 *    
 *  Responsible for creating the instance of ProcessorEventSeeder and setting the Global::EVENTSEEDER variable.
 * 
 *  @author F. Gaede, DESY
 *  @version $Id: ProcessorMgr.h,v 1.16 2007-08-13 10:38:39 gaede Exp $ 
 */
class ProcessorMgr : public LCRunListener, public LCEventListener 
{
  friend class  Processor ;     
  public:  
    /** Return the instance of this manager */
    static ProcessorMgr* instance() ;
    /** destructor deletes ProcessorEventSeeder */
    virtual ~ProcessorMgr() ;
    /** Add a processor of type processorType with name processorName to the list
     *  of active processors including a condition for the execution of the processEvent() method.
    */
    bool addActiveProcessor( const std::string& processorType , const std::string& processorName ,StringParameters* parameters , const std::string condition="true" ) ;
    /** Remove processor with name from list of active processors.
   */
    void removeActiveProcessor(  const std::string& name ) ;
    /** Return the active processor with the given name.
    * NULL if no processor exists. 
    */
    Processor* getActiveProcessor( const std::string& name ) ;
    /** Return the processor that has been registered with the given type.
    * NULL if no processor exists. 
    */
    Processor* getProcessor( const std::string& type ) ;
    virtual void init() ;
    virtual void processRunHeader( LCRunHeader* ) ;   
    virtual void processEvent( LCEvent* ) ;
    virtual void end() ;
    virtual void modifyRunHeader( LCRunHeader*) ; 
    virtual void modifyEvent( LCEvent *) ; 
    /** Set the return value for the given processor */
    virtual void setProcessorReturnValue( Processor* proc, bool val ) ;
    /** Set the named return value for the given processor */
    virtual void setProcessorReturnValue( Processor* proc, bool val , const std::string& name) ;
  protected:
    /** Register a processor with the given name. */
    void registerProcessor( Processor* processor ) ;
    /** Returns a list of all registered processors found */
    std::set< std::string > getAvailableProcessorTypes() ;
    ProcessorMgr() ;
  private:
    static ProcessorMgr*  _me ;
    ProcessorMap _map ;
    ProcessorMap _activeMap ;
    ProcessorList _list ;
    SkippedEventMap _skipMap ;
    ProcessorList _eventModifierList ;
    LogicalExpressions _conditions ;
};
#endif
