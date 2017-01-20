#include "ProcessorMgr.h"
#include "Global.h"
#include "Exceptions.h"
#include <sstream>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <set>
#include "EventModifier.h"
#include <time.h>

ProcessorMgr* ProcessorMgr::_me = nullptr ;

static clock_t start_t , end_t ; 
typedef std::map< Processor* , std::pair< double  , int > > TimeMap ;
static TimeMap tMap ;

// helper for sorting procs wrt to processing time
struct Cmp
{
  bool operator()(const TimeMap::value_type& v1, const TimeMap::value_type& v2 ) 
  {
    return v1.second.first > v2.second.first ;
  }
} ;

ProcessorMgr::ProcessorMgr(){ }
  
ProcessorMgr* ProcessorMgr::instance() 
{
  if( _me == nullptr ) _me = new ProcessorMgr ;
  return _me ;
}  
  
ProcessorMgr::~ProcessorMgr(){}

void ProcessorMgr::registerProcessor( Processor* processor )
{
  const std::string& name = processor->type()  ;
  if( _map.find( name ) != _map.end() ) return;
  else _map[ name ] = processor ;
}

std::set< std::string > ProcessorMgr::getAvailableProcessorTypes()
{
  std::set< std::string > ptypes;
  for(ProcessorMap::iterator i=_map.begin() ; i!= _map.end() ; i++) ptypes.insert(i->first);
  return ptypes;
}

Processor* ProcessorMgr::getProcessor( const std::string& type )
{
  return _map[ type ] ;
}

Processor* ProcessorMgr::getActiveProcessor( const std::string& name ) 
{
  return _activeMap[ name ] ;
}

void ProcessorMgr::removeActiveProcessor(  const std::string& name ) 
{
  _list.remove( _activeMap[name] ) ;
  _activeMap.erase( name ) ;
}

bool ProcessorMgr::addActiveProcessor( const std::string& processorType,const std::string& processorName, StringParameters* parameters ,const std::string condition) 
{
  Processor* processor = getProcessor( processorType ) ;
  if( processor == nullptr ) 
  {
    std::stringstream sstr ;
    sstr << " ProcessorMgr::registerProcessor: unknown processor with type " <<  processorType  << " ! " << std::endl   ;
    throw Exception( sstr.str() );
  }
  if( _activeMap.find( processorName ) != _activeMap.end() )
  {
    std::cerr << " ProcessorMgr::addActiveProcessor: processor " <<  processorName << " already registered ! "<< std::endl ;
    return false ;
  } 
  else 
  {
    Processor* newProcessor = processor->newProcessor() ;
    newProcessor->setName( processorName ) ;
    _activeMap[ processorName ] = newProcessor ;
    _list.push_back( newProcessor ) ;
    _conditions.addCondition( processorName, condition ) ;
    if( parameters != nullptr )newProcessor->setParameters( parameters  ) ;
  }
  return true ;
}

void ProcessorMgr::init()
{ 
  for( ProcessorList::iterator it = _list.begin() ; it != _list.end() ; ++it ) 
  {
	  (*it)->baseInit() ;
	  tMap[ *it ] = std::make_pair( 0 , 0 )  ;
	  EventModifier* em = dynamic_cast<EventModifier*>( *it ) ; 
	  if( em != nullptr ) 
	  {  
	    _eventModifierList.push_back( *it ) ;	 
	    std::cout << " -----------   " << std::endl
				      << " the following processor will modify the LCIO event :  "
				      << (*it)->name()  << " !! " <<  std::endl
				      << " ------------  "   << std::endl ; 
	  }
	}
}

void ProcessorMgr::processRunHeader( LCRunHeader* run)
{ 
  for( ProcessorList::iterator it = _list.begin() ; it != _list.end() ; ++it ) (*it)->processRunHeader( run ) ;
}   
  
void ProcessorMgr::modifyRunHeader( LCRunHeader* rhd )
{ 
  for( ProcessorList::iterator it = _eventModifierList.begin();  it !=  _eventModifierList.end()  ; ++ it) 
  {    
    (  dynamic_cast<EventModifier*>( *it )  )->modifyRunHeader( rhd ) ;
  }
}

void ProcessorMgr::modifyEvent( LCEvent* evt )
{ 
  for( ProcessorList::iterator it = _eventModifierList.begin();  it !=  _eventModifierList.end()  ; ++ it) 
  {
    (dynamic_cast<EventModifier*>( *it ))->modifyEvent( evt ) ;
  }
  bool check = ( Global::parameters->getStringVal("SupressCheck") != "true" ) ;
  bool modify = ( Global::parameters->getStringVal("AllowToModifyEvent") == "true" ) ;
  if( modify ) 
  {
	  try
	  { 
	    for( ProcessorList::iterator it = _list.begin() ; it != _list.end() ; ++it ) 
	    {
	      if( _conditions.conditionIsTrue( (*it)->name() ) ) 
	      {
	        start_t =  clock () ;  // start timer
	        (*it)->processEvent( evt ) ; 
	        if( check )  (*it)->check( evt ) ;
	        end_t =  clock () ;  // stop timer
	        TimeMap::iterator itT = tMap.find( *it ) ; 
	        itT->second.first += double( end_t - start_t  ) ; 
	        itT->second.second ++ ;
	        (*it)->setFirstEvent( false ) ;
	      }       
	    }    
    } 
    catch( SkipEventException& e)
    {
	    ++ _skipMap[ e.what() ] ;
    }  
	} // end modify
}
  
