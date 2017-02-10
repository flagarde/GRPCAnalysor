#include "Global.h"
#include "StringParameters.h"
#include "OutFileRoot.h"
#include "Geometry.h"
#include <vector>
#include <string>
#include "ConfigInfo.h"
#include "ElogInformations.h"
#include "HistoPlane.h"
StringParameters* Global::parameters = nullptr ;
OutFileRoot*   Global::out=nullptr;
Geometry* Global::geom=nullptr;
int Global::number=0;
int Global::totalevent=0;
ConfigInfos* Global::conf=nullptr;
ElogInformations* Global::eloginf=nullptr;
HistoPlane Global::HG;
