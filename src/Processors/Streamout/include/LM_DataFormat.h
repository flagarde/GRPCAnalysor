#pragma once
#include "DataFormat.h"
#include "TdcChannelBuffer.h"
#include "TdcChannels.h"
#include "buffer.h"
#include <fstream>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
class LM_DataFormat: public DataFormat
{
public:
    ~LM_DataFormat(){}
    
    virtual int Parse()
    {
        int loop=0;
        unsigned int FrameBCIDold=std::numeric_limits<unsigned int>::max();
        uint32_t bsize=(RawBuffer->getIntVector()[0]);
	    if (bsize <=0)
	    {
	      std::cout<<yellow<<"Cannot read anymore  DIF "<<bsize<<normal<<std::endl;
	      return -1;
	    }
	    uint32_t maxsize=0x100000;
        TdcChannelBuffer tdcBuf(maxsize);
        tdcBuf.setIsNoise(false);
        levbdim::buffer b(maxsize);
        b.setBuffer((char*)(RawBuffer->getCharBuffer()),RawBuffer->nBytes(),4);
        tdcBuf.clear();
        tdcBuf.setIsNoise(false);
        std::cout<<bsize<<std::endl;
	    if (bsize<=0)
	    {
	      printf("Cannot read anymore Read data\n ");
	      return -1 ;
	    }
	    b.setPayloadSize(bsize-(3*sizeof(uint32_t)+sizeof(uint64_t)));
	    b.uncompress();
	    if (b.detectorId() != 120 && b.detectorId()!=110) return -1;
	    uint32_t* ibuf=(uint32_t*) b.payload();
	    uint32_t absbcid=ibuf[3]; absbcid=(absbcid<<16)|ibuf[2];
        printf("\n event number %d, GTC %d, ABSBCID %lu, mezzanine number %u, ",ibuf[0],ibuf[1]&0xFFFF,absbcid,ibuf[4]);
 	    printf("IP address %u.%u.%u.%u,",ibuf[5]&0xFF,(ibuf[5]>>8)&0xFF,(ibuf[5]>>16)&0xFF,(ibuf[5]>>24)&0xFF);
 	    uint32_t nch=ibuf[6];
 	    printf("\n channels -> %d \n",nch);
	    if (ibuf[6]>0)
	    {
          if(b.detectorId()==120)
          {
            uint8_t* cbuf=( uint8_t*)&ibuf[7];
            for (uint32_t i=0;i<ibuf[6];i++)
		    {
		      tdcBuf.addChannel(&cbuf[8*i]);
		      TdcChannel &c=tdcBuf.last();
              c.setIP((ibuf[5]>>24)&0xFF);
              c.setMezzanine(ibuf[4]);
              std::cout<<int(c.channel())<<std::endl;
              if (tdcBuf.nTdcChannel()>bsize) 
              {
                std::cout << "WARNING TOO BIG EVENT skipping" << std::endl; 
                break;
              }
              IMPL::RawCalorimeterHitImpl *hit =new IMPL::RawCalorimeterHitImpl();
              if(hit==nullptr) continue;
              (*_cd)["DIF_Id"]=(int)(c.mezzanine());
              (*_cd)["Asic_Id"]=(int)(c.IP());
              std::cout<<c.channel()<<std::endl;
              if(c.channel()>64) return -1;
              (*_cd)["Channel"]=(int)(c.channel());
              (*_cd)["BarrelEndcapModule"]=(int)(c.fine());
              (*_cd).setCellID( hit ) ;
              hit->setCellID1(absbcid);
              hit->setAmplitude(c.coarse());
              std::cout<<red<<(*_cd)["DIF_Id"]<<"  "<<(*_cd)["Asic_Id"]<<"  "<<(*_cd)["Channel"]<<normal<<std::endl;
              std::cout<<yellow<<uint32_t(hit->getAmplitude())<<"  "<<c.coarse()<<normal<<std::endl;
              unsigned int TTT = (unsigned int)((cbuf[8*i+2]|(((uint16_t)cbuf[8*i+1])<<8)));
              if(FrameBCIDold-absbcid<0)
              {
                loop++;
                TTT=(unsigned int)(loop*16777215+(cbuf[8*i+2]|(((uint16_t)cbuf[8*i+1])<<8)));
                std::cout<<"oups"<<std::endl;
              }
              FrameBCIDold=absbcid;
              hit->setTimeStamp(TTT);		   		
              _RawVec->addElement(hit);
		    }
          }
        }
        return 0;
    }
};
