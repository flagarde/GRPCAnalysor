#include "streamlog/logstream.h"

#include "streamlog/logbuffer.h"
#include "streamlog/prefix.h"
#include "streamlog/loglevels.h"

namespace streamlog{
  
  logstream::logstream() : 
    _ns( new nullstream ) , 
    _os(0) , 
    _threshold(0) ,
    _active(false) , 
    _lb(0),
    _prefix( new streamlog::prefix) {
    
  } 

  logstream::~logstream() {
    
    delete _ns ;
    
    if( _os) 
      delete _os ;
    
    if( _lb ) 
      delete _lb ;
    
    //if(_prefix)
      delete _prefix ;
  }
  
  void logstream::init( std::ostream& os , const std::string name, bool addDefaultNames ) {
    
    static bool first=true ;
    
    if( first && os ) {
      
      //      _name = name ;      
      
      // create a new log buffer and attach this to a wrapper to the given ostream  

      _lb = new logbuffer( os.rdbuf() , this ) ;
      
      _os = new std::ostream( _lb ) ;
      
      //attach also the original stream to the logger...
      //os.rdbuf( _lb ) ; // this needs some work 
      // FIXME : this needs to go to the c'tor !!!!
     
      _prefix->_name = name ;

      if( addDefaultNames  ) 
	addDefaultLevelNames() ;
      
      
      first = false ;
    }
    
    else if( !os) {
      std::cerr << "ERROR: logstream::init() invalid ostream given " << std::endl ;      
    }
  }

  unsigned logstream::setThreshold( const std::string& levelName ) {

    unsigned l = _threshold ;
    LevelMap::iterator it = _map.find( levelName ) ;
    if( it != _map.end() ) {
      _threshold = it->second ;
    }
    return l ;
  } 


 
  std::ostream& logstream::operator()() { 
    
    if( _active && _os ) {
      
      _active = false ;
      
      return *_os ;
    }
    else
      return *_ns ;
    
  }

  void logstream::addDefaultLevelNames(){

    _map[ "DEBUG"  ] = DEBUG::level ;
    _map[ "DEBUG0" ] = DEBUG0::level ;
    _map[ "DEBUG1" ] = DEBUG1::level ;
    _map[ "DEBUG2" ] = DEBUG2::level ;
    _map[ "DEBUG3" ] = DEBUG3::level ;
    _map[ "DEBUG4" ] = DEBUG4::level ;
    _map[ "DEBUG5" ] = DEBUG5::level ;
    _map[ "DEBUG6" ] = DEBUG6::level ;
    _map[ "DEBUG7" ] = DEBUG7::level ;
    _map[ "DEBUG8" ] = DEBUG8::level ;
    _map[ "DEBUG9" ] = DEBUG9::level ;

    _map[ "MESSAGE"  ] = MESSAGE::level ;
    _map[ "MESSAGE0" ] = MESSAGE0::level ;
    _map[ "MESSAGE1" ] = MESSAGE1::level ;
    _map[ "MESSAGE2" ] = MESSAGE2::level ;
    _map[ "MESSAGE3" ] = MESSAGE3::level ;
    _map[ "MESSAGE4" ] = MESSAGE4::level ;
    _map[ "MESSAGE5" ] = MESSAGE5::level ;
    _map[ "MESSAGE6" ] = MESSAGE6::level ;
    _map[ "MESSAGE7" ] = MESSAGE7::level ;
    _map[ "MESSAGE8" ] = MESSAGE8::level ;
    _map[ "MESSAGE9" ] = MESSAGE9::level ;

    _map[ "WARNING"  ] = WARNING::level ;
    _map[ "WARNING0" ] = WARNING0::level ;
    _map[ "WARNING1" ] = WARNING1::level ;
    _map[ "WARNING2" ] = WARNING2::level ;
    _map[ "WARNING3" ] = WARNING3::level ;
    _map[ "WARNING4" ] = WARNING4::level ;
    _map[ "WARNING5" ] = WARNING5::level ;
    _map[ "WARNING6" ] = WARNING6::level ;
    _map[ "WARNING7" ] = WARNING7::level ;
    _map[ "WARNING8" ] = WARNING8::level ;
    _map[ "WARNING9" ] = WARNING9::level ;

    _map[ "ERROR"  ] = ERROR::level ;
    _map[ "ERROR0" ] = ERROR0::level ;
    _map[ "ERROR1" ] = ERROR1::level ;
    _map[ "ERROR2" ] = ERROR2::level ;
    _map[ "ERROR3" ] = ERROR3::level ;
    _map[ "ERROR4" ] = ERROR4::level ;
    _map[ "ERROR5" ] = ERROR5::level ;
    _map[ "ERROR6" ] = ERROR6::level ;
    _map[ "ERROR7" ] = ERROR7::level ;
    _map[ "ERROR8" ] = ERROR8::level ;
    _map[ "ERROR9" ] = ERROR9::level ;
    
    _map[ "SILENT" ] = 0xFFFFFFFF ;
  }




  /** global instance of logstream */
  logstream out ;
}
