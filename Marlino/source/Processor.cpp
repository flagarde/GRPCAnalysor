#include "Processor.h"
#include "ProcessorMgr.h" 
#include "Global.h"

using namespace lcio ;



  // set default verbosity level to MESSAGE
  //int Processor::Verbosity=Processor::MESSAGE;

  Processor::Processor(const std::string& typeName) :
    _description(" description not set by author ") ,
    _typeName( typeName ) ,
    _parameters(0) ,
    _isFirstEvent( true ),
    _str(0) {
  
    //register processor in map
    ProcessorMgr::instance()->registerProcessor( this ) ;
  
  }


  Processor::Processor() : _parameters(NULL), _isFirstEvent(false), _str(NULL) {}

  Processor::~Processor() {

    if( _parameters != 0 ){
      delete _parameters ;
    }

    if( _str !=0 )
      delete _str ;
  
    typedef ProcParamMap::iterator PMI ;
  
    for( PMI i = _map.begin() ; i != _map.end() ; i ++ ) {
    
      //     streamlog_out(DEBUG) << " deleting processor parameter " << i->second->name() 
      // 			 << " of processor [" << name() << "]" 
      // 			 << std::endl ;

      delete i->second ;
    }
  }


  void Processor::setParameters( StringParameters* parameters) {

    if( _parameters != 0 ){
      delete _parameters ;
      _parameters = 0 ;
    }
    _parameters = parameters ;

    updateParameters();
    
  }
  
  std::stringstream& Processor::log() const {
    
    if( _str !=0 )
      delete _str ;

    _str = new std::stringstream ;

    return *_str ;
  }







 

  //   ProcessorParameter* Processor::getProcessorParameter( const std::string name) {
  //     ProcParamMap::iterator it = _map.find(name) ;
  //     if( it != _map.end() )
  //       return it->second ;    
  //     else
  //       return 0 ;
  //   }

  bool Processor::parameterSet( const std::string& name ) {

    ProcParamMap::iterator it = _map.find(name) ;

    if( it != _map.end() )
      return it->second->valueSet() ;    
    else
      return false ;
  }
  
  void Processor::baseInit() {
    
    //fg: now in setParameters 
    // updateParameters();

    init() ;
  }
  
  void Processor::updateParameters() {

    typedef ProcParamMap::iterator PMI ;

    for( PMI i = _map.begin() ; i != _map.end() ; i ++ ) {

      i->second->setValue( _parameters ) ;
    }
  }

  void Processor::setReturnValue( bool val) {
    
    ProcessorMgr::instance()->setProcessorReturnValue(  this , val ) ;
  }
  
  void Processor::setLCIOInType(const std::string& collectionName,  const std::string& lcioInType) {
    _inTypeMap[ collectionName ] = lcioInType ;
  }

  std::string Processor::getLCIOInType( const std::string& colName ) {

    if( isInputCollectionName( colName )  )
      return _inTypeMap[ colName ] ;
    else
      return "" ;
  }


  std::string Processor::getLCIOOutType( const std::string& colName ) {

    if( isOutputCollectionName( colName )  )
      return _outTypeMap[ colName ] ;
    else
      return "" ;
  }


  bool Processor::isInputCollectionName( const std::string& pName  ) {
    return _inTypeMap.find( pName  ) != _inTypeMap.end() ;
  }
  
  void Processor::setLCIOOutType(const std::string& collectionName,  const std::string& lcioOutType) {
    _outTypeMap[ collectionName ] = lcioOutType ;
  }

  bool Processor::isOutputCollectionName( const std::string& pName  ) {
    return _outTypeMap.find( pName ) != _outTypeMap.end() ;
  }

  void Processor::setReturnValue( const std::string& name, bool val ){
  
    ProcessorMgr::instance()->setProcessorReturnValue(  this , val , name ) ;
  }
