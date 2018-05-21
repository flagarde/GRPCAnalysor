#pragma once
#include "DataFormat.h"
#include "SDHCAL_BufferNavigator.h"
#include <limits>

class SDHCAL_DataFormat: public DataFormat
{
public:
    virtual ~SDHCAL_DataFormat(){}
    virtual int Parse()
    {
      //RawBuffer->getBuffer().printBuffer();
      SDHCAL_RawBuffer_Navigator bufferNavigator(RawBuffer->getBuffer(),_BitsToSkip);
      uint32_t idstart = bufferNavigator.getStartOfDIF();
      //_DIFStarter[idstart]++;
      if (!bufferNavigator.validBuffer()) return -1;
      DIFPtr *d = bufferNavigator.getDIFPtr();
      //if (d != nullptr) _DIFPtrValueAtReturnedPos[bufferNavigator.getDIFBufferStart()[d->getGetFramePtrReturn()]]++;
      //_SizeAfterDIFPtr[bufferNavigator.getSizeAfterDIFPtr()]++;
      // create RawCalorimeterHit
      int loop=0;
      unsigned int FrameBCIDold=std::numeric_limits<unsigned int>::max();
      for (uint32_t i = 0; i < d->getNumberOfFrames(); i++) 
      {
        unsigned int nbr_hit_in_asic = 0;
        for (uint32_t j = 0; j < 64; j++) if (d->getFrameLevel(i, j, 0) || d->getFrameLevel(i, j, 1)) nbr_hit_in_asic++;
        if (!(nbr_hit_in_asic == 64 && _virer_full_asic == true)) 
        {
          for (uint32_t j = 0; j < 64; j++) 
          {
            if (!(d->getFrameLevel(i, j, 0) || d->getFrameLevel(i, j, 1)))  continue;
            IMPL::RawCalorimeterHitImpl *hit =new IMPL::RawCalorimeterHitImpl();
            if(hit==nullptr) continue;
            (*_cd)["DIF_Id"]=(int)d->getID();
            std::cout<<red<<(int)d->getID()<<normal<<std::endl;
            (*_cd)["Asic_Id"]=(int)d->getFrameAsicHeader(i);
            bitset<6> Channel(j);
            (*_cd)["Channel"]=(int)(Channel.to_ulong());
            (*_cd)["BarrelEndcapModule"]=0;
            (*_cd).setCellID( hit ) ;
            bitset<2> ThStatus;
            ThStatus[0] = d->getFrameLevel(i, j, 0);
            ThStatus[1] = d->getFrameLevel(i, j, 1);
            hit->setCellID1(d->getFrameBCID(i));
            hit->setAmplitude(ThStatus.to_ulong());
            unsigned int TTT = (unsigned int)(d->getFrameTimeToTrigger(i));
            if(FrameBCIDold-d->getFrameBCID(i)<0)
            {
              loop++;
              TTT=(unsigned int)(loop*16777215+d->getFrameTimeToTrigger(i));
              std::cout<<"oups"<<std::endl;
            }
            FrameBCIDold=d->getFrameBCID(i);
            hit->setTimeStamp(TTT);		      		
            _RawVec->addElement(hit);
          } // for (uint32_t j=0;j<64;j++)
        }
          // std::cout<<yellow<<yyy<<normal<<std::endl;
      } // for (uint32_t i=0;i<d->getNumberOfFrames();i++)
      // register Triggers'time : lots of values here ?
      lcio::IntVec trig(9);
      trig[0] = d->getDTC();
      trig[1] = d->getGTC();
      trig[2] = d->getBCID();
      trig[3] = d->getAbsoluteBCID() & 0xFFFFFF;
      trig[4] = (d->getAbsoluteBCID() / (0xFFFFFF + 1)) & 0xFFFFFF;
      // std::cout<<red<<d->getAbsoluteBCID()<<normal<<std::endl;
      trig[5] = d->getTASU1(); // what happen if no temperature ?
      trig[6] = d->getTASU2();
      trig[7] = d->getTDIF();
      std::string ss = "DIF" + std::to_string(d->getID()) + "_Triggers";
      _RawVec->parameters().setValues(ss, trig);
      //if (bufferNavigator.hasSlowControlData()) _hasSlowControl++;
      //if (bufferNavigator.badSCData()) _hasBadSlowControl++;
      Buffer eod = bufferNavigator.getEndOfAllData();
      //_SizeAfterAllData[eod.getsize()]++;
      //int nonzeroCount = 0;
      //for (uint8_t *it = eod.buffer(); it != eod.endOfBuffer(); it++)if (int(*it) != 0)nonzeroCount++;
      //_NonZeroValusAtEndOfData[nonzeroCount]++;
      return 0;
    }
};
