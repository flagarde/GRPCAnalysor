#include "IJKfiller.h"
#include "Global.h"
#include <iostream>
#include <string>
#include <fstream> 
#include "Processor.h"
#include "UTIL/LCTOOLS.h"
#include "UTIL/CellIDEncoder.h"
#include "EVENT/LCGenericObject.h"
#include "IMPL/LCCollectionVec.h"
#include "IMPL/LCEventImpl.h"
#include "Utilities.h"
#include "IMPL/CalorimeterHitImpl.h"
#include "IMPL/LCRunHeaderImpl.h"
#include "Colors.h"
#include "IMPL/LCParametersImpl.h"
#include "Mapping.h"
#include "EVENT/CalorimeterHit.h" 
#include "EVENT/RawCalorimeterHit.h"
#include "UTIL/CellIDDecoder.h"
#include "ConstructConverters.h"
using namespace lcio ;

void IJKfiller::FillIJK(RawCalorimeterHit* raw, LCCollectionVec* col,CellIDEncoder<CalorimeterHitImpl>& cd, CellIDDecoder<RawCalorimeterHit>& cd2)
{
  if(Global::Global::geom->GetDifNbrPlate(cd2(raw)["DIF_Id"])!=-1||_SupressHitsOfDifsNotInXML==false) 
	{
    CalorimeterHitImpl* caloHit = new CalorimeterHitImpl();
    converter->setType(cd2(raw)["DIF_Id"]);
    caloHit->setTime(float((*raw).getTimeStamp()));
    caloHit->setEnergy(float((*raw).getAmplitude()&3));
    cd["DIF_Id"]=int(cd2(raw)["DIF_Id"]);
    cd["Asic_Id"]=int(cd2(raw)["Asic_Id"]);
    cd["Channel"]=int(cd2(raw)["Channel"]);
    cd["I"] = converter->RawToIJK(cd["DIF_Id"],cd["Asic_Id"],cd["Channel"])[0] ;
    cd["J"] = converter->RawToIJK(cd["DIF_Id"],cd["Asic_Id"],cd["Channel"])[1] ;
    cd["K"] = converter->RawToIJK(cd["DIF_Id"],cd["Asic_Id"],cd["Channel"])[2] ;
    cd.setCellID( caloHit ) ;
    col->addElement(caloHit);
  }
}

IJKfiller aIJKfiller;

IJKfiller::IJKfiller():Processor("IJKfiller")
{
  _hcalCollections={"DHCALRawHits2"};
  registerInputCollections( LCIO::RAWCALORIMETERHIT,"HCALCollections","HCAL Collection Names",_hcalCollections,_hcalCollections);
  _SupressHitsOfDifsNotInXML = true;
  registerProcessorParameter("SupressHitsOfDifsNotInXML","SupressHitsOfDifsNotInXML",_SupressHitsOfDifsNotInXML,_SupressHitsOfDifsNotInXML);
} // end constructor

void IJKfiller::processRunHeader( LCRunHeader* run){}

void IJKfiller::init()
{ 
  printParameters();
  if(Global::geom) 
  {
    converter=new ConstructConverters(Global::geom);
    std::map<int, Dif >Difs=Global::Global::geom->GetDifs();;
    for(std::map<int, Dif >::iterator it=Difs.begin(); it!=Difs.end(); ++it) 
	  {
	    if(Global::geom->GetDifType(it->first)==bif)std::cout<<yellow<<"->Dif "<<it->first<<" is considered as BIF "<<normal<<std::endl;
	  }
	}
	else
	{
	  std::cout<<red<<"Need Geometry"<<normal<<std::endl;
	  std::exit(1);
	}
} // end init method