void ProcessorMgr::processEvent( LCEvent* evt )
{ 
  _conditions.clear() ;
  bool check = ( Global::parameters->getStringVal("SupressCheck") != "true" ) ;
  bool modify = ( Global::parameters->getStringVal("AllowToModifyEvent") == "true" ) ;
	if( modify ) return ;   // processorEventMethods already called in modifyEvent() ...
	try
	{ 
    for( ProcessorList::iterator it = _list.begin() ; it != _list.end() ; ++it ) 
    {
      if( _conditions.conditionIsTrue( (*it)->name() ) ) 
      {
        start_t =  clock () ;  // start timer
        (*it)->processEvent( evt ) ; 
        if( check )  (*it)->check( evt ) ;
        end_t =  clock () ;  // stop timer
        TimeMap::iterator itT = tMap.find( *it ) ;
        itT->second.first += double( end_t - start_t  ) ; 
        itT->second.second ++ ;
        (*it)->setFirstEvent( false ) ;
      }       
    }    
  } 
  catch( SkipEventException& e)
  {
    ++ _skipMap[ e.what() ] ;
  }  
}

void ProcessorMgr::setProcessorReturnValue( Processor* proc, bool val ) 
{
  _conditions.setValue( proc->name() , val ) ;
}
    
void ProcessorMgr::setProcessorReturnValue( Processor* proc, bool val, const std::string& name)
{
  std::string valName = proc->name() + "." + name ;
  _conditions.setValue( valName , val ) ;
}

void ProcessorMgr::end()
{ 
  for( ProcessorList::reverse_iterator it = _list.rbegin() ; it != _list.rend() ; ++it ) (*it)->end() ;
  std::cout<< " --------------------------------------------------------- " << std::endl;
  std::cout<< "  Events skipped by processors : " << std::endl ;
  unsigned nSkipped = 0 ;
  for( SkippedEventMap::iterator it = _skipMap.begin() ; it != _skipMap.end() ; it++) 
  {
    std::cout << "       " << it->first << ": \t" <<  it->second << std::endl ;
    nSkipped += it->second ;	
  }
  std::cout  << "  Total: " << nSkipped  << std::endl ;
  std::cout  << " --------------------------------------------------------- "  << std::endl<< std::endl ;
  // ----- print timing information ----------
  std::cout  << " --------------------------------------------------------- " << std::endl;
  std::cout<< "      Time used by processors ( in processEvent() ) :      " << std::endl<<std::endl ;
  // sort procs wrt processing time :
  typedef std::list< TimeMap::value_type > TMList  ;
  TMList l ;
  std::copy(  tMap.begin() , tMap.end() , std::back_inserter( l ) )  ;
  l.sort( Cmp() ) ; 
  double tTotal = 0.0 ;
  int evtTotal = 0 ;
  for( TMList::iterator itT = l.begin() ; itT != l.end() ; ++ itT ) 
  {
    char cName[40] = "                                     "  ;
    const std::string& sName = itT->first->name()  ;
    unsigned nChar = ( sName.size() > 30 ?  30 : sName.size() )  ;
    for(unsigned  i=0 ; i< nChar ; i++ ) cName[i] = sName[i] ;  
    double tProc = itT->second.first  / double(CLOCKS_PER_SEC) ;
    tTotal += tProc ;
    int evtProc = itT->second.second ;
    if( evtProc > evtTotal )evtTotal = evtProc ; 
    std::cout  <<cName<<  std::setw(12) << std::scientific  << tProc  << " s in " <<  std::setw(12) << evtProc << " events  ==> " ;
    if( evtProc > 0 )std::cout  <<  std::setw(12) << std::scientific  << tProc / evtProc << " [ s/evt.] "  ;
    else std::cout  <<  std::setw(12) << std::scientific  << "NaN"  << " [ s/evt.] "  ;
    std::cout <<  std::endl ;
  }
  std::cout  <<  "            Total:                   "  
  <<  std::setw(12) << std::scientific  << tTotal << " s in " 
  <<  std::setw(12) << evtTotal << " events  ==> " ;
  if( evtTotal > 0 ) std::cout  <<  std::setw(12) << std::scientific  << tTotal / evtTotal << " [ s/evt.] "  ;
  else std::cout  <<  std::setw(12) << std::scientific  << "NaN"  << " [ s/evt.] "  ;
  std::cout  <<  std::endl ;
  std::cout << " --------------------------------------------------------- "  << std::endl ;
}
