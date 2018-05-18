#include "Streamout.h"
#include "Global.h"
//#include "BufferNavigator.h"
//#include "LMGenericObject.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <cstdint>
#include <EVENT/LCCollection.h>
#include <EVENT/LCGenericObject.h>
#include <IMPL/LCGenericObjectImpl.h>
#include <UTIL/LCTOOLS.h>
#include "IMPL/LCCollectionVec.h"
#include "IMPL/LCFlagImpl.h"
#include "IMPL/LCParametersImpl.h"
#include "IMPL/RawCalorimeterHitImpl.h"
#include <bitset>
#include "UTIL/LCTOOLS.h"
#include "UTIL/CellIDEncoder.h"
#include "IMPL/LCEventImpl.h"
#include "IMPL/CalorimeterHitImpl.h"
#include <IMPL/LCRunHeaderImpl.h>
#include <sstream>
#include <set>
#include <limits>
using namespace lcio;

Streamout aStreamout;

Streamout::Streamout() : Processor("Streamout") 
{
  _description = "Streamout converts SDHCAL RawData into collection easier to analyse";
  registerInputCollection(LCIO::LCGENERICOBJECT,"XDAQCollectionName","XDAQ produced collection name", _XDAQCollectionNames,std::string("RU_XDAQ"));
  registerOutputCollection(LCIO::RAWCALORIMETERHIT, "OutputRawCaloHitCollectionName","Name of output collection containing raw calorimeter hits",
  _RawHitCollectionName, std::string("DHCALRawHits2"));
  _nevt = _nWrongObj = _nProcessedObject = _hasSlowControl =_hasBadSlowControl = 0;
  _BitsToSkip = 24;
  registerProcessorParameter("BitsToSkip", "BitsToSkip (default 24)",_BitsToSkip, _BitsToSkip);
  virer_full_asic = false;
  registerProcessorParameter("Virer_full_asic", "Virer_full_asic",virer_full_asic, virer_full_asic);
  _DataFormatType="SDHCAL";
  registerProcessorParameter("Data_Format_Type", "Data_Format_Type",_DataFormatType,_DataFormatType);
  chFlag.setBit(bitinfo.RCHBIT_LONG);   // raw calorimeter data -> format long                                     // //(sert a qq chose?)
  chFlag.setBit(bitinfo.RCHBIT_BARREL); // barrel
  chFlag.setBit(bitinfo.RCHBIT_ID1);    // cell ID
  chFlag.setBit(bitinfo.RCHBIT_TIME); // timestamp
  chFlag.setBit(bitinfo.RCHBIT_ENERGY_ERROR);
}

void Streamout::init() 
{
  printParameters();
  parseDataFormat= new ParseDataFormat();
  if(parseDataFormat->set_DataFormat(_DataFormatType)==false) std::exit(1);
}

void Streamout::modifyEvent( EVENT::LCEvent* evt )
{
  LCEventImpl *raw = dynamic_cast<LCEventImpl*>( evt) ;
  raw->setTimeStamp(evt->getTimeStamp()*1000000000);
  evt=raw;
}

void Streamout::modifyRunHeader(EVENT::LCRunHeader * rh)
{
  LCRunHeaderImpl *raw=nullptr;
  if(rh!=nullptr)
  {
    raw = dynamic_cast<LCRunHeaderImpl*>(rh);
    raw->setRunNumber(rh->getRunNumber());
    raw->setDetectorName(rh->getDetectorName());
  }
  else 
  {
    raw=new LCRunHeaderImpl;
    raw->setRunNumber(0);
    raw->setDetectorName("");
    raw->setDescription("");
  }
  rh=raw;
}

void Streamout::processRunHeader(LCRunHeader *run) 
{
}

void Streamout::processEvent(LCEvent *evt) 
{
  IMPL::LCCollectionVec *RawVec = new IMPL::LCCollectionVec(LCIO::RAWCALORIMETERHIT);
  RawVec->setFlag(chFlag.getFlag());
  CellIDEncoder<RawCalorimeterHitImpl>cd("DIF_Id:8,Asic_Id:8,Channel:6,BarrelEndcapModule:10,FrameBCID:32",RawVec ) ;
  parseDataFormat->SetCellIDEncoder(cd);
  parseDataFormat->SetLCCollectionVec(RawVec);
  try 
  {
    LCCollection *col = evt->getCollection(_XDAQCollectionNames);
    _nevt++;
    cd.setCellIDFlag();
    int nElement = col->getNumberOfElements();
    _CollectionSizeCounter[nElement]++;
    for (int iel = 0; iel < nElement; iel++) 
    {
      EVENT::LCObject* obj=col->getElementAt(iel);
      parseDataFormat->SetRawBuffer(obj);
      parseDataFormat->Parse();
    } // for (int iel=0; iel<nElement; iel++)
  } 
  catch (DataNotAvailableException &e) 
  {
    std::cout<<red<< _XDAQCollectionNames << " collection not available" << std::endl;
  }
  evt->addCollection(RawVec,_RawHitCollectionName);
}

void Streamout::printCounter(std::string description, std::map<int, int> &m) 
{
  std::cout << " statistics for " << description << " : ";
  for (std::map<int, int>::iterator it = m.begin(); it != m.end(); it++) 
  {
    if (it != m.begin()) std::cout << ",";
    std::cout << " [" << it->first << "]=" << it->second;
  }
  std::cout << std::endl;
}

void Streamout::end() 
{
  std::cout << "FINAL STATISTICS : " << std::endl;
  std::cout << " runs for " << _nevt << " events." << std::endl;
  printCounter("Size of GenericObject collections", _CollectionSizeCounter);
  std::cout << "Number of badly extracted LCGenericObject : "<< _nWrongObj << std::endl;
  std::cout << "Number of processed LCGenericObject : "<< _nProcessedObject << std::endl;
  printCounter("Start of DIF header", _DIFStarter);
  printCounter("Value after DIF data are processed", _DIFPtrValueAtReturnedPos);
  printCounter("Size remaining in buffer after end of DIF data",_SizeAfterDIFPtr);
  std::cout << "Number of Slow Control found " << _hasSlowControl<< " out of which " << _hasBadSlowControl << " are bad"<< std::endl;
  printCounter("Size remaining after all of data have been processed",_SizeAfterAllData);
  printCounter("Number on non zero values in end of data buffer",_NonZeroValusAtEndOfData);
  delete parseDataFormat;
}
