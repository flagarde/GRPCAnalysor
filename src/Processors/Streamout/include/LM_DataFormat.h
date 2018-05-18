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
    
    virtual void Parse()
    {
        std::ofstream fout("data.dat", ios::out | ios::binary);
        fout.write((char*)&(RawBuffer->getIntVector()[0]), RawBuffer->nBytes());
        fout.close();
        int32_t _fdIn=::open("data.dat", O_RDONLY | O_NONBLOCK,S_IRWXU);
        uint32_t maxsize=0x100000;
        levbdim::buffer b(maxsize);
        TdcChannelBuffer tdcBuf(maxsize);
        tdcBuf.setIsNoise(false);
        uint32_t bsize=0;
	    int ier=::read(1,&bsize,sizeof(uint32_t));
	    if (ier<=0)
	    {
	      printf("Cannot read anymore  DIF Size %d \n ",ier);
	      //return 0;
	    }
        std::cout<<bsize<<std::endl;
	    ier=::read(1,b.ptr(),bsize);
	    if (ier<=0)
	    {
	      printf("Cannot read anymore Read data %d \n ",ier);
	      //return 0;
	    }
	    //b.setPayloadSize(bsize-(3*sizeof(uint32_t)+sizeof(uint64_t)));
	    //b.uncompress();
        //std::cout<<b.detectorId()<<std::endl;
        /*
	    if (b.detectorId() != detectorId) continue;
	    uint32_t* ibuf=(uint32_t*) b.payload();
	    absbcid=ibuf[3]; absbcid=(absbcid<<32)|ibuf[2];
         	    //printf("\n event number %d, GTC %d, ABSBCID %lu, mezzanine number %u, ",ibuf[0],ibuf[1]&0xFFFF,absbcid,ibuf[4]);
 	   // printf("IP address %u.%u.%u.%u,",ibuf[5]&0xFF,(ibuf[5]>>8)&0xFF,(ibuf[5]>>16)&0xFF,(ibuf[5]>>24)&0xFF);
 	   // uint32_t nch=ibuf[6];
 	   // printf("\n channels -> %d \n",nch);
	    if (ibuf[6]>0)
	    {
	      ++nDIFwithChannel;
	      absbcidFoundInThisReadout.insert(absbcid);
	      uint8_t* cbuf=( uint8_t*)&ibuf[7];
	      for (int i=0;i<ibuf[6];i++)
		    {
		      tdcBuf.addChannel(&cbuf[8*i]);
		      TdcChannel &c=tdcBuf.last();
              //std::cout<<int(c.channel())<<std::endl;
          if (tdcBuf.nTdcChannel()>maxsize) 
          {
            std::cout << "WARNING TOO BIG EVENT skipping" << std::endl; 
            OKprocess=false; 
            break;
          }
          if(c.channel()==triggerChannel)
          {
              
            _lastTriggerAbsBCID=absbcid;
          }
		    c.setstrip(ibuf[4],(ibuf[5]>>24)&0xFF);
              
		    }
	    }*/
    }
};
