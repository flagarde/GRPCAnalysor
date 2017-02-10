#include "SpillStudy.h"
#include "Global.h"
#include "Processor.h"
#include "EVENT/RawCalorimeterHit.h"
#include "IMPL/LCCollectionVec.h"
TH1D* timestamp=nullptr;
TH1D* timestamps=nullptr;
TH1D* time2read=nullptr;
TH1D* time2readtime=nullptr;
TH1D* TimeSpill=new TH1D("Duration Spill","Duration Spill",50000,0,500);
TH1D* TimeRamFull=new TH1D("Duration Frame","Duration Frame",1000,0,1);
TH1D* UsefullTime=new TH1D("UselessTime","UselessTime",1000,0,1);
std::vector<TH1*>typeee;
unsigned long int total_time_file=0;
unsigned long int total_time_by_user=0;


unsigned long int min_by_user=0;
unsigned long int max_by_user=0;
//unsigned long long int HistoPlane::global_total_time =0;

TH1F diff("diff","diff",1000,-500,500);
std::map<std::string,std::vector<double>>Types;
//std::vector<std::string  > th1{"timestamp"};
//std::vector<std::string> th2 {};
//std::vector<std::string>th2_Asic{};

//END GLOBAL VARIABLES


SpillStudy aSpillStudy;

SpillStudy::SpillStudy() : Processor("SpillStudy")
{
  _hcalCollections={"DHCALRawHits"};
  registerInputCollections( LCIO::RAWCALORIMETERHIT,"HCALCollections","HCAL Collection Names",_hcalCollections,_hcalCollections);
} 

SpillStudy::~SpillStudy() {}

void SpillStudy::processRunHeader( LCRunHeader* run){}

void SpillStudy::init()
{ 
  printParameters();
}

