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
using namespace lcio ;

void IJKfiller::FillIJK(RawCalorimeterHit* raw, LCCollectionVec* col,CellIDEncoder<CalorimeterHitImpl>& cd, CellIDDecoder<RawCalorimeterHit>& cd2)
{
    CalorimeterHitImpl* caloHit = new CalorimeterHitImpl();
    int DIF_Id  = cd2(raw)["DIF_Id"];
    int Asic_Id = cd2(raw)["Asic_Id"];
    int Channel = cd2(raw)["Channel"];
    
    caloHit->setTime(float((*raw).getTimeStamp()));
    caloHit->setEnergy(float((*raw).getAmplitude()&3));
    int K =Global::geom->GetDifNbrPlate(DIF_Id);
    int I=0;
    int J=0;
    if(Global::geom->GetDifType(DIF_Id)==pad) 
    {
      I =(1+MapILargeHR2[Channel]+AsicShiftI[Asic_Id])+Global::geom->GetDifPositionX(DIF_Id);
      J =(32-(MapJLargeHR2[Channel]+AsicShiftJ[Asic_Id]))+Global::geom->GetDifPositionY(DIF_Id);
    }
    if(Global::geom->GetDifType(DIF_Id)==positional) 
    {
      if(Global::geom->GetDifUpDown(DIF_Id)==1) I =(2*Channel)+Global::geom->GetDifPositionX(DIF_Id);
      else I =2*(64-Channel)-1+Global::geom->GetDifPositionX(DIF_Id);
      J =0;
    }
    cd["DIF_Id"]=DIF_Id;
    cd["Asic_Id"]=Asic_Id;
    cd["Channel"]=Channel;
    cd["I"] = I ;
    cd["J"] = J ;
    cd["K"] = K ;
    cd.setCellID( caloHit ) ;
    col->addElement(caloHit);
}

IJKfiller aIJKfiller;

IJKfiller::IJKfiller():Processor("IJKfiller")
{
  _hcalCollections={"DHCALRawHits2"};
  registerInputCollections( LCIO::RAWCALORIMETERHIT,"HCALCollections","HCAL Collection Names",_hcalCollections,_hcalCollections);
} // end constructor

void IJKfiller::processRunHeader( LCRunHeader* run){}

void IJKfiller::init()
{ 
  printParameters();
  if(Global::geom) 
  {
    std::map<int, Dif >Difs=Global::Global::geom->GetDifs();;
    for(std::map<int, Dif >::iterator it=Difs.begin(); it!=Difs.end(); ++it) 
	  {
	    if(Global::geom->GetDifType(it->first)==BIF)std::cout<<yellow<<"->Dif "<<it->first<<" is considered as BIF "<<normal<<std::endl;
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
        int numElements = col->getNumberOfElements();
        ////verify if timestamp negative 
        bool HasTimeStampNegative=false;
        for (int ihit=0; ihit < numElements; ++ihit) 
        {
          RawCalorimeterHit *raw = dynamic_cast<RawCalorimeterHit*>( col->getElementAt(ihit)) ;
          if (raw != nullptr) 
	        {
	          if(Global::Global::geom->GetDifNbrPlate(decode(raw)["DIF_Id"])==-1) 
	          {
	            if(Warningg[decode(raw)["DIF_Id"]]!=true) 
		          {
		            Warningg[decode(raw)["DIF_Id"]]=true;
		            std::cout<<"Please add DIF "<<decode(raw)["DIF_Id"]<<" to your geometry file; I'm Skipping its data."<<std::endl;
		          }
	            continue;
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
          for (int ihit=0; ihit < numElements; ++ihit) 
          {
	          RawCalorimeterHit *raw = dynamic_cast<RawCalorimeterHit*>( col->getElementAt(ihit)) ;
	          CellIDEncoder<CalorimeterHitImpl> cd1( "I:8,J:7,K:10,DIF_Id:8,Asic_Id:6,Channel:7",col_event1) ;
	          col_event1->setFlag(col_event1->getFlag()|( 1 << LCIO::RCHBIT_LONG));
	          col_event1->setFlag(col_event1->getFlag()|( 1 << LCIO::RCHBIT_TIME));
	          col_event1->setFlag(col_event1->getFlag()|( 1 << LCIO::CHBIT_ID1));
	          col_event1->setFlag(col_event1->getFlag()|( 1 << LCIO::RCHBIT_BARREL));
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
    std::cout<<red<<"REMINDER::Data from Dif "<<it->first<<" are skipped !"<<normal<<std::endl;
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
