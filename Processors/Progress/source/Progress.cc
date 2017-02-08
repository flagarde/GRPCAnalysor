#include "Progress.h"
#include "Global.h"
#include <iostream> 
#include <string>
#include <sstream>
#include "Utilities.h"
Progress aProgress;

Progress::Progress() : Processor("Progress") 
{
  _description = "ShowProgression";
}

void Progress::init() 
{
  printParameters();
}

void Progress::processRunHeader(LCRunHeader *run) {}

void Progress::processEvent(LCEvent *evt) 
{
  Progression(Global::parameters->getIntVal("SkipNEvents"),Global::totalevent,Global::parameters->getIntVal("MaxRecordNumber"),evt->getEventNumber(),"");
}

void Progress::end() 
{
}

/*inline std::string Progress::Shift(double val)
{
  std::stringstream ss;
	if(val<10) ss<<"  "<<val;
	if(val>=10&&val<1000) ss<<" "<<val;
	if(val>=1000) ss<<val;
  return ss.str();
}*/

inline unsigned int Progress::Every(unsigned int & _maxRecord)
{
  if(_maxRecord<=0) return 1000;
  else if(_maxRecord<=10) return 1;
  else if(_maxRecord<=100) return 10;
  else if(_maxRecord<=1000) return 100;
  else  return 1000;
}


inline void Progress::Progression(unsigned int _skip,unsigned int _GlobalEvents, unsigned int _maxRecord, unsigned int _eventNr,std::string Perso="")
{
  unsigned int _rolling=Every(_maxRecord);
  unsigned int skip=0;
  if(_skip!=0)skip=_skip+1;
  unsigned int maxRecordplusskip=0;
  if(_maxRecord+skip>=_GlobalEvents) 
  {
     maxRecordplusskip=_GlobalEvents;  
  }
  else maxRecordplusskip=_maxRecord+skip;
  if(_maxRecord>=_GlobalEvents)_maxRecord=_GlobalEvents ;
  if(_eventNr %_rolling ==0 || _eventNr==_GlobalEvents || _eventNr==maxRecordplusskip )
  {
    if(_maxRecord==0)  
	{
    int percent=int((_eventNr-skip)*100.0/(_GlobalEvents-skip));
    if(percent<100) 
		{
		  if(Perso!="")std::cout<<Perso<<std::endl;
		  std::cout<<red<<"["<<Shift(percent)<<"%]"<<normal<<" Event Number : "<<Shift(_eventNr)<<"/"<<_GlobalEvents<<std::endl;
		}
	}
        else 
	{
	  int percent=int((_eventNr-skip)*100.0/(_maxRecord));
	  if(percent<100) 
		{
		if(Perso!="")std::cout<<Perso<<std::endl;
		std::cout<<red<<"["<<Shift(percent)<<"%]"<<normal<<" Event Number : "<<Shift(_eventNr)<<"/"<<maxRecordplusskip<<" Total : "<<_GlobalEvents<<std::endl;
		}
	}
  }
}