void SpillStudy::processEvent( LCEvent * evt )
{
   /* if(Global::geom) 
  {
    unsigned int NbrPlate =0;
    std::map<int, Dif >Difs=Global::geom->GetDifs();;
    for(std::map<int, Dif >::iterator it=Difs.begin(); it!=Difs.end(); ++it) 
	  {
	    if(Global::geom->GetDifType(it->first)!=temporal&&Global::geom->GetDifType(it->first)!=tcherenkov&&Global::geom->GetDifType(it->first)!=scintillator) 
	    {
	      NbrPlate=Global::geom->GetDifNbrPlate(it->first)-1;
	      if(HistoPlanes.find(NbrPlate)==HistoPlanes.end()) 
		    {
		      HistoPlanes.insert(std::make_pair(NbrPlate, new HistoPlane(_WantDistribution,NbrPlate,Global::geom->GetDifsInPlane(NbrPlate),Global::geom->GetSizeX(NbrPlate),Global::geom->GetSizeY(NbrPlate),th1,th2,th2_Asic)));
		    }
	    }
	  }
  }*/  
    std::map<unsigned long long int,int>Time_stamps;
    std::map<unsigned long long int,int>Time_stampss;
    unsigned long long int min_by_user=999999999999;
    unsigned long long int  max_by_user=1;
    int counter=0;
    for(unsigned int i =0;i!=Global::LCIOFiles.size();++i)
    {
        std::cout<<"I'm Readind The DATA in "<<Global::LCIOFiles[i]<<std::endl;
        LCReader* lcReader = LCFactory::getInstance()->createLCReader() ;
        lcReader->setReadCollectionNames(_hcalCollections) ;
        lcReader->open( Global::LCIOFiles[i] ) ;
        LCEvent *evt(nullptr);
        do 
        {
        LCCollection* col=evt->getCollection("DHCALRawHits");
        if(col!=nullptr)
		    {
		      counter++;
          RawCalorimeterHit * myhit = dynamic_cast<RawCalorimeterHit*>(col->getElementAt(0)) ;
         
          unsigned int dif_id=myhit->getCellID0()&0xFF;
          if (dif_id==0) return;
          std::string name="DIF"+std::to_string(dif_id)+"_Triggers";
          
          lcio::IntVec vTrigger;
          col->getParameters().getIntVals(name,vTrigger);
          unsigned long long _bcid=0;
          if (vTrigger.size()>=5)
          {
            unsigned long long Shift=16777216ULL;
  	        _bcid=vTrigger[4]*Shift+vTrigger[3];
          }
          Time_stamps[_bcid]++;
          for(int i=0;i<col->getNumberOfElements();++i)
          {
            myhit=dynamic_cast<RawCalorimeterHit*>(col->getElementAt(i));
           // unsigned int dif_id=myhit->getCellID0()&0xFF;
        
       
            /*if(Global::geom->GetDifType(int(dif_id))<=1)
            {
              if(Global::geom->GetDifNbrPlate(int(dif_id))!=-1) 
              {
                HistoPlanes[Global::geom->GetDifNbrPlate(dif_id)-1]->Return_TH1F("timestamp")->Fill(_bcid*200e-9,1);
              }
            }*/
        }
        Time_stampss[_bcid]=col->getNumberOfElements();
					if(_bcid<min_by_user)
          {
				    min_by_user=_bcid;
			 			
					}
        	if(_bcid>max_by_user)
					{
		       
						max_by_user=_bcid;
					}
		  }
		  
		  if(counter<=Global::parameters->getIntVal("MaxRecordNumber")) evt=lcReader->readNextEvent();
      else evt =nullptr;
	  }
	  while(evt!=nullptr);
    lcReader->close();
    delete lcReader ;
    delete evt;
  }
   //////////////////////////////////////////////
  unsigned int total_Ram_Full=0;
  unsigned int total_hits=0;
  for(std::map<unsigned long long int,int>::iterator it=Time_stampss.begin();it!=Time_stampss.end();++it) total_hits+=it->second;
  for(std::map<unsigned long long int,int>::iterator it=Time_stamps.begin();it!=Time_stamps.end();++it) total_Ram_Full+=it->second;
  double moyenne=total_hits/(total_Ram_Full*1.0);
  std::cout<<total_Ram_Full<<"  "<<total_hits<<"  "<<moyenne<<std::endl;
  //////////////////////////////////////////////
  std::cout<<Time_stampss.size()<<std::endl;
  unsigned long long int min=9999999999;
  unsigned long long int  max=1;
  double min_between=99999;
  double max_between=0;
  double min_betweentime=99999;
  double max_betweentime=0;
  std::vector<double>Vec_timebetween;
  std::vector<double>timebetweentime_toplot;
  std::map<double,std::vector<double>>Vec_timebetweentime;
  Vec_timebetween.reserve(Time_stamps.size()-1);
  std::map<unsigned long long int,int>::iterator ultimate=--Time_stamps.end();
  std::map<unsigned long long int,int>::iterator ultimateless=ultimate; --ultimateless;
  bool skip=false;
  for(std::map<unsigned long long int,int>::iterator it=Time_stamps.begin();it!=Time_stamps.end();++it)
  {
    if(skip) 
    {
      skip=false; 
      continue;
    }
    if(it!=ultimate)
    {
      std::map<unsigned long long int,int>::iterator itt=it;
		  ++itt;
      double timebetween=(itt->first-it->first)*200e-9;
      if(timebetween<min_between)min_between=timebetween;
      if(timebetween>max_between)max_between=timebetween;
      Vec_timebetween.push_back(timebetween);
      if(it==Time_stamps.begin())
      {
        Vec_timebetweentime[(itt->first-it->first)*200e-9].push_back(it->first);
			  Vec_timebetweentime[(itt->first-it->first)*200e-9].push_back(itt->first);
	    }
      if(it!=ultimateless)
		  {
			  std::map<unsigned long long int,int>::iterator ittt=itt;
			  ++ittt;
        double a= (ittt->first-itt->first);
			  double b=(itt->first-it->first);
			  double timebetweentime=(a-b)*200e-9;
        
        timebetweentime_toplot.push_back((b-a)*200e-9);
			
        if(timebetweentime<min_betweentime)min_betweentime=timebetweentime;
        if(timebetweentime>max_betweentime)max_betweentime=timebetweentime;
        std::map<double,std::vector<double>>::iterator toadd=Vec_timebetweentime.end();
			  --toadd;
        for(std::map<double,std::vector<double>>::iterator j=Vec_timebetweentime.begin();j!=Vec_timebetweentime.end();++j)
			  {
          
          if(fabs(fabs(timebetweentime)-j->first) <=0.50)
				  {
					  (j->second).push_back(ittt->first);
            
            break;
				  }
          else if(j==toadd&&it!=Time_stamps.begin()) 
          {
					  Vec_timebetweentime[(ittt->first-itt->first)*200e-9].push_back(ittt->first);
					 
            skip=true;
   					break;
				  }
			  }
		  }
	  }
	  if(it->first<min&&it->first!=0)min=it->first;
    if(it->first>max)max=it->first;
    
  }
   for(std::map<double,std::vector<double>>::iterator j=Vec_timebetweentime.begin();j!=Vec_timebetweentime.end();++j)
  {
	  if(j->first<=0.5) 
    {
		  //Between_spill[j->first]=j->second;
		  Types["RamFull"].insert(Types["RamFull"].end(),j->second.begin(),j->second.end());
	  }
    else if (j->first>=1) 
	  {
		  //Spills[j->first]=j->second;
		  Types["Spill"].insert(Types["Spill"].end(),j->second.begin(),j->second.end());
	  }
    else 
	  {
		  //Others[j->first]=j->second;
		  Types["Other"].insert(Types["Other"].end(),j->second.begin(),j->second.end());
	  }
  }
  time2readtime = new TH1D("Difference in time bettwen two time","Difference in time bettwen two time",10000000,-10,10);
  for(unsigned int i =0;i<timebetweentime_toplot.size();++i)
  {
	  time2readtime->Fill(timebetweentime_toplot[i]);
    
  }
  time2read = new TH1D("Distribution time between 2 read out","Distribution time between 2 read out",5000000,min_between,100);
  for(unsigned int i =0;i<Vec_timebetween.size();++i)
  {
	  time2read->Fill(Vec_timebetween[i]);
   
  }
  double diff=(max-min);
  if(diff>9999999)diff=9999999;
  timestamp= new TH1D("timestamp","timestamp",diff,min*200e-9,max*200e-9);
  timestamps= new TH1D("timestampS","timestampS",diff,min*200e-9,max*200e-9);
  for(std::map<double,std::vector<double>>::iterator j=Vec_timebetweentime.begin();j!=Vec_timebetweentime.end();++j)
  {
    std::string name = "type"+std::to_string(j->first);
	  typeee.push_back(new TH1D(name.c_str(),name.c_str(),1000000,0.,10000.));
    for(unsigned int i =0;i<j->second.size();++i)
	  {
		  typeee[typeee.size()-1]->Fill(j->second[i]*200e-9);
		  
	  }
  }

  for(std::map<unsigned long long int ,int>::iterator it=Time_stamps.begin();it!=Time_stamps.end();++it)
  {
	  timestamp->Fill(it->first*200e-9,it->second);
	  
    
  }
  for(std::map<unsigned long long int ,int>::iterator it=Time_stampss.begin();it!=Time_stampss.end();++it)
  {
	  timestamps->Fill(it->first*200e-9,it->second);
    
  }
  total_time_by_user=(max_by_user-min_by_user)*200e-9;
  total_time_file=(max-min)*200e-9;
}

void SpillStudy::end()
{  
  timestamp->Write();
  delete timestamp;
  timestamps->Write();
  delete timestamps;
  time2read->Write();
  delete time2read;
  time2readtime->Write();
  delete time2readtime;
  TimeSpill->Write();
  delete TimeSpill;
  TimeRamFull->Write();
  delete TimeRamFull;
  UsefullTime->Write();
  delete UsefullTime;
 /* for(unsigned int i=0; i<HistoPlanes.size(); ++i) 
  {
      HistoPlanes[i]->Save(hfile,namepdf);
  }
  for(std::map<int,HistoPlane*>::iterator it=HistoPlanes.begin();it!=HistoPlanes.end();++it)
  {
      delete(it->second);
  }*/
}