void IJKfiller::processEvent( LCEvent * evtP )
{
  if (evtP!= nullptr )
  {
    for(unsigned int i=0; i< _hcalCollections.size(); i++) 
    {
      try 
	    {
	      LCCollection* col = evtP ->getCollection(_hcalCollections[i].c_str());
	      CellIDDecoder<RawCalorimeterHit>decode(col);
        ////verify if timestamp negative 
        bool HasTimeStampNegative=false;
        for (int ihit=0; ihit < col->getNumberOfElements(); ++ihit) 
        {
          RawCalorimeterHit *raw = dynamic_cast<RawCalorimeterHit*>( col->getElementAt(ihit)) ;
          if (raw != nullptr) 
	        {
	          if(Global::Global::geom->GetDifNbrPlate(decode(raw)["DIF_Id"])==-1) 
	          {
	            if(Warningg[decode(raw)["DIF_Id"]]!=true) 
		          {
		            Warningg[decode(raw)["DIF_Id"]]=true;
		            if(_SupressHitsOfDifsNotInXML==false)
		            {
		              std::cout<<red<<"Please add DIF "<<decode(raw)["DIF_Id"]<<" to your geometry file; Dif considered as pads type"<<normal<<std::endl; 
		            }
		            else std::cout<<red<<"Please add DIF "<<decode(raw)["DIF_Id"]<<" to your geometry file; I'm skipping its data"<<normal<<std::endl; 
		          }
	          }
	          if(raw->getTimeStamp()<0)
	          {
	            std::vector<long long int>b{decode(raw)["DIF_Id"],decode(raw)["Asic_Id"],decode(raw)["Channel"]}; 
	            Negative[b][raw->getTimeStamp()]++;
	            std::cout<<"TimeStamp <=-1 : "<<raw->getTimeStamp()<<std::endl;
	            if(raw->getTimeStamp()<-1)HasTimeStampNegative=true;       
	          }
	        }
        }
        if(HasTimeStampNegative==false)
        {
          LCCollectionVec* col_event1 = new LCCollectionVec(LCIO::CALORIMETERHIT);
          CellIDEncoder<CalorimeterHitImpl> cd1( "I:8,J:7,K:10,DIF_Id:8,Asic_Id:6,Channel:7",col_event1) ;
	        col_event1->setFlag(col_event1->getFlag()|( 1 << LCIO::RCHBIT_LONG));
	        col_event1->setFlag(col_event1->getFlag()|( 1 << LCIO::RCHBIT_TIME));
	        col_event1->setFlag(col_event1->getFlag()|( 1 << LCIO::CHBIT_ID1));
	        col_event1->setFlag(col_event1->getFlag()|( 1 << LCIO::RCHBIT_BARREL));
          for (int ihit=0; ihit < col->getNumberOfElements(); ++ihit) 
          {
	          RawCalorimeterHit *raw = dynamic_cast<RawCalorimeterHit*>( col->getElementAt(ihit)) ;
	          if(Global::geom->GetDifNbrPlate(decode(raw)["DIF_Id"])!=-1) FillIJK(raw,col_event1,cd1,decode);
	          //delete raw;
	        }
	        evtP->addCollection(col_event1, "IJKFilled");
	        //delete col_event1;
	      }
	    }// end try block
      catch (DataNotAvailableException &e)
	    {
	      std::cout << "TRIGGER SKIPED ..."<<std::endl;
	      break;
	    }
    } //end  for(unsigned int i=0; i< _hcalCollections.size(); i++) 
  }
} //end processEvent

void IJKfiller::end()
{
  for(std::map<int,bool>::iterator it=Warningg.begin(); it!=Warningg.end(); it++) 
  {
    if(_SupressHitsOfDifsNotInXML==true)
		{
		  std::cout<<red<<"REMINDER::Data from Dif "<<it->first<<" are skipped !"<<normal<<std::endl;
		}
    else std::cout<<red<<"REMINDER::Data from Dif "<<it->first<<" are considered as pads type"<<normal<<std::endl;
  }
  if(Negative.size()!=0)
  {
    std::cout<<red<<"WARNING !!! : Negative Value(s) of timeStamp found. They are written in Negative_Values.txt"<<normal<<std::endl;
    std::ofstream fileNeg( "Negative_Values"+std::to_string(Global::number)+".txt", std::ios_base::out ); 
    for(std::map<std::vector<long long int>,std::map< long long int, long long int>>::iterator it=Negative.begin();it!=Negative.end();++it)
    {
	    fileNeg<<"Dif_Id : "<<it->first[0]<<" Asic_Id : "<<it->first[1]<<" Channel_Id : "<<it->first[2];
	    for(std::map< long long int, long long int>::iterator itt=it->second.begin();itt!=it->second.end();++itt)
	    {
	      fileNeg<<" Value : "<<itt->first<<" - "<<itt->second<<" Times; ";
	    }
	    fileNeg<<std::endl;
    }
    fileNeg.close();
  }
}
