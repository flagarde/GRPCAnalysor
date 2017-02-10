#include "ElogInformations.h"
#include <iostream> 
#include <string>
#include <sstream>
#include "Colors.h"
template <typename T>
std::string NumberToString(T pNumber)
{
 std::ostringstream oOStrStream;
 oOStrStream << pNumber;
 return oOStrStream.str();
}

void ElogInformations::AddBruteValue(std::string key,std::string value)
{
  BruteValues[key]=value;
}

void ElogInformations::ConstructMaps()
{
  std::map<std::string,int> Number={{"Zero",0},{"One",1},{"Two",2},{"Three",3},{"Four",4},{"Five",5},{"Six",6},{"Seven",7},{"Eight",8},{"Nine",9}};
  for(unsigned int i=1;i!=Number[BruteValues["Number_Dif"]];++i)
  {
    std::string key="Dif_Id"+NumberToString(i);
    std::string valueHV="HV"+NumberToString(i);
    std::string valueGasChannel="Gas Channel"+NumberToString(i);
    std::string valueHVChannel="HV Channel"+NumberToString(i);
    std::string valueType="Type"+NumberToString(i);
    std::cout<<key<<"  "<<valueHV<<"  "<<valueGasChannel<<" "<<valueHVChannel<<"  "<<valueType<<std::endl;
    HV[stoi(BruteValues[key])]=stod(BruteValues[valueHV]);
    GasChannel[stoi(BruteValues[key])]=BruteValues[valueGasChannel];
    HVChannel[stoi(BruteValues[key])]=BruteValues[valueHVChannel];
    Type[stoi(BruteValues[key])]=BruteValues[valueType];
  }
  for(std::map<int,double>::iterator it=HV.begin();it!=HV.end();++it)
  {
    std::cout<<it->first<<"  "<<it->second<<std::endl;
  }
}
