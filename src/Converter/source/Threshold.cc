#include "Threshold.h"
double ThresholdConv_0(int& i)
{
  return (i-90)*1.0/0.7;
}  

double ThresholdConv_1(int& i)
{
  return (i-98)*1.0/0.08;
}  

double ThresholdConv_2(int& i)
{
  return (i-98)*1.0e3/16.3;
}  

double ThresholdsConv(int i,int j)
{
  if(j==0) return ThresholdConv_0(i);
  else if(j==1) return ThresholdConv_1(i);
  else if (j==2) return ThresholdConv_2(i);
  else return -1;
}  
