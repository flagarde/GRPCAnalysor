#ifndef Dif_h
#define Dif_h
class Dif
{
 public:
  Dif(){};
  Dif(const double&,const double&, int, const double&, const double&,const double&, const int&, const int&, const int& );
  double GetPositionX();
  double GetPositionY();
  int GetDifId();
  double GetAngleXY();
  double GetAngleXZ();
  double GetAngleYZ();
  int GetNbrPlate();
  int GetDifType();
  int GetDifUpDown();
 private:
  double _PositionX;
  double _PositionY;
  int _DifId;
  double _AngleXY;
  double _AngleXZ;
  double _AngleYZ;
  int _nbrPlate;
  int _up_down;
  int _DifType;
};
#endif
