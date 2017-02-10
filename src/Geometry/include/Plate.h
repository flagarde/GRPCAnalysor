#ifndef Plate_h
#define Plate_h
#include <vector>
#include <string>
class Plate
{
  public:
    Plate(){};
    Plate(const Plate& _Plate);
    Plate(const double&,const double&,const double&, const double&,const double&,const double&,std::vector<int>,const double&, const double& ,const double&, const double&,const std::string&,const std::string&);
    double GetPositionX();
    double GetPositionY();
    double GetPositionZ();
    double GetAngleXY();
    double GetAngleXZ();
    double GetAngleYZ();
    int GetNbrDifInPlate();
    std::vector<int> GetDifInPlate();
    double GetSizeX();
    double GetSizeY();
    double GetGlassType();
    double GetGazNumber();
    std::string GetHVChannel();
    std::string GetGazChannel();
 private:
    double _PositionX;
    double _PositionY;
    double _PositionZ;
    double _AngleXY;
    double _AngleXZ;
    double _AngleYZ;
    std::vector<int> _DifInPlate;
    double _SizeX;
    double _SizeY;
    double m_glass_type;
    double m_Gaz_number;
    std::string m_HV_channel;
    std::string m_Gaz_channel;
};
#endif
