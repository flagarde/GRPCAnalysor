#ifndef Types
#define Types
enum Types {pad,positional,temporal,tcherenkov,tricot,scintillator,BIF};
const std::vector<std::string>Types_Dif{"Pad","Strip","Temporal","Tcherenkov","Tricot","Scintillator","Bif"};
const std::vector<std::string>Types_Electrode{"FloatGlass","LRGlass","Bakelite","Plastic"};
enum Tresholds {Threshold1,Threshold2,Threshold3,Threshold12,Threshold23,Thresholdall};
const std::array<std::string,6>Thresholds_name{{"Threshold1","Threshold2","Threshold3","Threshold12","Threshold23","Thresholdall"}};
#endif
