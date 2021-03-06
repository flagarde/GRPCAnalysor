#pragma once
#include <vector>
#include <map>
#include <string>
#include "Plate.h"
#include "Dif.h"
#include "Types.h"

class Geometry
{
  public:
    Geometry(){}
    ~Geometry(){};
    void AddDif(const double&,const double&, int,const double&,const double&,const double&,const int&,const int& );
    void AddPlate(const double&,const double&,const double&,const double&,const double&,const double&, std::vector<int>&,const double&,const double&,const double&, const double&,const std::string&,const std::string&);
    const double GetDifPositionX( int& i);
    const double GetDifPositionY( int& i);
    const double GetDifAlpha( const int& i);
    const double GetDifBeta( const int& i);
    const double GetDifGamma( const int& i);
    const double GetDifPositionXMax(const int& i );
    const double GetDifPositionYMax(const int& i );
    const double GetDifPlateAlpha( const int& i);
    const double GetDifPlateBeta( const int& i);
    const double GetDifPlateGamma( const int& i);
    const int GetDifId( int& i);
    const int GetNbrDifInPlate(int i);
    const double GetPlatePositionX(const int& i);
    const double GetSizeX(const int& i);
    const double GetSizeY(const int& i);
    const double GetPlatePositionY(const int& i);
    const double GetPlatePositionZ(const int& i);
    const int GetDifNbrPlate( const int& i);
    const int GetDifType( const int& i);
    const int GetAsicNbr(const int& i);
    const double GetWidthCell(const int& i);
    const double GetLengthCell(const int& i);
    const double GetAreaCell(const int& i);
    const double GetInterCellWidth(const int& i);
    const double GetInterCellLength(const int& i);
    const double GetAreaOneCellDif(const int& i);
    const double GetTotalAreaDif(const int& i);   
    const double GetTotalAreaPlane(const int& i);   
    const std::string GetDifTypeName( const int& i);
    const double GetAreaOneCellPlane(const int& i);
    const unsigned int GetNumberDifs();
    const unsigned int GetNumberPlates();
    const std::vector<Plate> GetPlates();
    const Plate GetPlate(const int& i);
    const std::map<int,Dif> GetDifs();
    const std::vector<int> GetDifsInPlane(int i);
    const std::vector<int> GetDifsList();
    const std::string GetGlassType(const int& i);
    double GetGazNumber(const int& i);
    std::string GetHVChannel(const int& i);
    std::string GetGazChannel(const int& i);
    int GetXMax();
    int GetYMax();
    void PrintGeom(); 
 private:
    std::vector<Plate> Plates;
    std::map<int, Dif > Difs;
};
