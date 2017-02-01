#ifndef ASICINFO
#define ASICINFO
#include <string>
#include <map>
#include <array>
#include <vector>
class AsicInfo
{
   public:
   AsicInfo();
   AsicInfo(std::string& asic_type,std::array<unsigned int,64> pagain,unsigned int& b0,unsigned int& b1,unsigned int& b2,unsigned int& dif_id,unsigned int& id, unsigned int& header,unsigned int& num);
   std::array<unsigned int,64>& ReturnMe();
   ~AsicInfo(){};
   std::string getAsicType();
   std::array<unsigned int,64>& getGain();
   unsigned int getGain(unsigned int& index_pad);
   std::vector<unsigned int> getThresholds();
   unsigned int getThreshold(int& threshold);
   unsigned int getThreshold0();
   unsigned int getThreshold1();
   unsigned int getThreshold2();
   unsigned int getDif_ID();
   unsigned int getID();
   unsigned int getHeader();
   unsigned int getNUM();
   private:
   std::string ASIC_TYPE;
   std::array<unsigned int,64>PAGAIN;
   unsigned int B0,B1,B2;
   unsigned int DIF_ID, ID, HEADER,NUM;
}; 
#endif
