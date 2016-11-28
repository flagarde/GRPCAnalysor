#include "LCIOOutputProcessor.h"
#include <iostream>

#include "IMPL/LCRunHeaderImpl.h"
#include "UTIL/LCTOOLS.h"
#include "EVENT/LCCollection.h"
#include "IMPL/LCCollectionVec.h"
#include "UTIL/LCSplitWriter.h"
#include "Colors.h"
#include <algorithm>
#include <bitset>
#include "Global.h"

LCIOOutputProcessor anLCIOOutputProcessor ;
  
LCIOOutputProcessor::~LCIOOutputProcessor() {} 

LCIOOutputProcessor::LCIOOutputProcessor(const std::string& typeName) : Processor(typeName), _lcWrt(nullptr),_nRun(-1),_nEvt(-1)   
{
  myConstruct() ;
}
  
LCIOOutputProcessor::LCIOOutputProcessor() : Processor("LCIOOutputProcessor"),_lcWrt(nullptr),_nRun(-1),_nEvt(-1)  
{
  myConstruct() ;
}

void LCIOOutputProcessor::myConstruct()
{
  _description = "Writes the current event to the specified LCIO outputfile. Needs to be the last ActiveProcessor." ;
  registerProcessorParameter( "LCIOOutputFile"," name of output file "  ,_lcioOutputFile ,std::string("outputfile_*_.slcio") ) ;
  registerProcessorParameter( "LCIOWriteMode" , "write mode for output file:  WRITE_APPEND or WRITE_NEW",_lcioWriteMode ,std::string("None") ) ;
  StringVec dropNamesExamples ;
  registerOptionalParameter("DropCollectionNames","drops the named collections from the event",_dropCollectionNames ,dropNamesExamples) ;
  StringVec dropTypesExample ;
  registerOptionalParameter("DropCollectionTypes","drops all collections of the given type from the event",_dropCollectionTypes,dropTypesExample ) ;
  StringVec keepNamesExample ;
  registerOptionalParameter( "KeepCollectionNames","force keep of the named collections - overrules DropCollectionTypes (and DropCollectionNames)",_keepCollectionNames ,keepNamesExample ) ;
  StringVec fullSubsetExample ;
  registerOptionalParameter( "FullSubsetCollections"," write complete objects in subset collections to the file (i.e. ignore subset flag)"  ,_fullSubsetCollections,fullSubsetExample ) ;
  registerOptionalParameter( "SplitFileSizekB","will split output file if size in kB exceeds given value - doesn't work with APPEND and NEW"  ,_splitFileSizekB, 19922940 ) ;  // 19 GB in kB
}

void LCIOOutputProcessor::init() 
{ 
  _realname=_lcioOutputFile;
  std::size_t found = _lcioOutputFile.find("*");
  if(found!=std::string::npos)_lcioOutputFile.replace(found,1,std::to_string(Global::number));
  std::cout<<green<<"Creating "<<_lcioOutputFile<<normal<<std::endl;
  if(  parameterSet("SplitFileSizekB") )
  {
    _lcWrt = new LCSplitWriter( LCFactory::getInstance()->createLCWriter(), _splitFileSizekB*1024  ) ;  
  } 
  else 
  {  
    _lcWrt = LCFactory::getInstance()->createLCWriter() ;
  }
  if( _lcioWriteMode == "WRITE_APPEND" ) 
  { 
    _lcWrt->open( _lcioOutputFile , LCIO::WRITE_APPEND ) ;
  }
  else if( _lcioWriteMode == "WRITE_NEW" ) 
  {  
    _lcWrt->open( _lcioOutputFile , LCIO::WRITE_NEW ) ;
  }
  else 
  {
    _lcWrt->open(_lcioOutputFile ) ;
  }
  printParameters() ;
  _nRun = 0 ;
  _nEvt = 0 ;
}

void LCIOOutputProcessor::processRunHeader( LCRunHeader* run) 
{ 
  _lcWrt->writeRunHeader( run ) ;
  _nRun++ ;
} 

void LCIOOutputProcessor::dropCollections( LCEvent * evt ) 
{ 
  const StringVec*  colNames = evt->getCollectionNames() ;    
  for( StringVec::const_iterator it = colNames->begin();it != colNames->end() ; it++ )
  {
    LCCollectionVec*  col =  dynamic_cast<LCCollectionVec*> (evt->getCollection( *it ) ) ;
    std::string type  = col->getTypeName() ;
    if( parameterSet("DropCollectionTypes") && std::find( _dropCollectionTypes.begin(),_dropCollectionTypes.end(), type ) != _dropCollectionTypes.end()  ) 
    {
      col->setTransient( true ) ;
    }
    if( parameterSet("DropCollectionNames") && std::find( _dropCollectionNames.begin(), _dropCollectionNames.end(), *it ) != _dropCollectionNames.end() ) 
    {
	    col->setTransient( true ) ;
    }
    if( parameterSet("KeepCollectionNames") && std::find( _keepCollectionNames.begin(), _keepCollectionNames.end(), *it ) != _keepCollectionNames.end() ) 
    {
	    col->setTransient( false ) ;
    }
    if( parameterSet("FullSubsetCollections") && std::find( _fullSubsetCollections.begin(), _fullSubsetCollections.end(), *it ) != _fullSubsetCollections.end() ) 
    {
	    if( col->isSubset() ) 
	    {
        col->setSubset( false ) ;
	      _subSets.push_back(col) ;
	    }
    }
  }
}

void LCIOOutputProcessor::processEvent( LCEvent * evt ) 
{ 
  dropCollections( evt ) ;
  _lcWrt->writeEvent( evt ) ;
  // revert subset flag - if any 
  for( SubSetVec::iterator sIt = _subSets.begin() ; sIt != _subSets.end() ;  ++sIt  ) 
  {  
    (*sIt)->setSubset( true ) ;
  }
  _subSets.clear() ;				 
  _nEvt ++ ;
}

void LCIOOutputProcessor::end()
{ 
  
  std::cout<<yellow<< std::endl 
			      << "LCIOOutputProcessor::end()  " << name() 
			      << ": " << _nEvt << " events in " << _nRun << " runs written to file  " 
			      <<  _lcioOutputFile  
			      << std::endl
			      <<normal<< std::endl ;
  _lcioOutputFile=_realname;
  _lcWrt->close() ;
}
