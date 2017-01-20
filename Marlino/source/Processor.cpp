#include "Processor.h"
#include "ProcessorMgr.h" 
#include "Global.h"

using namespace lcio ;

Processor::Processor(const std::string& typeName):_description(""),_typeName( typeName ),_parameters(nullptr),_isFirstEvent( true ),_str(nullptr) 
{
  //register processor in map
  ProcessorMgr::instance()->registerProcessor( this ) ;
}

Processor::Processor() : _parameters(nullptr), _isFirstEvent(false), _str(nullptr) {}

Processor::~Processor() 
{
  if( _parameters != nullptr )
  {
    delete _parameters ;
  }
  if( _str !=nullptr )delete _str ;
  typedef ProcParamMap::iterator PMI ;
  for( PMI i = _map.begin() ; i != _map.end() ; i ++ ) 
  {
    delete i->second ;
  }
}
  
void Processor::setParameters( StringParameters* parameters) 
{
  if( _parameters != nullptr )
  {
    delete _parameters ;
    _parameters = nullptr ;
  }
  _parameters = parameters ;
  updateParameters();
}
  
std::stringstream& Processor::log() const 
{  
  if( _str !=nullptr )delete _str ;
  _str = new std::stringstream ;
  return *_str ;
}

bool Processor::parameterSet( const std::string& name ) 
{
  ProcParamMap::iterator it = _map.find(name) ;
  if( it != _map.end() ) return it->second->valueSet() ;    
  else return false ;
}
  
void Processor::baseInit() 
{
  init() ;
}
  
void Processor::updateParameters() 
{
  typedef ProcParamMap::iterator PMI ;
  for( PMI i = _map.begin() ; i != _map.end() ; i ++ ) 
  {
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
