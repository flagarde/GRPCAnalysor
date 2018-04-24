#ifndef Types
#define Types
#include <vector>
#include <array>
enum Types {pad,strip,stripup,stripdown,temporal,tcherenkov,tricot,scintillator,bif,m3,CMS};
const std::vector<std::string>Types_Dif{"Pad","Strip","StripUp","StripDown","Temporal","Tcherenkov","Tricot","Scintillator","BIF","M3","CMS"};
const std::vector<std::string>Types_Electrode{"FloatGlass","LRGlass","Bakelite","Plastic"};
enum Tresholds {Threshold1=1,Threshold2,Threshold3,Threshold12,Threshold23,Thresholdall};
const std::array<std::string,6>Thresholds_name{{"Threshold1","Threshold2","Threshold3","Threshold12","Threshold23","Thresholdall"}};
enum Threshold{Threshold_2=1,Threshold_1,Threshold_3};
#endif
