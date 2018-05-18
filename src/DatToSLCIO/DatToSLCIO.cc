
#include <iostream>
#include <string>
#include <fcntl.h>
#include <unistd.h>

#include "lcio.h"
#include "IO/LCWriter.h"
#include "IOIMPL/LCFactory.h"
#include "EVENT/LCIO.h"
#include "IMPL/LCRunHeaderImpl.h"
#include "IMPL/LCEventImpl.h" 
#include "IMPL/LCCollectionVec.h"
#include "EVENT/LCIntVec.h"
#include "IMPL/LCGenericObjectImpl.h"

using namespace lcio ;

//helper class 
class myGenObj : public LCGenericObjectImpl
{
public:
  myGenObj(int nInt) : LCGenericObjectImpl(nInt,0,0) {}
  std::vector<int>& getIntVec() {return _intVec;}
};


int main(int argc, char *argv[])
{
  if (argc==1) { std::cerr << "please provide the file name to convert" << std::endl; return 1;}

  std::string inputFileName=argv[1];
  std::string outputFileName=inputFileName+".slcio";
  std::cout << "Conversion of " << inputFileName << " into " << outputFileName << std::endl;

  int32_t _fdIn=::open(inputFileName.c_str(), O_RDONLY | O_NONBLOCK,S_IRWXU);
  if (_fdIn<0) { std::cerr << "Can't open file :" << inputFileName << std::endl; return 2;}
  
  LCWriter* lcWrt = LCFactory::getInstance()->createLCWriter();
  lcWrt->open(outputFileName, LCIO::WRITE_NEW );

  
  LCRunHeaderImpl* runHdr = new LCRunHeaderImpl; 
  int runNumber=0;
  if (argc==3) runNumber=atoi(argv[2]);
  runHdr->setRunNumber( runNumber ) ;
  std::string detectorName="GIF++May2018";
  runHdr->setDetectorName( detectorName );
  lcWrt->writeRunHeader( runHdr ) ;

  
  uint32_t _event=0;
  int _totalevent=0;
  while (true)
    {
      int ier=::read(_fdIn,&_event,sizeof(uint32_t));
      if (ier<=0) { std::cout << "Cannot read anymore after " << _totalevent << " events." << std::endl; break;}
      _totalevent+=1;

      LCEventImpl*  evt = new LCEventImpl() ;
      evt->setRunNumber( runNumber );
      evt->setEventNumber( _event );
      evt->setDetectorName( detectorName );

      uint32_t theNumberOfDIF=0;
      ier=::read(_fdIn,&theNumberOfDIF,sizeof(uint32_t));
      if (ier<=0) { std::cout << "Cannot read anymore number of DIF" << std::endl; break;}
      evt->parameters().setValue("NumberOfDIFs",int(theNumberOfDIF));

      LCCollectionVec* GenObjectVec = new LCCollectionVec( LCIO::LCGENERICOBJECT );
	
      
      bool endReading=false;
      for (uint32_t idif=0;idif<theNumberOfDIF;idif++)
	{
	  uint32_t bsize=0;
	  ier=::read(_fdIn,&bsize,sizeof(uint32_t));
	  if (ier<=0)  { std::cout << "Cannot read anymore  DIF Size" << std::endl; endReading=true; break;}

	  uint32_t numberOfIntegers = bsize/sizeof(int); 
	  if ( bsize%sizeof(int) ) numberOfIntegers+=1;
	  numberOfIntegers+=1;
	  myGenObj *obj=new myGenObj(numberOfIntegers+1);
	  obj->getIntVec()[0]=bsize;
	  
	  ier=::read(_fdIn,&(obj->getIntVec()[1]),bsize);
	  if (ier<=0){ std::cout << "Cannot read anymore Read data" << std::endl; delete obj; endReading=true; break;}
	  GenObjectVec->push_back( obj );

	}
      if (endReading) break;

      evt->addCollection( GenObjectVec, "RU_XDAQ");
      lcWrt->writeEvent( evt ) ;
      delete evt ;
    }
  
  delete runHdr;
  lcWrt->close() ;
  delete lcWrt ;

  return 0;
}
